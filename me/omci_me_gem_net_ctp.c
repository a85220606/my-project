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
#include "omci_me_gem_net_ctp.h"
#include "omci_me_onu_g.h"
#include "omci_me_tcont.h"
#include "omci_me_pri_queue.h"
#include "omci_me_traffic_descript.h"
#include "omci_mng_onuinfo.h"
#include "omci_mng_main.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_hal.h"


static ca_omci_me_handler_t sg_omci_me_gem_net_ctp_handlers = {0};


ca_omci_me_attr_t g_omci_me_gem_net_ctp_attr[OMCI_ME_ATTR10] =
{
    {OMCI_ME_ATTR1,  "port_id"},
    {OMCI_ME_ATTR2,  "tcont_point"},
    {OMCI_ME_ATTR3,  "direction"},
    {OMCI_ME_ATTR4,  "us_traf_mng_point"},
    {OMCI_ME_ATTR5,  "us_traf_desc_point"},
    {OMCI_ME_ATTR6,  "uni_cnt"},
    {OMCI_ME_ATTR7,  "ds_pq_point"},
    {OMCI_ME_ATTR8,  "encrypt_state"},
    {OMCI_ME_ATTR9,  "ds_traf_desc_point"},
    {OMCI_ME_ATTR10, "encrypt_key_ring"},
};


