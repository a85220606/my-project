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
#include "omci_me_sip_agt_cfg.h"
#include "omci_me_large_string.h"
#include "omci_me_tcp_udp_cfg.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_sip_agt_cfg_handlers = {0};


ca_omci_me_attr_t g_omci_me_sip_agt_cfg_attr[OMCI_ME_ATTR15] =
{
    {OMCI_ME_ATTR1,  "proxy_srv_addr_point"},
    {OMCI_ME_ATTR2,  "outbound_proxy_addr_point"},
    {OMCI_ME_ATTR3,  "pri_sip_dns"},
    {OMCI_ME_ATTR4,  "snd_sip_dns"},
    {OMCI_ME_ATTR5,  "tcp_udp_pointer"},
    {OMCI_ME_ATTR6,  "reg_exp_time"},
    {OMCI_ME_ATTR7,  "rereg_head_start_time"},
    {OMCI_ME_ATTR8,  "host_part_url"},
    {OMCI_ME_ATTR9,  "sip_status"},
    {OMCI_ME_ATTR10, "sip_registrar"},
    {OMCI_ME_ATTR11, "softswitch"},
    {OMCI_ME_ATTR12, "sip_resp_table"},
    {OMCI_ME_ATTR13, "op_trans_ctl"},
    {OMCI_ME_ATTR14, "url_format"},
    {OMCI_ME_ATTR15, "redundant_agent_point"},
};


