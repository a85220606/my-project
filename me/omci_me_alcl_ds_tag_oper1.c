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
#include "omci_me_alcl_ds_tag_oper1.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_alcl_ds_tag_oper1_handlers = {0};


ca_omci_me_attr_t g_omci_me_alcl_ds_tag_oper1_attr[OMCI_ME_ATTR2] =
{
    {OMCI_ME_ATTR1, "uni_def_behavior"},
    {OMCI_ME_ATTR2, "uni_def_behavior_tbl"},
};


ca_omci_uint32_t ca_omci_me_alcl_ds_tag_oper1_attr_len_get(void)
{
    ca_omci_alcl_ds_tag_oper1_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.uniDefBehavior);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_alcl_ds_tag_oper1_new(void)
{
    ca_omci_alcl_ds_tag_oper1_t *me = (ca_omci_alcl_ds_tag_oper1_t *)omci_malloc(sizeof(ca_omci_alcl_ds_tag_oper1_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID, &sg_omci_me_alcl_ds_tag_oper1_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(alcl_ds_tag_oper1);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_alcl_ds_tag_oper1_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_alcl_ds_tag_oper1_t      *alclDsTagOper1 = (ca_omci_alcl_ds_tag_oper1_t *)me;
    ca_omci_alcl_ds_tag_oper1_attr_t  newVal;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint8_t  idx  = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_alcl_ds_tag_oper1_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_ALCL_DS_TAG_OPER1_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, newVal.uniDefBehavior, "uniDefBehavior", 1);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.uniDefBehaviorTbl[0].entryNum, "entryNum");
        OMCI_SET_TBL_UINT8(newVal.uniDefBehaviorTbl[0].vlanAct, "vlanAct");
        OMCI_SET_TBL_UINT16(newVal.uniDefBehaviorTbl[0].vid, "vid");
        OMCI_SET_TBL_UINT16(newVal.uniDefBehaviorTbl[0].reserved, "reserved");

        if (0x80 == (newVal.uniDefBehaviorTbl[0].vlanAct & 0x80))
        {
            //ADD or update an entry
            for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
            {
                if ((CA_OMCI_TRUE == alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid)
                 && (alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid == newVal.uniDefBehaviorTbl[0].vid))
                {
                    //update the entry
                    alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vlanAct = newVal.uniDefBehaviorTbl[0].vlanAct;
                    break;
                }
            }
            if (OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL == idx)
            {
                // add a new entry
                for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
                {
                    if (CA_OMCI_FALSE == alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid)
                    {
                        alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid   = CA_OMCI_TRUE;
                        alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid     = newVal.uniDefBehaviorTbl[0].vid;
                        alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vlanAct = newVal.uniDefBehaviorTbl[0].vlanAct;
                        break;
                    }
                }
            }
        }
        else
        {
            //remove a entry
            for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
            {
                if ((CA_OMCI_TRUE == alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid)
                 && (alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid == newVal.uniDefBehaviorTbl[0].vid))
                {
                    //remove this entry
                    alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid   = CA_OMCI_FALSE;
                    alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid     = 0;
                    alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vlanAct = 0;
                    break;
                }
            }
            if (OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL == idx)
            {
                OMCIERR("Can't remove a non-exist entry");
                return CA_OMCI_E;
            }
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_alcl_ds_tag_oper1_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_alcl_ds_tag_oper1_t *alclDsTagOper1 = (ca_omci_alcl_ds_tag_oper1_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ALCL_DS_TAG_OPER1_GET_MASK_FULL);

    //TODO: maybe need update value in me

    OMCI_GET_ATTR_UINT8(alclDsTagOper1->attr.uniDefBehavior, 1);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(alclDsTagOper1->attr.uniDefBehaviorTbl[0]) - sizeof(alclDsTagOper1->attr.uniDefBehaviorTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == alclDsTagOper1->attr.uniDefBehaviorTbl[tblIdx].valid)
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
        ca_omci_next_init(&alclDsTagOper1->uniDefBehaviorTbl, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_alcl_ds_tag_oper1_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_alcl_ds_tag_oper1_t *alclDsTagOper1 = (ca_omci_alcl_ds_tag_oper1_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(alclDsTagOper1);
    OMCI_UPLOAD_ATTR_SIZE(8, alclDsTagOper1->attr.uniDefBehavior, 1);
    for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
    {
        if (CA_OMCI_TRUE == alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(alclDsTagOper1->attr.uniDefBehaviorTbl[idx].entryNum);
            OMCI_UPLOAD_TBL_UINT8(alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vlanAct);
            OMCI_UPLOAD_TBL_UINT16(alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid);
            OMCI_UPLOAD_TBL_UINT16(alclDsTagOper1->attr.uniDefBehaviorTbl[idx].reserved);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR2, mask);
        }
    }
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_alcl_ds_tag_oper1_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_alcl_ds_tag_oper1_t *alclDsTagOper1 = (ca_omci_alcl_ds_tag_oper1_t *)me;
    ca_omci_uint32_t idx = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: alclDsTagOper1 \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        uniDefBehavior: 0x%x\n", alclDsTagOper1->attr.uniDefBehavior);
    for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
    {
        if (CA_OMCI_FALSE == alclDsTagOper1->attr.uniDefBehaviorTbl[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        uniDefBehaviorTbl[%d]:entryNum 0x%x\n", idx, alclDsTagOper1->attr.uniDefBehaviorTbl[idx].entryNum);
        omci_sprint(cli, "        uniDefBehaviorTbl[%d]:vlanAct 0x%x\n", idx, alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vlanAct);
        omci_sprint(cli, "        uniDefBehaviorTbl[%d]:vid 0x%x\n", idx, alclDsTagOper1->attr.uniDefBehaviorTbl[idx].vid);
        omci_sprint(cli, "        uniDefBehaviorTbl[%d]:reserved 0x%x\n", idx, alclDsTagOper1->attr.uniDefBehaviorTbl[idx].reserved);
    }
}


void ca_omci_me_alcl_ds_tag_oper1_init(void)
{
    sg_omci_me_alcl_ds_tag_oper1_handlers.meDisplay  = ca_omci_me_alcl_ds_tag_oper1_display;
    sg_omci_me_alcl_ds_tag_oper1_handlers.uploadProc = ca_omci_me_alcl_ds_tag_oper1_upload;

    sg_omci_me_alcl_ds_tag_oper1_handlers.setProc    = ca_omci_me_alcl_ds_tag_oper1_set;
    sg_omci_me_alcl_ds_tag_oper1_handlers.getProc    = ca_omci_me_alcl_ds_tag_oper1_get;
}

