/****************************************************************************
            Software License for Customer Use of CA Software
                          Grant Terms and Conditions

IMPORTANT NOTICE - READ CAREFULLY: This Software License for Customer Use
of CA Software ("LICENSE") is the agreement which governs use of
software of CA, Inc. and its subsidiaries ("CA"),
including computer software (source code and object code) and associated
printed materials ("SOFTWARE").  The SOFTWARE is protected by copyright laws
and international copyright treaties, as well as other intellectual property
laws and treaties.  The SOFTWARE is not sold, and instead is only licensed
for use, strictly in accordance with this document.  Any hardware sold by
CA is protected by various patents, and is sold but this LICENSE does
not cover that sale, since it may not necessarily be sold as a package with
the SOFTWARE.  This LICENSE sets forth the terms and conditions of the
SOFTWARE LICENSE only.  By downloading, installing, copying, or otherwise
using the SOFTWARE, you agree to be bound by the terms of this LICENSE.
If you do not agree to the terms of this LICENSE, then do not download the
SOFTWARE.

DEFINITIONS:  "DEVICE" means the CA(TM) G3  SDK product.
"You" or "CUSTOMER" means the entity or individual that uses the SOFTWARE.
"SOFTWARE" means the CA(TM) SDK software.

GRANT OF LICENSE:  Subject to the restrictions below, CA hereby grants
CUSTOMER a non-exclusive, non-assignable, non-transferable, royalty-free,
perpetual copyright license to (1) install and use the SOFTWARE for
reference only with the DEVICE; and (2) copy the SOFTWARE for your internal
use only for use with the DEVICE.

RESTRICTIONS:  The SOFTWARE must be used solely in conjunction with the
DEVICE and solely with Your own products that incorporate the DEVICE.  You
may not distribute the SOFTWARE to any third party.  You may not modify
the SOFTWARE or make derivatives of the SOFTWARE without assigning any and
all rights in such modifications and derivatives to CA.  You shall not
through incorporation, modification or distribution of the SOFTWARE cause
it to become subject to any open source licenses.  You may not
reverse-assemble, reverse-compile, or otherwise reverse-engineer any
SOFTWARE provided in binary or machine readable form.  You may not
distribute the SOFTWARE to your customers without written permission
from CA.

OWNERSHIP OF SOFTWARE AND COPYRIGHTS. All title and copyrights in and the
SOFTWARE and any accompanying printed materials, and copies of the SOFTWARE,
are owned by CA. The SOFTWARE protected by the copyright laws of the
United States and other countries, and international treaty provisions.
You may not remove any copyright notices from the SOFTWARE.  Except as
otherwise expressly provided, CA grants no express or implied right
under CA patents, copyrights, trademarks, or other intellectual
property rights.

DISCLAIMER OF WARRANTIES. THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY
EXPRESS OR IMPLIED WARRANTY OF ANY KIND, INCLUDING ANY IMPLIED WARRANTIES
OF MERCHANTABILITY, NONINFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE,
TITLE, AND NON-INFRINGEMENT.  CA does not warrant or assume
responsibility for the accuracy or completeness of any information, text,
graphics, links or other items contained within the SOFTWARE.  Without
limiting the foregoing, you are solely responsible for determining and
verifying that the SOFTWARE that you obtain and install is the appropriate
version for your purpose.

LIMITATION OF LIABILITY. IN NO EVENT SHALL CA OR ITS SUPPLIERS BE
LIABLE FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, LOST
PROFITS, BUSINESS INTERRUPTION, OR LOST INFORMATION) OR ANY LOSS ARISING OUT
OF THE USE OF OR INABILITY TO USE OF OR INABILITY TO USE THE SOFTWARE, EVEN
IF CA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
TERMINATION OF THIS LICENSE. This LICENSE will automatically terminate if
You fail to comply with any of the terms and conditions hereof. Upon
termination, You will immediately cease use of the SOFTWARE and destroy all
copies of the SOFTWARE or return all copies of the SOFTWARE in your control
to CA.  IF you commence or participate in any legal proceeding against
CA, then CA may, in its sole discretion, suspend or terminate all
license grants and any other rights provided under this LICENSE during the
pendency of such legal proceedings.
APPLICABLE LAWS. Claims arising under this LICENSE shall be governed by the
laws of the State of California, excluding its principles of conflict of
laws.  The United Nations Convention on Contracts for the International Sale
of Goods is specifically disclaimed.  You shall not export the SOFTWARE
without first obtaining any required export license or other approval from
the applicable governmental entity, if required.  This is the entire
agreement and understanding between You and CA relating to this subject
matter.
GOVERNMENT RESTRICTED RIGHTS. The SOFTWARE is provided with "RESTRICTED
RIGHTS." Use, duplication, or disclosure by the Government is subject to
restrictions as set forth in FAR52.227-14 and DFAR252.227-7013 et seq. or
its successor. Use of the SOFTWARE by the Government constitutes
acknowledgment of CA's proprietary rights therein. Contractor or
Manufacturer is CA.

Copyright (c) 2016 by CA Incorporated
****************************************************************************/
#include "omci_common.h"
#include "omci_mq.h"
#include "omci_hal.h"
#include "omci_me_base.h"
#include "omci_me.h"
#include "omci_cfg.h"
#include "omci_mng_am.h"
#include "omci_mng_upgrade.h"
#include "omci_pm.h"
#include "omci_mq.h"
#include "omci_timer.h"
#include "osal_sem.h"
#include "osal_cmn.h"
#include "port.h"
#include "omci_mibmng.h"
#include "gpon.h"


