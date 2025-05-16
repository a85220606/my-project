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
#include "omci_me_ext_vlan_tag_op.h"
#include "omci_me_mac_port_cfg.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_ip_host.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_veip.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_mng_onuinfo.h"
#include "omci_hal.h"


#define OMCI_EXT_VLAN_REMOVE_RULE  0xffffffff


static ca_omci_me_handler_t sg_omci_me_ext_vlan_tag_op_handlers = {0};


ca_omci_me_attr_t g_omci_me_ext_vlan_tag_op_attr[OMCI_ME_ATTR8] =
{
    {OMCI_ME_ATTR1, "assoc_type"},
    {OMCI_ME_ATTR2, "rx_tbl_max"},
    {OMCI_ME_ATTR3, "input_tpid"},
    {OMCI_ME_ATTR4, "output_tpid"},
    {OMCI_ME_ATTR5, "ds_mode"},
    {OMCI_ME_ATTR6, "rx_vlan_tbl"},
    {OMCI_ME_ATTR7, "assoc_point"},
    {OMCI_ME_ATTR8, "dscp_to_pbit"},
};


ca_omci_uint32_t ca_omci_me_ext_vlan_tag_op_attr_len_get(void)
{
    ca_omci_ext_vlan_tag_op_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.assocType);
    attrLen += sizeof(me->attr.rxTableMaxSize);
    attrLen += sizeof(me->attr.inputTpid);
    attrLen += sizeof(me->attr.outputTpid);
    attrLen += sizeof(me->attr.dsMode);
    attrLen += sizeof(me->attr.assocPoint);
    attrLen += sizeof(me->attr.dscp2Pbit);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ext_vlan_tag_op_new(void)
{
    ca_omci_ext_vlan_tag_op_t *me = (ca_omci_ext_vlan_tag_op_t *)omci_malloc(sizeof(ca_omci_ext_vlan_tag_op_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID, &sg_omci_me_ext_vlan_tag_op_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ext_vlan_tag_op);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(externVlanTagOp->attr.assocType, "assocType", 1);
    OMCI_SET_ATTR_UINT16(externVlanTagOp->attr.assocPoint, "assocPoint", 7);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ext_vlan_tag_op_t      *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_ext_vlan_tag_op_attr_t  newVal;
    ca_omci_uint16_t  offset     = 0;
    ca_omci_uint16_t  mask       = 0;
    ca_omci_uint8_t   idx        = 0;
    ca_omci_mebase_t *macPortCfg = NULL;
    ca_omci_mebase_t *dot1p      = NULL;
    ca_omci_mebase_t *pptpEth    = NULL;
    ca_omci_mebase_t *iphost     = NULL;
    ca_omci_mebase_t *gemIwTp    = NULL;
    ca_omci_mebase_t *multiGem   = NULL;
    ca_omci_mebase_t *veip       = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_ext_vlan_tag_op_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_EXT_VLAN_TAG_OP_SET_MASK_FULL);

    //remove old link
    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MAC_PORT == externVlanTagOp->attr.assocType)
    {
        macPortCfg = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, macPortCfg);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_8021P == externVlanTagOp->attr.assocType)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, dot1p);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_PPTP_ETH == externVlanTagOp->attr.assocType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, pptpEth);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_IPHOST == externVlanTagOp->attr.assocType)
    {
        iphost = ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, iphost);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_GEM_IWTP == externVlanTagOp->attr.assocType)
    {
        gemIwTp = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, gemIwTp);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MULTI_GEM == externVlanTagOp->attr.assocType)
    {
        multiGem = ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, multiGem);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_VEIP == externVlanTagOp->attr.assocType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, veip);
    }

    OMCI_UPDATE_ATTR_UINT(8, externVlanTagOp->attr.assocType, "assocType", 1);
    OMCI_UPDATE_ATTR_UINT(16, externVlanTagOp->attr.inputTpid, "inputTpid", 3);
    OMCI_UPDATE_ATTR_UINT(16, externVlanTagOp->attr.outputTpid, "outputTpid", 4);
    OMCI_UPDATE_ATTR_UINT(8, externVlanTagOp->attr.dsMode, "dsMode", 5);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        OMCI_SET_TBL_UINT32(newVal.rxVlanTbl[0].filterOutter, "filterOutter");
        OMCI_SET_TBL_UINT32(newVal.rxVlanTbl[0].filterInner, "filterInner");
        OMCI_SET_TBL_UINT32(newVal.rxVlanTbl[0].treatOutter, "treatOutter");
        OMCI_SET_TBL_UINT32(newVal.rxVlanTbl[0].treatInner, "treatInner");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
    }
    OMCI_UPDATE_ATTR_UINT(16, externVlanTagOp->attr.assocPoint, "assocPoint", 7);
    OMCI_UPDATE_ATTR_UINT_T(8, externVlanTagOp->attr.dscp2Pbit, "dscp2Pbit", OMCI_DSCP_TO_PBIT_LEN, 8);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, inMask))
    {
        ca_omci_hal_dscp_map_set(externVlanTagOp->attr.dscp2Pbit);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        //go though this rule list to get a used rule
        for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
        {
            if ((externVlanTagOp->attr.rxVlanTbl[idx].filterOutter == newVal.rxVlanTbl[0].filterOutter)
             && (externVlanTagOp->attr.rxVlanTbl[idx].filterInner == newVal.rxVlanTbl[0].filterInner)
             && (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[idx].valid))
            {
                OMCIMNGDBG("find rule[%d] for filterInner[0x%x] filterOutter[0x%x]", idx,
                            externVlanTagOp->attr.rxVlanTbl[idx].filterOutter, externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                break;
            }
        }

        //found rule match
        if (OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL != idx)
        {
            if ((OMCI_EXT_VLAN_REMOVE_RULE == newVal.rxVlanTbl[0].treatOutter)
             && (OMCI_EXT_VLAN_REMOVE_RULE == newVal.rxVlanTbl[0].treatInner))
            {
                //remove this rule;
                externVlanTagOp->attr.rxVlanTbl[idx].filterOutter = 0;
                externVlanTagOp->attr.rxVlanTbl[idx].filterInner  = 0;
                externVlanTagOp->attr.rxVlanTbl[idx].treatOutter  = 0;
                externVlanTagOp->attr.rxVlanTbl[idx].treatInner   = 0;
                externVlanTagOp->attr.rxVlanTbl[idx].valid        = CA_OMCI_FALSE;
                OMCIMNGDBG("remove extern vlan rule");
                externVlanTagOp->meBase.mibUploadCnt--;
            }
            else
            {
                OMCIMNGDBG("Update rule[%d] for filterInner[0x%x] filterOutter[0x%x]", idx,
                            externVlanTagOp->attr.rxVlanTbl[idx].filterOutter, externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                externVlanTagOp->attr.rxVlanTbl[idx].treatOutter  = newVal.rxVlanTbl[0].treatOutter;
                externVlanTagOp->attr.rxVlanTbl[idx].treatInner   = newVal.rxVlanTbl[0].treatInner;
            }
        }
        else
        {
            //no match rule found
            for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
            {
                if (CA_OMCI_FALSE == externVlanTagOp->attr.rxVlanTbl[idx].valid)
                {
                    break;
                }
            }

            if (OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL == idx)
            {
                OMCIMNGERR("No resource for rule to store");
            }
            else
            {
                if ((OMCI_EXT_VLAN_REMOVE_RULE == newVal.rxVlanTbl[0].treatOutter)
                 && (OMCI_EXT_VLAN_REMOVE_RULE == newVal.rxVlanTbl[0].treatInner))
                {
                    OMCIMNGDBG("No need to save a delete rule for filterInner[0x%x] filterOutter[0x%x]",
                                externVlanTagOp->attr.rxVlanTbl[idx].filterOutter, externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                    return CA_OMCI_OK;
                }
                externVlanTagOp->attr.rxVlanTbl[idx].filterOutter = newVal.rxVlanTbl[0].filterOutter;
                externVlanTagOp->attr.rxVlanTbl[idx].filterInner  = newVal.rxVlanTbl[0].filterInner;
                externVlanTagOp->attr.rxVlanTbl[idx].treatOutter  = newVal.rxVlanTbl[0].treatOutter;
                externVlanTagOp->attr.rxVlanTbl[idx].treatInner   = newVal.rxVlanTbl[0].treatInner;
                externVlanTagOp->attr.rxVlanTbl[idx].valid        = CA_OMCI_TRUE;
                OMCIMNGDBG("add rule[%d] for filterInner[0x%x] filterOutter[0x%x]", idx,
                            externVlanTagOp->attr.rxVlanTbl[idx].filterOutter, externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                externVlanTagOp->meBase.mibUploadCnt++;
            }
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_EXT_VLAN_TAG_OP_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT8(externVlanTagOp->attr.assocType, 1);
    OMCI_GET_ATTR_UINT16(externVlanTagOp->attr.rxTableMaxSize, 2);
    OMCI_GET_ATTR_UINT16(externVlanTagOp->attr.inputTpid, 3);
    OMCI_GET_ATTR_UINT16(externVlanTagOp->attr.outputTpid, 4);
    OMCI_GET_ATTR_UINT8(externVlanTagOp->attr.dsMode, 5);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(externVlanTagOp->attr.rxVlanTbl[0]) - sizeof(externVlanTagOp->attr.rxVlanTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblSeq = (tblSize + OMCI_MSG_GET_NEXT_LEN - 1) / OMCI_MSG_GET_NEXT_LEN;
        if (CA_OMCI_TRUE == ext)
        {
            tblSeq = (tblSize + OMCI_MSG_EXT_GET_NEXT_LEN - 1) / OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        tblSize = ca_omci_htonl(tblSize);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&externVlanTagOp->rxVlanTbl, tblSeq);
    }
    OMCI_GET_ATTR_UINT16(externVlanTagOp->attr.assocPoint, 7);
    OMCI_GET_ATTR_UINT8_T(externVlanTagOp->attr.dscp2Pbit, OMCI_DSCP_TO_PBIT_LEN, 8);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(externVlanTagOp->attr.rxVlanTbl[tblIdx]) - sizeof(externVlanTagOp->attr.rxVlanTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&externVlanTagOp->rxVlanTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        if (NULL == tblAttr)
        {
            result->result = OMCI_MSG_RST_PROC_ERR;
            return CA_OMCI_E_PARAM;
        }
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[tblIdx].filterOutter);
                OMCI_CP_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[tblIdx].filterInner);
                OMCI_CP_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[tblIdx].treatOutter);
                OMCI_CP_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[tblIdx].treatInner);
            }
        }

        offset = seqNum * OMCI_MSG_GET_NEXT_LEN;
        sglLen = OMCI_MSG_GET_NEXT_LEN;
        if (ext)
        {
            offset = seqNum * OMCI_MSG_EXT_GET_NEXT_LEN;
            sglLen = OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        attLen = (tblSize - offset) > sglLen ? sglLen : (tblSize - offset);
        OMCI_MEMCPY(attr, (tblAttr + offset), attLen);
        OMCI_FREE(tblAttr);
        result->attrLen = attLen;
        result->result  = OMCI_MSG_RST_SUCCESSFUL;
        ca_omci_next_success(&externVlanTagOp->rxVlanTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_mebase_t          *macPortCfg      = NULL;
    ca_omci_mebase_t          *dot1p           = NULL;
    ca_omci_mebase_t          *pptpEth         = NULL;
    ca_omci_mebase_t          *iphost          = NULL;
    ca_omci_mebase_t          *gemIwTp         = NULL;
    ca_omci_mebase_t          *multiGem        = NULL;
    ca_omci_mebase_t          *veip            = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MAC_PORT == externVlanTagOp->attr.assocType)
    {
        macPortCfg = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, macPortCfg);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_8021P == externVlanTagOp->attr.assocType)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, dot1p);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_PPTP_ETH == externVlanTagOp->attr.assocType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, pptpEth);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_IPHOST == externVlanTagOp->attr.assocType)
    {
        iphost = ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, iphost);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_GEM_IWTP == externVlanTagOp->attr.assocType)
    {
        gemIwTp = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, gemIwTp);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MULTI_GEM == externVlanTagOp->attr.assocType)
    {
        multiGem = ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, multiGem);
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_VEIP == externVlanTagOp->attr.assocType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, externVlanTagOp->attr.assocPoint);
        OMCI_RM_LINK_ME(externVlanTagOp, veip);
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ext_vlan_tag_op_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;
    ca_omci_uint16_t num     = 0;

    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(externVlanTagOp);
    if (CA_OMCI_TRUE == ext)
    {
        OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.assocType, 1);
        OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.rxTableMaxSize, 2);
        OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.inputTpid, 3);
        OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.outputTpid, 4);
        OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.dsMode, 5);
        for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
        {
            if (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].filterOutter);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].treatOutter);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].treatInner);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
            }
        }
        OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.assocPoint, 7);
        OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.dscp2Pbit, 8);
        OMCI_UPLOAD_MASK_ENCODE;
    }
    else
    {
        if (0 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.assocType, 1);
            OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.rxTableMaxSize, 2);
            OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.inputTpid, 3);
            OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.outputTpid, 4);
            OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.dsMode, 5);
            OMCI_UPLOAD_ATTR_SIZE(16, externVlanTagOp->attr.assocPoint, 7);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        if (1 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, externVlanTagOp->attr.dscp2Pbit, 8);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        //from seqNum 2, upload rx_vlan_tbl
        for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
        {
            if (CA_OMCI_TRUE == externVlanTagOp->attr.rxVlanTbl[idx].valid)
            {
                num++;
            }
            if (num == seqNum - 1)
            {
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].filterOutter);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].treatOutter);
                OMCI_UPLOAD_TBL_UINT32(externVlanTagOp->attr.rxVlanTbl[idx].treatInner);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
                OMCI_BASIC_UPLOAD_MASK_ENCODE;
                return CA_OMCI_OK;
            }
        }

        return CA_OMCI_OK;
    }
    return CA_OMCI_OK;
}


