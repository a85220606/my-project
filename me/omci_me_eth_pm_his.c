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
#include "omci_me_eth_pm_his.h"
#include "omci_me_pptp_eth.h"
#include "omci_hal.h"
#include "omci_mq.h"
#include "omci_timer.h"


static ca_omci_me_handler_t sg_omci_me_eth_pm_his_handlers = {0};


ca_omci_me_attr_t g_omci_me_eth_pm_his_attr[OMCI_ME_ATTR16] =
{
    {OMCI_ME_ATTR1,  "interval_end_time"},
    {OMCI_ME_ATTR2,  "thres_id"},
    {OMCI_ME_ATTR3,  "fcs_err"},
    {OMCI_ME_ATTR4,  "excess_coll_cnt"},
    {OMCI_ME_ATTR5,  "late_coll_cnt"},
    {OMCI_ME_ATTR6,  "frame_too_long"},
    {OMCI_ME_ATTR7,  "rx_buff_overflow"},
    {OMCI_ME_ATTR8,  "tx_buff_overflow"},
    {OMCI_ME_ATTR9,  "single_coll_frame_cnt"},
    {OMCI_ME_ATTR10, "multi_coll_frame_cnt"},
    {OMCI_ME_ATTR11, "sqe_cnt"},
    {OMCI_ME_ATTR12, "deferred_tx_cnt"},
    {OMCI_ME_ATTR13, "internal_mac_tx_err_cnt"},
    {OMCI_ME_ATTR14, "carrier_sense_err_cnt"},
    {OMCI_ME_ATTR15, "align_err_cnt"},
    {OMCI_ME_ATTR16, "internal_mac_rx_err_cnt"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
void ca_omci_me_eth_pm_his_timeout(void *data);


ca_omci_uint32_t ca_omci_me_eth_pm_his_attr_len_get(void)
{
    ca_omci_eth_pm_his_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.intervalEndT);
    attrLen += sizeof(me->attr.thresId);
    attrLen += sizeof(me->attr.fcsErr);
    attrLen += sizeof(me->attr.excessCollCnt);
    attrLen += sizeof(me->attr.lateCollCnt);
    attrLen += sizeof(me->attr.frameTooLong);
    attrLen += sizeof(me->attr.rxBuffOverflow);
    attrLen += sizeof(me->attr.txBuffOverflow);
    attrLen += sizeof(me->attr.singleCollFrameCnt);
    attrLen += sizeof(me->attr.multiCollFrameCnt);
    attrLen += sizeof(me->attr.sqeCnt);
    attrLen += sizeof(me->attr.deferredTxCnt);
    attrLen += sizeof(me->attr.internalMacTxErrCnt);
    attrLen += sizeof(me->attr.carrierSenseErrCnt);
    attrLen += sizeof(me->attr.alignErrCnt);
    attrLen += sizeof(me->attr.internalMacRxErrCnt);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_eth_pm_his_new(void)
{
    ca_omci_eth_pm_his_t *me = (ca_omci_eth_pm_his_t *)omci_malloc(sizeof(ca_omci_eth_pm_his_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ETH_PM_HIS_CLASS_ID, &sg_omci_me_eth_pm_his_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(eth_pm_his);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_eth_pm_his_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;
    ca_omci_uint16_t           mask     = 0;
    ca_omci_uint16_t           offset   = 0;
    ca_omci_pptp_eth_t        *pptpEth = NULL;
    ca_omci_eth_pm_his_attr_t  attrV   = {0};
    ca_omci_hal_pm_cfg_t       cfg     = {0};

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(ethPmHis->attr.thresId, "thresId", 2);

    //get eth port id
    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ethPmHis->meBase.instanceId);
    if (NULL == pptpEth)
    {
        OMCIMNGERR("Wrong ppth ETH for eth PM [0x%x]!", ethPmHis->meBase.instanceId);
        return CA_OMCI_E;
    }

    //clear hal static number first
    cfg.port    = ethPmHis->meBase.instanceId;
    cfg.type    = OMCI_PM_ETH;
    cfg.dataLen = CA_OMCI_TRUE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attrV))
    {
        OMCIMNGERR("get real time pm error ! ");
        return CA_OMCI_E;
    }

    //start timer to collect data
    if (CA_INVALID_TIMER != ethPmHis->timerId)
    {
        OMCIMNGERR("Can't create a timer twice for me eth_pm_his");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    ethPmHis->timerId = ca_circle_timer_add(OMCI_TIMER_PM_INTERVAL,
                                            ca_omci_me_eth_pm_his_timeout,
                                           (void *)ethPmHis);

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_pm_his_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_PM_HIS_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(16, ethPmHis->attr.thresId, "thresId", 2);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_pm_his_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_PM_HIS_GET_MASK_FULL);
    OMCI_GET_ATTR_UINT8(ethPmHis->preAttr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT16(ethPmHis->preAttr.thresId, 2);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.fcsErr, 3);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.excessCollCnt, 4);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.lateCollCnt, 5);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.frameTooLong, 6);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.rxBuffOverflow, 7);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.txBuffOverflow, 8);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.singleCollFrameCnt, 9);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.multiCollFrameCnt, 10);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.sqeCnt, 11);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.deferredTxCnt, 12);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.internalMacTxErrCnt, 13);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.carrierSenseErrCnt, 14);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.alignErrCnt, 15);
    OMCI_GET_ATTR_UINT32(ethPmHis->preAttr.internalMacRxErrCnt, 16);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_pm_his_get_current(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_eth_pm_his_t      *ethPmHis = (ca_omci_eth_pm_his_t *)me;
    ca_omci_uint16_t           offset   = 0;
    ca_omci_uint16_t           mask     = 0;
    ca_omci_pptp_eth_t        *pptpEth  = NULL;
    ca_omci_eth_pm_his_attr_t  attrV    = {0};
    ca_omci_hal_pm_cfg_t       cfg      = {0};

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ETH_PM_HIS_GET_MASK_FULL);

    //get eth port id
    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ethPmHis->meBase.instanceId);
    if (NULL == pptpEth)
    {
        OMCIMNGERR("Wrong ppth ETH for eth PM [0x%x]!", ethPmHis->meBase.instanceId);
        return CA_OMCI_E;
    }

    //clear hal static number first
    cfg.port    = ethPmHis->meBase.instanceId;
    cfg.type    = OMCI_PM_ETH;
    cfg.dataLen = CA_OMCI_FALSE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attrV))
    {
        OMCIMNGERR("get real time pm error ! ");
        return CA_OMCI_E;
    }

    OMCI_GET_REAL_TIME_ATTR(3,  fcsErr, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(4,  excessCollCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(5,  lateCollCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(6,  frameTooLong, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(7,  rxBuffOverflow, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(8,  txBuffOverflow, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(9,  singleCollFrameCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(10, multiCollFrameCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(11, sqeCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(12, deferredTxCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(13, internalMacTxErrCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(14, carrierSenseErrCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(15, alignErrCnt, ethPmHis);
    OMCI_GET_REAL_TIME_ATTR(16, internalMacRxErrCnt, ethPmHis);

    OMCI_GET_ATTR_UINT8(ethPmHis->attr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT16(ethPmHis->attr.thresId, 2);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.fcsErr, 3);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.excessCollCnt, 4);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.lateCollCnt, 5);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.frameTooLong, 6);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.rxBuffOverflow, 7);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.txBuffOverflow, 8);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.singleCollFrameCnt, 9);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.multiCollFrameCnt, 10);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.sqeCnt, 11);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.deferredTxCnt, 12);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.internalMacTxErrCnt, 13);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.carrierSenseErrCnt, 14);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.alignErrCnt, 15);
    OMCI_GET_ATTR_UINT32(ethPmHis->attr.internalMacRxErrCnt, 16);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_pm_his_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    //delete the timer
    if (CA_INVALID_TIMER != ethPmHis->timerId)
    {
        ca_timer_stop(ethPmHis->timerId);
        ca_timer_del(ethPmHis->timerId);
        ethPmHis->timerId = CA_INVALID_TIMER;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eth_pm_his_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(ethPmHis);
    OMCI_UPLOAD_ATTR_SIZE(8, ethPmHis->attr.intervalEndT, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, ethPmHis->attr.thresId, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.fcsErr, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.excessCollCnt, 4);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.lateCollCnt, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.frameTooLong, 6);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.rxBuffOverflow, 7);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.txBuffOverflow, 8);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.singleCollFrameCnt, 9);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.multiCollFrameCnt, 10);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.sqeCnt, 11);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.deferredTxCnt, 12);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.internalMacTxErrCnt, 13);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.carrierSenseErrCnt, 14);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.alignErrCnt, 15);
    OMCI_UPLOAD_ATTR_SIZE(32, ethPmHis->attr.internalMacRxErrCnt, 16);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_eth_pm_his_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell   *cli      = (const struct shell *)data;
    ca_omci_eth_pm_his_t *ethPmHis = (ca_omci_eth_pm_his_t *)me;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(data);

    omci_sprint(cli, "Me: ethPmHis \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "============= Current ==============\n");
    omci_sprint(cli, "        intervalEndT: %d\n",        ethPmHis->attr.intervalEndT);
    omci_sprint(cli, "        excessCollCnt: %d\n",       ethPmHis->attr.excessCollCnt);
    omci_sprint(cli, "        excessCollCnt: %d\n",       ethPmHis->attr.excessCollCnt);
    omci_sprint(cli, "        lateCollCnt: %d\n",         ethPmHis->attr.lateCollCnt);
    omci_sprint(cli, "        frameTooLong: %d\n",        ethPmHis->attr.frameTooLong);
    omci_sprint(cli, "        rxBuffOverflow: %d\n",      ethPmHis->attr.rxBuffOverflow);
    omci_sprint(cli, "        txBuffOverflow: %d\n",      ethPmHis->attr.txBuffOverflow);
    omci_sprint(cli, "        singleCollFrameCnt: %d\n",  ethPmHis->attr.singleCollFrameCnt);
    omci_sprint(cli, "        multiCollFrameCnt: %d\n",   ethPmHis->attr.multiCollFrameCnt);
    omci_sprint(cli, "        sqeCnt: %d\n",              ethPmHis->attr.sqeCnt);
    omci_sprint(cli, "        deferredTxCnt: %d\n",       ethPmHis->attr.deferredTxCnt);
    omci_sprint(cli, "        internalMacTxErrCnt: %d\n", ethPmHis->attr.internalMacTxErrCnt);
    omci_sprint(cli, "        carrierSenseErrCnt: %d\n",  ethPmHis->attr.carrierSenseErrCnt);
    omci_sprint(cli, "        alignErrCnt: %d\n",         ethPmHis->attr.alignErrCnt);
    omci_sprint(cli, "        internalMacRxErrCnt: %d\n", ethPmHis->attr.internalMacRxErrCnt);
    omci_sprint(cli, "=============== Pre ================\n");
    omci_sprint(cli, "        intervalEndT: %d\n",        ethPmHis->preAttr.intervalEndT);
    omci_sprint(cli, "        excessCollCnt: %d\n",       ethPmHis->preAttr.excessCollCnt);
    omci_sprint(cli, "        excessCollCnt: %d\n",       ethPmHis->preAttr.excessCollCnt);
    omci_sprint(cli, "        lateCollCnt: %d\n",         ethPmHis->preAttr.lateCollCnt);
    omci_sprint(cli, "        frameTooLong: %d\n",        ethPmHis->preAttr.frameTooLong);
    omci_sprint(cli, "        rxBuffOverflow: %d\n",      ethPmHis->preAttr.rxBuffOverflow);
    omci_sprint(cli, "        txBuffOverflow: %d\n",      ethPmHis->preAttr.txBuffOverflow);
    omci_sprint(cli, "        singleCollFrameCnt: %d\n",  ethPmHis->preAttr.singleCollFrameCnt);
    omci_sprint(cli, "        multiCollFrameCnt: %d\n",   ethPmHis->preAttr.multiCollFrameCnt);
    omci_sprint(cli, "        sqeCnt: %d\n",              ethPmHis->preAttr.sqeCnt);
    omci_sprint(cli, "        deferredTxCnt: %d\n",       ethPmHis->preAttr.deferredTxCnt);
    omci_sprint(cli, "        internalMacTxErrCnt: %d\n", ethPmHis->preAttr.internalMacTxErrCnt);
    omci_sprint(cli, "        carrierSenseErrCnt: %d\n",  ethPmHis->preAttr.carrierSenseErrCnt);
    omci_sprint(cli, "        alignErrCnt: %d\n",         ethPmHis->preAttr.alignErrCnt);
    omci_sprint(cli, "        internalMacRxErrCnt: %d\n", ethPmHis->preAttr.internalMacRxErrCnt);

}