ca_omci_uint32_t ca_omci_me_sip_agt_cfg_attr_len_get(void)
{
    ca_omci_sip_agt_cfg_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.proxySrvAddrPoint);
    attrLen += sizeof(me->attr.outboundProxyAddrP);
    attrLen += sizeof(me->attr.priSipDns);
    attrLen += sizeof(me->attr.sndSipDns);
    attrLen += sizeof(me->attr.tcpUdpPointer);
    attrLen += sizeof(me->attr.regExpTime);
    attrLen += sizeof(me->attr.reregHeadStartT);
    attrLen += sizeof(me->attr.hostPartUrl);
    attrLen += sizeof(me->attr.sipStatus);
    attrLen += sizeof(me->attr.sipRegistrar);
    attrLen += sizeof(me->attr.softswitch);
    attrLen += sizeof(me->attr.opTransCtl);
    attrLen += sizeof(me->attr.urlFormat);
    attrLen += sizeof(me->attr.redundantAgentP);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_sip_agt_cfg_new(void)
{
    ca_omci_sip_agt_cfg_t *me = (ca_omci_sip_agt_cfg_t *)omci_malloc(sizeof(ca_omci_sip_agt_cfg_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_SIP_AGT_CFG_CLASS_ID, &sg_omci_me_sip_agt_cfg_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(sip_agt_cfg);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_sip_agt_cfg_t *sipAgtCfg = (ca_omci_sip_agt_cfg_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(sipAgtCfg->attr.proxySrvAddrPoint, "proxySrvAddrPoint", 1);
    OMCI_SET_ATTR_UINT16(sipAgtCfg->attr.outboundProxyAddrP, "outboundProxyAddrP", 2);
    OMCI_SET_ATTR_UINT32(sipAgtCfg->attr.priSipDns, "priSipDns", 3);
    OMCI_SET_ATTR_UINT32(sipAgtCfg->attr.sndSipDns, "sndSipDns", 4);
    OMCI_SET_ATTR_UINT16(sipAgtCfg->attr.hostPartUrl, "hostPartUrl", 8);
    OMCI_SET_ATTR_UINT16(sipAgtCfg->attr.sipRegistrar, "sipRegistrar", 10);
    OMCI_SET_ATTR_UINT32(sipAgtCfg->attr.softswitch, "softswitch", 11);
    OMCI_SET_ATTR_UINT8(sipAgtCfg->attr.opTransCtl, "opTransCtl", 13);
    OMCI_SET_ATTR_UINT8(sipAgtCfg->attr.urlFormat, "urlFormat", 14);
    OMCI_SET_ATTR_UINT16(sipAgtCfg->attr.redundantAgentP, "redundantAgentP", 15);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_sip_agt_cfg_t      *sipAgtCfg = (ca_omci_sip_agt_cfg_t *)me;
    ca_omci_sip_agt_cfg_attr_t  newVal;
    ca_omci_uint16_t  offset        = 0;
    ca_omci_uint16_t  mask          = 0;
    ca_omci_mebase_t *proxy         = NULL;
    ca_omci_mebase_t *outboundProxy = NULL;
    ca_omci_mebase_t *tcpUdpCfg     = NULL;
    ca_omci_mebase_t *hostPartUri   = NULL;
    ca_omci_mebase_t *reduSipAgtCfg = NULL;
    ca_omci_uint16_t  idx           = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_sip_agt_cfg_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_SIP_AGT_CFG_SET_MASK_FULL);

    //remove old link here
    proxy         = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.proxySrvAddrPoint);
    outboundProxy = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.outboundProxyAddrP);
    tcpUdpCfg     = ca_omci_me_entry_lookup(OMCI_ME_TCP_UDP_CFG_CLASS_ID, sipAgtCfg->attr.tcpUdpPointer);
    hostPartUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.hostPartUrl);
    reduSipAgtCfg = ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, sipAgtCfg->attr.redundantAgentP);
    OMCI_RM_LINK_ME(sipAgtCfg, proxy);
    OMCI_RM_LINK_ME(sipAgtCfg, outboundProxy);
    OMCI_RM_LINK_ME(sipAgtCfg, tcpUdpCfg);
    OMCI_RM_LINK_ME(sipAgtCfg, hostPartUri);
    OMCI_RM_LINK_ME(sipAgtCfg, reduSipAgtCfg);

    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.proxySrvAddrPoint, "proxySrvAddrPoint", 1);
    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.outboundProxyAddrP, "outboundProxyAddrP", 2);
    OMCI_UPDATE_ATTR_UINT(32, sipAgtCfg->attr.priSipDns, "priSipDns", 3);
    OMCI_UPDATE_ATTR_UINT(32, sipAgtCfg->attr.sndSipDns, "sndSipDns", 4);
    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.tcpUdpPointer, "tcpUdpPointer", 5);
    OMCI_UPDATE_ATTR_UINT(32, sipAgtCfg->attr.regExpTime, "regExpTime", 6);
    OMCI_UPDATE_ATTR_UINT(32, sipAgtCfg->attr.reregHeadStartT, "reregHeadStartT", 7);
    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.hostPartUrl, "hostPartUrl", 8);
    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.sipRegistrar, "sipRegistrar", 10);
    OMCI_UPDATE_ATTR_UINT(32, sipAgtCfg->attr.softswitch, "softswitch", 11);
    OMCI_UPDATE_ATTR_UINT(8, sipAgtCfg->attr.opTransCtl, "opTransCtl", 13);
    OMCI_UPDATE_ATTR_UINT(8, sipAgtCfg->attr.urlFormat, "urlFormat", 14);
    OMCI_UPDATE_ATTR_UINT(16, sipAgtCfg->attr.redundantAgentP, "redundantAgentP", 15);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, mask))
    {
        OMCI_SET_TBL_UINT16(newVal.sipRespTable[0].sipRespCode, "sipRespCode");
        OMCI_SET_TBL_UINT8(newVal.sipRespTable[0].tone, "tone");
        OMCI_SET_TBL_UINT16(newVal.sipRespTable[0].textMsg, "textMsg");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);

        //clear the table
        if (0 == newVal.sipRespTable[0].sipRespCode)
        {
            OMCI_MEMSET(sipAgtCfg->attr.sipRespTable, 0, sizeof(ca_omci_sip_resp_table_t) * OMCI_SIP_AGT_CFG_SIP_RESP_TABLE);
        }
        else
        {
            //save the value
            for (idx = 0; idx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; idx++)
            {
                if ((CA_OMCI_TRUE == sipAgtCfg->attr.sipRespTable[idx].valid)
                 && (sipAgtCfg->attr.sipRespTable[idx].sipRespCode == newVal.sipRespTable[0].sipRespCode))
                {
                    //update the value
                    sipAgtCfg->attr.sipRespTable[idx].tone    = newVal.sipRespTable[0].tone;
                    sipAgtCfg->attr.sipRespTable[idx].textMsg = newVal.sipRespTable[0].textMsg;
                    break;
                }
            }

            if (OMCI_SIP_AGT_CFG_SIP_RESP_TABLE == idx)
            {
                //no old index match, save as a new one;
                for (idx = 0; idx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; idx++)
                {
                    if (CA_OMCI_FALSE == sipAgtCfg->attr.sipRespTable[idx].valid)
                    {
                        sipAgtCfg->attr.sipRespTable[idx].sipRespCode = newVal.sipRespTable[0].sipRespCode;
                        sipAgtCfg->attr.sipRespTable[idx].tone        = newVal.sipRespTable[0].tone;
                        sipAgtCfg->attr.sipRespTable[idx].textMsg     = newVal.sipRespTable[0].textMsg;
                        sipAgtCfg->attr.sipRespTable[idx].valid       = CA_OMCI_TRUE;
                        break;
                    }
                }

                if (OMCI_SIP_AGT_CFG_SIP_RESP_TABLE == idx)
                {
                    OMCIMNGERR("Not enough resource for sip response table");
                }
            }
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_sip_agt_cfg_t *sipAgtCfg = (ca_omci_sip_agt_cfg_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_SIP_AGT_CFG_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.proxySrvAddrPoint, 1);
    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.outboundProxyAddrP, 2);
    OMCI_GET_ATTR_UINT32(sipAgtCfg->attr.priSipDns, 3);
    OMCI_GET_ATTR_UINT32(sipAgtCfg->attr.sndSipDns, 4);
    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.tcpUdpPointer, 5);
    OMCI_GET_ATTR_UINT32(sipAgtCfg->attr.regExpTime, 6);
    OMCI_GET_ATTR_UINT32(sipAgtCfg->attr.reregHeadStartT, 7);
    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.hostPartUrl, 8);
    OMCI_GET_ATTR_UINT8(sipAgtCfg->attr.sipStatus, 9);
    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.sipRegistrar, 10);
    OMCI_GET_ATTR_UINT32(sipAgtCfg->attr.softswitch, 11);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(sipAgtCfg->attr.sipRespTable[0]) - sizeof(sipAgtCfg->attr.sipRespTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == sipAgtCfg->attr.sipRespTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&sipAgtCfg->sipRespTable, tblSeq);
    }
    OMCI_GET_ATTR_UINT8(sipAgtCfg->attr.opTransCtl, 13);
    OMCI_GET_ATTR_UINT8(sipAgtCfg->attr.urlFormat, 14);
    OMCI_GET_ATTR_UINT16(sipAgtCfg->attr.redundantAgentP, 15);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_sip_agt_cfg_t *sipAgtCfg = (ca_omci_sip_agt_cfg_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(sipAgtCfg->attr.sipRespTable[tblIdx]) - sizeof(sipAgtCfg->attr.sipRespTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&sipAgtCfg->sipRespTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == sipAgtCfg->attr.sipRespTable[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == sipAgtCfg->attr.sipRespTable[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(sipAgtCfg->attr.sipRespTable[tblIdx].sipRespCode);
                OMCI_CP_TBL_UINT8(sipAgtCfg->attr.sipRespTable[tblIdx].tone);
                OMCI_CP_TBL_UINT16(sipAgtCfg->attr.sipRespTable[tblIdx].textMsg);
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
        ca_omci_next_success(&sipAgtCfg->sipRespTable);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_sip_agt_cfg_t *sipAgtCfg     = (ca_omci_sip_agt_cfg_t *)me;
    ca_omci_mebase_t      *proxy         = NULL;
    ca_omci_mebase_t      *outboundProxy = NULL;
    ca_omci_mebase_t      *tcpUdpCfg     = NULL;
    ca_omci_mebase_t      *hostPartUri   = NULL;
    ca_omci_mebase_t      *reduSipAgtCfg = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    proxy         = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.proxySrvAddrPoint);
    outboundProxy = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.outboundProxyAddrP);
    tcpUdpCfg     = ca_omci_me_entry_lookup(OMCI_ME_TCP_UDP_CFG_CLASS_ID, sipAgtCfg->attr.tcpUdpPointer);
    hostPartUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipAgtCfg->attr.hostPartUrl);
    reduSipAgtCfg = ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, sipAgtCfg->attr.redundantAgentP);

    OMCI_RM_LINK_ME(sipAgtCfg, proxy);
    OMCI_RM_LINK_ME(sipAgtCfg, outboundProxy);
    OMCI_RM_LINK_ME(sipAgtCfg, tcpUdpCfg);
    OMCI_RM_LINK_ME(sipAgtCfg, hostPartUri);
    OMCI_RM_LINK_ME(sipAgtCfg, reduSipAgtCfg);

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_agt_cfg_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_sip_agt_cfg_t *sipAgtCfg = (ca_omci_sip_agt_cfg_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(sipAgtCfg);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.proxySrvAddrPoint, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.outboundProxyAddrP, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, sipAgtCfg->attr.priSipDns, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, sipAgtCfg->attr.sndSipDns, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.tcpUdpPointer, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, sipAgtCfg->attr.regExpTime, 6);
    OMCI_UPLOAD_ATTR_SIZE(32, sipAgtCfg->attr.reregHeadStartT, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.hostPartUrl, 8);
    OMCI_UPLOAD_ATTR_SIZE(8, sipAgtCfg->attr.sipStatus, 9);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.sipRegistrar, 10);
    OMCI_UPLOAD_ATTR_SIZE(32, sipAgtCfg->attr.softswitch, 11);
    for (idx = 0; idx < OMCI_SIP_AGT_CFG_SIP_RESP_TABLE; idx++)
    {
        if (CA_OMCI_TRUE == sipAgtCfg->attr.sipRespTable[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(sipAgtCfg->attr.sipRespTable[idx].sipRespCode);
            OMCI_UPLOAD_TBL_UINT8(sipAgtCfg->attr.sipRespTable[idx].tone);
            OMCI_UPLOAD_TBL_UINT16(sipAgtCfg->attr.sipRespTable[idx].textMsg);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);
        }
    }
    OMCI_UPLOAD_ATTR_SIZE(8, sipAgtCfg->attr.opTransCtl, 13);
    OMCI_UPLOAD_ATTR_SIZE(8, sipAgtCfg->attr.urlFormat, 14);
    OMCI_UPLOAD_ATTR_SIZE(16, sipAgtCfg->attr.redundantAgentP, 15);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_sip_agt_cfg_init(void)
{
    sg_omci_me_sip_agt_cfg_handlers.uploadProc = ca_omci_me_sip_agt_cfg_upload;

    sg_omci_me_sip_agt_cfg_handlers.createProc  = ca_omci_me_sip_agt_cfg_create;
    sg_omci_me_sip_agt_cfg_handlers.deleteProc  = ca_omci_me_sip_agt_cfg_delete;
    sg_omci_me_sip_agt_cfg_handlers.setProc     = ca_omci_me_sip_agt_cfg_set;
    sg_omci_me_sip_agt_cfg_handlers.getProc     = ca_omci_me_sip_agt_cfg_get;
    sg_omci_me_sip_agt_cfg_handlers.getNextProc = ca_omci_me_sip_agt_cfg_get_next;
}