void ca_omci_me_ext_vlan_tag_op_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_ext_vlan_tag_op_t *externVlanTagOp = (ca_omci_ext_vlan_tag_op_t *)me;
    ca_omci_uint32_t idx = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: externVlanTagOp \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        assocType:   0x%x\n", externVlanTagOp->attr.assocType);
    omci_sprint(cli, "        rxTableMaxSize: 0x%x\n", externVlanTagOp->attr.rxTableMaxSize);
    omci_sprint(cli, "        inputTpid:   0x%x\n", externVlanTagOp->attr.inputTpid);
    omci_sprint(cli, "        outputTpid:  0x%x\n", externVlanTagOp->attr.outputTpid);
    omci_sprint(cli, "        dsMode:      0x%x\n", externVlanTagOp->attr.dsMode);
    for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
    {
        if (CA_OMCI_FALSE == externVlanTagOp->attr.rxVlanTbl[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        rxVlanTbl[%d]:filterOutter 0x%x\n", idx, externVlanTagOp->attr.rxVlanTbl[idx].filterOutter);
        omci_sprint(cli, "        rxVlanTbl[%d]:filterInner 0x%x\n", idx, externVlanTagOp->attr.rxVlanTbl[idx].filterInner);
        omci_sprint(cli, "        rxVlanTbl[%d]:treatOutter 0x%x\n", idx, externVlanTagOp->attr.rxVlanTbl[idx].treatOutter);
        omci_sprint(cli, "        rxVlanTbl[%d]:treatInner 0x%x\n", idx, externVlanTagOp->attr.rxVlanTbl[idx].treatInner);
    }
    omci_sprint(cli, "        assocPoint:  0x%x\n", externVlanTagOp->attr.assocPoint);
    for (idx = 0; idx < OMCI_DSCP_TO_PBIT_LEN; idx++)
    {
        omci_sprint(cli, "        dscp2Pbit[%d]: 0x%x \n", idx, externVlanTagOp->attr.dscp2Pbit[idx]);
    }
}


void ca_omci_me_ext_vlan_tag_op_init(void)
{
    sg_omci_me_ext_vlan_tag_op_handlers.meDisplay  = ca_omci_me_ext_vlan_tag_op_display;
    sg_omci_me_ext_vlan_tag_op_handlers.uploadProc = ca_omci_me_ext_vlan_tag_op_upload;

    sg_omci_me_ext_vlan_tag_op_handlers.createProc  = ca_omci_me_ext_vlan_tag_op_create;
    sg_omci_me_ext_vlan_tag_op_handlers.deleteProc  = ca_omci_me_ext_vlan_tag_op_delete;
    sg_omci_me_ext_vlan_tag_op_handlers.setProc     = ca_omci_me_ext_vlan_tag_op_set;
    sg_omci_me_ext_vlan_tag_op_handlers.getProc     = ca_omci_me_ext_vlan_tag_op_get;
    sg_omci_me_ext_vlan_tag_op_handlers.getNextProc = ca_omci_me_ext_vlan_tag_op_get_next;
}


void ca_omci_me_ext_vlan_tag_op_link(void)
{
    ca_omci_meclass_t         *meClass    = NULL;
    ca_omci_mebase_t          *meInst     = NULL;
    ca_omci_ext_vlan_tag_op_t *me         = NULL;
    ca_omci_mac_port_cfg_t    *macPortCfg = NULL;
    ca_omci_ieee_dot1p_srv_t  *dot1p      = NULL;
    ca_omci_pptp_eth_t        *pptpEth    = NULL;
    ca_omci_ip_host_t         *iphost     = NULL;
    ca_omci_gem_iwtp_t        *gemIwTp    = NULL;
    ca_omci_multi_gem_iwtp_t  *multiGem   = NULL;
    ca_omci_veip_t            *veip       = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no ext_vlan_tag_op is created now");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me = (ca_omci_ext_vlan_tag_op_t *)meInst;
        if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MAC_PORT == me->attr.assocType)
        {
            macPortCfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, macPortCfg);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_8021P == me->attr.assocType)
        {
            dot1p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, dot1p);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_PPTP_ETH == me->attr.assocType)
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, pptpEth);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_IPHOST == me->attr.assocType)
        {
            iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, iphost);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_GEM_IWTP == me->attr.assocType)
        {
            gemIwTp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, gemIwTp);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MULTI_GEM == me->attr.assocType)
        {
            multiGem = (ca_omci_multi_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, multiGem);
        }
        else if (EXT_VLAN_TAG_OP_ASSOC_TYPE_VEIP == me->attr.assocType)
        {
            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, me->attr.assocPoint);
            OMCI_ME_LINK(me, veip);
        }
        else
        {
            OMCIMNGWARN("Unsupport assocType %d", me->attr.assocType);
        }
    }
}

