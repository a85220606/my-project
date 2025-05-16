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
#include "omci_me_multi_sub_cfg_info.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_multi_sub_cfg_info_handlers = {0};


ca_omci_me_attr_t g_omci_me_multi_sub_cfg_info_attr[OMCI_ME_ATTR7] =
{
    {OMCI_ME_ATTR1, "me_type"},
    {OMCI_ME_ATTR2, "multi_oper_prof_point"},
    {OMCI_ME_ATTR3, "max_simu_group"},
    {OMCI_ME_ATTR4, "max_multi_bw"},
    {OMCI_ME_ATTR5, "bw_enforce"},
    {OMCI_ME_ATTR6, "multi_srv_pkg_tbl"},
    {OMCI_ME_ATTR7, "allow_prev_group_tbl"},
};


ca_omci_uint32_t ca_omci_me_multi_sub_cfg_info_attr_len_get(void)
{
    ca_omci_multi_sub_cfg_info_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.meType);
    attrLen += sizeof(me->attr.multiOperProfPoint);
    attrLen += sizeof(me->attr.maxSimuGroup);
    attrLen += sizeof(me->attr.maxMultiBw);
    attrLen += sizeof(me->attr.beEnforce);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_multi_sub_cfg_info_new(void)
{
    ca_omci_multi_sub_cfg_info_t *me = (ca_omci_multi_sub_cfg_info_t *)omci_malloc(sizeof(ca_omci_multi_sub_cfg_info_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID, &sg_omci_me_multi_sub_cfg_info_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(multi_sub_cfg_info);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_multi_sub_cfg_info_t *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(multiSubCfgInfo->attr.meType, "meType", 1);
    OMCI_SET_ATTR_UINT16(multiSubCfgInfo->attr.multiOperProfPoint, "multiOperProfPoint", 2);
    OMCI_SET_ATTR_UINT16(multiSubCfgInfo->attr.maxSimuGroup, "maxSimuGroup", 3);
    OMCI_SET_ATTR_UINT32(multiSubCfgInfo->attr.maxMultiBw, "maxMultiBw", 4);
    OMCI_SET_ATTR_UINT8(multiSubCfgInfo->attr.beEnforce, "beEnforce", 5);

    //when create, attribute multiSrvPkgTbl won't configure, so just find the link with attribute multiOperProfPoint
    if ((MULTI_SUB_CFG_INFO_ME_TYPE_MAC_PORT != multiSubCfgInfo->attr.meType)
     && (MULTI_SUB_CFG_INFO_ME_TYPE_8021P != multiSubCfgInfo->attr.meType))
    {
        OMCIMNGERR("Unsupport me type, not defined in G.988");
        result->result = OMCI_MSG_RST_NOT_SUPPORT;
        return CA_OMCI_E_PARAM;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_multi_sub_cfg_info_t      *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;
    ca_omci_multi_sub_cfg_info_attr_t  newVal;
    ca_omci_uint16_t                   offset        = 0;
    ca_omci_uint16_t                   mask          = 0;
    ca_omci_uint8_t                    idx           = 0;
    ca_omci_uint8_t                    tblUseFlag    = CA_OMCI_FALSE;
    ca_omci_mebase_t                  *multiOperProf = NULL;
    ca_omci_mebase_t                  *macPort       = NULL;
    ca_omci_mebase_t                  *ieeeDot1p     = NULL;
    ca_omci_srv_pkg_ctl_t             *srvPkgCtl     = NULL;
    ca_omci_srv_pkg_ctl_t             *oriSrvPkgCtl  = NULL;
    ca_omci_prev_group_ctl_t          *preCtl        = NULL;
    ca_omci_prev_group_ctl_t          *oriPreCtl     = NULL;
    ca_omci_allow_prev_group_info_t   *preTbl        = NULL;
    ca_omci_allow_prev_group_info_t   *oriPreTbl     = NULL;
    ca_omci_uint16_t                   tblCtl        = 0;
    ca_omci_uint16_t                   oriTblCtl     = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_multi_sub_cfg_info_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_SUB_CFG_INFO_SET_MASK_FULL);

    //check the multicast service package table first
    for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
        {
            tblUseFlag = CA_OMCI_TRUE;

            //remove old link here
            multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfgInfo->attr.multiSrvPkgTbl[idx].multiOperProfPoint);
            OMCI_RM_LINK_ME(multiSubCfgInfo, multiOperProf);
        }
    }

    //ignore attribute multiOperProfPoint when table used, so just remove old link when no table exist
    if (CA_OMCI_FALSE == tblUseFlag)
    {
        //remove old link here
        multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfgInfo->attr.multiOperProfPoint);
        OMCI_RM_LINK_ME(multiSubCfgInfo, multiOperProf);
    }

    //remove the me link self
    if (MULTI_SUB_CFG_INFO_ME_TYPE_MAC_PORT == multiSubCfgInfo->attr.meType)
    {
        macPort = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, multiSubCfgInfo->meBase.instanceId);
        OMCI_RM_LINK_ME(multiSubCfgInfo, macPort);
    }
    if (MULTI_SUB_CFG_INFO_ME_TYPE_8021P == multiSubCfgInfo->attr.meType)
    {
        ieeeDot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, multiSubCfgInfo->meBase.instanceId);
        OMCI_RM_LINK_ME(multiSubCfgInfo, ieeeDot1p);
    }

    //update all the attributes
    OMCI_UPDATE_ATTR_UINT(8, multiSubCfgInfo->attr.meType, "meType", 1);
    OMCI_UPDATE_ATTR_UINT(16, multiSubCfgInfo->attr.multiOperProfPoint, "multiOperProfPoint", 2);
    OMCI_UPDATE_ATTR_UINT(16, multiSubCfgInfo->attr.maxSimuGroup, "maxSimuGroup", 3);
    OMCI_UPDATE_ATTR_UINT(32, multiSubCfgInfo->attr.maxMultiBw, "maxMultiBw", 4);
    OMCI_UPDATE_ATTR_UINT(8, multiSubCfgInfo->attr.beEnforce, "beEnforce", 5);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        OMCI_SET_TBL_UINT16(newVal.multiSrvPkgTbl[0].tblCtl, "tblCtl");
        OMCI_SET_TBL_UINT16(newVal.multiSrvPkgTbl[0].vlanId, "vlanId");
        OMCI_SET_TBL_UINT16(newVal.multiSrvPkgTbl[0].maxSimuGroup, "maxSimuGroup");
        OMCI_SET_TBL_UINT32(newVal.multiSrvPkgTbl[0].maxMultiBw, "maxMultiBw");
        OMCI_SET_TBL_UINT16(newVal.multiSrvPkgTbl[0].multiOperProfPoint, "multiOperProfPoint");
        OMCI_SET_TBL_UINT8_T(newVal.multiSrvPkgTbl[0].rvc, "rvc", OMCI_MULTI_SUB_CFG_INFO_RVC);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);

        srvPkgCtl = (ca_omci_srv_pkg_ctl_t *)(&(newVal.multiSrvPkgTbl[0].tblCtl));
        if (MULTICAST_CFG_TBL_CTL_DEL_ALL == srvPkgCtl->option)
        {
            OMCIMNGDBG("Remove all multiSrv row ");
            OMCI_MEMSET(multiSubCfgInfo->attr.multiSrvPkgTbl, 0, sizeof(ca_omci_multi_srv_pkg_tbl_t) * OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL);
        }
        else if (MULTICAST_CFG_TBL_CTL_DEL == srvPkgCtl->option)
        {
            for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
            {
                oriSrvPkgCtl = (ca_omci_srv_pkg_ctl_t *)(&(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].tblCtl));

                //if this rowkey configure before, get it and del it
                if ((CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
                 && (oriSrvPkgCtl->rowKey == srvPkgCtl->rowKey))
                {
                    OMCIMNGDBG("Remove multiSrv row key[%d]", oriSrvPkgCtl->rowKey);
                    OMCI_MEMSET(&multiSubCfgInfo->attr.multiSrvPkgTbl[idx], 0, sizeof(ca_omci_multi_srv_pkg_tbl_t));
                    break;
                }
            }
        }
        else
        {
            //update the table row
            for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
            {
                oriSrvPkgCtl = (ca_omci_srv_pkg_ctl_t *)(&(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].tblCtl));

                //if this rowkey configure before, get it and update it
                if ((CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
                 && (oriSrvPkgCtl->rowKey == srvPkgCtl->rowKey))
                {
                    OMCIMNGDBG("Update multiSrv row key[%d]", oriSrvPkgCtl->rowKey);
                    multiSubCfgInfo->attr.multiSrvPkgTbl[idx].tblCtl             = newVal.multiSrvPkgTbl[0].tblCtl;
                    multiSubCfgInfo->attr.multiSrvPkgTbl[idx].vlanId             = newVal.multiSrvPkgTbl[0].vlanId;
                    multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxSimuGroup       = newVal.multiSrvPkgTbl[0].maxSimuGroup;
                    multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxMultiBw         = newVal.multiSrvPkgTbl[0].maxMultiBw;
                    multiSubCfgInfo->attr.multiSrvPkgTbl[idx].multiOperProfPoint = newVal.multiSrvPkgTbl[0].multiOperProfPoint;
                    OMCI_MEMCPY(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].rvc, newVal.multiSrvPkgTbl[0].rvc, OMCI_MULTI_SUB_CFG_INFO_RVC);
                    break;
                }
            }

            //save a new one
            if (OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL == idx)
            {
                for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
                {
                    if (CA_OMCI_FALSE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
                    {
                        OMCIMNGDBG("save a new multiSrv row");
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid              = CA_OMCI_TRUE;
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].tblCtl             = newVal.multiSrvPkgTbl[0].tblCtl;
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].vlanId             = newVal.multiSrvPkgTbl[0].vlanId;
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxSimuGroup       = newVal.multiSrvPkgTbl[0].maxSimuGroup;
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxMultiBw         = newVal.multiSrvPkgTbl[0].maxMultiBw;
                        multiSubCfgInfo->attr.multiSrvPkgTbl[idx].multiOperProfPoint = newVal.multiSrvPkgTbl[0].multiOperProfPoint;
                        OMCI_MEMCPY(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].rvc, newVal.multiSrvPkgTbl[0].rvc, OMCI_MULTI_SUB_CFG_INFO_RVC);
                        break;
                    }
                }
            }
        }
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        OMCI_SET_TBL_UINT8_T(newVal.allowPrevGroupTbl[0].tblData, "tblData", OMCI_MULTI_SUB_CFG_INFO_TBLDATA);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);

        preTbl = (ca_omci_allow_prev_group_info_t *)newVal.allowPrevGroupTbl[0].tblData;
        tblCtl = ca_omci_ntohs(preTbl->tblCtl);
        preCtl = (ca_omci_prev_group_ctl_t *)(&tblCtl);
        if (MULTICAST_CFG_TBL_CTL_DEL_ALL == preCtl->setCtl)
        {
            OMCIMNGDBG("Remove all preGroup row ");
            OMCI_MEMSET(multiSubCfgInfo->attr.allowPrevGroupTbl, 0, sizeof(ca_omci_allow_prev_group_tbl_t) * OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL);
        }
        else if (MULTICAST_CFG_TBL_CTL_DEL == preCtl->setCtl)
        {
            for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; idx++)
            {
                oriPreTbl = (ca_omci_allow_prev_group_info_t *)multiSubCfgInfo->attr.allowPrevGroupTbl[idx].tblData;
                oriTblCtl = ca_omci_ntohs(oriPreTbl->tblCtl);
                oriPreCtl = (ca_omci_prev_group_ctl_t *)(&oriTblCtl);
                if ((CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[idx].valid)
                 && (oriPreCtl->rowKey == preCtl->rowKey))
                {
                    OMCIMNGDBG("Remove prevGroup row key[%d]", oriPreCtl->rowKey);
                    OMCI_MEMSET(&multiSubCfgInfo->attr.allowPrevGroupTbl[idx], 0, sizeof(ca_omci_allow_prev_group_tbl_t));
                    //can't break here, there maybe two rows for one row key
                }
            }
        }
        else
        {
            //update the table row
            for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; idx++)
            {
                oriPreTbl = (ca_omci_allow_prev_group_info_t *)multiSubCfgInfo->attr.allowPrevGroupTbl[idx].tblData;
                oriTblCtl = ca_omci_ntohs(oriPreTbl->tblCtl);
                oriPreCtl = (ca_omci_prev_group_ctl_t *)(&oriTblCtl);

                //if this rowkey configure before, get it and update it
                if ((CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[idx].valid)
                 && (oriPreCtl->rowKey == preCtl->rowKey)
                 && (oriPreCtl->rowPart == preCtl->rowPart))
                {
                    OMCIMNGDBG("Update prevTbl row key[%d]", oriPreCtl->rowKey);
                    OMCI_MEMCPY(multiSubCfgInfo->attr.allowPrevGroupTbl[idx].tblData,
                                newVal.allowPrevGroupTbl[0].tblData,
                                OMCI_MULTI_SUB_CFG_INFO_TBLDATA);
                    break;
                }
            }

            //save a new one
            if (OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL == idx)
            {
                for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; idx++)
                {
                    if (CA_OMCI_FALSE == multiSubCfgInfo->attr.allowPrevGroupTbl[idx].valid)
                    {
                        OMCIMNGDBG("save a new prevTbl row");
                        multiSubCfgInfo->attr.allowPrevGroupTbl[idx].valid = CA_OMCI_TRUE;
                        OMCI_MEMCPY(multiSubCfgInfo->attr.allowPrevGroupTbl[idx].tblData,
                                    newVal.allowPrevGroupTbl[0].tblData,
                                    OMCI_MULTI_SUB_CFG_INFO_TBLDATA);
                        break;
                    }
                }
            }
        }
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_multi_sub_cfg_info_t *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_MULTI_SUB_CFG_INFO_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT8(multiSubCfgInfo->attr.meType, 1);
    OMCI_GET_ATTR_UINT16(multiSubCfgInfo->attr.multiOperProfPoint, 2);
    OMCI_GET_ATTR_UINT16(multiSubCfgInfo->attr.maxSimuGroup, 3);
    OMCI_GET_ATTR_UINT32(multiSubCfgInfo->attr.maxMultiBw, 4);
    OMCI_GET_ATTR_UINT8(multiSubCfgInfo->attr.beEnforce, 5);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[0]) - sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiSubCfgInfo->multiSrvPkgTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[0]) - sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].valid)
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
        ca_omci_next_init(&multiSubCfgInfo->allowPrevGroupTbl, tblSeq);
    }

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_multi_sub_cfg_info_t *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx]) - sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiSubCfgInfo->multiSrvPkgTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].tblCtl);
                OMCI_CP_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].vlanId);
                OMCI_CP_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].maxSimuGroup);
                OMCI_CP_TBL_UINT32(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].maxMultiBw);
                OMCI_CP_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].multiOperProfPoint);
                OMCI_MEMCPY((tblAttr + offset), &multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].rvc, sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].rvc));
                offset += sizeof(multiSubCfgInfo->attr.multiSrvPkgTbl[tblIdx].rvc);
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
        ca_omci_next_success(&multiSubCfgInfo->multiSrvPkgTbl);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx]) - sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&multiSubCfgInfo->allowPrevGroupTbl, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].valid)
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
        for (tblIdx = 0; tblIdx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].tblData, sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].tblData));
                offset += sizeof(multiSubCfgInfo->attr.allowPrevGroupTbl[tblIdx].tblData);
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
        ca_omci_next_success(&multiSubCfgInfo->allowPrevGroupTbl);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_multi_sub_cfg_info_t *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;
    ca_omci_uint8_t               idx             = 0;
    ca_omci_uint8_t               tblUseFlag      = CA_OMCI_FALSE;
    ca_omci_mebase_t             *multiOperProf   = NULL;
    ca_omci_mebase_t             *macPort         = NULL;
    ca_omci_mebase_t             *ieeeDot1p       = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    //check the multicast service package table first
    for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
        {
            tblUseFlag = CA_OMCI_TRUE;

            //remove old link here
            multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfgInfo->attr.multiSrvPkgTbl[idx].multiOperProfPoint);
            OMCI_RM_LINK_ME(multiSubCfgInfo, multiOperProf);
        }
    }

    //ignore attribute multiOperProfPoint when table used, so just remove old link when no table exist
    if (CA_OMCI_FALSE == tblUseFlag)
    {
        //remove old link here
        multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfgInfo->attr.multiOperProfPoint);
        OMCI_RM_LINK_ME(multiSubCfgInfo, multiOperProf);
    }

    //remove the me link self
    if (MULTI_SUB_CFG_INFO_ME_TYPE_MAC_PORT == multiSubCfgInfo->attr.meType)
    {
        macPort = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, multiSubCfgInfo->meBase.instanceId);
        OMCI_RM_LINK_ME(multiSubCfgInfo, macPort);
    }
    if (MULTI_SUB_CFG_INFO_ME_TYPE_8021P == multiSubCfgInfo->attr.meType)
    {
        ieeeDot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, multiSubCfgInfo->meBase.instanceId);
        OMCI_RM_LINK_ME(multiSubCfgInfo, ieeeDot1p);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_multi_sub_cfg_info_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_multi_sub_cfg_info_t *multiSubCfgInfo = (ca_omci_multi_sub_cfg_info_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(multiSubCfgInfo);
    OMCI_UPLOAD_ATTR_SIZE(8, multiSubCfgInfo->attr.meType, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, multiSubCfgInfo->attr.multiOperProfPoint, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, multiSubCfgInfo->attr.maxSimuGroup, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, multiSubCfgInfo->attr.maxMultiBw, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, multiSubCfgInfo->attr.beEnforce, 5);
    for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubCfgInfo->attr.multiSrvPkgTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].tblCtl);
            OMCI_UPLOAD_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].vlanId);
            OMCI_UPLOAD_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxSimuGroup);
            OMCI_UPLOAD_TBL_UINT32(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].maxMultiBw);
            OMCI_UPLOAD_TBL_UINT16(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].multiOperProfPoint);
            OMCI_UPLOAD_TBL_UINT8(multiSubCfgInfo->attr.multiSrvPkgTbl[idx].rvc);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        }
    }
    for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_ALLOW_PREV_GROUP_TBL; idx++)
    {
        if (CA_OMCI_TRUE == multiSubCfgInfo->attr.allowPrevGroupTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(multiSubCfgInfo->attr.allowPrevGroupTbl[idx].tblData);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
        }
    }
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_multi_sub_cfg_info_init(void)
{
    sg_omci_me_multi_sub_cfg_info_handlers.uploadProc = ca_omci_me_multi_sub_cfg_info_upload;

    sg_omci_me_multi_sub_cfg_info_handlers.createProc  = ca_omci_me_multi_sub_cfg_info_create;
    sg_omci_me_multi_sub_cfg_info_handlers.deleteProc  = ca_omci_me_multi_sub_cfg_info_delete;
    sg_omci_me_multi_sub_cfg_info_handlers.setProc     = ca_omci_me_multi_sub_cfg_info_set;
    sg_omci_me_multi_sub_cfg_info_handlers.getProc     = ca_omci_me_multi_sub_cfg_info_get;
    sg_omci_me_multi_sub_cfg_info_handlers.getNextProc = ca_omci_me_multi_sub_cfg_info_get_next;
}