ca_omci_uint32_t ca_omci_me_gem_net_ctp_attr_len_get(void)
{
    ca_omci_gem_net_ctp_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.portId);
    attrLen += sizeof(me->attr.tcontPoint);
    attrLen += sizeof(me->attr.direction);
    attrLen += sizeof(me->attr.usTrafMngPoint);
    attrLen += sizeof(me->attr.usTrafDescPoint);
    attrLen += sizeof(me->attr.uniCnt);
    attrLen += sizeof(me->attr.dsPqPoint);
    attrLen += sizeof(me->attr.encryptState);
    attrLen += sizeof(me->attr.dsTrafMngPoint);
    attrLen += sizeof(me->attr.encryptKeyRing);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_gem_net_ctp_new(void)
{
    ca_omci_gem_net_ctp_t *me = (ca_omci_gem_net_ctp_t *)omci_malloc(sizeof(ca_omci_gem_net_ctp_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_GEM_NET_CTP_CLASS_ID, &sg_omci_me_gem_net_ctp_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(gem_net_ctp);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_gem_net_ctp_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.portId, "portId", 1);
    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.tcontPoint, "tcontPoint", 2);
    OMCI_SET_ATTR_UINT8(gemNetCtp->attr.direction, "direction", 3);
    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.usTrafMngPoint, "usTrafMngPoint", 4);
    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.usTrafDescPoint, "usTrafDescPoint", 5);
    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.dsPqPoint, "dsPqPoint", 7);
    OMCI_SET_ATTR_UINT16(gemNetCtp->attr.dsTrafMngPoint, "dsTrafMngPoint", 9);
    OMCI_SET_ATTR_UINT8(gemNetCtp->attr.encryptKeyRing, "encryptKeyRing", 10);

    if (GEM_NET_CTP_ENCRYPT_NO < gemNetCtp->attr.encryptKeyRing)
    {
        gemNetCtp->attr.encryptState = OMCI_ENCRYPTION_AES_128;
    }
    else
    {
        gemNetCtp->attr.encryptState = OMCI_ENCRYPTION_NONE;
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_net_ctp_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;
    ca_omci_uint16_t  offset    = 0;
    ca_omci_uint16_t  mask      = inMask;
    ca_omci_mebase_t *tcont     = NULL;
    ca_omci_mebase_t *priQ      = NULL;
    ca_omci_mebase_t *priQDs    = NULL;
    ca_omci_mebase_t *trafficUs = NULL;
    ca_omci_mebase_t *trafficDs = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_GEM_NET_CTP_SET_MASK_FULL);

    if (0 == mask)
    {
        OMCIDBG("No mask configure for Gem Net Ctp");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    //remove old link here
    tcont     = ca_omci_me_entry_lookup(OMCI_ME_TCONT_CLASS_ID, gemNetCtp->attr.tcontPoint);
    priQ      = ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gemNetCtp->attr.usTrafMngPoint);
    trafficUs = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gemNetCtp->attr.usTrafDescPoint);
    trafficDs = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gemNetCtp->attr.dsTrafMngPoint);
    priQDs    = ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gemNetCtp->attr.dsPqPoint);
    OMCI_RM_LINK_ME(gemNetCtp, trafficUs);
    OMCI_RM_LINK_ME(gemNetCtp, trafficDs);
    OMCI_RM_LINK_ME(gemNetCtp, priQ);
    OMCI_RM_LINK_ME(gemNetCtp, tcont);
    OMCI_RM_LINK_ME(gemNetCtp, priQDs);

    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.portId, "portId", 1);
    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.tcontPoint, "tcontPoint", 2);
    OMCI_UPDATE_ATTR_UINT(8,  gemNetCtp->attr.direction, "direction", 3);
    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.usTrafMngPoint, "usTrafMngPoint", 4);
    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.usTrafDescPoint, "usTrafDescPoint", 5);
    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.dsPqPoint, "dsPqPoint", 7);
    OMCI_UPDATE_ATTR_UINT(16, gemNetCtp->attr.dsTrafMngPoint, "dsTrafMngPoint", 9);
    OMCI_UPDATE_ATTR_UINT(8,  gemNetCtp->attr.encryptKeyRing, "encryptKeyRing", 10);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        if (GEM_NET_CTP_ENCRYPT_NO < gemNetCtp->attr.encryptKeyRing)
        {
            gemNetCtp->attr.encryptState = OMCI_ENCRYPTION_AES_128;
        }
        else
        {
            gemNetCtp->attr.encryptState = OMCI_ENCRYPTION_NONE;
        }
    }

    //only set encryptKeyRing
    if (0x40 == mask)
    {
        //call hal to set gemport encrypt key ring, don't modify the whole gemport
        ca_omci_hal_gem_keyring_set((void *)&gemNetCtp->attr.portId, (void *)&gemNetCtp->attr.encryptKeyRing);
        return CA_OMCI_OK;
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_net_ctp_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_GEM_NET_CTP_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.portId, 1);
    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.tcontPoint, 2);
    OMCI_GET_ATTR_UINT8(gemNetCtp->attr.direction, 3);
    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.usTrafMngPoint, 4);
    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.usTrafDescPoint, 5);
    OMCI_GET_ATTR_UINT8(gemNetCtp->attr.uniCnt, 6);
    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.dsPqPoint, 7);
    OMCI_GET_ATTR_UINT8(gemNetCtp->attr.encryptState, 8);
    OMCI_GET_ATTR_UINT16(gemNetCtp->attr.dsTrafMngPoint, 9);
    OMCI_GET_ATTR_UINT8(gemNetCtp->attr.encryptKeyRing, 10);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_net_ctp_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;
    ca_omci_onu_g_t       *onuG      = NULL;
    ca_omci_mebase_t      *tcont     = NULL;
    ca_omci_mebase_t      *priQ      = NULL;
    ca_omci_mebase_t      *priQDs    = NULL;
    ca_omci_mebase_t      *trafficUs = NULL;
    ca_omci_mebase_t      *trafficDs = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    onuG = (ca_omci_onu_g_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_G_CLASS_ID, 0);
    if (GEM_NET_CTP_DIR_DS != gemNetCtp->attr.direction)
    {
        tcont = ca_omci_me_entry_lookup(OMCI_ME_TCONT_CLASS_ID, gemNetCtp->attr.tcontPoint);
        OMCI_RM_LINK_ME(gemNetCtp, tcont);

        if ((NULL != onuG)
        && ((ONU_G_TRAF_OP_PRI == onuG->attr.trafficMngOption) || (ONU_G_TRAF_OP_PRI_RATE == onuG->attr.trafficMngOption)))
        {
            priQ = ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gemNetCtp->attr.usTrafMngPoint);
            OMCI_RM_LINK_ME(gemNetCtp, priQ);
        }
    }

    if ((NULL != onuG)
    && ((ONU_G_TRAF_OP_PRI == onuG->attr.trafficMngOption) || (ONU_G_TRAF_OP_PRI_RATE == onuG->attr.trafficMngOption)))
    {
        trafficUs = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gemNetCtp->attr.usTrafDescPoint);
        trafficDs = ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gemNetCtp->attr.dsTrafMngPoint);
        OMCI_RM_LINK_ME(gemNetCtp, trafficUs);
        OMCI_RM_LINK_ME(gemNetCtp, trafficDs);
    }

    if (GEM_NET_CTP_DIR_US != gemNetCtp->attr.direction)
    {
        priQDs = ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gemNetCtp->attr.dsPqPoint);
        OMCI_RM_LINK_ME(gemNetCtp, priQDs);
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_gem_net_ctp_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(gemNetCtp);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.portId, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.tcontPoint, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, gemNetCtp->attr.direction, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.usTrafMngPoint, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.usTrafDescPoint, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, gemNetCtp->attr.uniCnt, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.dsPqPoint, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, gemNetCtp->attr.encryptState, 8);
    OMCI_UPLOAD_ATTR_SIZE(16, gemNetCtp->attr.dsTrafMngPoint, 9);
    OMCI_UPLOAD_ATTR_SIZE(8, gemNetCtp->attr.encryptKeyRing, 10);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_gem_net_ctp_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_gem_net_ctp_t *gemNetCtp = (ca_omci_gem_net_ctp_t *)me;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: gemNetCtp \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        portId:      0x%x\n", gemNetCtp->attr.portId);
    omci_sprint(cli, "        tcontPoint:  0x%x\n", gemNetCtp->attr.tcontPoint);
    omci_sprint(cli, "        direction:   0x%x\n", gemNetCtp->attr.direction);
    omci_sprint(cli, "        usTrafMngPoint: 0x%x\n", gemNetCtp->attr.usTrafMngPoint);
    omci_sprint(cli, "        usTrafDescPoint: 0x%x\n", gemNetCtp->attr.usTrafDescPoint);
    omci_sprint(cli, "        uniCnt:      0x%x\n", gemNetCtp->attr.uniCnt);
    omci_sprint(cli, "        dsPqPoint:   0x%x\n", gemNetCtp->attr.dsPqPoint);
    omci_sprint(cli, "        encryptState: 0x%x\n", gemNetCtp->attr.encryptState);
    omci_sprint(cli, "        dsTrafMngPoint: 0x%x\n", gemNetCtp->attr.dsTrafMngPoint);
    omci_sprint(cli, "        encryptKeyRing: 0x%x\n", gemNetCtp->attr.encryptKeyRing);
}


