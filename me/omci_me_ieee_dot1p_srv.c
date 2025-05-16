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
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_veip.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_ieee_dot1p_srv_handlers = {0};


ca_omci_me_attr_t g_omci_me_ieee_dot1p_srv_attr[OMCI_ME_ATTR13] =
{
    {OMCI_ME_ATTR1,  "tp_pointer"},
    {OMCI_ME_ATTR2,  "iwtp_pri0"},
    {OMCI_ME_ATTR3,  "iwtp_pri1"},
    {OMCI_ME_ATTR4,  "iwtp_pri2"},
    {OMCI_ME_ATTR5,  "iwtp_pri3"},
    {OMCI_ME_ATTR6,  "iwtp_pri4"},
    {OMCI_ME_ATTR7,  "iwtp_pri5"},
    {OMCI_ME_ATTR8,  "iwtp_pri6"},
    {OMCI_ME_ATTR9,  "iwtp_pri7"},
    {OMCI_ME_ATTR10, "unmark_frame_op"},
    {OMCI_ME_ATTR11, "dscp_to_pbit"},
    {OMCI_ME_ATTR12, "def_pbit_assu"},
    {OMCI_ME_ATTR13, "tp_type"},
};


ca_omci_uint32_t ca_omci_me_ieee_dot1p_srv_attr_len_get(void)
{
    ca_omci_ieee_dot1p_srv_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.tpPointer);
    attrLen += sizeof(me->attr.iwtpPri0);
    attrLen += sizeof(me->attr.iwtpPri1);
    attrLen += sizeof(me->attr.iwtpPri2);
    attrLen += sizeof(me->attr.iwtpPri3);
    attrLen += sizeof(me->attr.iwtpPri4);
    attrLen += sizeof(me->attr.iwtpPri5);
    attrLen += sizeof(me->attr.iwtpPri6);
    attrLen += sizeof(me->attr.iwtpPri7);
    attrLen += sizeof(me->attr.unmarkFrameOp);
    attrLen += sizeof(me->attr.dscp2Pbit);
    attrLen += sizeof(me->attr.defPbitAssu);
    attrLen += sizeof(me->attr.tpType);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ieee_dot1p_srv_new(void)
{
    ca_omci_ieee_dot1p_srv_t *me = (ca_omci_ieee_dot1p_srv_t *)omci_malloc(sizeof(ca_omci_ieee_dot1p_srv_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, &sg_omci_me_ieee_dot1p_srv_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ieee_dot1p_srv);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_ieee_dot1p_srv_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_ieee_dot1p_srv_t *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.tpPointer, "tpPointer", 1);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri0, "iwtpPri0", 2);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri1, "iwtpPri1", 3);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri2, "iwtpPri2", 4);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri3, "iwtpPri3", 5);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri4, "iwtpPri4", 6);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri5, "iwtpPri5", 7);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri6, "iwtpPri6", 8);
    OMCI_SET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri7, "iwtpPri7", 9);
    OMCI_SET_ATTR_UINT8(ieeeDot1pSrv->attr.unmarkFrameOp, "unmarkFrameOp", 10);
    OMCI_SET_ATTR_UINT8(ieeeDot1pSrv->attr.defPbitAssu, "defPbitAssu", 12);
    OMCI_SET_ATTR_UINT8(ieeeDot1pSrv->attr.tpType, "tpType", 13);

    if ((IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == ieeeDot1pSrv->attr.tpType)
     && (OMCI_ME_NULL_POINTER != ieeeDot1pSrv->attr.tpPointer))
    {
        OMCIDBG("tp point is error when tp type is bridge");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieeeDot1pSrv->attr.tpType)
     && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieeeDot1pSrv->attr.tpType)
     && (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE != ieeeDot1pSrv->attr.tpType))
    {
        OMCIWARN("Unsupport tpType %d Need to add", ieeeDot1pSrv->attr.tpType);
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ieee_dot1p_srv_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ieee_dot1p_srv_t      *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_ieee_dot1p_srv_attr_t  newVal;
    ca_omci_uint16_t  offset   = 0;
    ca_omci_uint16_t  mask     = 0;
    ca_omci_uint8_t   idx      = 0;
    ca_omci_mebase_t *gemIwTp0 = NULL;
    ca_omci_mebase_t *gemIwTp1 = NULL;
    ca_omci_mebase_t *gemIwTp2 = NULL;
    ca_omci_mebase_t *gemIwTp3 = NULL;
    ca_omci_mebase_t *gemIwTp4 = NULL;
    ca_omci_mebase_t *gemIwTp5 = NULL;
    ca_omci_mebase_t *gemIwTp6 = NULL;
    ca_omci_mebase_t *gemIwTp7 = NULL;
    ca_omci_mebase_t *pptpEth  = NULL;
    ca_omci_mebase_t *veip     = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_ieee_dot1p_srv_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_IEEE_DOT1P_SRV_SET_MASK_FULL);

    //remove old link
    if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieeeDot1pSrv->attr.tpType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieeeDot1pSrv->attr.tpPointer);
        OMCI_RM_LINK_ME(ieeeDot1pSrv, pptpEth);
    }

    if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieeeDot1pSrv->attr.tpType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieeeDot1pSrv->attr.tpPointer);
        OMCI_RM_LINK_ME(ieeeDot1pSrv, veip);
    }

    gemIwTp0 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri0);
    gemIwTp1 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri1);
    gemIwTp2 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri2);
    gemIwTp3 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri3);
    gemIwTp4 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri4);
    gemIwTp5 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri5);
    gemIwTp6 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri6);
    gemIwTp7 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri7);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp0);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp1);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp2);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp3);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp4);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp5);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp6);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp7);

    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.tpPointer, "tpPointer", 1);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri0, "iwtpPri0", 2);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri1, "iwtpPri1", 3);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri2, "iwtpPri2", 4);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri3, "iwtpPri3", 5);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri4, "iwtpPri4", 6);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri5, "iwtpPri5", 7);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri6, "iwtpPri6", 8);
    OMCI_UPDATE_ATTR_UINT(16, ieeeDot1pSrv->attr.iwtpPri7, "iwtpPri7", 9);
    OMCI_UPDATE_ATTR_UINT(8, ieeeDot1pSrv->attr.unmarkFrameOp, "unmarkFrameOp", 10);
    OMCI_UPDATE_ATTR_UINT_T(8, ieeeDot1pSrv->attr.dscp2Pbit, "dscp2Pbit", OMCI_DSCP_TO_PBIT_LEN, 11);
    OMCI_UPDATE_ATTR_UINT(8, ieeeDot1pSrv->attr.defPbitAssu, "defPbitAssu", 12);
    OMCI_UPDATE_ATTR_UINT(8, newVal.tpType, "tpType", 13);

    if ((OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR13, mask))
     && (newVal.tpType != ieeeDot1pSrv->attr.tpType))
    {
        ieeeDot1pSrv->attr.tpType = newVal.tpType;
        if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieeeDot1pSrv->attr.tpType)
         && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieeeDot1pSrv->attr.tpType))
        {
            OMCIWARN("Unsupport tpType %d Need to add", ieeeDot1pSrv->attr.tpType);
        }

    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ieee_dot1p_srv_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ieee_dot1p_srv_t *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_IEEE_DOT1P_SRV_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.tpPointer, 1);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri0, 2);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri1, 3);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri2, 4);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri3, 5);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri4, 6);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri5, 7);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri6, 8);
    OMCI_GET_ATTR_UINT16(ieeeDot1pSrv->attr.iwtpPri7, 9);
    OMCI_GET_ATTR_UINT8(ieeeDot1pSrv->attr.unmarkFrameOp, 10);
    OMCI_GET_ATTR_UINT8_T(ieeeDot1pSrv->attr.dscp2Pbit, OMCI_DSCP_TO_PBIT_LEN, 11);
    OMCI_GET_ATTR_UINT8(ieeeDot1pSrv->attr.defPbitAssu, 12);
    OMCI_GET_ATTR_UINT8(ieeeDot1pSrv->attr.tpType, 13);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ieee_dot1p_srv_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_ieee_dot1p_srv_t *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_mebase_t         *gemIwTp0     = NULL;
    ca_omci_mebase_t         *gemIwTp1     = NULL;
    ca_omci_mebase_t         *gemIwTp2     = NULL;
    ca_omci_mebase_t         *gemIwTp3     = NULL;
    ca_omci_mebase_t         *gemIwTp4     = NULL;
    ca_omci_mebase_t         *gemIwTp5     = NULL;
    ca_omci_mebase_t         *gemIwTp6     = NULL;
    ca_omci_mebase_t         *gemIwTp7     = NULL;
    ca_omci_mebase_t         *pptpEth      = NULL;
    ca_omci_mebase_t         *veip         = NULL;


    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieeeDot1pSrv->attr.tpType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieeeDot1pSrv->attr.tpPointer);
        OMCI_RM_LINK_ME(ieeeDot1pSrv, pptpEth);
    }

    if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieeeDot1pSrv->attr.tpType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieeeDot1pSrv->attr.tpPointer);
        OMCI_RM_LINK_ME(ieeeDot1pSrv, veip);
    }

    gemIwTp0 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri0);
    gemIwTp1 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri1);
    gemIwTp2 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri2);
    gemIwTp3 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri3);
    gemIwTp4 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri4);
    gemIwTp5 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri5);
    gemIwTp6 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri6);
    gemIwTp7 = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieeeDot1pSrv->attr.iwtpPri7);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp0);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp1);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp2);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp3);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp4);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp5);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp6);
    OMCI_RM_LINK_ME(ieeeDot1pSrv, gemIwTp7);

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ieee_dot1p_srv_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ieee_dot1p_srv_t *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(ieeeDot1pSrv);
    if (CA_OMCI_TRUE == ext)
    {
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.tpPointer, 1);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri0, 2);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri1, 3);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri2, 4);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri3, 5);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri4, 6);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri5, 7);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri6, 8);
        OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri7, 9);
        OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.unmarkFrameOp, 10);
        OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.dscp2Pbit, 11);
        OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.defPbitAssu, 12);
        OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.tpType, 13);
        OMCI_UPLOAD_MASK_ENCODE;
    }
    else
    {
        if (0 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.tpPointer, 1);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri0, 2);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri1, 3);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri2, 4);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri3, 5);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri4, 6);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri5, 7);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri6, 8);
            OMCI_UPLOAD_ATTR_SIZE(16, ieeeDot1pSrv->attr.iwtpPri7, 9);
            OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.unmarkFrameOp, 10);
            OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.defPbitAssu, 12);
            OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.tpType, 13);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        if (1 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, ieeeDot1pSrv->attr.dscp2Pbit, 11);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
        }
    }
    return CA_OMCI_OK;
}


