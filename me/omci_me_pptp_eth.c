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
#include "omci_mng_zephyr_cmd.h"
#include "omci_common.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_me_pptp_eth.h"
#include "omci_timer.h"
#include "omci_mng_am.h"
#include "omci_mq.h"
#include "omci_hal.h"


static ca_omci_me_handler_t sg_omci_me_pptp_eth_handlers = {0};


ca_omci_me_attr_t g_omci_me_pptp_eth_attr[OMCI_ME_ATTR15] =
{
    {OMCI_ME_ATTR1,  "expect_type"},
    {OMCI_ME_ATTR2,  "sensed_type"},
    {OMCI_ME_ATTR3,  "auto_detec_cfg"},
    {OMCI_ME_ATTR4,  "eth_loopback_cfg"},
    {OMCI_ME_ATTR5,  "admin"},
    {OMCI_ME_ATTR6,  "oper_state"},
    {OMCI_ME_ATTR7,  "cfg_ind"},
    {OMCI_ME_ATTR8,  "max_frame_size"},
    {OMCI_ME_ATTR9,  "dte_dce_ind"},
    {OMCI_ME_ATTR10, "pause_time"},
    {OMCI_ME_ATTR11, "brg_ip_ind"},
    {OMCI_ME_ATTR12, "arc"},
    {OMCI_ME_ATTR13, "arc_interval"},
    {OMCI_ME_ATTR14, "pppoe_filter"},
    {OMCI_ME_ATTR15, "power_control"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
ca_omci_st_e ca_omci_me_pptp_eth_state_get(ca_omci_pptp_eth_t * me,ca_omci_uint16_t mask);
void ca_omci_me_pptp_eth_st_report(ca_omci_uint16_t instance, ca_omci_uint32_t state);


ca_omci_uint32_t ca_omci_me_pptp_eth_attr_len_get(void)
{
    ca_omci_pptp_eth_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.expectType);
    attrLen += sizeof(me->attr.sensedType);
    attrLen += sizeof(me->attr.autoDetectCfg);
    attrLen += sizeof(me->attr.ethLoopbackCfg);
    attrLen += sizeof(me->attr.admin);
    attrLen += sizeof(me->attr.operState);
    attrLen += sizeof(me->attr.cfgInd);
    attrLen += sizeof(me->attr.maxFrameSize);
    attrLen += sizeof(me->attr.dteDceInd);
    attrLen += sizeof(me->attr.pauseTime);
    attrLen += sizeof(me->attr.brgIpInd);
    attrLen += sizeof(me->attr.arc);
    attrLen += sizeof(me->attr.arcInterval);
    attrLen += sizeof(me->attr.pppoeFilter);
    attrLen += sizeof(me->attr.powerCtrl);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_pptp_eth_new(void)
{
    ca_omci_pptp_eth_t *me = (ca_omci_pptp_eth_t *)omci_malloc(sizeof(ca_omci_pptp_eth_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_PPTP_ETH_CLASS_ID, &sg_omci_me_pptp_eth_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(pptp_eth);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_pptp_eth_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_pptp_eth_t      *pptpEth = (ca_omci_pptp_eth_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_bool_t   state   = CA_OMCI_FALSE;
    ca_omci_uint32_t flag    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_PPTP_ETH_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.expectType, "expectType", 1);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.autoDetectCfg, "autoDetectCfg", 3);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.ethLoopbackCfg, "ethLoopbackCfg", 4);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.admin, "admin", 5);
    OMCI_UPDATE_ATTR_UINT(16, pptpEth->attr.maxFrameSize, "maxFrameSize", 8);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.dteDceInd, "dteDceInd", 9);
    OMCI_UPDATE_ATTR_UINT(16, pptpEth->attr.pauseTime, "pauseTime", 10);
    OMCI_UPDATE_ATTR_UINT(16, pptpEth->attr.brgIpInd, "brgIpInd", 11);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.arc, "arc", 12);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.arcInterval, "arcInterval", 13);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.pppoeFilter, "pppoeFilter", 14);
    OMCI_UPDATE_ATTR_UINT(8, pptpEth->attr.powerCtrl, "powerCtrl", 15);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR3, mask))
    {
        ca_omci_hal_eth_mode_set(&pptpEth->meBase.instanceId, pptpEth->attr.autoDetectCfg);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR4, mask))
    {
        ca_omci_hal_eth_lb_set(&pptpEth->meBase.instanceId, pptpEth->attr.ethLoopbackCfg);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, mask))
    {
        //get port state, if the port is down, OMCI need to report the alarm
        ca_omci_hal_eth_state_get(&pptpEth->meBase.instanceId, &state);
        if (CA_OMCI_FALSE == state)
        {
            //when the port do not link up, OMCI need to report alarm and AVC
            flag = (CA_OMCI_LOCK == pptpEth->attr.admin) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
            OMCIDBG("report port %x %s", pptpEth->meBase.instanceId, CA_OMCI_TRUE == flag ? "los-clear":"los");
            ca_omci_me_pptp_eth_st_report(pptpEth->meBase.instanceId, flag);
        }
        ca_omci_hal_eth_admin_set(&pptpEth->meBase.instanceId, pptpEth->attr.admin);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, mask))
    {
        ca_omci_hal_eth_max_frame_set(&pptpEth->meBase.instanceId, pptpEth->attr.maxFrameSize);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_pptp_eth_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_pptp_eth_t *pptpEth = (ca_omci_pptp_eth_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_PPTP_ETH_GET_MASK_FULL);

    //update value in me
    ca_omci_me_pptp_eth_state_get(pptpEth, inMask);

    OMCI_GET_ATTR_UINT8(pptpEth->attr.expectType, 1);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.sensedType, 2);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.autoDetectCfg, 3);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.ethLoopbackCfg, 4);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.admin, 5);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.operState, 6);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.cfgInd, 7);
    OMCI_GET_ATTR_UINT16(pptpEth->attr.maxFrameSize, 8);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.dteDceInd, 9);
    OMCI_GET_ATTR_UINT16(pptpEth->attr.pauseTime, 10);
    OMCI_GET_ATTR_UINT16(pptpEth->attr.brgIpInd, 11);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.arc, 12);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.arcInterval, 13);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.pppoeFilter, 14);
    OMCI_GET_ATTR_UINT8(pptpEth->attr.powerCtrl, 15);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_pptp_eth_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_pptp_eth_t *pptpEth = (ca_omci_pptp_eth_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(pptpEth);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.expectType, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.sensedType, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.autoDetectCfg, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.ethLoopbackCfg, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.admin, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.operState, 6);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.cfgInd, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, pptpEth->attr.maxFrameSize, 8);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.dteDceInd, 9);
    OMCI_UPLOAD_ATTR_SIZE(16, pptpEth->attr.pauseTime, 10);
    OMCI_UPLOAD_ATTR_SIZE(16, pptpEth->attr.brgIpInd, 11);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.arc, 12);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.arcInterval, 13);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.pppoeFilter, 14);
    OMCI_UPLOAD_ATTR_SIZE(8, pptpEth->attr.powerCtrl, 15);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_pptp_eth_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_pptp_eth_t *pptpEth = (ca_omci_pptp_eth_t *)me;

    OMCI_PARAM_IS_NULL(me);
    (void) data;

    omci_sprint(cli, "Me: pptpEth \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        expectType:  0x%x\n", pptpEth->attr.expectType);
    omci_sprint(cli, "        sensedType:  0x%x\n", pptpEth->attr.sensedType);
    omci_sprint(cli, "        autoDetectCfg: 0x%x\n", pptpEth->attr.autoDetectCfg);
    omci_sprint(cli, "        ethLoopbackCfg: 0x%x\n", pptpEth->attr.ethLoopbackCfg);
    omci_sprint(cli, "        admin:       0x%x\n", pptpEth->attr.admin);
    omci_sprint(cli, "        operState:   0x%x\n", pptpEth->attr.operState);
    omci_sprint(cli, "        cfgInd:      0x%x\n", pptpEth->attr.cfgInd);
    omci_sprint(cli, "        maxFrameSize: 0x%x\n", pptpEth->attr.maxFrameSize);
    omci_sprint(cli, "        dteDceInd:   0x%x\n", pptpEth->attr.dteDceInd);
    omci_sprint(cli, "        pauseTime:   0x%x\n", pptpEth->attr.pauseTime);
    omci_sprint(cli, "        brgIpInd:    0x%x\n", pptpEth->attr.brgIpInd);
    omci_sprint(cli, "        arc:         0x%x\n", pptpEth->attr.arc);
    omci_sprint(cli, "        arcInterval: 0x%x\n", pptpEth->attr.arcInterval);
    omci_sprint(cli, "        pppoeFilter: 0x%x\n", pptpEth->attr.pppoeFilter);
    omci_sprint(cli, "        powerCtrl:   0x%x\n", pptpEth->attr.powerCtrl);
}


