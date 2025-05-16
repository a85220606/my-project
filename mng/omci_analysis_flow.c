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
#include "omci_avl.h"
#include "omci_analysis.h"
#include "omci_analysis_flow.h"
#include "omci_timer.h"
#include "omci_mq.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_codec_pkt.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_gal_eth_prof.h"
#include "omci_me_tcont.h"
#include "omci_me_ext_vlan_tag_op.h"
#include "omci_me_mac_port_cfg.h"
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_vlan_tag_filter.h"
#include "omci_me_traffic_descript.h"
#include "omci_me_pri_queue.h"
#include "omci_me_traffic_scheg.h"
#include "omci_me_ip_host.h"
#include "omci_me_veip.h"
#include "omci_me_net_addr.h"
#include "omci_me_auth_scy.h"
#include "omci_me_large_string.h"
#include "omci_me_tcp_udp_cfg.h"
#include "omci_me_bbrtr69_mng_srv.h"
#include "omci_me_voip_cfg.h"
#include "omci_me_mgc_cfg_data.h"
#include "omci_me_voip_ctp.h"
#include "omci_me_pptp_pots.h"
#include "omci_me_multicast_oper_prof.h"
#include "omci_me.h"
#include "omci_hal.h"
#include "omci_mng_onuinfo.h"
#include "omci_me_alcl_ds_tag_oper1.h"
#include "osal_sem.h"


extern ca_uint g_omciAnalysisSem;
extern ca_uint g_omciCfgSem;


static ca_omci_analyse_flow_t sg_omciCfgFlowTbl;
static ca_omci_analyse_flow_t sg_omciAnalyseFlowTbl;
static ca_omci_uint32_t       sg_omciUpdateFlowFlag = 0;


ca_omci_int8_t *ca_omci_tpid_filter_str[] =
{
    [OMCI_FILTER_TPID_DEI_NONE]   = "None",
    [1]                           = "Not Support",
    [2]                           = "Not Support",
    [3]                           = "Not Support",
    [OMCI_FILTER_TPID_DEI_8100]   = "0x8100",
    [OMCI_FILTER_TPID_NOCARE_DEI] = "= input TPID",
    [OMCI_FILTER_TPID_DEI_0]      = "= input TPID, DEI = 0",
    [OMCI_FILTER_TPID_DEI_1]      = "= input TPID, DEI = 1",
};


ca_omci_int8_t *ca_omci_tpid_treat_str[] =
{
    [OMCI_TREATMENT_TPID_DEI_COPY_INNER]     = "CP inner Tag TPID",
    [OMCI_TREATMENT_TPID_DEI_COPY_OUTER]     = "CP outer Tag TPID",
    [OMCI_TREATMENT_TPID_SET_DEI_COPY_INNER] = "= output TPID, copy inner DEI",
    [OMCI_TREATMENT_TPID_SET_DEI_COPY_OUTER] = "= output TPID, copy outer DEI",
    [OMCI_TREATMENT_TPID_SET_8100]           = "0x8100",
    [5]                                      = "Reserved",
    [OMCI_TREATMENT_TPID_SET_88A8_DEI_0]     = "= output TPID, DEI = 0",
    [OMCI_TREATMENT_TPID_SET_88A8_DEI_1]     = "= output TPID, DEI = 1",
};


ca_omci_int8_t *ca_omci_untag_action_str[] =
{
    [CA_OMCI_UNTAG_FWD_DROP]                      = "drop",
    [CA_OMCI_UNTAG_FWD_TRANSPARENT]               = "transparent",
    [CA_OMCI_UNTAG_FWD_ADD_VID_PRI]               = "add vlan",
    [CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI]          = "add vid, pbit map dscp",
    [CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_PRI]          = "add double tag",
    [CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_INN_DSCP_PRI] = "add outer vlan, add inner vid inner pbit map dscp",
    [CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_OUT_DSCP_PRI] = "add outer vid outer pbit map dscp, add inner vlan",
    [CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_DSCP_PRI]     = "add both vid pbit both map dscp",
};


ca_omci_int8_t *ca_omci_sgl_tag_action_str[] =
{
    [CA_OMCI_SGL_TAG_FWD_DROP]                     = "drop",
    [CA_OMCI_SGL_TAG_FWD_TRANSPARENT]              = "transparent",
    [CA_OMCI_SGL_TAG_FWD_MD_TAG]                   = "modify vlan",
    [CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP]              = "modify vid, pbit map dscp",
    [CA_OMCI_SGL_TAG_FWD_ADD_TAG]                  = "add vlan",
    [CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP]             = "add vid, pbit map dscp",
    [CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_TAG]           = "modify vlan, add vlan",
    [CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_VID_DSCP]      = "modify vlan, add vid pbit map dscp ",
    [CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_TAG]      = "modify vid, pbit map dscp, add vlan",
    [CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_VID_DSCP] = "modify vid, add vlan, both pbit map dscp",
    [CA_OMCI_SGL_TAG_FWD_REMOVE_TAG]               = "remove tag",
};


ca_omci_int8_t *ca_omci_dbl_tag_action_str[] =
{
    [CA_OMCI_DBL_TAG_FWD_DROP]               = "drop",
    [CA_OMCI_DBL_TAG_FWD_TRANSPARENT]        = "transparent",
    [CA_OMCI_DBL_TAG_FWD_MD_OTAG]            = "modify outer vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID]            = "modify outer vid",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP]      = "modify outer vid pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OPRI]            = "modify outer pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_ODSCP]           = "modify outer pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_TAGS]            = "modify both tags",
    [CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID]       = "modify outer vlan, inner vid",
    [CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID_IDSCP] = "modify outer vlan, inner vid pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG]      = "remove outer tag, modify vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_OTAG_IPRI]       = "modify outer vlan, inner pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_OTAG_IDSCP]      = "modify outer vlan, inner pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_ITAG]       = "modify outer vid, inner vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_VIDS]            = "modify both vids",
    [CA_OMCI_DBL_TAG_FWD_MD_VIDS_IDSCP]      = "modify both vids, inner pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_IPRI]       = "modify outer vid, inner pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_IDSCP]      = "modify outer vid, inner pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_ITAG] = "modify outer vid pbit map dscp, inner vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_VIDS_ODSCP]      = "modify both vids, outer pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_VIDS_DSCP]       = "modify both vids, both pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_IPRI] = "modify outer vid, outer pbit map dscp, modify inner pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_OVID_DSCP]       = "modify outer vid, both pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID]      = "remove vlan, modify vlan",
    [CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP] = "remove vlan, modify vid pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_RM_TAGS]            = "remove both vlans",
    [CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI]      = "remove vlan, modify pbit",
    [CA_OMCI_DBL_TAG_FWD_RM_ITAG]            = "remove inner vlan",
    [CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP]        = "remove vlan, both pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG]       = "modify outer pbit, inner vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID]       = "modify outer pbit, inner vid",
    [CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID_IDSCP] = "modify outer pbit, inner vid pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_PRIS]            = "modify both pbits",
    [CA_OMCI_DBL_TAG_FWD_MD_OPRI_IDSCP]      = "modify outer pbit, inner pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_ITAG]            = "modify inner vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_IVID]            = "modify inner vid",
    [CA_OMCI_DBL_TAG_FWD_MD_IVID_IDSCP]      = "modify inner vid pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_IPRI]            = "modify inner pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_IDSCP]           = "modify inner pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_MD_ODSCP_ITAG]      = "modify outer pbit map dscp, inner vlan",
    [CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IVID]      = "modify outer pbit map dscp, inner vid",
    [CA_OMCI_DBL_TAG_FWD_MD_DSCP_IVID]       = "modify both pbit map dscp, inner vid",
    [CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IPRI]      = "modify outer pbit map dscp, inner pbit",
    [CA_OMCI_DBL_TAG_FWD_MD_DSCP]            = "modify both pbit map dscp",
    [CA_OMCI_DBL_TAG_FWD_RM_OTAG]            = "remove outer vlan",
    [CA_OMCI_DBL_TAG_ADD_TAG]                = "add vlan",
};


ca_omci_int8_t *ca_omci_ethertype_str[] =
{
    [CA_OMCI_FILTER_NONE]           = "Don't care",
    [CA_OMCI_FILTER_IPOE_FRAMES]    = "IPOE",
    [CA_OMCI_FILTER_PPPOE_FRAMES]   = "PPPOE",
    [CA_OMCI_FILTER_ARP_FRAMES]     = "ARP",
    [CA_OMCI_FILTER_IPOE_V6_FRAMES] = "IPOEV6",
};


extern void ca_omci_me_gem_net_ctp_link(void);
extern void ca_omci_me_gem_iwtp_link(void);
extern void ca_omci_me_ieee_dot1p_srv_link(void);
extern void ca_omci_me_multi_gem_iwtp_link(void);
extern void ca_omci_me_mac_port_cfg_link(void);
extern void ca_omci_me_traffic_scheg_link(void);
extern void ca_omci_me_ext_vlan_tag_op_link(void);
extern void ca_omci_me_veip_link(void);
extern void ca_omci_me_bbrtr69_mng_srv_link(void);
extern void ca_omci_me_tcp_udp_cfg_link(void);
extern void ca_omci_me_net_addr_link(void);
extern void ca_omci_me_voip_cfg_link(void);
extern void ca_omci_me_mgc_cfg_data_link(void);
extern void ca_omci_me_voip_ctp_link(void);
extern void ca_omci_me_voip_media_prof_link(void);
extern void ca_omci_me_voice_srv_prof_link(void);
extern void ca_omci_me_rtp_prof_data_link(void);
extern void ca_omci_me_sip_usr_link(void);
extern void ca_omci_me_sip_agt_cfg_link(void);
extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


#if DESCRIPTION("Tool functions")
ca_omci_uint8_t ca_omci_gemflow_pri_get(ca_omci_uint16_t instanceId, ca_omci_ieee_dot1p_srv_t *pMe)
{
    ca_omci_uint8_t pri = 0 ;

    if (instanceId == pMe->attr.iwtpPri0)
    {
        pri = pri | (1 << 0);
    }
    if (instanceId == pMe->attr.iwtpPri1)
    {
        pri = pri | (1 << 1);
    }
    if (instanceId == pMe->attr.iwtpPri2)
    {
        pri = pri | (1 << 2);
    }
    if (instanceId == pMe->attr.iwtpPri3)
    {
        pri = pri | (1 << 3);
    }
    if (instanceId == pMe->attr.iwtpPri4)
    {
        pri = pri | (1 << 4);
    }
    if (instanceId == pMe->attr.iwtpPri5)
    {
        pri = pri | (1 << 5);
    }
    if (instanceId == pMe->attr.iwtpPri6)
    {
        pri = pri | (1 << 6);
    }
    if (instanceId == pMe->attr.iwtpPri7)
    {
        pri = pri | (1 << 7);
    }

    return pri;
}


ca_omci_bool_t ca_omci_gemflow_change(ca_omci_gem_flow_t *pOri, ca_omci_gem_flow_t*pNew)
{
    ca_omci_uint8_t idx = 0;

    if (NULL == pOri || NULL == pNew)
    {
        return CA_OMCI_FALSE;
    }

    CA_OMCI_COMPARE_CFG_ITEM(gemId);
    CA_OMCI_COMPARE_CFG_ITEM(tcontId);
    CA_OMCI_COMPARE_CFG_ITEM(allocId);
    CA_OMCI_COMPARE_CFG_ITEM(direction);
    CA_OMCI_COMPARE_CFG_ITEM(usQueue);
    CA_OMCI_COMPARE_CFG_ITEM(usQueueDpcm);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.mode);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.pps);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.cir);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.cbs);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.pir);
    CA_OMCI_COMPARE_CFG_ITEM(usTrafficDes.pbs);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueue);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueueDpcm);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.mode);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.pps);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.cir);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.cbs);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.pir);
    CA_OMCI_COMPARE_CFG_ITEM(dsTrafficDes.pbs);
    CA_OMCI_COMPARE_CFG_ITEM(aesState);
    CA_OMCI_COMPARE_CFG_ITEM(aesKeyRing);
    CA_OMCI_COMPARE_CFG_ITEM(gemPri);
    CA_OMCI_COMPARE_CFG_ITEM(transUntag);

    for (idx = 0; idx < OMCI_VLAN_LIST_NUM; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(vidList[idx]);
    }
    return CA_OMCI_FALSE;
}


ca_omci_bool_t ca_omci_tcont_change(ca_omci_tcont_cfg_t *pOri, ca_omci_tcont_cfg_t*pNew)
{
    if (NULL == pOri || NULL == pNew)
    {
        return CA_OMCI_FALSE;
    }

    CA_OMCI_COMPARE_CFG_ITEM(tcontId);
    CA_OMCI_COMPARE_CFG_ITEM(allocId);
    return CA_OMCI_FALSE;
}


ca_omci_bool_t ca_omci_stream_change(ca_omci_stream_cfg_t *pOri, ca_omci_stream_cfg_t*pNew)
{
    ca_omci_uint8_t idx = 0;

    if (NULL == pOri || NULL == pNew)
    {
        return CA_OMCI_FALSE;
    }

    CA_OMCI_COMPARE_CFG_ITEM(gemId);
    CA_OMCI_COMPARE_CFG_ITEM(uniPortId);
    CA_OMCI_COMPARE_CFG_ITEM(portType);
    CA_OMCI_COMPARE_CFG_ITEM(filterFlag);
    CA_OMCI_COMPARE_CFG_ITEM(filterPri);
    CA_OMCI_COMPARE_CFG_ITEM(dsMode);
    CA_OMCI_COMPARE_CFG_ITEM(inputTpid);
    CA_OMCI_COMPARE_CFG_ITEM(outputTpid);
    CA_OMCI_COMPARE_CFG_ITEM(innerTpid.filter);
    CA_OMCI_COMPARE_CFG_ITEM(innerTpid.treat);
    CA_OMCI_COMPARE_CFG_ITEM(outerTpid.filter);
    CA_OMCI_COMPARE_CFG_ITEM(outerTpid.treat);
    CA_OMCI_COMPARE_CFG_ITEM(tcontId);
    CA_OMCI_COMPARE_CFG_ITEM(usQueue);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueue);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueueWeight);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoe.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterIpoev6.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterPppoe.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterArp.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(untagFilterDef.dsTagAction);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.vlanFilter.vid);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.vlanFilter.pri);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(sglTagFilterDef.outerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.isValid);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.fwdType);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.etherTypeFilter);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.innerVlanFilter.vid);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.innerVlanFilter.pri);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.outerVlanFilter.vid);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.outerVlanFilter.pri);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.innerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.innerVlanTreatment.pri);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.outerVlanTreatment.vid);
    CA_OMCI_COMPARE_CFG_ITEM(dblTagFilterDef.outerVlanTreatment.pri);

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].isValid);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].fwdType);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].etherTypeFilter);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].vlanFilter.vid);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].vlanFilter.pri);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].innerVlanTreatment.vid);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].innerVlanTreatment.pri);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].outerVlanTreatment.vid);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].outerVlanTreatment.pri);
        CA_OMCI_COMPARE_CFG_ITEM(sglTagFilter[idx].dsTagAction);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].isValid);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].fwdType);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].etherTypeFilter);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].innerVlanFilter.vid);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].innerVlanFilter.pri);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].outerVlanFilter.vid);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].outerVlanFilter.pri);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].innerVlanTreatment.vid);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].innerVlanTreatment.pri);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].outerVlanTreatment.vid);
        CA_OMCI_COMPARE_CFG_ITEM(dblTagFilter[idx].outerVlanTreatment.pri);
    }

    return CA_OMCI_FALSE;
}


ca_omci_bool_t ca_omci_h248_cfg_change(ca_omci_h248_voip_t *pOri, ca_omci_h248_voip_t*pNew)
{
    ca_omci_uint8_t idx = 0;

    if (NULL == pOri || NULL == pNew)
    {
        return CA_OMCI_FALSE;
    }

    CA_OMCI_COMPARE_CFG_ITEM(potsId);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.ipOpt);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.ipAddr);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.mask);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.gateway);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.priDns);
    CA_OMCI_COMPARE_CFG_ITEM(voipIphost.secDns);
    CA_OMCI_COMPARE_CFG_ITEM(version);
    CA_OMCI_COMPARE_CFG_ITEM(msgFormat);
    CA_OMCI_COMPARE_CFG_ITEM(maxRetryTime);
    CA_OMCI_COMPARE_CFG_ITEM(maxRetryAttem);
    CA_OMCI_COMPARE_CFG_ITEM(srvChangeDelay);
    CA_OMCI_COMPARE_CFG_ITEM(softswitch);

    for (idx = 0; idx < OMCI_H248_MGC_LEN; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(priMgc[idx]);
        CA_OMCI_COMPARE_CFG_ITEM(sndMgc[idx]);
    }

    for (idx = 0; idx < OMCI_H248_TERM_ID_BASE_LEN; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(termIdBase[idx]);
    }

    for (idx = 0; idx < OMCI_H248_MSG_ID_LEN; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(msgId[idx]);
    }

    return CA_OMCI_FALSE;
}


ca_omci_bool_t ca_omci_filter_stream_change(ca_omci_filter_stream_cfg_t *pOri, ca_omci_filter_stream_cfg_t*pNew)
{
    ca_omci_uint8_t idx = 0;

    if (NULL == pOri || NULL == pNew)
    {
        return CA_OMCI_FALSE;
    }

    CA_OMCI_COMPARE_CFG_ITEM(gemId);
    CA_OMCI_COMPARE_CFG_ITEM(uniPortId);
    CA_OMCI_COMPARE_CFG_ITEM(portType);
    CA_OMCI_COMPARE_CFG_ITEM(priBit);
    CA_OMCI_COMPARE_CFG_ITEM(externVlanFlag);
    CA_OMCI_COMPARE_CFG_ITEM(usQueueDpcm);
    CA_OMCI_COMPARE_CFG_ITEM(inputTpid);
    CA_OMCI_COMPARE_CFG_ITEM(outputTpid);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueue);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueueWeight);
    CA_OMCI_COMPARE_CFG_ITEM(dsQueueDpcm);
    for (idx = 0; idx < OMCI_VLAN_LIST_NUM; idx++)
    {
        CA_OMCI_COMPARE_CFG_ITEM(vidList[idx]);
    }

    return CA_OMCI_FALSE;
}
#endif


#if DESCRIPTION("Debug functions")
void ca_omci_gemflow_dbg_print(ca_omci_gem_flow_t *pGemFlow)
{
    ca_omci_uint8_t idx = 0;

    OMCI_PARAM_IS_NULL(pGemFlow);

    OMCIALDBG("gemId %d",             pGemFlow->gemId);
    OMCIALDBG("tcontId %d",           pGemFlow->tcontId);
    OMCIALDBG("allocId %d",           pGemFlow->allocId);
    OMCIALDBG("direction %d",         pGemFlow->direction);
    OMCIALDBG("usQueue %d",           pGemFlow->usQueue);
    OMCIALDBG("usQueueWeight %d",     pGemFlow->usQueueWeight);
    OMCIALDBG("usTrafficDes mode %d", pGemFlow->usTrafficDes.mode);
    OMCIALDBG("usTrafficDes pps %d",  pGemFlow->usTrafficDes.pps);
    OMCIALDBG("usTrafficDes cir %d",  pGemFlow->usTrafficDes.cir);
    OMCIALDBG("usTrafficDes cbs %d",  pGemFlow->usTrafficDes.cbs);
    OMCIALDBG("usTrafficDes pir %d",  pGemFlow->usTrafficDes.pir);
    OMCIALDBG("usTrafficDes pbs %d",  pGemFlow->usTrafficDes.pbs);
    OMCIALDBG("dsQueue %d",           pGemFlow->dsQueue);
    OMCIALDBG("dsQueueWeight %d",     pGemFlow->dsQueueWeight);
    OMCIALDBG("dsTrafficDes mode %d", pGemFlow->dsTrafficDes.mode);
    OMCIALDBG("dsTrafficDes pps %d",  pGemFlow->dsTrafficDes.pps);
    OMCIALDBG("dsTrafficDes cir %d",  pGemFlow->dsTrafficDes.cir);
    OMCIALDBG("dsTrafficDes cbs %d",  pGemFlow->dsTrafficDes.cbs);
    OMCIALDBG("dsTrafficDes pir %d",  pGemFlow->dsTrafficDes.pir);
    OMCIALDBG("dsTrafficDes pbs %d",  pGemFlow->dsTrafficDes.pbs);
    OMCIALDBG("aesState %d",          pGemFlow->aesState);
    OMCIALDBG("aesKeyRing %d",        pGemFlow->aesKeyRing);
    OMCIALDBG("gemPri 0x%x",          pGemFlow->gemPri);

    for (idx = 0; idx < OMCI_VLAN_LIST_NUM; idx++)
    {
        OMCIALDBG("vid%d 0x%x", idx, pGemFlow->vidList[idx]);
    }
}


void ca_omci_tcont_dbg_print(ca_omci_tcont_cfg_t *pTcCfg)
{
    ca_omci_uint8_t idx = 0;

    OMCI_PARAM_IS_NULL(pTcCfg);

    OMCIALDBG("tcontId %d", pTcCfg->tcontId);
    OMCIALDBG("allocId %d", pTcCfg->allocId);
    OMCIALDBG("policy %d",  pTcCfg->policy);
    for (idx = 0; idx < OMCI_PQ_NUM_PER_TCONT; idx++)
    {
        OMCIALDBG("wrrQue[%d] %d", idx, pTcCfg->wrrQue[idx]);
    }
    OMCIALDBG("schedId %d",     pTcCfg->schedId);
    OMCIALDBG("schedPoint %d",  pTcCfg->schedPoint);
    OMCIALDBG("schedPolicy %d", pTcCfg->schedPolicy);
    OMCIALDBG("schedWeight %d", pTcCfg->schedWeight);
}
#endif


#if DESCRIPTION("Data manage functions")
static ca_omci_int32_t ca_omci_compare_filter_stream_key(const void *itemP, const void *dataP, void *paramP)
{
    const ca_omci_filter_stream_cfg_t *entryItemP = (const ca_omci_filter_stream_cfg_t *)itemP;
    const ca_omci_filter_stream_cfg_t *entryDataP = (const ca_omci_filter_stream_cfg_t *)dataP;
    ca_omci_int32_t             ret        = 0;

    (void)paramP;
    if (NULL == itemP || NULL == dataP)
    {
        return -1;
    }

    ret = entryItemP->portType - entryDataP->portType;
    if (0 == ret)
    {
        ret = entryItemP->gemId - entryDataP->gemId;
        if (0 == ret)
        {
            ret = entryItemP->uniPortId - entryDataP->uniPortId;
        }
    }

    return ret;
}


static ca_omci_int32_t ca_omci_compare_stream_key(const void *itemP, const void *dataP, void *paramP)
{
    const ca_omci_stream_cfg_t *entryItemP = (const ca_omci_stream_cfg_t *)itemP;
    const ca_omci_stream_cfg_t *entryDataP = (const ca_omci_stream_cfg_t *)dataP;
    ca_omci_int32_t             ret        = 0;

    (void)paramP;
    if (NULL == itemP || NULL == dataP)
    {
        return -1;
    }

    ret = entryItemP->portType - entryDataP->portType;
    if (0 == ret)
    {
        ret = entryItemP->gemId - entryDataP->gemId;
        if (0 == ret)
        {
            ret = entryItemP->uniPortId - entryDataP->uniPortId;
        }
    }

    return ret;
}


static ca_omci_int32_t ca_omci_compare_gem_key(const void *itemP, const void *dataP, void *paramP)
{
    const ca_omci_gem_flow_t *entryItemP = (const ca_omci_gem_flow_t *)itemP;
    const ca_omci_gem_flow_t *entryDataP = (const ca_omci_gem_flow_t *)dataP;

    (void)paramP;
    if (NULL == itemP || NULL == dataP)
    {
        return -1;
    }

    return (entryItemP->gemId - entryDataP->gemId);
}


static ca_omci_int32_t ca_omci_compare_tcont_key(const void *itemP, const void *dataP, void *paramP)
{
    const ca_omci_tcont_cfg_t *entryItemP = (const ca_omci_tcont_cfg_t *)itemP;
    const ca_omci_tcont_cfg_t *entryDataP = (const ca_omci_tcont_cfg_t *)dataP;

    (void)paramP;
    if (NULL == itemP || NULL == dataP)
    {
        return -1;
    }

    return (entryItemP->tcontId - entryDataP->tcontId);
}


void ca_omci_analysis_flow_tbl_destory(ca_omci_flow_t *pData)
{
    OMCI_PARAM_IS_NULL(pData);

    if (NULL != pData->tcCfg)
    {
        omci_avl_destroy(pData->tcCfg, NULL);
        pData->tcCfg = NULL;
    }

    if (NULL != pData->gemFlow)
    {
        omci_avl_destroy(pData->gemFlow, NULL);
        pData->gemFlow = NULL;
    }

    if (NULL != pData->usFlow)
    {
        omci_avl_destroy(pData->usFlow, NULL);
        pData->usFlow = NULL;
    }

    if (NULL != pData->dsFlow)
    {
        omci_avl_destroy(pData->dsFlow, NULL);
        pData->dsFlow = NULL;
    }

    if (NULL != pData->filterFlow)
    {
        omci_avl_destroy(pData->filterFlow, NULL);
        pData->filterFlow = NULL;
    }
}


void ca_omci_analysis_flow_tbl_init(ca_omci_flow_t *pData)
{
    OMCI_PARAM_IS_NULL(pData);

    pData->tcCfg = omci_avl_create((omci_avl_comparison_func*)ca_omci_compare_tcont_key,
                                   (void *)(sizeof(ca_omci_tcont_cfg_t)),
                                    sizeof(ca_omci_tcont_cfg_t),
                                    NULL);
    if (NULL == pData->tcCfg)
    {
        ca_omci_analysis_flow_tbl_destory(pData);
        OMCIALERR("create tcont config table failed!");
        return;
    }

    pData->gemFlow = omci_avl_create((omci_avl_comparison_func*)ca_omci_compare_gem_key,
                                     (void *)(sizeof(ca_omci_gem_flow_t)),
                                      sizeof(ca_omci_gem_flow_t),
                                      NULL);
    if (NULL == pData->gemFlow)
    {
        ca_omci_analysis_flow_tbl_destory(pData);
        OMCIALERR("create gem flow table failed!");
        return;
    }

    pData->usFlow = omci_avl_create((omci_avl_comparison_func*)ca_omci_compare_stream_key,
                                    (void *)(sizeof(ca_omci_stream_cfg_t)),
                                     sizeof(ca_omci_stream_cfg_t),
                                     NULL);
    if (NULL == pData->usFlow)
    {
        ca_omci_analysis_flow_tbl_destory(pData);
        OMCIALERR("create us stream table failed!");
        return;
    }

    pData->dsFlow = omci_avl_create((omci_avl_comparison_func*)ca_omci_compare_stream_key,
                                    (void *)(sizeof(ca_omci_stream_cfg_t)),
                                     sizeof(ca_omci_stream_cfg_t),
                                     NULL);
    if (NULL == pData->dsFlow)
    {
        ca_omci_analysis_flow_tbl_destory(pData);
        OMCIALERR("create ds stream table failed!");
        return;
    }

    pData->filterFlow = omci_avl_create((omci_avl_comparison_func*)ca_omci_compare_filter_stream_key,
                                        (void *)(sizeof(ca_omci_filter_stream_cfg_t)),
                                         sizeof(ca_omci_filter_stream_cfg_t),
                                         NULL);
    if (NULL == pData->filterFlow)
    {
        ca_omci_analysis_flow_tbl_destory(pData);
        OMCIALERR("create filter stream table failed!");
        return;
    }
}


