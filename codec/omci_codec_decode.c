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
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_mibmng.h"
#include "omci_mic.h"
#include "omci_mng_am.h"
#include "omci_hal.h"
#include "omci_mng_upgrade.h"
#include "omci_pm.h"
#include "omci_cfg.h"
#include "omci_mng_onuinfo.h"
#include "osal_cmn.h"
#include "osal_mutex.h"
#include "osal_sem.h"


extern void ca_omci_me_ani_g_test(ca_omci_mebase_t *me);
extern ca_uint g_omciAnalysisSem;


ca_omci_mebase_t *omci_codec_get_me_base(ca_omci_pkt_hdr_t *pHdr)
{
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;

    if (NULL == pHdr)
    {
        OMCICOERR("NULL pointer");
        return NULL;
    }

    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);
    if (OMCI_ME_CLASS_ID_MAX == innerCls)
    {
        OMCICOERR("unknow class %d", classId);
        return NULL;
    }

    OMCICODBG("me class[%d] instance[0x%x]", classId, instId);

    return ca_omci_me_entry_lookup(innerCls, instId);
}


void omci_codec_decode_create(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t          *pHdr     = NULL;
    ca_omci_mebase_t           *meBase   = NULL;
    ca_omci_me_create_result_t  rsl      = {0};
    ca_omci_uint16_t            classId  = 0;
    ca_omci_class_id_e          innerCls = 0;
    ca_omci_uint16_t            instId   = 0;
    ca_omci_uint8_t            *attr     = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr = (ca_omci_pkt_hdr_t *)pkt;
    attr = pkt + sizeof(ca_omci_pkt_hdr_t);

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL != meBase)
    {
        OMCICOERR("me %d already exist", ca_omci_ntohs(pHdr->classId));
        rsl.result = OMCI_MSG_RST_INST_EXIST;
        omci_codec_encode_create_rsp(pkt, &rsl);
        return;
    }

    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);
    meBase   = ca_omci_me_new(innerCls);
    if (NULL == meBase)
    {
        OMCICOERR("unknow class[%d] \n", classId);
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_create_rsp(pkt, &rsl);
        return;
    }

    if (NULL == meBase->handlers->createProc)
    {
        OMCICOERR("class[%d] don't support create\n", classId);
        rsl.result = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_create_rsp(pkt, &rsl);
        return;
    }

    meBase->instanceId = instId;
    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        attr = pkt + sizeof(ca_omci_ext_pkt_hdr_t);
    }

    if (CA_OMCI_OK != meBase->handlers->createProc(meBase, attr, &rsl))
    {
        OMCICOERR("class[%d] create error\n", classId);
        ca_omci_me_delete(meBase);
        omci_codec_encode_create_rsp(pkt, &rsl);
        return;
    }

    ca_omci_me_entry_add(meBase);
    ca_omci_mibdata_inc();
    omci_codec_encode_create_rsp(pkt, &rsl);
}


void omci_codec_decode_delete(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_mebase_t   *meBase   = NULL;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;
    ca_omci_uint16_t    classId  = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr    = (ca_omci_pkt_hdr_t *)pkt;
    classId = ca_omci_ntohs(pHdr->classId);

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_delete_rsp(pkt, rsl);
        return;
    }

    if (NULL == meBase->handlers->deleteProc)
    {
        OMCICOERR("class[%d] don't support delete\n", classId);
        rsl = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_delete_rsp(pkt, rsl);
        return;
    }

    if (CA_OMCI_OK != meBase->handlers->deleteProc(meBase, &rsl))
    {
        OMCICOERR("class[%d] create error\n", classId);
        omci_codec_encode_delete_rsp(pkt, rsl);
        return;
    }

    omci_codec_encode_delete_rsp(pkt, rsl);
    ca_omci_me_entry_delete(meBase->classId, meBase->instanceId);
    ca_omci_mibdata_inc();
    ca_omci_me_delete(meBase);
}


