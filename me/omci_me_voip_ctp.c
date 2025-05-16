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
#include "omci_me_voip_ctp.h"
#include "omci_me_voip_cfg.h"
#include "omci_me_sip_usr.h"
#include "omci_me_mgc_cfg_data.h"
#include "omci_me_pptp_pots.h"
#include "omci_me_voip_media_prof.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_voip_ctp_handlers = {0};


ca_omci_me_attr_t g_omci_me_voip_ctp_attr[OMCI_ME_ATTR4] =
{
    {OMCI_ME_ATTR1, "user_pro_pointer"},
    {OMCI_ME_ATTR2, "pptp_pointer"},
    {OMCI_ME_ATTR3, "media_prof_point"},
    {OMCI_ME_ATTR4, "signal_code"},
};


ca_omci_uint32_t ca_omci_me_voip_ctp_attr_len_get(void)
{
    ca_omci_voip_ctp_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.userProPointer);
    attrLen += sizeof(me->attr.pptpPointer);
    attrLen += sizeof(me->attr.mediaProfPoint);
    attrLen += sizeof(me->attr.signalCode);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_voip_ctp_new(void)
{
    ca_omci_voip_ctp_t *me = (ca_omci_voip_ctp_t *)omci_malloc(sizeof(ca_omci_voip_ctp_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_VOIP_CTP_CLASS_ID, &sg_omci_me_voip_ctp_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(voip_ctp);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_voip_ctp_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_voip_ctp_t *voipCtp = (ca_omci_voip_ctp_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(voipCtp->attr.userProPointer, "userProPointer", 1);
    OMCI_SET_ATTR_UINT16(voipCtp->attr.pptpPointer, "pptpPointer", 2);
    OMCI_SET_ATTR_UINT16(voipCtp->attr.mediaProfPoint, "mediaProfPoint", 3);
    OMCI_SET_ATTR_UINT8(voipCtp->attr.signalCode, "signalCode", 4);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_ctp_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_voip_ctp_t *voipCtp   = (ca_omci_voip_ctp_t *)me;
    ca_omci_uint16_t    offset    = 0;
    ca_omci_uint16_t    mask      = 0;
    ca_omci_voip_cfg_t *voipCfg   = NULL;
    ca_omci_mebase_t   *sipCfg    = NULL;
    ca_omci_mebase_t   *mgcCfg    = NULL;
    ca_omci_mebase_t   *pptpPots  = NULL;
    ca_omci_mebase_t   *mediaProf = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_VOIP_CTP_SET_MASK_FULL);

    pptpPots  = ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, voipCtp->attr.pptpPointer);
    mediaProf = ca_omci_me_entry_lookup(OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID, voipCtp->attr.mediaProfPoint);
    OMCI_RM_LINK_ME(voipCtp, pptpPots);
    OMCI_RM_LINK_ME(voipCtp, mediaProf);

    voipCfg = (ca_omci_voip_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_CFG_CLASS_ID, 0);
    if (NULL != voipCfg)
    {
        if (VOIP_CFG_USED_PROTO_SIP == voipCfg->attr.sigProtoUsed)
        {
            sipCfg = ca_omci_me_entry_lookup(OMCI_ME_SIP_USR_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_RM_LINK_ME(voipCtp, sipCfg);
        }
        else if (VOIP_CFG_USED_PROTO_H248 == voipCfg->attr.sigProtoUsed)
        {
            mgcCfg = ca_omci_me_entry_lookup(OMCI_ME_MGC_CFG_DATA_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_RM_LINK_ME(voipCtp, mgcCfg);
        }
    }

    OMCI_UPDATE_ATTR_UINT(16, voipCtp->attr.userProPointer, "userProPointer", 1);
    OMCI_UPDATE_ATTR_UINT(16, voipCtp->attr.pptpPointer, "pptpPointer", 2);
    OMCI_UPDATE_ATTR_UINT(16, voipCtp->attr.mediaProfPoint, "mediaProfPoint", 3);
    OMCI_UPDATE_ATTR_UINT(8, voipCtp->attr.signalCode, "signalCode", 4);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_ctp_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_voip_ctp_t *voipCtp = (ca_omci_voip_ctp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_VOIP_CTP_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(voipCtp->attr.userProPointer, 1);
    OMCI_GET_ATTR_UINT16(voipCtp->attr.pptpPointer, 2);
    OMCI_GET_ATTR_UINT16(voipCtp->attr.mediaProfPoint, 3);
    OMCI_GET_ATTR_UINT8(voipCtp->attr.signalCode, 4);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_ctp_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_voip_ctp_t *voipCtp   = (ca_omci_voip_ctp_t *)me;
    ca_omci_voip_cfg_t *voipCfg   = NULL;
    ca_omci_mebase_t   *sipCfg    = NULL;
    ca_omci_mebase_t   *mgcCfg    = NULL;
    ca_omci_mebase_t   *pptpPots  = NULL;
    ca_omci_mebase_t   *mediaProf = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    (void)result;

    pptpPots  = ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, voipCtp->attr.pptpPointer);
    mediaProf = ca_omci_me_entry_lookup(OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID, voipCtp->attr.mediaProfPoint);
    OMCI_RM_LINK_ME(voipCtp, pptpPots);
    OMCI_RM_LINK_ME(voipCtp, mediaProf);

    voipCfg = (ca_omci_voip_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_CFG_CLASS_ID, 0);
    if (NULL != voipCfg)
    {
        if (VOIP_CFG_USED_PROTO_SIP == voipCfg->attr.sigProtoUsed)
        {
            sipCfg = ca_omci_me_entry_lookup(OMCI_ME_SIP_USR_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_RM_LINK_ME(voipCtp, sipCfg);
        }
        else if (VOIP_CFG_USED_PROTO_H248 == voipCfg->attr.sigProtoUsed)
        {
            mgcCfg = ca_omci_me_entry_lookup(OMCI_ME_MGC_CFG_DATA_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_RM_LINK_ME(voipCtp, mgcCfg);
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voip_ctp_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_voip_ctp_t *voipCtp = (ca_omci_voip_ctp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(voipCtp);
    OMCI_UPLOAD_ATTR_SIZE(16, voipCtp->attr.userProPointer, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, voipCtp->attr.pptpPointer, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, voipCtp->attr.mediaProfPoint, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, voipCtp->attr.signalCode, 4);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_voip_ctp_init(void)
{
    sg_omci_me_voip_ctp_handlers.uploadProc = ca_omci_me_voip_ctp_upload;

    sg_omci_me_voip_ctp_handlers.createProc = ca_omci_me_voip_ctp_create;
    sg_omci_me_voip_ctp_handlers.deleteProc = ca_omci_me_voip_ctp_delete;
    sg_omci_me_voip_ctp_handlers.setProc    = ca_omci_me_voip_ctp_set;
    sg_omci_me_voip_ctp_handlers.getProc    = ca_omci_me_voip_ctp_get;
}


void ca_omci_me_voip_ctp_link(void)
{
    ca_omci_meclass_t         *meClass   = NULL;
    ca_omci_mebase_t          *meInst    = NULL;
    ca_omci_voip_ctp_t        *voipCtp   = NULL;
    ca_omci_voip_cfg_t        *voipCfg   = NULL;
    ca_omci_sip_usr_t         *sipCfg    = NULL;
    ca_omci_mgc_cfg_data_t    *mgcCfg    = NULL;
    ca_omci_pptp_pots_t       *pptpPots  = NULL;
    ca_omci_voip_media_prof_t *mediaProf = NULL;

    meClass = ca_omci_me_get(OMCI_ME_VOIP_CTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    voipCfg = (ca_omci_voip_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_CFG_CLASS_ID, 0);

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        voipCtp = (ca_omci_voip_ctp_t *)meInst;

        pptpPots  = (ca_omci_pptp_pots_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, voipCtp->attr.pptpPointer);
        mediaProf = (ca_omci_voip_media_prof_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID, voipCtp->attr.mediaProfPoint);
        OMCI_ME_LINK(voipCtp, pptpPots);
        OMCI_ME_LINK(voipCtp, mediaProf);

        if (VOIP_CFG_USED_PROTO_SIP == voipCfg->attr.sigProtoUsed)
        {
            sipCfg = (ca_omci_sip_usr_t *)ca_omci_me_entry_lookup(OMCI_ME_SIP_USR_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_ME_LINK(voipCtp, sipCfg);
        }
        else if (VOIP_CFG_USED_PROTO_H248 == voipCfg->attr.sigProtoUsed)
        {
            mgcCfg = (ca_omci_mgc_cfg_data_t *)ca_omci_me_entry_lookup(OMCI_ME_MGC_CFG_DATA_CLASS_ID, voipCtp->attr.userProPointer);
            OMCI_ME_LINK(voipCtp, mgcCfg);
        }
    }
}