void ca_omci_analysis_flow_tbl_show(const struct shell *cli, ca_omci_uint8_t flowType)
{
    struct avl_traverser         avlTrav;
    ca_omci_gem_flow_t          *flowCfg = NULL;
    ca_omci_filter_stream_cfg_t *filter  = NULL;
    ca_omci_stream_cfg_t        *stream  = NULL;
    ca_omci_uint8_t              idx     = 0;

    //show connection in hal
    if (CA_OSAL_OK != ca_sem_wait(g_omciAnalysisSem, 0))
    {
        shell_print(cli, "Analysis task is busy, please wait for a moment");
        return;
    }

    if ((OMCI_FLOW_TYPE_GEM == flowType) && (NULL != sg_omciCfgFlowTbl.flow.gemFlow))
    {
        shell_print(cli, "======================= GEMFLOW =========================");
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.gemFlow);
        while (NULL != flowCfg)
        {
            // get all flow cfg then to cfg them
            shell_print(cli, "gemId %d direction %d", flowCfg->gemId, flowCfg->direction);
            shell_print(cli, "    tcontId 0x%x allocId 0x%x", flowCfg->tcontId, flowCfg->allocId);
            shell_print(cli, "    usQueue 0x%x Weight %d ColorMark %d", flowCfg->usQueue, flowCfg->usQueueWeight, flowCfg->usQueueDpcm);
            shell_print(cli, "        usTrafficDes mode %d pps %d cir %d cbs %d pir %d pbs %d",
                           flowCfg->usTrafficDes.mode,
                           flowCfg->usTrafficDes.pps,
                           flowCfg->usTrafficDes.cir,
                           flowCfg->usTrafficDes.cbs,
                           flowCfg->usTrafficDes.pir,
                           flowCfg->usTrafficDes.pbs);
            shell_print(cli, "    dsQueue 0x%x Weight %d ColorMark %d", flowCfg->dsQueue, flowCfg->dsQueueWeight, flowCfg->dsQueueDpcm);
            shell_print(cli, "        dsTrafficDes mode %d pps %d cir %d cbs %d pir %d pbs %d",
                           flowCfg->dsTrafficDes.mode,
                           flowCfg->dsTrafficDes.pps,
                           flowCfg->dsTrafficDes.cir,
                           flowCfg->dsTrafficDes.cbs,
                           flowCfg->dsTrafficDes.pir,
                           flowCfg->dsTrafficDes.pbs);
            shell_print(cli, "    aesState %d aesKeyRing %d transUntag %d", flowCfg->aesState, flowCfg->aesKeyRing, flowCfg->transUntag);
            omci_sprint(cli, "    gemPri 0x%x", flowCfg->gemPri);
            for (idx = 0; idx < OMCI_VLAN_LIST_NUM; idx++)
            {
                if (0 !=  flowCfg->vidList[idx])
                {
                    omci_sprint(cli, " Vid%d %d", idx, flowCfg->vidList[idx]);
                }
            }
            omci_sprint(cli, "\r\n");

            flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
            shell_print(cli, "=========================================================");
        }
    }

    //show stream in hal
    if ((OMCI_FLOW_TYPE_US_STR == flowType) && (NULL != sg_omciCfgFlowTbl.flow.usFlow))
    {
        shell_print(cli, "======================= STREAM_US =========================");
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.usFlow);
        while (NULL != stream)
        {
            // get all stream cfg then to show them
            shell_print(cli, "gemId %d uniPortId 0x%x portType %d dsMode %d filterPri 0x%x",
                            stream->gemId, stream->uniPortId,
                            stream->portType, stream->dsMode,
                            stream->filterPri);
            shell_print(cli, "    Input TPID 0x%x output TPID 0x%x",
                            stream->inputTpid, stream->outputTpid);
            shell_print(cli, "    Filter TPID inner %s | outer %s ",
                            ca_omci_tpid_filter_str[stream->innerTpid.filter],
                            ca_omci_tpid_filter_str[stream->outerTpid.filter]);
            shell_print(cli, "    Treat TPID inner %s | outer %s ",
                            ca_omci_tpid_treat_str[stream->innerTpid.treat],
                            ca_omci_tpid_treat_str[stream->outerTpid.treat]);
            //show untag rule
            if (CA_OMCI_TRUE == stream->untagFilterDef.isValid)
            {
                shell_print(cli, "    Untag Default Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterDef.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterDef.innerVlanTreatment.vid,
                                stream->untagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterDef.outerVlanTreatment.vid,
                                stream->untagFilterDef.outerVlanTreatment.pri);
                if (0 != stream->untagFilterDef.dsTagAction)
                {
                    shell_print(cli, "        ds Uni Tag Opreation %d", stream->untagFilterDef.dsTagAction);
                }
            }

            if (CA_OMCI_TRUE == stream->untagFilterIpoe.isValid)
            {
                shell_print(cli, "    Untag Ipoe Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterIpoe.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterIpoe.innerVlanTreatment.vid,
                                stream->untagFilterIpoe.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterIpoe.outerVlanTreatment.vid,
                                stream->untagFilterIpoe.outerVlanTreatment.pri);
                if (0 != stream->untagFilterIpoe.dsTagAction)
                {
                    shell_print(cli, "        ds Uni Tag Opreation %d", stream->untagFilterIpoe.dsTagAction);
                }
            }

            if (CA_OMCI_TRUE == stream->untagFilterIpoev6.isValid)
            {
                shell_print(cli, "    Untag Ipoev6 Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterIpoev6.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterIpoev6.innerVlanTreatment.vid,
                                stream->untagFilterIpoev6.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterIpoev6.outerVlanTreatment.vid,
                                stream->untagFilterIpoev6.outerVlanTreatment.pri);
                if (0 != stream->untagFilterIpoev6.dsTagAction)
                {
                    shell_print(cli, "        ds Uni Tag Opreation %d", stream->untagFilterIpoev6.dsTagAction);
                }
            }

            if (CA_OMCI_TRUE == stream->untagFilterPppoe.isValid)
            {
                shell_print(cli, "    Untag PPPoE Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterPppoe.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterPppoe.innerVlanTreatment.vid,
                                stream->untagFilterPppoe.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterPppoe.outerVlanTreatment.vid,
                                stream->untagFilterPppoe.outerVlanTreatment.pri);
                if (0 != stream->untagFilterPppoe.dsTagAction)
                {
                    shell_print(cli, "        ds Uni Tag Opreation %d", stream->untagFilterPppoe.dsTagAction);
                }
            }

            if (CA_OMCI_TRUE == stream->untagFilterArp.isValid)
            {
                shell_print(cli, "    Untag ARP Rule: ");
                shell_print(cli, "        fwdType %s ", ca_omci_untag_action_str[stream->untagFilterArp.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterArp.innerVlanTreatment.vid,
                                stream->untagFilterArp.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterArp.outerVlanTreatment.vid,
                                stream->untagFilterArp.outerVlanTreatment.pri);
                if (0 != stream->untagFilterArp.dsTagAction)
                {
                    shell_print(cli, "        ds Uni Tag Opreation %d", stream->untagFilterArp.dsTagAction);
                }
            }

            //single tag rules
            if (CA_OMCI_TRUE == stream->sglTagFilterDef.isValid)
            {
                shell_print(cli, "    Single Tag Default Rule: ");
                shell_print(cli, "        fwdType :%s  for etherType %s",
                                ca_omci_sgl_tag_action_str[stream->sglTagFilterDef.fwdType],
                                ca_omci_ethertype_str[stream->sglTagFilterDef.etherTypeFilter]);
                shell_print(cli, "        filter vid %d pri %d",
                                stream->sglTagFilterDef.vlanFilter.vid,
                                stream->sglTagFilterDef.vlanFilter.pri);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->sglTagFilterDef.innerVlanTreatment.vid,
                                stream->sglTagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->sglTagFilterDef.outerVlanTreatment.vid,
                                stream->sglTagFilterDef.outerVlanTreatment.pri);
            }

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                if (CA_OMCI_TRUE == stream->sglTagFilter[idx].isValid)
                {
                    shell_print(cli, "    Single Tag Rule idx[%d] : ", idx);
                    shell_print(cli, "        fwdType :%s  for etherType %s",
                                    ca_omci_sgl_tag_action_str[stream->sglTagFilter[idx].fwdType],
                                    ca_omci_ethertype_str[stream->sglTagFilter[idx].etherTypeFilter]);
                    shell_print(cli, "        filter vid %d pri %d",
                                    stream->sglTagFilter[idx].vlanFilter.vid,
                                    stream->sglTagFilter[idx].vlanFilter.pri);
                    shell_print(cli, "        inner vid %d pri %d",
                                    stream->sglTagFilter[idx].innerVlanTreatment.vid,
                                    stream->sglTagFilter[idx].innerVlanTreatment.pri);
                    shell_print(cli, "        outer vid %d pri %d",
                                    stream->sglTagFilter[idx].outerVlanTreatment.vid,
                                    stream->sglTagFilter[idx].outerVlanTreatment.pri);
                    if (0 != stream->sglTagFilter[idx].dsTagAction)
                    {
                        shell_print(cli, "        ds Uni Tag Opreation %d", stream->sglTagFilter[idx].dsTagAction);
                    }
                }
            }

            //double tag rules
            if (CA_OMCI_TRUE == stream->dblTagFilterDef.isValid)
            {
                shell_print(cli, "    Double Tag Default Rule: ");
                shell_print(cli, "        fwdType :%s  for etherType %s",
                                ca_omci_dbl_tag_action_str[stream->dblTagFilterDef.fwdType],
                                ca_omci_ethertype_str[stream->dblTagFilterDef.etherTypeFilter]);
                shell_print(cli, "        filter inner vid %d pri %d",
                                stream->dblTagFilterDef.innerVlanFilter.vid,
                                stream->dblTagFilterDef.innerVlanFilter.pri);
                shell_print(cli, "        filter outer vid %d pri %d",
                                stream->dblTagFilterDef.outerVlanFilter.vid,
                                stream->dblTagFilterDef.outerVlanFilter.pri);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->dblTagFilterDef.innerVlanTreatment.vid,
                                stream->dblTagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->dblTagFilterDef.outerVlanTreatment.vid,
                                stream->dblTagFilterDef.outerVlanTreatment.pri);
            }

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                if (CA_OMCI_TRUE == stream->dblTagFilter[idx].isValid)
                {
                    shell_print(cli, "    Double Tag Rule idx [%d]: ", idx);
                    shell_print(cli, "        fwdType :%s  for etherType %s",
                                    ca_omci_dbl_tag_action_str[stream->dblTagFilter[idx].fwdType],
                                    ca_omci_ethertype_str[stream->dblTagFilter[idx].etherTypeFilter]);
                    shell_print(cli, "        filter inner vid %d pri %d",
                                    stream->dblTagFilter[idx].innerVlanFilter.vid,
                                    stream->dblTagFilter[idx].innerVlanFilter.pri);
                    shell_print(cli, "        filter outer vid %d pri %d",
                                    stream->dblTagFilter[idx].outerVlanFilter.vid,
                                    stream->dblTagFilter[idx].outerVlanFilter.pri);
                    shell_print(cli, "        inner vid %d pri %d",
                                    stream->dblTagFilter[idx].innerVlanTreatment.vid,
                                    stream->dblTagFilter[idx].innerVlanTreatment.pri);
                    shell_print(cli, "        outer vid %d pri %d",
                                    stream->dblTagFilter[idx].outerVlanTreatment.vid,
                                    stream->dblTagFilter[idx].outerVlanTreatment.pri);
                }
            }

            stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            shell_print(cli, "=========================================================");
        }
    }


    //show filter class
    if ((OMCI_FLOW_TYPE_FILTER == flowType) && (NULL != sg_omciCfgFlowTbl.flow.filterFlow))
    {
        shell_print(cli, "======================= FilterStream =========================");
        filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.filterFlow);
        while (NULL != filter)
        {
            shell_print(cli, "gemId %d direction %d color mark %d",
                      filter->gemId, filter->direction, filter->usQueueDpcm);
            shell_print(cli, "    uniPortInstance 0x%x portType %d ",
                      filter->uniPortId, filter->portType);
            shell_print(cli, "    inputTpid 0x%x outputTpid 0x%x ",
                      filter->inputTpid, filter->outputTpid);
            omci_sprint(cli, "    filter PriBit 0x%x", filter->priBit);
            for (idx = 0; idx < OMCI_VLAN_LIST_NUM; idx++)
            {
                if (0 !=  filter->vidList[idx])
                {
                    omci_sprint(cli, " Vid%d %d", idx, filter->vidList[idx]);
                }
            }
            omci_sprint(cli, "\r\n");

            shell_print(cli, "    externVlanFlag %d", filter->externVlanFlag);
            shell_print(cli, "    dsQueue 0x%x dsQueueWeight %d color mark %d", filter->dsQueue, filter->dsQueueWeight, filter->dsQueueDpcm);

            filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            shell_print(cli, "=========================================================");
        }
    }

    //show filter class
    if ((OMCI_FLOW_TYPE_DS_STR == flowType) && (NULL != sg_omciCfgFlowTbl.flow.dsFlow))
    {
        shell_print(cli, "======================= STREAM_DS =========================");
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.dsFlow);
        while (NULL != stream)
        {
            // get all stream cfg then to show them
            shell_print(cli, "gemId %d uniPortId 0x%x portType %d dsMode %d filterPri 0x%x",
                            stream->gemId, stream->uniPortId,
                            stream->portType, stream->dsMode,
                            stream->filterPri);

            //show untag rule
            if (CA_OMCI_TRUE == stream->untagFilterDef.isValid)
            {
                shell_print(cli, "    Untag Default Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterDef.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterDef.innerVlanTreatment.vid,
                                stream->untagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterDef.outerVlanTreatment.vid,
                                stream->untagFilterDef.outerVlanTreatment.pri);
            }

            if (CA_OMCI_TRUE == stream->untagFilterIpoe.isValid)
            {
                shell_print(cli, "    Untag Ipoe Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterIpoe.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterIpoe.innerVlanTreatment.vid,
                                stream->untagFilterIpoe.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterIpoe.outerVlanTreatment.vid,
                                stream->untagFilterIpoe.outerVlanTreatment.pri);
            }

            if (CA_OMCI_TRUE == stream->untagFilterIpoev6.isValid)
            {
                shell_print(cli, "    Untag Ipoev6 Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterIpoev6.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterIpoev6.innerVlanTreatment.vid,
                                stream->untagFilterIpoev6.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterIpoev6.outerVlanTreatment.vid,
                                stream->untagFilterIpoev6.outerVlanTreatment.pri);
            }

            if (CA_OMCI_TRUE == stream->untagFilterPppoe.isValid)
            {
                shell_print(cli, "    Untag PPPoE Rule: ");
                shell_print(cli, "        fwdType :%s ", ca_omci_untag_action_str[stream->untagFilterPppoe.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterPppoe.innerVlanTreatment.vid,
                                stream->untagFilterPppoe.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterPppoe.outerVlanTreatment.vid,
                                stream->untagFilterPppoe.outerVlanTreatment.pri);
            }

            if (CA_OMCI_TRUE == stream->untagFilterArp.isValid)
            {
                shell_print(cli, "    Untag ARP Rule: ");
                shell_print(cli, "        fwdType %d ", ca_omci_untag_action_str[stream->untagFilterArp.fwdType]);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->untagFilterArp.innerVlanTreatment.vid,
                                stream->untagFilterArp.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->untagFilterArp.outerVlanTreatment.vid,
                                stream->untagFilterArp.outerVlanTreatment.pri);
            }

            //single tag rules
            if (CA_OMCI_TRUE == stream->sglTagFilterDef.isValid)
            {
                shell_print(cli, "    Single Tag Default Rule: ");
                shell_print(cli, "        fwdType :%s  for etherType %s",
                                ca_omci_sgl_tag_action_str[stream->sglTagFilterDef.fwdType],
                                ca_omci_ethertype_str[stream->sglTagFilterDef.etherTypeFilter]);
                shell_print(cli, "        filter vid %d pri %d",
                                stream->sglTagFilterDef.vlanFilter.vid,
                                stream->sglTagFilterDef.vlanFilter.pri);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->sglTagFilterDef.innerVlanTreatment.vid,
                                stream->sglTagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->sglTagFilterDef.outerVlanTreatment.vid,
                                stream->sglTagFilterDef.outerVlanTreatment.pri);
            }

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                if (CA_OMCI_TRUE == stream->sglTagFilter[idx].isValid)
                {
                    shell_print(cli, "    Single Tag Rule idx[%d] : ", idx);
                    shell_print(cli, "        fwdType :%s  for etherType %s",
                                    ca_omci_sgl_tag_action_str[stream->sglTagFilter[idx].fwdType],
                                    ca_omci_ethertype_str[stream->sglTagFilter[idx].etherTypeFilter]);
                    shell_print(cli, "        filter vid %d pri %d",
                                    stream->sglTagFilter[idx].vlanFilter.vid,
                                    stream->sglTagFilter[idx].vlanFilter.pri);
                    shell_print(cli, "        inner vid %d pri %d",
                                    stream->sglTagFilter[idx].innerVlanTreatment.vid,
                                    stream->sglTagFilter[idx].innerVlanTreatment.pri);
                    shell_print(cli, "        outer vid %d pri %d",
                                    stream->sglTagFilter[idx].outerVlanTreatment.vid,
                                    stream->sglTagFilter[idx].outerVlanTreatment.pri);
                }
            }

            //double tag rules
            if (CA_OMCI_TRUE == stream->dblTagFilterDef.isValid)
            {
                shell_print(cli, "    Double Tag Default Rule: ");
                shell_print(cli, "        fwdType :%s  for etherType %s",
                                ca_omci_dbl_tag_action_str[stream->dblTagFilterDef.fwdType],
                                ca_omci_ethertype_str[stream->dblTagFilterDef.etherTypeFilter]);
                shell_print(cli, "        filter inner vid %d pri %d",
                                stream->dblTagFilterDef.innerVlanFilter.vid,
                                stream->dblTagFilterDef.innerVlanFilter.pri);
                shell_print(cli, "        filter outer vid %d pri %d",
                                stream->dblTagFilterDef.outerVlanFilter.vid,
                                stream->dblTagFilterDef.outerVlanFilter.pri);
                shell_print(cli, "        inner vid %d pri %d",
                                stream->dblTagFilterDef.innerVlanTreatment.vid,
                                stream->dblTagFilterDef.innerVlanTreatment.pri);
                shell_print(cli, "        outer vid %d pri %d",
                                stream->dblTagFilterDef.outerVlanTreatment.vid,
                                stream->dblTagFilterDef.outerVlanTreatment.pri);
            }

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                if (CA_OMCI_TRUE == stream->dblTagFilter[idx].isValid)
                {
                    shell_print(cli, "    Double Tag Rule idx [%d]: ", idx);
                    shell_print(cli, "        fwdType :%s  for etherType %s",
                                    ca_omci_dbl_tag_action_str[stream->dblTagFilter[idx].fwdType],
                                    ca_omci_ethertype_str[stream->dblTagFilter[idx].etherTypeFilter]);
                    shell_print(cli, "        filter inner vid %d pri %d",
                                    stream->dblTagFilter[idx].innerVlanFilter.vid,
                                    stream->dblTagFilter[idx].innerVlanFilter.pri);
                    shell_print(cli, "        filter outer vid %d pri %d",
                                    stream->dblTagFilter[idx].outerVlanFilter.vid,
                                    stream->dblTagFilter[idx].outerVlanFilter.pri);
                    shell_print(cli, "        inner vid %d pri %d",
                                    stream->dblTagFilter[idx].innerVlanTreatment.vid,
                                    stream->dblTagFilter[idx].innerVlanTreatment.pri);
                    shell_print(cli, "        outer vid %d pri %d",
                                    stream->dblTagFilter[idx].outerVlanTreatment.vid,
                                    stream->dblTagFilter[idx].outerVlanTreatment.pri);
                }
            }

            stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            shell_print(cli, "=========================================================");
        }
    }

    ca_sem_post(g_omciAnalysisSem);
}


void ca_omci_analysis_flow_cfg_tbl_finish(void)
{
    ca_omci_analysis_flow_tbl_destory(&sg_omciCfgFlowTbl.flow);
}


void ca_omci_analysis_flow_cfg_tbl_init(void)
{
    OMCI_MEMSET(&sg_omciCfgFlowTbl, 0, sizeof(ca_omci_analyse_flow_t));
    ca_omci_analysis_flow_tbl_init(&sg_omciCfgFlowTbl.flow);
}


void ca_omci_gemflow_tbl_cfg_save(ca_omci_gem_flow_t *pData)
{
    ca_omci_gem_flow_t *pGemFlow = NULL;

    OMCI_ENTER;
    pGemFlow = (ca_omci_gem_flow_t *)omci_avl_find(sg_omciCfgFlowTbl.flow.gemFlow, pData);
    if (NULL != pGemFlow)
    {
        OMCIALERR("no need to save a same gem flow now ");
        return;
    }

    pGemFlow = omci_avl_insert(sg_omciCfgFlowTbl.flow.gemFlow, pData);
    if (NULL == pGemFlow)
    {
        OMCIALERR("save gem flow failed");
        return;
    }
}


void ca_omci_gemflow_tbl_add(ca_omci_gem_flow_t *pData)
{
    ca_omci_gem_flow_t *pGemFlow = NULL;

    OMCI_ENTER;
    pGemFlow = (ca_omci_gem_flow_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.gemFlow, pData);
    if (NULL != pGemFlow)
    {
        OMCIALERR("add same gem flow now ");
        return;
    }

    pGemFlow = omci_avl_insert(sg_omciAnalyseFlowTbl.flow.gemFlow, pData);
    if (NULL == pGemFlow)
    {
        OMCIALERR("add gem flow failed");
        return;
    }
    ca_omci_gemflow_dbg_print(pGemFlow);
    OMCI_LEAVE;
}


void ca_omci_tcont_cfg_tbl_save(ca_omci_tcont_cfg_t *pData)
{
    ca_omci_tcont_cfg_t *pTcCfg = NULL;

    OMCI_ENTER;
    pTcCfg = (ca_omci_tcont_cfg_t *)omci_avl_find(sg_omciCfgFlowTbl.flow.tcCfg, pData);
    if (NULL != pTcCfg)
    {
        OMCIALERR("no need to save a same tcont now ");
        return;
    }

    pTcCfg = omci_avl_insert(sg_omciCfgFlowTbl.flow.tcCfg, pData);
    if (NULL == pTcCfg)
    {
        OMCIALERR("save tcont failed");
        return;
    }
}


void ca_omci_tcont_cfg_tbl_add(ca_omci_tcont_cfg_t *pData)
{
    ca_omci_tcont_cfg_t *pTcCfg = NULL;

    OMCI_ENTER;
    pTcCfg = (ca_omci_tcont_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.tcCfg, pData);
    if (NULL != pTcCfg)
    {
        OMCIALERR("add same tcont now ");
        return;
    }

    pTcCfg = omci_avl_insert(sg_omciAnalyseFlowTbl.flow.tcCfg, pData);
    if (NULL == pTcCfg)
    {
        OMCIALERR("add tcont failed");
        return;
    }
    ca_omci_tcont_dbg_print(pTcCfg);
    OMCI_LEAVE;
}


ca_omci_analyse_flow_t *ca_omci_analysis_cfg_tbl_get(void)
{
    return &sg_omciCfgFlowTbl;
}


void ca_omci_us_stream_tbl_cfg_save(ca_omci_stream_cfg_t *pData)
{
    ca_omci_stream_cfg_t *pUsFlow = NULL;

    OMCI_ENTER;
    pUsFlow = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciCfgFlowTbl.flow.usFlow, pData);
    if (NULL != pUsFlow)
    {
        OMCIALERR("no need to save a same us flow now ");
        return;
    }

    pUsFlow = omci_avl_insert(sg_omciCfgFlowTbl.flow.usFlow, pData);
    if (NULL == pUsFlow)
    {
        OMCIALERR("save us flow failed");
        return;
    }
}


void ca_omci_filter_stream_tbl_cfg_save(ca_omci_filter_stream_cfg_t *pData)
{
    ca_omci_filter_stream_cfg_t *pFilterFlow = NULL;

    OMCI_ENTER;
    pFilterFlow = (ca_omci_filter_stream_cfg_t *)omci_avl_find(sg_omciCfgFlowTbl.flow.filterFlow, pData);
    if (NULL != pFilterFlow)
    {
        OMCIALERR("no need to save a same filter flow now ");
        return;
    }

    pFilterFlow = omci_avl_insert(sg_omciCfgFlowTbl.flow.filterFlow, pData);
    if (NULL == pFilterFlow)
    {
        OMCIALERR("save filter flow failed");
        return;
    }
}


void ca_omci_ds_ivs_stream_tbl_cfg_save(ca_omci_stream_cfg_t *pData)
{
    ca_omci_stream_cfg_t *pDsFlow = NULL;

    OMCI_ENTER;
    pDsFlow = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciCfgFlowTbl.flow.dsFlow, pData);
    if (NULL != pDsFlow)
    {
        OMCIALERR("no need to save a same ds flow now ");
        return;
    }

    pDsFlow = omci_avl_insert(sg_omciCfgFlowTbl.flow.dsFlow, pData);
    if (NULL == pDsFlow)
    {
        OMCIALERR("save us flow failed");
        return;
    }
}



void ca_omci_ds_mcast_gemid_get(ca_omci_uint16_t *gemId)
{
    ca_omci_meclass_t         *meClass    = NULL;
    ca_omci_mebase_t          *meInst     = NULL;
    ca_omci_multi_gem_iwtp_t  *multiGem   = NULL;
    ca_omci_gem_net_ctp_t     *gpnctp     = NULL;

    meClass = ca_omci_me_get(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIALERR("no multicast gemport is created now !\n");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        multiGem = (ca_omci_multi_gem_iwtp_t *)meInst;
        gpnctp   = (ca_omci_gem_net_ctp_t *)(ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, multiGem->attr.gemNetCtpPoint));
        if (NULL != gpnctp)
        {
            *gemId = gpnctp->attr.portId;
            break;
        }
    }
}
#endif


