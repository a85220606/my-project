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
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_me_ip_host_pm.h"
#include "omci_hal.h"
#include "omci_mq.h"
#include "omci_timer.h"


static ca_omci_me_handler_t sg_omci_me_ip_host_pm_handlers = {0};


ca_omci_me_attr_t g_omci_me_ip_host_pm_attr[OMCI_ME_ATTR13] =
{
    {OMCI_ME_ATTR1,  "interval_end_time"},
    {OMCI_ME_ATTR2,  "thres_id"},
    {OMCI_ME_ATTR3,  "icmp_err"},
    {OMCI_ME_ATTR4,  "dns_err"},
    {OMCI_ME_ATTR5,  "dhcp_timeouts"},
    {OMCI_ME_ATTR6,  "ip_addr_conflict"},
    {OMCI_ME_ATTR7,  "out_of_mem"},
    {OMCI_ME_ATTR8,  "internal_err"},
    {OMCI_ME_ATTR9,  "dhcp_attempts_cnt"},
    {OMCI_ME_ATTR10, "dhcp_ack_rx_cnt"},
    {OMCI_ME_ATTR11, "dhcp_nacks_cnt"},
    {OMCI_ME_ATTR12, "dhcp_rsp_err"},
    {OMCI_ME_ATTR13, "dhcp_rsp_incom"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
void ca_omci_me_ip_host_pm_timeout(void *data);


ca_omci_uint32_t ca_omci_me_ip_host_pm_attr_len_get(void)
{
    ca_omci_ip_host_pm_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.itvEndTime);
    attrLen += sizeof(me->attr.thresholdData);
    attrLen += sizeof(me->attr.icmpErr);
    attrLen += sizeof(me->attr.dnsErr);
    attrLen += sizeof(me->attr.dhcpTimeout);
    attrLen += sizeof(me->attr.ipAddrConf);
    attrLen += sizeof(me->attr.outOfMem);
    attrLen += sizeof(me->attr.internalErr);
    attrLen += sizeof(me->attr.dhcpAttempCnt);
    attrLen += sizeof(me->attr.dhcpAckRxCnt);
    attrLen += sizeof(me->attr.dhcpNacksCnt);
    attrLen += sizeof(me->attr.dhcpRspErr);
    attrLen += sizeof(me->attr.dhcpRspIncom);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ip_host_pm_new(void)
{
    ca_omci_ip_host_pm_t *me = (ca_omci_ip_host_pm_t *)omci_malloc(sizeof(ca_omci_ip_host_pm_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_IP_HOST_PM_CLASS_ID, &sg_omci_me_ip_host_pm_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ip_host_pm);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_ip_host_pm_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(iphostPm->attr.thresholdData, "thresholdData", 2);

    //clear hal static number first
    //TODO:
    //ca_omci_hal_ip_host_pm_pm_clear(me->meBase.instanceId);

    //start timer to collect data
    if (CA_INVALID_TIMER != iphostPm->timerId)
    {
        OMCIMNGERR("Can't create a timer twice for me ip_host_pm");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    iphostPm->timerId = ca_circle_timer_add(OMCI_TIMER_PM_INTERVAL,
                                            ca_omci_me_ip_host_pm_timeout,
                                            (void *)iphostPm);

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ip_host_pm_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_IP_HOST_PM_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(16, iphostPm->attr.thresholdData, "thresholdData", 2);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ip_host_pm_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_IP_HOST_PM_GET_MASK_FULL);
    OMCI_GET_ATTR_UINT8(iphostPm->preAttr.itvEndTime, 1);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.thresholdData, 2);
    OMCI_GET_ATTR_UINT32(iphostPm->preAttr.icmpErr, 3);
    OMCI_GET_ATTR_UINT32(iphostPm->preAttr.dnsErr, 4);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.dhcpTimeout, 5);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.ipAddrConf, 6);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.outOfMem, 7);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.internalErr, 8);
    OMCI_GET_ATTR_UINT32(iphostPm->preAttr.dhcpAttempCnt, 9);
    OMCI_GET_ATTR_UINT32(iphostPm->preAttr.dhcpAckRxCnt, 10);
    OMCI_GET_ATTR_UINT32(iphostPm->preAttr.dhcpNacksCnt, 11);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.dhcpRspErr, 12);
    OMCI_GET_ATTR_UINT16(iphostPm->preAttr.dhcpRspIncom, 13);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ip_host_pm_get_current(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_IP_HOST_PM_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(iphostPm->attr.itvEndTime, 1);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.thresholdData, 2);
    OMCI_GET_ATTR_UINT32(iphostPm->attr.icmpErr, 3);
    OMCI_GET_ATTR_UINT32(iphostPm->attr.dnsErr, 4);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.dhcpTimeout, 5);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.ipAddrConf, 6);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.outOfMem, 7);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.internalErr, 8);
    OMCI_GET_ATTR_UINT32(iphostPm->attr.dhcpAttempCnt, 9);
    OMCI_GET_ATTR_UINT32(iphostPm->attr.dhcpAckRxCnt, 10);
    OMCI_GET_ATTR_UINT32(iphostPm->attr.dhcpNacksCnt, 11);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.dhcpRspErr, 12);
    OMCI_GET_ATTR_UINT16(iphostPm->attr.dhcpRspIncom, 13);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ip_host_pm_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    //delete the timer
    if (CA_INVALID_TIMER != iphostPm->timerId)
    {
        ca_timer_stop(iphostPm->timerId);
        ca_timer_del(iphostPm->timerId);
        iphostPm->timerId = CA_INVALID_TIMER;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ip_host_pm_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ip_host_pm_t *iphostPm = (ca_omci_ip_host_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(iphostPm);
    OMCI_UPLOAD_ATTR_SIZE(8, iphostPm->attr.itvEndTime, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.thresholdData, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, iphostPm->attr.icmpErr, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, iphostPm->attr.dnsErr, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.dhcpTimeout, 5);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.ipAddrConf, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.outOfMem, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.internalErr, 8);
    OMCI_UPLOAD_ATTR_SIZE(32, iphostPm->attr.dhcpAttempCnt, 9);
    OMCI_UPLOAD_ATTR_SIZE(32, iphostPm->attr.dhcpAckRxCnt, 10);
    OMCI_UPLOAD_ATTR_SIZE(32, iphostPm->attr.dhcpNacksCnt, 11);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.dhcpRspErr, 12);
    OMCI_UPLOAD_ATTR_SIZE(16, iphostPm->attr.dhcpRspIncom, 13);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_ip_host_pm_init(void)
{
    sg_omci_me_ip_host_pm_handlers.uploadProc = ca_omci_me_ip_host_pm_upload;

    sg_omci_me_ip_host_pm_handlers.createProc = ca_omci_me_ip_host_pm_create;
    sg_omci_me_ip_host_pm_handlers.deleteProc = ca_omci_me_ip_host_pm_delete;
    sg_omci_me_ip_host_pm_handlers.setProc    = ca_omci_me_ip_host_pm_set;
    sg_omci_me_ip_host_pm_handlers.getProc    = ca_omci_me_ip_host_pm_get;
    sg_omci_me_ip_host_pm_handlers.getCurProc = ca_omci_me_ip_host_pm_get_current;
}


void ca_omci_me_ip_host_pm_timeout_handle(void *pData)
{
    ca_omci_uint16_t     *inst = (ca_omci_uint16_t *)pData;
    ca_omci_ip_host_pm_t *me   = NULL;

    OMCI_ENTER;
    me = (ca_omci_ip_host_pm_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_PM_CLASS_ID, *inst);
    OMCI_PARAM_IS_NULL(me);

    //get static value from hal, and put these value in preAttr;
    //TODO:
    //ca_omci_hal_ip_host_pm_pm_get(me->meBase.instanceId, value);

}


void ca_omci_me_ip_host_pm_timeout(void *data)
{
    ca_omci_ip_host_pm_t *me = (ca_omci_ip_host_pm_t *)data;
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_PM_CHECK;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_timer_def_e);//pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, sizeof(ca_omci_timer_def_e));//cp timerDef
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e)), &me->meBase.classId, sizeof(ca_omci_uint16_t));//cp classId
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e) + sizeof(ca_omci_uint16_t)), &me->meBase.instanceId, sizeof(ca_omci_uint16_t));//cp instanceId
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}

