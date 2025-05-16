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
#include "omci_me_eth_frm_pm_ds.h"
#include "omci_me_mac_port_cfg.h"
#include "omci_me_pptp_eth.h"
#include "omci_hal.h"
#include "omci_mq.h"
#include "omci_timer.h"


static ca_omci_me_handler_t sg_omci_me_eth_frm_pm_ds_handlers = {0};


ca_omci_me_attr_t g_omci_me_eth_frm_pm_ds_attr[OMCI_ME_ATTR16] =
{
    {OMCI_ME_ATTR1,  "interval_end_time"},
    {OMCI_ME_ATTR2,  "ctl_block"},
    {OMCI_ME_ATTR3,  "drop_event"},
    {OMCI_ME_ATTR4,  "octets"},
    {OMCI_ME_ATTR5,  "frames"},
    {OMCI_ME_ATTR6,  "broadcast_frame"},
    {OMCI_ME_ATTR7,  "muticast_frame"},
    {OMCI_ME_ATTR8,  "crc_err_frame"},
    {OMCI_ME_ATTR9,  "undersize_frame"},
    {OMCI_ME_ATTR10, "oversize_frame"},
    {OMCI_ME_ATTR11, "frame_oct_64"},
    {OMCI_ME_ATTR12, "frame_oct_127"},
    {OMCI_ME_ATTR13, "frame_oct_255"},
    {OMCI_ME_ATTR14, "frame_oct_511"},
    {OMCI_ME_ATTR15, "frame_oct_1023"},
    {OMCI_ME_ATTR16, "frame_oct_1518"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
static ca_omci_st_e ca_omci_me_pm_ds_eth_port_get(ca_omci_uint16_t instance, ca_omci_uint32_t * portInstance);
void ca_omci_me_eth_frm_pm_ds_timeout(void *data);


ca_omci_uint32_t ca_omci_me_eth_frm_pm_ds_attr_len_get(void)
{
    ca_omci_eth_frm_pm_ds_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.intervalEndT);
    attrLen += sizeof(me->attr.ctlBlock);
    attrLen += sizeof(me->attr.dropEvent);
    attrLen += sizeof(me->attr.octets);
    attrLen += sizeof(me->attr.frames);
    attrLen += sizeof(me->attr.broadcastFrame);
    attrLen += sizeof(me->attr.muticastFrame);
    attrLen += sizeof(me->attr.crcErrFrame);
    attrLen += sizeof(me->attr.undersizeFrame);
    attrLen += sizeof(me->attr.oversizeFrame);
    attrLen += sizeof(me->attr.frameOct64);
    attrLen += sizeof(me->attr.frameOct127);
    attrLen += sizeof(me->attr.frameOct255);
    attrLen += sizeof(me->attr.frameOct511);
    attrLen += sizeof(me->attr.frameOct1023);
    attrLen += sizeof(me->attr.frameOct1518);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_eth_frm_pm_ds_new(void)
{
    ca_omci_eth_frm_pm_ds_t *me = (ca_omci_eth_frm_pm_ds_t *)omci_malloc(sizeof(ca_omci_eth_frm_pm_ds_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ETH_FRM_PM_DS_CLASS_ID, &sg_omci_me_eth_frm_pm_ds_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(eth_frm_pm_ds);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_eth_frm_pm_ds_t *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;
    ca_omci_uint16_t             mask   = 0;
    ca_omci_uint16_t             offset = 0;
    ca_omci_uint8_t              idx    = 0;
    ca_omci_eth_frm_pm_ds_attr_t attrV  = {0};
    ca_omci_hal_pm_cfg_t         cfg    = {0};
    ca_omci_uint32_t             port   = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8_T(ethFrmPmDs->attr.ctlBlock, "ctlBlock", OMCI_CTL_BLOCK_LEN, 2);

    //get eth port id
    if (CA_OMCI_OK != ca_omci_me_pm_ds_eth_port_get(ethFrmPmDs->meBase.instanceId, &port))
    {
        return CA_OMCI_E_PARAM;
    }

    //clear hal static number first
    cfg.port    = port;
    cfg.type    = OMCI_PM_ETH_FRM_DS;
    cfg.dataLen = CA_OMCI_TRUE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attrV))
    {
        OMCIMNGERR("get real time pm error ! ");
        return CA_OMCI_E;
    }

    //start timer to collect data
    if (CA_INVALID_TIMER != ethFrmPmDs->timerId)
    {
        OMCIMNGERR("Can't create a timer twice for me eth_frm_pm_ds");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    ethFrmPmDs->timerId = ca_circle_timer_add(OMCI_TIMER_PM_INTERVAL,
                                              ca_omci_me_eth_frm_pm_ds_timeout,
                                             (void *)ethFrmPmDs);

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_eth_frm_pm_ds_t *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint8_t  idx     = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_FRM_PM_DS_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT_T(8, ethFrmPmDs->attr.ctlBlock, "ctlBlock", OMCI_CTL_BLOCK_LEN, 2);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_eth_frm_pm_ds_t *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_FRM_PM_DS_GET_MASK_FULL);
    OMCI_GET_ATTR_UINT8(ethFrmPmDs->preAttr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT8_T(ethFrmPmDs->preAttr.ctlBlock, OMCI_CTL_BLOCK_LEN, 2);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.dropEvent, 3);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.octets, 4);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frames, 5);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.broadcastFrame, 6);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.muticastFrame, 7);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.crcErrFrame, 8);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.undersizeFrame, 9);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.oversizeFrame, 10);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct64, 11);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct127, 12);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct255, 13);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct511, 14);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct1023, 15);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->preAttr.frameOct1518, 16);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_get_current(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_eth_frm_pm_ds_t     *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;
    ca_omci_uint16_t             offset  = 0;
    ca_omci_uint16_t             mask    = 0;
    ca_omci_eth_frm_pm_ds_attr_t attrV   = {0};
    ca_omci_hal_pm_cfg_t         cfg     = {0};
    ca_omci_uint32_t             port    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_FRM_PM_DS_GET_MASK_FULL);

    //get eth port id
    if (CA_OMCI_OK != ca_omci_me_pm_ds_eth_port_get(ethFrmPmDs->meBase.instanceId, &port))
    {
        return CA_OMCI_E;
    }

    //for now port and datalen has no use for twdm pms
    cfg.port    = port;
    cfg.type    = OMCI_PM_ETH_FRM_DS;
    cfg.dataLen = CA_OMCI_FALSE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attrV))
    {
        OMCIMNGERR("get real time pm error ! ");
        return CA_OMCI_E;
    }

    OMCI_GET_REAL_TIME_ATTR(3,  dropEvent, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(4,  octets, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(5,  frames, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(6,  broadcastFrame, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(7,  muticastFrame, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(8,  crcErrFrame, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(9,  undersizeFrame, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(10, oversizeFrame, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(11, frameOct64, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(12, frameOct127, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(13, frameOct255, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(14, frameOct511, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(15, frameOct1023, ethFrmPmDs);
    OMCI_GET_REAL_TIME_ATTR(16, frameOct1518, ethFrmPmDs);

    OMCI_GET_ATTR_UINT8(ethFrmPmDs->attr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT8_T(ethFrmPmDs->attr.ctlBlock, OMCI_CTL_BLOCK_LEN, 2);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.dropEvent, 3);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.octets, 4);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frames, 5);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.broadcastFrame, 6);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.muticastFrame, 7);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.crcErrFrame, 8);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.undersizeFrame, 9);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.oversizeFrame, 10);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct64, 11);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct127, 12);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct255, 13);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct511, 14);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct1023, 15);
    OMCI_GET_ATTR_UINT32(ethFrmPmDs->attr.frameOct1518, 16);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_eth_frm_pm_ds_t *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    //delete the timer
    if (CA_INVALID_TIMER != ethFrmPmDs->timerId)
    {
        ca_timer_stop(ethFrmPmDs->timerId);
        ca_timer_del(ethFrmPmDs->timerId);
        ethFrmPmDs->timerId = CA_INVALID_TIMER;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_frm_pm_ds_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_eth_frm_pm_ds_t *ethFrmPmDs = (ca_omci_eth_frm_pm_ds_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(ethFrmPmDs);
    OMCI_UPLOAD_ATTR_SIZE(8, ethFrmPmDs->attr.intervalEndT, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, ethFrmPmDs->attr.ctlBlock, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.dropEvent, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.octets, 4);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frames, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.broadcastFrame, 6);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.muticastFrame, 7);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.crcErrFrame, 8);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.undersizeFrame, 9);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.oversizeFrame, 10);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct64, 11);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct127, 12);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct255, 13);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct511, 14);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct1023, 15);
    OMCI_UPLOAD_ATTR_SIZE(32, ethFrmPmDs->attr.frameOct1518, 16);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_eth_frm_pm_ds_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell      *cli = (const struct shell *)data;
    ca_omci_eth_frm_pm_ds_t *pPm = (ca_omci_eth_frm_pm_ds_t *)me;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(data);

    omci_sprint(cli, "Me: ethFrmPmDs \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "============= Current ==============\n");
    omci_sprint(cli, "        dropEvent: %d\n",      pPm->attr.dropEvent);
    omci_sprint(cli, "        octets: %d\n",         pPm->attr.octets);
    omci_sprint(cli, "        frames: %d\n",         pPm->attr.frames);
    omci_sprint(cli, "        broadcastFrame: %d\n", pPm->attr.broadcastFrame);
    omci_sprint(cli, "        muticastFrame: %d\n",  pPm->attr.muticastFrame);
    omci_sprint(cli, "        crcErrFrame: %d\n",    pPm->attr.crcErrFrame);
    omci_sprint(cli, "        undersizeFrame: %d\n", pPm->attr.undersizeFrame);
    omci_sprint(cli, "        oversizeFrame: %d\n",  pPm->attr.oversizeFrame);
    omci_sprint(cli, "        frameOct64: %d\n",     pPm->attr.frameOct64);
    omci_sprint(cli, "        frameOct127: %d\n",    pPm->attr.frameOct127);
    omci_sprint(cli, "        frameOct255: %d\n",    pPm->attr.frameOct255);
    omci_sprint(cli, "        frameOct511: %d\n",    pPm->attr.frameOct511);
    omci_sprint(cli, "        frameOct1023: %d\n",   pPm->attr.frameOct1023);
    omci_sprint(cli, "        frameOct1518: %d\n",   pPm->attr.frameOct1518);
    omci_sprint(cli, "=============== Pre ================\n");
    omci_sprint(cli, "        dropEvent: %d\n",      pPm->preAttr.dropEvent);
    omci_sprint(cli, "        octets: %d\n",         pPm->preAttr.octets);
    omci_sprint(cli, "        frames: %d\n",         pPm->preAttr.frames);
    omci_sprint(cli, "        broadcastFrame: %d\n", pPm->preAttr.broadcastFrame);
    omci_sprint(cli, "        muticastFrame: %d\n",  pPm->preAttr.muticastFrame);
    omci_sprint(cli, "        crcErrFrame: %d\n",    pPm->preAttr.crcErrFrame);
    omci_sprint(cli, "        undersizeFrame: %d\n", pPm->preAttr.undersizeFrame);
    omci_sprint(cli, "        oversizeFrame: %d\n",  pPm->preAttr.oversizeFrame);
    omci_sprint(cli, "        frameOct64: %d\n",     pPm->preAttr.frameOct64);
    omci_sprint(cli, "        frameOct127: %d\n",    pPm->preAttr.frameOct127);
    omci_sprint(cli, "        frameOct255: %d\n",    pPm->preAttr.frameOct255);
    omci_sprint(cli, "        frameOct511: %d\n",    pPm->preAttr.frameOct511);
    omci_sprint(cli, "        frameOct1023: %d\n",   pPm->preAttr.frameOct1023);
    omci_sprint(cli, "        frameOct1518: %d\n",   pPm->preAttr.frameOct1518);

}


