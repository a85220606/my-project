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
#include "omci_me_mac_port_cfg.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_ip_host.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_veip.h"
#include "omci_me_traffic_descript.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_mac_port_cfg_handlers = {0};


ca_omci_me_attr_t g_omci_me_mac_port_cfg_attr[OMCI_ME_ATTR13] =
{
    {OMCI_ME_ATTR1,  "brg_id_pointer"},
    {OMCI_ME_ATTR2,  "port_num"},
    {OMCI_ME_ATTR3,  "tp_type"},
    {OMCI_ME_ATTR4,  "tp_pointer"},
    {OMCI_ME_ATTR5,  "port_priority"},
    {OMCI_ME_ATTR6,  "port_path_cost"},
    {OMCI_ME_ATTR7,  "port_span_tree_ind"},
    {OMCI_ME_ATTR8,  "deprecated1"},
    {OMCI_ME_ATTR9,  "deprecated2"},
    {OMCI_ME_ATTR10, "port_mac_addr"},
    {OMCI_ME_ATTR11, "outbound_td_point"},
    {OMCI_ME_ATTR12, "inbound_td_point"},
    {OMCI_ME_ATTR13, "mac_learn_depth"},
};


extern void ca_omci_me_mac_port_des_auto_create(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_des_delete(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_filter_auto_create(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_filter_delete(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_brg_auto_create(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_brg_delete(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_filter_pre_auto_create(ca_omci_uint16_t instance);
extern void ca_omci_me_mac_port_filter_pre_delete(ca_omci_uint16_t instance);
extern void ca_omci_me_brv6_pre_assi_auto_create(ca_omci_uint16_t instance);
extern void ca_omci_me_brv6_pre_assi_delete(ca_omci_uint16_t instance);


ca_omci_uint32_t ca_omci_me_mac_port_cfg_attr_len_get(void)
{
    ca_omci_mac_port_cfg_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.brgIdPointer);
    attrLen += sizeof(me->attr.portNum);
    attrLen += sizeof(me->attr.tpType);
    attrLen += sizeof(me->attr.tpPointer);
    attrLen += sizeof(me->attr.portPriority);
    attrLen += sizeof(me->attr.portPathCost);
    attrLen += sizeof(me->attr.portSpanningTreeInd);
    attrLen += sizeof(me->attr.deprecated1);
    attrLen += sizeof(me->attr.deprecated2);
    attrLen += sizeof(me->attr.portMacAddr);
    attrLen += sizeof(me->attr.outboundTdPoint);
    attrLen += sizeof(me->attr.inboundTdPoint);
    attrLen += sizeof(me->attr.macLearnDepth);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_mac_port_cfg_new(void)
{
    ca_omci_mac_port_cfg_t *me = (ca_omci_mac_port_cfg_t *)omci_malloc(sizeof(ca_omci_mac_port_cfg_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MAC_PORT_CFG_CLASS_ID, &sg_omci_me_mac_port_cfg_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(mac_port_cfg);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_mac_port_cfg_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(macPortCfg->attr.brgIdPointer, "brgIdPointer", 1);
    OMCI_SET_ATTR_UINT8(macPortCfg->attr.portNum, "portNum", 2);
    OMCI_SET_ATTR_UINT8(macPortCfg->attr.tpType, "tpType", 3);
    OMCI_SET_ATTR_UINT16(macPortCfg->attr.tpPointer, "tpPointer", 4);
    OMCI_SET_ATTR_UINT16(macPortCfg->attr.portPriority, "portPriority", 5);
    OMCI_SET_ATTR_UINT16(macPortCfg->attr.portPathCost, "portPathCost", 6);
    OMCI_SET_ATTR_UINT8(macPortCfg->attr.portSpanningTreeInd, "portSpanningTreeInd", 7);
    OMCI_SET_ATTR_UINT8(macPortCfg->attr.deprecated1, "deprecated1", 8);
    OMCI_SET_ATTR_UINT16(macPortCfg->attr.deprecated2, "deprecated2", 9);
    OMCI_SET_ATTR_UINT8(macPortCfg->attr.macLearnDepth, "macLearnDepth", 13);

    if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != macPortCfg->attr.tpType)
     && (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV != macPortCfg->attr.tpType)
     && (MAC_PORT_CFG_TP_TYPE_IPHOST != macPortCfg->attr.tpType)
     && (MAC_PORT_CFG_TP_TYPE_GEM_IWTP != macPortCfg->attr.tpType)
     && (MAC_PORT_CFG_TP_TYPE_MULTI_GEM != macPortCfg->attr.tpType)
     && (MAC_PORT_CFG_TP_TYPE_VEIP != macPortCfg->attr.tpType))
    {
        OMCIWARN("unsupport tp type [%d]", macPortCfg->attr.tpType);
    }

    ca_omci_me_mac_port_des_auto_create(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_filter_auto_create(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_brg_auto_create(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_filter_pre_auto_create(macPortCfg->meBase.instanceId);
    ca_omci_me_brv6_pre_assi_auto_create(macPortCfg->meBase.instanceId);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_cfg_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_uint16_t  offset     = 0;
    ca_omci_uint16_t  mask       = 0;
    ca_omci_mebase_t *inboundTc  = NULL;
    ca_omci_mebase_t *outboundTc = NULL;
    ca_omci_mebase_t *macBrgSrv  = NULL;
    ca_omci_mebase_t *pptpEth    = NULL;
    ca_omci_mebase_t *dot1p      = NULL;
    ca_omci_mebase_t *iphost     = NULL;
    ca_omci_mebase_t *gemIwTp    = NULL;
    ca_omci_mebase_t *multiGem   = NULL;
    ca_omci_mebase_t *veip       = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MAC_PORT_CFG_SET_MASK_FULL);

    //remove old link
    macBrgSrv  = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, macPortCfg->attr.brgIdPointer);
    outboundTc = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, macPortCfg->attr.outboundTdPoint);
    inboundTc  = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, macPortCfg->attr.inboundTdPoint);
    OMCI_RM_LINK_ME(macPortCfg, macBrgSrv);
    OMCI_RM_LINK_ME(macPortCfg, outboundTc);
    OMCI_RM_LINK_ME(macPortCfg, inboundTc);

    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == macPortCfg->attr.tpType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, pptpEth);
    }

    if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == macPortCfg->attr.tpType)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, dot1p);
    }

    if (MAC_PORT_CFG_TP_TYPE_IPHOST == macPortCfg->attr.tpType)
    {
        //may add iphost v6 me here
        iphost = ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, iphost);
    }

    if (MAC_PORT_CFG_TP_TYPE_GEM_IWTP == macPortCfg->attr.tpType)
    {
        gemIwTp = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, gemIwTp);
    }

    if (MAC_PORT_CFG_TP_TYPE_MULTI_GEM == macPortCfg->attr.tpType)
    {
        multiGem = ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, multiGem);
    }

    if (MAC_PORT_CFG_TP_TYPE_VEIP == macPortCfg->attr.tpType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, veip);
    }

    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.brgIdPointer, "brgIdPointer", 1);
    OMCI_UPDATE_ATTR_UINT(8, macPortCfg->attr.portNum, "portNum", 2);
    OMCI_UPDATE_ATTR_UINT(8, macPortCfg->attr.tpType, "tpType", 3);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.tpPointer, "tpPointer", 4);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.portPriority, "portPriority", 5);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.portPathCost, "portPathCost", 6);
    OMCI_UPDATE_ATTR_UINT(8, macPortCfg->attr.portSpanningTreeInd, "portSpanningTreeInd", 7);
    OMCI_UPDATE_ATTR_UINT(8, macPortCfg->attr.deprecated1, "deprecated1", 8);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.deprecated2, "deprecated2", 9);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.outboundTdPoint, "outboundTdPoint", 11);
    OMCI_UPDATE_ATTR_UINT(16, macPortCfg->attr.inboundTdPoint, "inboundTdPoint", 12);
    OMCI_UPDATE_ATTR_UINT(8, macPortCfg->attr.macLearnDepth, "macLearnDepth", 13);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_cfg_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MAC_PORT_CFG_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(macPortCfg->attr.brgIdPointer, 1);
    OMCI_GET_ATTR_UINT8(macPortCfg->attr.portNum, 2);
    OMCI_GET_ATTR_UINT8(macPortCfg->attr.tpType, 3);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.tpPointer, 4);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.portPriority, 5);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.portPathCost, 6);
    OMCI_GET_ATTR_UINT8(macPortCfg->attr.portSpanningTreeInd, 7);
    OMCI_GET_ATTR_UINT8(macPortCfg->attr.deprecated1, 8);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.deprecated2, 9);
    OMCI_GET_ATTR_UINT8_T(macPortCfg->attr.portMacAddr, OMCI_MAC_ADDR_LEN, 10);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.outboundTdPoint, 11);
    OMCI_GET_ATTR_UINT16(macPortCfg->attr.inboundTdPoint, 12);
    OMCI_GET_ATTR_UINT8(macPortCfg->attr.macLearnDepth, 13);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_cfg_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_mebase_t       *macBrgSrv  = NULL;
    ca_omci_mebase_t       *pptpEth    = NULL;
    ca_omci_mebase_t       *dot1p      = NULL;
    ca_omci_mebase_t       *iphost     = NULL;
    ca_omci_mebase_t       *gemIwTp    = NULL;
    ca_omci_mebase_t       *multiGem   = NULL;
    ca_omci_mebase_t       *veip       = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    macBrgSrv = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, macPortCfg->attr.brgIdPointer);
    OMCI_RM_LINK_ME(macPortCfg, macBrgSrv);

    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == macPortCfg->attr.tpType)
    {
        pptpEth = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, pptpEth);
    }

    if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == macPortCfg->attr.tpType)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, dot1p);
    }

    if (MAC_PORT_CFG_TP_TYPE_IPHOST == macPortCfg->attr.tpType)
    {
        //may add iphost v6 me here
        iphost = ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, iphost);
    }

    if (MAC_PORT_CFG_TP_TYPE_GEM_IWTP == macPortCfg->attr.tpType)
    {
        gemIwTp = ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, gemIwTp);
    }

    if (MAC_PORT_CFG_TP_TYPE_MULTI_GEM == macPortCfg->attr.tpType)
    {
        multiGem = ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, multiGem);
    }

    if (MAC_PORT_CFG_TP_TYPE_VEIP == macPortCfg->attr.tpType)
    {
        veip = ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, macPortCfg->attr.tpPointer);
        OMCI_RM_LINK_ME(macPortCfg, veip);
    }

    ca_omci_me_mac_port_des_delete(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_filter_delete(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_brg_delete(macPortCfg->meBase.instanceId);
    ca_omci_me_mac_port_filter_pre_delete(macPortCfg->meBase.instanceId);
    ca_omci_me_brv6_pre_assi_delete(macPortCfg->meBase.instanceId);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_cfg_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(macPortCfg);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.brgIdPointer, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.portNum, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.tpType, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.tpPointer, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.portPriority, 5);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.portPathCost, 6);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.portSpanningTreeInd, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.deprecated1, 8);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.deprecated2, 9);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.portMacAddr, 10);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.outboundTdPoint, 11);
    OMCI_UPLOAD_ATTR_SIZE(16, macPortCfg->attr.inboundTdPoint, 12);
    OMCI_UPLOAD_ATTR_SIZE(8, macPortCfg->attr.macLearnDepth, 13);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_mac_port_cfg_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_mac_port_cfg_t *macPortCfg = (ca_omci_mac_port_cfg_t *)me;
    ca_omci_uint32_t idx = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: macPortCfg \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        brgIdPointer: 0x%x\n", macPortCfg->attr.brgIdPointer);
    omci_sprint(cli, "        portNum:     0x%x\n", macPortCfg->attr.portNum);
    omci_sprint(cli, "        tpType:      0x%x\n", macPortCfg->attr.tpType);
    omci_sprint(cli, "        tpPointer:   0x%x\n", macPortCfg->attr.tpPointer);
    omci_sprint(cli, "        portPriority: 0x%x\n", macPortCfg->attr.portPriority);
    omci_sprint(cli, "        portPathCost: 0x%x\n", macPortCfg->attr.portPathCost);
    omci_sprint(cli, "        portSpanningTreeInd: 0x%x\n", macPortCfg->attr.portSpanningTreeInd);
    omci_sprint(cli, "        deprecated1: 0x%x\n", macPortCfg->attr.deprecated1);
    omci_sprint(cli, "        deprecated2: 0x%x\n", macPortCfg->attr.deprecated2);
    for (idx = 0; idx < OMCI_MAC_ADDR_LEN; idx++)
    {
        omci_sprint(cli, "        portMacAddr[%d]: 0x%x \n", idx, macPortCfg->attr.portMacAddr[idx]);
    }
    omci_sprint(cli, "        outboundTdPoint: 0x%x\n", macPortCfg->attr.outboundTdPoint);
    omci_sprint(cli, "        inboundTdPoint: 0x%x\n", macPortCfg->attr.inboundTdPoint);
    omci_sprint(cli, "        macLearnDepth: 0x%x\n", macPortCfg->attr.macLearnDepth);
}


