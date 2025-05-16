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
#include "omci_me_mac_port_filter.h"


static ca_omci_me_handler_t sg_omci_me_mac_port_filter_handlers = {0};


ca_omci_me_attr_t g_omci_me_mac_port_filter_attr[OMCI_ME_ATTR1] =
{
    {OMCI_ME_ATTR1, "mac_filter_tbl"},
};


ca_omci_mebase_t *ca_omci_me_mac_port_filter_new(void)
{
    ca_omci_mac_port_filter_t *me = (ca_omci_mac_port_filter_t *)omci_malloc(sizeof(ca_omci_mac_port_filter_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MAC_PORT_FILTER_CLASS_ID, &sg_omci_me_mac_port_filter_handlers);

    /* This me don't need to mib upload */
    me->meBase.mibUploadCnt = 0;

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_mac_port_filter_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_mac_port_filter_t      *macPortFilter = (ca_omci_mac_port_filter_t *)me;
    ca_omci_mac_port_filter_attr_t  newVal;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint8_t  idx     = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_mac_port_filter_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_MAC_PORT_FILTER_SET_MASK_FULL);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.macFilterTbl[0].index, "index");
        OMCI_SET_TBL_UINT8(newVal.macFilterTbl[0].filter, "filter");
        OMCI_SET_TBL_UINT8_T(newVal.macFilterTbl[0].macAddr, "macAddr", OMCI_MAC_ADDR_LEN);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR1, mask);
    }
    //TODO: update table attribute and call API
    if (0x80 == (newVal.macFilterTbl[0].filter & 0xC0))
    {
        // Clear entire table
        OMCI_MEMSET(macPortFilter->attr.macFilterTbl, 0, sizeof(macPortFilter->attr.macFilterTbl));
        return CA_OMCI_OK;
    }

    if (0 == (newVal.macFilterTbl[0].filter & 0xC0))
    {
        //Remove entry
        for (idx = 0; idx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; idx++)
        {
            if (newVal.macFilterTbl[0].index == macPortFilter->attr.macFilterTbl[idx].index)
            {
                OMCI_MEMSET(&macPortFilter->attr.macFilterTbl[idx], 0, sizeof(ca_omci_mac_filter_tbl_t));
                return CA_OMCI_OK;
            }
        }
    }

    if (0x40 == (newVal.macFilterTbl[0].filter & 0xC0))
    {
        //Add entry
        for (idx = 0; idx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; idx++)
        {
            if (CA_OMCI_FALSE == macPortFilter->attr.macFilterTbl[idx].valid)
            {
                macPortFilter->attr.macFilterTbl[idx].valid  = CA_OMCI_TRUE;
                macPortFilter->attr.macFilterTbl[idx].index  = newVal.macFilterTbl[0].index;
                macPortFilter->attr.macFilterTbl[idx].filter = newVal.macFilterTbl[0].filter;
                OMCI_MEMCPY(macPortFilter->attr.macFilterTbl[idx].macAddr, newVal.macFilterTbl[0].macAddr, OMCI_MAC_ADDR_LEN);
                return CA_OMCI_OK;
            }
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_filter_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_mac_port_filter_t *macPortFilter = (ca_omci_mac_port_filter_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MAC_PORT_FILTER_GET_MASK_FULL);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(macPortFilter->attr.macFilterTbl[0]) - sizeof(macPortFilter->attr.macFilterTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == macPortFilter->attr.macFilterTbl[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR1, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&macPortFilter->macFilterTbl, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_filter_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_mac_port_filter_t *macPortFilter = (ca_omci_mac_port_filter_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(macPortFilter->attr.macFilterTbl[tblIdx]) - sizeof(macPortFilter->attr.macFilterTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&macPortFilter->macFilterTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == macPortFilter->attr.macFilterTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == macPortFilter->attr.macFilterTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT8(macPortFilter->attr.macFilterTbl[tblIdx].index);
                OMCI_CP_TBL_UINT8(macPortFilter->attr.macFilterTbl[tblIdx].filter);
                OMCI_MEMCPY((tblAttr + offset), &macPortFilter->attr.macFilterTbl[tblIdx].macAddr, sizeof(macPortFilter->attr.macFilterTbl[tblIdx].macAddr));
                offset += sizeof(macPortFilter->attr.macFilterTbl[tblIdx].macAddr);
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
        ca_omci_next_success(&macPortFilter->macFilterTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_mac_port_filter_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_mac_port_filter_t *macPortFilter = (ca_omci_mac_port_filter_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(macPortFilter);
    for (idx = 0; idx < OMCI_MAC_PORT_FILTER_MAC_FILTER_TBL; idx++)
    {
        if (CA_OMCI_TRUE == macPortFilter->attr.macFilterTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(macPortFilter->attr.macFilterTbl[idx].index);
            OMCI_UPLOAD_TBL_UINT8(macPortFilter->attr.macFilterTbl[idx].filter);
            OMCI_UPLOAD_TBL_UINT8(macPortFilter->attr.macFilterTbl[idx].macAddr);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR1, mask);
        }
    }
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_mac_port_filter_init(void)
{
    sg_omci_me_mac_port_filter_handlers.uploadProc = ca_omci_me_mac_port_filter_upload;

    sg_omci_me_mac_port_filter_handlers.setProc     = ca_omci_me_mac_port_filter_set;
    sg_omci_me_mac_port_filter_handlers.getProc     = ca_omci_me_mac_port_filter_get;
    sg_omci_me_mac_port_filter_handlers.getNextProc = ca_omci_me_mac_port_filter_get_next;
}