#if DESCRIPTION("Stream relate functions")
void ca_omci_stream_tbl_double_tag_set(ca_omci_dbl_tag_filter_t *dblTag, ca_omci_rx_vlan_tbl_t *rule)
{
    ca_omci_filter_inner_t  *infilter  = NULL;
    ca_omci_filter_outter_t *outfilter = NULL;
    ca_omci_treat_outter_t  *outtreat  = NULL;
    ca_omci_treat_inner_t   *intreat   = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(dblTag);
    OMCI_PARAM_IS_NULL(rule);

    outfilter = (ca_omci_filter_outter_t *)(&(rule->filterOutter));
    infilter  = (ca_omci_filter_inner_t *)(&(rule->filterInner));
    outtreat  = (ca_omci_treat_outter_t *)(&(rule->treatOutter));
    intreat   = (ca_omci_treat_inner_t *)(&(rule->treatInner));

    if (OMCI_TREATMENT_REMOVE_ZERO_TAG == outtreat->trearTag2Remove)
    {
        //for ALCL OLT double tag rule will add inner vid, so just treat inner vid for outter
        dblTag->isValid                = CA_OMCI_TRUE;
        dblTag->innerVlanFilter.vid    = infilter->filterInnVid;
        dblTag->innerVlanFilter.pri    = (OMCI_FILTER_PRI_DEF == infilter->filterInnPri) ? OMCI_FILTER_PRI_NONE : infilter->filterInnPri;
        dblTag->outerVlanFilter.vid    = outfilter->filterOutVid;
        dblTag->outerVlanFilter.pri    = (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri) ? OMCI_FILTER_PRI_NONE : outfilter->filterOutPri;
        dblTag->etherTypeFilter        = infilter->filterEthType; // now it matches, if value changes, need to add an map
        dblTag->outerVlanTreatment.vid = intreat->treatInnVid;
        dblTag->outerVlanTreatment.pri = intreat->treatInnPri;
        dblTag->fwdType                = CA_OMCI_DBL_TAG_ADD_TAG;

        OMCIALDBG("ALCL filter double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                  dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                  dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
        OMCIALDBG("Add vid [%d] pri [%d]", dblTag->outerVlanTreatment.vid, dblTag->outerVlanTreatment.pri);
        return;
    }

    if (OMCI_TREATMENT_REMOVE_ONE_TAG == outtreat->trearTag2Remove)
    {
        if (OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri)
        {
            dblTag->isValid                = CA_OMCI_TRUE;
            dblTag->innerVlanFilter.vid    = infilter->filterInnVid;
            dblTag->innerVlanFilter.pri    = infilter->filterInnPri;
            dblTag->outerVlanFilter.vid    = outfilter->filterOutVid;
            dblTag->outerVlanFilter.pri    = (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri) ? OMCI_FILTER_PRI_NONE : outfilter->filterOutPri;
            dblTag->etherTypeFilter        = infilter->filterEthType; // now it matches, if value changes, need to add an map
            dblTag->outerVlanTreatment.vid = intreat->treatInnVid;
            dblTag->outerVlanTreatment.pri = intreat->treatInnPri;
            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG;

            OMCIALDBG("ALCL filter double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                      dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                      dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);

            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OVID;
                dblTag->outerVlanTreatment.pri = 0;
                OMCIALDBG("Modify outer vid only");
            }
            OMCIALDBG("Vid to %d pri to %d", dblTag->outerVlanTreatment.vid, dblTag->outerVlanTreatment.pri);
            return;
        }
    }

    dblTag->isValid             = CA_OMCI_TRUE;
    dblTag->innerVlanFilter.vid = infilter->filterInnVid;
    dblTag->innerVlanFilter.pri = infilter->filterInnPri;
    dblTag->outerVlanFilter.vid = outfilter->filterOutVid;
    dblTag->outerVlanFilter.pri = (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri) ? OMCI_FILTER_PRI_NONE : outfilter->filterOutPri;
    dblTag->etherTypeFilter     = infilter->filterEthType; // now it matches, if value changes, need to add an map

    // no need to analysis other values, single packet drop
    if (OMCI_TREATMENT_DISCARD == outtreat->trearTag2Remove)
    {
        dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_DROP;
        OMCIALDBG("Drop all match double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                  dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                  dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
        return;
    }

    if (OMCI_TREATMENT_REMOVE_ZERO_TAG == outtreat->trearTag2Remove)
    {
        dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_TRANSPARENT;
        OMCIALDBG("Transparent all match double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                  dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                  dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
        return;
    }

    if (OMCI_TREATMENT_REMOVE_ONE_TAG == outtreat->trearTag2Remove)
    {
        if ((OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
         && (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid))
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_TRANSPARENT;
            OMCIALDBG("Transparent all match double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                      dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                      dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
            return;
        }

        if (OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_OTAG;
            OMCIALDBG("Remove outer tag for match double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                      dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                      dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
            return;
        }

        if (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid)
        {
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_ODSCP;
                OMCIALDBG("modify double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d] out pri as dscp value",
                          dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                          dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
                return;
            }

            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI;
            dblTag->outerVlanTreatment.pri = outtreat->treatOutPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
            {
                if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
                {
                    OMCIALERR("ERROR: Can't copy inner pri without specfic");
                    dblTag->isValid             = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid = 0;
                    dblTag->innerVlanFilter.pri = 0;
                    dblTag->outerVlanFilter.vid = 0;
                    dblTag->outerVlanFilter.pri = 0;
                    dblTag->etherTypeFilter     = 0;
                    return;
                }
                dblTag->outerVlanTreatment.pri = infilter->filterInnPri;
            }
            OMCIALDBG("Modify outer tag pri [%d] only", dblTag->outerVlanTreatment.pri);
            return;
        }

        dblTag->outerVlanTreatment.vid = outtreat->treatOutVid;
        if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
        {
            if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
            {
                OMCIALERR("ERROR: Can't copy inner vid without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                return;
            }
            dblTag->outerVlanTreatment.vid = infilter->filterInnVid;
        }

        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP;
            OMCIALDBG("Modify outer tag vid [%d] pri base on dscp", dblTag->outerVlanTreatment.vid);
            return;
        }

        if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID;
            OMCIALDBG("Modify outer tag vid [%d] keep ori pri", dblTag->outerVlanTreatment.vid);
            return;
        }

        dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG;
        dblTag->outerVlanTreatment.pri = outtreat->treatOutPri;
        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
        {
            if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
            {
                OMCIALERR("ERROR: Can't copy inner pri without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                dblTag->outerVlanTreatment.pri = 0;
                return;
            }
            dblTag->outerVlanTreatment.pri = infilter->filterInnPri;
        }
        OMCIALDBG("Modify outer tag vid [%d] pri [%d]", dblTag->outerVlanTreatment.vid, dblTag->outerVlanTreatment.pri);
        return;
    }

    if (OMCI_TREATMENT_REMOVE_TWO_TAGS == outtreat->trearTag2Remove)
    {
        OMCIALDBG("Match double tag packets outer vid [%d] pri [%d] inner vid [%d] pri [%d]",
                  dblTag->outerVlanFilter.vid, dblTag->outerVlanFilter.pri,
                  dblTag->innerVlanFilter.vid, dblTag->innerVlanFilter.pri);
        if (OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
        {
            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAGS;
                OMCIALDBG("Remove both tags");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP;
                    OMCIALDBG("Remove outer tags and set dscp");
                    return;
                }

                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_ITAG;
                    OMCIALDBG("Remove outer tags and set inner tag same as outer tag");
                    return;
                }

                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
                    {
                        OMCIALERR("ERROR: Can't copy inner pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = infilter->filterInnPri;
                }
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI;
                OMCIALDBG("Remove outer tags and set inner tag priority[%d]", dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
                {
                    OMCIALERR("ERROR: Can't copy inner pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = infilter->filterInnVid;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP;
                OMCIALDBG("Remove outer tags and set vid[%d] dscp", dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID;
                OMCIALDBG("Remove outer tags and set vid[%d]", dblTag->innerVlanTreatment.vid);
                return;
            }

            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
                {
                    OMCIALERR("ERROR: Can't copy inner pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = infilter->filterInnPri;
            }
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG;
            OMCIALDBG("Remove outer tags and set inner vid[%d] priority[%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        if ((OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
         && (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid)
         && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
         && (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid))
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_TRANSPARENT;
            OMCIALDBG("Transparent all");
            return;
        }

        if ((OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
         && (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid))
        {
            OMCIALDBG("Out tag remain the same");
            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_ITAG;
                OMCIALDBG("Remove inner Tag");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_IDSCP;
                    OMCIALDBG("Modify inner tag priority base on DSCP");
                    return;
                }

                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_IPRI;
                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                    {
                        OMCIALERR("ERROR: Can't copy out pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
                }

                OMCIALDBG("Modify inner tag priority to [%d] only", dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                {
                    OMCIALERR("ERROR: Can't copy out vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
            }

            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_IVID_IDSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("Modify inner tag vid to [%d] inner prioriy base on DSCP",
                           dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_IVID;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("Modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy out pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_ITAG;
            OMCIALDBG("Modify inner tag vid to [%d] priority [%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        //out tag only change dscp priority
        if ((OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
         && (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid))
        {
            OMCIALDBG("modify out tag priority base on DSCP");

            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP;
                OMCIALDBG("Remove inner Tag and change prority depend on dscp");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_DSCP;
                    OMCIALDBG("modify both tags priority base on DSCP");
                    return;
                }

                if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_ODSCP;
                    OMCIALDBG("modify out tag priority base on DSCP");
                    return;
                }

                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IPRI;
                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                    {
                        OMCIALERR("ERROR: Can't copy out pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
                }
                OMCIALDBG("modify inner tag priority to [%d]", dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                {
                    OMCIALERR("ERROR: Can't copy out vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
            }

            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_DSCP_IVID;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d], priority base on dscp",
                           dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IVID;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy out pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_ODSCP_ITAG;
            OMCIALDBG("modify inner tag vid to [%d], priority to [%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        //out tag only change priority
        if (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid)
        {
            dblTag->outerVlanTreatment.pri = outtreat->treatOutPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
            {
                if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
                {
                    OMCIALERR("ERROR: Can't copy inner pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->outerVlanTreatment.pri = infilter->filterInnPri;
            }
            OMCIALDBG("modify out tag priority to [%d]", dblTag->outerVlanTreatment.pri);

            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI;
                dblTag->outerVlanTreatment.pri = 0;
                OMCIALDBG("Remove inner Tag and change prority");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI_IDSCP;
                    dblTag->innerVlanTreatment.pri = 0;
                    OMCIALDBG("modify inner tag priority base on dscp");
                    return;
                }

                if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI;
                    dblTag->innerVlanTreatment.pri = 0;
                    OMCIALDBG("modify outer tag priority only");
                    return;
                }

                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_PRIS;
                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                    {
                        OMCIALERR("ERROR: Can't copy outer pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->outerVlanTreatment.pri = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
                }
                OMCIALDBG("modify inner tag priority to [%d] only", dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                {
                    OMCIALERR("ERROR: Can't copy outer vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID_IDSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] priority base on dscp",
                           dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
                return;
            }

            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG;
            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy outer pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            OMCIALDBG("modify inner tag vid to [%d] priority to [%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        //out tag only change vid
        if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
        {
            dblTag->outerVlanTreatment.vid = outtreat->treatOutVid;
            if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
            {
                if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
                {
                    OMCIALERR("ERROR: Can't copy inner vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->outerVlanTreatment.vid = infilter->filterInnVid;
            }
            OMCIALDBG("modify out tag vid to [%d]", dblTag->outerVlanTreatment.vid);

            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID;
                OMCIALDBG("Remove inner Tag and change vid");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID_IDSCP;
                    OMCIALDBG("modify inner tag priority base on dscp");
                    return;
                }

                if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID;
                    OMCIALDBG("Remain inner tag");
                    return;
                }

                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                    {
                        OMCIALERR("ERROR: Can't copy outer pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->outerVlanTreatment.vid = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
                }
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID_IPRI;
                OMCIALDBG("modify inner tag priority to [%d]", dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                {
                    OMCIALERR("ERROR: Can't copy outer vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_VIDS_IDSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] priority base on dscp",
                           dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_VIDS;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
                return;
            }

            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OVID_ITAG;
            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy outer pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            OMCIALDBG("modify inner tag vid to [%d] priority to [%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        //out tag change vid and dscp out priority
        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
        {
            dblTag->outerVlanTreatment.vid = outtreat->treatOutVid;
            if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
            {
                if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
                {
                    OMCIALERR("ERROR: Can't copy inner vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    return;
                }
                dblTag->outerVlanTreatment.vid = infilter->filterInnVid;
            }
            OMCIALDBG("modify out tag vid to [%d] priority base on dscp",
                       dblTag->outerVlanTreatment.vid);

            if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP;
                OMCIALDBG("Remove inner Tag and change vid");
                return;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
            {
                if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OVID_DSCP;
                    dblTag->innerVlanTreatment.pri = 0;
                    OMCIALDBG("modify inner tag priority base on dscp");
                    return;
                }

                if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
                {
                    dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP;
                    dblTag->innerVlanTreatment.pri = 0;
                    OMCIALDBG("remain inner tag priority");
                    return;
                }

                dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
                if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
                {
                    if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                    {
                        OMCIALERR("ERROR: Can't copy outer pri without specfic");
                        dblTag->isValid                = CA_OMCI_FALSE;
                        dblTag->innerVlanFilter.vid    = 0;
                        dblTag->innerVlanFilter.pri    = 0;
                        dblTag->outerVlanFilter.vid    = 0;
                        dblTag->outerVlanFilter.pri    = 0;
                        dblTag->etherTypeFilter        = 0;
                        dblTag->outerVlanTreatment.vid = 0;
                        dblTag->outerVlanTreatment.pri = 0;
                        dblTag->innerVlanTreatment.vid = 0;
                        dblTag->innerVlanTreatment.pri = 0;
                        return;
                    }
                    dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
                }
                dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_IPRI;
                OMCIALDBG("modify inner tag priority to [%d]",
                           dblTag->innerVlanTreatment.pri);
                return;
            }

            dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
            if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
            {
                if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                {
                    OMCIALERR("ERROR: Can't copy outer vid without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_VIDS_DSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] priority base on dscp",
                           dblTag->innerVlanTreatment.vid);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_VIDS_ODSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
                return;
            }

            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_ITAG;
            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy outer pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            OMCIALDBG("modify inner tag vid to [%d] priority to [%d]",
                       dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
            return;
        }

        //out tag vid and pri both change
        dblTag->outerVlanTreatment.vid = outtreat->treatOutVid;
        dblTag->outerVlanTreatment.pri = outtreat->treatOutPri;
        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
        {
            if (OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
            {
                OMCIALERR("ERROR: Can't copy inner pri without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                dblTag->outerVlanTreatment.pri = 0;
                dblTag->innerVlanTreatment.vid = 0;
                dblTag->innerVlanTreatment.pri = 0;
                return;
            }
            dblTag->outerVlanTreatment.pri = infilter->filterInnPri;
        }

        if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
        {
            if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
            {
                OMCIALERR("ERROR: Can't copy inner vid without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                dblTag->outerVlanTreatment.pri = 0;
                dblTag->innerVlanTreatment.vid = 0;
                dblTag->innerVlanTreatment.pri = 0;
                return;
            }
            dblTag->outerVlanTreatment.vid = infilter->filterInnVid;
        }

        OMCIALDBG("modify out tag vid to [%d] priority to [%d]",
                   dblTag->outerVlanTreatment.vid, dblTag->outerVlanTreatment.pri);

        if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
        {
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG;
            OMCIALDBG("Remove inner Tag and change Tag");
            return;
        }

        if (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
        {
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG_IDSCP;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("modify inner tag priority base on dscp");
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
            {
                dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG;
                dblTag->innerVlanTreatment.pri = 0;
                OMCIALDBG("remain inner tag priority");
                return;
            }

            dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
            if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
            {
                if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
                {
                    OMCIALERR("ERROR: Can't copy outer pri without specfic");
                    dblTag->isValid                = CA_OMCI_FALSE;
                    dblTag->innerVlanFilter.vid    = 0;
                    dblTag->innerVlanFilter.pri    = 0;
                    dblTag->outerVlanFilter.vid    = 0;
                    dblTag->outerVlanFilter.pri    = 0;
                    dblTag->etherTypeFilter        = 0;
                    dblTag->outerVlanTreatment.vid = 0;
                    dblTag->outerVlanTreatment.pri = 0;
                    dblTag->innerVlanTreatment.vid = 0;
                    dblTag->innerVlanTreatment.pri = 0;
                    return;
                }
                dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
            }
            dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_OTAG_IPRI;
            OMCIALDBG("modify inner tag priority to [%d]",
                       dblTag->innerVlanTreatment.pri);
            return;
        }

        dblTag->innerVlanTreatment.vid = intreat->treatInnVid;
        if (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid)
        {
            if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
            {
                OMCIALERR("ERROR: Can't copy outer vid without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                dblTag->outerVlanTreatment.pri = 0;
                dblTag->innerVlanTreatment.vid = 0;
                dblTag->innerVlanTreatment.pri = 0;
                return;
            }
            dblTag->innerVlanTreatment.vid = outfilter->filterOutVid;
        }

        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
        {
            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID_IDSCP;
            dblTag->innerVlanTreatment.pri = 0;
            OMCIALDBG("modify inner tag vid to [%d] priority base on dscp",
                       dblTag->innerVlanTreatment.vid);
            return;
        }

        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
        {
            dblTag->fwdType                = CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID;
            dblTag->innerVlanTreatment.pri = 0;
            OMCIALDBG("modify inner tag vid to [%d] only", dblTag->innerVlanTreatment.vid);
            return;
        }

        dblTag->innerVlanTreatment.pri = intreat->treatInnPri;
        if (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
        {
            if (OMCI_FILTER_PRI_NONE == outfilter->filterOutPri)
            {
                OMCIALERR("ERROR: Can't copy outer pri without specfic");
                dblTag->isValid                = CA_OMCI_FALSE;
                dblTag->innerVlanFilter.vid    = 0;
                dblTag->innerVlanFilter.pri    = 0;
                dblTag->outerVlanFilter.vid    = 0;
                dblTag->outerVlanFilter.pri    = 0;
                dblTag->etherTypeFilter        = 0;
                dblTag->outerVlanTreatment.vid = 0;
                dblTag->outerVlanTreatment.pri = 0;
                dblTag->innerVlanTreatment.vid = 0;
                dblTag->innerVlanTreatment.pri = 0;
                return;
            }
            dblTag->innerVlanTreatment.pri = outfilter->filterOutPri;
        }
        dblTag->fwdType = CA_OMCI_DBL_TAG_FWD_MD_TAGS;
        OMCIALDBG("modify inner tag vid to [%d] priority to [%d]",
                   dblTag->innerVlanTreatment.vid, dblTag->innerVlanTreatment.pri);
        return;
    }
}


void ca_omci_stream_tbl_single_tag_set(ca_omci_sgl_tag_filter_t *sglTag, ca_omci_rx_vlan_tbl_t *rule)
{
    ca_omci_filter_inner_t  *infilter  = NULL;
    ca_omci_treat_outter_t  *outtreat  = NULL;
    ca_omci_treat_inner_t   *intreat   = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(sglTag);
    OMCI_PARAM_IS_NULL(rule);

    //ignore all outfilter values;
    infilter  = (ca_omci_filter_inner_t *)(&(rule->filterInner));
    outtreat  = (ca_omci_treat_outter_t *)(&(rule->treatOutter));
    intreat   = (ca_omci_treat_inner_t *)(&(rule->treatInner));

    //filter invalid rules
    if (OMCI_TREATMENT_REMOVE_TWO_TAGS == outtreat->trearTag2Remove)
    {
        OMCIALERR("ERROR: single tag rule shouldn't remove 2 tags!");
        return;
    }

    if ((OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
     || (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
     || (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid)
     || (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid))
    {
        OMCIALERR("ERROR: single rule can't copy outer pri or vid!");
        return;
    }

    if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
    {
        if ((OMCI_FILTER_PRI_NONE == infilter->filterInnPri)
         || (OMCI_FILTER_PRI_DEF == infilter->filterInnPri))
        {
            OMCIALERR("ERROR: single rule can't copy inner pri without specific!");
            return;
        }
    }

    if ((OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri)
     && (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid))
    {
        if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
        {
            OMCIALERR("ERROR: single rule treat inner can't copy inner vid without specific!");
            return;
        }
    }

    if ((OMCI_TREATMENT_REMOVE_ZERO_TAG == outtreat->trearTag2Remove)
     && (OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri))
    {
        OMCIALWARN("Warning: Special treatment for single tag rule to add inner tag");

        sglTag->isValid                = CA_OMCI_TRUE;
        sglTag->vlanFilter.pri         = (OMCI_FILTER_PRI_DEF == infilter->filterInnPri) ? OMCI_FILTER_PRI_NONE : infilter->filterInnPri;
        sglTag->vlanFilter.vid         = infilter->filterInnVid;
        sglTag->etherTypeFilter        = infilter->filterEthType; // now it matches, if value changes, need to add an map
        sglTag->outerVlanTreatment.vid = intreat->treatInnVid;
        sglTag->outerVlanTreatment.pri = intreat->treatInnPri;
        sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_ADD_TAG;

        OMCIALDBG("Filter single tag packets vid [%d] pri[%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
        OMCIALDBG("Add vid [%d] pri [%d]", sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri);
        return;
    }

    sglTag->isValid         = CA_OMCI_TRUE;
    sglTag->etherTypeFilter = infilter->filterEthType; // now it matches, if value changes, need to add an map
    sglTag->vlanFilter.pri  = (OMCI_FILTER_PRI_DEF == infilter->filterInnPri) ? OMCI_FILTER_PRI_NONE : infilter->filterInnPri;
    sglTag->vlanFilter.vid  = infilter->filterInnVid;

    // no need to analysis other values, single packet drop
    if (OMCI_TREATMENT_DISCARD == outtreat->trearTag2Remove)
    {
        sglTag->fwdType = CA_OMCI_SGL_TAG_FWD_DROP;
        OMCIALDBG("Drop single tag packets vid [%d] pri [%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
        return;
    }

    // no need to remove tag
    if (OMCI_TREATMENT_REMOVE_ZERO_TAG == outtreat->trearTag2Remove)
    {
        if (OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
        {
            OMCIALDBG("Transparent single tag vid [%d], pri [%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
            sglTag->fwdType = CA_OMCI_SGL_TAG_FWD_TRANSPARENT;
            return;
        }

        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
        {
            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP;
            sglTag->outerVlanTreatment.pri = 0;
            sglTag->outerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid : outtreat->treatOutVid;
            OMCIALDBG("Add outer tag vid [%d], pri [%d]", sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri);
            return;
        }

        sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_ADD_TAG;
        sglTag->outerVlanTreatment.pri = outtreat->treatOutPri;
        sglTag->outerVlanTreatment.vid = outtreat->treatOutVid;
        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
        {
            sglTag->outerVlanTreatment.pri = infilter->filterInnPri;
        }

        if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
        {
            sglTag->outerVlanTreatment.vid = infilter->filterInnVid;
        }

        OMCIALDBG("Add outer tag vid [%d], add pri [%d]", sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri);
        return;
    }

    if (OMCI_TREATMENT_REMOVE_ONE_TAG == outtreat->trearTag2Remove)
    {
        if ((OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
         && (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri))
        {
            sglTag->fwdType = CA_OMCI_SGL_TAG_FWD_REMOVE_TAG;
            OMCIALDBG("Remove single tag vid [%d], pri [%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
            return;
        }

        //only add inner tag, so it modify inner tag
        if (OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
        {
            if ((OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
             && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri))
            {
                sglTag->fwdType = CA_OMCI_SGL_TAG_FWD_TRANSPARENT;
                OMCIALDBG("Transparent single tag vid [%d], pri [%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
            {
                sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP;
                sglTag->innerVlanTreatment.pri = 0;
                sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid) ? infilter->filterInnVid : intreat->treatInnVid;
                OMCIALDBG("Modify single tag vid [%d], pri from dscp", sglTag->innerVlanTreatment.vid);
                return;
            }

            sglTag->innerVlanTreatment.pri = (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri) ? infilter->filterInnPri : intreat->treatInnPri;
            sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid) ? infilter->filterInnVid: intreat->treatInnVid;
            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_TAG;
            OMCIALDBG("Modify single tag vid [%d], pri [%d]", sglTag->innerVlanTreatment.vid, sglTag->innerVlanTreatment.pri);
            return;
        }

        //only add outer tag, so it modify tag
        if (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri)
        {
            if ((OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
             && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri))
            {
                sglTag->fwdType = CA_OMCI_SGL_TAG_FWD_TRANSPARENT;
                OMCIALDBG("Transparent single tag vid [%d], pri [%d]", sglTag->vlanFilter.vid, sglTag->vlanFilter.pri);
                return;
            }

            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
            {
                sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP;
                sglTag->innerVlanTreatment.pri = 0;
                sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid : outtreat->treatOutVid;
                OMCIALDBG("Modify single tag vid [%d], pri from dscp", sglTag->innerVlanTreatment.vid);
                return;
            }

            sglTag->innerVlanTreatment.pri = (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri) ? infilter->filterInnPri : outtreat->treatOutPri;
            sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid: outtreat->treatOutVid;
            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_TAG;
            OMCIALDBG("Modify single tag vid [%d], pri [%d]", sglTag->innerVlanTreatment.vid, sglTag->innerVlanTreatment.pri);
            return;
        }

        //add double tags, remove ori tag
        if ((OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
         && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri))
        {
            //no change the inner tag, just add an outer tag
            //same as remove 0 tag and add outer tag rules
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
            {
                sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP;
                sglTag->outerVlanTreatment.pri = 0;
                sglTag->outerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid : outtreat->treatOutVid;
                OMCIALDBG("Add outer tag vid [%d], pri [%d]", sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri);
                return;
            }

            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_ADD_TAG;
            sglTag->outerVlanTreatment.pri = outtreat->treatOutPri;
            sglTag->outerVlanTreatment.vid = outtreat->treatOutVid;
            if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
            {
                sglTag->outerVlanTreatment.pri = infilter->filterInnPri;
            }

            if (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
            {
                sglTag->outerVlanTreatment.vid = infilter->filterInnVid;
            }

            OMCIALDBG("Add outer tag vid [%d], add pri [%d]", sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri);
            return;
        }

        //modify inner tag and add outer tag
        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
        {
            sglTag->innerVlanTreatment.pri = 0;
            sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid) ? infilter->filterInnVid : intreat->treatInnVid;
            sglTag->outerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid : outtreat->treatOutVid;
            if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
            {
                sglTag->outerVlanTreatment.pri = 0;
                sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_VID_DSCP;
                OMCIALDBG("Add ovid [%d], Modify ivid [%d] both dscp pri", sglTag->outerVlanTreatment.vid, sglTag->innerVlanTreatment.vid);
                return;
            }
            sglTag->outerVlanTreatment.pri = (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri) ? infilter->filterInnPri : outtreat->treatOutPri;
            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_TAG;
            OMCIALDBG("Add tag vid[%d] pri[%d], Modify ivid [%d] idscp",
                       sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri,
                       sglTag->innerVlanTreatment.vid);
            return;
        }

        sglTag->innerVlanTreatment.pri = intreat->treatInnPri;
        sglTag->innerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid) ? infilter->filterInnVid : intreat->treatInnVid;
        sglTag->outerVlanTreatment.vid = (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid) ? infilter->filterInnVid : outtreat->treatOutVid;
        if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
        {
            sglTag->outerVlanTreatment.pri = 0;
            sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_VID_DSCP;
            OMCIALDBG("Add tag vid[%d] dscp, Modify ivid [%d] pri[%d]",
                       sglTag->outerVlanTreatment.vid,
                       sglTag->innerVlanTreatment.vid, sglTag->innerVlanTreatment.pri);
            return;
        }

        sglTag->outerVlanTreatment.pri = (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri) ? infilter->filterInnPri : outtreat->treatOutPri;
        sglTag->fwdType                = CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_TAG;
        OMCIALDBG("Add tag vid[%d] pri[%d], Modify ivid [%d] pri[%d]",
                   sglTag->outerVlanTreatment.vid, sglTag->outerVlanTreatment.pri,
                   sglTag->innerVlanTreatment.vid, sglTag->innerVlanTreatment.pri);
        return;
    }
}


/* untag rule analysis */
void ca_omci_stream_tbl_untag_set(ca_omci_untag_filter_t *untag, ca_omci_rx_vlan_tbl_t *rule)
{
    ca_omci_treat_outter_t  *outtreat  = NULL;
    ca_omci_treat_inner_t   *intreat   = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(untag);
    OMCI_PARAM_IS_NULL(rule);
    outtreat  = (ca_omci_treat_outter_t *)(&(rule->treatOutter));
    intreat   = (ca_omci_treat_inner_t *)(&(rule->treatInner));

    if (CA_OMCI_TRUE == untag->isValid)
    {
        OMCIALERR("ERROR: conflict untag rule, already add a untag rule before!");
        return;
    }

    OMCIALDBG("untag rule Don't care filter value!");

    //make sure untag rule is right
    if ((OMCI_TREATMENT_REMOVE_ONE_TAG == outtreat->trearTag2Remove)
     && (OMCI_TREATMENT_REMOVE_TWO_TAGS == outtreat->trearTag2Remove))
    {
        OMCIALERR("ERROR: untag rule should be transparent or drop!");
        return;
    }

    if (OMCI_TREATMENT_NOT_ADD_TAG != outtreat->treatOutPri)
    {
        //check the valid value for out vid and pbit
        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == outtreat->treatOutPri)
         || (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == outtreat->treatOutPri)
         || (OMCI_TREATMENT_COPY_VID_INNER == outtreat->treatOutVid)
         || (OMCI_TREATMENT_COPY_VID_OUTER == outtreat->treatOutVid))
        {
            OMCIALERR("ERROR: untag rule can't copy outer pri or vid for any treatment!");
            return;
        }
    }

    if (OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri)
    {
        //check the valid value for inner vid and pbit
        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == intreat->treatInnPri)
         || (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == intreat->treatInnPri)
         || (OMCI_TREATMENT_COPY_VID_INNER == intreat->treatInnVid)
         || (OMCI_TREATMENT_COPY_VID_OUTER == intreat->treatInnVid))
        {
            OMCIALERR("ERROR: untag rule can't copy inner pri or vid for any treatment!");
            return;
        }
    }

    untag->isValid         = CA_OMCI_TRUE;
    untag->etherTypeFilter = CA_OMCI_FILTER_NONE;

    // no need to analysis other values, untag packet drop
    if (OMCI_TREATMENT_DISCARD == outtreat->trearTag2Remove)
    {
        untag->fwdType = CA_OMCI_UNTAG_FWD_DROP;
        OMCIALDBG("Drop all untag packets");
        return;
    }

    if ((OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
     && (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri))
    {
        untag->fwdType = CA_OMCI_UNTAG_FWD_TRANSPARENT;
        OMCIALDBG("Transparent all untag packets");
        return;
    }

    if ((OMCI_TREATMENT_NOT_ADD_TAG != outtreat->treatOutPri)
     && (OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri))
    {
        OMCIALDBG("Add double tags for untag packets now");
        untag->innerVlanTreatment.vid = intreat->treatInnVid;
        untag->innerVlanTreatment.pri = intreat->treatInnPri;
        untag->outerVlanTreatment.vid = outtreat->treatOutVid;
        untag->outerVlanTreatment.pri = outtreat->treatOutPri;

        if ((OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
         && (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri))
        {
            untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_DSCP_PRI;
            untag->innerVlanTreatment.pri = 0;
            untag->outerVlanTreatment.pri = 0;
            OMCIALDBG("Add inner vid [%d] outer vid [%d] for untag packets, pri all base on dscp",
                      untag->innerVlanTreatment.vid, untag->outerVlanTreatment.vid);
        }
        else if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
        {
            untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_OUT_DSCP_PRI;
            untag->outerVlanTreatment.pri = 0;
            OMCIALDBG("Add inner vid [%d] pri [%d] outer vid [%d] for untag packets, inner pri base on dscp",
                      untag->innerVlanTreatment.vid, untag->innerVlanTreatment.pri, untag->outerVlanTreatment.vid);
        }
        else if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
        {
            untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_INN_DSCP_PRI;
            untag->innerVlanTreatment.pri = 0;
            OMCIALDBG("Add inner vid [%d] outer vid [%d] pri [%d] for untag packets, outer pri base on dscp",
                      untag->innerVlanTreatment.vid, untag->outerVlanTreatment.vid, untag->outerVlanTreatment.pri);
        }
        else
        {
            untag->fwdType = CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_PRI;
            OMCIALDBG("Add inner vid [%d] pri [%d] outer vid [%d] pri [%d] for untag packets",
                      untag->innerVlanTreatment.vid, untag->innerVlanTreatment.pri,
                      untag->outerVlanTreatment.vid, untag->outerVlanTreatment.pri);
        }
        return;
    }

    // there is no inner or outer for add one tag
    if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI > outtreat->treatOutPri)
    {
        untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_VID_PRI;
        untag->innerVlanTreatment.vid = outtreat->treatOutVid;
        untag->innerVlanTreatment.pri = outtreat->treatOutPri;
        OMCIALDBG("Add out tag vid [%d] pri [%d]", untag->innerVlanTreatment.vid, untag->innerVlanTreatment.pri);
        return;
    }

    if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI > intreat->treatInnPri)
    {
        untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_VID_PRI;
        untag->innerVlanTreatment.vid = intreat->treatInnVid;
        untag->innerVlanTreatment.pri = intreat->treatInnPri;
        OMCIALDBG("Add inn tag vid [%d] pri [%d]", untag->innerVlanTreatment.vid, untag->innerVlanTreatment.pri);
        return;
    }

    if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == outtreat->treatOutPri)
    {
        untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI;
        untag->innerVlanTreatment.vid = outtreat->treatOutVid;
        OMCIALDBG("Add out tag vid [%d] pri from dscp", untag->innerVlanTreatment.vid);
        return;
    }

    if (OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI == intreat->treatInnPri)
    {
        untag->fwdType                = CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI;
        untag->innerVlanTreatment.vid = intreat->treatInnVid;
        OMCIALDBG("Add inner tag vid [%d] pri from dscp", untag->innerVlanTreatment.vid);
        return;
    }
}


void ca_omci_ds_ivs_stream_tbl_merge(ca_omci_stream_cfg_t *stream, ca_omci_ext_vlan_tag_op_t *extVlan)
{
    ca_omci_stream_cfg_t        *streamDs   = NULL;
    ca_omci_gem_flow_t          *flowCfg    = NULL;
    ca_omci_filter_stream_cfg_t *filter     = NULL;
    ca_omci_filter_inner_t      *infilter   = NULL;
    ca_omci_filter_outter_t     *outfilter  = NULL;
    ca_omci_treat_outter_t      *outtreat   = NULL;
    ca_omci_treat_inner_t       *intreat    = NULL;
    ca_omci_uint32_t             idx        = 0;
    ca_omci_uint32_t             idxVid     = 0;
    ca_omci_uint32_t             sglNum     = 0;
    ca_omci_uint32_t             dblNum     = 0;
    ca_omci_bool_t               dsFlowFlag = CA_OMCI_FALSE;
    ca_omci_bool_t               filterFlag = CA_OMCI_FALSE;
    ca_omci_uint16_t             mcastGemId = 0;
    ca_omci_uint16_t             vidList[OMCI_VLAN_LIST_NUM];
    struct avl_traverser         avlTrav;
    struct avl_traverser         avlTravFilter;

    OMCI_PARAM_IS_NULL(stream);
    OMCI_PARAM_IS_NULL(extVlan);

    streamDs = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.dsFlow, stream);
    if (NULL != streamDs)
    {
        OMCIALERR("add same ds stream now ");
        return;
    }

    //get downstream only gemport
    //don't set mcast ds gemport
    ca_omci_ds_mcast_gemid_get(&mcastGemId);

    if (NULL != sg_omciAnalyseFlowTbl.flow.gemFlow)
    {
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.gemFlow);
        while (NULL != flowCfg)
        {
            if ((flowCfg->gemId == stream->gemId)
             && (GEM_NET_CTP_DIR_DS == flowCfg->direction)
             && (flowCfg->gemId != mcastGemId))
            {
                dsFlowFlag = CA_OMCI_TRUE;

                //also check the vlan filter table, don't care the priority
                if (0 != flowCfg->vidList[0])
                {
                    filterFlag = CA_OMCI_TRUE;
                    OMCI_MEMSET(vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                    OMCI_MEMCPY(vidList, flowCfg->vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                }
                break;
            }
            flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
        }
    }

    if (CA_OMCI_TRUE != dsFlowFlag)
    {
        return;
    }

    //Handle the downstream stream table;
    streamDs = omci_avl_insert(sg_omciAnalyseFlowTbl.flow.dsFlow, stream);
    if (NULL == streamDs)
    {
        OMCIALERR("add Ds stream failed");
        return;
    }

    streamDs->filterFlag = filterFlag;
    streamDs->inputTpid  = extVlan->attr.inputTpid;
    streamDs->outputTpid = extVlan->attr.outputTpid;

    if (NULL != sg_omciAnalyseFlowTbl.flow.filterFlow)
    {
        filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTravFilter, sg_omciAnalyseFlowTbl.flow.filterFlow);
        while (NULL != filter)
        {
            if (filter->gemId == stream->gemId)
            {
                filter->externVlanFlag = CA_OMCI_TRUE;
                break;
            }
            filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTravFilter);
        }
    }

    if (CA_OMCI_TRUE == filterFlag)
    {
        for (idxVid = 0; idxVid < OMCI_VLAN_LIST_NUM; idxVid++)
        {
            if (0 != vidList[idxVid])
            {
                for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
                {
                    OMCI_CHECK_PARAM_CNU(CA_OMCI_FALSE == extVlan->attr.rxVlanTbl[idx].valid);

                    outfilter = (ca_omci_filter_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].filterOutter));
                    infilter  = (ca_omci_filter_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].filterInner));
                    outtreat  = (ca_omci_treat_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].treatOutter));
                    intreat   = (ca_omci_treat_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].treatInner));

                    //for downstream flow, only treat the vlan match one
                    if ((vidList[idxVid] == outtreat->treatOutVid)
                     || (vidList[idxVid] == intreat->treatInnVid))
                    {
                        //untag rule
                        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
                         && (OMCI_FILTER_PRI_IGNORE == infilter->filterInnPri))
                        {
                            if (OMCI_FILTER_ETH_TYPE_NONE == infilter->filterEthType)
                            {
                                OMCIALDBG("Default untag rule");
                                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            if (OMCI_FILTER_ETH_TYPE_IPOE == infilter->filterEthType)
                            {
                                OMCIALDBG("IPoE untag rule");
                                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterIpoe, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            if (OMCI_FILTER_ETH_TYPE_IPOE_V6 == infilter->filterEthType)
                            {
                                OMCIALDBG("IPoEv6 untag rule");
                                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterIpoev6, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            if (OMCI_FILTER_ETH_TYPE_PPPOE == infilter->filterEthType)
                            {
                                OMCIALDBG("PPPoE untag rule");
                                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterPppoe, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            if (OMCI_FILTER_ETH_TYPE_ARP == infilter->filterEthType)
                            {
                                OMCIALDBG("ARP untag rule");
                                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterArp, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }
                        }

                        //single tag rule
                        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
                         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
                        {
                            if (OMCI_FILTER_PRI_DEF == infilter->filterInnPri)
                            {
                                OMCIALDBG("Default single tag rule");
                                ca_omci_stream_tbl_single_tag_set(&streamDs->sglTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            OMCIALDBG("Single tag rule");
                            ca_omci_stream_tbl_single_tag_set(&streamDs->sglTagFilter[sglNum], &extVlan->attr.rxVlanTbl[idx]);
                            sglNum++;
                            continue;
                        }

                        if ((OMCI_FILTER_PRI_IGNORE != outfilter->filterOutPri)
                         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
                        {
                            if (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri)
                            {
                                OMCIALDBG("Default double tag rule");
                                ca_omci_stream_tbl_double_tag_set(&streamDs->dblTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                                continue;
                            }

                            OMCIALDBG("double tag rule");
                            ca_omci_stream_tbl_double_tag_set(&streamDs->dblTagFilter[dblNum], &extVlan->attr.rxVlanTbl[idx]);
                            dblNum++;
                            continue;
                        }
                    }
                }
            }
        }
        return;
    }

    //no filter vlan
    for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
    {
        OMCI_CHECK_PARAM_CNU(CA_OMCI_FALSE == extVlan->attr.rxVlanTbl[idx].valid);

        outfilter = (ca_omci_filter_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].filterOutter));
        infilter  = (ca_omci_filter_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].filterInner));
        outtreat  = (ca_omci_treat_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].treatOutter));
        intreat   = (ca_omci_treat_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].treatInner));

        //untag rule
        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE == infilter->filterInnPri))
        {
            if (OMCI_FILTER_ETH_TYPE_NONE == infilter->filterEthType)
            {
                OMCIALDBG("Default untag rule");
                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_IPOE == infilter->filterEthType)
            {
                OMCIALDBG("IPoE untag rule");
                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterIpoe, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_IPOE_V6 == infilter->filterEthType)
            {
                OMCIALDBG("IPoEv6 untag rule");
                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterIpoev6, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_PPPOE == infilter->filterEthType)
            {
                OMCIALDBG("PPPoE untag rule");
                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterPppoe, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_ARP == infilter->filterEthType)
            {
                OMCIALDBG("ARP untag rule");
                ca_omci_stream_tbl_untag_set(&streamDs->untagFilterArp, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }
        }

        //single tag rule
        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
        {
            if (OMCI_FILTER_PRI_DEF == infilter->filterInnPri)
            {
                OMCIALDBG("Default single tag rule");
                ca_omci_stream_tbl_single_tag_set(&streamDs->sglTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            OMCIALDBG("Single tag rule");
            ca_omci_stream_tbl_single_tag_set(&streamDs->sglTagFilter[sglNum], &extVlan->attr.rxVlanTbl[idx]);
            sglNum++;
            continue;
        }

        if ((OMCI_FILTER_PRI_IGNORE != outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
        {
            if (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri)
            {
                OMCIALDBG("Default double tag rule");
                ca_omci_stream_tbl_double_tag_set(&streamDs->dblTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            OMCIALDBG("double tag rule");
            ca_omci_stream_tbl_double_tag_set(&streamDs->dblTagFilter[dblNum], &extVlan->attr.rxVlanTbl[idx]);
            dblNum++;
            continue;
        }
    }
}


//save for future use, for down stream is totally new, not relate to ds mode
void ca_omci_ds_stream_tbl_merge(ca_omci_stream_cfg_t *stream, ca_omci_ext_vlan_tag_op_t *extVlan)
{
    OMCI_PARAM_IS_NULL(stream);
    OMCI_PARAM_IS_NULL(extVlan);

    if (EXT_VLAN_TAG_OP_DS_MODE_INVERSE == extVlan->attr.dsMode)
    {
        ca_omci_ds_ivs_stream_tbl_merge(stream, extVlan);
        return;
    }

    //save for funture use; maybe other ds mode
}


void ca_omci_us_stream_tbl_merge(ca_omci_stream_cfg_t *stream, ca_omci_ext_vlan_tag_op_t *extVlan)
{
    ca_omci_stream_cfg_t        *streamUs   = NULL;
    ca_omci_filter_inner_t      *infilter   = NULL;
    ca_omci_filter_outter_t     *outfilter  = NULL;
    ca_omci_treat_outter_t      *outtreat   = NULL;
    ca_omci_treat_inner_t       *intreat    = NULL;
    ca_omci_gem_flow_t          *flowCfg    = NULL;
    ca_omci_filter_stream_cfg_t *filter     = NULL;
    ca_omci_onuinfo_obj_t       *pOnuInfo   = ca_omci_onuinfo_get();
    ca_omci_meclass_t           *meClass    = NULL;
    ca_omci_mebase_t            *meInst     = NULL;
    ca_omci_mac_port_cfg_t      *brgportcfg = NULL;
    ca_omci_ieee_dot1p_srv_t    *ieee8021p  = NULL;
    ca_omci_gem_iwtp_t          *gemiwtp    = NULL;
    ca_omci_gem_net_ctp_t       *gpnctp     = NULL;
    ca_omci_uint32_t             idx        = 0;
    ca_omci_uint32_t             idxVid     = 0;
    ca_omci_uint32_t             sglNum     = 0;
    ca_omci_uint32_t             dblNum     = 0;
    ca_omci_uint32_t             deactNum   = 0;
    ca_omci_bool_t               ignore     = CA_OMCI_FALSE;
    ca_omci_bool_t               filterFlag = CA_OMCI_FALSE;
    ca_omci_uint8_t              filterPri  = 0;
    ca_omci_bool_t               transUntag = CA_OMCI_FALSE;
    ca_omci_uint16_t             vidList[OMCI_VLAN_LIST_NUM];
    struct avl_traverser         avlTrav;
    struct avl_traverser         avlTravFilter;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(stream);
    OMCI_PARAM_IS_NULL(extVlan);

    //check if any rule configure now, if no rule configure, just return
    for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
    {
        if (CA_OMCI_FALSE == extVlan->attr.rxVlanTbl[idx].valid)
        {
            deactNum++;
        }
    }

    if (OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL == deactNum)
    {
        //check the TPID and update the value to filterFlow table
        if (NULL != sg_omciAnalyseFlowTbl.flow.filterFlow)
        {
            filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.filterFlow);
            while (NULL != filter)
            {
                if (filter->gemId == stream->gemId)
                {
                    filter->inputTpid  = extVlan->attr.inputTpid;
                    filter->outputTpid = extVlan->attr.outputTpid;
                    break;
                }
                filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            }
        }
        return;
    }

    streamUs = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.usFlow, stream);
    if (NULL != streamUs)
    {
        OMCIALERR("add same us stream now ");
        return;
    }

    //get vlan list then to configure in stream table
    if (NULL != sg_omciAnalyseFlowTbl.flow.gemFlow)
    {
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.gemFlow);
        while (NULL != flowCfg)
        {
            if (flowCfg->gemId == stream->gemId)
            {
                if ((0 != flowCfg->vidList[0]) || ((0 != flowCfg->gemPri) && (0xff != flowCfg->gemPri)))
                {
                    filterFlag = CA_OMCI_TRUE;
                    filterPri  = flowCfg->gemPri;
                    transUntag = flowCfg->transUntag;
                    OMCI_MEMSET(vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                    OMCI_MEMCPY(vidList, flowCfg->vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                }
                //merge the tcont and queue, when tcont and queue change ,stream need to change too!
                stream->tcontId       = flowCfg->tcontId;
                stream->usQueue       = flowCfg->usQueue;
                stream->dsQueue       = flowCfg->dsQueue;
                stream->dsQueueWeight = flowCfg->dsQueueWeight;
                break;
            }
            flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
        }
    }

    streamUs = omci_avl_insert(sg_omciAnalyseFlowTbl.flow.usFlow, stream);
    if (NULL == streamUs)
    {
        OMCIALERR("add us stream failed");
        return;
    }
    streamUs->filterFlag = filterFlag;
    streamUs->filterPri  = filterPri;
    streamUs->dsMode     = extVlan->attr.dsMode;
    streamUs->inputTpid  = extVlan->attr.inputTpid;
    streamUs->outputTpid = extVlan->attr.outputTpid;

    if (NULL != sg_omciAnalyseFlowTbl.flow.filterFlow)
    {
        filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTravFilter, sg_omciAnalyseFlowTbl.flow.filterFlow);
        while (NULL != filter)
        {
            if (filter->gemId == stream->gemId)
            {
                filter->externVlanFlag = CA_OMCI_TRUE;
                break;
            }
            filter = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTravFilter);
        }
    }

    for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
    {
        OMCI_CHECK_PARAM_CNU(CA_OMCI_FALSE == extVlan->attr.rxVlanTbl[idx].valid);

        outfilter = (ca_omci_filter_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].filterOutter));
        infilter  = (ca_omci_filter_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].filterInner));
        outtreat  = (ca_omci_treat_outter_t *)(&(extVlan->attr.rxVlanTbl[idx].treatOutter));
        intreat   = (ca_omci_treat_inner_t *)(&(extVlan->attr.rxVlanTbl[idx].treatInner));

        //get TPID action here, only support make it to a port base value
        streamUs->innerTpid.filter = (0 != infilter->filterInnTpid) ? infilter->filterInnTpid : streamUs->innerTpid.filter;
        streamUs->innerTpid.treat  = (0 != intreat->treatInnTpid) ? intreat->treatInnTpid : streamUs->innerTpid.treat;
        streamUs->outerTpid.filter = (0 != outfilter->filterOutTpid) ? outfilter->filterOutTpid : streamUs->outerTpid.filter;
        streamUs->outerTpid.treat  = (0 != outtreat->treatOutTpid) ? outtreat->treatOutTpid : streamUs->outerTpid.treat;

        /* For ALCL OLT, get the mapped gem for flow via outtreat pad bit */
        if ((OMCI_OLT_TYPE_ALCL == pOnuInfo->oltType) && (0 != outtreat->pad)) {
            ignore = CA_OMCI_TRUE;
            meClass = ca_omci_me_get(OMCI_ME_MAC_PORT_CFG_CLASS_ID);
            if ((meClass != NULL) && (meClass->instHash != NULL) && (0 != meClass->instHash->count)) {
                for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
                     meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst)) {
                    brgportcfg = (ca_omci_mac_port_cfg_t *)meInst;
                    if (outtreat->pad == brgportcfg->attr.portNum) {
                        if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == brgportcfg->attr.tpType) {
                            ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, brgportcfg->attr.tpPointer);
                            if (NULL != ieee8021p) {
                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri0);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri1);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri2);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri3);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri4);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri5);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri6);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }

                                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, ieee8021p->attr.iwtpPri7);
                                if (NULL != gemiwtp) {
                                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                                    if (NULL != gpnctp) {
                                        if (stream->gemId == gpnctp->attr.portId) {
                                            ignore = CA_OMCI_FALSE;
                                            OMCIALDBG("Map to gem %d ", stream->gemId);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            OMCI_CHECK_PARAM_CNU(CA_OMCI_TRUE == ignore);
        }

        //check the filter vlan and the extern vlan match
        if (CA_OMCI_TRUE == filterFlag)
        {
            ignore = CA_OMCI_TRUE;
            for (idxVid = 0; idxVid < OMCI_VLAN_LIST_NUM; idxVid++)
            {
                if (0 != vidList[idxVid])
                {
                    if ((OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
                     && (OMCI_TREATMENT_NOT_ADD_TAG == intreat->treatInnPri))
                    {
                        //compare to the filter value
                        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
                         && (OMCI_FILTER_PRI_IGNORE == infilter->filterInnPri))
                        {
                            ignore = (CA_OMCI_TRUE == transUntag) ? CA_OMCI_FALSE : CA_OMCI_TRUE;
                            OMCIALDBG("Untag rule trans for gem %d %s", stream->gemId, (CA_OMCI_TRUE == ignore) ? "ignore" : "pass");
                            break;
                        }

                        if (OMCI_FILTER_PRI_IGNORE != outfilter->filterOutPri)
                        {
                            if (OMCI_FILTER_VID_NONE == outfilter->filterOutVid)
                            {
                                //double tag rule don't care vid
                                OMCIALDBG("Don't care vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            else if (vidList[idxVid] == outfilter->filterOutVid)
                            {
                                OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            break;
                        }

                        if (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri)
                        {
                            if (OMCI_FILTER_VID_NONE == infilter->filterInnVid)
                            {
                                //single tag rule don't care vid
                                OMCIALDBG("Don't care vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            else if (vidList[idxVid] == infilter->filterInnVid)
                            {
                                OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            break;
                        }

                    }

                    if ((OMCI_TREATMENT_NOT_ADD_TAG != outtreat->treatOutPri)
                     && (OMCI_TREATMENT_COPY_VID_INNER != outtreat->treatOutVid)
                     && (OMCI_TREATMENT_COPY_VID_OUTER != outtreat->treatOutVid)
                     && (vidList[idxVid] == outtreat->treatOutVid))
                    {
                        //when VID match, check pbit
                        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI > outtreat->treatOutPri)
                          && (0 != streamUs->filterPri)
                          && (0xff != streamUs->filterPri))
                        {
                            //check the pbit map the filter PRI
                            if (0 != ((1 << outtreat->treatOutPri) & streamUs->filterPri))
                            {
                                OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            break;
                        }

                        OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                        ignore = CA_OMCI_FALSE;
                        break;
                    }

                    if ((OMCI_TREATMENT_NOT_ADD_TAG != intreat->treatInnPri)
                     && (OMCI_TREATMENT_COPY_VID_INNER != intreat->treatInnVid)
                     && (OMCI_TREATMENT_COPY_VID_OUTER != intreat->treatInnVid)
                     && (vidList[idxVid] == intreat->treatInnVid))
                    {
                        //when VID match, check pbit
                        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI > intreat->treatInnPri)
                          && (0 != streamUs->filterPri)
                          && (0xff != streamUs->filterPri))
                        {
                            //check the pbit map the filter PRI
                            if (0 != ((1 << intreat->treatInnPri) & streamUs->filterPri))
                            {
                                OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                                ignore = CA_OMCI_FALSE;
                            }
                            break;
                        }
                        OMCIALDBG("Find a match vid for gem %d", stream->gemId);
                        ignore = CA_OMCI_FALSE;
                        break;
                    }
                }
            }

            if ((OMCI_VLAN_LIST_NUM == idxVid)
             && (0 == vidList[0])
             && (0 != streamUs->filterPri)
             && (0xff != streamUs->filterPri))
            {
                OMCIALDBG("No vid filter gem %d, check pbit", stream->gemId);
                ignore = CA_OMCI_FALSE;

                if ((8 > outtreat->treatOutPri) && (0 == ((1 << outtreat->treatOutPri) & streamUs->filterPri)))
                {
                    OMCIALDBG("Don't match dot1p for gem %d", stream->gemId);
                    ignore = CA_OMCI_TRUE;
                }
                else
                {
                    if (OMCI_TREATMENT_NOT_ADD_TAG == outtreat->treatOutPri)
                    {
                        if ((8 > intreat->treatInnPri) && (0 == ((1 << intreat->treatInnPri) & streamUs->filterPri)))
                        {
                            OMCIALDBG("Don't match dot1p for gem %d", stream->gemId);
                            ignore = CA_OMCI_TRUE;
                        }
                    }
                }
            }
        }

        OMCI_CHECK_PARAM_CNU(CA_OMCI_TRUE == ignore);

        //untag rule
        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE == infilter->filterInnPri))
        {
            if (OMCI_FILTER_ETH_TYPE_NONE == infilter->filterEthType)
            {
                OMCIALDBG("Default untag rule");
                ca_omci_stream_tbl_untag_set(&streamUs->untagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_IPOE == infilter->filterEthType)
            {
                OMCIALDBG("IPoE untag rule");
                ca_omci_stream_tbl_untag_set(&streamUs->untagFilterIpoe, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_IPOE_V6 == infilter->filterEthType)
            {
                OMCIALDBG("IPoEv6 untag rule");
                ca_omci_stream_tbl_untag_set(&streamUs->untagFilterIpoev6, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_PPPOE == infilter->filterEthType)
            {
                OMCIALDBG("PPPoE untag rule");
                ca_omci_stream_tbl_untag_set(&streamUs->untagFilterPppoe, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            if (OMCI_FILTER_ETH_TYPE_ARP == infilter->filterEthType)
            {
                OMCIALDBG("ARP untag rule");
                ca_omci_stream_tbl_untag_set(&streamUs->untagFilterArp, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }
        }

        //single tag rule
        if ((OMCI_FILTER_PRI_IGNORE == outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
        {
            if (OMCI_FILTER_PRI_DEF == infilter->filterInnPri)
            {
                OMCIALDBG("Default single tag rule");
                ca_omci_stream_tbl_single_tag_set(&streamUs->sglTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            OMCIALDBG("Single tag rule");
            ca_omci_stream_tbl_single_tag_set(&streamUs->sglTagFilter[sglNum], &extVlan->attr.rxVlanTbl[idx]);
            sglNum++;
            continue;
        }

        if ((OMCI_FILTER_PRI_IGNORE != outfilter->filterOutPri)
         && (OMCI_FILTER_PRI_IGNORE != infilter->filterInnPri))
        {
            if (OMCI_FILTER_PRI_DEF == outfilter->filterOutPri)
            {
                OMCIALDBG("Default double tag rule");
                ca_omci_stream_tbl_double_tag_set(&streamUs->dblTagFilterDef, &extVlan->attr.rxVlanTbl[idx]);
                continue;
            }

            OMCIALDBG("double tag rule");
            ca_omci_stream_tbl_double_tag_set(&streamUs->dblTagFilter[dblNum], &extVlan->attr.rxVlanTbl[idx]);
            dblNum++;
            continue;
        }
    }
}


void ca_omci_filter_stream_tbl_add(ca_omci_filter_stream_cfg_t *stream)
{
    ca_omci_filter_stream_cfg_t *filterStream = NULL;
    ca_omci_gem_flow_t          *flowCfg      = NULL;
    struct avl_traverser         avlTrav;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(stream);

    filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.filterFlow, stream);
    if (NULL != filterStream)
    {
        OMCIALERR("add same filter stream now ");
        return;
    }

    //get vlan list then to configure in stream table
    if (NULL != sg_omciAnalyseFlowTbl.flow.gemFlow)
    {
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.gemFlow);
        while (NULL != flowCfg)
        {
            if (flowCfg->gemId == stream->gemId)
            {
                stream->priBit = flowCfg->gemPri;
                if (0 != flowCfg->vidList[0])
                {
                    OMCI_MEMCPY(stream->vidList, flowCfg->vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                }
                stream->usQueueDpcm   = flowCfg->usQueueDpcm;
                stream->dsQueue       = flowCfg->dsQueue;
                stream->dsQueueWeight = flowCfg->dsQueueWeight;
                stream->dsQueueDpcm   = flowCfg->dsQueueDpcm;
                break;
            }
            flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
        }
    }

    filterStream = omci_avl_insert(sg_omciAnalyseFlowTbl.flow.filterFlow, stream);
    if (NULL == filterStream)
    {
        OMCIALERR("add filter stream failed");
        return;
    }
}
#endif


#if DESCRIPTION("Gem flow functions")
static ca_omci_bool_t ca_omci_analysis_flow_mcast(ca_omci_mebase_t *meInst)
{
    ca_omci_gem_net_ctp_t      *gpnctp     = (ca_omci_gem_net_ctp_t *)meInst;
    ca_omci_me_link_t          *link       = meInst->linkedMeHeader;
    ca_omci_me_link_t          *linkbrgp   = NULL;
    ca_omci_multi_gem_iwtp_t   *multiGem   = NULL;
    ca_omci_mac_port_cfg_t     *brgportcfg = NULL;
    ca_omci_mac_brg_srv_t      *macbrg     = NULL;
    ca_omci_ieee_dot1p_srv_t   *ieee8021p  = NULL;
    ca_omci_traffic_descript_t *dsTrafDesc = NULL;
    ca_omci_pptp_eth_t         *pptpEth    = NULL;
    ca_omci_ip_host_t          *iphost     = NULL;
    ca_omci_veip_t             *veip       = NULL;
    ca_omci_bool_t              uniFlag    = CA_OMCI_FALSE;
    ca_omci_gem_flow_t          gemFlow    = {0};

    multiGem   = (ca_omci_multi_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
    dsTrafDesc = (ca_omci_traffic_descript_t *)(ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gpnctp->attr.dsTrafMngPoint));

    /* If gemport iwtp is not found nor linked, the flow is not complete */
    OMCI_CHECK_PARAM_RET((NULL == multiGem), CA_OMCI_FALSE);
    OMCI_CHECK_PARAM_RET((0 == multiGem->meBase.linkedCount), CA_OMCI_FALSE);

    // bridge mode for multicast
    if ((MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGem->attr.iwOption)
     || (MULTI_GEM_IWTP_IW_OPTION_DONT_CARE == multiGem->attr.iwOption))
    {
        link = multiGem->meBase.linkedMeHeader;
        OMCI_CHECK_PARAM_RET((NULL == link), CA_OMCI_FALSE);
        uniFlag = CA_OMCI_FALSE;

        while (link)
        {
            /* find the mac port cfg data me only, it is the ANI side mac port */
            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            link       = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

            macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
            OMCI_CHECK_PARAM_CNU(NULL == macbrg);
            OMCI_CHECK_PARAM_CNU(0 == macbrg->meBase.linkedCount);

            linkbrgp = macbrg->meBase.linkedMeHeader;
            while (linkbrgp)
            {
                /* find the mac port cfg data me only */
                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                {
                    linkbrgp = linkbrgp->next;
                    continue;
                }

                /* Get the bridge mode UNI side */
                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                linkbrgp   = linkbrgp->next;
                OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                 && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                 && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                {
                    continue;
                }

                if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                {
                    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
                    uniFlag = CA_OMCI_TRUE;
                }
                if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                {
                    iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == iphost);
                    uniFlag = CA_OMCI_TRUE;
                }
                if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                {
                    veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == veip);
                    uniFlag = CA_OMCI_TRUE;
                }
            }
        }

        if (CA_OMCI_FALSE == uniFlag)
        {
            OMCIALERR("multicast Flow is not complete, no uni side");
            return CA_OMCI_FALSE;
        }

        OMCI_MEMSET(&gemFlow, 0,  sizeof(ca_omci_gem_flow_t));
        gemFlow.gemId      = gpnctp->attr.portId;
        gemFlow.direction  = gpnctp->attr.direction;
        gemFlow.dsQueue    = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
        gemFlow.aesState   = gpnctp->attr.encryptState;
        gemFlow.aesKeyRing = gpnctp->attr.encryptKeyRing;
        gemFlow.mcFlag     = CA_OMCI_TRUE;

        if (NULL != dsTrafDesc)
        {
            //gemFlow.dsTrafficDes.mode = dsTrafDesc->attr.;
            //gemFlow.dsTrafficDes.pps    = dsTrafDesc->attr.;
            gemFlow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
            gemFlow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
            gemFlow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
            gemFlow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
        }
        ca_omci_gemflow_tbl_add(&gemFlow);
        return CA_OMCI_TRUE;
    }

    if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGem->attr.iwOption)
    {
        link = multiGem->meBase.linkedMeHeader;
        OMCI_CHECK_PARAM_RET((NULL == link), CA_OMCI_FALSE);
        uniFlag = CA_OMCI_FALSE;

        while (link)
        {
            /* find the 802.1p me only */
            if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            link      = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

            /* for mutilcast downstream, 802.1p direct link to uni */
            if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
             && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
             && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
            {
                continue;
            }

            if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
            {
                pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieee8021p->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
                uniFlag = CA_OMCI_TRUE;
            }
            if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
            {
                iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, ieee8021p->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == iphost);
                uniFlag = CA_OMCI_TRUE;
            }
            if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
            {
                veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieee8021p->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == veip);
                uniFlag = CA_OMCI_TRUE;
            }
        }

        if (CA_OMCI_FALSE == uniFlag)
        {
            OMCIALERR("multicast Flow is not complete, no uni side");
            return CA_OMCI_FALSE;
        }

        OMCI_MEMSET(&gemFlow, 0,  sizeof(ca_omci_gem_flow_t));
        gemFlow.gemId      = gpnctp->attr.portId;
        gemFlow.direction  = gpnctp->attr.direction;
        gemFlow.dsQueue    = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
        gemFlow.aesState   = gpnctp->attr.encryptState;
        gemFlow.aesKeyRing = gpnctp->attr.encryptKeyRing;
        gemFlow.mcFlag     = CA_OMCI_TRUE;

        if (NULL != dsTrafDesc)
        {
            //gemFlow.dsTrafficDes.mode = dsTrafDesc->attr.;
            //gemFlow.dsTrafficDes.pps    = dsTrafDesc->attr.;
            gemFlow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
            gemFlow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
            gemFlow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
            gemFlow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
        }
        ca_omci_gemflow_tbl_add(&gemFlow);
        return CA_OMCI_TRUE;
    }

    return CA_OMCI_FALSE;
}

static ca_omci_bool_t ca_omci_analysis_flow_unicast(ca_omci_mebase_t *meInst)
{
    ca_omci_gem_net_ctp_t      *gpnctp     = (ca_omci_gem_net_ctp_t *)meInst;
    ca_omci_me_link_t          *link       = meInst->linkedMeHeader;
    ca_omci_me_link_t          *linkbrgp   = NULL;
    ca_omci_tcont_t            *tcont      = NULL;
    ca_omci_gem_iwtp_t         *gemiwtp    = NULL;
    ca_omci_mac_brg_srv_t      *macbrg     = NULL;
    ca_omci_mac_port_cfg_t     *brgportcfg = NULL;
    ca_omci_vlan_tag_filter_t  *vlanfilter = NULL;
    ca_omci_traffic_descript_t *usTrafDesc = NULL;
    ca_omci_traffic_descript_t *dsTrafDesc = NULL;
    ca_omci_ieee_dot1p_srv_t   *ieee8021p  = NULL;
    ca_omci_pri_queue_t        *usPriQ     = NULL;
    ca_omci_pri_queue_t        *dsPriQ     = NULL;
    ca_omci_pptp_eth_t         *pptpEth    = NULL;
    ca_omci_ip_host_t          *iphost     = NULL;
    ca_omci_veip_t             *veip       = NULL;
    ca_omci_traffic_scheg_t    *trafSche   = NULL;
    ca_omci_gem_flow_t          gemflow    = {0};
    ca_omci_uint8_t             pri        = 0;
    ca_omci_uint8_t             idx        = 0;
    ca_omci_bool_t              uniFlag    = CA_OMCI_FALSE;
    ca_omci_uint16_t            vidList[OMCI_VLAN_LIST_NUM];

    OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);

    tcont      = (ca_omci_tcont_t *)(ca_omci_me_entry_lookup(OMCI_ME_TCONT_CLASS_ID, gpnctp->attr.tcontPoint));
    gemiwtp    = (ca_omci_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
    usTrafDesc = (ca_omci_traffic_descript_t *)(ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gpnctp->attr.usTrafDescPoint));
    dsTrafDesc = (ca_omci_traffic_descript_t *)(ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID, gpnctp->attr.dsTrafMngPoint));
    usPriQ     = (ca_omci_pri_queue_t *)(ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gpnctp->attr.usTrafMngPoint));
    dsPriQ     = (ca_omci_pri_queue_t *)(ca_omci_me_entry_lookup(OMCI_ME_PRI_QUEUE_CLASS_ID, gpnctp->attr.dsPqPoint));

    /* if can't find gemport interwork tp or this me isn't linked, the flow is not complete */
    OMCI_CHECK_PARAM_RET((NULL == gemiwtp), CA_OMCI_FALSE);
    OMCI_CHECK_PARAM_RET((0 == gemiwtp->meBase.linkedCount), CA_OMCI_FALSE);

    /* if can't find the tcont , the flow is not complete*/
    if (GEM_IWTP_IW_OPTION_DS_BROADCAST != gemiwtp->attr.iwOption)
    {
        OMCI_CHECK_PARAM_RET((NULL == tcont), CA_OMCI_FALSE);
        OMCI_CHECK_PARAM_RET(((0xffff == tcont->attr.allocId) || (0 == tcont->attr.allocId) || (0Xff == tcont->attr.allocId)), CA_OMCI_FALSE);
    }

    link = gemiwtp->meBase.linkedMeHeader;
    while (link)
    {
        /* find the mac port cfg data me for gem and find the vlan tag filter for ani-side */
        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
        {
            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            if (NULL != brgportcfg)
            {
                vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    //normally only one side will be configured.
                    OMCIALDBG("Have vlan filter for ani side");
                    OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                    }
                }
            }
        }
        link = link->next;
    }

    /* flow is N:1 or 1:P or M:P mode*/
    if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemiwtp->attr.iwOption)
    {
        macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));
        /* If mac bridge srv is null, the flow is not complete */
        OMCI_CHECK_PARAM_RET((NULL == macbrg), CA_OMCI_FALSE);
        OMCI_CHECK_PARAM_RET((0 == macbrg->meBase.linkedCount), CA_OMCI_FALSE);

        link    = macbrg->meBase.linkedMeHeader;
        uniFlag = CA_OMCI_FALSE;
        while (link)
        {
            /* find the mac port cfg data me for gemflow analysis and extern vlan for stream analysis */
            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            /* Get the bridge mode UNI side */
            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            link       = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

            if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
            {
                //no need to check the ani side, just get uni side
                continue;
            }

            vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
            if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
            {
                OMCIALDBG("Have vlan filter for uni side : 1:P or N:P mode Now");
                OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                for (idx = 0; idx < vlanfilter->attr.number; idx++)
                {
                    vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                }
            }

            //when have mutil-uni ports, it will be N:P
            if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
            {
                pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
                uniFlag = CA_OMCI_TRUE;
            }
            if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
            {
                iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == iphost);
                uniFlag = CA_OMCI_TRUE;
            }
            if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
            {
                veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == veip);
                uniFlag = CA_OMCI_TRUE;
            }
        }

        if (CA_OMCI_FALSE == uniFlag)
        {
            OMCIALERR("Flow is not complete, no uni side");
            return CA_OMCI_FALSE;
        }

        OMCI_MEMSET(&gemflow, 0, sizeof(ca_omci_gem_flow_t));
        OMCI_MEMCPY(gemflow.vidList, vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
        gemflow.gemId         = gpnctp->attr.portId;
        gemflow.tcontId       = tcont->meBase.instanceId;
        gemflow.allocId       = tcont->attr.allocId;
        gemflow.direction     = gpnctp->attr.direction;
        gemflow.usQueue       = (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction) ? gpnctp->attr.usTrafMngPoint : 0;
        gemflow.dsQueue       = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
        gemflow.aesState      = gpnctp->attr.encryptState;
        gemflow.aesKeyRing    = gpnctp->attr.encryptKeyRing;
        gemflow.usQueueDpcm   = (NULL != usPriQ) ? usPriQ->attr.colorMark : 0;
        gemflow.usQueueWeight = (NULL != usPriQ) ? usPriQ->attr.weight : 0;
        gemflow.dsQueueDpcm   = (NULL != dsPriQ) ? dsPriQ->attr.colorMark : 0;
        gemflow.dsQueueWeight = (NULL != dsPriQ) ? dsPriQ->attr.weight : 0;

        if (NULL != usPriQ)
        {
            trafSche = (ca_omci_traffic_scheg_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_SCHEG_CLASS_ID, usPriQ->attr.trafSchedPoint);
            if ((NULL != trafSche) && (0 == usPriQ->attr.weight))
            {
                OMCIALERR("ALCL OLT set WRR queue weigh 0, don't create this gemport %d", gemflow.gemId);
                return CA_OMCI_FALSE;
            }
        }

        if (NULL != usTrafDesc)
        {
            //gemflow.usTrafficDes.mode = usTrafDesc->attr.;
            //gemflow.usTrafficDes.pps    = usTrafDesc->attr.;
            gemflow.usTrafficDes.cir = usTrafDesc->attr.cir;
            gemflow.usTrafficDes.cbs = usTrafDesc->attr.cbs;
            gemflow.usTrafficDes.pir = usTrafDesc->attr.pir;
            gemflow.usTrafficDes.pbs = usTrafDesc->attr.pbs;
        }
        if (NULL != dsTrafDesc)
        {
            //gemflow.dsTrafficDes.mode = dsTrafDesc->attr.;
            //gemflow.dsTrafficDes.pps    = dsTrafDesc->attr.;
            gemflow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
            gemflow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
            gemflow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
            gemflow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
        }
        ca_omci_gemflow_tbl_add(&gemflow);
        return CA_OMCI_TRUE;
    }

    if (GEM_IWTP_IW_OPTION_8021P == gemiwtp->attr.iwOption)
    {
        ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));
        /* flow is not complete when dot1p is not exist nor linked */
        OMCI_CHECK_PARAM_RET((NULL == ieee8021p), CA_OMCI_FALSE);
        OMCI_CHECK_PARAM_RET((0 == ieee8021p->meBase.linkedCount), CA_OMCI_FALSE);

        pri = ca_omci_gemflow_pri_get(gemiwtp->meBase.instanceId, ieee8021p);
        /* when 802.1p links to Bridge, the mode is 802.1p + Bridge
                      * N:M or 1:MP or N:MP mode */
        if (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == ieee8021p->attr.tpType)
        {
            link = ieee8021p->meBase.linkedMeHeader;
            while (link)
            {
                /* find the mac port cfg data me only */
                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                {
                    link = link->next;
                    continue;
                }

                /* Get the bridge for 802.1p, there is only one bridge for a 802.1p */
                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                link       = link->next;
                OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    //Have vlan filter 1:MP or N:MP mode Now
                    OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                    }
                }

                //If no mac brg found, the flow is not complete
                macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                OMCI_CHECK_PARAM_CNU(NULL == macbrg);

                if (macbrg->meBase.linkedCount)
                {
                    linkbrgp = macbrg->meBase.linkedMeHeader;
                    uniFlag  = CA_OMCI_FALSE;
                    while (linkbrgp)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                        {
                            linkbrgp = linkbrgp->next;
                            continue;
                        }

                        /* Get the bridge mode UNI side */
                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                        linkbrgp   = linkbrgp->next;
                        OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                        if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                        {
                            continue;
                        }

                        vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                        if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                        {
                            //Have vlan filter link to uni side mac port
                            OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                            for (idx = 0; idx < vlanfilter->attr.number; idx++)
                            {
                                vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                            }
                        }

                        if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                        {
                            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
                            uniFlag = CA_OMCI_TRUE;
                        }
                        if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                        {
                            iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == iphost);
                            uniFlag = CA_OMCI_TRUE;
                        }
                        if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                        {
                            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == veip);
                            uniFlag = CA_OMCI_TRUE;
                        }
                    }

                    if (CA_OMCI_FALSE == uniFlag)
                    {
                        OMCIALERR("Flow is not complete, no uni side");
                        return CA_OMCI_FALSE;
                    }
                }
            }

            OMCI_MEMSET(&gemflow, 0, sizeof(ca_omci_gem_flow_t));
            OMCI_MEMCPY(gemflow.vidList, vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
            gemflow.gemId         = gpnctp->attr.portId;
            gemflow.tcontId       = tcont->meBase.instanceId;
            gemflow.allocId       = tcont->attr.allocId;
            gemflow.direction     = gpnctp->attr.direction;
            gemflow.usQueue       = (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction) ? gpnctp->attr.usTrafMngPoint : 0;
            gemflow.dsQueue       = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
            gemflow.aesState      = gpnctp->attr.encryptState;
            gemflow.aesKeyRing    = gpnctp->attr.encryptKeyRing;
            gemflow.gemPri        = pri;
            gemflow.usQueueDpcm   = (NULL != usPriQ) ? usPriQ->attr.colorMark : 0;
            gemflow.usQueueWeight = (NULL != usPriQ) ? usPriQ->attr.weight : 0;
            gemflow.dsQueueDpcm   = (NULL != dsPriQ) ? dsPriQ->attr.colorMark : 0;
            gemflow.dsQueueWeight = (NULL != dsPriQ) ? dsPriQ->attr.weight : 0;

            if ((ieee8021p->attr.unmarkFrameOp) && (0 != (pri & (1 << ieee8021p->attr.defPbitAssu)))) {
                gemflow.transUntag = CA_OMCI_TRUE;
            }

            if (NULL != usPriQ)
            {
                trafSche = (ca_omci_traffic_scheg_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_SCHEG_CLASS_ID, usPriQ->attr.trafSchedPoint);
                if ((NULL != trafSche) && (0 == usPriQ->attr.weight))
                {
                    OMCIALERR("ALCL OLT set WRR queue weigh 0, don't create this gemport %d", gemflow.gemId);
                    return CA_OMCI_FALSE;
                }
            }

            if (NULL != usTrafDesc)
            {
                //gemflow.usTrafficDes.mode = usTrafDesc->attr.;
                //gemflow.usTrafficDes.pps    = usTrafDesc->attr.;
                gemflow.usTrafficDes.cir = usTrafDesc->attr.cir;
                gemflow.usTrafficDes.cbs = usTrafDesc->attr.cbs;
                gemflow.usTrafficDes.pir = usTrafDesc->attr.pir;
                gemflow.usTrafficDes.pbs = usTrafDesc->attr.pbs;
            }
            if (NULL != dsTrafDesc)
            {
                //gemflow.dsTrafficDes.mode = dsTrafDesc->attr.;
                //gemflow.dsTrafficDes.pps    = dsTrafDesc->attr.;
                gemflow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
                gemflow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
                gemflow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
                gemflow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
            }
            ca_omci_gemflow_tbl_add(&gemflow);
            return CA_OMCI_TRUE;;
        }

        //other mode, support these type below now
        if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
         && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
         && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
        {
            OMCIALDBG("Unsupport type %d need to add code", ieee8021p->attr.tpType);
            return CA_OMCI_FALSE;
        }

        if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieee8021p->attr.tpPointer);
            OMCI_CHECK_PARAM_RET((NULL == pptpEth), CA_OMCI_FALSE);
        }
        if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
        {
            iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, ieee8021p->attr.tpPointer);
            OMCI_CHECK_PARAM_RET((NULL == iphost), CA_OMCI_FALSE);
        }
        if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
        {
            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieee8021p->attr.tpPointer);
            OMCI_CHECK_PARAM_RET((NULL == veip), CA_OMCI_FALSE);
        }

        OMCI_MEMSET(&gemflow, 0, sizeof(ca_omci_gem_flow_t));
        OMCI_MEMCPY(gemflow.vidList, vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
        gemflow.gemId         = gpnctp->attr.portId;
        gemflow.tcontId       = tcont->meBase.instanceId;
        gemflow.allocId       = tcont->attr.allocId;
        gemflow.direction     = gpnctp->attr.direction;
        gemflow.usQueue       = (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction) ? gpnctp->attr.usTrafMngPoint : 0;
        gemflow.dsQueue       = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
        gemflow.aesState      = gpnctp->attr.encryptState;
        gemflow.aesKeyRing    = gpnctp->attr.encryptKeyRing;
        gemflow.gemPri        = pri;
        gemflow.usQueueDpcm   = (NULL != usPriQ) ? usPriQ->attr.colorMark : 0;
        gemflow.usQueueWeight = (NULL != usPriQ) ? usPriQ->attr.weight : 0;
        gemflow.dsQueueDpcm   = (NULL != dsPriQ) ? dsPriQ->attr.colorMark : 0;
        gemflow.dsQueueWeight = (NULL != dsPriQ) ? dsPriQ->attr.weight : 0;

        if ((ieee8021p->attr.unmarkFrameOp) && (0 != (pri & (1 << ieee8021p->attr.defPbitAssu)))) {
            gemflow.transUntag = CA_OMCI_TRUE;
        }

        if (NULL != usPriQ)
        {
            trafSche = (ca_omci_traffic_scheg_t *)ca_omci_me_entry_lookup(OMCI_ME_TRAFFIC_SCHEG_CLASS_ID, usPriQ->attr.trafSchedPoint);
            if ((NULL != trafSche) && (0 == usPriQ->attr.weight))
            {
                OMCIALERR("ALCL OLT set WRR queue weigh 0, don't create this gemport %d", gemflow.gemId);
                return CA_OMCI_FALSE;
            }
        }

        if (NULL != usTrafDesc)
        {
            //gemflow.usTrafficDes.mode = usTrafDesc->attr.;
            //gemflow.usTrafficDes.pps    = usTrafDesc->attr.;
            gemflow.usTrafficDes.cir = usTrafDesc->attr.cir;
            gemflow.usTrafficDes.cbs = usTrafDesc->attr.cbs;
            gemflow.usTrafficDes.pir = usTrafDesc->attr.pir;
            gemflow.usTrafficDes.pbs = usTrafDesc->attr.pbs;
        }
        if (NULL != dsTrafDesc)
        {
            //gemflow.dsTrafficDes.mode = dsTrafDesc->attr.;
            //gemflow.dsTrafficDes.pps    = dsTrafDesc->attr.;
            gemflow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
            gemflow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
            gemflow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
            gemflow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
        }
        ca_omci_gemflow_tbl_add(&gemflow);
        return CA_OMCI_TRUE;
    }

    if (GEM_IWTP_IW_OPTION_DS_BROADCAST == gemiwtp->attr.iwOption)
    {
        //when this me create for ds broadcast, it point to NULL, only be pointed
        link = gemiwtp->meBase.linkedMeHeader;
        while (link)
        {
            /* find the mac port cfg data me only */
            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            link       = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

            vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
            if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
            {
                //Have vlan filter 1:MP or N:MP mode Now
                OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                for (idx = 0; idx < vlanfilter->attr.number; idx++)
                {
                    vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                }
            }

            //If no mac brg found, the flow is not complete
            macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
            OMCI_CHECK_PARAM_CNU(NULL == macbrg);

            if (macbrg->meBase.linkedCount)
            {
                linkbrgp = macbrg->meBase.linkedMeHeader;
                uniFlag  = CA_OMCI_FALSE;
                while (linkbrgp)
                {
                    /* find the mac port cfg data me only */
                    if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                    {
                        linkbrgp = linkbrgp->next;
                        continue;
                    }

                    /* Get the bridge mode UNI side */
                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                    linkbrgp   = linkbrgp->next;
                    OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                    if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                     && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                     && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                    {
                        continue;
                    }

                    vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                    if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                    {
                        //Have vlan filter link to uni side mac port
                        OMCI_MEMSET(&vidList, 0, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
                        for (idx = 0; idx < vlanfilter->attr.number; idx++)
                        {
                            vidList[idx] = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                        }
                    }

                    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                    {
                        pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == pptpEth);
                        uniFlag = CA_OMCI_TRUE;
                    }
                    if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                    {
                        iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == iphost);
                        uniFlag = CA_OMCI_TRUE;
                    }
                    if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                    {
                        veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == veip);
                        uniFlag = CA_OMCI_TRUE;
                    }
                }

                if (CA_OMCI_FALSE == uniFlag)
                {
                    OMCIALERR("downstream broadcast flow is not complete, no uni side");
                    return CA_OMCI_FALSE;
                }
            }

            OMCI_MEMSET(&gemflow, 0, sizeof(ca_omci_gem_flow_t));
            OMCI_MEMCPY(gemflow.vidList, vidList, sizeof(ca_omci_uint16_t) * OMCI_VLAN_LIST_NUM);
            gemflow.gemId         = gpnctp->attr.portId;
            gemflow.direction     = gpnctp->attr.direction;
            gemflow.usQueue       = (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction) ? gpnctp->attr.usTrafMngPoint : 0;
            gemflow.dsQueue       = (GEM_NET_CTP_DIR_US != gpnctp->attr.direction) ? gpnctp->attr.dsPqPoint : 0;
            gemflow.aesState      = gpnctp->attr.encryptState;
            gemflow.aesKeyRing    = gpnctp->attr.encryptKeyRing;
            gemflow.gemPri        = pri;

            if (NULL != dsTrafDesc)
            {
                //gemflow.dsTrafficDes.mode = dsTrafDesc->attr.;
                //gemflow.dsTrafficDes.pps    = dsTrafDesc->attr.;
                gemflow.dsTrafficDes.cir = dsTrafDesc->attr.cir;
                gemflow.dsTrafficDes.cbs = dsTrafDesc->attr.cbs;
                gemflow.dsTrafficDes.pir = dsTrafDesc->attr.pir;
                gemflow.dsTrafficDes.pbs = dsTrafDesc->attr.pbs;
            }
            ca_omci_gemflow_tbl_add(&gemflow);
            return CA_OMCI_TRUE;
        }
    }

    return CA_OMCI_FALSE;
}
#endif


#if DESCRIPTION("Stream functions")
static void ca_omci_analysis_stream_mac_srv(
    ca_omci_uint16_t           entityId,
    ca_omci_ext_vlan_tag_op_t *extvlan,
    ca_omci_gem_net_ctp_t     *gpnctp)
{
    ca_omci_mac_brg_srv_t     *macbrg     = NULL;
    ca_omci_me_link_t         *link       = NULL;
    ca_omci_mac_port_cfg_t    *brgportcfg = NULL;
    ca_omci_pptp_eth_t        *pptpEth    = NULL;
    ca_omci_ip_host_t         *iphost     = NULL;
    ca_omci_veip_t            *veip       = NULL;
    ca_omci_stream_cfg_t       streamUs   = {0};
    ca_omci_stream_cfg_t       streamDs   = {0};

    macbrg = (ca_omci_mac_brg_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, entityId);
    OMCI_CHECK_PARAM_RETURN(NULL == macbrg);

    link = macbrg->meBase.linkedMeHeader;
    while (link)
    {
        /* find all uni side mac port and uni */
        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
        {
            link = link->next;
            continue;
        }

        /* Get the bridge mode UNI side */
        brgportcfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
        link       = link->next;
        OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

        if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
         && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
         && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
        {
            continue;
        }

        if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
        {
            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

            if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                streamUs.gemId     = gpnctp->attr.portId;
                streamUs.uniPortId = brgportcfg->attr.tpPointer;
                streamUs.portType  = OMCI_CFG_STREAM_PORT_ETH;
                ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
            }
            if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                streamDs.gemId     = gpnctp->attr.portId;
                streamDs.uniPortId = brgportcfg->attr.tpPointer;
                streamDs.portType  = OMCI_CFG_STREAM_PORT_ETH;
                ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
            }

            OMCIALDBG("uni type is pptp eth [0x%x]", brgportcfg->attr.tpPointer);
            continue;
        }

        if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
        {
            iphost = (ca_omci_ip_host_t*)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == iphost);

            if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                streamUs.gemId     = gpnctp->attr.portId;
                streamUs.uniPortId = brgportcfg->attr.tpPointer;
                streamUs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
            }
            if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                streamDs.gemId     = gpnctp->attr.portId;
                streamDs.uniPortId = brgportcfg->attr.tpPointer;
                streamDs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
            }

            OMCIALDBG("uni type is iphost [0x%x]", brgportcfg->attr.tpPointer);
            continue;
        }

        if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
        {
            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
            OMCI_CHECK_PARAM_CNU(NULL == veip);

            if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                streamUs.gemId     = gpnctp->attr.portId;
                streamUs.uniPortId = brgportcfg->attr.tpPointer;
                streamUs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
            }
            if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
            {
                OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                streamDs.gemId     = gpnctp->attr.portId;
                streamDs.uniPortId = brgportcfg->attr.tpPointer;
                streamDs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
            }

            OMCIALDBG("uni type is veip [0x%x]", brgportcfg->attr.tpPointer);
            continue;
        }
    }
}


static void ca_omci_analysis_stream_dot1p(
    ca_omci_uint16_t           entityId,
    ca_omci_ext_vlan_tag_op_t *extvlan,
    ca_omci_gem_net_ctp_t     *gpnctp)
{
    ca_omci_ieee_dot1p_srv_t *ieee8021p  = NULL;
    ca_omci_me_link_t        *link       = NULL;
    ca_omci_mac_port_cfg_t   *brgportcfg = NULL;
    ca_omci_pptp_eth_t       *pptpEth    = NULL;
    ca_omci_ip_host_t        *iphost     = NULL;
    ca_omci_veip_t           *veip       = NULL;
    ca_omci_stream_cfg_t      streamUs   = {0};
    ca_omci_stream_cfg_t      streamDs   = {0};

    ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, entityId);
    OMCI_CHECK_PARAM_RETURN(NULL == ieee8021p);

    if ((IEEE_DOT1P_SRV_TP_TYPE_BRIDGE != ieee8021p->attr.tpType)
     && (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
     && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
     && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
    {
        return;
    }

    if (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == ieee8021p->attr.tpType)
    {
        OMCIALDBG("type bridge+802.1p");

        link = ieee8021p->meBase.linkedMeHeader;
        while (link)
        {
            /* Get the bridge link dot1p and mac srv side */
            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            brgportcfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link       = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

            ca_omci_analysis_stream_mac_srv(brgportcfg->attr.brgIdPointer, extvlan, gpnctp);
            return;
        }

        return;
    }

    OMCIALDBG("type 802.1p");
    if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
    {
        pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieee8021p->attr.tpPointer);
        OMCI_CHECK_PARAM_RETURN(NULL == pptpEth);

        if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
            streamUs.gemId     = gpnctp->attr.portId;
            streamUs.uniPortId = ieee8021p->attr.tpPointer;
            streamUs.portType  = OMCI_CFG_STREAM_PORT_ETH;
            ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
        }
        if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
            streamDs.gemId     = gpnctp->attr.portId;
            streamDs.uniPortId = ieee8021p->attr.tpPointer;
            streamDs.portType  = OMCI_CFG_STREAM_PORT_ETH;
            ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
        }

        OMCIALDBG("uni type is pptp eth [0x%x]", ieee8021p->attr.tpPointer);
        return;
    }

    if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
    {
        iphost = (ca_omci_ip_host_t*)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, ieee8021p->attr.tpPointer);
        OMCI_CHECK_PARAM_RETURN(NULL == iphost);

        if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
            streamUs.gemId     = gpnctp->attr.portId;
            streamUs.uniPortId = ieee8021p->attr.tpPointer;
            streamUs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
            ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
        }
        if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
            streamDs.gemId     = gpnctp->attr.portId;
            streamDs.uniPortId = ieee8021p->attr.tpPointer;
            streamDs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
            ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
        }

        OMCIALDBG("uni type is iphost [0x%x]", ieee8021p->attr.tpPointer);
        return;
    }

    if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
    {
        veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieee8021p->attr.tpPointer);
        OMCI_CHECK_PARAM_RETURN(NULL == veip);

        if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
            streamUs.gemId     = gpnctp->attr.portId;
            streamUs.uniPortId = ieee8021p->attr.tpPointer;
            streamUs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
            ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
        }
        if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
        {
            OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
            streamDs.gemId     = gpnctp->attr.portId;
            streamDs.uniPortId = ieee8021p->attr.tpPointer;
            streamDs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
            ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
        }

        OMCIALDBG("uni type is veip [0x%x]", ieee8021p->attr.tpPointer);
        return;
    }
}