void ca_omci_me_sip_agt_cfg_link(void)
{
    ca_omci_meclass_t      *meClass       = NULL;
    ca_omci_mebase_t       *meInst        = NULL;
    ca_omci_sip_agt_cfg_t  *me            = NULL;
    ca_omci_large_string_t *proxy         = NULL;
    ca_omci_large_string_t *outboundProxy = NULL;
    ca_omci_tcp_udp_cfg_t  *tcpUdpCfg     = NULL;
    ca_omci_large_string_t *hostPartUri   = NULL;
    ca_omci_sip_agt_cfg_t  *reduSipAgtCfg = NULL;

    meClass = ca_omci_me_get(OMCI_ME_SIP_AGT_CFG_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me            = (ca_omci_sip_agt_cfg_t *)meInst;
        proxy         = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.proxySrvAddrPoint);
        outboundProxy = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.outboundProxyAddrP);
        tcpUdpCfg     = (ca_omci_tcp_udp_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_TCP_UDP_CFG_CLASS_ID, me->attr.tcpUdpPointer);
        hostPartUri   = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.hostPartUrl);
        reduSipAgtCfg = (ca_omci_sip_agt_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, me->attr.redundantAgentP);

        OMCI_ME_LINK(me, proxy);
        OMCI_ME_LINK(me, outboundProxy);
        OMCI_ME_LINK(me, tcpUdpCfg);
        OMCI_ME_LINK(me, hostPartUri);
        OMCI_ME_LINK(me, reduSipAgtCfg);
    }
}