void ca_omci_me_eth_frm_pm_ds_init(void)
{
    sg_omci_me_eth_frm_pm_ds_handlers.uploadProc = ca_omci_me_eth_frm_pm_ds_upload;
    sg_omci_me_eth_frm_pm_ds_handlers.meDisplay  = ca_omci_me_eth_frm_pm_ds_display;

    sg_omci_me_eth_frm_pm_ds_handlers.createProc = ca_omci_me_eth_frm_pm_ds_create;
    sg_omci_me_eth_frm_pm_ds_handlers.deleteProc = ca_omci_me_eth_frm_pm_ds_delete;
    sg_omci_me_eth_frm_pm_ds_handlers.setProc    = ca_omci_me_eth_frm_pm_ds_set;
    sg_omci_me_eth_frm_pm_ds_handlers.getProc    = ca_omci_me_eth_frm_pm_ds_get;
    sg_omci_me_eth_frm_pm_ds_handlers.getCurProc = ca_omci_me_eth_frm_pm_ds_get_current;
}


static ca_omci_st_e ca_omci_me_pm_ds_eth_port_get(
    ca_omci_uint16_t instance,
    ca_omci_uint32_t *portInstance)
{
    ca_omci_mac_port_cfg_t *macPort = NULL;
    ca_omci_pptp_eth_t     *pptpEth = NULL;

    macPort = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, instance);
    if ((NULL == macPort) || (MAC_PORT_CFG_TP_TYPE_PPTP_ETH != macPort->attr.tpType))
    {
        OMCIMNGERR("Only support ppth ETH for eth frame PM [0x%x]!", instance);
        return CA_OMCI_E;
    }

    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, macPort->attr.tpPointer);
    if (NULL == pptpEth)
    {
        OMCIMNGERR("Wrong ppth ETH for eth frame PM [0x%x]!", instance);
        return CA_OMCI_E;
    }

    *portInstance = pptpEth->meBase.instanceId;
    return CA_OMCI_OK;
}