void ca_omci_me_pptp_eth_init(void)
{
    sg_omci_me_pptp_eth_handlers.meDisplay  = ca_omci_me_pptp_eth_display;
    sg_omci_me_pptp_eth_handlers.uploadProc = ca_omci_me_pptp_eth_upload;

    sg_omci_me_pptp_eth_handlers.setProc    = ca_omci_me_pptp_eth_set;
    sg_omci_me_pptp_eth_handlers.getProc    = ca_omci_me_pptp_eth_get;
}


ca_omci_st_e ca_omci_me_pptp_eth_state_get(
    ca_omci_pptp_eth_t *me,
    ca_omci_uint16_t mask)
{
    ca_omci_uint8_t mode  = 0;
    ca_omci_bool_t  state = CA_OMCI_FALSE;

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        //get mode
        ca_omci_hal_eth_state_get(&me->meBase.instanceId, &state);
        if (CA_OMCI_TRUE == state)
        {
            ca_omci_hal_eth_mode_get(&me->meBase.instanceId, &mode);
            me->attr.cfgInd = mode;
        }
        else
        {
            me->attr.cfgInd = 0; //status is unknown
        }
    }

    return CA_OMCI_OK;
}


void ca_omci_me_pptp_eth_st_report(ca_omci_uint16_t instance, ca_omci_uint32_t state)
{
    ca_omci_alarm_report_t *pData = NULL;
    ca_omci_inner_msg_t     msg;

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    pData             = (ca_omci_alarm_report_t *)&msg.data;
    pData->alarmId    = CA_OMCI_ALARM_PPTP_ETH_LAN_LOS;
    pData->instanceId = instance;
    pData->state      = state;
    msg.hdr.msgType   = OMCI_INNER_MSG_TYPE_ALARM;
    msg.hdr.dataLen   = sizeof(ca_omci_alarm_report_t);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIHALERR("send to inner mq error");
    }
}