void ca_omci_me_gem_net_ctp_init(void)
{
    sg_omci_me_gem_net_ctp_handlers.meDisplay  = ca_omci_me_gem_net_ctp_display;
    sg_omci_me_gem_net_ctp_handlers.uploadProc = ca_omci_me_gem_net_ctp_upload;

    sg_omci_me_gem_net_ctp_handlers.createProc = ca_omci_me_gem_net_ctp_create;
    sg_omci_me_gem_net_ctp_handlers.deleteProc = ca_omci_me_gem_net_ctp_delete;
    sg_omci_me_gem_net_ctp_handlers.setProc    = ca_omci_me_gem_net_ctp_set;
    sg_omci_me_gem_net_ctp_handlers.getProc    = ca_omci_me_gem_net_ctp_get;
}


void ca_omci_me_gem_net_ctp_link(void)
{
    ca_omci_meclass_t          *meClass    = NULL;
    ca_omci_mebase_t           *meInst     = NULL;
    ca_omci_gem_net_ctp_t      *gpnctp     = NULL;
    ca_omci_tcont_t            *tcont      = NULL;
    ca_omci_onu_g_t            *onuG       = NULL;
    ca_omci_pri_queue_t        *priQ       = NULL;
    ca_omci_pri_queue_t        *priQDs     = NULL;
    ca_omci_traffic_descript_t *trafficUs  = NULL;
    ca_omci_traffic_descript_t *trafficDs  = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_GEM_NET_CTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no gemport is created now , flow is not complete");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        gpnctp = (ca_omci_gem_net_ctp_t *)meInst;

        onuG = (ca_omci_onu_g_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_G_CLASS_ID, 0);
        OMCI_CHECK_PARAM_CNU(NULL == onuG);

        if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
        {
            tcont = (ca_omci_tcont_t *)ca_omci_me_entry_lookup(OMCI_ME_TCONT_CLASS_ID, gpnctp->attr.tcontPoint);
            /* if can't find the tcont , the flow is not complete*/
            OMCI_CHECK_PARAM_CNU(NULL == tcont);

            if ((ONU_G_TRAF_OP_PRI == onuG->attr.trafficMngOption)
             || (ONU_G_TRAF_OP_PRI_RATE == onuG->attr.trafficMngOption))
            {
                priQ = (ca_omci_pri_queue_t *)ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gpnctp->attr.usTrafMngPoint);
                /* if can't find the priQ , the flow is not complete*/
                OMCI_CHECK_PARAM_CNU(NULL == priQ);
            }
        }

        if ((ONU_G_TRAF_OP_PRI == onuG->attr.trafficMngOption)
         || (ONU_G_TRAF_OP_PRI_RATE == onuG->attr.trafficMngOption))
        {
            if (0 != gpnctp->attr.usTrafDescPoint)
            {
                trafficUs = (ca_omci_traffic_descript_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gpnctp->attr.usTrafDescPoint);
                OMCI_CHECK_PARAM_CNU(NULL == trafficUs);
            }
            if (0 != gpnctp->attr.dsTrafMngPoint)
            {
                trafficDs = (ca_omci_traffic_descript_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gpnctp->attr.dsTrafMngPoint);
                OMCI_CHECK_PARAM_CNU(NULL == trafficDs);
            }
        }

        if ((GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
         && (OMCI_ME_NULL_POINTER != gpnctp->attr.dsPqPoint))
        {
            priQDs = (ca_omci_pri_queue_t *)ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gpnctp->attr.dsPqPoint);
            OMCI_CHECK_PARAM_CNU(NULL == priQDs);
        }

        //need add link at final, or should delete link when error
        OMCI_ME_LINK(gpnctp, priQ);
        OMCI_ME_LINK(gpnctp, tcont);
        OMCI_ME_LINK(gpnctp, priQDs);
        OMCI_ME_LINK(gpnctp, trafficUs);
        OMCI_ME_LINK(gpnctp, trafficDs);
    }
}