#define OMCI_MAIN_DEF_MSGQ_NAME     "/omci_mq"


void ca_omci_msg_invalid_handle(void *pData);
void ca_omci_ploamd_reboot_handle(void *pData);
extern void ca_omci_mng_upg_load_image_set(ca_omci_uint8_t flag);
extern void ca_omci_pkt_handle(void *pData);
extern void ca_omci_alarm_handle(void *pData);
extern void ca_omci_avc_handle(void *pData);
extern void ca_omci_omcc_st_handle(void *pData);
extern void ca_omci_timeout_handle(void *pData);
extern void ca_omci_test_rslt_handle(void *pData);
extern void ca_omci_trans_id_increase(void);
extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
extern int ca_omci_analysis_init(void);
extern void ca_omci_analysis_entry(void *p1, void *p2, void *p3);
extern void ca_omci_onuinfo_init(void);
extern ca_uint g_omciCfgSem;


static ca_omci_mq_t  sg_omciMainMsgQ = {0};


static omci_mq_callback_table_t sg_omciMqHandle[] =
{
    {OMCI_INNER_MSG_MIN,                ca_omci_msg_invalid_handle},
    {OMCI_INNER_MSG_TYPE_PKT,           ca_omci_pkt_handle},
    {OMCI_INNER_MSG_TYPE_DEBUG_PKT,     ca_omci_pkt_handle},
    {OMCI_INNER_MSG_TYPE_ALARM,         ca_omci_alarm_handle},
    {OMCI_INNER_MSG_TYPE_AVC,           ca_omci_avc_handle},
    {OMCI_INNER_MSG_TYPE_OMCC_STATE,    ca_omci_omcc_st_handle},
    {OMCI_INNER_MSG_TYPE_TIMEOUT,       ca_omci_timeout_handle},
    {OMCI_INNER_MSG_TYPE_TEST_RSLT,     ca_omci_test_rslt_handle},
    {OMCI_INNER_MSG_TYPE_PLOAMD_REBOOT, ca_omci_ploamd_reboot_handle},
};


void ca_omci_ploamd_reboot_handle(void *pData)
{
    ca_omci_inner_msg_t *pMsg = (ca_omci_inner_msg_t *)pData;
    ca_omci_uint8_t      flag = 0;

    /*Just print Flags becasuse no VOIP APIs for CA-OMCI*/
    OMCIMNGWARN("Reboot flag %d", pMsg->data[15]);
    flag = pMsg->data[15];

    /*Reboot depth handle*/
    if (0 == pMsg->data[12]){
        ca_omci_mng_mib_reset();
        return;
    }

    /*Reboot image handle  sg_halRebootCfg[1]  = pMsg->data[13];*/
    /*TODO: hook api to set Load and execute the image */
    ca_omci_mng_upg_load_image_set(pMsg->data[13]);

    if (0x1 == pMsg->data[12]) {
        ca_omci_hal_onu_reboot((void *)&flag);
        return;
    }

    if (0x2 == pMsg->data[12]) {
        OMCIMNGERR("No API for power cycle reboot");
        return;
    }

    if (0x3 == pMsg->data[12]) {
        ca_omci_mng_mib_reset();
        ca_omci_hal_onu_reboot((void *)&flag);
        return;
    }
}


void ca_omci_msg_invalid_handle(void *pData)
{
    ca_omci_inner_msg_hdr_t *hdr = (ca_omci_inner_msg_hdr_t *)pData;
    OMCIERR("invalid msg %d", hdr->msgType);
}


