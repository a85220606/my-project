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
#include "omci_me_multi_sub_moni.h"


static ca_omci_me_handler_t sg_omci_me_multi_sub_moni_handlers = {0};


ca_omci_me_attr_t g_omci_me_multi_sub_moni_attr[OMCI_ME_ATTR6] =
{
    {OMCI_ME_ATTR1, "me_type"},
    {OMCI_ME_ATTR2, "curr_multi_bw"},
    {OMCI_ME_ATTR3, "join_msg_cnt"},
    {OMCI_ME_ATTR4, "bw_excee_cnt"},
    {OMCI_ME_ATTR5, "ipv4_act_group_tbl"},
    {OMCI_ME_ATTR6, "ipv6_act_group_tbl"},
};


ca_omci_uint32_t ca_omci_me_multi_sub_moni_attr_len_get(void)
{
    ca_omci_multi_sub_moni_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.meType);
    attrLen += sizeof(me->attr.currMultiBw);
    attrLen += sizeof(me->attr.joinMsgCnt);
    attrLen += sizeof(me->attr.bwExceeCnt);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_multi_sub_moni_new(void)
{
    ca_omci_multi_sub_moni_t *me = (ca_omci_multi_sub_moni_t *)omci_malloc(sizeof(ca_omci_multi_sub_moni_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MULTI_SUB_MONI_CLASS_ID, &sg_omci_me_multi_sub_moni_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(multi_sub_moni);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_multi_sub_moni_t *multiSubMoni = (ca_omci_multi_sub_moni_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(multiSubMoni->attr.meType, "meType", 1);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_multi_sub_moni_t *multiSubMoni = (ca_omci_multi_sub_moni_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_SUB_MONI_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, multiSubMoni->attr.meType, "meType", 1);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_multi_sub_moni_t *multiSubMoni = (ca_omci_multi_sub_moni_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_SUB_MONI_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(multiSubMoni->attr.meType, 1);
    OMCI_GET_ATTR_UINT32(multiSubMoni->attr.currMultiBw, 2);
    OMCI_GET_ATTR_UINT32(multiSubMoni->attr.joinMsgCnt, 3);
    OMCI_GET_ATTR_UINT32(multiSubMoni->attr.bwExceeCnt, 4);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiSubMoni->attr.ipv4ActiveGroupTbl[0]) - sizeof(multiSubMoni->attr.ipv4ActiveGroupTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV4_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&multiSubMoni->ipv4ActiveGroupTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[0]) - sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV6_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiSubMoni->ipv6ActiveGroupTbl, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_multi_sub_moni_t *multiSubMoni = (ca_omci_multi_sub_moni_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx]) - sizeof(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiSubMoni->ipv4ActiveGroupTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV4_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV4_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].vlanId);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].srcIpAddr);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].multiDstIp);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].besEffActBw);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].clientIp);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].lastJoinT);
                OMCI_CP_TBL_UINT16(multiSubMoni->attr.ipv4ActiveGroupTbl[tblIdx].rsv);
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
        ca_omci_next_success(&multiSubMoni->ipv4ActiveGroupTbl);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx]) - sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiSubMoni->ipv6ActiveGroupTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV6_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_MONI_IPV6_ACT_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].vlanId);
                OMCI_MEMCPY((tblAttr + offset), &multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].srcIpAddr, sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].srcIpAddr));
                offset += sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].srcIpAddr);
                OMCI_MEMCPY((tblAttr + offset), &multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].multiDstIp, sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].multiDstIp));
                offset += sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].multiDstIp);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].besEffActBw);
                OMCI_MEMCPY((tblAttr + offset), &multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].clientIp, sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].clientIp));
                offset += sizeof(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].clientIp);
                OMCI_CP_TBL_UINT32(multiSubMoni->attr.ipv6ActiveGroupTbl[tblIdx].lastJoinT);
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
        ca_omci_next_success(&multiSubMoni->ipv6ActiveGroupTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_moni_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_multi_sub_moni_t *multiSubMoni = (ca_omci_multi_sub_moni_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(multiSubMoni);
    OMCI_UPLOAD_ATTR_SIZE(8, multiSubMoni->attr.meType, 1);
    OMCI_UPLOAD_ATTR_SIZE(32, multiSubMoni->attr.currMultiBw, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, multiSubMoni->attr.joinMsgCnt, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, multiSubMoni->attr.bwExceeCnt, 4);
    for (idx = 0; idx < OMCI_MULTI_SUB_MONI_IPV4_ACT_GROUP_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubMoni->attr.ipv4ActiveGroupTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].vlanId);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].srcIpAddr);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].multiDstIp);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].besEffActBw);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].clientIp);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].lastJoinT);
            OMCI_UPLOAD_TBL_UINT16(multiSubMoni->attr.ipv4ActiveGroupTbl[idx].rsv);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
        }
    }
    for (idx = 0; idx < OMCI_MULTI_SUB_MONI_IPV6_ACT_GROUP_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubMoni->attr.ipv6ActiveGroupTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].vlanId);
            OMCI_UPLOAD_TBL_UINT8(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].srcIpAddr);
            OMCI_UPLOAD_TBL_UINT8(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].multiDstIp);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].besEffActBw);
            OMCI_UPLOAD_TBL_UINT8(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].clientIp);
            OMCI_UPLOAD_TBL_UINT32(multiSubMoni->attr.ipv6ActiveGroupTbl[idx].lastJoinT);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        }
    }
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_multi_sub_moni_init(void)
{
    sg_omci_me_multi_sub_moni_handlers.uploadProc = ca_omci_me_multi_sub_moni_upload;

    sg_omci_me_multi_sub_moni_handlers.createProc  = ca_omci_me_multi_sub_moni_create;
    sg_omci_me_multi_sub_moni_handlers.deleteProc  = ca_omci_me_multi_sub_moni_delete;
    sg_omci_me_multi_sub_moni_handlers.setProc     = ca_omci_me_multi_sub_moni_set;
    sg_omci_me_multi_sub_moni_handlers.getProc     = ca_omci_me_multi_sub_moni_get;
    sg_omci_me_multi_sub_moni_handlers.getNextProc = ca_omci_me_multi_sub_moni_get_next;
}

