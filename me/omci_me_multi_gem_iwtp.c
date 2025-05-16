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
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_multi_gem_iwtp_handlers = {0};


ca_omci_me_attr_t g_omci_me_multi_gem_iwtp_attr[OMCI_ME_ATTR10] =
{
    {OMCI_ME_ATTR1,  "gem_net_ctp_point"},
    {OMCI_ME_ATTR2,  "iw_option"},
    {OMCI_ME_ATTR3,  "srv_prof_point"},
    {OMCI_ME_ATTR4,  "not_used1"},
    {OMCI_ME_ATTR5,  "pptp_counter"},
    {OMCI_ME_ATTR6,  "oper_state"},
    {OMCI_ME_ATTR7,  "gal_prof_point"},
    {OMCI_ME_ATTR8,  "not_used2"},
    {OMCI_ME_ATTR9,  "ipv4_multi_addr_tbl"},
    {OMCI_ME_ATTR10, "ipv6_multi_addr_tbl"},
};


ca_omci_uint32_t ca_omci_me_multi_gem_iwtp_attr_len_get(void)
{
    ca_omci_multi_gem_iwtp_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.gemNetCtpPoint);
    attrLen += sizeof(me->attr.iwOption);
    attrLen += sizeof(me->attr.srvProfPoint);
    attrLen += sizeof(me->attr.notUsed1);
    attrLen += sizeof(me->attr.pptpCounter);
    attrLen += sizeof(me->attr.operState);
    attrLen += sizeof(me->attr.galProfPoint);
    attrLen += sizeof(me->attr.notUsed2);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_multi_gem_iwtp_new(void)
{
    ca_omci_multi_gem_iwtp_t *me = (ca_omci_multi_gem_iwtp_t *)omci_malloc(sizeof(ca_omci_multi_gem_iwtp_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, &sg_omci_me_multi_gem_iwtp_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(multi_gem_iwtp);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(multiGemIwtp->attr.gemNetCtpPoint, "gemNetCtpPoint", 1);
    OMCI_SET_ATTR_UINT8(multiGemIwtp->attr.iwOption, "iwOption", 2);
    OMCI_SET_ATTR_UINT16(multiGemIwtp->attr.srvProfPoint, "srvProfPoint", 3);
    OMCI_SET_ATTR_UINT16(multiGemIwtp->attr.notUsed1, "notUsed1", 4);
    OMCI_SET_ATTR_UINT16(multiGemIwtp->attr.galProfPoint, "galProfPoint", 7);
    OMCI_SET_ATTR_UINT8(multiGemIwtp->attr.notUsed2, "notUsed2", 8);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_multi_gem_iwtp_t      *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_multi_gem_iwtp_attr_t  newVal;
    ca_omci_uint16_t  offset    = 0;
    ca_omci_uint16_t  mask      = inMask;
    ca_omci_uint8_t   idx       = 0;
    ca_omci_mebase_t *gemNetCtp = NULL;
    ca_omci_mebase_t *dot1p     = NULL;
    ca_omci_mebase_t *macBrgSrv = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_multi_gem_iwtp_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_GEM_IWTP_SET_MASK_FULL);

    //remove old link
    gemNetCtp = ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, multiGemIwtp->attr.gemNetCtpPoint);
    OMCI_RM_LINK_ME(multiGemIwtp, gemNetCtp);

    if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGemIwtp->attr.iwOption)
    {
        macBrgSrv = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, multiGemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(multiGemIwtp, macBrgSrv);
    }

    if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGemIwtp->attr.iwOption)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, multiGemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(multiGemIwtp, dot1p);
    }

    OMCI_UPDATE_ATTR_UINT(16, multiGemIwtp->attr.gemNetCtpPoint, "gemNetCtpPoint", 1);
    OMCI_UPDATE_ATTR_UINT(8, multiGemIwtp->attr.iwOption, "iwOption", 2);
    OMCI_UPDATE_ATTR_UINT(16, multiGemIwtp->attr.srvProfPoint, "srvProfPoint", 3);
    OMCI_UPDATE_ATTR_UINT(16, multiGemIwtp->attr.notUsed1, "notUsed1", 4);
    OMCI_UPDATE_ATTR_UINT(16, multiGemIwtp->attr.galProfPoint, "galProfPoint", 7);
    OMCI_UPDATE_ATTR_UINT(8, multiGemIwtp->attr.notUsed2, "notUsed2", 8);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, mask))
    {
        OMCI_SET_TBL_UINT16(newVal.ipv4MultiAddrTbl[0].gemId, "gemId");
        OMCI_SET_TBL_UINT16(newVal.ipv4MultiAddrTbl[0].sndKey, "sndKey");
        OMCI_SET_TBL_UINT32(newVal.ipv4MultiAddrTbl[0].ipAddrStart, "ipAddrStart");
        OMCI_SET_TBL_UINT32(newVal.ipv4MultiAddrTbl[0].ipAddrEnd, "ipAddrEnd");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);

        for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; idx++)
        {
            if (CA_OMCI_FALSE == multiGemIwtp->attr.ipv4MultiAddrTbl[idx].valid)
            {
                multiGemIwtp->attr.ipv4MultiAddrTbl[idx].gemId       = newVal.ipv4MultiAddrTbl[0].gemId;
                multiGemIwtp->attr.ipv4MultiAddrTbl[idx].sndKey      = newVal.ipv4MultiAddrTbl[0].sndKey;
                multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrStart = newVal.ipv4MultiAddrTbl[0].ipAddrStart;
                multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrEnd   = newVal.ipv4MultiAddrTbl[0].ipAddrEnd;
                multiGemIwtp->attr.ipv4MultiAddrTbl[idx].valid       = CA_OMCI_TRUE;
                break;
            }
        }
        if (OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL == idx)
        {
            OMCIWARN("table ipv4MultiAddrTbl for me multi_gem_iwtp is full");
        }
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        OMCI_SET_TBL_UINT16(newVal.ipv6MultiAddrTbl[0].gemId, "gemId");
        OMCI_SET_TBL_UINT16(newVal.ipv6MultiAddrTbl[0].sndKey, "sndKey");
        OMCI_SET_TBL_UINT32(newVal.ipv6MultiAddrTbl[0].lsbIpDstAddrStart, "lsbIpDstAddrStart");
        OMCI_SET_TBL_UINT32(newVal.ipv6MultiAddrTbl[0].lsbIpDstAddrEnd, "lsbIpDstAddrEnd");
        OMCI_SET_TBL_UINT8_T(newVal.ipv6MultiAddrTbl[0].msbIpDstAddr, "msbIpDstAddr", OMCI_IPV6_LEAD_LEN);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);

        for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; idx++)
        {
            if (CA_OMCI_FALSE == multiGemIwtp->attr.ipv6MultiAddrTbl[idx].valid)
            {
                multiGemIwtp->attr.ipv6MultiAddrTbl[idx].valid             = CA_OMCI_TRUE;
                multiGemIwtp->attr.ipv6MultiAddrTbl[idx].gemId             = newVal.ipv6MultiAddrTbl[0].gemId;
                multiGemIwtp->attr.ipv6MultiAddrTbl[idx].sndKey            = newVal.ipv6MultiAddrTbl[0].sndKey;
                multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrStart = newVal.ipv6MultiAddrTbl[0].lsbIpDstAddrStart;
                multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrEnd   = newVal.ipv6MultiAddrTbl[0].lsbIpDstAddrEnd;
                OMCI_MEMCPY(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].msbIpDstAddr, newVal.ipv6MultiAddrTbl[0].msbIpDstAddr, OMCI_IPV6_LEAD_LEN);
                break;
            }
        }
        if (OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL == idx)
        {
            OMCIWARN("table ipv6MultiAddrTbl for me multi_gem_iwtp is full");
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_GEM_IWTP_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(multiGemIwtp->attr.gemNetCtpPoint, 1);
    OMCI_GET_ATTR_UINT8(multiGemIwtp->attr.iwOption, 2);
    OMCI_GET_ATTR_UINT16(multiGemIwtp->attr.srvProfPoint, 3);
    OMCI_GET_ATTR_UINT16(multiGemIwtp->attr.notUsed1, 4);
    OMCI_GET_ATTR_UINT8(multiGemIwtp->attr.pptpCounter, 5);
    OMCI_GET_ATTR_UINT8(multiGemIwtp->attr.operState, 6);
    OMCI_GET_ATTR_UINT16(multiGemIwtp->attr.galProfPoint, 7);
    OMCI_GET_ATTR_UINT8(multiGemIwtp->attr.notUsed2, 8);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiGemIwtp->attr.ipv4MultiAddrTbl[0]) - sizeof(multiGemIwtp->attr.ipv4MultiAddrTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&multiGemIwtp->ipv4MultiAddrTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[0]) - sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&multiGemIwtp->ipv6MultiAddrTbl, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx]) - sizeof(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiGemIwtp->ipv4MultiAddrTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].gemId);
                OMCI_CP_TBL_UINT16(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].sndKey);
                OMCI_CP_TBL_UINT32(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].ipAddrStart);
                OMCI_CP_TBL_UINT32(multiGemIwtp->attr.ipv4MultiAddrTbl[tblIdx].ipAddrEnd);
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
        ca_omci_next_success(&multiGemIwtp->ipv4MultiAddrTbl);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx]) - sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiGemIwtp->ipv6MultiAddrTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].gemId);
                OMCI_CP_TBL_UINT16(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].sndKey);
                OMCI_CP_TBL_UINT32(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].lsbIpDstAddrStart);
                OMCI_CP_TBL_UINT32(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].lsbIpDstAddrEnd);
                OMCI_MEMCPY((tblAttr + offset), &multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].msbIpDstAddr, sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].msbIpDstAddr));
                offset += sizeof(multiGemIwtp->attr.ipv6MultiAddrTbl[tblIdx].msbIpDstAddr);
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
        ca_omci_next_success(&multiGemIwtp->ipv6MultiAddrTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_mebase_t         *gemNetCtp    = NULL;
    ca_omci_mebase_t         *dot1p        = NULL;
    ca_omci_mebase_t         *macBrgSrv    = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    gemNetCtp = ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, multiGemIwtp->attr.gemNetCtpPoint);
    OMCI_RM_LINK_ME(multiGemIwtp, gemNetCtp);

    if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGemIwtp->attr.iwOption)
    {
        macBrgSrv = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, multiGemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(multiGemIwtp, macBrgSrv);
    }

    if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGemIwtp->attr.iwOption)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, multiGemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(multiGemIwtp, dot1p);
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_gem_iwtp_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(multiGemIwtp);
    OMCI_UPLOAD_ATTR_SIZE(16, multiGemIwtp->attr.gemNetCtpPoint, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, multiGemIwtp->attr.iwOption, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, multiGemIwtp->attr.srvProfPoint, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, multiGemIwtp->attr.notUsed1, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, multiGemIwtp->attr.pptpCounter, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, multiGemIwtp->attr.operState, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, multiGemIwtp->attr.galProfPoint, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, multiGemIwtp->attr.notUsed2, 8);
    for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv4MultiAddrTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiGemIwtp->attr.ipv4MultiAddrTbl[idx].gemId);
            OMCI_UPLOAD_TBL_UINT16(multiGemIwtp->attr.ipv4MultiAddrTbl[idx].sndKey);
            OMCI_UPLOAD_TBL_UINT32(multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrStart);
            OMCI_UPLOAD_TBL_UINT32(multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrEnd);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        }
    }
    for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiGemIwtp->attr.ipv6MultiAddrTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].gemId);
            OMCI_UPLOAD_TBL_UINT16(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].sndKey);
            OMCI_UPLOAD_TBL_UINT32(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrStart);
            OMCI_UPLOAD_TBL_UINT32(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrEnd);
            OMCI_UPLOAD_TBL_UINT8(multiGemIwtp->attr.ipv6MultiAddrTbl[idx].msbIpDstAddr);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        }
    }
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_multi_gem_iwtp_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_multi_gem_iwtp_t *multiGemIwtp = (ca_omci_multi_gem_iwtp_t *)me;
    ca_omci_uint32_t idx = 0;
    ca_omci_uint32_t i   = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: multiGemIwtp \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        gemNetCtpPoint: 0x%x\n", multiGemIwtp->attr.gemNetCtpPoint);
    omci_sprint(cli, "        iwOption:    0x%x\n", multiGemIwtp->attr.iwOption);
    omci_sprint(cli, "        srvProfPoint: 0x%x\n", multiGemIwtp->attr.srvProfPoint);
    omci_sprint(cli, "        notUsed1:    0x%x\n", multiGemIwtp->attr.notUsed1);
    omci_sprint(cli, "        operState:   0x%x\n", multiGemIwtp->attr.operState);
    omci_sprint(cli, "        galProfPoint: 0x%x\n", multiGemIwtp->attr.galProfPoint);
    omci_sprint(cli, "        notUsed2:    0x%x\n", multiGemIwtp->attr.notUsed2);
    for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV4_MULTI_ADDR_TBL; idx++)
    {
        if (CA_OMCI_FALSE == multiGemIwtp->attr.ipv4MultiAddrTbl[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        ipv4MultiAddrTbl[%d]:gemId 0x%x\n", idx, multiGemIwtp->attr.ipv4MultiAddrTbl[idx].gemId);
        omci_sprint(cli, "        ipv4MultiAddrTbl[%d]:sndKey 0x%x\n", idx, multiGemIwtp->attr.ipv4MultiAddrTbl[idx].sndKey);
        omci_sprint(cli, "        ipv4MultiAddrTbl[%d]:ipAddrStart 0x%x\n", idx, multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrStart);
        omci_sprint(cli, "        ipv4MultiAddrTbl[%d]:ipAddrEnd 0x%x\n", idx, multiGemIwtp->attr.ipv4MultiAddrTbl[idx].ipAddrEnd);
    }
    for (idx = 0; idx < OMCI_MULTI_GEM_IWTP_IPV6_MULTI_ADDR_TBL; idx++)
    {
        if (CA_OMCI_FALSE == multiGemIwtp->attr.ipv6MultiAddrTbl[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        ipv6MultiAddrTbl[%d]:gemId 0x%x\n", idx, multiGemIwtp->attr.ipv6MultiAddrTbl[idx].gemId);
        omci_sprint(cli, "        ipv6MultiAddrTbl[%d]:sndKey 0x%x\n", idx, multiGemIwtp->attr.ipv6MultiAddrTbl[idx].sndKey);
        omci_sprint(cli, "        ipv6MultiAddrTbl[%d]:lsbIpDstAddrStart 0x%x\n", idx, multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrStart);
        omci_sprint(cli, "        ipv6MultiAddrTbl[%d]:lsbIpDstAddrEnd 0x%x\n", idx, multiGemIwtp->attr.ipv6MultiAddrTbl[idx].lsbIpDstAddrEnd);
        for (i = 0; i < OMCI_IPV6_LEAD_LEN; i++)
        {
            omci_sprint(cli, "        ipv6MultiAddrTbl[%d].msbIpDstAddr[%d]: 0x%x \n", idx, i, multiGemIwtp->attr.ipv6MultiAddrTbl[idx].msbIpDstAddr[i]);
        }
    }
}


void ca_omci_me_multi_gem_iwtp_init(void)
{
    sg_omci_me_multi_gem_iwtp_handlers.meDisplay  = ca_omci_me_multi_gem_iwtp_display;
    sg_omci_me_multi_gem_iwtp_handlers.uploadProc = ca_omci_me_multi_gem_iwtp_upload;

    sg_omci_me_multi_gem_iwtp_handlers.createProc  = ca_omci_me_multi_gem_iwtp_create;
    sg_omci_me_multi_gem_iwtp_handlers.deleteProc  = ca_omci_me_multi_gem_iwtp_delete;
    sg_omci_me_multi_gem_iwtp_handlers.setProc     = ca_omci_me_multi_gem_iwtp_set;
    sg_omci_me_multi_gem_iwtp_handlers.getProc     = ca_omci_me_multi_gem_iwtp_get;
    sg_omci_me_multi_gem_iwtp_handlers.getNextProc = ca_omci_me_multi_gem_iwtp_get_next;
}


void ca_omci_me_multi_gem_iwtp_link(void)
{
    ca_omci_meclass_t          *meClass   = NULL;
    ca_omci_mebase_t           *meInst    = NULL;
    ca_omci_multi_gem_iwtp_t   *me        = NULL;
    ca_omci_gem_net_ctp_t      *gemNetCtp = NULL;
    ca_omci_ieee_dot1p_srv_t   *dot1p     = NULL;
    ca_omci_mac_brg_srv_t      *macBrgSrv = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no multi_gem_iwtp is created now");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me = (ca_omci_multi_gem_iwtp_t *)meInst;

        gemNetCtp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, me->attr.gemNetCtpPoint);
        OMCI_CHECK_PARAM_CNU(NULL == gemNetCtp);
        OMCI_ME_LINK(me, gemNetCtp);

        if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == me->attr.iwOption)
        {
            macBrgSrv = (ca_omci_mac_brg_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, me->attr.srvProfPoint);
            OMCI_CHECK_PARAM_CNU(NULL == macBrgSrv);
            OMCI_ME_LINK(me, macBrgSrv);
        }

        if (MULTI_GEM_IWTP_IW_OPTION_8021P == me->attr.iwOption)
        {
            dot1p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, me->attr.srvProfPoint);
            OMCI_CHECK_PARAM_CNU(NULL == dot1p);
            OMCI_ME_LINK(me, dot1p);
        }
    }
}