ca_omci_st_e ca_omci_main_pkt_send(ca_omci_uint8_t *pkt, ca_omci_uint32_t len)
{
    ca_omci_inner_msg_t msg;

    OMCI_PARAM_NULL_RETURN(pkt);
    if ((0 == len) || (len > OMCI_MQ_MSG_MAX_LEN))
    {
        OMCIERR("receive error packet len %d", len);
        return CA_OMCI_E_PARAM;
    }

    ca_omci_trans_id_increase();
    OMCI_MEMSET(&msg, 0, sizeof(msg));
    OMCI_MEMCPY(msg.data, pkt, len);
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_DEBUG_PKT;
    msg.hdr.dataLen = len;

    return omci_mq_send(&sg_omciMainMsgQ, (ca_omci_int8_t *)&msg, sizeof(ca_omci_inner_msg_t));
}


ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg)
{
    OMCI_PARAM_NULL_RETURN(msg);
    return omci_mq_send(&sg_omciMainMsgQ, (ca_omci_int8_t *)msg, sizeof(ca_omci_inner_msg_t));
}


ca_omci_st_e ca_omci_main_mq_init(void)
{
    ca_omci_st_e ret = CA_OMCI_OK;

    OMCI_STRCPY(sg_omciMainMsgQ.name, OMCI_MAIN_DEF_MSGQ_NAME);

    ret = omci_mq_create(&sg_omciMainMsgQ);
    OMCI_CHECK_NOK_RET(ret, "create msgQ failed!");
    return ret;
}


void ca_omci_main_entry(void *p1, void *p2, void *p3)
{
    ca_omci_inner_msg_hdr_t *hdr = NULL;
    ca_omci_int8_t           pdu[OMCI_MQ_MSG_MAX_LEN] = {0};
    ca_omci_uint32_t         pduLen = 0;

    (void)p1;
    (void)p2;
    (void)p3;

    ca_sem_wait(g_omciCfgSem, -1);//wait forever

    while (1)
    {
        if (CA_OMCI_OK != omci_mq_recv(&sg_omciMainMsgQ, pdu, &pduLen))
        {
            OMCIERR("recv msg from mq failed");
            continue;
        }

        hdr = (ca_omci_inner_msg_hdr_t *)pdu;
        if (hdr->msgType <= OMCI_INNER_MSG_MIN || hdr->msgType >= OMCI_INNER_MSG_MAX)
        {
            OMCIERR("invalid msg %d", hdr->msgType);
            continue;
        }
        sg_omciMqHandle[hdr->msgType].func((void *)pdu);
    }
}


void ca_omci_main_finish(void)
{
    ca_omci_me_finish();
}


void ca_omci_main_init(void)
{
    ca_omci_st_e ret = CA_OMCI_OK;

    ca_omci_onuinfo_init();
    omci_log_init();
    ca_omci_cfg_init();
    ca_omci_alarm_init();
    ca_omci_mng_sw_img_init();
    ret = ca_omci_main_mq_init();
    OMCI_CHECK_NOK(ret, "init omci main msgQ failed");

    ret = ca_omci_hal_init();
    OMCI_CHECK_NOK(ret, "init omci hal failed");

    ret = ca_omci_me_init();
    OMCI_CHECK_NOK(ret, "init omci me failed");
}



#define OMCI_STACKSIZE     20480
#define OMCI_AL_STACKSIZE  20480
struct k_thread omci_thread;
struct k_thread omci_al_thread;
K_THREAD_STACK_DEFINE(omci_stack, OMCI_STACKSIZE);
K_THREAD_STACK_DEFINE(omci_al_stack, OMCI_AL_STACKSIZE);


void ca_omci_init(void)
{
    k_tid_t mainTid;
    k_tid_t analysisTid;

    ca_omci_main_init();
    ca_omci_analysis_init();

    mainTid = k_thread_create(&omci_thread, omci_stack, OMCI_STACKSIZE,
                              ca_omci_main_entry, NULL, NULL, NULL, -1, K_USER, K_NO_WAIT);

    analysisTid = k_thread_create(&omci_al_thread, omci_al_stack, OMCI_AL_STACKSIZE,
                                  ca_omci_analysis_entry, NULL, NULL, NULL, -1, K_USER, K_NO_WAIT);

    k_thread_name_set(mainTid, "omci_stack");
    k_thread_name_set(analysisTid, "omci_al_stack");
    ca_printf("omci is ready\r\n");

    ca_gpon_port_laser_mode_set(0, CA_PORT_ID(CA_PORT_TYPE_GPON,7), CA_GPON_PORT_LASER_MODE_BURST);
    ca_port_enable_set(0, CA_PORT_ID(CA_PORT_TYPE_GPON,7), TRUE);
    ca_printf("enable laser and port at last\r\n");
}

