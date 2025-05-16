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
#include "omci_mq.h"
#include "omci_me_base.h"
#include "omci_codec_pkt.h"
#include "omci_codec_mic.h"
#include "omci_hal.h"
#include "omci_cfg.h"
#include "omci_mic.h"


static ca_omci_uint8_t  sg_rspPkt[OMCI_SAVED_PKT_NUM][OMCI_PKT_MAX_LEN]; // 2 packets enough now
static ca_omci_uint8_t  sg_rspIdx      = 0;
static ca_omci_uint16_t sg_lastTransId = 0;


extern ca_omci_bool_t g_smaEnable;
extern ca_omci_st_e ca_omci_main_pkt_send(ca_omci_uint8_t *pkt, ca_omci_uint32_t len);
extern void ca_omci_pkt_tx_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);
extern void ca_omci_pkt_rx_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);
extern void ca_omci_pkt_rx_info_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);
extern void ca_omci_pkt_tx_info_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);
extern void ca_omci_pkt_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);


ca_omci_st_e ca_omci_pkt_check(ca_omci_uint8_t *pkt)
{
    ca_omci_pkt_hdr_t  *pHdr    = (ca_omci_pkt_hdr_t *)pkt;

    OMCI_PARAM_NULL_RETURN(pkt);

    /* check packet DB AK bit first */
    if (OMCI_DB_FLAG(pHdr->msgType) || OMCI_AK_FLAG(pHdr->msgType))
    {
        OMCICOERR("msgType = [0x%x]", pHdr->msgType);
        return CA_OMCI_E_PARAM;
    }

    /* Update MIC key if MIC check failed for the first time */
    if (CA_OMCI_OK != ca_omci_mic_check(pkt))
    {
        if (CA_OMCI_FALSE == g_smaEnable)
        {
            ca_omci_gen_mic_init();
            return ca_omci_mic_check(pkt);
        }
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_uint8_t *ca_omci_pkt_tx_hdr_build(ca_omci_uint8_t *inPdu)
{
    ca_omci_uint8_t   *buf    = sg_rspPkt[sg_rspIdx == 0 ? 1 : 0];
    ca_omci_pkt_hdr_t *outPdu = (ca_omci_pkt_hdr_t *)buf;


    OMCI_MEMSET(buf, 0x00, OMCI_PKT_MAX_LEN);
    OMCI_MEMCPY(buf, inPdu, sizeof(ca_omci_pkt_hdr_t));

    outPdu->msgType &= 0xbf;
    outPdu->msgType |= 0x20;
    return buf;
}


ca_omci_st_e ca_omci_pkt_tx(ca_omci_uint8_t *buffer, ca_omci_bool_t ext)
{
    ca_omci_uint32_t       len     = OMCI_BASE_PKT_LEN;
    ca_omci_ext_pkt_hdr_t *pExtHdr = NULL;
    ca_omci_pkt_hdr_t     *pHdr    = (ca_omci_pkt_hdr_t *)buffer;

    OMCI_PARAM_NULL_RETURN(buffer);
    if (CA_OMCI_TRUE != ca_omci_cfg_omcc_state_get())
    {
        OMCICODBG("omcc link down, shouldn't rsp OMCI pkt");
        return CA_OMCI_OK;
    }

    if (CA_OMCI_TRUE == ext)
    {
        pExtHdr = (ca_omci_ext_pkt_hdr_t *)buffer;
        len     = ca_omci_ntohs(pExtHdr->len) + sizeof(ca_omci_ext_pkt_hdr_t) + OMCI_PKT_MIC_LEN;
    }

    ca_omci_pkt_tx_hex_print(buffer, len);
    ca_omci_pkt_tx_info_print(buffer, len);
    sg_rspIdx      = ((sg_rspIdx == 1) ? 0 : 1);
    sg_lastTransId = ca_omci_ntohs(pHdr->transId);
    ca_omci_hal_pkt_tx(buffer, len);

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_pkt_tx_for_cli(ca_omci_uint8_t *buffer, ca_omci_bool_t ext)
{
    ca_omci_uint32_t       len     = OMCI_BASE_PKT_LEN;
    ca_omci_ext_pkt_hdr_t *pExtHdr = NULL;

    OMCI_PARAM_NULL_RETURN(buffer);
    if (CA_OMCI_TRUE != ca_omci_cfg_omcc_state_get())
    {
        OMCICODBG("omcc link down, shouldn't rsp OMCI pkt");
        return CA_OMCI_OK;
    }

    if (CA_OMCI_TRUE == ext)
    {
        pExtHdr = (ca_omci_ext_pkt_hdr_t *)buffer;
        len     = ca_omci_ntohs(pExtHdr->len) + sizeof(ca_omci_ext_pkt_hdr_t) + OMCI_PKT_MIC_LEN;
    }

    sg_rspIdx = ((sg_rspIdx == 1) ? 0 : 1);
    ca_omci_main_pkt_send(buffer, len);
    return CA_OMCI_OK;
}


void ca_omci_pkt_handle(void *pData)
{
    ca_omci_inner_msg_t *pInnerMsg = (ca_omci_inner_msg_t *)pData;
    ca_omci_pkt_hdr_t   *pHdr      = NULL;

    OMCI_PARAM_IS_NULL(pData);
    ca_omci_pkt_rx_hex_print(pInnerMsg->data, pInnerMsg->hdr.dataLen);
    ca_omci_pkt_rx_info_print(pInnerMsg->data, pInnerMsg->hdr.dataLen);

    if (CA_OMCI_OK != ca_omci_pkt_check(pInnerMsg->data))
    {
        OMCICOERR("receive invalid packet");
        printk("\r\n=====================================[rx]=====================================");
        ca_omci_pkt_hex_print(pInnerMsg->data, pInnerMsg->hdr.dataLen);
        return;
    }

    /*
        ** If the request packet has the same transaction identifier as
        ** the last processed, it is assumed as a retransmitted packet.
        ** Just resend response packet if exists, otherwise handle the packet.
        ** Exclude download section from applying the rule to respond properly
        ** for entire retransmitted segment.
     */
    pHdr = (ca_omci_pkt_hdr_t *)pInnerMsg->data;

    if ((OMCI_MSG_TYPE_DOWNLOAD_SEC != (pHdr->msgType & 0x1f))
     && (0 != OMCI_AR_FLAG(pHdr->msgType))
     && (sg_lastTransId == ca_omci_ntohs(pHdr->transId)))
    {
        OMCICOERR("retransmitted packet, reply it directly\n");
        ca_omci_pkt_tx((ca_omci_uint8_t *)sg_rspPkt[sg_rspIdx], pHdr->devId == OMCI_EXT_DEVICE_ID);

        /* If transaction is same, don't change the sg_rspIdx */
        sg_rspIdx = (sg_rspIdx == 1 ? 0 : 1);
        return;
    }

    sg_lastTransId = ca_omci_ntohs(pHdr->transId);
    if (CA_OMCI_OK != ca_omci_decode_pkt((void *)pInnerMsg->data))
    {
        printk("\r\n=====================================[rx]=====================================");
        ca_omci_pkt_hex_print(pInnerMsg->data, pInnerMsg->hdr.dataLen);
    }
}


