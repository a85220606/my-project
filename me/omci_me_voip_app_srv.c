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
#include "omci_me_voip_app_srv.h"
#include "omci_me_large_string.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_voip_app_srv_handlers = {0};


ca_omci_me_attr_t g_omci_me_voip_app_srv_attr[OMCI_ME_ATTR8] =
{
    {OMCI_ME_ATTR1, "cid_feature"},
    {OMCI_ME_ATTR2, "call_wait_feature"},
    {OMCI_ME_ATTR3, "call_prog_feature"},
    {OMCI_ME_ATTR4, "call_pres_feature"},
    {OMCI_ME_ATTR5, "dire_conn_featurn"},
    {OMCI_ME_ATTR6, "dire_conn_up"},
    {OMCI_ME_ATTR7, "bri_line_agt_up"},
    {OMCI_ME_ATTR8, "conf_fact_up"},
};


ca_omci_uint32_t ca_omci_me_voip_app_srv_attr_len_get(void)
{
    ca_omci_voip_app_srv_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.cidFeature);
    attrLen += sizeof(me->attr.callWaitFeature);
    attrLen += sizeof(me->attr.callProgFeature);
    attrLen += sizeof(me->attr.callPresFeature);
    attrLen += sizeof(me->attr.direConnFeaturn);
    attrLen += sizeof(me->attr.direConnUp);
    attrLen += sizeof(me->attr.briLineAgtUp);
    attrLen += sizeof(me->attr.confFactUp);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_voip_app_srv_new(void)
{
    ca_omci_voip_app_srv_t *me = (ca_omci_voip_app_srv_t *)omci_malloc(sizeof(ca_omci_voip_app_srv_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_VOIP_APP_SRV_CLASS_ID, &sg_omci_me_voip_app_srv_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(voip_app_srv);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_voip_app_srv_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_voip_app_srv_t *voipAppSrv = (ca_omci_voip_app_srv_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(voipAppSrv->attr.cidFeature, "cidFeature", 1);
    OMCI_SET_ATTR_UINT8(voipAppSrv->attr.callWaitFeature, "callWaitFeature", 2);
    OMCI_SET_ATTR_UINT16(voipAppSrv->attr.callProgFeature, "callProgFeature", 3);
    OMCI_SET_ATTR_UINT16(voipAppSrv->attr.callPresFeature, "callPresFeature", 4);
    OMCI_SET_ATTR_UINT8(voipAppSrv->attr.direConnFeaturn, "direConnFeaturn", 5);
    OMCI_SET_ATTR_UINT16(voipAppSrv->attr.direConnUp, "direConnUp", 6);
    OMCI_SET_ATTR_UINT16(voipAppSrv->attr.briLineAgtUp, "briLineAgtUp", 7);
    OMCI_SET_ATTR_UINT16(voipAppSrv->attr.confFactUp, "confFactUp", 8);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_app_srv_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_voip_app_srv_t *voipAppSrv = (ca_omci_voip_app_srv_t *)me;
    ca_omci_uint16_t  offset      = 0;
    ca_omci_uint16_t  mask        = 0;
    ca_omci_mebase_t *direConnUri = NULL;
    ca_omci_mebase_t *briAgtUri   = NULL;
    ca_omci_mebase_t *conFacUri   = NULL;


    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_VOIP_APP_SRV_SET_MASK_FULL);

    //remove old link here
    direConnUri = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.direConnUp);
    briAgtUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.briLineAgtUp);
    conFacUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.confFactUp);
    OMCI_RM_LINK_ME(voipAppSrv, direConnUri);
    OMCI_RM_LINK_ME(voipAppSrv, briAgtUri);
    OMCI_RM_LINK_ME(voipAppSrv, conFacUri);

    OMCI_UPDATE_ATTR_UINT(8, voipAppSrv->attr.cidFeature, "cidFeature", 1);
    OMCI_UPDATE_ATTR_UINT(8, voipAppSrv->attr.callWaitFeature, "callWaitFeature", 2);
    OMCI_UPDATE_ATTR_UINT(16, voipAppSrv->attr.callProgFeature, "callProgFeature", 3);
    OMCI_UPDATE_ATTR_UINT(16, voipAppSrv->attr.callPresFeature, "callPresFeature", 4);
    OMCI_UPDATE_ATTR_UINT(8, voipAppSrv->attr.direConnFeaturn, "direConnFeaturn", 5);
    OMCI_UPDATE_ATTR_UINT(16, voipAppSrv->attr.direConnUp, "direConnUp", 6);
    OMCI_UPDATE_ATTR_UINT(16, voipAppSrv->attr.briLineAgtUp, "briLineAgtUp", 7);
    OMCI_UPDATE_ATTR_UINT(16, voipAppSrv->attr.confFactUp, "confFactUp", 8);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_app_srv_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_voip_app_srv_t *voipAppSrv = (ca_omci_voip_app_srv_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_VOIP_APP_SRV_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(voipAppSrv->attr.cidFeature, 1);
    OMCI_GET_ATTR_UINT8(voipAppSrv->attr.callWaitFeature, 2);
    OMCI_GET_ATTR_UINT16(voipAppSrv->attr.callProgFeature, 3);
    OMCI_GET_ATTR_UINT16(voipAppSrv->attr.callPresFeature, 4);
    OMCI_GET_ATTR_UINT8(voipAppSrv->attr.direConnFeaturn, 5);
    OMCI_GET_ATTR_UINT16(voipAppSrv->attr.direConnUp, 6);
    OMCI_GET_ATTR_UINT16(voipAppSrv->attr.briLineAgtUp, 7);
    OMCI_GET_ATTR_UINT16(voipAppSrv->attr.confFactUp, 8);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_app_srv_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_voip_app_srv_t *voipAppSrv  = (ca_omci_voip_app_srv_t *)me;
    ca_omci_mebase_t       *direConnUri = NULL;
    ca_omci_mebase_t       *briAgtUri   = NULL;
    ca_omci_mebase_t       *conFacUri   = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    direConnUri = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.direConnUp);
    briAgtUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.briLineAgtUp);
    conFacUri   = ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, voipAppSrv->attr.confFactUp);

    OMCI_RM_LINK_ME(voipAppSrv, direConnUri);
    OMCI_RM_LINK_ME(voipAppSrv, briAgtUri);
    OMCI_RM_LINK_ME(voipAppSrv, conFacUri);

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_app_srv_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_voip_app_srv_t *voipAppSrv = (ca_omci_voip_app_srv_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(voipAppSrv);
    OMCI_UPLOAD_ATTR_SIZE(8, voipAppSrv->attr.cidFeature, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, voipAppSrv->attr.callWaitFeature, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, voipAppSrv->attr.callProgFeature, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, voipAppSrv->attr.callPresFeature, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, voipAppSrv->attr.direConnFeaturn, 5);
    OMCI_UPLOAD_ATTR_SIZE(16, voipAppSrv->attr.direConnUp, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, voipAppSrv->attr.briLineAgtUp, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, voipAppSrv->attr.confFactUp, 8);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_voip_app_srv_init(void)
{
    sg_omci_me_voip_app_srv_handlers.uploadProc = ca_omci_me_voip_app_srv_upload;

    sg_omci_me_voip_app_srv_handlers.createProc = ca_omci_me_voip_app_srv_create;
    sg_omci_me_voip_app_srv_handlers.deleteProc = ca_omci_me_voip_app_srv_delete;
    sg_omci_me_voip_app_srv_handlers.setProc    = ca_omci_me_voip_app_srv_set;
    sg_omci_me_voip_app_srv_handlers.getProc    = ca_omci_me_voip_app_srv_get;
}


void ca_omci_me_voip_app_srv_link(void)
{
    ca_omci_meclass_t      *meClass     = NULL;
    ca_omci_mebase_t       *meInst      = NULL;
    ca_omci_voip_app_srv_t *me          = NULL;
    ca_omci_large_string_t *direConnUri = NULL;
    ca_omci_large_string_t *briAgtUri   = NULL;
    ca_omci_large_string_t *conFacUri   = NULL;

    meClass = ca_omci_me_get(OMCI_ME_VOIP_APP_SRV_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me          = (ca_omci_voip_app_srv_t *)meInst;
        direConnUri = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.direConnUp);
        briAgtUri   = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.briLineAgtUp);
        conFacUri   = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, me->attr.confFactUp);

        OMCI_ME_LINK(me, direConnUri);
        OMCI_ME_LINK(me, briAgtUri);
        OMCI_ME_LINK(me, conFacUri);
    }
}

