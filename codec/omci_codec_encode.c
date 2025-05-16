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
#include "omci_codec_pkt.h"
#include "omci_mic.h"
#include "omci_cfg.h"
#include "omci_me.h"
#include "omci_hal.h"
#include "omci_mng_am.h"


void omci_codec_encode_create_rsp(ca_omci_uint8_t *pkt, ca_omci_me_create_result_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_CREATE_RSP_MSG_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    rsl->exeMask = ca_omci_htons(rsl->exeMask);

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(rsl->result));
    offset += sizeof(rsl->result);
    OMCI_MEMCPY(outPdu + offset, &rsl->exeMask, sizeof(rsl->exeMask));
    offset += sizeof(rsl->exeMask);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_delete_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_DELETE_RSP_MSG_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_set_rsp(ca_omci_uint8_t *pkt, ca_omci_me_set_result_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_SET_RSP_MSG_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    rsl->optMask = ca_omci_htons(rsl->optMask);
    rsl->exeMask = ca_omci_htons(rsl->exeMask);

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(rsl->result));
    offset += sizeof(rsl->result);
    OMCI_MEMCPY(outPdu + offset, &rsl->optMask, sizeof(rsl->optMask));
    offset += sizeof(rsl->optMask);
    OMCI_MEMCPY(outPdu + offset, &rsl->exeMask, sizeof(rsl->exeMask));
    offset += sizeof(rsl->exeMask);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_get_rsp(ca_omci_uint8_t *outPdu, ca_omci_me_get_result_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(outPdu);
    pHdr = (ca_omci_pkt_hdr_t *)outPdu;

    // if attrlen is 0, add this value have no effect, so add it
    msgLen       = OMCI_GET_RSP_MSG_BASE_LEN + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t) + rsl->attrLen;
    rsl->mask    = ca_omci_htons(rsl->mask);
    rsl->optMask = ca_omci_htons(rsl->optMask);
    rsl->exeMask = ca_omci_htons(rsl->exeMask);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(msgLen);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(rsl->result));
    offset += sizeof(rsl->result);
    OMCI_MEMCPY(outPdu + offset, &rsl->mask, sizeof(rsl->mask));
    offset += sizeof(rsl->mask);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        OMCI_MEMCPY(outPdu + offset, &rsl->optMask, sizeof(rsl->optMask));
        offset += sizeof(rsl->optMask);
        OMCI_MEMCPY(outPdu + offset, &rsl->exeMask, sizeof(rsl->exeMask));
        offset += sizeof(rsl->exeMask);
    }

    if (OMCI_BASE_DEVICE_ID == pHdr->devId)
    {
        offset += OMCI_MAX_GET_LEN;

        OMCI_MEMCPY(outPdu + offset, &rsl->optMask, sizeof(rsl->optMask));
        offset += sizeof(rsl->optMask);
        OMCI_MEMCPY(outPdu + offset, &rsl->exeMask, sizeof(rsl->exeMask));
        offset += sizeof(rsl->exeMask);
    }

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_mib_upload_rsp(ca_omci_uint8_t *pkt, ca_omci_uint16_t seqNum)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;
    ca_omci_uint16_t   num    = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_MIB_UPLOAD_RSP_MSG_BASE_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    num = ca_omci_htons(seqNum);
    OMCI_MEMCPY(outPdu + offset, &num, sizeof(ca_omci_uint16_t));
    offset += sizeof(ca_omci_uint16_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_get_all_alarms_rsp(ca_omci_uint8_t *pkt, ca_omci_uint16_t seqNum)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;
    ca_omci_uint16_t   num    = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_GET_ALL_AM_RSP_MSG_BASE_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    num = ca_omci_htons(seqNum);
    OMCI_MEMCPY(outPdu + offset, &num, sizeof(ca_omci_uint16_t));
    offset += sizeof(ca_omci_uint16_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_action_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_ACTION_RSP_MSG_BASE_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_start_sw_dl_rsp(
    ca_omci_uint8_t        *pkt,
    ca_omci_me_st_dl_rsl_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_ST_DL_CONT_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(outPdu + offset, &rsl->windowSize, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_dl_sec_rsp(
    ca_omci_uint8_t     *pkt,
    ca_omci_uint8_t      rsl,
    ca_omci_me_dl_sec_t *sec)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    OMCI_PARAM_IS_NULL(sec);

    if (1 != sec->isLast)
    {
        return;
    }

    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);
    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_DL_SEC_CONT_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(outPdu + offset, &sec->sectionNum, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_end_sw_dl_rsp(
    ca_omci_uint8_t *pkt,
    ca_omci_uint8_t  rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_END_DL_CONT_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_get_next_rsp(
    ca_omci_uint8_t             *outPdu,
    ca_omci_me_getnext_result_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(outPdu);
    pHdr = (ca_omci_pkt_hdr_t *)outPdu;

    // if attrlen is 0, add this value have no effect, so add it
    msgLen = OMCI_GET_NEXT_RSP_MSG_BASE_LEN + rsl->attrLen;

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(msgLen);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(rsl->result));
    offset += sizeof(rsl->result);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_get_curr_rsp(ca_omci_uint8_t *outPdu, ca_omci_me_get_result_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(outPdu);
    pHdr = (ca_omci_pkt_hdr_t *)outPdu;

    // if attrlen is 0, add this value have no effect, so add it
    msgLen       = OMCI_GET_RSP_MSG_BASE_LEN + rsl->attrLen + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t);
    rsl->mask    = ca_omci_htons(rsl->mask);
    rsl->optMask = ca_omci_htons(rsl->optMask);
    rsl->exeMask = ca_omci_htons(rsl->exeMask);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(msgLen);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(rsl->result));
    offset += sizeof(rsl->result);
    OMCI_MEMCPY(outPdu + offset, &rsl->mask, sizeof(rsl->mask));
    offset += sizeof(rsl->mask);

    if (OMCI_BASE_DEVICE_ID == pHdr->devId)
    {
        offset += OMCI_MAX_GET_LEN;
    }
    OMCI_MEMCPY(outPdu + offset, &rsl->optMask, sizeof(rsl->optMask));
    offset += sizeof(rsl->optMask);
    OMCI_MEMCPY(outPdu + offset, &rsl->exeMask, sizeof(rsl->exeMask));
    offset += sizeof(rsl->exeMask);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_sync_time_rsp(ca_omci_uint8_t *pkt, ca_omci_me_sync_time_rsl_t *rsl)
{
    ca_omci_pkt_hdr_t *pHdr   = NULL;
    ca_omci_uint8_t   *outPdu = NULL;
    ca_omci_uint16_t   msgLen = 0;
    ca_omci_uint16_t   offset = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext    = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(pkt);
    pHdr   = (ca_omci_pkt_hdr_t *)pkt;
    outPdu = ca_omci_pkt_tx_hdr_build(pkt);

    if (OMCI_EXT_DEVICE_ID == pHdr->devId)
    {
        msgLen = ca_omci_htons(OMCI_SYNC_TIME_RSP_MSG_LEN);
        OMCI_MEMCPY((outPdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        ext = CA_OMCI_TRUE;
    }

    OMCI_MEMCPY(outPdu + offset, &rsl->result, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(outPdu + offset, &rsl->rslInfo, sizeof(ca_omci_uint8_t));
    offset += sizeof(ca_omci_uint8_t);

    ca_omci_mic_build(outPdu, ext);
    ca_omci_pkt_tx(outPdu, ext);
}


void omci_codec_encode_dg_alarm(ca_omci_mebase_t *me)
{
    ca_omci_uint8_t    pdu[OMCI_PKT_MAX_LEN] = {0};
    ca_omci_pkt_hdr_t *pHdr                  = (ca_omci_pkt_hdr_t *)pdu ;
    ca_omci_uint16_t   msgLen                = 0;
    ca_omci_uint16_t   offset                = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_uint8_t    alarmSeq              = ca_omci_mng_alarm_seq_get();
    ca_omci_bool_t     ext                   = ca_omci_cfg_stack_ext_get();
    ca_omci_uint16_t   classId               = 0;
    ca_omci_uint16_t   len                   = OMCI_BASE_PKT_LEN;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(me);

    alarmSeq++;
    classId  = ca_omci_me_get_standard_class(me->classId);

    // The trans-id generated autonomously by an ONU is 0.
    pHdr->transId = 0;
    pHdr->msgType = OMCI_MSG_TYPE_ALARM;
    pHdr->classId = ca_omci_htons(classId);
    pHdr->instId  = ca_omci_htons(me->instanceId);

    if (CA_OMCI_TRUE == ext)
    {
        pHdr->devId = OMCI_EXT_DEVICE_ID;
        msgLen      = ca_omci_htons(OMCI_ALARM_RSP_CONT_LEN);
        OMCI_MEMCPY((pdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        len = offset + OMCI_ALARM_RSP_CONT_LEN + OMCI_PKT_MIC_LEN;
    }
    else
    {
        pHdr->devId = OMCI_BASE_DEVICE_ID;
    }

    OMCI_MEMCPY((pdu + offset), me->amBmp->stdAmBmp, OMCI_STD_ALARM_BMP_LEN);
    offset += OMCI_STD_ALARM_BMP_LEN;
    OMCI_MEMCPY((pdu + offset), me->amBmp->orgAmBmp, OMCI_ORG_ALARM_BMP_LEN);
    offset += OMCI_ORG_ALARM_BMP_LEN;

    if (CA_OMCI_FALSE == ext)
    {
        offset += OMCI_ALARM_ZERO_PADDING_LEN; /* For baseline msg, seq should be the 40 byte */
    }

    OMCI_MEMCPY((pdu + offset), &alarmSeq, sizeof(alarmSeq));

    /*build CRC and update the packet */
    ca_omci_mic_build(pdu, ext);
    ca_omci_hal_onu_dg_alarm_set((void *)&pdu, (void *)&len);
}


void omci_codec_encode_alarm(ca_omci_mebase_t *me)
{
    ca_omci_uint8_t    pdu[OMCI_PKT_MAX_LEN] = {0};
    ca_omci_pkt_hdr_t *pHdr                  = (ca_omci_pkt_hdr_t *)pdu ;
    ca_omci_uint16_t   msgLen                = 0;
    ca_omci_uint16_t   offset                = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_uint8_t    alarmSeq              = 0;
    ca_omci_bool_t     ext                   = ca_omci_cfg_stack_ext_get();
    ca_omci_uint16_t   classId               = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(me);

    ca_omci_mng_alarm_seq_inc();
    alarmSeq = ca_omci_mng_alarm_seq_get();
    classId  = ca_omci_me_get_standard_class(me->classId);

    // The trans-id generated autonomously by an ONU is 0. 
    pHdr->transId = 0;
    pHdr->msgType = OMCI_MSG_TYPE_ALARM;
    pHdr->classId = ca_omci_htons(classId);
    pHdr->instId  = ca_omci_htons(me->instanceId);

    if (CA_OMCI_TRUE == ext)
    {
        pHdr->devId = OMCI_EXT_DEVICE_ID;
        msgLen      = ca_omci_htons(OMCI_ALARM_RSP_CONT_LEN);
        OMCI_MEMCPY((pdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
    }
    else
    {
        pHdr->devId = OMCI_BASE_DEVICE_ID;
    }


    OMCI_MEMCPY((pdu + offset), me->amBmp->stdAmBmp, OMCI_STD_ALARM_BMP_LEN);
    offset += OMCI_STD_ALARM_BMP_LEN;
    OMCI_MEMCPY((pdu + offset), me->amBmp->orgAmBmp, OMCI_ORG_ALARM_BMP_LEN);
    offset += OMCI_ORG_ALARM_BMP_LEN;

    if (CA_OMCI_FALSE == ext)
    {
        offset += OMCI_ALARM_ZERO_PADDING_LEN; /* For baseline msg, seq should be the 40 byte */
    }

    OMCI_MEMCPY((pdu + offset), &alarmSeq, sizeof(alarmSeq));

    /*build CRC and send the packet */
    ca_omci_mic_build(pdu, ext);
    ca_omci_pkt_tx(pdu, ext);
}


void omci_codec_encode_avc(
    ca_omci_mebase_t  *me,
    ca_omci_uint16_t   mask,
    ca_omci_pkt_avc_t *pAvcAttr)
{
    ca_omci_uint8_t    pdu[OMCI_PKT_MAX_LEN] = {0};
    ca_omci_pkt_hdr_t *pHdr                  = (ca_omci_pkt_hdr_t *)pdu ;
    ca_omci_uint16_t   msgLen                = 0;
    ca_omci_uint16_t   offset                = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t     ext                   = ca_omci_cfg_stack_ext_get();
    ca_omci_uint16_t   classId               = 0;
    ca_omci_uint8_t    idx                   = 0;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(pAvcAttr);

    classId  = ca_omci_me_get_standard_class(me->classId);

    // The trans-id generated autonomously by an ONU is 0. 
    pHdr->transId = 0;
    pHdr->msgType = OMCI_MSG_TYPE_ATTR_VALUE_CHANGE;
    pHdr->classId = ca_omci_htons(classId);
    pHdr->instId  = ca_omci_htons(me->instanceId);

    //calculate message length
    msgLen = sizeof(ca_omci_uint16_t);
    for (idx = 0; idx < pAvcAttr->attrNum; idx++)
    {
        msgLen += pAvcAttr->attr[idx].attrLen;
    }

    if (CA_OMCI_TRUE == ext)
    {
        pHdr->devId = OMCI_EXT_DEVICE_ID;
        OMCI_MEMCPY((pdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
    }
    else
    {
        pHdr->devId = OMCI_BASE_DEVICE_ID;
    }

    /*build mask and value */
    mask = ca_omci_htons(mask);
    OMCI_MEMCPY((pdu + offset), &mask, sizeof(ca_omci_uint16_t));
    offset += sizeof(ca_omci_uint16_t);
    for (idx = 0; idx < pAvcAttr->attrNum; idx++)
    {
        OMCI_MEMCPY((pdu + offset), pAvcAttr->attr[idx].pAttr, pAvcAttr->attr[idx].attrLen);
        offset += pAvcAttr->attr[idx].attrLen;
    }

    /*build CRC and send the packet */
    ca_omci_mic_build(pdu, ext);
    ca_omci_pkt_tx(pdu, ext);
}


void omci_codec_encode_test_rsl(
    ca_omci_mebase_t *me,
    void             *pData)
{
    ca_omci_uint8_t         pdu[OMCI_PKT_MAX_LEN] = {0};
    ca_omci_pkt_hdr_t      *pHdr                  = (ca_omci_pkt_hdr_t *)pdu ;
    ca_omci_uint16_t        msgLen                = 0;
    ca_omci_uint16_t        offset                = sizeof(ca_omci_pkt_hdr_t);
    ca_omci_bool_t          ext                   = ca_omci_cfg_stack_ext_get();
    ca_omci_uint16_t        classId               = 0;
    ca_omci_uint8_t         valueType             = 0;
    ca_omci_uint16_t        value                 = 0;
    ca_omci_hal_pon_info_t *ponInfo               = (ca_omci_hal_pon_info_t *)pData;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(pData);

    classId  = ca_omci_me_get_standard_class(me->classId);

    // The trans-id generated autonomously by an ONU is 0. 
    pHdr->transId = 0;
    pHdr->msgType = OMCI_MSG_TYPE_TEST_RESULT;
    pHdr->classId = ca_omci_htons(classId);
    pHdr->instId  = ca_omci_htons(me->instanceId);
    msgLen        = ca_omci_htons(OMCI_TEST_RSL_CONT_LEN);

    if (CA_OMCI_TRUE == ext)
    {
        pHdr->devId = OMCI_EXT_DEVICE_ID;
        OMCI_MEMCPY((pdu + offset), &msgLen, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
    }
    else
    {
        pHdr->devId = OMCI_BASE_DEVICE_ID;
    }

    /*build content */
    valueType = OMCI_TEST_RSL_VOLTAGE;
    OMCI_MEMCPY((pdu + offset), &valueType, sizeof(valueType));
    offset += sizeof(valueType);
    value = ca_omci_htons(ponInfo->voltage);
    OMCI_MEMCPY((pdu + offset), &value, sizeof(value));
    offset += sizeof(value);

    valueType = OMCI_TEST_RSL_RX_POWER;
    OMCI_MEMCPY((pdu + offset), &valueType, sizeof(valueType));
    offset += sizeof(valueType);
    value = ca_omci_htons(ponInfo->rxPowerDbm);
    OMCI_MEMCPY((pdu + offset), &value, sizeof(value));
    offset += sizeof(value);

    valueType = OMCI_TEST_RSL_MEAN_LAUNCH_POWER;
    OMCI_MEMCPY((pdu + offset), &valueType, sizeof(valueType));
    offset += sizeof(valueType);
    value = ca_omci_htons(ponInfo->minLaunchPowerDbm);
    OMCI_MEMCPY((pdu + offset), &value, sizeof(value));
    offset += sizeof(value);

    valueType = OMCI_TEST_RSL_LASER_BIAS_CURRENT;
    OMCI_MEMCPY((pdu + offset), &valueType, sizeof(valueType));
    offset += sizeof(valueType);
    value = ca_omci_htons(ponInfo->laserBiasCurrent);
    OMCI_MEMCPY((pdu + offset), &value, sizeof(value));
    offset += sizeof(value);

    valueType = OMCI_TEST_RSL_TEMPERATURE;
    OMCI_MEMCPY((pdu + offset), &valueType, sizeof(valueType));
    offset += sizeof(valueType);
    value = ca_omci_htons(ponInfo->temperature);
    OMCI_MEMCPY((pdu + offset), &value, sizeof(value));
    offset += sizeof(value);

    /*build CRC and send the packet */
    ca_omci_mic_build(pdu, ext);
    ca_omci_pkt_tx(pdu, ext);
}