void omci_codec_decode_set(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t       *pHdr     = NULL;
    ca_omci_mebase_t        *meBase   = NULL;
    ca_omci_me_set_result_t  rsl      = {0};
    ca_omci_uint16_t         classId  = 0;
    ca_omci_uint16_t         instId   = 0;
    ca_omci_uint16_t         inMask   = 0;
    ca_omci_uint8_t         *attr     = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr    = (ca_omci_pkt_hdr_t *)pkt;
    classId = ca_omci_ntohs(pHdr->classId);
    instId  = ca_omci_ntohs(pHdr->instId);
    attr    = pkt + sizeof(ca_omci_pkt_hdr_t) + sizeof(inMask);
    OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(inMask));

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        attr = pkt + sizeof(ca_omci_ext_pkt_hdr_t) + sizeof(inMask);
        OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(inMask));
    }

    inMask = ca_omci_ntohs(inMask);
    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me class[%d] instance [%d]doesn't exist\n", classId, instId);
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_set_rsp(pkt, &rsl);
        return;
    }

    if (NULL == meBase->handlers->setProc)
    {
        OMCICOERR("class[%d] don't support set\n", classId);
        rsl.result = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_set_rsp(pkt, &rsl);
        return;
    }

    /* accoding to G.988, need to judge the mask of set to decide include mib data or not */
    if (CA_OMCI_OK != meBase->handlers->setProc(meBase, inMask, attr, &rsl))
    {
        OMCICOERR("class[%d] instance[%d]set error\n", classId, instId);
        omci_codec_encode_set_rsp(pkt, &rsl);
        return;
    }

    ca_omci_mibdata_inc();
    omci_codec_encode_set_rsp(pkt, &rsl);
}


void omci_codec_decode_get(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t       *pHdr     = NULL;
    ca_omci_mebase_t        *meBase   = NULL;
    ca_omci_me_get_result_t  rsl      = {0};
    ca_omci_uint16_t         classId  = 0;
    ca_omci_uint16_t         inMask   = 0;
    ca_omci_uint8_t         *outPdu   = NULL;
    ca_omci_uint8_t         *attr     = NULL;
    ca_omci_uint8_t          ext      = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr    = (ca_omci_pkt_hdr_t *)pkt;
    classId = ca_omci_ntohs(pHdr->classId);
    ext     = (OMCI_EXT_DEVICE_ID == pHdr->devId) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    outPdu  = ca_omci_pkt_tx_hdr_build(pkt);

    OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(inMask));
    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(inMask));
    }

    inMask = ca_omci_ntohs(inMask);
    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me class [%d] doesn't exist", classId);
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_get_rsp(outPdu, &rsl);
        return;
    }

    if (NULL == meBase->handlers->getProc)
    {
        OMCICOERR("class[%d] don't support get\n", classId);
        rsl.result  = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_get_rsp(outPdu, &rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        //header + rsl + mask + 2 masks
        attr = outPdu + sizeof(ca_omci_ext_pkt_hdr_t) + OMCI_GET_RSP_MSG_BASE_LEN + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t);
    }
    else
    {
        attr = outPdu + sizeof(ca_omci_pkt_hdr_t) + OMCI_GET_RSP_MSG_BASE_LEN;
    }

    if (CA_OMCI_OK != meBase->handlers->getProc(meBase, ext, inMask, attr, &rsl))
    {
        OMCICOERR("class[%d] get error\n", classId);
    }

    omci_codec_encode_get_rsp(outPdu, &rsl);
}


void omci_codec_decode_get_all_alarms(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint16_t    seqNum   = 0;
    ca_omci_bool_t      ext      = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);

    if ((OMCI_ME_ONU_DATA_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d or instance 0x%x", classId, instId);
        omci_codec_encode_get_all_alarms_rsp(pkt, seqNum);
        return;
    }

    ext    = (OMCI_EXT_DEVICE_ID == pHdr->devId) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    seqNum = ca_omci_mng_get_all_alarms_num_get(ext);
    omci_codec_encode_get_all_alarms_rsp(pkt, seqNum);
}