void ca_omci_me_eth_pm_his_init(void)
{
    sg_omci_me_eth_pm_his_handlers.uploadProc = ca_omci_me_eth_pm_his_upload;
    sg_omci_me_eth_pm_his_handlers.meDisplay  = ca_omci_me_eth_pm_his_display;

    sg_omci_me_eth_pm_his_handlers.createProc = ca_omci_me_eth_pm_his_create;
    sg_omci_me_eth_pm_his_handlers.deleteProc = ca_omci_me_eth_pm_his_delete;
    sg_omci_me_eth_pm_his_handlers.setProc    = ca_omci_me_eth_pm_his_set;
    sg_omci_me_eth_pm_his_handlers.getProc    = ca_omci_me_eth_pm_his_get;
    sg_omci_me_eth_pm_his_handlers.getCurProc = ca_omci_me_eth_pm_his_get_current;
}


void ca_omci_me_eth_pm_his_timeout_handle(void *pData)
{
    ca_omci_uint16_t          *inst    = (ca_omci_uint16_t *)pData;
    ca_omci_eth_pm_his_t      *me      = NULL;
    ca_omci_pptp_eth_t        *pptpEth = NULL;
    ca_omci_eth_pm_his_attr_t  attr    = {0};
    ca_omci_hal_pm_cfg_t       cfg     = {0};

    OMCI_ENTER;
    me = (ca_omci_eth_pm_his_t *)ca_omci_me_entry_lookup(OMCI_ME_ETH_PM_HIS_CLASS_ID, *inst);
    OMCI_PARAM_IS_NULL(me);

    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, me->meBase.instanceId);
    if (NULL == pptpEth)
    {
        OMCIMNGERR("Wrong ppth ETH for eth PM [0x%x]!", me->meBase.instanceId);
        return;
    }

    //for now port and datalen has no use for twdm pms
    cfg.port    = me->meBase.instanceId;
    cfg.type    = OMCI_PM_ETH;
    cfg.dataLen = CA_OMCI_TRUE;//use for read clear
    if (CA_OMCI_OK != ca_omci_hal_pm_get((void *)&cfg, (void *)&attr))
    {
        OMCIMNGERR("get real time pm error ! ");
        return;
    }

    OMCI_UPDATE_PM(fcsErr);
    OMCI_UPDATE_PM(excessCollCnt);
    OMCI_UPDATE_PM(lateCollCnt);
    OMCI_UPDATE_PM(frameTooLong);
    OMCI_UPDATE_PM(rxBuffOverflow);
    OMCI_UPDATE_PM(txBuffOverflow);
    OMCI_UPDATE_PM(singleCollFrameCnt);
    OMCI_UPDATE_PM(multiCollFrameCnt);
    OMCI_UPDATE_PM(sqeCnt);
    OMCI_UPDATE_PM(deferredTxCnt);
    OMCI_UPDATE_PM(internalMacTxErrCnt);
    OMCI_UPDATE_PM(carrierSenseErrCnt);
    OMCI_UPDATE_PM(alignErrCnt);
}


void ca_omci_me_eth_pm_his_timeout(void *data)
{
    ca_omci_eth_pm_his_t *me = (ca_omci_eth_pm_his_t *)data;
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