static void ca_omci_analysis_stream_gem(
    ca_omci_uint16_t           entityId,
    ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_gem_iwtp_t       *gemiwtp    = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp     = NULL;
    ca_omci_me_link_t        *link       = NULL;
    ca_omci_mac_port_cfg_t   *brgportcfg = NULL;
    ca_omci_pptp_eth_t       *pptpEth    = NULL;
    ca_omci_ip_host_t        *iphost     = NULL;
    ca_omci_veip_t           *veip       = NULL;
    ca_omci_stream_cfg_t      streamUs   = {0};
    ca_omci_stream_cfg_t      streamDs   = {0};

    gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, entityId);
    OMCI_CHECK_PARAM_RETURN(NULL == gemiwtp);

    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == gpnctp);

    if ((GEM_IWTP_IW_OPTION_MAC_BRG_LAN != gemiwtp->attr.iwOption)
     && (GEM_IWTP_IW_OPTION_8021P != gemiwtp->attr.iwOption)
     && (GEM_IWTP_IW_OPTION_DS_BROADCAST != gemiwtp->attr.iwOption))
    {
        return;
    }

    OMCIALDBG("extern vlan link to gem_iwtp [0x%x] dir [%d]", entityId, gpnctp->attr.direction);

    if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemiwtp->attr.iwOption)
    {
        OMCIALDBG("type bridge");
        ca_omci_analysis_stream_mac_srv(gemiwtp->attr.srvProfPoint, extvlan, gpnctp);
        return;
    }

    if (GEM_IWTP_IW_OPTION_8021P == gemiwtp->attr.iwOption)
    {
        ca_omci_analysis_stream_dot1p(gemiwtp->attr.srvProfPoint, extvlan, gpnctp);
        return;
    }

    if (GEM_IWTP_IW_OPTION_DS_BROADCAST == gemiwtp->attr.iwOption)
    {
        link = gemiwtp->meBase.linkedMeHeader;
        while (link)
        {
            /* find all uni side mac port and uni */
            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
            {
                link = link->next;
                continue;
            }

            /* Get the bridge mode UNI side */
            brgportcfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link       = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

            if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
            {
                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
            {
                pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = brgportcfg->attr.tpPointer;
                    streamUs.portType  = OMCI_CFG_STREAM_PORT_ETH;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = brgportcfg->attr.tpPointer;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_ETH;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }

                OMCIALDBG("uni type is pptp eth [0x%x]", brgportcfg->attr.tpPointer);
                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
            {
                iphost = (ca_omci_ip_host_t*)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == iphost);

                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = brgportcfg->attr.tpPointer;
                    streamUs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = brgportcfg->attr.tpPointer;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }

                OMCIALDBG("uni type is iphost [0x%x]", brgportcfg->attr.tpPointer);
                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
            {
                veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == veip);

                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = brgportcfg->attr.tpPointer;
                    streamUs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = brgportcfg->attr.tpPointer;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }

                OMCIALDBG("uni type is veip [0x%x]", brgportcfg->attr.tpPointer);
                continue;
            }
        }

        return;
    }
}