void omci_codec_decode_get_all_alarms_next(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint16_t    seqNum   = 0;
    ca_omci_bool_t      ext      = CA_OMCI_FALSE;
    ca_omci_uint8_t    *outPdu   = NULL;
    ca_omci_uint8_t    *contents = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);
    outPdu   = ca_omci_pkt_tx_hdr_build(pkt);
    contents = outPdu + sizeof(ca_omci_pkt_hdr_t);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        ext = CA_OMCI_TRUE;
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
    }
    else
    {
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
    }
    seqNum = ca_omci_ntohs(seqNum);

    if ((OMCI_ME_ONU_DATA_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d or instance 0x%x", classId, instId);
        ca_omci_mic_build(outPdu, ext);
        ca_omci_pkt_tx(outPdu, ext);
        return;
    }

    ca_omci_mng_get_all_alarm_next(seqNum, ext, contents);
    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_decode_mib_upload(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint16_t    seqNum   = 0;
    ca_omci_bool_t      ext      = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);
    if ((OMCI_ME_ONU_DATA_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d or instance 0x%x", classId, instId);
        omci_codec_encode_mib_upload_rsp(pkt, seqNum);
        return;
    }

    ext = (OMCI_EXT_DEVICE_ID == pHdr->devId) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    seqNum = ca_omci_mng_mibmng_seq_num_get(ext);
    omci_codec_encode_mib_upload_rsp(pkt, seqNum);
}


void omci_codec_decode_mib_upload_next(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint16_t    seqNum   = 0;
    ca_omci_bool_t      ext      = CA_OMCI_FALSE;
    ca_omci_uint8_t    *outPdu   = NULL;
    ca_omci_uint8_t    *contents = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);
    outPdu   = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        ext = CA_OMCI_TRUE;
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
        contents = outPdu + sizeof(ca_omci_pkt_hdr_t);//need to get all len when do mib upload next
    }
    else
    {
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
        contents = outPdu + sizeof(ca_omci_pkt_hdr_t);
    }
    seqNum = ca_omci_ntohs(seqNum);

    if ((OMCI_ME_ONU_DATA_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d or instance 0x%x", classId, instId);
        ca_omci_mic_build(outPdu, ext);
        ca_omci_pkt_tx(outPdu, ext);
        return;
    }

    ca_omci_mng_mibmng_upload_next(seqNum, ext, contents);
    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_decode_mib_reset(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);

    if ((OMCI_ME_ONU_DATA_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d or instance 0x%x", classId, instId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    rsl = ca_omci_mng_mib_reset();
    omci_codec_encode_action_rsp(pkt, rsl);
}


ca_omci_uint8_t omci_codec_decode_anig_test(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t *pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    ca_omci_mebase_t  *meBase = NULL;

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL != meBase)
    {
        ca_omci_me_ani_g_test(meBase);
        return OMCI_MSG_RST_SUCCESSFUL;
    }

    return OMCI_MSG_RST_UNKNOWN_ME;
}


static ca_omci_decode_test_cb_t sg_testCb[] =
{
    {OMCI_ME_ANI_G_CLASS_ID, omci_codec_decode_anig_test},
};


void omci_codec_decode_test(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t      *pHdr     = NULL;
    ca_omci_uint16_t        classId  = 0;
    ca_omci_class_id_e      innerCls = 0;
    ca_omci_uint8_t         rsl      = OMCI_MSG_RST_SUCCESSFUL;
    ca_omci_uint8_t         i        = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);

    for (i = 0; i < sizeof(sg_testCb)/sizeof(ca_omci_decode_test_cb_t); i++)
    {
        if (sg_testCb[i].classId == innerCls)
        {
            rsl = sg_testCb[i].func(pkt);
            break;
        }
    }

    if (i >= sizeof(sg_testCb)/sizeof(ca_omci_decode_test_cb_t))
    {
        OMCICOERR("no function for class(%d)", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
    }

    omci_codec_encode_action_rsp(pkt, rsl);
}


void omci_codec_decode_start_sw_download(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t      *pHdr     = NULL;
    ca_omci_mebase_t       *meBase   = NULL;
    ca_omci_uint8_t        *contents = NULL;
    ca_omci_uint16_t        classId  = 0;
    ca_omci_class_id_e      innerCls = 0;
    ca_omci_uint16_t        instId   = 0;
    ca_omci_me_st_dl_rsl_t  rsl      = {0};

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);

    ca_omci_mibdata_inc();

    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    if (OMCI_ME_SOFT_IMAGE_CLASS_ID != innerCls)
    {
        OMCICOERR("error class %d", classId);
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_start_sw_dl_rsp(pkt, &rsl);
        return;
    }

    if (OMCI_PARALLEL_DL_SUPPORT == instId)
    {
        OMCICOWARN("Unsupport parallel download now");
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME_INST;
        omci_codec_encode_start_sw_dl_rsp(pkt, &rsl);
        return;
    }

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_start_sw_dl_rsp(pkt, &rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        contents = pkt + sizeof(ca_omci_pkt_hdr_t) + sizeof(ca_omci_uint16_t); // add msg len 2 bytes
    }
    else
    {
        contents = pkt + sizeof(ca_omci_pkt_hdr_t);
    }

    ca_omci_mng_st_dl_process(meBase, contents, &rsl);
    omci_codec_encode_start_sw_dl_rsp(pkt, &rsl);
}



