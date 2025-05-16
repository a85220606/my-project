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
#include "omci_me_sip_usr.h"
#include "omci_me_sip_agt_cfg.h"
#include "omci_me_large_string.h"
#include "omci_me_auth_scy.h"
#include "omci_me_net_addr.h"
#include "omci_me_net_dial_plan.h"
#include "omci_me_voip_app_srv.h"
#include "omci_me_voip_acces_code.h"
#include "omci_me_pptp_pots.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_sip_usr_handlers = {0};


ca_omci_me_attr_t g_omci_me_sip_usr_attr[OMCI_ME_ATTR13] =
{
    {OMCI_ME_ATTR1,  "sip_agt_point"},
    {OMCI_ME_ATTR2,  "usr_part_aor"},
    {OMCI_ME_ATTR3,  "sip_display_name"},
    {OMCI_ME_ATTR4,  "usr_pwd_point"},
    {OMCI_ME_ATTR5,  "mail_srv_sip_url"},
    {OMCI_ME_ATTR6,  "mail_subscr_exp_time"},
    {OMCI_ME_ATTR7,  "new_dial_plan_point"},
    {OMCI_ME_ATTR8,  "app_srv_prof_point"},
    {OMCI_ME_ATTR9,  "feature_code_point"},
    {OMCI_ME_ATTR10, "pptp_point"},
    {OMCI_ME_ATTR11, "release_timer"},
    {OMCI_ME_ATTR12, "roh_timer"},
    {OMCI_ME_ATTR13, "mali_com_id_fail"},
};