void ca_omci_me_mac_port_cfg_init(void)
{
    sg_omci_me_mac_port_cfg_handlers.meDisplay  = ca_omci_me_mac_port_cfg_display;
    sg_omci_me_mac_port_cfg_handlers.uploadProc = ca_omci_me_mac_port_cfg_upload;

    sg_omci_me_mac_port_cfg_handlers.createProc = ca_omci_me_mac_port_cfg_create;
    sg_omci_me_mac_port_cfg_handlers.deleteProc = ca_omci_me_mac_port_cfg_delete;
    sg_omci_me_mac_port_cfg_handlers.setProc    = ca_omci_me_mac_port_cfg_set;
    sg_omci_me_mac_port_cfg_handlers.getProc    = ca_omci_me_mac_port_cfg_get;
}


void ca_omci_me_mac_port_cfg_link(void)
{
    ca_omci_meclass_t          *meClass    = NULL;
    ca_omci_mebase_t           *meInst     = NULL;
    ca_omci_mac_port_cfg_t     *me         = NULL;
    ca_omci_mac_brg_srv_t      *macBrgSrv  = NULL;
    ca_omci_pptp_eth_t         *pptpEth    = NULL;
    ca_omci_ieee_dot1p_srv_t   *dot1p      = NULL;
    ca_omci_ip_host_t          *iphost     = NULL;
    ca_omci_gem_iwtp_t         *gemIwTp    = NULL;
    ca_omci_multi_gem_iwtp_t   *multiGem   = NULL;
    ca_omci_veip_t             *veip       = NULL;
    ca_omci_traffic_descript_t *inboundTc  = NULL;
    ca_omci_traffic_descript_t *outboundTc = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_MAC_PORT_CFG_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no mac_port_cfg is created now");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me = (ca_omci_mac_port_cfg_t *)meInst;

        macBrgSrv  = (ca_omci_mac_brg_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, me->attr.brgIdPointer);
        outboundTc = (ca_omci_traffic_descript_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, me->attr.outboundTdPoint);
        inboundTc  = (ca_omci_traffic_descript_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, me->attr.inboundTdPoint);
        OMCI_CHECK_PARAM_CNU(NULL == macBrgSrv);
        // no need to check these 2 is null or not
        OMCI_ME_LINK(me, inboundTc);
        OMCI_ME_LINK(me, outboundTc);

        if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == me->attr.tpType)
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, pptpEth);
        }
        else if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == me->attr.tpType)
        {
            dot1p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == dot1p);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, dot1p);
        }
        else if (MAC_PORT_CFG_TP_TYPE_IPHOST == me->attr.tpType)
        {
            //may add iphost v6 me here
            iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == iphost);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, iphost);
        }
        else if (MAC_PORT_CFG_TP_TYPE_GEM_IWTP == me->attr.tpType)
        {
            gemIwTp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == gemIwTp);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, gemIwTp);
        }
        else if (MAC_PORT_CFG_TP_TYPE_MULTI_GEM == me->attr.tpType)
        {
            multiGem = (ca_omci_multi_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == multiGem);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, multiGem);
        }
        else if (MAC_PORT_CFG_TP_TYPE_VEIP == me->attr.tpType)
        {
            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, me->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == veip);
            OMCI_ME_LINK(me, macBrgSrv);
            OMCI_ME_LINK(me, veip);
        }
        else
        {
            OMCIWARN("unsupport tp type [%d]", me->attr.tpType);
        }
    }
}