void ca_omci_me_multi_sub_cfg_info_link(void)
{
    ca_omci_meclass_t            *meClass       = NULL;
    ca_omci_mebase_t             *meInst        = NULL;
    ca_omci_multi_sub_cfg_info_t *multiSubCfg   = NULL;
    ca_omci_mebase_t             *macPort       = NULL;
    ca_omci_mebase_t             *ieeeDot1p     = NULL;
    ca_omci_uint8_t               idx           = 0;
    ca_omci_uint8_t               tblUseFlag    = CA_OMCI_FALSE;
    ca_omci_mebase_t             *multiOperProf = NULL;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIERR("no multi sub cfg is created now");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        multiSubCfg = (ca_omci_multi_sub_cfg_info_t *)meInst;

        if (MULTI_SUB_CFG_INFO_ME_TYPE_MAC_PORT == multiSubCfg->attr.meType)
        {
            macPort = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, multiSubCfg->meBase.instanceId);
            OMCI_ME_LINK(macPort, (ca_omci_mebase_t *)multiSubCfg);
        }
        if (MULTI_SUB_CFG_INFO_ME_TYPE_8021P == multiSubCfg->attr.meType)
        {
            ieeeDot1p = ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, multiSubCfg->meBase.instanceId);
            OMCI_ME_LINK(ieeeDot1p, (ca_omci_mebase_t *)multiSubCfg);
        }

        tblUseFlag =  CA_OMCI_FALSE;
        for (idx = 0; idx < OMCI_MULTI_SUB_CFG_INFO_MULTI_SRV_PKG_TBL; idx++)
        {
            if (CA_OMCI_TRUE == multiSubCfg->attr.multiSrvPkgTbl[idx].valid)
            {
                tblUseFlag = CA_OMCI_TRUE;

                //add link here
                multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfg->attr.multiSrvPkgTbl[idx].multiOperProfPoint);
                OMCI_ME_LINK((ca_omci_mebase_t *)multiSubCfg, multiOperProf);
            }
        }

        //ignore attribute multiOperProfPoint when table used, so just add link when no table exist
        if (CA_OMCI_FALSE == tblUseFlag)
        {
            //remove old link here
            multiOperProf = ca_omci_me_entry_lookup(OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, multiSubCfg->attr.multiOperProfPoint);
            OMCI_ME_LINK((ca_omci_mebase_t *)multiSubCfg, multiOperProf);
        }
    }
}

