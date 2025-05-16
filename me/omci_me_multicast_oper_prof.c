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
#include "omci_me_multicast_oper_prof.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_multicast_oper_prof_handlers = {0};


ca_omci_me_attr_t g_omci_me_multicast_oper_prof_attr[OMCI_ME_ATTR16] =
{
    {OMCI_ME_ATTR1,  "igmp_ver"},
    {OMCI_ME_ATTR2,  "igmp_func"},
    {OMCI_ME_ATTR3,  "immedi_leave"},
    {OMCI_ME_ATTR4,  "us_tci"},
    {OMCI_ME_ATTR5,  "us_tag_ctl"},
    {OMCI_ME_ATTR6,  "us_rate"},
    {OMCI_ME_ATTR7,  "dynamic_tbl"},
    {OMCI_ME_ATTR8,  "static_tbl"},
    {OMCI_ME_ATTR9,  "lost_tbl"},
    {OMCI_ME_ATTR10, "robust"},
    {OMCI_ME_ATTR11, "query_ip_addr"},
    {OMCI_ME_ATTR12, "query_interval"},
    {OMCI_ME_ATTR13, "query_max_rsp_time"},
    {OMCI_ME_ATTR14, "last_mb_query_itv"},
    {OMCI_ME_ATTR15, "unauth_join_req_beh"},
    {OMCI_ME_ATTR16, "ds_tci"},
};