void ca_omci_me_ieee_dot1p_srv_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_ieee_dot1p_srv_t *ieeeDot1pSrv = (ca_omci_ieee_dot1p_srv_t *)me;
    ca_omci_uint32_t idx = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: ieeeDot1pSrv \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        tpPointer:   0x%x\n", ieeeDot1pSrv->attr.tpPointer);
    omci_sprint(cli, "        iwtpPri0:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri0);
    omci_sprint(cli, "        iwtpPri1:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri1);
    omci_sprint(cli, "        iwtpPri2:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri2);
    omci_sprint(cli, "        iwtpPri3:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri3);
    omci_sprint(cli, "        iwtpPri4:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri4);
    omci_sprint(cli, "        iwtpPri5:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri5);
    omci_sprint(cli, "        iwtpPri6:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri6);
    omci_sprint(cli, "        iwtpPri7:    0x%x\n", ieeeDot1pSrv->attr.iwtpPri7);
    omci_sprint(cli, "        unmarkFrameOp: 0x%x\n", ieeeDot1pSrv->attr.unmarkFrameOp);
    for (idx = 0; idx < OMCI_DSCP_TO_PBIT_LEN; idx++)
    {
        omci_sprint(cli, "        dscp2Pbit[%d]: 0x%x \n", idx, ieeeDot1pSrv->attr.dscp2Pbit[idx]);
    }
    omci_sprint(cli, "        defPbitAssu: 0x%x\n", ieeeDot1pSrv->attr.defPbitAssu);
    omci_sprint(cli, "        tpType:      0x%x\n", ieeeDot1pSrv->attr.tpType);
}


void ca_omci_me_ieee_dot1p_srv_init(void)
{
    sg_omci_me_ieee_dot1p_srv_handlers.meDisplay  = ca_omci_me_ieee_dot1p_srv_display;
    sg_omci_me_ieee_dot1p_srv_handlers.uploadProc = ca_omci_me_ieee_dot1p_srv_upload;

    sg_omci_me_ieee_dot1p_srv_handlers.createProc = ca_omci_me_ieee_dot1p_srv_create;
    sg_omci_me_ieee_dot1p_srv_handlers.deleteProc = ca_omci_me_ieee_dot1p_srv_delete;
    sg_omci_me_ieee_dot1p_srv_handlers.setProc    = ca_omci_me_ieee_dot1p_srv_set;
    sg_omci_me_ieee_dot1p_srv_handlers.getProc    = ca_omci_me_ieee_dot1p_srv_get;
}


void ca_omci_me_ieee_dot1p_srv_link(void)
{
    ca_omci_meclass_t          *meClass    = NULL;
    ca_omci_mebase_t           *meInst     = NULL;
    ca_omci_ieee_dot1p_srv_t   *me         = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp0   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp1   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp2   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp3   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp4   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp5   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp6   = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp7   = NULL;
    ca_omci_pptp_eth_t         *pptpEth    = NULL;
    ca_omci_veip_t             *veip       = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no dot1p is created now");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me = (ca_omci_ieee_dot1p_srv_t *)meInst;

        if ((IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == me->attr.tpType)
         && (OMCI_ME_NULL_POINTER != me->attr.tpPointer))
        {
            OMCIDBG("tp point is error when tp type is bridge");
            continue;
        }

        if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != me->attr.tpType)
         && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != me->attr.tpType)
         && (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE != me->attr.tpType))
        {
            OMCIWARN("Unsupport tpType %d Need to add", me->attr.tpType);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri0)
        {
            gemIwTp0 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri0);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp0);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri1)
        {
            gemIwTp1 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri1);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp1);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri2)
        {
            gemIwTp2 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri2);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp2);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri3)
        {
            gemIwTp3 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri3);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp3);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri4)
        {
            gemIwTp4 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri4);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp4);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri5)
        {
            gemIwTp5 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri5);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp5);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri6)
        {
            gemIwTp6 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri6);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp6);
        }

        if (OMCI_ME_NULL_POINTER != me->attr.iwtpPri7)
        {
            gemIwTp7 = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.iwtpPri7);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp7);
        }

        if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == me->attr.tpType)
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
        }

        if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == me->attr.tpType)
        {
            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == veip);
        }

        OMCI_ME_LINK(me, gemIwTp0);
        OMCI_ME_LINK(me, gemIwTp1);
        OMCI_ME_LINK(me, gemIwTp2);
        OMCI_ME_LINK(me, gemIwTp3);
        OMCI_ME_LINK(me, gemIwTp4);
        OMCI_ME_LINK(me, gemIwTp5);
        OMCI_ME_LINK(me, gemIwTp6);
        OMCI_ME_LINK(me, gemIwTp7);
        OMCI_ME_LINK(me, pptpEth);
        OMCI_ME_LINK(me, veip);
    }
}