ca_omci_uint32_t ca_omci_me_sip_usr_attr_len_get(void)
{
    ca_omci_sip_usr_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.sipAgtPoint);
    attrLen += sizeof(me->attr.usrPartAor);
    attrLen += sizeof(me->attr.sipDisplayName);
    attrLen += sizeof(me->attr.usrPwdPoint);
    attrLen += sizeof(me->attr.mailSrvSipUrl);
    attrLen += sizeof(me->attr.mailSubscrExpTime);
    attrLen += sizeof(me->attr.newDialPlanPoint);
    attrLen += sizeof(me->attr.appArvProfPoint);
    attrLen += sizeof(me->attr.featureCodePoint);
    attrLen += sizeof(me->attr.pptpPoint);
    attrLen += sizeof(me->attr.releaseTimer);
    attrLen += sizeof(me->attr.rohTimer);
    attrLen += sizeof(me->attr.maliComIdFail);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_sip_usr_new(void)
{
    ca_omci_sip_usr_t *me = (ca_omci_sip_usr_t *)omci_malloc(sizeof(ca_omci_sip_usr_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_SIP_USR_CLASS_ID, &sg_omci_me_sip_usr_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(sip_usr);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_sip_usr_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_sip_usr_t *sipUsr = (ca_omci_sip_usr_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(sipUsr->attr.sipAgtPoint, "sipAgtPoint", 1);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.usrPartAor, "usrPartAor", 2);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.usrPwdPoint, "usrPwdPoint", 4);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.mailSrvSipUrl, "mailSrvSipUrl", 5);
    OMCI_SET_ATTR_UINT32(sipUsr->attr.mailSubscrExpTime, "mailSubscrExpTime", 6);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.newDialPlanPoint, "newDialPlanPoint", 7);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.appArvProfPoint, "appArvProfPoint", 8);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.featureCodePoint, "featureCodePoint", 9);
    OMCI_SET_ATTR_UINT16(sipUsr->attr.pptpPoint, "pptpPoint", 10);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_usr_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_sip_usr_t *sipUsr = (ca_omci_sip_usr_t *)me;
    ca_omci_uint16_t  offset        = 0;
    ca_omci_uint16_t  mask          = 0;
    ca_omci_uint8_t   idx           = 0;
    ca_omci_mebase_t *sipAgtCfg     = NULL;
    ca_omci_mebase_t *largeStr      = NULL;
    ca_omci_mebase_t *authScy       = NULL;
    ca_omci_mebase_t *netAddr       = NULL;
    ca_omci_mebase_t *netDialPlan   = NULL;
    ca_omci_mebase_t *voipAppSrv    = NULL;
    ca_omci_mebase_t *voipAccesCode = NULL;
    ca_omci_mebase_t *pptpPots      = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_SIP_USR_SET_MASK_FULL);

    //remove old link here
    sipAgtCfg     = ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, sipUsr->attr.sipAgtPoint);
    largeStr      = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipUsr->attr.usrPartAor);
    authScy       = ca_omci_me_entry_lookup(OMCI_ME_AUTH_SCY_CLASS_ID, sipUsr->attr.usrPwdPoint);
    netAddr       = ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, sipUsr->attr.mailSrvSipUrl);
    netDialPlan   = ca_omci_me_entry_lookup(OMCI_ME_NET_DIAL_PLAN_CLASS_ID, sipUsr->attr.newDialPlanPoint);
    voipAppSrv    = ca_omci_me_entry_lookup(OMCI_ME_VOIP_APP_SRV_CLASS_ID, sipUsr->attr.appArvProfPoint);
    voipAccesCode = ca_omci_me_entry_lookup(OMCI_ME_VOIP_ACCES_CODE_CLASS_ID, sipUsr->attr.featureCodePoint);
    pptpPots      = ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, sipUsr->attr.pptpPoint);
    OMCI_RM_LINK_ME(sipUsr, sipAgtCfg);
    OMCI_RM_LINK_ME(sipUsr, largeStr);
    OMCI_RM_LINK_ME(sipUsr, authScy);
    OMCI_RM_LINK_ME(sipUsr, netAddr);
    OMCI_RM_LINK_ME(sipUsr, netDialPlan);
    OMCI_RM_LINK_ME(sipUsr, voipAppSrv);
    OMCI_RM_LINK_ME(sipUsr, voipAccesCode);
    OMCI_RM_LINK_ME(sipUsr, pptpPots);

    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.sipAgtPoint, "sipAgtPoint", 1);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.usrPartAor, "usrPartAor", 2);
    OMCI_UPDATE_ATTR_UINT_T(8, sipUsr->attr.sipDisplayName, "sipDisplayName", OMCI_SIP_USR_SIP_DISPLAY_NAME, 3);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.usrPwdPoint, "usrPwdPoint", 4);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.mailSrvSipUrl, "mailSrvSipUrl", 5);
    OMCI_UPDATE_ATTR_UINT(32, sipUsr->attr.mailSubscrExpTime, "mailSubscrExpTime", 6);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.newDialPlanPoint, "newDialPlanPoint", 7);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.appArvProfPoint, "appArvProfPoint", 8);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.featureCodePoint, "featureCodePoint", 9);
    OMCI_UPDATE_ATTR_UINT(16, sipUsr->attr.pptpPoint, "pptpPoint", 10);
    OMCI_UPDATE_ATTR_UINT(8, sipUsr->attr.releaseTimer, "releaseTimer", 11);
    OMCI_UPDATE_ATTR_UINT(8, sipUsr->attr.rohTimer, "rohTimer", 12);
    OMCI_UPDATE_ATTR_UINT(8, sipUsr->attr.maliComIdFail, "maliComIdFail", 13);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_usr_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_sip_usr_t *sipUsr = (ca_omci_sip_usr_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_SIP_USR_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(sipUsr->attr.sipAgtPoint, 1);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.usrPartAor, 2);
    OMCI_GET_ATTR_UINT8_T(sipUsr->attr.sipDisplayName, OMCI_SIP_USR_SIP_DISPLAY_NAME, 3);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.usrPwdPoint, 4);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.mailSrvSipUrl, 5);
    OMCI_GET_ATTR_UINT32(sipUsr->attr.mailSubscrExpTime, 6);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.newDialPlanPoint, 7);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.appArvProfPoint, 8);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.featureCodePoint, 9);
    OMCI_GET_ATTR_UINT16(sipUsr->attr.pptpPoint, 10);
    OMCI_GET_ATTR_UINT8(sipUsr->attr.releaseTimer, 11);
    OMCI_GET_ATTR_UINT8(sipUsr->attr.rohTimer, 12);
    OMCI_GET_ATTR_UINT8(sipUsr->attr.maliComIdFail, 13);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_usr_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_sip_usr_t *sipUsr        = (ca_omci_sip_usr_t *)me;
    ca_omci_mebase_t  *sipAgtCfg     = NULL;
    ca_omci_mebase_t  *largeStr      = NULL;
    ca_omci_mebase_t  *authScy       = NULL;
    ca_omci_mebase_t  *netAddr       = NULL;
    ca_omci_mebase_t  *netDialPlan   = NULL;
    ca_omci_mebase_t  *voipAppSrv    = NULL;
    ca_omci_mebase_t  *voipAccesCode = NULL;
    ca_omci_mebase_t  *pptpPots      = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    sipAgtCfg     = ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, sipUsr->attr.sipAgtPoint);
    largeStr      = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sipUsr->attr.usrPartAor);
    authScy       = ca_omci_me_entry_lookup(OMCI_ME_AUTH_SCY_CLASS_ID, sipUsr->attr.usrPwdPoint);
    netAddr       = ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, sipUsr->attr.mailSrvSipUrl);
    netDialPlan   = ca_omci_me_entry_lookup(OMCI_ME_NET_DIAL_PLAN_CLASS_ID, sipUsr->attr.newDialPlanPoint);
    voipAppSrv    = ca_omci_me_entry_lookup(OMCI_ME_VOIP_APP_SRV_CLASS_ID, sipUsr->attr.appArvProfPoint);
    voipAccesCode = ca_omci_me_entry_lookup(OMCI_ME_VOIP_ACCES_CODE_CLASS_ID, sipUsr->attr.featureCodePoint);
    pptpPots      = ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, sipUsr->attr.pptpPoint);

    OMCI_RM_LINK_ME(sipUsr, sipAgtCfg);
    OMCI_RM_LINK_ME(sipUsr, largeStr);
    OMCI_RM_LINK_ME(sipUsr, authScy);
    OMCI_RM_LINK_ME(sipUsr, netAddr);
    OMCI_RM_LINK_ME(sipUsr, netDialPlan);
    OMCI_RM_LINK_ME(sipUsr, voipAppSrv);
    OMCI_RM_LINK_ME(sipUsr, voipAccesCode);
    OMCI_RM_LINK_ME(sipUsr, pptpPots);

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_sip_usr_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_sip_usr_t *sipUsr = (ca_omci_sip_usr_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(sipUsr);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.sipAgtPoint, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.usrPartAor, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, sipUsr->attr.sipDisplayName, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.usrPwdPoint, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.mailSrvSipUrl, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, sipUsr->attr.mailSubscrExpTime, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.newDialPlanPoint, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.appArvProfPoint, 8);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.featureCodePoint, 9);
    OMCI_UPLOAD_ATTR_SIZE(16, sipUsr->attr.pptpPoint, 10);
    OMCI_UPLOAD_ATTR_SIZE(8, sipUsr->attr.releaseTimer, 11);
    OMCI_UPLOAD_ATTR_SIZE(8, sipUsr->attr.rohTimer, 12);
    OMCI_UPLOAD_ATTR_SIZE(8, sipUsr->attr.maliComIdFail, 13);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_sip_usr_init(void)
{
    sg_omci_me_sip_usr_handlers.uploadProc = ca_omci_me_sip_usr_upload;

    sg_omci_me_sip_usr_handlers.createProc = ca_omci_me_sip_usr_create;
    sg_omci_me_sip_usr_handlers.deleteProc = ca_omci_me_sip_usr_delete;
    sg_omci_me_sip_usr_handlers.setProc    = ca_omci_me_sip_usr_set;
    sg_omci_me_sip_usr_handlers.getProc    = ca_omci_me_sip_usr_get;
}