static void ca_omci_analysis_stream_multi_gem(
    ca_omci_uint16_t           entityId,
    ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_multi_gem_iwtp_t *multiGem   = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp     = NULL;

    multiGem = (ca_omci_multi_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, entityId);
    OMCI_CHECK_PARAM_RETURN(NULL == multiGem);

    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, multiGem->attr.gemNetCtpPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == gpnctp);

    OMCIALDBG("extern vlan link to multicast gem [0x%x] dir [%d]", entityId, gpnctp->attr.direction);
    if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGem->attr.iwOption)
    {
        OMCIALDBG("type ds broadcast bridge");
        ca_omci_analysis_stream_mac_srv(multiGem->attr.srvProfPoint, extvlan, gpnctp);
        return;
    }

    if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGem->attr.iwOption)
    {
        OMCIALDBG("type ds broadcast");
        ca_omci_analysis_stream_dot1p(multiGem->attr.srvProfPoint, extvlan, gpnctp);
        return;
    }
}


static void ca_omci_analysis_stream_ieee_dot1p(
    ca_omci_uint16_t           entityId,
    ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_ieee_dot1p_srv_t *ieee8021p = NULL;
    ca_omci_me_link_t        *link      = NULL;
    ca_omci_gem_iwtp_t       *gemiwtp   = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp    = NULL;

    ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, entityId);
    OMCI_CHECK_PARAM_RETURN(NULL == ieee8021p);

    link = ieee8021p->meBase.linkedMeHeader;
    while (link)
    {
        if (OMCI_ME_GEM_IWTP_CLASS_ID != link->classId)
        {
            link = link->next;
            continue;
        }

        //find the gemport
        gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
        link    = link->next;
        OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

        gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
        OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

        ca_omci_analysis_stream_dot1p(entityId, extvlan, gpnctp);
    }
}