void ca_omci_me_pptp_eth_link_st_timeout_handler(void *data)
{
    ca_omci_pptp_eth_t *me       = (ca_omci_pptp_eth_t *)data;
    ca_omci_pkt_avc_t   avcAttr  = {0};
    ca_omci_uint16_t    mask     = 0;
    ca_omci_uint8_t     idx      = 0;
    ca_omci_uint8_t     alarmSt  = 0;

    if (NULL == me)
    {
        OMCIMNGERR("Can't report instance");
        return;
    }

    alarmSt = (CA_OMCI_OPER_ENABLE == me->attr.operState) ? CA_OMCI_ALARM_CLEAR : CA_OMCI_ALARM_RAISE;
    OMCI_MEMSET(&avcAttr, 0, sizeof(ca_omci_pkt_avc_t));
    avcAttr.attr[avcAttr.attrNum].pAttr = omci_malloc(sizeof(ca_omci_uint8_t));
    if (NULL == avcAttr.attr[avcAttr.attrNum].pAttr)
    {
        OMCIMNGERR("No resource for avc report");
        return;
    }
    OMCI_MEMCPY(avcAttr.attr[avcAttr.attrNum].pAttr, &me->attr.operState, sizeof(ca_omci_uint8_t));
    avcAttr.attr[avcAttr.attrNum].attrLen = sizeof(ca_omci_uint8_t);
    avcAttr.attrNum++;
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);

    avcAttr.attr[avcAttr.attrNum].pAttr = omci_malloc(sizeof(ca_omci_uint8_t));
    if (NULL == avcAttr.attr[avcAttr.attrNum].pAttr)
    {
        OMCIMNGERR("No resource for avc report");
        OMCI_FREE(avcAttr.attr[0].pAttr);
        return;
    }
    OMCI_MEMCPY(avcAttr.attr[avcAttr.attrNum].pAttr, &me->attr.arc, sizeof(ca_omci_uint8_t));
    avcAttr.attr[avcAttr.attrNum].attrLen = sizeof(ca_omci_uint8_t);
    avcAttr.attrNum++;
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);

    omci_codec_encode_avc(&me->meBase, mask, &avcAttr);
    ca_omci_alarm_report(&me->meBase, 0, alarmSt);

    for (idx = 0; idx < avcAttr.attrNum; idx++)
    {
        if (NULL != avcAttr.attr[idx].pAttr)
        {
            OMCI_FREE(avcAttr.attr[idx].pAttr);
        }
    }
}


