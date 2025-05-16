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
#include "omci_me_dot1ag_mep_ccm.h"


static ca_omci_me_handler_t sg_omci_me_dot1ag_mep_ccm_handlers = {0};


ca_omci_me_attr_t g_omci_me_dot1ag_mep_ccm_attr[OMCI_ME_ATTR12] =
{
    {OMCI_ME_ATTR1,  "rmep_db_tbl_1"},
    {OMCI_ME_ATTR2,  "rmep_db_tbl_2"},
    {OMCI_ME_ATTR3,  "rmep_db_tbl_3"},
    {OMCI_ME_ATTR4,  "rmep_db_tbl_4"},
    {OMCI_ME_ATTR5,  "rmep_db_tbl_5"},
    {OMCI_ME_ATTR6,  "rmep_db_tbl_6"},
    {OMCI_ME_ATTR7,  "rmep_db_tbl_7"},
    {OMCI_ME_ATTR8,  "rmep_db_tbl_8"},
    {OMCI_ME_ATTR9,  "rmep_db_tbl_9"},
    {OMCI_ME_ATTR10, "rmep_db_tbl_10"},
    {OMCI_ME_ATTR11, "rmep_db_tbl_11"},
    {OMCI_ME_ATTR12, "rmep_db_tbl_12"},
};


ca_omci_mebase_t *ca_omci_me_dot1ag_mep_ccm_new(void)
{
    ca_omci_dot1ag_mep_ccm_t *me = (ca_omci_dot1ag_mep_ccm_t *)omci_malloc(sizeof(ca_omci_dot1ag_mep_ccm_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID, &sg_omci_me_dot1ag_mep_ccm_handlers);

    /* This me don't need to mib upload */
    me->meBase.mibUploadCnt = 0;

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_dot1ag_mep_ccm_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_dot1ag_mep_ccm_t *dot1agMepCcm = (ca_omci_dot1ag_mep_ccm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_DOT1AG_MEP_CCM_GET_MASK_FULL);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl1[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl1[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl1[tblIdx].valid)
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
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl1, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl2[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl2[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl2[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR2, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl2, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR3, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl3[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl3[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl3[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR3, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl3, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR4, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl4[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl4[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl4[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR4, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl4, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl5[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl5[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl5[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl5, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl6[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl6[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl6[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl6, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl7[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl7[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl7[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl7, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl8[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl8[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl8[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR8, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl8, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl9[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl9[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl9[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl9, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl10[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl10[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl10[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl10, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl11[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl11[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl11[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl11, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl12[0]) - sizeof(dot1agMepCcm->attr.rmepDbTbl12[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl12[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&dot1agMepCcm->rmepDbTbl12, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_dot1ag_mep_ccm_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_dot1ag_mep_ccm_t *dot1agMepCcm = (ca_omci_dot1ag_mep_ccm_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl1, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl1[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl1[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl1[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl1[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl1[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl1);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl2, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl2[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl2[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl2[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl2[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl2[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl2);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR3, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl3, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl3[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl3[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl3[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl3[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl3[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl3);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR4, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl4, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl4[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl4[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl4[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl4[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl4[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl4);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl5, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl5[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl5[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl5[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl5[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl5[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl5);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl6, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl6[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl6[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl6[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl6[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl6[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl6);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl7, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl7[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl7[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl7[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl7[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl7[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl7);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl8, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl8[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl8[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl8[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl8[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl8[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl8);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl9, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl9[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl9[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl9[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl9[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl9[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl9);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl10, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl10[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl10[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl10[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl10[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl10[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl10);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl11, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl11[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl11[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl11[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl11[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl11[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl11);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx]) - sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&dot1agMepCcm->rmepDbTbl12, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl12[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_RMEP_TABLE_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == dot1agMepCcm->attr.rmepDbTbl12[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].rmepId);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].rmepSt);
                OMCI_CP_TBL_UINT32(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].failOkTime);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl12[tblIdx].macAddr, sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].macAddr));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].macAddr);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].rdi);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].portSt);
                OMCI_CP_TBL_UINT8(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].ifSt);
                OMCI_MEMCPY((tblAttr + offset), &dot1agMepCcm->attr.rmepDbTbl12[tblIdx].senderIdTlv, sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].senderIdTlv));
                offset += sizeof(dot1agMepCcm->attr.rmepDbTbl12[tblIdx].senderIdTlv);
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
        ca_omci_next_success(&dot1agMepCcm->rmepDbTbl12);
    }
    return CA_OMCI_OK;
}


void ca_omci_me_dot1ag_mep_ccm_init(void)
{
    sg_omci_me_dot1ag_mep_ccm_handlers.getProc     = ca_omci_me_dot1ag_mep_ccm_get;
    sg_omci_me_dot1ag_mep_ccm_handlers.getNextProc = ca_omci_me_dot1ag_mep_ccm_get_next;
}