void omci_codec_decode_download_section(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t   *pHdr     = NULL;
    ca_omci_mebase_t    *meBase   = NULL;
    ca_omci_uint16_t     classId  = 0;
    ca_omci_class_id_e   innerCls = 0;
    ca_omci_uint16_t     instId   = 0;
    ca_omci_uint8_t      rsl      = 0;
    ca_omci_me_dl_sec_t  secInfo  = {0};

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);

    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    instId   = ca_omci_ntohs(pHdr->instId);

    secInfo.isLast = (pHdr->msgType & 0x40) ? 1 : 0;

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        OMCI_MEMCPY(&secInfo.sectionNum, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(ca_omci_uint8_t));
        OMCI_MEMCPY(&secInfo.dataLen, pkt + sizeof(ca_omci_pkt_hdr_t), sizeof(ca_omci_uint16_t));
        secInfo.dataLen = ca_omci_ntohs(secInfo.dataLen);

        if (secInfo.dataLen <=  sizeof(secInfo.sectionNum))
        {
            OMCICOERR("Data len is too small %d \n", secInfo.dataLen);
            rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
            omci_codec_encode_dl_sec_rsp(pkt, rsl, &secInfo);
        }

        secInfo.dataLen -= sizeof(secInfo.sectionNum);
        secInfo.data    =  pkt + sizeof(ca_omci_ext_pkt_hdr_t) + sizeof(ca_omci_uint8_t);
    }
    else
    {
        OMCI_MEMCPY(&secInfo.sectionNum, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(ca_omci_uint8_t));
        secInfo.dataLen = OMCI_MSG_IMG_DATA_LEN;
        secInfo.data    = pkt + sizeof(ca_omci_pkt_hdr_t) + sizeof(ca_omci_uint8_t);
    }


    if (OMCI_ME_SOFT_IMAGE_CLASS_ID != innerCls)
    {
        OMCICOERR("error class %d", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_dl_sec_rsp(pkt, rsl, &secInfo);
        return;
    }

    if (OMCI_PARALLEL_DL_SUPPORT == instId)
    {
        OMCICOWARN("Unsupport parallel download now");
        rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        omci_codec_encode_dl_sec_rsp(pkt, rsl, &secInfo);
        return;
    }

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_dl_sec_rsp(pkt, rsl, &secInfo);
        return;
    }

    ca_omci_mng_dl_sec_process(meBase, &secInfo, &rsl);
    omci_codec_encode_dl_sec_rsp(pkt, rsl, &secInfo);
}


void omci_codec_decode_end_sw_download(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_mebase_t   *meBase   = NULL;
    ca_omci_uint8_t    *contents = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);

    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    if (OMCI_ME_SOFT_IMAGE_CLASS_ID != innerCls)
    {
        OMCICOERR("error class %d", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_end_sw_dl_rsp(pkt, rsl);
        return;
    }

    if (OMCI_PARALLEL_DL_SUPPORT == instId)
    {
        OMCICOWARN("Unsupport parallel download now");
        rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        omci_codec_encode_end_sw_dl_rsp(pkt, rsl);
        return;
    }

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_end_sw_dl_rsp(pkt, rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        contents = pkt + sizeof(ca_omci_pkt_hdr_t) + sizeof(ca_omci_uint16_t); // add msg len 2 bytes
    }
    else
    {
        contents = pkt + sizeof(ca_omci_pkt_hdr_t);
    }

    ca_omci_mng_end_dl_process(meBase, contents, &rsl);

    if (OMCI_MSG_RST_SUCCESSFUL == rsl)
    {
        ca_omci_mibdata_inc();
    }

    omci_codec_encode_end_sw_dl_rsp(pkt, rsl);
}



void omci_codec_decode_activate_img(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_mebase_t   *meBase   = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;
    ca_omci_uint8_t     flag     = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);

    ca_omci_mibdata_inc();

    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    if (OMCI_ME_SOFT_IMAGE_CLASS_ID != innerCls)
    {
        OMCICOERR("error class %d for active image", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        OMCI_MEMCPY(&flag, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(flag));
    }
    else
    {
        OMCI_MEMCPY(&flag, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(flag));
    }

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    ca_omci_mng_img_active_process(meBase, flag, &rsl);
    omci_codec_encode_action_rsp(pkt, rsl);
}