ca_omci_uint32_t ca_omci_me_multicast_oper_prof_attr_len_get(void)
{
    ca_omci_multicast_oper_prof_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.igmpVer);
    attrLen += sizeof(me->attr.igmpFunc);
    attrLen += sizeof(me->attr.immediLeave);
    attrLen += sizeof(me->attr.usTci);
    attrLen += sizeof(me->attr.usTagCtl);
    attrLen += sizeof(me->attr.usRate);
    attrLen += sizeof(me->attr.robust);
    attrLen += sizeof(me->attr.queryIpAddr);
    attrLen += sizeof(me->attr.queryItv);
    attrLen += sizeof(me->attr.queryMaxRspT);
    attrLen += sizeof(me->attr.lastMbQueryItv);
    attrLen += sizeof(me->attr.unauthJoinReqBehave);
    attrLen += sizeof(me->attr.dsTci);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_multicast_oper_prof_new(void)
{
    ca_omci_multicast_oper_prof_t *me = (ca_omci_multicast_oper_prof_t *)omci_malloc(sizeof(ca_omci_multicast_oper_prof_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, &sg_omci_me_multicast_oper_prof_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(multicast_oper_prof);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_multicast_oper_prof_t *multiOperProf = (ca_omci_multicast_oper_prof_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(multiOperProf->attr.igmpVer, "igmpVer", 1);
    OMCI_SET_ATTR_UINT8(multiOperProf->attr.igmpFunc, "igmpFunc", 2);
    OMCI_SET_ATTR_UINT8(multiOperProf->attr.immediLeave, "immediLeave", 3);
    OMCI_SET_ATTR_UINT16(multiOperProf->attr.usTci, "usTci", 4);
    OMCI_SET_ATTR_UINT8(multiOperProf->attr.usTagCtl, "usTagCtl", 5);
    OMCI_SET_ATTR_UINT32(multiOperProf->attr.usRate, "usRate", 6);
    OMCI_SET_ATTR_UINT8(multiOperProf->attr.robust, "robust", 10);
    OMCI_SET_ATTR_UINT32(multiOperProf->attr.queryIpAddr, "queryIpAddr", 11);
    OMCI_SET_ATTR_UINT32(multiOperProf->attr.queryItv, "queryItv", 12);
    OMCI_SET_ATTR_UINT32(multiOperProf->attr.queryMaxRspT, "queryMaxRspT", 13);
    OMCI_SET_TBL_UINT8(multiOperProf->attr.dsTci.ctl, "ctl");
    OMCI_SET_TBL_UINT16(multiOperProf->attr.dsTci.tci, "tci");
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR16, mask);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_multicast_oper_prof_t      *multiOperProf = (ca_omci_multicast_oper_prof_t *)me;
    ca_omci_multicast_oper_prof_attr_t  newVal;
    ca_omci_uint16_t        offset       = 0;
    ca_omci_uint16_t        mask         = 0;
    ca_omci_uint8_t         idx          = 0;
    ca_omci_acl_tbl_info_t *dynAclTbl    = NULL;
    ca_omci_acl_tbl_info_t *oriDynAclTbl = NULL;
    ca_omci_acl_ctl_t      *dynAclCtl    = NULL;
    ca_omci_acl_ctl_t      *oriDynAclCtl = NULL;
    ca_omci_acl_tbl_info_t *staAclTbl    = NULL;
    ca_omci_acl_tbl_info_t *oriStaAclTbl = NULL;
    ca_omci_acl_ctl_t      *staAclCtl    = NULL;
    ca_omci_acl_ctl_t      *oriStaAclCtl = NULL;
    ca_omci_uint16_t        tblCtl       = 0;
    ca_omci_uint16_t        oriDynTblCtl = 0;
    ca_omci_uint16_t        oriStaTblCtl = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_multicast_oper_prof_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_MULTICAST_OPER_PROF_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.igmpVer, "igmpVer", 1);
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.igmpFunc, "igmpFunc", 2);
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.immediLeave, "immediLeave", 3);
    OMCI_UPDATE_ATTR_UINT(16, multiOperProf->attr.usTci, "usTci", 4);
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.usTagCtl, "usTagCtl", 5);
    OMCI_UPDATE_ATTR_UINT(32, multiOperProf->attr.usRate, "usRate", 6);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, inMask))
    {
        OMCI_SET_TBL_UINT8_T(newVal.dynamicTbl[0].tblCtlData, "tblCtlData", OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, inMask))
    {
        OMCI_SET_TBL_UINT8_T(newVal.staticTbl[0].tblCtlData, "tblCtlData", OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR8, mask);
    }
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.robust, "robust", 10);
    OMCI_UPDATE_ATTR_UINT(32, multiOperProf->attr.queryIpAddr, "queryIpAddr", 11);
    OMCI_UPDATE_ATTR_UINT(32, multiOperProf->attr.queryItv, "queryItv", 12);
    OMCI_UPDATE_ATTR_UINT(32, multiOperProf->attr.queryMaxRspT, "queryMaxRspT", 13);
    OMCI_UPDATE_ATTR_UINT(32, multiOperProf->attr.lastMbQueryItv, "lastMbQueryItv", 14);
    OMCI_UPDATE_ATTR_UINT(8, multiOperProf->attr.unauthJoinReqBehave, "unauthJoinReqBehave", 15);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR16, inMask))
    {
        OMCI_SET_TBL_UINT8(multiOperProf->attr.dsTci.ctl, "ctl");
        OMCI_SET_TBL_UINT16(multiOperProf->attr.dsTci.tci, "tci");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR16, mask);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        dynAclTbl = (ca_omci_acl_tbl_info_t *)(&(newVal.dynamicTbl[0].tblCtlData));
        tblCtl    = ca_omci_ntohs(dynAclTbl->tblCtl);
        dynAclCtl = (ca_omci_acl_ctl_t *)(&tblCtl);
        if (MULTICAST_CFG_TBL_CTL_DEL_ALL == dynAclCtl->setCtl)
        {
            OMCIMNGDBG("Remove all dynamic row");
            OMCI_MEMSET(multiOperProf->attr.dynamicTbl, 0, sizeof(ca_omci_acl_tbl_t) * OMCI_IGMP_MAX_TBL_NUM);
        }
        else if (MULTICAST_CFG_TBL_CTL_DEL == dynAclCtl->setCtl)
        {
            for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
            {
                oriDynAclTbl = (ca_omci_acl_tbl_info_t *)(&(multiOperProf->attr.dynamicTbl[idx].tblCtlData));
                oriDynTblCtl = ca_omci_ntohs(oriDynAclTbl->tblCtl);
                oriDynAclCtl = (ca_omci_acl_ctl_t *)(&oriDynTblCtl);

                //if this rowkey configure before, get it and del it
                if ((CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[idx].valid)
                 && (oriDynAclCtl->rowKey == dynAclCtl->rowKey))
                {
                    OMCIMNGDBG("Remove dynamic row key[%d]", oriDynAclCtl->rowKey);
                    OMCI_MEMSET(&multiOperProf->attr.dynamicTbl[idx], 0, sizeof(ca_omci_acl_tbl_t));
                    break;
                }
            }
        }
        else
        {
            //update the table row
            for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
            {
                oriDynAclTbl = (ca_omci_acl_tbl_info_t *)(&(multiOperProf->attr.dynamicTbl[idx].tblCtlData));
                oriDynTblCtl = ca_omci_ntohs(oriDynAclTbl->tblCtl);
                oriDynAclCtl = (ca_omci_acl_ctl_t *)(&oriDynTblCtl);

                //if this rowkey configure before, get it and update it
                if ((CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[idx].valid)
                 && (oriDynAclCtl->rowKey == dynAclCtl->rowKey))
                {
                    OMCIMNGDBG("Update dynamic row key[%d]", oriDynAclCtl->rowKey);
                    OMCI_MEMCPY(multiOperProf->attr.dynamicTbl[idx].tblCtlData, newVal.dynamicTbl[0].tblCtlData, OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
                    break;
                }
            }

            //save a new one
            if (OMCI_IGMP_MAX_TBL_NUM == idx)
            {
                for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
                {
                    if (CA_OMCI_FALSE == multiOperProf->attr.dynamicTbl[idx].valid)
                    {
                        OMCIMNGDBG("save a new dynamic row");
                        multiOperProf->attr.dynamicTbl[idx].valid = CA_OMCI_TRUE;
                        OMCI_MEMCPY(multiOperProf->attr.dynamicTbl[idx].tblCtlData, newVal.dynamicTbl[0].tblCtlData, OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
                        break;
                    }
                }
            }
        }
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, mask))
    {
        staAclTbl = (ca_omci_acl_tbl_info_t *)(&(newVal.staticTbl[0].tblCtlData));
        tblCtl    = ca_omci_ntohs(staAclTbl->tblCtl);
        staAclCtl = (ca_omci_acl_ctl_t *)(&tblCtl);
        if (MULTICAST_CFG_TBL_CTL_DEL_ALL == staAclCtl->setCtl)
        {
            OMCIMNGDBG("Remove all static row");
            OMCI_MEMSET(multiOperProf->attr.staticTbl, 0, sizeof(ca_omci_acl_tbl_t) * OMCI_IGMP_MAX_TBL_NUM);
        }
        else if (MULTICAST_CFG_TBL_CTL_DEL == staAclCtl->setCtl)
        {
            for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
            {
                oriStaAclTbl = (ca_omci_acl_tbl_info_t *)(&(multiOperProf->attr.staticTbl[idx].tblCtlData));
                oriStaTblCtl = ca_omci_ntohs(oriStaAclTbl->tblCtl);
                oriStaAclCtl = (ca_omci_acl_ctl_t *)(&oriStaTblCtl);

                //if this rowkey configure before, get it and del it
                if ((CA_OMCI_TRUE == multiOperProf->attr.staticTbl[idx].valid)
                 && (oriStaAclCtl->rowKey == staAclCtl->rowKey))
                {
                    OMCIMNGDBG("Remove static row key[%d]", oriStaAclCtl->rowKey);
                    OMCI_MEMSET(&multiOperProf->attr.staticTbl[idx], 0, sizeof(ca_omci_acl_tbl_t));
                    break;
                }
            }
        }
        else
        {
            //update the table row
            for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
            {
                oriStaAclTbl = (ca_omci_acl_tbl_info_t *)(&(multiOperProf->attr.staticTbl[idx].tblCtlData));
                oriStaTblCtl = ca_omci_ntohs(oriStaAclTbl->tblCtl);
                oriStaAclCtl = (ca_omci_acl_ctl_t *)(&oriStaTblCtl);

                //if this rowkey configure before, get it and update it
                if ((CA_OMCI_TRUE == multiOperProf->attr.staticTbl[idx].valid)
                 && (oriStaAclCtl->rowKey == staAclCtl->rowKey))
                {
                    OMCIMNGDBG("Update static row key[%d]", oriStaAclCtl->rowKey);
                    OMCI_MEMCPY(multiOperProf->attr.staticTbl[idx].tblCtlData, newVal.staticTbl[0].tblCtlData, OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
                    break;
                }
            }

            //save a new one
            if (OMCI_IGMP_MAX_TBL_NUM == idx)
            {
                for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
                {
                    if (CA_OMCI_FALSE == multiOperProf->attr.staticTbl[idx].valid)
                    {
                        OMCIMNGDBG("save a new static row");
                        multiOperProf->attr.staticTbl[idx].valid = CA_OMCI_TRUE;
                        OMCI_MEMCPY(multiOperProf->attr.staticTbl[idx].tblCtlData, newVal.staticTbl[0].tblCtlData, OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
                        break;
                    }
                }
            }
        }
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_multicast_oper_prof_t *multiOperProf = (ca_omci_multicast_oper_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MULTICAST_OPER_PROF_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(multiOperProf->attr.igmpVer, 1);
    OMCI_GET_ATTR_UINT8(multiOperProf->attr.igmpFunc, 2);
    OMCI_GET_ATTR_UINT8(multiOperProf->attr.immediLeave, 3);
    OMCI_GET_ATTR_UINT16(multiOperProf->attr.usTci, 4);
    OMCI_GET_ATTR_UINT8(multiOperProf->attr.usTagCtl, 5);
    OMCI_GET_ATTR_UINT32(multiOperProf->attr.usRate, 6);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.dynamicTbl[0]) - sizeof(multiOperProf->attr.dynamicTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiOperProf->dynamicTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.staticTbl[0]) - sizeof(multiOperProf->attr.staticTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.staticTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiOperProf->staticTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.lostTbl[0]) - sizeof(multiOperProf->attr.lostTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_IGMP_LOST_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.lostTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiOperProf->lostTbl, tblSeq);
    }
    OMCI_GET_ATTR_UINT8(multiOperProf->attr.robust, 10);
    OMCI_GET_ATTR_UINT32(multiOperProf->attr.queryIpAddr, 11);
    OMCI_GET_ATTR_UINT32(multiOperProf->attr.queryItv, 12);
    OMCI_GET_ATTR_UINT32(multiOperProf->attr.queryMaxRspT, 13);
    OMCI_GET_ATTR_UINT32(multiOperProf->attr.lastMbQueryItv, 14);
    OMCI_GET_ATTR_UINT8(multiOperProf->attr.unauthJoinReqBehave, 15);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR16, inMask))
    {
        OMCI_GET_TBL_UINT8(multiOperProf->attr.dsTci.ctl);
        OMCI_GET_TBL_UINT16(multiOperProf->attr.dsTci.tci);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR16, mask);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_multicast_oper_prof_t *multiOperProf = (ca_omci_multicast_oper_prof_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.dynamicTbl[tblIdx]) - sizeof(multiOperProf->attr.dynamicTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiOperProf->dynamicTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[tblIdx].valid)
            {
                ca_omci_uint8_t tblCtlData[OMCI_MULTICAST_OPER_PROF_TBLCTLDATA] = {0};

                OMCI_MEMCPY(tblCtlData, multiOperProf->attr.dynamicTbl[tblIdx].tblCtlData, OMCI_MULTICAST_OPER_PROF_TBLCTLDATA);
                tblCtlData[0] = tblCtlData[0] & 0x3f;//don't response set ctrl
                OMCI_MEMCPY((tblAttr + offset), tblCtlData, sizeof(tblCtlData));
                offset += sizeof(tblCtlData);
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
        ca_omci_next_success(&multiOperProf->dynamicTbl);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.staticTbl[tblIdx]) - sizeof(multiOperProf->attr.staticTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiOperProf->staticTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.staticTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_IGMP_MAX_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.staticTbl[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &multiOperProf->attr.staticTbl[tblIdx].tblCtlData, sizeof(multiOperProf->attr.staticTbl[tblIdx].tblCtlData));
                offset += sizeof(multiOperProf->attr.staticTbl[tblIdx].tblCtlData);
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
        ca_omci_next_success(&multiOperProf->staticTbl);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR9, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiOperProf->attr.lostTbl[tblIdx]) - sizeof(multiOperProf->attr.lostTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiOperProf->lostTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_IGMP_LOST_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.lostTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_IGMP_LOST_TBL_NUM; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiOperProf->attr.lostTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiOperProf->attr.lostTbl[tblIdx].vlanId);
                OMCI_CP_TBL_UINT32(multiOperProf->attr.lostTbl[tblIdx].srcIpAddr);
                OMCI_CP_TBL_UINT32(multiOperProf->attr.lostTbl[tblIdx].dstMultiIp);
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
        ca_omci_next_success(&multiOperProf->lostTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multicast_oper_prof_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_multicast_oper_prof_t *multiOperProf = (ca_omci_multicast_oper_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(multiOperProf);
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.igmpVer, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.igmpFunc, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.immediLeave, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, multiOperProf->attr.usTci, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.usTagCtl, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, multiOperProf->attr.usRate, 6);
    for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
    {
        if (CA_OMCI_TRUE == multiOperProf->attr.dynamicTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(multiOperProf->attr.dynamicTbl[idx].tblCtlData);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
        }
    }
    for (idx = 0; idx < OMCI_IGMP_MAX_TBL_NUM; idx++)
    {
        if (CA_OMCI_TRUE == multiOperProf->attr.staticTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(multiOperProf->attr.staticTbl[idx].tblCtlData);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR8, mask);
        }
    }
    for (idx = 0; idx < OMCI_IGMP_LOST_TBL_NUM; idx++)
    {
        if (CA_OMCI_TRUE == multiOperProf->attr.lostTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiOperProf->attr.lostTbl[idx].vlanId);
            OMCI_UPLOAD_TBL_UINT32(multiOperProf->attr.lostTbl[idx].srcIpAddr);
            OMCI_UPLOAD_TBL_UINT32(multiOperProf->attr.lostTbl[idx].dstMultiIp);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        }
    }
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.robust, 10);
    OMCI_UPLOAD_ATTR_SIZE(32, multiOperProf->attr.queryIpAddr, 11);
    OMCI_UPLOAD_ATTR_SIZE(32, multiOperProf->attr.queryItv, 12);
    OMCI_UPLOAD_ATTR_SIZE(32, multiOperProf->attr.queryMaxRspT, 13);
    OMCI_UPLOAD_ATTR_SIZE(32, multiOperProf->attr.lastMbQueryItv, 14);
    OMCI_UPLOAD_ATTR_SIZE(8, multiOperProf->attr.unauthJoinReqBehave, 15);
    OMCI_UPLOAD_TBL_UINT8(multiOperProf->attr.dsTci.ctl);
    OMCI_UPLOAD_TBL_UINT16(multiOperProf->attr.dsTci.tci);
    OMCI_MASK_ATTR_SET(OMCI_ME_ATTR16, mask);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_multicast_oper_prof_init(void)
{
    sg_omci_me_multicast_oper_prof_handlers.uploadProc = ca_omci_me_multicast_oper_prof_upload;

    sg_omci_me_multicast_oper_prof_handlers.createProc  = ca_omci_me_multicast_oper_prof_create;
    sg_omci_me_multicast_oper_prof_handlers.deleteProc  = ca_omci_me_multicast_oper_prof_delete;
    sg_omci_me_multicast_oper_prof_handlers.setProc     = ca_omci_me_multicast_oper_prof_set;
    sg_omci_me_multicast_oper_prof_handlers.getProc     = ca_omci_me_multicast_oper_prof_get;
    sg_omci_me_multicast_oper_prof_handlers.getNextProc = ca_omci_me_multicast_oper_prof_get_next;
}

