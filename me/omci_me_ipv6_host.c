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
#include "omci_me_ipv6_host.h"


static ca_omci_me_handler_t sg_omci_me_ipv6_host_handlers = {0};


ca_omci_me_attr_t g_omci_me_ipv6_host_attr[OMCI_ME_ATTR15] =
{
    {OMCI_ME_ATTR1,  "ip_option"},
    {OMCI_ME_ATTR2,  "mac_addr"},
    {OMCI_ME_ATTR3,  "onu_id"},
    {OMCI_ME_ATTR4,  "ipv6_link_local_addr"},
    {OMCI_ME_ATTR5,  "ipv6_addr"},
    {OMCI_ME_ATTR6,  "def_router"},
    {OMCI_ME_ATTR7,  "pri_dns"},
    {OMCI_ME_ATTR8,  "snd_dns"},
    {OMCI_ME_ATTR9,  "curr_addr_table"},
    {OMCI_ME_ATTR10, "curr_def_router_table"},
    {OMCI_ME_ATTR11, "curr_dns_table"},
    {OMCI_ME_ATTR12, "duid"},
    {OMCI_ME_ATTR13, "on_link_prefix"},
    {OMCI_ME_ATTR14, "curr_on_link_pre_table"},
    {OMCI_ME_ATTR15, "relay_agent_option"},
};


