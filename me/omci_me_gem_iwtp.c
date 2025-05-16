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
#include "omci_me_gem_iwtp.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_gal_eth_prof.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_mng_onuinfo.h"



static ca_omci_me_handler_t sg_omci_me_gem_iwtp_handlers = {0};


ca_omci_me_attr_t g_omci_me_gem_iwtp_attr[OMCI_ME_ATTR8] =
{
    {OMCI_ME_ATTR1, "gem_net_ctp_point"},
    {OMCI_ME_ATTR2, "iw_option"},
    {OMCI_ME_ATTR3, "srv_prof_point"},
    {OMCI_ME_ATTR4, "iwtp_point"},
    {OMCI_ME_ATTR5, "pptp_cnt"},
    {OMCI_ME_ATTR6, "oper_state"},
    {OMCI_ME_ATTR7, "gal_prof_point"},
    {OMCI_ME_ATTR8, "gal_loopback_cfg"},
};


ca_omci_uint32_t ca_omci_me_gem_iwtp_attr_len_get(void)
{
    ca_omci_gem_iwtp_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.gemNetCtpPoint);
    attrLen += sizeof(me->attr.iwOption);
    attrLen += sizeof(me->attr.srvProfPoint);
    attrLen += sizeof(me->attr.iwtpPoint);
    attrLen += sizeof(me->attr.pptpCnt);
    attrLen += sizeof(me->attr.operState);
    attrLen += sizeof(me->attr.galProfPoint);
    attrLen += sizeof(me->attr.galLoopbackCfg);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_gem_iwtp_new(void)
{
    ca_omci_gem_iwtp_t *me = (ca_omci_gem_iwtp_t *)omci_malloc(sizeof(ca_omci_gem_iwtp_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_GEM_IWTP_CLASS_ID, &sg_omci_me_gem_iwtp_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(gem_iwtp);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_gem_iwtp_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_gem_iwtp_t *gemIwtp = (ca_omci_gem_iwtp_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(gemIwtp->attr.gemNetCtpPoint, "gemNetCtpPoint", 1);
    OMCI_SET_ATTR_UINT8(gemIwtp->attr.iwOption, "iwOption", 2);
    OMCI_SET_ATTR_UINT16(gemIwtp->attr.srvProfPoint, "srvProfPoint", 3);
    OMCI_SET_ATTR_UINT16(gemIwtp->attr.iwtpPoint, "iwtpPoint", 4);
    OMCI_SET_ATTR_UINT16(gemIwtp->attr.galProfPoint, "galProfPoint", 7);

    if ((GEM_IWTP_IW_OPTION_MAC_BRG_LAN != gemIwtp->attr.iwOption)
     && (GEM_IWTP_IW_OPTION_8021P != gemIwtp->attr.iwOption)
     && (GEM_IWTP_IW_OPTION_DS_BROADCAST != gemIwtp->attr.iwOption))
    {
        OMCIWARN("unsupport service profile type [%d]", gemIwtp->attr.iwOption);
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_iwtp_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_gem_iwtp_t *gemIwtp = (ca_omci_gem_iwtp_t *)me;
    ca_omci_uint16_t  offset    = 0;
    ca_omci_uint16_t  mask      = 0;
    ca_omci_mebase_t *gemNetCtp = NULL;
    ca_omci_mebase_t *dot1p     = NULL;
    ca_omci_mebase_t *macBrgSrv = NULL;
    ca_omci_mebase_t *pptpEth   = NULL;
    ca_omci_mebase_t *galEth    = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_GEM_IWTP_SET_MASK_FULL);

    //remove old link first, then update attribute value
    gemNetCtp = ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemIwtp->attr.gemNetCtpPoint);
    galEth    = ca_omci_me_entry_lookup(OMCI_ME_GAL_ETH_PROF_CLASS_ID, gemIwtp->attr.galProfPoint);
    pptpEth   = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, gemIwtp->attr.iwtpPoint);
    OMCI_RM_LINK_ME(gemIwtp, gemNetCtp);
    OMCI_RM_LINK_ME(gemIwtp, galEth);
    OMCI_RM_LINK_ME(gemIwtp, pptpEth);

    if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemIwtp->attr.iwOption)
    {
        macBrgSrv = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(gemIwtp, macBrgSrv);
    }

    if (GEM_IWTP_IW_OPTION_8021P == gemIwtp->attr.iwOption)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(gemIwtp, dot1p);
    }

    OMCI_UPDATE_ATTR_UINT(16, gemIwtp->attr.gemNetCtpPoint, "gemNetCtpPoint", 1);
    OMCI_UPDATE_ATTR_UINT(8,  gemIwtp->attr.iwOption, "iwOption", 2);
    OMCI_UPDATE_ATTR_UINT(16, gemIwtp->attr.srvProfPoint, "srvProfPoint", 3);
    OMCI_UPDATE_ATTR_UINT(16, gemIwtp->attr.iwtpPoint, "iwtpPoint", 4);
    OMCI_UPDATE_ATTR_UINT(16, gemIwtp->attr.galProfPoint, "galProfPoint", 7);
    OMCI_UPDATE_ATTR_UINT(8, gemIwtp->attr.galLoopbackCfg, "galLoopbackCfg", 8);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_iwtp_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_gem_iwtp_t *gemIwtp = (ca_omci_gem_iwtp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_GEM_IWTP_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(gemIwtp->attr.gemNetCtpPoint, 1);
    OMCI_GET_ATTR_UINT8(gemIwtp->attr.iwOption, 2);
    OMCI_GET_ATTR_UINT16(gemIwtp->attr.srvProfPoint, 3);
    OMCI_GET_ATTR_UINT16(gemIwtp->attr.iwtpPoint, 4);
    OMCI_GET_ATTR_UINT8(gemIwtp->attr.pptpCnt, 5);
    OMCI_GET_ATTR_UINT8(gemIwtp->attr.operState, 6);
    OMCI_GET_ATTR_UINT16(gemIwtp->attr.galProfPoint, 7);
    OMCI_GET_ATTR_UINT8(gemIwtp->attr.galLoopbackCfg, 8);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_iwtp_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_gem_iwtp_t *gemIwtp   = (ca_omci_gem_iwtp_t *)me;
    ca_omci_mebase_t   *gemNetCtp = NULL;
    ca_omci_mebase_t   *dot1p     = NULL;
    ca_omci_mebase_t   *macBrgSrv = NULL;
    ca_omci_mebase_t   *pptpEth   = NULL;
    ca_omci_mebase_t   *galEth    = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    gemNetCtp = ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemIwtp->attr.gemNetCtpPoint);
    galEth    = ca_omci_me_entry_lookup(OMCI_ME_GAL_ETH_PROF_CLASS_ID, gemIwtp->attr.galProfPoint);
    pptpEth   = ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, gemIwtp->attr.iwtpPoint);
    OMCI_RM_LINK_ME(gemIwtp, gemNetCtp);
    OMCI_RM_LINK_ME(gemIwtp, galEth);
    OMCI_RM_LINK_ME(gemIwtp, pptpEth);

    if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemIwtp->attr.iwOption)
    {
        macBrgSrv = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(gemIwtp, macBrgSrv);
    }

    if (GEM_IWTP_IW_OPTION_8021P == gemIwtp->attr.iwOption)
    {
        dot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
        OMCI_RM_LINK_ME(gemIwtp, dot1p);
    }
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_iwtp_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_gem_iwtp_t *gemIwtp = (ca_omci_gem_iwtp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(gemIwtp);
    OMCI_UPLOAD_ATTR_SIZE(16, gemIwtp->attr.gemNetCtpPoint, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, gemIwtp->attr.iwOption, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, gemIwtp->attr.srvProfPoint, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, gemIwtp->attr.iwtpPoint, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, gemIwtp->attr.pptpCnt, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, gemIwtp->attr.operState, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, gemIwtp->attr.galProfPoint, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, gemIwtp->attr.galLoopbackCfg, 8);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_gem_iwtp_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_gem_iwtp_t *gemIwtp = (ca_omci_gem_iwtp_t *)me;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: gemIwtp \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        gemNetCtpPoint: 0x%x\n", gemIwtp->attr.gemNetCtpPoint);
    omci_sprint(cli, "        iwOption:    0x%x\n", gemIwtp->attr.iwOption);
    omci_sprint(cli, "        srvProfPoint: 0x%x\n", gemIwtp->attr.srvProfPoint);
    omci_sprint(cli, "        iwtpPoint:   0x%x\n", gemIwtp->attr.iwtpPoint);
    omci_sprint(cli, "        pptpCnt:     0x%x\n", gemIwtp->attr.pptpCnt);
    omci_sprint(cli, "        operState:   0x%x\n", gemIwtp->attr.operState);
    omci_sprint(cli, "        galProfPoint: 0x%x\n", gemIwtp->attr.galProfPoint);
    omci_sprint(cli, "        galLoopbackCfg: 0x%x\n", gemIwtp->attr.galLoopbackCfg);
}