void ca_omci_me_pptp_eth_link_st_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e  timerDef = CA_OMCI_TIMER_ME_PPTP_ETH_ARC_IVL;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_pptp_eth_t) + sizeof(ca_omci_timer_def_e);//pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef,sizeof(ca_omci_timer_def_e)); //cp timer def
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e)), data, sizeof(ca_omci_pptp_eth_t));//cp pData
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}

//state only have up or down, false mean down, true mean up
void ca_omci_me_pptp_eth_link_st_change(
    ca_omci_uint16_t instance,
    ca_omci_uint32_t state)
{
    ca_omci_pptp_eth_t *me      = NULL;
    ca_omci_pkt_avc_t   avcAttr = {0};
    ca_omci_uint16_t    mask    = 0;
    ca_omci_uint8_t     idx     = 0;
    ca_omci_uint8_t     alarmSt = 0;
    ca_omci_uint8_t     operSt  = CA_OMCI_OPER_ENABLE;
    ca_omci_bool_t      link    = CA_OMCI_FALSE;

    alarmSt = (CA_OMCI_FALSE == state) ? CA_OMCI_ALARM_RAISE : CA_OMCI_ALARM_CLEAR;
    operSt  = (CA_OMCI_FALSE == state) ? CA_OMCI_OPER_DISABLE : CA_OMCI_OPER_ENABLE;
    me      = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, instance);
    if (NULL == me)
    {
        OMCIMNGERR("Can't find me for instance 0x%x", instance);
        return;
    }

    //no need to report the state when no change
    if (me->attr.operState == operSt)
    {
        //when the port link down, and still got the report the alarm, no need to report the AVC
        ca_omci_hal_eth_state_get(&me->meBase.instanceId, &link);
        if (CA_OMCI_FALSE == link)
        {
            ca_omci_alarm_report(&me->meBase, 0, alarmSt);
            OMCIDBG("only report port alarm %x %s", me->meBase.instanceId, CA_OMCI_ALARM_RAISE == alarmSt ? "raise":"clear");
        }
        return;
    }

    OMCIDBG("report port alarm and avc %x %s %d", me->meBase.instanceId, CA_OMCI_ALARM_RAISE == alarmSt ? "raise":"clear", operSt);

    //encode the operstate avc
    me->attr.operState = operSt;
    OMCI_MEMSET(&avcAttr, 0, sizeof(ca_omci_pkt_avc_t));
    avcAttr.attr[avcAttr.attrNum].pAttr = omci_malloc(sizeof(ca_omci_uint8_t));
    if (NULL == avcAttr.attr[avcAttr.attrNum].pAttr)
    {
        OMCIMNGERR("No resource for avc report");
        goto freeavc;
    }
    OMCI_MEMCPY(avcAttr.attr[avcAttr.attrNum].pAttr, &me->attr.operState, sizeof(ca_omci_uint8_t));
    avcAttr.attr[avcAttr.attrNum].attrLen = sizeof(ca_omci_uint8_t);
    avcAttr.attrNum++;
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);

    //if arc disable, report avc now
    if (CA_OMCI_FALSE == me->attr.arc)
    {
        omci_codec_encode_avc(&me->meBase, mask, &avcAttr);
        ca_omci_alarm_report(&me->meBase, 0, alarmSt);
        goto freeavc;
    }

    //if avc is enable, so check the interval
    me->attr.arc = CA_OMCI_FALSE;
    avcAttr.attr[avcAttr.attrNum].pAttr = omci_malloc(sizeof(ca_omci_uint8_t));
    if (NULL == avcAttr.attr[avcAttr.attrNum].pAttr)
    {
        OMCIMNGERR("No resource for avc report");
        goto freeavc;
    }
    OMCI_MEMCPY(avcAttr.attr[avcAttr.attrNum].pAttr, &me->attr.arc, sizeof(ca_omci_uint8_t));
    avcAttr.attr[avcAttr.attrNum].attrLen = sizeof(ca_omci_uint8_t);
    avcAttr.attrNum++;
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);

    //when interval is 255, don't report forever
    if (255 == me->attr.arcInterval)
    {
        OMCIMNGDBG("Don't report avc for interval is 255");
        goto freeavc;
    }

    //if interval, report now
    if (0 == me->attr.arcInterval)
    {
        //send the arc now, with arc bit
        omci_codec_encode_avc(&me->meBase, mask, &avcAttr);
        ca_omci_alarm_report(&me->meBase, 0, alarmSt);
        goto freeavc;
    }

    ca_timer_add((me->attr.arcInterval * 60 * 10000),
                 ca_omci_me_pptp_eth_link_st_timeout,
                (void *)me);