void ca_omci_me_sip_usr_link(void)
{
    ca_omci_meclass_t         *meClass       = NULL;
    ca_omci_mebase_t          *meInst        = NULL;
    ca_omci_sip_usr_t         *me            = NULL;
    ca_omci_sip_agt_cfg_t     *sipAgtCfg     = NULL;
    ca_omci_large_string_t    *largeStr      = NULL;
    ca_omci_auth_scy_t        *authScy       = NULL;
    ca_omci_net_addr_t        *netAddr       = NULL;
    ca_omci_net_dial_plan_t   *netDialPlan   = NULL;
    ca_omci_voip_app_srv_t    *voipAppSrv    = NULL;
    ca_omci_voip_acces_code_t *voipAccesCode = NULL;
    ca_omci_pptp_pots_t       *pptpPots      = NULL;

    meClass = ca_omci_me_get(OMCI_ME_SIP_USR_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me            = (ca_omci_sip_usr_t *)meInst;
        sipAgtCfg     = (ca_omci_sip_agt_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_SIP_AGT_CFG_CLASS_ID, me->attr.sipAgtPoint);
        largeStr      = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.usrPartAor);
        authScy       = (ca_omci_auth_scy_t *)ca_omci_me_entry_lookup(OMCI_ME_AUTH_SCY_CLASS_ID, me->attr.usrPwdPoint);
        netAddr       = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, me->attr.mailSrvSipUrl);
        netDialPlan   = (ca_omci_net_dial_plan_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_DIAL_PLAN_CLASS_ID, me->attr.newDialPlanPoint);
        voipAppSrv    = (ca_omci_voip_app_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_APP_SRV_CLASS_ID, me->attr.appArvProfPoint);
        voipAccesCode = (ca_omci_voip_acces_code_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_ACCES_CODE_CLASS_ID, me->attr.featureCodePoint);
        pptpPots      = (ca_omci_pptp_pots_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, me->attr.pptpPoint);

        OMCI_ME_LINK(me, sipAgtCfg);
        OMCI_ME_LINK(me, largeStr);
        OMCI_ME_LINK(me, authScy);
        OMCI_ME_LINK(me, netAddr);
        OMCI_ME_LINK(me, netDialPlan);
        OMCI_ME_LINK(me, voipAppSrv);
        OMCI_ME_LINK(me, voipAccesCode);
        OMCI_ME_LINK(me, pptpPots);
    }
}