void ca_omci_me_gem_iwtp_init(void)
{
    sg_omci_me_gem_iwtp_handlers.meDisplay  = ca_omci_me_gem_iwtp_display;
    sg_omci_me_gem_iwtp_handlers.uploadProc = ca_omci_me_gem_iwtp_upload;

    sg_omci_me_gem_iwtp_handlers.createProc = ca_omci_me_gem_iwtp_create;
    sg_omci_me_gem_iwtp_handlers.deleteProc = ca_omci_me_gem_iwtp_delete;
    sg_omci_me_gem_iwtp_handlers.setProc    = ca_omci_me_gem_iwtp_set;
    sg_omci_me_gem_iwtp_handlers.getProc    = ca_omci_me_gem_iwtp_get;
}


void ca_omci_me_gem_iwtp_link(void)
{
    ca_omci_meclass_t          *meClass    = NULL;
    ca_omci_mebase_t           *meInst     = NULL;
    ca_omci_gem_iwtp_t         *gemIwtp    = NULL;
    ca_omci_gem_net_ctp_t      *gemNetCtp  = NULL;
    ca_omci_ieee_dot1p_srv_t   *dot1p      = NULL;
    ca_omci_mac_brg_srv_t      *macBrgSrv  = NULL;
    ca_omci_pptp_eth_t         *pptpEth    = NULL;
    ca_omci_gal_eth_prof_t     *galEth     = NULL;
    ca_omci_onuinfo_obj_t      *onuInfo    = ca_omci_onuinfo_get();

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_GEM_IWTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no gemport is created now , flow is not complete");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        gemIwtp   = (ca_omci_gem_iwtp_t *)meInst;
        gemNetCtp = NULL;
        galEth    = NULL;
        macBrgSrv = NULL;
        dot1p     = NULL;
        pptpEth   = NULL;

        if ((GEM_IWTP_IW_OPTION_MAC_BRG_LAN != gemIwtp->attr.iwOption)
         && (GEM_IWTP_IW_OPTION_8021P != gemIwtp->attr.iwOption)
         && (GEM_IWTP_IW_OPTION_DS_BROADCAST != gemIwtp->attr.iwOption))
        {
            OMCIWARN("unsupport service profile type [%d]", gemIwtp->attr.iwOption);
        }

        gemNetCtp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemIwtp->attr.gemNetCtpPoint);
        galEth    = (ca_omci_gal_eth_prof_t *)ca_omci_me_entry_lookup(OMCI_ME_GAL_ETH_PROF_CLASS_ID, gemIwtp->attr.galProfPoint);
        OMCI_CHECK_PARAM_CNU(NULL == gemNetCtp);
        if ((GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemIwtp->attr.iwOption)
         || (GEM_IWTP_IW_OPTION_VIDEO == gemIwtp->attr.iwOption)
         || (GEM_IWTP_IW_OPTION_8021P == gemIwtp->attr.iwOption))
        {
            //some tr247 case create wrong gal eth profile value
            if (OMCI_OLT_TYPE_SIM != onuInfo->oltType)
                OMCI_CHECK_PARAM_CNU(NULL == galEth);
        }

        if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemIwtp->attr.iwOption)
        {
            macBrgSrv = (ca_omci_mac_brg_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
            OMCI_CHECK_PARAM_CNU(NULL == macBrgSrv);
        }

        if (GEM_IWTP_IW_OPTION_8021P == gemIwtp->attr.iwOption)
        {
            dot1p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemIwtp->attr.srvProfPoint);
            OMCI_CHECK_PARAM_CNU(NULL == dot1p);
        }

        if ((0 != gemIwtp->attr.iwtpPoint) && (0xffff != gemIwtp->attr.iwtpPoint))
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, gemIwtp->attr.iwtpPoint);
            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
        }

        OMCI_ME_LINK(gemIwtp, gemNetCtp);
        OMCI_ME_LINK(gemIwtp, macBrgSrv);
        OMCI_ME_LINK(gemIwtp, dot1p);
        OMCI_ME_LINK(gemIwtp, pptpEth);
        OMCI_ME_LINK(gemIwtp, galEth);
    }
}