freeavc:
    for (idx = 0; idx < avcAttr.attrNum; idx++)
    {
        if (NULL != avcAttr.attr[idx].pAttr)
        {
            OMCI_FREE(avcAttr.attr[idx].pAttr);
        }
    }
}


void ca_omci_me_pptp_eth_restore(void)
{
    ca_omci_meclass_t    *meClass = NULL;
    ca_omci_mebase_t     *meInst  = NULL;
    ca_omci_pptp_eth_t   *me      = NULL;
    ca_omci_bool_t        state   = CA_OMCI_FALSE;
    ca_omci_uint32_t      flag    = CA_OMCI_FALSE;

    meClass = ca_omci_me_get(OMCI_ME_PPTP_ETH_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me    = (ca_omci_pptp_eth_t *)meInst;
        state = CA_OMCI_FALSE;

        ca_omci_hal_eth_mode_set(&me->meBase.instanceId, me->attr.autoDetectCfg);
        ca_omci_hal_eth_lb_set(&me->meBase.instanceId, me->attr.ethLoopbackCfg);
        ca_omci_hal_eth_max_frame_set(&me->meBase.instanceId, me->attr.maxFrameSize);

        //get port state, if the port is down, OMCI need to report the alarm
        ca_omci_hal_eth_state_get(&me->meBase.instanceId, &state);
        if (CA_OMCI_FALSE == state)
        {
            //when the port do not link up, OMCI need to report alarm and AVC
            flag = (CA_OMCI_LOCK == me->attr.admin) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
            OMCIDBG("report port %x %s", me->meBase.instanceId, CA_OMCI_TRUE == flag ? "los-clear":"los");
            ca_omci_me_pptp_eth_st_report(me->meBase.instanceId, flag);
        }

        ca_omci_hal_eth_admin_set(&me->meBase.instanceId, me->attr.admin);
    }

    return;
}