void omci_codec_decode_commit_img(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_mebase_t   *meBase   = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);

    ca_omci_mibdata_inc();

    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    if (OMCI_ME_SOFT_IMAGE_CLASS_ID != innerCls)
    {
        OMCICOERR("error class %d for active image", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    ca_omci_mng_img_commit_process(meBase, &rsl);
    omci_codec_encode_action_rsp(pkt, rsl);
}


void omci_codec_decode_sync_time(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t          *pHdr     = NULL;
    ca_omci_uint16_t            classId  = 0;
    ca_omci_class_id_e          innerCls = 0;
    ca_omci_uint16_t            instId   = 0;
    ca_omci_me_sync_time_rsl_t  rsl      = {0};
    ca_omci_cfg_sync_time_t     synTime  = {0};
    ca_omci_uint16_t            offset   = 0;
    ca_omci_bool_t              flag     = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);

    if ((OMCI_ME_ONU_G_CLASS_ID != innerCls) || (0 != instId))
    {
        OMCICOERR("error class %d instance 0x%x for syn time", classId, instId);
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_sync_time_rsp(pkt, &rsl);
        return;
    }

    //decode time;
    offset = sizeof(ca_omci_pkt_hdr_t);
    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        offset = sizeof(ca_omci_ext_pkt_hdr_t);
    }

    OMCI_MEMCPY(&synTime.year, (pkt + offset), sizeof(ca_omci_uint16_t));
    synTime.year = ca_omci_ntohs(synTime.year);
    offset += sizeof(ca_omci_uint16_t);
    OMCI_MEMCPY(&synTime.month, (pkt + offset), sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(&synTime.day, (pkt + offset), sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(&synTime.hour, (pkt + offset), sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(&synTime.min, (pkt + offset), sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(&synTime.sec, (pkt + offset), sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    rsl.rslInfo = OMCI_SYNC_TIME_15_TICK_OK;
    if (CA_OMCI_OK != ca_omci_cfg_syn_time(&synTime))
    {
        rsl.rslInfo = OMCI_SYNC_TIME_15_TICK_OK;
    }

    if ((0 == synTime.year) && (0 == synTime.month) && (0 == synTime.day)
     && (0 == synTime.hour) && (0 == synTime.min) && (0 == synTime.sec))
    {
        flag = CA_OMCI_TRUE;
    }

    ca_omci_pm_sync_time(flag);
    omci_codec_encode_sync_time_rsp(pkt, &rsl);
}


void omci_codec_decode_reboot(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr     = NULL;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = 0;
    ca_omci_uint16_t    instId   = 0;
    ca_omci_uint8_t     rsl      = OMCI_MSG_RST_SUCCESSFUL;
    ca_omci_uint8_t     flag     = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    classId  = ca_omci_ntohs(pHdr->classId);
    instId   = ca_omci_ntohs(pHdr->instId);
    innerCls = ca_omci_me_get_inner_class(classId);

    if ((OMCI_ME_ONU_G_CLASS_ID != innerCls) && (OMCI_ME_CIRCUIT_PACK_CLASS_ID != innerCls))
    {
        OMCICOERR("error class %d for reboot", classId);
        rsl = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_action_rsp(pkt, rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        OMCI_MEMCPY(&flag, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(flag));
    }
    else
    {
        OMCI_MEMCPY(&flag, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(flag));
    }

    if (OMCI_ME_ONU_G_CLASS_ID == innerCls)
    {
        if (0 != instId)
        {
            OMCICOERR("error instance0x%x for onu_g  for reboot", instId);
            rsl = OMCI_MSG_RST_UNKNOWN_ME;
            omci_codec_encode_action_rsp(pkt, rsl);
            return;
        }

        omci_codec_encode_action_rsp(pkt, rsl);
        ca_omci_hal_onu_reboot((void *)&flag);
        return;
    }

    //TODO: add circuit pack reboot
    omci_codec_encode_action_rsp(pkt, rsl);
}


void omci_codec_decode_get_next(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t             *pHdr     = NULL;
    ca_omci_mebase_t              *meBase   = NULL;
    ca_omci_me_getnext_result_t    rsl      = {0};
    ca_omci_bool_t                 ext      = CA_OMCI_FALSE;
    ca_omci_uint16_t               oriMask  = 0;
    ca_omci_uint16_t               mask     = 0;
    ca_omci_uint16_t               seqNum   = 0;
    ca_omci_uint16_t               classId  = 0;
    ca_omci_uint8_t               *outPdu   = NULL;
    ca_omci_uint8_t               *contents = NULL;
    ca_omci_uint8_t               *attr     = NULL;
    ca_omci_uint8_t                maskNum  = 0;
    ca_omci_uint8_t                idx      = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr     = (ca_omci_pkt_hdr_t *)pkt;
    meBase   = omci_codec_get_me_base(pHdr);
    classId  = ca_omci_ntohs(pHdr->classId);
    outPdu   = ca_omci_pkt_tx_hdr_build(pkt);

    if (NULL == meBase)
    {
        OMCICOERR("me not exist");
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_get_next_rsp(outPdu, &rsl);
        return;
    }

    if (NULL == meBase->handlers->getNextProc)
    {
        OMCICOERR("class[%d] don't support get next\n", classId);
        rsl.result = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_get_next_rsp(outPdu, &rsl);
        return;
    }

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        ext = CA_OMCI_TRUE;
        OMCI_MEMCPY(&oriMask, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_ext_pkt_hdr_t) + sizeof(ca_omci_uint16_t)), sizeof(ca_omci_uint16_t));
        contents = outPdu + sizeof(ca_omci_ext_pkt_hdr_t) + sizeof (ca_omci_uint8_t);
    }
    else
    {
        OMCI_MEMCPY(&oriMask, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(ca_omci_uint16_t));
        OMCI_MEMCPY(&seqNum, (pkt + sizeof(ca_omci_pkt_hdr_t) + sizeof(ca_omci_uint16_t)), sizeof(ca_omci_uint16_t));
        contents = outPdu + sizeof(ca_omci_pkt_hdr_t) + sizeof (ca_omci_uint8_t);
    }

    mask   = ca_omci_ntohs(oriMask);
    seqNum = ca_omci_ntohs(seqNum);
    for (idx = 0; idx < OMCI_MAX_ATTR_NUM; idx++)
    {
        if (OMCI_MASK_ATTR_EXIST(idx, mask))
        {
            maskNum++;
        }
    }
    if (1 != maskNum)
    {
        OMCICOERR("can't get next for multiple attributes");
        rsl.result = OMCI_MSG_RST_PARAM_ERR;
        omci_codec_encode_get_next_rsp(outPdu, &rsl);
        return;
    }

    //if mask check right, copy it here
    OMCI_MEMCPY(contents, &oriMask, sizeof(ca_omci_uint16_t));
    attr = contents + sizeof(ca_omci_uint16_t);

    if (CA_OMCI_OK != meBase->handlers->getNextProc(meBase, ext, seqNum, mask, attr, &rsl))
    {
        OMCICOERR("class[%d] get next error\n", classId);
    }

    omci_codec_encode_get_next_rsp(outPdu, &rsl);
}



void omci_codec_decode_get_current_data(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t       *pHdr     = NULL;
    ca_omci_mebase_t        *meBase   = NULL;
    ca_omci_me_get_result_t  rsl      = {0};
    ca_omci_uint16_t         classId  = 0;
    ca_omci_uint16_t         inMask   = 0;
    ca_omci_uint8_t         *outPdu   = NULL;
    ca_omci_uint8_t         *attr     = NULL;
    ca_omci_uint8_t          ext      = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr    = (ca_omci_pkt_hdr_t *)pkt;
    classId = ca_omci_ntohs(pHdr->classId);
    ext     = (OMCI_EXT_DEVICE_ID == pHdr->devId) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    outPdu  = ca_omci_pkt_tx_hdr_build(pkt);
    attr    = outPdu + sizeof(ca_omci_pkt_hdr_t) + OMCI_GET_RSP_MSG_BASE_LEN;
    OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_pkt_hdr_t)), sizeof(inMask));

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        //header + rsp + 3 masks
        OMCI_MEMCPY(&inMask, (pkt + sizeof(ca_omci_ext_pkt_hdr_t)), sizeof(inMask));
        attr = outPdu + sizeof(ca_omci_ext_pkt_hdr_t) + OMCI_GET_RSP_MSG_BASE_LEN + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t); // add msg len 2 bytes
    }

    inMask = ca_omci_ntohs(inMask);
    meBase = omci_codec_get_me_base(pHdr);
    if (NULL == meBase)
    {
        OMCICOERR("me doesn't exist");
        rsl.result = OMCI_MSG_RST_UNKNOWN_ME;
        omci_codec_encode_get_curr_rsp(outPdu, &rsl);
        return;
    }

    if (NULL == meBase->handlers->getCurProc)
    {
        OMCICOERR("class[%d] don't support get current data\n", classId);
        rsl.result  = OMCI_MSG_RST_NOT_SUPPORT;
        omci_codec_encode_get_curr_rsp(outPdu, &rsl);
        return;
    }

    if (CA_OMCI_OK != meBase->handlers->getCurProc(meBase, ext, inMask, attr, &rsl))
    {
        OMCICOERR("class[%d] get current error\n", classId);
    }

    omci_codec_encode_get_curr_rsp(outPdu, &rsl);
}