static void ca_omci_analysis_stream_mac_port_uni(
    ca_omci_uint16_t           entity,
    ca_omci_ext_vlan_tag_op_t *extvlan,
    ca_omci_uint32_t           portType)
{
    ca_omci_mac_port_cfg_t   *brgportcfg = NULL;
    ca_omci_mac_brg_srv_t    *macbrg     = NULL;
    ca_omci_me_link_t        *link       = NULL;
    ca_omci_mac_port_cfg_t   *linkPort   = NULL;
    ca_omci_multi_gem_iwtp_t *multiGem   = NULL;
    ca_omci_ieee_dot1p_srv_t *ieee8021p  = NULL;
    ca_omci_me_link_t        *linkGem    = NULL;
    ca_omci_gem_iwtp_t       *gemiwtp    = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp     = NULL;
    ca_omci_stream_cfg_t      streamUs   = {0};
    ca_omci_stream_cfg_t      streamDs   = {0};

    brgportcfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, entity);
    OMCI_CHECK_PARAM_RETURN(NULL == brgportcfg);

    macbrg = (ca_omci_mac_brg_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer);
    OMCI_CHECK_PARAM_RETURN(NULL == macbrg);

    link = macbrg->meBase.linkedMeHeader;
    while (link)
    {
        //skip itself
        if (link->classId == OMCI_ME_MAC_PORT_CFG_CLASS_ID && link->instanceId == entity)
        {
            link = link->next;
            continue;
        }

        //find the macport only link to dot1p and multicast gemport
        if (link->classId == OMCI_ME_MAC_PORT_CFG_CLASS_ID)
        {
            linkPort = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link     = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == linkPort);

            if ((MAC_PORT_CFG_TP_TYPE_MULTI_GEM != linkPort->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV != linkPort->attr.tpType)
             && (MAC_PORT_CFG_TP_TYPE_GEM_IWTP != linkPort->attr.tpType))
            {
                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_MULTI_GEM == linkPort->attr.tpType)
            {
                multiGem = (ca_omci_multi_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_MULTI_GEM_IWTP_CLASS_ID, linkPort->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == multiGem);

                gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, multiGem->attr.gemNetCtpPoint);
                OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                streamDs.gemId     = gpnctp->attr.portId;
                streamDs.uniPortId = brgportcfg->attr.tpPointer;
                streamDs.portType  = portType;
                ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == linkPort->attr.tpType)
            {
                ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, linkPort->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

                linkGem = ieee8021p->meBase.linkedMeHeader;
                while (linkGem)
                {
                    if (OMCI_ME_GEM_IWTP_CLASS_ID != linkGem->classId)
                    {
                        linkGem = linkGem->next;
                        continue;
                    }

                    //find the gemport
                    gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(linkGem->classId, linkGem->instanceId);
                    linkGem = linkGem->next;
                    OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

                    gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                    OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                    if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                    {
                        OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                        streamUs.gemId     = gpnctp->attr.portId;
                        streamUs.uniPortId = brgportcfg->attr.tpPointer;
                        streamUs.portType  = portType;
                        ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                    }
                    if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                    {
                        OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                        streamDs.gemId     = gpnctp->attr.portId;
                        streamDs.uniPortId = brgportcfg->attr.tpPointer;
                        streamDs.portType  = portType;
                        ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                    }
                }

                continue;
            }

            if (MAC_PORT_CFG_TP_TYPE_GEM_IWTP == linkPort->attr.tpType)
            {
                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_IWTP_CLASS_ID, linkPort->attr.tpPointer);
                OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

                gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = brgportcfg->attr.tpPointer;
                    streamUs.portType  = portType;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }

                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = brgportcfg->attr.tpPointer;
                    streamDs.portType  = portType;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }
                continue;
            }
            continue;
        }

        link = link->next;
        continue;
    }
}

static void ca_omci_analysis_stream_pptp_eth(ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_pptp_eth_t       *pptpEth   = NULL;
    ca_omci_me_link_t        *link      = NULL;
    ca_omci_ieee_dot1p_srv_t *ieee8021p = NULL;
    ca_omci_me_link_t        *linkGem   = NULL;
    ca_omci_gem_iwtp_t       *gemiwtp   = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp    = NULL;
    ca_omci_stream_cfg_t      streamUs  = {0};
    ca_omci_stream_cfg_t      streamDs  = {0};

    OMCI_ENTER;

    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, extvlan->attr.assocPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == pptpEth);

    link = pptpEth->meBase.linkedMeHeader;
    while (link)
    {
        if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID == link->classId)
        {
            ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link      = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

            linkGem = ieee8021p->meBase.linkedMeHeader;
            while (linkGem)
            {
                if (OMCI_ME_GEM_IWTP_CLASS_ID != linkGem->classId)
                {
                    linkGem = linkGem->next;
                    continue;
                }

                //find the gemport
                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(linkGem->classId, linkGem->instanceId);
                linkGem = linkGem->next;
                OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

                gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                //TODO: add priority in gemport
                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId           = gpnctp->attr.portId;
                    streamUs.uniPortId       = extvlan->attr.assocPoint;
                    streamUs.portType        = OMCI_CFG_STREAM_PORT_ETH;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = extvlan->attr.assocPoint;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_ETH;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }
            }

            continue;
        }

        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
        {
            ca_omci_analysis_stream_mac_port_uni(link->instanceId, extvlan,
                                                 OMCI_CFG_STREAM_PORT_ETH);
            link = link->next;
            continue;
        }

        link = link->next;
        continue;
    }

    OMCI_LEAVE;
}


static void ca_omci_analysis_stream_iphost(ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_ip_host_t        *iphost    = NULL;
    ca_omci_me_link_t        *link      = NULL;
    ca_omci_ieee_dot1p_srv_t *ieee8021p = NULL;
    ca_omci_me_link_t        *linkGem   = NULL;
    ca_omci_gem_iwtp_t       *gemiwtp   = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp    = NULL;
    ca_omci_stream_cfg_t      streamUs  = {0};
    ca_omci_stream_cfg_t      streamDs  = {0};

    iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, extvlan->attr.assocPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == iphost);

    link = iphost->meBase.linkedMeHeader;
    while (link)
    {
        if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID == link->classId)
        {
            ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link      = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

            linkGem = ieee8021p->meBase.linkedMeHeader;
            while (linkGem)
            {
                if (OMCI_ME_GEM_IWTP_CLASS_ID != linkGem->classId)
                {
                    linkGem = linkGem->next;
                    continue;
                }

                //find the gemport
                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(linkGem->classId, linkGem->instanceId);
                linkGem = linkGem->next;
                OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

                gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                //TODO: add priority in gemport
                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = extvlan->attr.assocPoint;
                    streamUs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = extvlan->attr.assocPoint;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_IPHOST;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }
            }

            continue;
        }

        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
        {
            ca_omci_analysis_stream_mac_port_uni(link->instanceId, extvlan,
                                                 OMCI_CFG_STREAM_PORT_IPHOST);
            link = link->next;
            continue;
        }

        link = link->next;
        continue;
    }
}


static void ca_omci_analysis_stream_veip(ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_veip_t           *veip      = NULL;
    ca_omci_me_link_t        *link      = NULL;
    ca_omci_ieee_dot1p_srv_t *ieee8021p = NULL;
    ca_omci_me_link_t        *linkGem   = NULL;
    ca_omci_gem_iwtp_t       *gemiwtp   = NULL;
    ca_omci_gem_net_ctp_t    *gpnctp    = NULL;
    ca_omci_stream_cfg_t      streamUs  = {0};
    ca_omci_stream_cfg_t      streamDs  = {0};

    veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, extvlan->attr.assocPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == veip);

    link = veip->meBase.linkedMeHeader;
    while (link)
    {
        if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID == link->classId)
        {
            ieee8021p = (ca_omci_ieee_dot1p_srv_t *)ca_omci_me_entry_lookup(link->classId, link->instanceId);
            link      = link->next;
            OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

            linkGem = ieee8021p->meBase.linkedMeHeader;
            while (linkGem)
            {
                if (OMCI_ME_GEM_IWTP_CLASS_ID != linkGem->classId)
                {
                    linkGem = linkGem->next;
                    continue;
                }

                //find the gemport
                gemiwtp = (ca_omci_gem_iwtp_t *)ca_omci_me_entry_lookup(linkGem->classId, linkGem->instanceId);
                linkGem = linkGem->next;
                OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

                gpnctp = (ca_omci_gem_net_ctp_t *)ca_omci_me_entry_lookup(OMCI_ME_GEM_NET_CTP_CLASS_ID, gemiwtp->attr.gemNetCtpPoint);
                OMCI_CHECK_PARAM_CNU(NULL == gpnctp);

                //TODO: add priority in gemport
                if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamUs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamUs.gemId     = gpnctp->attr.portId;
                    streamUs.uniPortId = extvlan->attr.assocPoint;
                    streamUs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                    ca_omci_us_stream_tbl_merge(&streamUs, extvlan);
                }
                if (GEM_NET_CTP_DIR_US != gpnctp->attr.direction)
                {
                    OMCI_MEMSET(&streamDs, 0, sizeof(ca_omci_stream_cfg_t));
                    streamDs.gemId     = gpnctp->attr.portId;
                    streamDs.uniPortId = extvlan->attr.assocPoint;
                    streamDs.portType  = OMCI_CFG_STREAM_PORT_VEIP;
                    ca_omci_ds_stream_tbl_merge(&streamDs, extvlan);
                }
            }

            continue;
        }

        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
        {
            ca_omci_analysis_stream_mac_port_uni(link->instanceId, extvlan,
                                                 OMCI_CFG_STREAM_PORT_VEIP);
            link = link->next;
            continue;
        }

        link = link->next;
        continue;
    }
}


static void ca_omci_analysis_stream_mac_port(ca_omci_ext_vlan_tag_op_t *extvlan)
{
    ca_omci_mac_port_cfg_t   *brgportcfg = NULL;

    brgportcfg = (ca_omci_mac_port_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, extvlan->attr.assocPoint);
    OMCI_CHECK_PARAM_RETURN(NULL == brgportcfg);

    if (MAC_PORT_CFG_TP_TYPE_DOT1P_SRV == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_ieee_dot1p(brgportcfg->attr.tpPointer, extvlan);
        return;
    }

    if (MAC_PORT_CFG_TP_TYPE_GEM_IWTP == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_gem(brgportcfg->attr.tpPointer, extvlan);
        return;
    }

    if (MAC_PORT_CFG_TP_TYPE_MULTI_GEM == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_multi_gem(brgportcfg->attr.tpPointer, extvlan);
        return;
    }

    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_mac_port_uni(brgportcfg->meBase.instanceId,
                                             extvlan,
                                             OMCI_CFG_STREAM_PORT_ETH);
        return;
    }

    if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_mac_port_uni(brgportcfg->meBase.instanceId,
                                             extvlan,
                                             OMCI_CFG_STREAM_PORT_IPHOST);
        return;
    }

    if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
    {
        ca_omci_analysis_stream_mac_port_uni(brgportcfg->meBase.instanceId,
                                             extvlan,
                                             OMCI_CFG_STREAM_PORT_VEIP);
        return;
    }
}


static void ca_omci_analysis_stream(ca_omci_mebase_t *meInst)
{
    /* extvlan will never be null, so no need to check it */
    ca_omci_ext_vlan_tag_op_t *extvlan = (ca_omci_ext_vlan_tag_op_t *)meInst;

    if ((EXT_VLAN_TAG_OP_ASSOC_TYPE_XDSL == extvlan->attr.assocType)
     || (EXT_VLAN_TAG_OP_ASSOC_TYPE_MOCA == extvlan->attr.assocType)
     || (EXT_VLAN_TAG_OP_ASSOC_TYPE_ETH_FLOW == extvlan->attr.assocType)
     || (EXT_VLAN_TAG_OP_ASSOC_TYPE_MPLS == extvlan->attr.assocType)
     || (EXT_VLAN_TAG_OP_ASSOC_TYPE_RESERVED == extvlan->attr.assocType))
    {
        OMCIALDBG("unsupport assoc type [%d]", extvlan->attr.assocType);
        return;
    }

    //it links to a gemport
    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_GEM_IWTP == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_gem(extvlan->attr.assocPoint, extvlan);
        return;
    }

    //it links to a multicast gemport
    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MULTI_GEM == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_multi_gem(extvlan->attr.assocPoint, extvlan);
        return;
    }

    //it links to a ieee 802.1p
    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_8021P == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_ieee_dot1p(extvlan->attr.assocPoint, extvlan);
        return;
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_PPTP_ETH == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_pptp_eth(extvlan);
        return;
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_IPHOST == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_iphost(extvlan);
        return;
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_VEIP == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_veip(extvlan);
        return;
    }

    if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MAC_PORT == extvlan->attr.assocType)
    {
        ca_omci_analysis_stream_mac_port(extvlan);
        return;
    }

}


static void ca_omci_analysis_filter_stream(void)
{
    ca_omci_meclass_t           *meClass    = NULL;
    ca_omci_mebase_t            *meInst     = NULL;
    ca_omci_me_link_t           *link       = NULL;
    ca_omci_me_link_t           *linkbrgp   = NULL;
    ca_omci_gem_net_ctp_t       *gpnctp     = NULL;
    ca_omci_gem_iwtp_t          *gemiwtp    = NULL;
    ca_omci_mac_brg_srv_t       *macbrg     = NULL;
    ca_omci_mac_port_cfg_t      *brgportcfg = NULL;
    ca_omci_pptp_eth_t          *pptpEth    = NULL;
    ca_omci_ip_host_t           *iphost     = NULL;
    ca_omci_veip_t              *veip       = NULL;
    ca_omci_ieee_dot1p_srv_t    *ieee8021p  = NULL;
    ca_omci_multi_gem_iwtp_t    *multiGem   = NULL;
    ca_omci_filter_stream_cfg_t  stream     = {0};

    OMCI_ENTER;

    meClass = ca_omci_me_get(OMCI_ME_GEM_NET_CTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        OMCIALDBG("no gemport is created now !");
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        gpnctp = (ca_omci_gem_net_ctp_t *)meInst;

        /* if gemport network ctp isn't connected , the flow is not complete*/
        OMCI_CHECK_PARAM_CNU(1 != gpnctp->meBase.linkedCount);

        link = meInst->linkedMeHeader;
        OMCI_CHECK_PARAM_CNU(NULL == link);
        if (OMCI_ME_GEM_IWTP_CLASS_ID == link->classId)
        {
            gemiwtp = (ca_omci_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            OMCI_CHECK_PARAM_CNU(NULL == gemiwtp);

            if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemiwtp->attr.iwOption)
            {
                macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));

                /* If mac bridge srv is null, the flow is not complete */
                OMCI_CHECK_PARAM_CNU(NULL == macbrg);
                OMCI_CHECK_PARAM_CNU(0 == macbrg->meBase.linkedCount);

                link = macbrg->meBase.linkedMeHeader;
                while (link)
                {
                    if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                    {
                        link = link->next;
                        continue;
                    }

                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                    link       = link->next;
                    OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);
                    if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                     && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                     && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                    {
                        continue;
                    }

                    OMCI_MEMSET(&stream, 0, sizeof(ca_omci_filter_stream_cfg_t));

                    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                    {
                        pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                        stream.portType = OMCI_CFG_STREAM_PORT_ETH;
                    }
                    if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                    {
                        iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == iphost);

                        stream.portType = OMCI_CFG_STREAM_PORT_IPHOST;
                    }
                    if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                    {
                        veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == veip);

                        stream.portType = OMCI_CFG_STREAM_PORT_VEIP;
                    }

                    stream.gemId     = gpnctp->attr.portId;
                    stream.uniPortId = brgportcfg->attr.tpPointer;
                    stream.direction = gpnctp->attr.direction;
                    ca_omci_filter_stream_tbl_add(&stream);
                }
                continue;
            }

            if (GEM_IWTP_IW_OPTION_8021P == gemiwtp->attr.iwOption)
            {
                ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));
                /* flow is not complete when dot1p is not exist nor linked */
                OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);
                OMCI_CHECK_PARAM_CNU(0 == ieee8021p->meBase.linkedCount);

                if (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == ieee8021p->attr.tpType)
                {
                    link = ieee8021p->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        /* Get the bridge for 802.1p, there is only one bridge for a 802.1p */
                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link       = link->next;
                        OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                        macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                        OMCI_CHECK_PARAM_CNU(NULL == macbrg);

                        if (macbrg->meBase.linkedCount)
                        {
                            linkbrgp = macbrg->meBase.linkedMeHeader;
                            while (linkbrgp)
                            {
                                /* find the mac port cfg data me only */
                                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                                {
                                    linkbrgp = linkbrgp->next;
                                    continue;
                                }

                                /* Get the bridge mode UNI side */
                                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                                linkbrgp   = linkbrgp->next;
                                OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                                if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                                {
                                    continue;
                                }

                                OMCI_MEMSET(&stream, 0, sizeof(ca_omci_filter_stream_cfg_t));

                                if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                                {
                                    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                                    OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                                    stream.portType = OMCI_CFG_STREAM_PORT_ETH;
                                }
                                if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                                {
                                    iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                                    OMCI_CHECK_PARAM_CNU(NULL == iphost);

                                    stream.portType = OMCI_CFG_STREAM_PORT_IPHOST;
                                }
                                if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                                {
                                    veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                                    OMCI_CHECK_PARAM_CNU(NULL == veip);

                                    stream.portType = OMCI_CFG_STREAM_PORT_VEIP;
                                }

                                stream.gemId     = gpnctp->attr.portId;
                                stream.uniPortId = brgportcfg->attr.tpPointer;
                                stream.direction = gpnctp->attr.direction;
                                ca_omci_filter_stream_tbl_add(&stream);
                            }
                        }
                    }
                    continue;
                }

                //other mode, support these type below now
                if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
                 && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
                 && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
                {
                    OMCIALDBG("Unsupport type %d need to add code", ieee8021p->attr.tpType);
                    continue;
                }

                OMCI_MEMSET(&stream, 0, sizeof(ca_omci_filter_stream_cfg_t));

                if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
                {
                    pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieee8021p->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                    stream.portType = OMCI_CFG_STREAM_PORT_ETH;
                }
                if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
                {
                    iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, ieee8021p->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == iphost);

                    stream.portType = OMCI_CFG_STREAM_PORT_IPHOST;
                }
                if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
                {
                    veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieee8021p->attr.tpPointer);
                    OMCI_CHECK_PARAM_CNU(NULL == veip);

                    stream.portType = OMCI_CFG_STREAM_PORT_VEIP;
                }

                stream.gemId     = gpnctp->attr.portId;
                stream.uniPortId = ieee8021p->attr.tpPointer;
                stream.direction = gpnctp->attr.direction;
                ca_omci_filter_stream_tbl_add(&stream);
                continue;
            }

            continue;
        }

        if (OMCI_ME_MULTI_GEM_IWTP_CLASS_ID == link->classId)
        {
            multiGem = (ca_omci_multi_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            OMCI_CHECK_PARAM_CNU(NULL == multiGem);
            OMCI_CHECK_PARAM_CNU(0 == multiGem->meBase.linkedCount);

            if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGem->attr.iwOption)
            {
                link = multiGem->meBase.linkedMeHeader;
                OMCI_CHECK_PARAM_CNU(NULL == link);
                while (link)
                {
                    /* find the mac port cfg data me only, it is the ANI side mac port */
                    if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                    {
                        link = link->next;
                        continue;
                    }

                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                    link       = link->next;
                    OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                    macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                    OMCI_CHECK_PARAM_CNU(NULL == macbrg);
                    OMCI_CHECK_PARAM_CNU(0 == macbrg->meBase.linkedCount);

                    linkbrgp = macbrg->meBase.linkedMeHeader;
                    while (linkbrgp)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                        {
                            linkbrgp = linkbrgp->next;
                            continue;
                        }

                        /* Get the bridge mode UNI side */
                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                        linkbrgp   = linkbrgp->next;
                        OMCI_CHECK_PARAM_CNU(NULL == brgportcfg);

                        if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                        {
                            continue;
                        }

                        OMCI_MEMSET(&stream, 0, sizeof(ca_omci_filter_stream_cfg_t));

                        if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                        {
                            pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                            stream.portType = OMCI_CFG_STREAM_PORT_ETH;
                        }
                        if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                        {
                            iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == iphost);

                            stream.portType = OMCI_CFG_STREAM_PORT_IPHOST;
                        }
                        if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                        {
                            veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, brgportcfg->attr.tpPointer);
                            OMCI_CHECK_PARAM_CNU(NULL == veip);

                            stream.portType = OMCI_CFG_STREAM_PORT_VEIP;
                        }

                        stream.gemId     = gpnctp->attr.portId;
                        stream.uniPortId = brgportcfg->attr.tpPointer;
                        stream.direction = gpnctp->attr.direction;
                        ca_omci_filter_stream_tbl_add(&stream);
                        continue;
                    }
                }
                continue;
            }

            if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGem->attr.iwOption)
            {
                link = multiGem->meBase.linkedMeHeader;
                OMCI_CHECK_PARAM_CNU(NULL == link);

                while (link)
                {
                    /* find the 802.1p me only */
                    if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID != link->classId)
                    {
                        link = link->next;
                        continue;
                    }

                    ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                    link      = link->next;
                    OMCI_CHECK_PARAM_CNU(NULL == ieee8021p);

                    /* for mutilcast downstream, 802.1p direct link to uni */
                    if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
                     && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
                     && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
                    {
                        continue;
                    }

                    OMCI_MEMSET(&stream, 0, sizeof(ca_omci_filter_stream_cfg_t));
                    if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
                    {
                        pptpEth = (ca_omci_pptp_eth_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_ETH_CLASS_ID, ieee8021p->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == pptpEth);

                        stream.portType = OMCI_CFG_STREAM_PORT_ETH;
                    }
                    if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
                    {
                        iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, ieee8021p->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == iphost);

                        stream.portType = OMCI_CFG_STREAM_PORT_IPHOST;
                    }
                    if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
                    {
                        veip = (ca_omci_veip_t *)ca_omci_me_entry_lookup(OMCI_ME_VEIP_CLASS_ID, ieee8021p->attr.tpPointer);
                        OMCI_CHECK_PARAM_CNU(NULL == veip);

                        stream.portType = OMCI_CFG_STREAM_PORT_VEIP;
                    }

                    stream.gemId     = gpnctp->attr.portId;
                    stream.uniPortId = ieee8021p->attr.tpPointer;
                    stream.direction = gpnctp->attr.direction;
                    ca_omci_filter_stream_tbl_add(&stream);
                    continue;
                }
                continue;
            }
            continue;
        }
    }
    OMCI_LEAVE;
}