ca_omci_uint32_t ca_omci_me_ipv6_host_attr_len_get(void)
{
    ca_omci_ipv6_host_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.ipOption);
    attrLen += sizeof(me->attr.macAddr);
    attrLen += sizeof(me->attr.onuId);
    attrLen += sizeof(me->attr.ipv6LinkLocalAddr);
    attrLen += sizeof(me->attr.ipv6Addr);
    attrLen += sizeof(me->attr.defRouter);
    attrLen += sizeof(me->attr.priDns);
    attrLen += sizeof(me->attr.sndDns);
    attrLen += sizeof(me->attr.duid);
    attrLen += sizeof(me->attr.onLinkPrefix);
    attrLen += sizeof(me->attr.relayAgentOption);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ipv6_host_new(void)
{
    ca_omci_ipv6_host_t *me = (ca_omci_ipv6_host_t *)omci_malloc(sizeof(ca_omci_ipv6_host_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_IPV6_HOST_CLASS_ID, &sg_omci_me_ipv6_host_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ipv6_host);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_ipv6_host_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ipv6_host_t      *ipv6host = (ca_omci_ipv6_host_t *)me;
    ca_omci_ipv6_host_attr_t  newVal;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint8_t  idx     = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_ipv6_host_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_IPV6_HOST_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, ipv6host->attr.ipOption, "ipOption", 1);
    OMCI_UPDATE_ATTR_UINT_T(8, ipv6host->attr.onuId, "onuId", OMCI_ONU_ID_LEN, 3);
    OMCI_UPDATE_ATTR_UINT_T(8, ipv6host->attr.ipv6Addr, "ipv6Addr", OMCI_IPV6_ADDR_LEN, 5);
    OMCI_UPDATE_ATTR_UINT_T(8, ipv6host->attr.defRouter, "defRouter", OMCI_IPV6_ADDR_LEN, 6);
    OMCI_UPDATE_ATTR_UINT_T(8, ipv6host->attr.priDns, "priDns", OMCI_IPV6_ADDR_LEN, 7);
    OMCI_UPDATE_ATTR_UINT_T(8, ipv6host->attr.sndDns, "sndDns", OMCI_IPV6_ADDR_LEN, 8);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR13, inMask))
    {
        OMCI_SET_TBL_UINT8(ipv6host->attr.onLinkPrefix.length, "length");
        OMCI_SET_TBL_UINT8_T(ipv6host->attr.onLinkPrefix.prefix, "prefix", OMCI_IPV6_ADDR_LEN);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR14, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.currOnLinkPreTable[0].length, "length");
        OMCI_SET_TBL_UINT8(newVal.currOnLinkPreTable[0].flag, "flag");
        OMCI_SET_TBL_UINT8_T(newVal.currOnLinkPreTable[0].prefix, "prefix", OMCI_IPV6_ADDR_LEN);
        OMCI_SET_TBL_UINT32(newVal.currOnLinkPreTable[0].preLifetRemain, "preLifetRemain");
        OMCI_SET_TBL_UINT32(newVal.currOnLinkPreTable[0].validLifetRemain, "validLifetRemain");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR14, mask);
    }
    OMCI_UPDATE_ATTR_UINT(16, ipv6host->attr.relayAgentOption, "relayAgentOption", 15);
    //TODO: update table attribute
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ipv6_host_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ipv6_host_t *ipv6host = (ca_omci_ipv6_host_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_IPV6_HOST_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(ipv6host->attr.ipOption, 1);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.macAddr, OMCI_MAC_ADDR_LEN, 2);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.onuId, OMCI_ONU_ID_LEN, 3);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.ipv6LinkLocalAddr, OMCI_IPV6_ADDR_LEN, 4);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.ipv6Addr, OMCI_IPV6_ADDR_LEN, 5);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.defRouter, OMCI_IPV6_ADDR_LEN, 6);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.priDns, OMCI_IPV6_ADDR_LEN, 7);
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.sndDns, OMCI_IPV6_ADDR_LEN, 8);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currAddrTable[0]) - sizeof(ipv6host->attr.currAddrTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ADDR_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currAddrTable[tblIdx].valid)
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
        ca_omci_next_init(&ipv6host->currAddrTable, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currDefRouterTable[0]) - sizeof(ipv6host->attr.currDefRouterTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DEF_ROUTER_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDefRouterTable[tblIdx].valid)
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
        ca_omci_next_init(&ipv6host->currDefRouterTable, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currDnsTable[0]) - sizeof(ipv6host->attr.currDnsTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DNS_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDnsTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&ipv6host->currDnsTable, tblSeq);
    }
    OMCI_GET_ATTR_UINT8_T(ipv6host->attr.duid, OMCI_IPV6_HOST_DUID, 12);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR13, inMask))
    {
        OMCI_GET_TBL_UINT8(ipv6host->attr.onLinkPrefix.length);
        OMCI_GET_TBL_UINT8(ipv6host->attr.onLinkPrefix.prefix);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR14, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currOnLinkPreTable[0]) - sizeof(ipv6host->attr.currOnLinkPreTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ON_LINK_PRE_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currOnLinkPreTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR14, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&ipv6host->currOnLinkPreTable, tblSeq);
    }
    OMCI_GET_ATTR_UINT16(ipv6host->attr.relayAgentOption, 15);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ipv6_host_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_ipv6_host_t *ipv6host = (ca_omci_ipv6_host_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currAddrTable[tblIdx]) - sizeof(ipv6host->attr.currAddrTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&ipv6host->currAddrTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ADDR_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currAddrTable[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ADDR_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currAddrTable[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &ipv6host->attr.currAddrTable[tblIdx].ipAddr, sizeof(ipv6host->attr.currAddrTable[tblIdx].ipAddr));
                offset += sizeof(ipv6host->attr.currAddrTable[tblIdx].ipAddr);
                OMCI_CP_TBL_UINT32(ipv6host->attr.currAddrTable[tblIdx].preLifetRemain);
                OMCI_CP_TBL_UINT32(ipv6host->attr.currAddrTable[tblIdx].validLifetRemain);
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
        ca_omci_next_success(&ipv6host->currAddrTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currDefRouterTable[tblIdx]) - sizeof(ipv6host->attr.currDefRouterTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&ipv6host->currDefRouterTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DEF_ROUTER_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDefRouterTable[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DEF_ROUTER_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDefRouterTable[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &ipv6host->attr.currDefRouterTable[tblIdx].ipAddr, sizeof(ipv6host->attr.currDefRouterTable[tblIdx].ipAddr));
                offset += sizeof(ipv6host->attr.currDefRouterTable[tblIdx].ipAddr);
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
        ca_omci_next_success(&ipv6host->currDefRouterTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currDnsTable[tblIdx]) - sizeof(ipv6host->attr.currDnsTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&ipv6host->currDnsTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DNS_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDnsTable[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_DNS_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currDnsTable[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &ipv6host->attr.currDnsTable[tblIdx].ipAddr, sizeof(ipv6host->attr.currDnsTable[tblIdx].ipAddr));
                offset += sizeof(ipv6host->attr.currDnsTable[tblIdx].ipAddr);
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
        ca_omci_next_success(&ipv6host->currDnsTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR14, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(ipv6host->attr.currOnLinkPreTable[tblIdx]) - sizeof(ipv6host->attr.currOnLinkPreTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&ipv6host->currOnLinkPreTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ON_LINK_PRE_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currOnLinkPreTable[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_IPV6_HOST_CURR_ON_LINK_PRE_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == ipv6host->attr.currOnLinkPreTable[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT8(ipv6host->attr.currOnLinkPreTable[tblIdx].length);
                OMCI_CP_TBL_UINT8(ipv6host->attr.currOnLinkPreTable[tblIdx].flag);
                OMCI_MEMCPY((tblAttr + offset), &ipv6host->attr.currOnLinkPreTable[tblIdx].prefix, sizeof(ipv6host->attr.currOnLinkPreTable[tblIdx].prefix));
                offset += sizeof(ipv6host->attr.currOnLinkPreTable[tblIdx].prefix);
                OMCI_CP_TBL_UINT32(ipv6host->attr.currOnLinkPreTable[tblIdx].preLifetRemain);
                OMCI_CP_TBL_UINT32(ipv6host->attr.currOnLinkPreTable[tblIdx].validLifetRemain);
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
        ca_omci_next_success(&ipv6host->currOnLinkPreTable);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ipv6_host_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ipv6_host_t *ipv6host = (ca_omci_ipv6_host_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(ipv6host);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.ipOption, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.macAddr, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.onuId, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.ipv6LinkLocalAddr, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.ipv6Addr, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.defRouter, 6);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.priDns, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.sndDns, 8);
    for (idx = 0; idx < OMCI_IPV6_HOST_CURR_ADDR_TABLE; idx++)
    {
        if (CA_OMCI_TRUE == ipv6host->attr.currAddrTable[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currAddrTable[idx].ipAddr);
            OMCI_UPLOAD_TBL_UINT32(ipv6host->attr.currAddrTable[idx].preLifetRemain);
            OMCI_UPLOAD_TBL_UINT32(ipv6host->attr.currAddrTable[idx].validLifetRemain);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        }
    }
    for (idx = 0; idx < OMCI_IPV6_HOST_CURR_DEF_ROUTER_TABLE; idx++)
    {
        if (CA_OMCI_TRUE == ipv6host->attr.currDefRouterTable[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currDefRouterTable[idx].ipAddr);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        }
    }
    for (idx = 0; idx < OMCI_IPV6_HOST_CURR_DNS_TABLE; idx++)
    {
        if (CA_OMCI_TRUE == ipv6host->attr.currDnsTable[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currDnsTable[idx].ipAddr);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
        }
    }
    OMCI_UPLOAD_ATTR_SIZE(8, ipv6host->attr.duid, 12);
    OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.onLinkPrefix.length);
    OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.onLinkPrefix.prefix);
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);
    for (idx = 0; idx < OMCI_IPV6_HOST_CURR_ON_LINK_PRE_TABLE; idx++)
    {
        if (CA_OMCI_TRUE == ipv6host->attr.currOnLinkPreTable[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currOnLinkPreTable[idx].length);
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currOnLinkPreTable[idx].flag);
            OMCI_UPLOAD_TBL_UINT8(ipv6host->attr.currOnLinkPreTable[idx].prefix);
            OMCI_UPLOAD_TBL_UINT32(ipv6host->attr.currOnLinkPreTable[idx].preLifetRemain);
            OMCI_UPLOAD_TBL_UINT32(ipv6host->attr.currOnLinkPreTable[idx].validLifetRemain);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR14, mask);
        }
    }
    OMCI_UPLOAD_ATTR_SIZE(16, ipv6host->attr.relayAgentOption, 15);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_ipv6_host_init(void)
{
    sg_omci_me_ipv6_host_handlers.uploadProc = ca_omci_me_ipv6_host_upload;

    sg_omci_me_ipv6_host_handlers.setProc     = ca_omci_me_ipv6_host_set;
    sg_omci_me_ipv6_host_handlers.getProc     = ca_omci_me_ipv6_host_get;
    sg_omci_me_ipv6_host_handlers.getNextProc = ca_omci_me_ipv6_host_get_next;
}