void omci_codec_decode_set_tbl(ca_omci_uint8_t *pkt)
{
    (void)pkt;
}


static ca_omci_codec_decode_cb_t sg_omciDecodeCbTbl[] =
{
    {OMCI_MSG_TYPE_CREATE,            omci_codec_decode_create},
    {OMCI_MSG_TYPE_DELETE,            omci_codec_decode_delete},
    {OMCI_MSG_TYPE_SET,               omci_codec_decode_set},
    {OMCI_MSG_TYPE_GET,               omci_codec_decode_get},
    {OMCI_MSG_TYPE_GET_ALL_ALM,       omci_codec_decode_get_all_alarms},
    {OMCI_MSG_TYPE_GET_ALL_ALM_NEXT,  omci_codec_decode_get_all_alarms_next},
    {OMCI_MSG_TYPE_MIB_UPLOAD,        omci_codec_decode_mib_upload},
    {OMCI_MSG_TYPE_MIB_UPLOAD_NEXT,   omci_codec_decode_mib_upload_next},
    {OMCI_MSG_TYPE_MIB_RESET,         omci_codec_decode_mib_reset},
    {OMCI_MSG_TYPE_TEST,              omci_codec_decode_test},
    {OMCI_MSG_TYPE_START_SW_DOWNLOAD, omci_codec_decode_start_sw_download},
    {OMCI_MSG_TYPE_DOWNLOAD_SEC,      omci_codec_decode_download_section},
    {OMCI_MSG_TYPE_END_SW_DOWNLOAD,   omci_codec_decode_end_sw_download},
    {OMCI_MSG_TYPE_ACTIVATE_SW,       omci_codec_decode_activate_img},
    {OMCI_MSG_TYPE_COMMIT_SW,         omci_codec_decode_commit_img},
    {OMCI_MSG_TYPE_SYNCH_TIME,        omci_codec_decode_sync_time},
    {OMCI_MSG_TYPE_REBOOT,            omci_codec_decode_reboot},
    {OMCI_MSG_TYPE_GET_NEXT,          omci_codec_decode_get_next},
    {OMCI_MSG_TYPE_GET_CURR_DATA,     omci_codec_decode_get_current_data},
    {OMCI_MSG_TYPE_SET_TABLE,         omci_codec_decode_set_tbl},
};


ca_omci_st_e ca_omci_decode_pkt(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t *pHdr = (ca_omci_pkt_hdr_t *)pkt;
    ca_omci_uint8_t    i    = 0;

    OMCI_PARAM_NULL_RETURN(pkt);

    //need to check the flow analysis finish or not
    if (CA_OSAL_OK != ca_sem_wait(g_omciAnalysisSem, 0))
    {
        OMCICOERR("Busy for analysis now");
        return CA_OMCI_OK;
    }

    for (i = 0; i < sizeof(sg_omciDecodeCbTbl)/sizeof(ca_omci_codec_decode_cb_t); i++)
    {
        if(sg_omciDecodeCbTbl[i].msgType == (pHdr->msgType & 0x1f))
        {
            sg_omciDecodeCbTbl[i].func(pkt);
            break;
        }
    }

    //release the sem after handle the packet
    ca_sem_post(g_omciAnalysisSem);

    if (i >= sizeof(sg_omciDecodeCbTbl)/sizeof(ca_omci_codec_decode_cb_t))
    {
        OMCICOERR("no function for msg type(%d)", (pHdr->msgType & 0x1f));
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}