void ca_omci_me_eth_frm_pm_ds_timeout_handle(void *pData)
{
    ca_omci_uint16_t             *inst = (ca_omci_uint16_t *)pData;
    ca_omci_eth_frm_pm_ds_t      *me   = NULL;
    ca_omci_eth_frm_pm_ds_attr_t  attr = {0};
    ca_omci_hal_pm_cfg_t          cfg  = {0};
    ca_omci_uint32_t              port = 0;

    OMCI_ENTER;
    me = (ca_omci_eth_frm_pm_ds_t *)ca_omci_me_entry_lookup(OMCI_ME_ETH_FRM_PM_DS_CLASS_ID, *inst);
    OMCI_PARAM_IS_NULL(me);

    //get eth port id
    if (CA_OMCI_OK != ca_omci_me_pm_ds_eth_port_get(me->meBase.instanceId, &port))
    {
        return;
    }

    //for now port and datalen has no use for twdm pms
    cfg.port    = port;
    cfg.type    = OMCI_PM_ETH_FRM_DS;
    cfg.dataLen = CA_OMCI_TRUE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attr))
    {
        OMCIMNGERR("get real time pm error!");
        return;
    }

    //update interval end time
    me->preAttr.intervalEndT = me->attr.intervalEndT;
    me->attr.intervalEndT++;

    OMCI_UPDATE_PM(dropEvent);
    OMCI_UPDATE_PM(octets);
    OMCI_UPDATE_PM(frames);
    OMCI_UPDATE_PM(broadcastFrame);
    OMCI_UPDATE_PM(muticastFrame);
    OMCI_UPDATE_PM(crcErrFrame);
    OMCI_UPDATE_PM(undersizeFrame);
    OMCI_UPDATE_PM(oversizeFrame);
    OMCI_UPDATE_PM(frameOct64);
    OMCI_UPDATE_PM(frameOct127);
    OMCI_UPDATE_PM(frameOct255);
    OMCI_UPDATE_PM(frameOct511);
    OMCI_UPDATE_PM(frameOct1023);
    OMCI_UPDATE_PM(frameOct1518);
}


void ca_omci_me_eth_frm_pm_ds_timeout(void *data)
{
    ca_omci_eth_frm_pm_ds_t *me = (ca_omci_eth_frm_pm_ds_t *)data;
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