//just need to go through the single tag rule and update the ds action
static void ca_omci_anaysis_stream_adjust(void)
{
    struct avl_traverser         avlTrav;
    ca_omci_stream_cfg_t        *stream = NULL;
    ca_omci_alcl_ds_tag_oper1_t *me     = NULL;
    ca_omci_uint8_t              idx    = 0;
    ca_omci_uint8_t              index  = 0;

    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.usFlow);
    while (NULL != stream)
    {
        //Go through all the stream to match the same uniport and vid;
        if ((OMCI_CFG_STREAM_PORT_VEIP != stream->portType)
         && (OMCI_CFG_STREAM_PORT_ETH != stream->portType))
        {
            stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            continue;
        }

        me = (ca_omci_alcl_ds_tag_oper1_t *)ca_omci_me_entry_lookup(OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID, stream->uniPortId);
        if (NULL != me)
        {
            for (idx = 0; idx < OMCI_ALCL_DS_TAG_OPER1_UNI_DEF_BEHAVIOR_TBL; idx++)
            {
                if (CA_OMCI_TRUE == me->attr.uniDefBehaviorTbl[idx].valid)
                {
                    //only use act rm and ptag
                    if ((ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_RM_OUTER_TAG == (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3))
                     || (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PRI_TAG == (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3)))
                    {
                        //update the single tag rule for it
                        for (index = 0; index < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; index++)
                        {
                            //only need to go through the single tag rules is enough now
                            if (me->attr.uniDefBehaviorTbl[idx].vid == stream->sglTagFilter[index].vlanFilter.vid)
                            {
                                stream->sglTagFilter[index].dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                            }
                        }
                    }

                    //PVID rule only need to merge pass tag and pri tag
                    if ((ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PASS_TAG == (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3))
                     || (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PRI_TAG == (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3)))
                    {
                        //check untag rules too for PVID
                        if (0 != stream->untagFilterIpoe.innerVlanTreatment.vid)
                        {
                            stream->untagFilterIpoe.dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                        }
                        if (0 != stream->untagFilterIpoev6.innerVlanTreatment.vid)
                        {
                            stream->untagFilterIpoev6.dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                        }
                        if (0 != stream->untagFilterPppoe.innerVlanTreatment.vid)
                        {
                            stream->untagFilterPppoe.dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                        }
                        if (0 != stream->untagFilterArp.innerVlanTreatment.vid)
                        {
                            stream->untagFilterArp.dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                        }
                        if (0 != stream->untagFilterDef.innerVlanTreatment.vid)
                        {
                            stream->untagFilterDef.dsTagAction = (me->attr.uniDefBehaviorTbl[idx].vlanAct & 0x3);
                        }
                    }
                }
            }
        }

        stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
    }
}
#endif


#if DESCRIPTION("Tcont and traffic schedule functions")
static void ca_omci_analysis_tcont(void)
{
    ca_omci_meclass_t       *meClass    = NULL;
    ca_omci_mebase_t        *meInst     = NULL;
    ca_omci_me_link_t       *link       = NULL;
    ca_omci_tcont_t         *tcont      = NULL;
    ca_omci_pri_queue_t     *priQ       = NULL;
    ca_omci_traffic_scheg_t *trafficSch = NULL;
    ca_omci_onuinfo_obj_t   *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_tcont_cfg_t      tcCfg      = {0};
    ca_omci_uint32_t         priQIdx    = 0;

    OMCI_ENTER;
    meClass = ca_omci_me_get(OMCI_ME_TCONT_CLASS_ID);

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        tcont = (ca_omci_tcont_t *)meInst;

        //if alloc id is not configure, this tcont is not used
        OMCI_CHECK_PARAM_CNU((0 == tcont->attr.allocId) || (0xffff == tcont->attr.allocId) || (0xff == tcont->attr.allocId));

        //for now, there is no traffic scheduling, Strict priority or WRR, so can't set SP+WRR
        OMCI_MEMSET(&tcCfg, 0, sizeof(ca_omci_tcont_cfg_t));
        tcCfg.tcontId = tcont->meBase.instanceId;
        tcCfg.allocId = tcont->attr.allocId;
        tcCfg.policy  = tcont->attr.policy;

        link = tcont->meBase.linkedMeHeader;
        while (link)
        {
            /* find the priority queue me */
            if (OMCI_ME_PRI_QUEUE_CLASS_ID == link->classId)
            {
                priQ = (ca_omci_pri_queue_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                OMCI_CHECK_PARAM_CNU(NULL == priQ);

                priQIdx = (onuinfo->maxQueuePerTc - 1) - (priQ->meBase.instanceId & 0x7fff) % onuinfo->maxQueuePerTc;
                tcCfg.wrrQue[priQIdx] = (0 != priQ->attr.weight) ? priQ->attr.weight : 1;
                link = link->next;
                continue;
            }

            /* find the traffic schedule me */
            if (OMCI_ME_TRAFFIC_SCHEG_CLASS_ID == link->classId)
            {
                trafficSch = (ca_omci_traffic_scheg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                OMCI_CHECK_PARAM_CNU(NULL == trafficSch);

                tcCfg.schedId     = trafficSch->meBase.instanceId;
                tcCfg.schedPoint  = trafficSch->attr.trafSchedPoint;
                tcCfg.schedWeight = trafficSch->attr.priWeight;
                tcCfg.schedPolicy = trafficSch->attr.policy;
                link = link->next;
                continue;
            }

            // if SP+WRR exist, need to get all pri queue MEs traffic schedule ME;
            //TODO:
            link = link->next;
        }

        OMCIALDBG("cfg tcont to hal");
        ca_omci_tcont_cfg_tbl_add(&tcCfg);
    }
}
#endif


#if DESCRIPTION("Other function for analysis")
static void ca_omci_analysis_tr69(void)
{
    ca_omci_meclass_t         *meClass  = NULL;
    ca_omci_mebase_t          *meInst   = NULL;
    ca_omci_veip_t            *veip     = NULL;
    ca_omci_tcp_udp_cfg_t     *tcpUdp   = NULL;
    ca_omci_bbrtr69_mng_srv_t *bbrTr69  = NULL;
    ca_omci_ip_host_t         *iphost   = NULL;
    ca_omci_net_addr_t        *netAddr  = NULL;
    ca_omci_auth_scy_t        *authScy  = NULL;
    ca_omci_large_string_t    *largeStr = NULL;
    ca_omci_uint8_t            idx      = 0;
    ca_omci_large_str_part_t   part[15];

    OMCI_ENTER;
    OMCI_MEMSET(part, 0, sizeof(ca_omci_large_str_part_t) * 15);

    meClass = ca_omci_me_get(OMCI_ME_VEIP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        veip    = (ca_omci_veip_t *)meInst;
        bbrTr69 = (ca_omci_bbrtr69_mng_srv_t *)ca_omci_me_entry_lookup(OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID, veip->meBase.instanceId);
        if ((NULL != bbrTr69)
         && (CA_OMCI_UNLOCK == bbrTr69->attr.admin)
         && (CA_OMCI_UNLOCK == veip->attr.admin))
        {
            //only analysis this veip, and this kind veip should only have one!!!
            tcpUdp = (ca_omci_tcp_udp_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_TCP_UDP_CFG_CLASS_ID, veip->attr.tcpUdpPointer);
            if (NULL != tcpUdp)
            {
                iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, tcpUdp->attr.iphostPoint);
                if (NULL != iphost)
                {
                    sg_omciAnalyseFlowTbl.acs.localIp = iphost->attr.ipAddr;
                }
            }

            netAddr = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, bbrTr69->attr.acsNetAddr);
            if (NULL != netAddr)
            {
                authScy  = (ca_omci_auth_scy_t *)ca_omci_me_entry_lookup(OMCI_ME_AUTH_SCY_CLASS_ID, netAddr->attr.securityPoint);
                largeStr = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, netAddr->attr.addrPoint);

                if (NULL != authScy)
                {
                    OMCI_MEMCPY(sg_omciAnalyseFlowTbl.acs.usrname, authScy->attr.usrName1, OMCI_ACS_USERNAME_LEN);
                    OMCI_MEMCPY(sg_omciAnalyseFlowTbl.acs.passwd,  authScy->attr.pwd,      OMCI_ACS_PASSWD_LEN);
                }

                if (NULL != largeStr)
                {
                    OMCI_MEMCPY(part[0].part, largeStr->attr.part1, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[1].part, largeStr->attr.part2, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[2].part, largeStr->attr.part3, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[3].part, largeStr->attr.part4, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[4].part, largeStr->attr.part5, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[5].part, largeStr->attr.part6, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[6].part, largeStr->attr.part7, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[7].part, largeStr->attr.part8, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[8].part, largeStr->attr.part9, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[9].part, largeStr->attr.part10, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[10].part, largeStr->attr.part11, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[11].part, largeStr->attr.part12, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[12].part, largeStr->attr.part13, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[13].part, largeStr->attr.part14, OMCI_LARGE_STR_LEN);
                    OMCI_MEMCPY(part[14].part, largeStr->attr.part15, OMCI_LARGE_STR_LEN);

                    if (OMCI_LARGE_STR_LEN * largeStr->attr.partNum > OMCI_ACS_URL_LEN)
                    {
                        OMCIERR("OMCI cfg acs url beyond 128 bytes!!!");
                        return;
                    }

                    for (idx = 0; idx < largeStr->attr.partNum; idx++)
                    {
                        OMCI_MEMCPY((sg_omciAnalyseFlowTbl.acs.url + (OMCI_LARGE_STR_LEN * idx)), part[idx].part, OMCI_LARGE_STR_LEN);
                    }
                }
            }

            break;
        }
    }

    OMCI_LEAVE;
}


static void ca_omci_analysis_voip_link(void)
{
    ca_omci_me_voip_cfg_link();
    ca_omci_me_tcp_udp_cfg_link();
    ca_omci_me_mgc_cfg_data_link();
    ca_omci_me_net_addr_link();
    ca_omci_me_voip_ctp_link();
    ca_omci_me_voip_media_prof_link();
    ca_omci_me_voice_srv_prof_link();
    ca_omci_me_rtp_prof_data_link();
    ca_omci_me_sip_usr_link();
    ca_omci_me_sip_agt_cfg_link();
}


static void ca_omci_analysis_non_omci_voip(void)
{
    ca_omci_meclass_t        *meClass  = NULL;
    ca_omci_mebase_t         *meInst   = NULL;
    ca_omci_voip_cfg_t       *voipCfg  = NULL;
    ca_omci_net_addr_t       *netAddr  = NULL;
    ca_omci_auth_scy_t       *authScy  = NULL;
    ca_omci_large_string_t   *largeStr = NULL;
    ca_omci_uint8_t          idx       = 0;
    ca_omci_large_str_part_t part[15];

    meClass = ca_omci_me_get(OMCI_ME_VOIP_CFG_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        voipCfg  = (ca_omci_voip_cfg_t *)meInst;

        netAddr  = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, voipCfg->attr.voipAddrPoint);
        if (NULL != netAddr)
        {
            authScy  = (ca_omci_auth_scy_t *)ca_omci_me_entry_lookup(OMCI_ME_AUTH_SCY_CLASS_ID, netAddr->attr.securityPoint);
            largeStr = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, netAddr->attr.addrPoint);

            if (NULL != authScy)
            {
                OMCI_MEMCPY(sg_omciAnalyseFlowTbl.nonOmciVoip.usrname, authScy->attr.usrName1, OMCI_ACS_USERNAME_LEN);
                OMCI_MEMCPY(sg_omciAnalyseFlowTbl.nonOmciVoip.passwd,  authScy->attr.pwd,      OMCI_ACS_PASSWD_LEN);
            }

            if (NULL != largeStr)
            {
                OMCI_MEMCPY(part[0].part, largeStr->attr.part1, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[1].part, largeStr->attr.part2, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[2].part, largeStr->attr.part3, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[3].part, largeStr->attr.part4, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[4].part, largeStr->attr.part5, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[5].part, largeStr->attr.part6, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[6].part, largeStr->attr.part7, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[7].part, largeStr->attr.part8, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[8].part, largeStr->attr.part9, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[9].part, largeStr->attr.part10, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[10].part, largeStr->attr.part11, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[11].part, largeStr->attr.part12, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[12].part, largeStr->attr.part13, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[13].part, largeStr->attr.part14, OMCI_LARGE_STR_LEN);
                OMCI_MEMCPY(part[14].part, largeStr->attr.part15, OMCI_LARGE_STR_LEN);

                if (OMCI_LARGE_STR_LEN * largeStr->attr.partNum > OMCI_ACS_URL_LEN)
                {
                    OMCIERR("OMCI cfg acs url beyond 128 bytes!!!");
                    return;
                }

                for (idx = 0; idx < largeStr->attr.partNum; idx++)
                {
                    OMCI_MEMCPY((sg_omciAnalyseFlowTbl.nonOmciVoip.url + (OMCI_LARGE_STR_LEN * idx)), part[idx].part, OMCI_LARGE_STR_LEN);
                }
            }
        }
    }
}


static void ca_omci_analysis_h248_voip(void)
{
    ca_omci_meclass_t        *meClass  = NULL;
    ca_omci_mebase_t         *meInst   = NULL;
    ca_omci_mgc_cfg_data_t   *mgcCfg   = NULL;
    ca_omci_net_addr_t       *priNet   = NULL;
    ca_omci_net_addr_t       *sndNet   = NULL;
    ca_omci_tcp_udp_cfg_t    *tcpUdp   = NULL;
    ca_omci_large_string_t   *msgId    = NULL;
    ca_omci_large_string_t   *largeStr = NULL;
    ca_omci_ip_host_t        *iphost   = NULL;
    ca_omci_me_link_t        *link     = NULL;
    ca_omci_voip_ctp_t       *voipCtp  = NULL;
    ca_omci_pptp_pots_t      *pots     = NULL;
    ca_omci_onuinfo_obj_t    *pObj     = ca_omci_onuinfo_get();
    ca_omci_uint8_t           idx      = 0;
    ca_omci_uint8_t           portIdx  = 0;
    ca_omci_large_str_part_t  part[15];

    meClass = ca_omci_me_get(OMCI_ME_MGC_CFG_DATA_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    if (meClass->instHash->count != 1)
    {
        OMCIALERR("More than 1 mgc config data create now, not support in this field");
        return;
    }

    OMCI_MEMSET(&sg_omciAnalyseFlowTbl.h248Voip, 0, sizeof(ca_omci_h248_voip_t));
    meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
    if (NULL != meInst)
    {
        mgcCfg = (ca_omci_mgc_cfg_data_t *)meInst;

        priNet = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID,     mgcCfg->attr.priMgc);
        sndNet = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID,     mgcCfg->attr.sndMgc);
        tcpUdp = (ca_omci_tcp_udp_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_TCP_UDP_CFG_CLASS_ID,  mgcCfg->attr.tcpUdpPointer);
        msgId  = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, mgcCfg->attr.msgIdPointer);
        OMCI_PARAM_IS_NULL(priNet);
        OMCI_PARAM_IS_NULL(sndNet);
        OMCI_PARAM_IS_NULL(tcpUdp);
        OMCI_PARAM_IS_NULL(msgId);

        //get all H248 configure
        sg_omciAnalyseFlowTbl.h248Voip.version        = mgcCfg->attr.version;
        sg_omciAnalyseFlowTbl.h248Voip.msgFormat      = mgcCfg->attr.msgFormat;//this value should match the port in mgc config(H248 stack should check)
        sg_omciAnalyseFlowTbl.h248Voip.maxRetryTime   = mgcCfg->attr.maxRetryTime;
        sg_omciAnalyseFlowTbl.h248Voip.maxRetryAttem  = mgcCfg->attr.maxRetryAtte;
        sg_omciAnalyseFlowTbl.h248Voip.srvChangeDelay = mgcCfg->attr.srvChDelay;
        sg_omciAnalyseFlowTbl.h248Voip.softswitch     = mgcCfg->attr.softswitch;
        OMCI_MEMCPY(sg_omciAnalyseFlowTbl.h248Voip.termIdBase, mgcCfg->attr.termIdBase, OMCI_H248_TERM_ID_BASE_LEN);

        //get Primary MGC first
        largeStr = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, priNet->attr.addrPoint);
        if (NULL != largeStr)
        {
            OMCI_MEMSET(part, 0 , (sizeof(ca_omci_large_str_part_t) * 15));
            OMCI_MEMCPY(part[0].part, largeStr->attr.part1, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[1].part, largeStr->attr.part2, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[2].part, largeStr->attr.part3, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[3].part, largeStr->attr.part4, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[4].part, largeStr->attr.part5, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[5].part, largeStr->attr.part6, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[6].part, largeStr->attr.part7, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[7].part, largeStr->attr.part8, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[8].part, largeStr->attr.part9, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[9].part, largeStr->attr.part10, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[10].part, largeStr->attr.part11, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[11].part, largeStr->attr.part12, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[12].part, largeStr->attr.part13, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[13].part, largeStr->attr.part14, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[14].part, largeStr->attr.part15, OMCI_LARGE_STR_LEN);

            if (OMCI_LARGE_STR_LEN * largeStr->attr.partNum > OMCI_H248_MGC_LEN)
            {
                OMCIERR("OMCI cfg mgc beyond 128 bytes!!!");
                return;
            }

            for (idx = 0; idx < largeStr->attr.partNum; idx++)
            {
                OMCI_MEMCPY((sg_omciAnalyseFlowTbl.h248Voip.priMgc + (OMCI_LARGE_STR_LEN * idx)), part[idx].part, OMCI_LARGE_STR_LEN);
            }
        }

        //get Secondary MGC
        largeStr = (ca_omci_large_string_t *)ca_omci_me_entry_lookup(OMCI_ME_LARGE_STRING_CLASS_ID, sndNet->attr.addrPoint);
        if (NULL != largeStr)
        {
            OMCI_MEMSET(part, 0 , (sizeof(ca_omci_large_str_part_t) * 15));
            OMCI_MEMCPY(part[0].part, largeStr->attr.part1, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[1].part, largeStr->attr.part2, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[2].part, largeStr->attr.part3, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[3].part, largeStr->attr.part4, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[4].part, largeStr->attr.part5, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[5].part, largeStr->attr.part6, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[6].part, largeStr->attr.part7, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[7].part, largeStr->attr.part8, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[8].part, largeStr->attr.part9, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[9].part, largeStr->attr.part10, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[10].part, largeStr->attr.part11, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[11].part, largeStr->attr.part12, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[12].part, largeStr->attr.part13, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[13].part, largeStr->attr.part14, OMCI_LARGE_STR_LEN);
            OMCI_MEMCPY(part[14].part, largeStr->attr.part15, OMCI_LARGE_STR_LEN);

            if (OMCI_LARGE_STR_LEN * largeStr->attr.partNum > OMCI_H248_MGC_LEN)
            {
                OMCIERR("OMCI cfg mgc beyond 128 bytes!!!");
                return;
            }

            for (idx = 0; idx < largeStr->attr.partNum; idx++)
            {
                OMCI_MEMCPY((sg_omciAnalyseFlowTbl.h248Voip.sndMgc + (OMCI_LARGE_STR_LEN * idx)), part[idx].part, OMCI_LARGE_STR_LEN);
            }
        }

        //get Message  ID
        OMCI_MEMSET(part, 0 , (sizeof(ca_omci_large_str_part_t) * 15));
        OMCI_MEMCPY(part[0].part, msgId->attr.part1, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[1].part, msgId->attr.part2, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[2].part, msgId->attr.part3, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[3].part, msgId->attr.part4, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[4].part, msgId->attr.part5, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[5].part, msgId->attr.part6, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[6].part, msgId->attr.part7, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[7].part, msgId->attr.part8, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[8].part, msgId->attr.part9, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[9].part, msgId->attr.part10, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[10].part, msgId->attr.part11, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[11].part, msgId->attr.part12, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[12].part, msgId->attr.part13, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[13].part, msgId->attr.part14, OMCI_LARGE_STR_LEN);
        OMCI_MEMCPY(part[14].part, msgId->attr.part15, OMCI_LARGE_STR_LEN);

        if (OMCI_LARGE_STR_LEN * msgId->attr.partNum > OMCI_H248_MSG_ID_LEN)
        {
            OMCIERR("OMCI cfg mgc msg id beyond 128 bytes!!!");
            return;
        }

        for (idx = 0; idx < msgId->attr.partNum; idx++)
        {
            OMCI_MEMCPY((sg_omciAnalyseFlowTbl.h248Voip.msgId + (OMCI_LARGE_STR_LEN * idx)), part[idx].part, OMCI_LARGE_STR_LEN);
        }

        //Get the iphost configure for H248
        iphost = (ca_omci_ip_host_t *)ca_omci_me_entry_lookup(OMCI_ME_IP_HOST_CLASS_ID, tcpUdp->attr.iphostPoint);
        OMCI_PARAM_IS_NULL(iphost);
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.ipOpt   = iphost->attr.ipOption;
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.ipAddr  = iphost->attr.ipAddr;
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.mask    = iphost->attr.mask;
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.gateway = iphost->attr.gateway;
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.priDns  = iphost->attr.priDns;
        sg_omciAnalyseFlowTbl.h248Voip.voipIphost.secDns  = iphost->attr.sndDns;

        //Get the Pots Number Map
        link = mgcCfg->meBase.linkedMeHeader;
        while (link)
        {
            /* find the voip voice CTP only to find the pots */
            if (OMCI_ME_VOIP_CTP_CLASS_ID == link->classId)
            {
                voipCtp = (ca_omci_voip_ctp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                if (NULL != voipCtp)
                {
                    pots = (ca_omci_pptp_pots_t *)ca_omci_me_entry_lookup(OMCI_ME_PPTP_POTS_CLASS_ID, voipCtp->attr.pptpPointer);
                    if (NULL == pots)
                    {
                        OMCIALERR("No pots link to MGC !");
                        break;
                    }

                    portIdx = (((voipCtp->attr.pptpPointer & 0xff00) >> 8) - pObj->potsSlot) * 8 + (voipCtp->attr.pptpPointer & 0xff);
                    sg_omciAnalyseFlowTbl.h248Voip.potsId = sg_omciAnalyseFlowTbl.h248Voip.potsId | (1 << (portIdx - 1));
                }
            }
            link = link->next;
        }
    }
}


static void ca_omci_analysis_sip(void)
{
//TODO: add when debug with OLT
}


static void ca_omci_analysis_voip(void)
{
    ca_omci_voip_cfg_t *voipCfg = NULL;

    OMCI_ENTER;

    voipCfg = (ca_omci_voip_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_CFG_CLASS_ID, 0);
    if (voipCfg == NULL)
    {
        return;
    }

    // go through me to link all
    ca_omci_analysis_voip_link();

    if ((VOIP_CFG_USED_PROTO_NONE == voipCfg->attr.sigProtoUsed)
     || (VOIP_CFG_USED_PROTO_NON_OMCI == voipCfg->attr.sigProtoUsed))
    {
        ca_omci_analysis_non_omci_voip();
    }

    if (VOIP_CFG_USED_PROTO_H248 == voipCfg->attr.sigProtoUsed)
    {
        ca_omci_analysis_h248_voip();
    }

    if (VOIP_CFG_USED_PROTO_SIP == voipCfg->attr.sigProtoUsed)
    {
        ca_omci_analysis_sip();
    }
}
#endif

#if DESCRIPTION("Main entrance function for analysis")
static void ca_omci_analysis_flow_link(void)
{
    ca_omci_me_gem_net_ctp_link();
    ca_omci_me_gem_iwtp_link();
    ca_omci_me_ieee_dot1p_srv_link();
    ca_omci_me_multi_gem_iwtp_link();
    ca_omci_me_mac_port_cfg_link();
    ca_omci_me_traffic_scheg_link();
    ca_omci_me_ext_vlan_tag_op_link();
    ca_omci_me_veip_link();
    ca_omci_me_bbrtr69_mng_srv_link();
    ca_omci_me_tcp_udp_cfg_link();
    ca_omci_me_net_addr_link();
}


ca_omci_st_e ca_omci_analysis_flow(void)
{
    ca_omci_meclass_t     *meClass = NULL;
    ca_omci_mebase_t      *meInst  = NULL;
    ca_omci_me_link_t     *link    = NULL;
    ca_omci_gem_net_ctp_t *gpnctp  = NULL;
    ca_omci_bool_t         cfgFlag = CA_OMCI_FALSE;

    OMCI_ENTER;
    OMCI_MEMSET(&sg_omciAnalyseFlowTbl, 0, sizeof(ca_omci_analyse_flow_t));
    ca_omci_analysis_flow_tbl_init(&sg_omciAnalyseFlowTbl.flow);

    // go through me to link all
    ca_omci_analysis_flow_link();

    meClass = ca_omci_me_get(OMCI_ME_GEM_NET_CTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL) || (0 == meClass->instHash->count))
    {
        OMCIALDBG("no gemport is created now !");
        return CA_OMCI_OK;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        gpnctp = (ca_omci_gem_net_ctp_t *)meInst;

        /* if gemport network ctp isn't connected , the flow is not complete*/
        OMCI_CHECK_PARAM_CNU(1 != gpnctp->meBase.linkedCount);

        link = meInst->linkedMeHeader;
        OMCI_CHECK_PARAM_CNU(NULL == link);

        if (OMCI_ME_GEM_IWTP_CLASS_ID == link->classId)
        {
            //any unicast is complete, the flag should be true
            if (CA_OMCI_TRUE == ca_omci_analysis_flow_unicast(meInst))
            {
                cfgFlag = CA_OMCI_TRUE;
            }
            continue;
        }

        if (OMCI_ME_MULTI_GEM_IWTP_CLASS_ID == link->classId)
        {
            //any multicast is complete, the flag should be true
            if (CA_OMCI_TRUE == ca_omci_analysis_flow_mcast(meInst))
            {
                cfgFlag = CA_OMCI_TRUE;
            }
            continue;
        }
    }

    //cfg tcont first, and seprate from gemflow. Because maybe muti-gem link to one tcont
    if (CA_OMCI_TRUE != cfgFlag)
    {
        OMCIALERR("Flow is not complete");
        return CA_OMCI_OK;
    }

    //cfg tcont first, and seprate from gemflow. Because maybe muti-gem link to one tcont
    ca_omci_analysis_tcont();
    ca_omci_analysis_tr69();

    //when finish analysis flow, analysis stream now
    OMCIALDBG("Analysis vlan filter stream first !");
    ca_omci_analysis_filter_stream();

    meClass = ca_omci_me_get(OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID);
    if ((meClass != NULL) && (meClass->instHash == NULL))
    {
        OMCIALDBG("No extern vlan tag Operation created !");
        return CA_OMCI_OK;
    }

    OMCIALDBG("Analysis stream with extern vlan tag operation ME !");
    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        ca_omci_analysis_stream(meInst);
    }

    ca_omci_anaysis_stream_adjust();
    OMCI_LEAVE;
    return CA_OMCI_OK;
}


void ca_omci_cfg_timeout_handle(void *pData)
{
    (void)pData;

    OMCI_ENTER;

    ca_omci_cfg_timer_stop();

    // no matter which bit is on, if OLT cfg is not finished, should wait it
    if (CA_OSAL_OK != ca_sem_wait(g_omciAnalysisSem, 0))
    {
        // still analysis now
        OMCIALDBG("Can't get analysis sem");
        ca_omci_update_flow(OMCI_CFG_FLOW_RETRY);
        return;
    }

    // remove cfg for hal and reset the flag when omcc link down
    if (sg_omciUpdateFlowFlag & OMCI_CFG_FLOW_OMCC_DOWN)
    {
        ca_sem_post(g_omciCfgSem);

        //receive link up right after receive link down
        if (sg_omciUpdateFlowFlag & OMCI_CFG_FLOW_UPDATE)
        {
            OMCIALDBG("Analysis after handler link down");
            sg_omciUpdateFlowFlag = sg_omciUpdateFlowFlag & 0xD;
            ca_omci_update_flow(OMCI_CFG_FLOW_RETRY);
            return;
        }

        sg_omciUpdateFlowFlag = 0;
        return;
    }

    //If the flag is voip, voice and data flow may configure at the same time, shouldn't return
    if (sg_omciUpdateFlowFlag & OMCI_CFG_FLOW_UPDATE_VOIP)
    {
        ca_omci_analysis_voip();
    }

    //default is data flow, analysis flow now, then cfg it to hal
    ca_omci_analysis_flow();
    ca_sem_post(g_omciCfgSem);
    sg_omciUpdateFlowFlag = 0;

    OMCI_LEAVE;
}


void ca_omci_cfg_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_CFG;

    (void)data;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_timer_def_e);//no pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, msg.hdr.dataLen);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


void ca_omci_cfg_onu_shut_down(void)
{
    sg_omciUpdateFlowFlag |= OMCI_CFG_FLOW_OMCC_DOWN;
    ca_omci_cfg_timeout(NULL);
}


//just save the flag, then start the cfg timer
void ca_omci_update_flow(ca_omci_uint32_t flag)
{
    OMCI_ENTER;
    sg_omciUpdateFlowFlag = flag;
    ca_omci_cfg_timer_start();
}
#endif


#if DESCRIPTION("Function for cfg to hal")
void ca_omci_analysis_tcont_set_all(void)
{
    struct avl_traverser avlTrav;
    ca_omci_tcont_cfg_t *tcCfg   = NULL;
    ca_omci_uint8_t      ponPort = 1;

    OMCI_ENTER;

    tcCfg = (ca_omci_tcont_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.tcCfg);
    if (NULL == tcCfg)
    {
        OMCIALWARN("No tcont cfg now, retry later");
        return;
    }

    while (NULL != tcCfg)
    {
        // set all tcont cfg then to save them
        ca_omci_hal_tcont_set(&ponPort, (void *)tcCfg);
        ca_omci_tcont_cfg_tbl_save(tcCfg);
        tcCfg = (ca_omci_tcont_cfg_t *)omci_avl_t_next(&avlTrav);
    }
}


void ca_omci_analysis_tcont_reset(void)
{
    struct avl_traverser avlTrav;
    ca_omci_tcont_cfg_t *tcCfg    = NULL;
    ca_omci_tcont_cfg_t *tmpTcCfg = NULL;
    ca_omci_tcont_cfg_t *pTcont   = NULL;
    ca_omci_uint8_t      ponPort  = 1;

    OMCI_ENTER;
    if (NULL == sg_omciAnalyseFlowTbl.flow.tcCfg)
    {
        tcCfg = (ca_omci_tcont_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.tcCfg);
        while (NULL != tcCfg)
        {
            //remove all tcont val;
            OMCIALDBG("tcont reset all");
            tmpTcCfg = tcCfg;
            tcCfg    = (ca_omci_tcont_cfg_t *)omci_avl_t_next(&avlTrav);
            ca_omci_hal_tcont_reset(&ponPort, (void *)tmpTcCfg);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.tcCfg, tmpTcCfg);
        }
        return;
    }

    tcCfg = (ca_omci_tcont_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.tcCfg);
    if (NULL == tcCfg)
    {
        return;
    }

    while (NULL != tcCfg)
    {
        // get the tcont in analysis table
        pTcont   = (ca_omci_tcont_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.tcCfg, tcCfg);
        tmpTcCfg = tcCfg;
        tcCfg    = (ca_omci_tcont_cfg_t *)omci_avl_t_next(&avlTrav);

        //if this tcont can't be found in analysis table, means that this tcont is reset by OLT
        if (NULL == pTcont)
        {
            OMCIALDBG("tcont reset");
            ca_omci_hal_tcont_reset(&ponPort, (void *)tmpTcCfg);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.tcCfg, tmpTcCfg);
            continue;
        }
    }
}


void ca_omci_analysis_tcont_cfg(void)
{
    struct avl_traverser avlTrav;
    struct avl_traverser anaAvlTrav;
    ca_omci_tcont_cfg_t *tcCfg    = NULL;
    ca_omci_tcont_cfg_t *tmpTcCfg = NULL;
    ca_omci_tcont_cfg_t *pTcont   = NULL;
    ca_omci_tcont_cfg_t *tcAnaly  = NULL;
    ca_omci_uint8_t      ponPort  = 1;

    OMCI_ENTER;
    if (NULL == sg_omciAnalyseFlowTbl.flow.tcCfg)
    {
        //tcont reset should be at last, so don't reset here
        return;
    }

    tcCfg = (ca_omci_tcont_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.tcCfg);
    if (NULL == tcCfg)
    {
        //never cfg flow before, just cfg flow and save configure;
        ca_omci_analysis_tcont_set_all();
        return;
    }

    while (NULL != tcCfg)
    {
        // get the tcont in analysis table
        pTcont   = (ca_omci_tcont_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.tcCfg, tcCfg);
        tmpTcCfg = tcCfg;
        tcCfg    = (ca_omci_tcont_cfg_t *)omci_avl_t_next(&avlTrav);

        //if this tcont can't be found in analysis table, means that this tcont is reset by OLT
        if (NULL == pTcont)
        {
            //tcont reset should be at last, so don't reset here
            continue;
        }

        //compare the analysis table and the saved cfg table, when tcont changes,set new value
        if (CA_OMCI_TRUE == ca_omci_tcont_change(pTcont, tmpTcCfg))
        {
            OMCIALDBG("tcont modify");
            if (tmpTcCfg->allocId == pTcont->allocId)
                ca_omci_hal_tcont_reset(&ponPort, (void *)tmpTcCfg);

            ca_omci_hal_tcont_set(&ponPort, (void *)pTcont);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.tcCfg, tmpTcCfg);

            ca_omci_tcont_cfg_tbl_save(pTcont);
            pTcont->cfgFlag = CA_OMCI_ANALYSIS_CFG_MODIFY;
            continue;
        }

        // flow is complete same, no need to change
        OMCIALDBG("tcont same, no need to change");
        pTcont->cfgFlag = CA_OMCI_ANALYSIS_CFG_REMAIN;
        ca_omci_hal_tcont_restore(&ponPort, (void *)pTcont);
    }

    //after go through cfg table, then go through analysis table to set new tcont
    tcAnaly = (ca_omci_tcont_cfg_t *)omci_avl_t_first(&anaAvlTrav, sg_omciAnalyseFlowTbl.flow.tcCfg);
    while (NULL != tcAnaly)
    {
        if (CA_OMCI_ANALYSIS_CFG_INIT == tcAnaly->cfgFlag)
        {
            ca_omci_hal_tcont_set(&ponPort, (void *)tcAnaly);
            ca_omci_tcont_cfg_tbl_save(tcAnaly);
        }

        tcAnaly = (ca_omci_tcont_cfg_t *)omci_avl_t_next(&anaAvlTrav);
    }
}


void ca_omci_analysis_flow_add_all(void)
{
    struct avl_traverser avlTrav;
    ca_omci_gem_flow_t  *flowCfg = NULL;

    OMCI_ENTER;

    flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.gemFlow);
    if (NULL == flowCfg)
    {
        OMCIALWARN("No flow analysis now, retry later");
        return;
    }

    while (NULL != flowCfg)
    {
        // get all flow cfg then to cfg them
        ca_omci_hal_gemflow_add(flowCfg);
        ca_omci_gemflow_tbl_cfg_save(flowCfg);
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
    }
}


void ca_omci_analysis_flow_cfg(void)
{
    struct avl_traverser avlTrav;
    struct avl_traverser anaAvlTrav;
    ca_omci_gem_flow_t  *flowCfg    = NULL;
    ca_omci_gem_flow_t  *flowAnaly  = NULL;

    OMCI_ENTER;

    if (NULL == sg_omciAnalyseFlowTbl.flow.gemFlow)
    {
        //remove gemport after remove classifier
        return;
    }

    flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.gemFlow);
    if (NULL == flowCfg)
    {
        //never cfg flow before, just cfg flow and save configure;
        ca_omci_analysis_flow_add_all();
        return;
    }

    //after go through cfg table, then go through analysis table to add new flow
    flowAnaly = (ca_omci_gem_flow_t *)omci_avl_t_first(&anaAvlTrav, sg_omciAnalyseFlowTbl.flow.gemFlow);
    while (NULL != flowAnaly)
    {
        if (CA_OMCI_GEMFLOW_CFG_INIT == flowAnaly->cfgFlag)
        {
            OMCIALDBG("add new gemflow");
            ca_omci_hal_gemflow_add(flowAnaly);
            ca_omci_gemflow_tbl_cfg_save(flowAnaly);
        }

        if (CA_OMCI_GEMFLOW_CFG_MODIFY == flowAnaly->cfgFlag)
        {
            OMCIALDBG("gemflow modify, add the new one");
            ca_omci_hal_gemflow_add(flowAnaly);
            ca_omci_gemflow_tbl_cfg_save(flowAnaly);
        }

        flowAnaly = (ca_omci_gem_flow_t *)omci_avl_t_next(&anaAvlTrav);
    }
}


void ca_omci_analysis_flow_remove(void)
{
    struct avl_traverser avlTrav;
    ca_omci_gem_flow_t  *flowCfg    = NULL;
    ca_omci_gem_flow_t  *tmpFlowCfg = NULL;
    ca_omci_gem_flow_t  *pFlow      = NULL;

    OMCI_ENTER;

    if (NULL == sg_omciAnalyseFlowTbl.flow.gemFlow)
    {
        flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.gemFlow);
        while (NULL != flowCfg)
        {
            //remove all flow;
            OMCIALDBG("gemflow remove all");
            tmpFlowCfg = flowCfg;
            flowCfg    = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);
            ca_omci_hal_gemflow_del(tmpFlowCfg);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.gemFlow, tmpFlowCfg);
        }
        return;
    }

    flowCfg = (ca_omci_gem_flow_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.gemFlow);
    if (NULL == flowCfg)
    {
        return;
    }

    while (NULL != flowCfg)
    {
        // get the flow in analysis table
        pFlow      = (ca_omci_gem_flow_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.gemFlow, flowCfg);
        tmpFlowCfg = flowCfg;
        flowCfg    = (ca_omci_gem_flow_t *)omci_avl_t_next(&avlTrav);

        //if this flow can't be found in analysis table, means that this flow is removed by OLT
        if (NULL == pFlow)
        {
            OMCIALDBG("gemflow remove");
            ca_omci_hal_gemflow_del(tmpFlowCfg);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.gemFlow, tmpFlowCfg);
            continue;
        }

        //compare the analysis table and the saved cfg table, when flow changes, remove old flow
        if (CA_OMCI_TRUE == ca_omci_gemflow_change(pFlow, tmpFlowCfg))
        {
            OMCIALDBG("gemflow modify, remove the old one");
            ca_omci_hal_gemflow_del(tmpFlowCfg);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.gemFlow, tmpFlowCfg);
            pFlow->cfgFlag = CA_OMCI_GEMFLOW_CFG_MODIFY;
            continue;
        }

        // flow is complete same, no need to change
        OMCIALDBG("gemflow same, no need to change");
        pFlow->cfgFlag = CA_OMCI_GEMFLOW_CFG_REMAIN;
    }
}


void ca_omci_analysis_us_stream_add_all(void)
{
    struct avl_traverser  avlTrav;
    ca_omci_stream_cfg_t *stream = NULL;

    OMCI_ENTER;

    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.usFlow);
    if (NULL == stream)
    {
        OMCIALWARN("No us stream analysis now, retry later");
        return;
    }

    while (NULL != stream)
    {
        // get all flow cfg then to cfg them
        ca_omci_hal_us_flow_add(stream);
        ca_omci_us_stream_tbl_cfg_save(stream);
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
    }
}


void ca_omci_analysis_ds_ivs_stream_add_all()
{
    struct avl_traverser  avlTrav;
    ca_omci_stream_cfg_t *stream = NULL;

    OMCI_ENTER;

    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.dsFlow);
    if (NULL == stream)
    {
        OMCIALWARN("No ds stream analysis now, retry later");
        return;
    }

    while (NULL != stream)
    {
        // get all flow cfg then to cfg them
        ca_omci_hal_ds_ivs_flow_add(stream);
        ca_omci_ds_ivs_stream_tbl_cfg_save(stream);
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
    }
}


void ca_omci_analysis_stream_ds_ivs_cfg(void)
{
    struct avl_traverser  avlTrav;
    struct avl_traverser  cfgAvlTrav;
    ca_omci_stream_cfg_t *stream    = NULL;
    ca_omci_stream_cfg_t *streamAna = NULL;

    OMCI_ENTER;

    //cfg us flow first, then ds
    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.dsFlow);
    if (NULL == stream)
    {
        //never cfg flow before, just cfg flow and save configure;
        ca_omci_analysis_ds_ivs_stream_add_all();
        return;
    }

    //after go through cfg table, then go through analysis table to add new flow
    streamAna = (ca_omci_stream_cfg_t *)omci_avl_t_first(&cfgAvlTrav, sg_omciAnalyseFlowTbl.flow.dsFlow);
    while (NULL != streamAna)
    {
        if (CA_OMCI_ANALYSIS_CFG_INIT == streamAna->cfgFlag)
        {
            OMCIALDBG("add the new stream");
            ca_omci_hal_ds_ivs_flow_add(streamAna);
            ca_omci_ds_ivs_stream_tbl_cfg_save(streamAna);
        }

        if (CA_OMCI_ANALYSIS_CFG_MODIFY == streamAna->cfgFlag)
        {
            OMCIALDBG("ds stream modify, add the new one");
            ca_omci_hal_ds_ivs_flow_add(streamAna);
            ca_omci_ds_ivs_stream_tbl_cfg_save(streamAna);
        }

        streamAna = (ca_omci_stream_cfg_t *)omci_avl_t_next(&cfgAvlTrav);
    }
    OMCI_LEAVE;
}


void ca_omci_analysis_stream_ds_ivs_remove(void)
{
    struct avl_traverser  avlTrav;
    ca_omci_stream_cfg_t *stream    = NULL;
    ca_omci_stream_cfg_t *tmpStream = NULL;
    ca_omci_stream_cfg_t *pStream   = NULL;

    OMCI_ENTER;

    //cfg us flow first, then ds
    if (NULL == sg_omciAnalyseFlowTbl.flow.dsFlow)
    {
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.dsFlow);
        while (NULL != stream)
        {
            //remove all flow;
            OMCIALDBG("ds stream remove all");
            tmpStream = stream;
            stream    = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            ca_omci_hal_ds_ivs_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.dsFlow, tmpStream);
        }
        return;
    }

    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.dsFlow);
    while (NULL != stream)
    {
        // get the flow in analysis table
        pStream   = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.dsFlow, stream);
        tmpStream = stream;
        stream    = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);

        //if this flow can't be found in analysis table, means that this flow is removed by OLT
        if (NULL == pStream)
        {
            OMCIALDBG("ds stream remove");
            ca_omci_hal_ds_ivs_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.dsFlow, tmpStream);
            continue;
        }

        //compare the analysis table and the saved cfg table, when flow changes, remove old flow
        if (CA_OMCI_TRUE == ca_omci_stream_change(pStream, tmpStream))
        {
            OMCIALDBG("ds stream modify, remove the old one");
            ca_omci_hal_ds_ivs_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.dsFlow, tmpStream);
            pStream->cfgFlag = CA_OMCI_ANALYSIS_CFG_MODIFY;
            continue;
        }

        // flow is complete same, no need to change
        OMCIALDBG("ds stream same, no need to change");
        pStream->cfgFlag = CA_OMCI_ANALYSIS_CFG_REMAIN;
    }

    OMCI_LEAVE;
}


void ca_omci_analysis_stream_us_cfg(void)
{
    struct avl_traverser  avlTrav;
    struct avl_traverser  cfgAvlTrav;
    ca_omci_stream_cfg_t *stream    = NULL;
    ca_omci_stream_cfg_t *streamAna = NULL;

    OMCI_ENTER;

    //cfg us flow first, then ds
    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.usFlow);
    if (NULL == stream)
    {
        //never cfg flow before, just cfg flow and save configure;
        ca_omci_analysis_us_stream_add_all();
        return;
    }

    //after go through cfg table, then go through analysis table to add new flow
    streamAna = (ca_omci_stream_cfg_t *)omci_avl_t_first(&cfgAvlTrav, sg_omciAnalyseFlowTbl.flow.usFlow);
    while (NULL != streamAna)
    {
        if (CA_OMCI_ANALYSIS_CFG_INIT == streamAna->cfgFlag)
        {
            OMCIALDBG("add new us stream");
            ca_omci_hal_us_flow_add(streamAna);
            ca_omci_us_stream_tbl_cfg_save(streamAna);
        }

        if (CA_OMCI_ANALYSIS_CFG_MODIFY == streamAna->cfgFlag)
        {
            OMCIALDBG("us stream modify, add new one");
            ca_omci_hal_us_flow_add(streamAna);
            ca_omci_us_stream_tbl_cfg_save(streamAna);
        }

        streamAna = (ca_omci_stream_cfg_t *)omci_avl_t_next(&cfgAvlTrav);
    }
    OMCI_LEAVE;
}


void ca_omci_analysis_stream_us_remove(void)
{
    struct avl_traverser  avlTrav;
    ca_omci_stream_cfg_t *stream    = NULL;
    ca_omci_stream_cfg_t *tmpStream = NULL;
    ca_omci_stream_cfg_t *pStream   = NULL;

    OMCI_ENTER;

    //cfg us flow first, then ds
    if (NULL == sg_omciAnalyseFlowTbl.flow.usFlow)
    {
        stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.usFlow);
        while (NULL != stream)
        {
            //remove all flow;
            OMCIALDBG("us stream remove all");
            tmpStream = stream;
            stream    = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            ca_omci_hal_us_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.usFlow, tmpStream);
        }
        return;
    }

    stream = (ca_omci_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.usFlow);
    while (NULL != stream)
    {
        // get the flow in analysis table
        pStream   = (ca_omci_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.usFlow, stream);
        tmpStream = stream;
        stream    = (ca_omci_stream_cfg_t *)omci_avl_t_next(&avlTrav);

        //if this flow can't be found in analysis table, means that this flow is removed by OLT
        if (NULL == pStream)
        {
            OMCIALDBG("us stream remove");
            ca_omci_hal_us_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.usFlow, tmpStream);
            continue;
        }

        //compare the analysis table and the saved cfg table, when flow changes, remove old flow
        if (CA_OMCI_TRUE == ca_omci_stream_change(pStream, tmpStream))
        {
            OMCIALDBG("us stream modify, remove the old one");
            ca_omci_hal_us_flow_del(tmpStream);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.usFlow, tmpStream);
            pStream->cfgFlag = CA_OMCI_ANALYSIS_CFG_MODIFY;
            continue;
        }

        // flow is complete same, no need to change
        OMCIALDBG("us stream same, no need to change");
        pStream->cfgFlag = CA_OMCI_ANALYSIS_CFG_REMAIN;
    }

    OMCI_LEAVE;
}


void ca_omci_analysis_stream_cfg(void)
{
    OMCI_ENTER;

    //cfg us flow first, then ds
    ca_omci_analysis_stream_us_cfg();

    //Add ds multicast and broadcast stream
    ca_omci_analysis_stream_ds_ivs_cfg();

    //TODO: add ds stream later
    OMCI_LEAVE;
}


void ca_omci_analysis_stream_remove(void)
{
    OMCI_ENTER;

    //remove us flow first, then ds
    ca_omci_analysis_stream_us_remove();

    //remove ds multicast and broadcast stream
    ca_omci_analysis_stream_ds_ivs_remove();

    //TODO: add ds stream later
    OMCI_LEAVE;
}


void ca_omci_analysis_filter_stream_add_all(void)
{
    struct avl_traverser         avlTrav;
    ca_omci_filter_stream_cfg_t *stream = NULL;

    OMCI_ENTER;

    stream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciAnalyseFlowTbl.flow.filterFlow);
    if (NULL == stream)
    {
        OMCIALWARN("No filter stream analysis now, retry later");
        return;
    }

    while (NULL != stream)
    {
        // get all flow cfg then to cfg them
        ca_omci_hal_filter_flow_add(stream);
        ca_omci_filter_stream_tbl_cfg_save(stream);
        stream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTrav);
    }
}


void ca_omci_analysis_filter_stream_cfg(void)
{
    struct avl_traverser         avlTrav;
    struct avl_traverser         cfgAvlTrav;
    ca_omci_filter_stream_cfg_t *filterStream = NULL;
    ca_omci_filter_stream_cfg_t *filterAna    = NULL;

    OMCI_ENTER;
    filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.filterFlow);
    if (NULL == filterStream)
    {
        //never cfg flow before, just cfg flow and save configure;
        ca_omci_analysis_filter_stream_add_all();
        return;
    }

    //after go through cfg table, then go through analysis table to add new flow
    filterAna = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&cfgAvlTrav, sg_omciAnalyseFlowTbl.flow.filterFlow);
    while (NULL != filterAna)
    {
        if (CA_OMCI_ANALYSIS_CFG_INIT == filterAna->cfgFlag)
        {
            OMCIALDBG("add new filter stream");
            ca_omci_hal_filter_flow_add(filterAna);
            ca_omci_filter_stream_tbl_cfg_save(filterAna);
        }

        if (CA_OMCI_ANALYSIS_CFG_MODIFY == filterAna->cfgFlag)
        {
            OMCIALDBG("filter stream modify, add the new one");
            ca_omci_hal_filter_flow_add(filterAna);
            ca_omci_filter_stream_tbl_cfg_save(filterAna);
        }

        filterAna = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&cfgAvlTrav);
    }

    OMCI_LEAVE;
}


void ca_omci_analysis_filter_stream_remove(void)
{
    struct avl_traverser         avlTrav;
    ca_omci_filter_stream_cfg_t *filterStream = NULL;
    ca_omci_filter_stream_cfg_t *tmpFilter    = NULL;
    ca_omci_filter_stream_cfg_t *pFilter      = NULL;

    OMCI_ENTER;
    if (NULL == sg_omciAnalyseFlowTbl.flow.filterFlow)
    {
        filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.filterFlow);
        while (NULL != filterStream)
        {
            //remove all flow;
            OMCIALDBG("filter stream remove all");
            tmpFilter    = filterStream;
            filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTrav);
            ca_omci_hal_filter_flow_del(tmpFilter);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.filterFlow, tmpFilter);
        }
        return;
    }

    filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_first(&avlTrav, sg_omciCfgFlowTbl.flow.filterFlow);
    while (NULL != filterStream)
    {
        // get the flow in analysis table
        pFilter      = (ca_omci_filter_stream_cfg_t *)omci_avl_find(sg_omciAnalyseFlowTbl.flow.filterFlow, filterStream);
        tmpFilter    = filterStream;
        filterStream = (ca_omci_filter_stream_cfg_t *)omci_avl_t_next(&avlTrav);

        //if this flow can't be found in analysis table, means that this flow is removed by OLT
        if (NULL == pFilter)
        {
            OMCIALDBG("filter stream remove");
            ca_omci_hal_filter_flow_del(tmpFilter);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.filterFlow, tmpFilter);
            continue;
        }

        if (CA_OMCI_TRUE == ca_omci_filter_stream_change(pFilter, tmpFilter))
        {
            OMCIALDBG("filter stream modify, remove the old one");
            ca_omci_hal_filter_flow_del(tmpFilter);
            omci_avl_delete(sg_omciCfgFlowTbl.flow.filterFlow, tmpFilter);
            pFilter->cfgFlag = CA_OMCI_ANALYSIS_CFG_MODIFY;
            continue;
        }

        // flow is complete same, no need to change
        OMCIALDBG("filter stream same, no need to change");
        pFilter->cfgFlag = CA_OMCI_ANALYSIS_CFG_REMAIN;
    }

    OMCI_LEAVE;
}


//Then release the analyseFlowTbl
void ca_omci_analysis_tbl_realease(void)
{
    ca_omci_analysis_flow_tbl_destory(&sg_omciAnalyseFlowTbl.flow);
}


void ca_omci_analysis_tr69_cfg(void)
{
    //remove old tr69 cfg
    if ((0 == sg_omciAnalyseFlowTbl.acs.localIp)
      && 0 != sg_omciCfgFlowTbl.acs.localIp)
    {
        //TODO call api
        OMCIALDBG("Remove acs configue");
        OMCI_MEMSET(&sg_omciCfgFlowTbl.acs, 0, sizeof(ca_omci_acs_cfg_t));
        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.acs.localIp)
      && 0 != sg_omciCfgFlowTbl.acs.localIp)
    {
        if ((sg_omciAnalyseFlowTbl.acs.localIp == sg_omciCfgFlowTbl.acs.localIp)
         && (0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.acs.usrname, sg_omciCfgFlowTbl.acs.usrname, OMCI_ACS_USERNAME_LEN))
         && (0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.acs.passwd, sg_omciCfgFlowTbl.acs.passwd, OMCI_ACS_PASSWD_LEN))
         && (0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.acs.url, sg_omciCfgFlowTbl.acs.url, OMCI_ACS_URL_LEN)))
        {
            return;
        }

        //TODO call api
        OMCIALDBG("Configure acs using new one");
        OMCI_MEMCPY(&sg_omciCfgFlowTbl.acs, &sg_omciAnalyseFlowTbl.acs, sizeof(ca_omci_acs_cfg_t));
        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.acs.localIp)
      && 0 == sg_omciCfgFlowTbl.acs.localIp)
    {
        //TODO call api
        OMCIALDBG("Configure acs the first time");
        OMCI_MEMCPY(&sg_omciCfgFlowTbl.acs, &sg_omciAnalyseFlowTbl.acs, sizeof(ca_omci_acs_cfg_t));
        return;
    }
}


static void ca_omci_analysis_non_omci_voip_cfg(void)
{
    //remove old non omci voip cfg
    if ((0 == sg_omciAnalyseFlowTbl.nonOmciVoip.usrname[0])
      && 0 != sg_omciCfgFlowTbl.nonOmciVoip.usrname[0])
    {
        //TODO call api
        OMCIALDBG("Remove non omci voip configue");
        OMCI_MEMSET(&sg_omciCfgFlowTbl.nonOmciVoip, 0, sizeof(ca_omci_non_omci_voip_t));
        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.nonOmciVoip.usrname[0])
      && 0 != sg_omciCfgFlowTbl.nonOmciVoip.usrname[0])
    {
        if ((0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.nonOmciVoip.usrname, sg_omciCfgFlowTbl.nonOmciVoip.usrname, OMCI_ACS_USERNAME_LEN))
         && (0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.nonOmciVoip.passwd, sg_omciCfgFlowTbl.nonOmciVoip.passwd, OMCI_ACS_PASSWD_LEN))
         && (0 == OMCI_MEMCMP(sg_omciAnalyseFlowTbl.nonOmciVoip.url, sg_omciCfgFlowTbl.nonOmciVoip.url, OMCI_ACS_URL_LEN)))
        {
            return;
        }

        //TODO call api
        OMCIALDBG("Configure nonOmciVoip using new one");
        OMCI_MEMCPY(&sg_omciCfgFlowTbl.nonOmciVoip, &sg_omciAnalyseFlowTbl.nonOmciVoip, sizeof(ca_omci_non_omci_voip_t));
        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.nonOmciVoip.usrname[0])
      && 0 == sg_omciCfgFlowTbl.nonOmciVoip.usrname[0])
    {
        //TODO call api
        OMCIALDBG("Configure nonOmciVoip the first time");
        OMCI_MEMCPY(&sg_omciCfgFlowTbl.nonOmciVoip, &sg_omciAnalyseFlowTbl.nonOmciVoip, sizeof(ca_omci_non_omci_voip_t));
        return;
    }
}


static void ca_omci_analysis_h248_voip_Cfg(void)
{
    //remove old h248 cfg
    if ((0 == sg_omciAnalyseFlowTbl.h248Voip.potsId)
      && 0 != sg_omciCfgFlowTbl.h248Voip.potsId)
    {
        //TODO call api
        OMCIALDBG("Remove h248 omci voip configue");
        OMCI_MEMSET(&sg_omciCfgFlowTbl.h248Voip, 0, sizeof(ca_omci_h248_voip_t));
        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.h248Voip.potsId)
      && 0 != sg_omciCfgFlowTbl.h248Voip.potsId)
    {
        if (CA_OMCI_TRUE == ca_omci_h248_cfg_change(&sg_omciCfgFlowTbl.h248Voip, &sg_omciAnalyseFlowTbl.h248Voip))
        {
            //TODO call api
            OMCIALDBG("Configure h248 using new one");
            OMCI_MEMCPY(&sg_omciCfgFlowTbl.h248Voip, &sg_omciAnalyseFlowTbl.h248Voip, sizeof(ca_omci_h248_voip_t));
        }

        return;
    }

    if ((0 != sg_omciAnalyseFlowTbl.h248Voip.potsId)
      && 0 == sg_omciAnalyseFlowTbl.h248Voip.potsId)
    {
        //TODO call api
        OMCIALDBG("Configure h248 the first time");
        OMCI_MEMCPY(&sg_omciCfgFlowTbl.h248Voip, &sg_omciAnalyseFlowTbl.h248Voip, sizeof(ca_omci_h248_voip_t));
        return;
    }
}



void ca_omci_analysis_voip_cfg(void)
{
    ca_omci_voip_cfg_t *voipCfg = NULL;

    OMCI_ENTER;

    voipCfg = (ca_omci_voip_cfg_t *)ca_omci_me_entry_lookup(OMCI_ME_VOIP_CFG_CLASS_ID, 0);
    if (voipCfg == NULL)
    {
        return;
    }

    if ((VOIP_CFG_USED_PROTO_NONE == voipCfg->attr.sigProtoUsed)
     || (VOIP_CFG_USED_PROTO_NON_OMCI == voipCfg->attr.sigProtoUsed))
    {
        ca_omci_analysis_non_omci_voip_cfg();
    }

    if (VOIP_CFG_USED_PROTO_H248 == voipCfg->attr.sigProtoUsed)
    {
        ca_omci_analysis_h248_voip_Cfg();
    }
}
#endif

