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

/****************************column description***************
* This file is auto-generated.  Edits to this file may be lost
**************************************************************/
#include "omci_common.h"
#include "omci_hash.h"
#include "omci_me_onu.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_me_onu_data.h"
#include "omci_me_cardholder.h"
#include "omci_me_circuit_pack.h"
#include "omci_me_soft_image.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_eth_pm_his.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_me_mac_brg_cfg.h"
#include "omci_me_mac_port_cfg.h"
#include "omci_me_mac_port_des.h"
#include "omci_me_mac_port_filter.h"
#include "omci_me_mac_port_brg.h"
#include "omci_me_mac_brg_pm.h"
#include "omci_me_mac_port_pm.h"
#include "omci_me_pptp_pots.h"
#include "omci_me_voice_srv_prof.h"
#include "omci_me_vlan_tag_oper.h"
#include "omci_me_mac_port_filter_pre.h"
#include "omci_me_vlan_tag_filter.h"
#include "omci_me_eth_pm_his2.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_olt_g.h"
#include "omci_me_onu_power_shed.h"
#include "omci_me_ip_host.h"
#include "omci_me_ip_host_pm.h"
#include "omci_me_tcp_udp_cfg.h"
#include "omci_me_net_addr.h"
#include "omci_me_voip_cfg.h"
#include "omci_me_voip_ctp.h"
#include "omci_me_call_ctl_pm.h"
#include "omci_me_voip_line_st.h"
#include "omci_me_voip_media_prof.h"
#include "omci_me_rtp_prof_data.h"
#include "omci_me_rtp_pm.h"
#include "omci_me_net_dial_plan.h"
#include "omci_me_voip_app_srv.h"
#include "omci_me_voip_acces_code.h"
#include "omci_me_auth_scy.h"
#include "omci_me_sip_cfg_portal.h"
#include "omci_me_sip_agt_cfg.h"
#include "omci_me_sip_agt_pm.h"
#include "omci_me_sip_call_init_pm.h"
#include "omci_me_sip_usr.h"
#include "omci_me_mgc_cfg_portal.h"
#include "omci_me_mgc_cfg_data.h"
#include "omci_me_mgc_pm.h"
#include "omci_me_large_string.h"
#include "omci_me_onu_remote_debug.h"
#include "omci_me_eq_prot_prof.h"
#include "omci_me_eq_ext_pkg.h"
#include "omci_me_ext_vlan_tag_op.h"
#include "omci_me_onu_g.h"
#include "omci_me_onu2_g.h"
#include "omci_me_tcont.h"
#include "omci_me_ani_g.h"
#include "omci_me_uni_g.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_gemport_pm.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_gal_eth_prof.h"
#include "omci_me_threshold1.h"
#include "omci_me_threshold2.h"
#include "omci_me_gal_eth_pm.h"
#include "omci_me_pri_queue.h"
#include "omci_me_traffic_scheg.h"
#include "omci_me_protect_data.h"
#include "omci_me_traffic_descript.h"
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_omci.h"
#include "omci_me_dot1x_port_ext_pkg.h"
#include "omci_me_dot1x_pm_his.h"
#include "omci_me_eth_pm_his3.h"
#include "omci_me_port_map_pkg.h"
#include "omci_me_dot1_rate_limiter.h"
#include "omci_me_dot1ag_domain.h"
#include "omci_me_dot1ag_asso.h"
#include "omci_me_dot1ag_md_level.h"
#include "omci_me_dot1ag_mep.h"
#include "omci_me_dot1ag_mep_st.h"
#include "omci_me_dot1ag_mep_ccm.h"
#include "omci_me_dot1ag_cfm.h"
#include "omci_me_dot1ag_chas_info.h"
#include "omci_me_oct_str.h"
#include "omci_me_gen_pur_buff.h"
#include "omci_me_multicast_oper_prof.h"
#include "omci_me_multi_sub_cfg_info.h"
#include "omci_me_multi_sub_moni.h"
#include "omci_me_fec_pm.h"
#include "omci_me_file_trans_ctl.h"
#include "omci_me_eth_frm_pm_ds.h"
#include "omci_me_eth_frm_pm_us.h"
#include "omci_me_veip.h"
#include "omci_me_gen_st_portal.h"
#include "omci_me_enhance_sec_ctl.h"
#include "omci_me_eth_frame_pm.h"
#include "omci_me_snmp_cfg.h"
#include "omci_me_dyn_power_ctl.h"
#include "omci_me_bbrtr69_mng_srv.h"
#include "omci_me_gpn_ctp_pm.h"
#include "omci_me_tcp_udp_pm.h"
#include "omci_me_ener_consu_pm.h"
#include "omci_me_xgpon_tc_pm.h"
#include "omci_me_xgpon_ds_mng_pm.h"
#include "omci_me_xgpon_us_mng_pm.h"
#include "omci_me_ipv6_host.h"
#include "omci_me_brv6_pre_assi.h"
#include "omci_me_poe_ctrl.h"
#include "omci_me_enhance_fec_pm.h"
#include "omci_me_enhance_tc_pm.h"
#include "omci_me_sync_e_oper.h"
#include "omci_me_ptp.h"
#include "omci_me_ptp_status.h"
#include "omci_me_alcl_ont_genv2.h"
#include "omci_me_alcl_ds_tag_oper1.h"
#include "omci_me_onu3_g.h"

ca_omci_me_classid_map_t g_omci_me_map[OMCI_ME_CLASS_ID_MAX] =
{
    {2,     OMCI_ME_ONU_DATA_CLASS_ID},
    {5,     OMCI_ME_CARDHOLDER_CLASS_ID},
    {6,     OMCI_ME_CIRCUIT_PACK_CLASS_ID},
    {7,     OMCI_ME_SOFT_IMAGE_CLASS_ID},
    {11,    OMCI_ME_PPTP_ETH_CLASS_ID},
    {24,    OMCI_ME_ETH_PM_HIS_CLASS_ID},
    {45,    OMCI_ME_MAC_BRG_SRV_CLASS_ID},
    {46,    OMCI_ME_MAC_BRG_CFG_CLASS_ID},
    {47,    OMCI_ME_MAC_PORT_CFG_CLASS_ID},
    {48,    OMCI_ME_MAC_PORT_DES_CLASS_ID},
    {49,    OMCI_ME_MAC_PORT_FILTER_CLASS_ID},
    {50,    OMCI_ME_MAC_PORT_BRG_CLASS_ID},
    {51,    OMCI_ME_MAC_BRG_PM_CLASS_ID},
    {52,    OMCI_ME_MAC_PORT_PM_CLASS_ID},
    {53,    OMCI_ME_PPTP_POTS_CLASS_ID},
    {58,    OMCI_ME_VOICE_SRV_PROF_CLASS_ID},
    {78,    OMCI_ME_VLAN_TAG_OPER_CLASS_ID},
    {79,    OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID},
    {84,    OMCI_ME_VLAN_TAG_FILTER_CLASS_ID},
    {89,    OMCI_ME_ETH_PM_HIS2_CLASS_ID},
    {130,   OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID},
    {131,   OMCI_ME_OLT_G_CLASS_ID},
    {133,   OMCI_ME_ONU_POWER_SHED_CLASS_ID},
    {134,   OMCI_ME_IP_HOST_CLASS_ID},
    {135,   OMCI_ME_IP_HOST_PM_CLASS_ID},
    {136,   OMCI_ME_TCP_UDP_CFG_CLASS_ID},
    {137,   OMCI_ME_NET_ADDR_CLASS_ID},
    {138,   OMCI_ME_VOIP_CFG_CLASS_ID},
    {139,   OMCI_ME_VOIP_CTP_CLASS_ID},
    {140,   OMCI_ME_CALL_CTL_PM_CLASS_ID},
    {141,   OMCI_ME_VOIP_LINE_ST_CLASS_ID},
    {142,   OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID},
    {143,   OMCI_ME_RTP_PROF_DATA_CLASS_ID},
    {144,   OMCI_ME_RTP_PM_CLASS_ID},
    {145,   OMCI_ME_NET_DIAL_PLAN_CLASS_ID},
    {146,   OMCI_ME_VOIP_APP_SRV_CLASS_ID},
    {147,   OMCI_ME_VOIP_ACCES_CODE_CLASS_ID},
    {148,   OMCI_ME_AUTH_SCY_CLASS_ID},
    {149,   OMCI_ME_SIP_CFG_PORTAL_CLASS_ID},
    {150,   OMCI_ME_SIP_AGT_CFG_CLASS_ID},
    {151,   OMCI_ME_SIP_AGT_PM_CLASS_ID},
    {152,   OMCI_ME_SIP_CALL_INIT_PM_CLASS_ID},
    {153,   OMCI_ME_SIP_USR_CLASS_ID},
    {154,   OMCI_ME_MGC_CFG_PORTAL_CLASS_ID},
    {155,   OMCI_ME_MGC_CFG_DATA_CLASS_ID},
    {156,   OMCI_ME_MGC_PM_CLASS_ID},
    {157,   OMCI_ME_LARGE_STRING_CLASS_ID},
    {158,   OMCI_ME_ONU_REMOTE_DEBUG_CLASS_ID},
    {159,   OMCI_ME_EQ_PROT_PROF_CLASS_ID},
    {160,   OMCI_ME_EQ_EXT_PKG_CLASS_ID},
    {171,   OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID},
    {256,   OMCI_ME_ONU_G_CLASS_ID},
    {257,   OMCI_ME_ONU2_G_CLASS_ID},
    {262,   OMCI_ME_TCONT_CLASS_ID},
    {263,   OMCI_ME_ANI_G_CLASS_ID},
    {264,   OMCI_ME_UNI_G_CLASS_ID},
    {266,   OMCI_ME_GEM_IWTP_CLASS_ID},
    {267,   OMCI_ME_GEMPORT_PM_CLASS_ID},
    {268,   OMCI_ME_GEM_NET_CTP_CLASS_ID},
    {272,   OMCI_ME_GAL_ETH_PROF_CLASS_ID},
    {273,   OMCI_ME_THRESHOLD1_CLASS_ID},
    {274,   OMCI_ME_THRESHOLD2_CLASS_ID},
    {276,   OMCI_ME_GAL_ETH_PM_CLASS_ID},
    {277,   OMCI_ME_PRI_QUEUE_CLASS_ID},
    {278,   OMCI_ME_TRAFFIC_SCHEG_CLASS_ID},
    {279,   OMCI_ME_PROTECT_DATA_CLASS_ID},
    {280,   OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID},
    {281,   OMCI_ME_MULTI_GEM_IWTP_CLASS_ID},
    {287,   OMCI_ME_OMCI_CLASS_ID},
    {290,   OMCI_ME_DOT1X_PORT_EXT_PKG_CLASS_ID},
    {292,   OMCI_ME_DOT1X_PM_HIS_CLASS_ID},
    {296,   OMCI_ME_ETH_PM_HIS3_CLASS_ID},
    {297,   OMCI_ME_PORT_MAP_PKG_CLASS_ID},
    {298,   OMCI_ME_DOT1_RATE_LIMITER_CLASS_ID},
    {299,   OMCI_ME_DOT1AG_DOMAIN_CLASS_ID},
    {300,   OMCI_ME_DOT1AG_ASSO_CLASS_ID},
    {301,   OMCI_ME_DOT1AG_MD_LEVEL_CLASS_ID},
    {302,   OMCI_ME_DOT1AG_MEP_CLASS_ID},
    {303,   OMCI_ME_DOT1AG_MEP_ST_CLASS_ID},
    {304,   OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID},
    {305,   OMCI_ME_DOT1AG_CFM_CLASS_ID},
    {306,   OMCI_ME_DOT1AG_CHAS_INFO_CLASS_ID},
    {307,   OMCI_ME_OCT_STR_CLASS_ID},
    {308,   OMCI_ME_GEN_PUR_BUFF_CLASS_ID},
    {309,   OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID},
    {310,   OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID},
    {311,   OMCI_ME_MULTI_SUB_MONI_CLASS_ID},
    {312,   OMCI_ME_FEC_PM_CLASS_ID},
    {318,   OMCI_ME_FILE_TRANS_CTL_CLASS_ID},
    {321,   OMCI_ME_ETH_FRM_PM_DS_CLASS_ID},
    {322,   OMCI_ME_ETH_FRM_PM_US_CLASS_ID},
    {329,   OMCI_ME_VEIP_CLASS_ID},
    {330,   OMCI_ME_GEN_ST_PORTAL_CLASS_ID},
    {332,   OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID},
    {334,   OMCI_ME_ETH_FRAME_PM_CLASS_ID},
    {335,   OMCI_ME_SNMP_CFG_CLASS_ID},
    {336,   OMCI_ME_DYN_POWER_CTL_CLASS_ID},
    {340,   OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID},
    {341,   OMCI_ME_GPN_CTP_PM_CLASS_ID},
    {342,   OMCI_ME_TCP_UDP_PM_CLASS_ID},
    {343,   OMCI_ME_ENER_CONSU_PM_CLASS_ID},
    {344,   OMCI_ME_XGPON_TC_PM_CLASS_ID},
    {345,   OMCI_ME_XGPON_DS_MNG_PM_CLASS_ID},
    {346,   OMCI_ME_XGPON_US_MNG_PM_CLASS_ID},
    {347,   OMCI_ME_IPV6_HOST_CLASS_ID},
    {348,   OMCI_ME_BRV6_PRE_ASSI_CLASS_ID},
    {349,   OMCI_ME_POE_CTRL_CLASS_ID},
    {441,   OMCI_ME_ONU3_G_CLASS_ID},
    {453,   OMCI_ME_ENHANCE_FEC_PM_CLASS_ID},
    {454,   OMCI_ME_ENHANCE_TC_PM_CLASS_ID},
    {464,   OMCI_ME_SYNC_E_OPER_CLASS_ID},
    {465,   OMCI_ME_PTP_CLASS_ID},
    {466,   OMCI_ME_PTP_STATUS_CLASS_ID},
    {65296, OMCI_ME_ALCL_ONT_GENV2_CLASS_ID},
    {65305, OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID},
};


ca_omci_meclass_t g_omci_me_list[OMCI_ME_CLASS_ID_MAX] =
{
    {OMCI_ME_ONU_DATA_CLASS_ID,            1, "onuData",          ca_omci_me_onu_data_init,            ca_omci_me_onu_data_new,            NULL},
    {OMCI_ME_CARDHOLDER_CLASS_ID,          1, "cardholder",       ca_omci_me_cardholder_init,          ca_omci_me_cardholder_new,          NULL},
    {OMCI_ME_CIRCUIT_PACK_CLASS_ID,        1, "circuitPack",      ca_omci_me_circuit_pack_init,        ca_omci_me_circuit_pack_new,        NULL},
    {OMCI_ME_SOFT_IMAGE_CLASS_ID,          1, "softImage",        ca_omci_me_soft_image_init,          ca_omci_me_soft_image_new,          NULL},
    {OMCI_ME_PPTP_ETH_CLASS_ID,            1, "pptpEth",          ca_omci_me_pptp_eth_init,            ca_omci_me_pptp_eth_new,            NULL},
    {OMCI_ME_ETH_PM_HIS_CLASS_ID,          1, "ethPmHis",         ca_omci_me_eth_pm_his_init,          ca_omci_me_eth_pm_his_new,          NULL},
    {OMCI_ME_MAC_BRG_SRV_CLASS_ID,         1, "macBrgSrv",        ca_omci_me_mac_brg_srv_init,         ca_omci_me_mac_brg_srv_new,         NULL},
    {OMCI_ME_MAC_BRG_CFG_CLASS_ID,         1, "macBrgCfg",        ca_omci_me_mac_brg_cfg_init,         ca_omci_me_mac_brg_cfg_new,         NULL},
    {OMCI_ME_MAC_PORT_CFG_CLASS_ID,        1, "macPortCfg",       ca_omci_me_mac_port_cfg_init,        ca_omci_me_mac_port_cfg_new,        NULL},
    {OMCI_ME_MAC_PORT_DES_CLASS_ID,        1, "macPortDes",       ca_omci_me_mac_port_des_init,        ca_omci_me_mac_port_des_new,        NULL},
    {OMCI_ME_MAC_PORT_FILTER_CLASS_ID,     1, "macPortFilter",    ca_omci_me_mac_port_filter_init,     ca_omci_me_mac_port_filter_new,     NULL},
    {OMCI_ME_MAC_PORT_BRG_CLASS_ID,        1, "macPortBrg",       ca_omci_me_mac_port_brg_init,        ca_omci_me_mac_port_brg_new,        NULL},
    {OMCI_ME_MAC_BRG_PM_CLASS_ID,          1, "macBrgPm",         ca_omci_me_mac_brg_pm_init,          ca_omci_me_mac_brg_pm_new,          NULL},
    {OMCI_ME_MAC_PORT_PM_CLASS_ID,         1, "macPortPm",        ca_omci_me_mac_port_pm_init,         ca_omci_me_mac_port_pm_new,         NULL},
    {OMCI_ME_PPTP_POTS_CLASS_ID,           1, "pptpPots",         ca_omci_me_pptp_pots_init,           ca_omci_me_pptp_pots_new,           NULL},
    {OMCI_ME_VOICE_SRV_PROF_CLASS_ID,      1, "voiceSrvProf",     ca_omci_me_voice_srv_prof_init,      ca_omci_me_voice_srv_prof_new,      NULL},
    {OMCI_ME_VLAN_TAG_OPER_CLASS_ID,       1, "vlanTagOper",      ca_omci_me_vlan_tag_oper_init,       ca_omci_me_vlan_tag_oper_new,       NULL},
    {OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, 1, "macPortFilterPre", ca_omci_me_mac_port_filter_pre_init, ca_omci_me_mac_port_filter_pre_new, NULL},
    {OMCI_ME_VLAN_TAG_FILTER_CLASS_ID,     1, "vlanTagFilter",    ca_omci_me_vlan_tag_filter_init,     ca_omci_me_vlan_tag_filter_new,     NULL},
    {OMCI_ME_ETH_PM_HIS2_CLASS_ID,         1, "ethPmHis2",        ca_omci_me_eth_pm_his2_init,         ca_omci_me_eth_pm_his2_new,         NULL},
    {OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID,      1, "ieeeDot1pSrv",     ca_omci_me_ieee_dot1p_srv_init,      ca_omci_me_ieee_dot1p_srv_new,      NULL},
    {OMCI_ME_OLT_G_CLASS_ID,               1, "oltG",             ca_omci_me_olt_g_init,               ca_omci_me_olt_g_new,               NULL},
    {OMCI_ME_ONU_POWER_SHED_CLASS_ID,      1, "onuPowerShed",     ca_omci_me_onu_power_shed_init,      ca_omci_me_onu_power_shed_new,      NULL},
    {OMCI_ME_IP_HOST_CLASS_ID,             1, "iphost",           ca_omci_me_ip_host_init,             ca_omci_me_ip_host_new,             NULL},
    {OMCI_ME_IP_HOST_PM_CLASS_ID,          1, "iphostPm",         ca_omci_me_ip_host_pm_init,          ca_omci_me_ip_host_pm_new,          NULL},
    {OMCI_ME_TCP_UDP_CFG_CLASS_ID,         1, "tcpUdpCfg",        ca_omci_me_tcp_udp_cfg_init,         ca_omci_me_tcp_udp_cfg_new,         NULL},
    {OMCI_ME_NET_ADDR_CLASS_ID,            1, "netAddr",          ca_omci_me_net_addr_init,            ca_omci_me_net_addr_new,            NULL},
    {OMCI_ME_VOIP_CFG_CLASS_ID,            1, "voipCfg",          ca_omci_me_voip_cfg_init,            ca_omci_me_voip_cfg_new,            NULL},
    {OMCI_ME_VOIP_CTP_CLASS_ID,            1, "voipCtp",          ca_omci_me_voip_ctp_init,            ca_omci_me_voip_ctp_new,            NULL},
    {OMCI_ME_CALL_CTL_PM_CLASS_ID,         1, "callCtlPm",        ca_omci_me_call_ctl_pm_init,         ca_omci_me_call_ctl_pm_new,         NULL},
    {OMCI_ME_VOIP_LINE_ST_CLASS_ID,        1, "voipLineSt",       ca_omci_me_voip_line_st_init,        ca_omci_me_voip_line_st_new,        NULL},
    {OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID,     1, "voipMediaProf",    ca_omci_me_voip_media_prof_init,     ca_omci_me_voip_media_prof_new,     NULL},
    {OMCI_ME_RTP_PROF_DATA_CLASS_ID,       1, "rtpProfData",      ca_omci_me_rtp_prof_data_init,       ca_omci_me_rtp_prof_data_new,       NULL},
    {OMCI_ME_RTP_PM_CLASS_ID,              1, "rtpPm",            ca_omci_me_rtp_pm_init,              ca_omci_me_rtp_pm_new,              NULL},
    {OMCI_ME_NET_DIAL_PLAN_CLASS_ID,       1, "netDialPlan",      ca_omci_me_net_dial_plan_init,       ca_omci_me_net_dial_plan_new,       NULL},
    {OMCI_ME_VOIP_APP_SRV_CLASS_ID,        1, "voipAppSrv",       ca_omci_me_voip_app_srv_init,        ca_omci_me_voip_app_srv_new,        NULL},
    {OMCI_ME_VOIP_ACCES_CODE_CLASS_ID,     1, "voipAccesCode",    ca_omci_me_voip_acces_code_init,     ca_omci_me_voip_acces_code_new,     NULL},
    {OMCI_ME_AUTH_SCY_CLASS_ID,            1, "authScy",          ca_omci_me_auth_scy_init,            ca_omci_me_auth_scy_new,            NULL},
    {OMCI_ME_SIP_CFG_PORTAL_CLASS_ID,      0, "sipCfgPortal",     ca_omci_me_sip_cfg_portal_init,      ca_omci_me_sip_cfg_portal_new,      NULL},
    {OMCI_ME_SIP_AGT_CFG_CLASS_ID,         1, "sipAgtCfg",        ca_omci_me_sip_agt_cfg_init,         ca_omci_me_sip_agt_cfg_new,         NULL},
    {OMCI_ME_SIP_AGT_PM_CLASS_ID,          1, "sipAgtPm",         ca_omci_me_sip_agt_pm_init,          ca_omci_me_sip_agt_pm_new,          NULL},
    {OMCI_ME_SIP_CALL_INIT_PM_CLASS_ID,    1, "sipCallInitPm",    ca_omci_me_sip_call_init_pm_init,    ca_omci_me_sip_call_init_pm_new,    NULL},
    {OMCI_ME_SIP_USR_CLASS_ID,             1, "sipUsr",           ca_omci_me_sip_usr_init,             ca_omci_me_sip_usr_new,             NULL},
    {OMCI_ME_MGC_CFG_PORTAL_CLASS_ID,      0, "mgcCfgPortal",     ca_omci_me_mgc_cfg_portal_init,      ca_omci_me_mgc_cfg_portal_new,      NULL},
    {OMCI_ME_MGC_CFG_DATA_CLASS_ID,        1, "mgcCfgData",       ca_omci_me_mgc_cfg_data_init,        ca_omci_me_mgc_cfg_data_new,        NULL},
    {OMCI_ME_MGC_PM_CLASS_ID,              1, "mgcPm",            ca_omci_me_mgc_pm_init,              ca_omci_me_mgc_pm_new,              NULL},
    {OMCI_ME_LARGE_STRING_CLASS_ID,        1, "largeString",      ca_omci_me_large_string_init,        ca_omci_me_large_string_new,        NULL},
    {OMCI_ME_ONU_REMOTE_DEBUG_CLASS_ID,    0, "onuRemoteDbg",     ca_omci_me_onu_remote_debug_init,    ca_omci_me_onu_remote_debug_new,    NULL},
    {OMCI_ME_EQ_PROT_PROF_CLASS_ID,        1, "eqProtProf",       ca_omci_me_eq_prot_prof_init,        ca_omci_me_eq_prot_prof_new,        NULL},
    {OMCI_ME_EQ_EXT_PKG_CLASS_ID,          1, "eqExtPkg",         ca_omci_me_eq_ext_pkg_init,          ca_omci_me_eq_ext_pkg_new,          NULL},
    {OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID,     1, "externVlanTagOp",  ca_omci_me_ext_vlan_tag_op_init,     ca_omci_me_ext_vlan_tag_op_new,     NULL},
    {OMCI_ME_ONU_G_CLASS_ID,               1, "onug",             ca_omci_me_onu_g_init,               ca_omci_me_onu_g_new,               NULL},
    {OMCI_ME_ONU2_G_CLASS_ID,              1, "onu2g",            ca_omci_me_onu2_g_init,              ca_omci_me_onu2_g_new,              NULL},
    {OMCI_ME_TCONT_CLASS_ID,               1, "tcont",            ca_omci_me_tcont_init,               ca_omci_me_tcont_new,               NULL},
    {OMCI_ME_ANI_G_CLASS_ID,               1, "anig",             ca_omci_me_ani_g_init,               ca_omci_me_ani_g_new,               NULL},
    {OMCI_ME_UNI_G_CLASS_ID,               1, "unig",             ca_omci_me_uni_g_init,               ca_omci_me_uni_g_new,               NULL},
    {OMCI_ME_GEM_IWTP_CLASS_ID,            1, "gemIwtp",          ca_omci_me_gem_iwtp_init,            ca_omci_me_gem_iwtp_new,            NULL},
    {OMCI_ME_GEMPORT_PM_CLASS_ID,          1, "gemportPm",        ca_omci_me_gemport_pm_init,          ca_omci_me_gemport_pm_new,          NULL},
    {OMCI_ME_GEM_NET_CTP_CLASS_ID,         1, "gemNetCtp",        ca_omci_me_gem_net_ctp_init,         ca_omci_me_gem_net_ctp_new,         NULL},
    {OMCI_ME_GAL_ETH_PROF_CLASS_ID,        1, "galEthProf",       ca_omci_me_gal_eth_prof_init,        ca_omci_me_gal_eth_prof_new,        NULL},
    {OMCI_ME_THRESHOLD1_CLASS_ID,          1, "threshold1",       ca_omci_me_threshold1_init,          ca_omci_me_threshold1_new,          NULL},
    {OMCI_ME_THRESHOLD2_CLASS_ID,          1, "threshold2",       ca_omci_me_threshold2_init,          ca_omci_me_threshold2_new,          NULL},
    {OMCI_ME_GAL_ETH_PM_CLASS_ID,          1, "galEthPm",         ca_omci_me_gal_eth_pm_init,          ca_omci_me_gal_eth_pm_new,          NULL},
    {OMCI_ME_PRI_QUEUE_CLASS_ID,           1, "priQueue",         ca_omci_me_pri_queue_init,           ca_omci_me_pri_queue_new,           NULL},
    {OMCI_ME_TRAFFIC_SCHEG_CLASS_ID,       1, "trafficScheduler", ca_omci_me_traffic_scheg_init,       ca_omci_me_traffic_scheg_new,       NULL},
    {OMCI_ME_PROTECT_DATA_CLASS_ID,        1, "protectData",      ca_omci_me_protect_data_init,        ca_omci_me_protect_data_new,        NULL},
    {OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID,    1, "trafDesc",         ca_omci_me_traffic_descript_init,    ca_omci_me_traffic_descript_new,    NULL},
    {OMCI_ME_MULTI_GEM_IWTP_CLASS_ID,      1, "multiGemIwtp",     ca_omci_me_multi_gem_iwtp_init,      ca_omci_me_multi_gem_iwtp_new,      NULL},
    {OMCI_ME_OMCI_CLASS_ID,                0, "omci",             ca_omci_me_omci_init,                ca_omci_me_omci_new,                NULL},
    {OMCI_ME_DOT1X_PORT_EXT_PKG_CLASS_ID,  1, "dot1xPortExtPkg",  ca_omci_me_dot1x_port_ext_pkg_init,  ca_omci_me_dot1x_port_ext_pkg_new,  NULL},
    {OMCI_ME_DOT1X_PM_HIS_CLASS_ID,        1, "dot1xPmHis",       ca_omci_me_dot1x_pm_his_init,        ca_omci_me_dot1x_pm_his_new,        NULL},
    {OMCI_ME_ETH_PM_HIS3_CLASS_ID,         1, "ethPmHis3",        ca_omci_me_eth_pm_his3_init,         ca_omci_me_eth_pm_his3_new,         NULL},
    {OMCI_ME_PORT_MAP_PKG_CLASS_ID,        1, "portMapPkg",       ca_omci_me_port_map_pkg_init,        ca_omci_me_port_map_pkg_new,        NULL},
    {OMCI_ME_DOT1_RATE_LIMITER_CLASS_ID,   1, "dot1RateLimit",    ca_omci_me_dot1_rate_limiter_init,   ca_omci_me_dot1_rate_limiter_new,   NULL},
    {OMCI_ME_DOT1AG_DOMAIN_CLASS_ID,       1, "dot1agDomain",     ca_omci_me_dot1ag_domain_init,       ca_omci_me_dot1ag_domain_new,       NULL},
    {OMCI_ME_DOT1AG_ASSO_CLASS_ID,         1, "dot1agAsso",       ca_omci_me_dot1ag_asso_init,         ca_omci_me_dot1ag_asso_new,         NULL},
    {OMCI_ME_DOT1AG_MD_LEVEL_CLASS_ID,     1, "dot1agMdLevel",    ca_omci_me_dot1ag_md_level_init,     ca_omci_me_dot1ag_md_level_new,     NULL},
    {OMCI_ME_DOT1AG_MEP_CLASS_ID,          1, "dot1agMep",        ca_omci_me_dot1ag_mep_init,          ca_omci_me_dot1ag_mep_new,          NULL},
    {OMCI_ME_DOT1AG_MEP_ST_CLASS_ID,       0, "dot1agMepSt",      ca_omci_me_dot1ag_mep_st_init,       ca_omci_me_dot1ag_mep_st_new,       NULL},
    {OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID,      0, "dot1agMepCcm",     ca_omci_me_dot1ag_mep_ccm_init,      ca_omci_me_dot1ag_mep_ccm_new,      NULL},
    {OMCI_ME_DOT1AG_CFM_CLASS_ID,          1, "dot1agCfm",        ca_omci_me_dot1ag_cfm_init,          ca_omci_me_dot1ag_cfm_new,          NULL},
    {OMCI_ME_DOT1AG_CHAS_INFO_CLASS_ID,    1, "dot1agChasInfo",   ca_omci_me_dot1ag_chas_info_init,    ca_omci_me_dot1ag_chas_info_new,    NULL},
    {OMCI_ME_OCT_STR_CLASS_ID,             1, "octStr",           ca_omci_me_oct_str_init,             ca_omci_me_oct_str_new,             NULL},
    {OMCI_ME_GEN_PUR_BUFF_CLASS_ID,        0, "genPurBuff",       ca_omci_me_gen_pur_buff_init,        ca_omci_me_gen_pur_buff_new,        NULL},
    {OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, 1, "multiOperProf",    ca_omci_me_multicast_oper_prof_init, ca_omci_me_multicast_oper_prof_new, NULL},
    {OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID,  1, "multiSubCfgInfo",  ca_omci_me_multi_sub_cfg_info_init,  ca_omci_me_multi_sub_cfg_info_new,  NULL},
    {OMCI_ME_MULTI_SUB_MONI_CLASS_ID,      1, "multiSubMoni",     ca_omci_me_multi_sub_moni_init,      ca_omci_me_multi_sub_moni_new,      NULL},
    {OMCI_ME_FEC_PM_CLASS_ID,              1, "fecPm",            ca_omci_me_fec_pm_init,              ca_omci_me_fec_pm_new,              NULL},
    {OMCI_ME_FILE_TRANS_CTL_CLASS_ID,      1, "fileTransCtl",     ca_omci_me_file_trans_ctl_init,      ca_omci_me_file_trans_ctl_new,      NULL},
    {OMCI_ME_ETH_FRM_PM_DS_CLASS_ID,       1, "ethFrmPmDs",       ca_omci_me_eth_frm_pm_ds_init,       ca_omci_me_eth_frm_pm_ds_new,       NULL},
    {OMCI_ME_ETH_FRM_PM_US_CLASS_ID,       1, "ethFrmPmUs",       ca_omci_me_eth_frm_pm_us_init,       ca_omci_me_eth_frm_pm_us_new,       NULL},
    {OMCI_ME_VEIP_CLASS_ID,                1, "veip",             ca_omci_me_veip_init,                ca_omci_me_veip_new,                NULL},
    {OMCI_ME_GEN_ST_PORTAL_CLASS_ID,       1, "genStPortal",      ca_omci_me_gen_st_portal_init,       ca_omci_me_gen_st_portal_new,       NULL},
    {OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID,     1, "enhanceSecCtl",    ca_omci_me_enhance_sec_ctl_init,     ca_omci_me_enhance_sec_ctl_new,     NULL},
    {OMCI_ME_ETH_FRAME_PM_CLASS_ID,        1, "ethFramePm",       ca_omci_me_eth_frame_pm_init,        ca_omci_me_eth_frame_pm_new,        NULL},
    {OMCI_ME_SNMP_CFG_CLASS_ID,            1, "snmpCfg",          ca_omci_me_snmp_cfg_init,            ca_omci_me_snmp_cfg_new,            NULL},
    {OMCI_ME_DYN_POWER_CTL_CLASS_ID,       1, "dynPowerCtl",      ca_omci_me_dyn_power_ctl_init,       ca_omci_me_dyn_power_ctl_new,       NULL},
    {OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID,     1, "bbrtr69MngSrv",    ca_omci_me_bbrtr69_mng_srv_init,     ca_omci_me_bbrtr69_mng_srv_new,     NULL},
    {OMCI_ME_GPN_CTP_PM_CLASS_ID,          1, "gpnCtpPm",         ca_omci_me_gpn_ctp_pm_init,          ca_omci_me_gpn_ctp_pm_new,          NULL},
    {OMCI_ME_TCP_UDP_PM_CLASS_ID,          1, "tcpUdpPm",         ca_omci_me_tcp_udp_pm_init,          ca_omci_me_tcp_udp_pm_new,          NULL},
    {OMCI_ME_ENER_CONSU_PM_CLASS_ID,       1, "enerConsuPm",      ca_omci_me_ener_consu_pm_init,       ca_omci_me_ener_consu_pm_new,       NULL},
    {OMCI_ME_XGPON_TC_PM_CLASS_ID,         1, "xgponTcPm",        ca_omci_me_xgpon_tc_pm_init,         ca_omci_me_xgpon_tc_pm_new,         NULL},
    {OMCI_ME_XGPON_DS_MNG_PM_CLASS_ID,     1, "xgponDsMngPm",     ca_omci_me_xgpon_ds_mng_pm_init,     ca_omci_me_xgpon_ds_mng_pm_new,     NULL},
    {OMCI_ME_XGPON_US_MNG_PM_CLASS_ID,     1, "xgponUsMngPm",      ca_omci_me_xgpon_us_mng_pm_init,     ca_omci_me_xgpon_us_mng_pm_new,     NULL},
    {OMCI_ME_IPV6_HOST_CLASS_ID,           1, "ipv6host",         ca_omci_me_ipv6_host_init,           ca_omci_me_ipv6_host_new,           NULL},
    {OMCI_ME_BRV6_PRE_ASSI_CLASS_ID,       1, "brv6PreAssi",      ca_omci_me_brv6_pre_assi_init,       ca_omci_me_brv6_pre_assi_new,       NULL},
    {OMCI_ME_POE_CTRL_CLASS_ID,            1, "poeCtrl",           ca_omci_me_poe_ctrl_init,            ca_omci_me_poe_ctrl_new,            NULL},
    {OMCI_ME_ONU3_G_CLASS_ID,              1, "onu3G",            ca_omci_me_onu3_g_init,              ca_omci_me_onu3_g_new,              NULL},
    {OMCI_ME_ENHANCE_FEC_PM_CLASS_ID,      1, "enhanceFecPpm",     ca_omci_me_enhance_fec_pm_init,      ca_omci_me_enhance_fec_pm_new,      NULL},
    {OMCI_ME_ENHANCE_TC_PM_CLASS_ID,       1, "enhanceTcPm",       ca_omci_me_enhance_tc_pm_init,       ca_omci_me_enhance_tc_pm_new,       NULL},
    {OMCI_ME_SYNC_E_OPER_CLASS_ID,         1, "syncEOper",         ca_omci_me_sync_e_oper_init,         ca_omci_me_sync_e_oper_new,         NULL},
    {OMCI_ME_PTP_CLASS_ID,                 1, "ptp",               ca_omci_me_ptp_init,                 ca_omci_me_ptp_new,                 NULL},
    {OMCI_ME_PTP_STATUS_CLASS_ID,          1, "ptpStatus",         ca_omci_me_ptp_status_init,          ca_omci_me_ptp_status_new,          NULL},
    {OMCI_ME_ALCL_ONT_GENV2_CLASS_ID,      1, "alclOntGenV2",     ca_omci_me_alcl_ont_genv2_init,      ca_omci_me_alcl_ont_genv2_new,      NULL},
    {OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID,   1, "alclDsTagOper1",   ca_omci_me_alcl_ds_tag_oper1_init,   ca_omci_me_alcl_ds_tag_oper1_new,   NULL},
};


ca_omci_me_dscpt_t g_omci_me_info_list[OMCI_ME_CLASS_ID_MAX] =
{
    {OMCI_ME_ONU_DATA_CLASS_ID,            g_omci_me_onu_data_attr},
    {OMCI_ME_CARDHOLDER_CLASS_ID,          g_omci_me_cardholder_attr},
    {OMCI_ME_CIRCUIT_PACK_CLASS_ID,        g_omci_me_circuit_pack_attr},
    {OMCI_ME_SOFT_IMAGE_CLASS_ID,          g_omci_me_soft_image_attr},
    {OMCI_ME_PPTP_ETH_CLASS_ID,            g_omci_me_pptp_eth_attr},
    {OMCI_ME_ETH_PM_HIS_CLASS_ID,          g_omci_me_eth_pm_his_attr},
    {OMCI_ME_MAC_BRG_SRV_CLASS_ID,         g_omci_me_mac_brg_srv_attr},
    {OMCI_ME_MAC_BRG_CFG_CLASS_ID,         g_omci_me_mac_brg_cfg_attr},
    {OMCI_ME_MAC_PORT_CFG_CLASS_ID,        g_omci_me_mac_port_cfg_attr},
    {OMCI_ME_MAC_PORT_DES_CLASS_ID,        g_omci_me_mac_port_des_attr},
    {OMCI_ME_MAC_PORT_FILTER_CLASS_ID,     g_omci_me_mac_port_filter_attr},
    {OMCI_ME_MAC_PORT_BRG_CLASS_ID,        g_omci_me_mac_port_brg_attr},
    {OMCI_ME_MAC_BRG_PM_CLASS_ID,          g_omci_me_mac_brg_pm_attr},
    {OMCI_ME_MAC_PORT_PM_CLASS_ID,         g_omci_me_mac_port_pm_attr},
    {OMCI_ME_PPTP_POTS_CLASS_ID,           g_omci_me_pptp_pots_attr},
    {OMCI_ME_VOICE_SRV_PROF_CLASS_ID,      g_omci_me_voice_srv_prof_attr},
    {OMCI_ME_VLAN_TAG_OPER_CLASS_ID,       g_omci_me_vlan_tag_oper_attr},
    {OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, g_omci_me_mac_port_filter_pre_attr},
    {OMCI_ME_VLAN_TAG_FILTER_CLASS_ID,     g_omci_me_vlan_tag_filter_attr},
    {OMCI_ME_ETH_PM_HIS2_CLASS_ID,         g_omci_me_eth_pm_his2_attr},
    {OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID,      g_omci_me_ieee_dot1p_srv_attr},
    {OMCI_ME_OLT_G_CLASS_ID,               g_omci_me_olt_g_attr},
    {OMCI_ME_ONU_POWER_SHED_CLASS_ID,      g_omci_me_onu_power_shed_attr},
    {OMCI_ME_IP_HOST_CLASS_ID,             g_omci_me_ip_host_attr},
    {OMCI_ME_IP_HOST_PM_CLASS_ID,          g_omci_me_ip_host_pm_attr},
    {OMCI_ME_TCP_UDP_CFG_CLASS_ID,         g_omci_me_tcp_udp_cfg_attr},
    {OMCI_ME_NET_ADDR_CLASS_ID,            g_omci_me_net_addr_attr},
    {OMCI_ME_VOIP_CFG_CLASS_ID,            g_omci_me_voip_cfg_attr},
    {OMCI_ME_VOIP_CTP_CLASS_ID,            g_omci_me_voip_ctp_attr},
    {OMCI_ME_CALL_CTL_PM_CLASS_ID,         g_omci_me_call_ctl_pm_attr},
    {OMCI_ME_VOIP_LINE_ST_CLASS_ID,        g_omci_me_voip_line_st_attr},
    {OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID,     g_omci_me_voip_media_prof_attr},
    {OMCI_ME_RTP_PROF_DATA_CLASS_ID,       g_omci_me_rtp_prof_data_attr},
    {OMCI_ME_RTP_PM_CLASS_ID,              g_omci_me_rtp_pm_attr},
    {OMCI_ME_NET_DIAL_PLAN_CLASS_ID,       g_omci_me_net_dial_plan_attr},
    {OMCI_ME_VOIP_APP_SRV_CLASS_ID,        g_omci_me_voip_app_srv_attr},
    {OMCI_ME_VOIP_ACCES_CODE_CLASS_ID,     g_omci_me_voip_acces_code_attr},
    {OMCI_ME_AUTH_SCY_CLASS_ID,            g_omci_me_auth_scy_attr},
    {OMCI_ME_SIP_CFG_PORTAL_CLASS_ID,      g_omci_me_sip_cfg_portal_attr},
    {OMCI_ME_SIP_AGT_CFG_CLASS_ID,         g_omci_me_sip_agt_cfg_attr},
    {OMCI_ME_SIP_AGT_PM_CLASS_ID,          g_omci_me_sip_agt_pm_attr},
    {OMCI_ME_SIP_CALL_INIT_PM_CLASS_ID,    g_omci_me_sip_call_init_pm_attr},
    {OMCI_ME_SIP_USR_CLASS_ID,             g_omci_me_sip_usr_attr},
    {OMCI_ME_MGC_CFG_PORTAL_CLASS_ID,      g_omci_me_mgc_cfg_portal_attr},
    {OMCI_ME_MGC_CFG_DATA_CLASS_ID,        g_omci_me_mgc_cfg_data_attr},
    {OMCI_ME_MGC_PM_CLASS_ID,              g_omci_me_mgc_pm_attr},
    {OMCI_ME_LARGE_STRING_CLASS_ID,        g_omci_me_large_string_attr},
    {OMCI_ME_ONU_REMOTE_DEBUG_CLASS_ID,    g_omci_me_onu_remote_debug_attr},
    {OMCI_ME_EQ_PROT_PROF_CLASS_ID,        g_omci_me_eq_prot_prof_attr},
    {OMCI_ME_EQ_EXT_PKG_CLASS_ID,          g_omci_me_eq_ext_pkg_attr},
    {OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID,     g_omci_me_ext_vlan_tag_op_attr},
    {OMCI_ME_ONU_G_CLASS_ID,               g_omci_me_onu_g_attr},
    {OMCI_ME_ONU2_G_CLASS_ID,              g_omci_me_onu2_g_attr},
    {OMCI_ME_TCONT_CLASS_ID,               g_omci_me_tcont_attr},
    {OMCI_ME_ANI_G_CLASS_ID,               g_omci_me_ani_g_attr},
    {OMCI_ME_UNI_G_CLASS_ID,               g_omci_me_uni_g_attr},
    {OMCI_ME_GEM_IWTP_CLASS_ID,            g_omci_me_gem_iwtp_attr},
    {OMCI_ME_GEMPORT_PM_CLASS_ID,          g_omci_me_gemport_pm_attr},
    {OMCI_ME_GEM_NET_CTP_CLASS_ID,         g_omci_me_gem_net_ctp_attr},
    {OMCI_ME_GAL_ETH_PROF_CLASS_ID,        g_omci_me_gal_eth_prof_attr},
    {OMCI_ME_THRESHOLD1_CLASS_ID,          g_omci_me_threshold1_attr},
    {OMCI_ME_THRESHOLD2_CLASS_ID,          g_omci_me_threshold2_attr},
    {OMCI_ME_GAL_ETH_PM_CLASS_ID,          g_omci_me_gal_eth_pm_attr},
    {OMCI_ME_PRI_QUEUE_CLASS_ID,           g_omci_me_pri_queue_attr},
    {OMCI_ME_TRAFFIC_SCHEG_CLASS_ID,       g_omci_me_traffic_scheg_attr},
    {OMCI_ME_PROTECT_DATA_CLASS_ID,        g_omci_me_protect_data_attr},
    {OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID,    g_omci_me_traffic_descript_attr},
    {OMCI_ME_MULTI_GEM_IWTP_CLASS_ID,      g_omci_me_multi_gem_iwtp_attr},
    {OMCI_ME_OMCI_CLASS_ID,                g_omci_me_omci_attr},
    {OMCI_ME_DOT1X_PORT_EXT_PKG_CLASS_ID,  g_omci_me_dot1x_port_ext_pkg_attr},
    {OMCI_ME_DOT1X_PM_HIS_CLASS_ID,        g_omci_me_dot1x_pm_his_attr},
    {OMCI_ME_ETH_PM_HIS3_CLASS_ID,         g_omci_me_eth_pm_his3_attr},
    {OMCI_ME_PORT_MAP_PKG_CLASS_ID,        g_omci_me_port_map_pkg_attr},
    {OMCI_ME_DOT1_RATE_LIMITER_CLASS_ID,   g_omci_me_dot1_rate_limiter_attr},
    {OMCI_ME_DOT1AG_DOMAIN_CLASS_ID,       g_omci_me_dot1ag_domain_attr},
    {OMCI_ME_DOT1AG_ASSO_CLASS_ID,         g_omci_me_dot1ag_asso_attr},
    {OMCI_ME_DOT1AG_MD_LEVEL_CLASS_ID,     g_omci_me_dot1ag_md_level_attr},
    {OMCI_ME_DOT1AG_MEP_CLASS_ID,          g_omci_me_dot1ag_mep_attr},
    {OMCI_ME_DOT1AG_MEP_ST_CLASS_ID,       g_omci_me_dot1ag_mep_st_attr},
    {OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID,      g_omci_me_dot1ag_mep_ccm_attr},
    {OMCI_ME_DOT1AG_CFM_CLASS_ID,          g_omci_me_dot1ag_cfm_attr},
    {OMCI_ME_DOT1AG_CHAS_INFO_CLASS_ID,    g_omci_me_dot1ag_chas_info_attr},
    {OMCI_ME_OCT_STR_CLASS_ID,             g_omci_me_oct_str_attr},
    {OMCI_ME_GEN_PUR_BUFF_CLASS_ID,        g_omci_me_gen_pur_buff_attr},
    {OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, g_omci_me_multicast_oper_prof_attr},
    {OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID,  g_omci_me_multi_sub_cfg_info_attr},
    {OMCI_ME_MULTI_SUB_MONI_CLASS_ID,      g_omci_me_multi_sub_moni_attr},
    {OMCI_ME_FEC_PM_CLASS_ID,              g_omci_me_fec_pm_attr},
    {OMCI_ME_FILE_TRANS_CTL_CLASS_ID,      g_omci_me_file_trans_ctl_attr},
    {OMCI_ME_ETH_FRM_PM_DS_CLASS_ID,       g_omci_me_eth_frm_pm_ds_attr},
    {OMCI_ME_ETH_FRM_PM_US_CLASS_ID,       g_omci_me_eth_frm_pm_us_attr},
    {OMCI_ME_VEIP_CLASS_ID,                g_omci_me_veip_attr},
    {OMCI_ME_GEN_ST_PORTAL_CLASS_ID,       g_omci_me_gen_st_portal_attr},
    {OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID,     g_omci_me_enhance_sec_ctl_attr},
    {OMCI_ME_ETH_FRAME_PM_CLASS_ID,        g_omci_me_eth_frame_pm_attr},
    {OMCI_ME_SNMP_CFG_CLASS_ID,            g_omci_me_snmp_cfg_attr},
    {OMCI_ME_DYN_POWER_CTL_CLASS_ID,       g_omci_me_dyn_power_ctl_attr},
    {OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID,     g_omci_me_bbrtr69_mng_srv_attr},
    {OMCI_ME_GPN_CTP_PM_CLASS_ID,          g_omci_me_gpn_ctp_pm_attr},
    {OMCI_ME_TCP_UDP_PM_CLASS_ID,          g_omci_me_tcp_udp_pm_attr},
    {OMCI_ME_ENER_CONSU_PM_CLASS_ID,       g_omci_me_ener_consu_pm_attr},
    {OMCI_ME_XGPON_TC_PM_CLASS_ID,         g_omci_me_xgpon_tc_pm_attr},
    {OMCI_ME_XGPON_DS_MNG_PM_CLASS_ID,     g_omci_me_xgpon_ds_mng_pm_attr},
    {OMCI_ME_XGPON_US_MNG_PM_CLASS_ID,     g_omci_me_xgpon_us_mng_pm_attr},
    {OMCI_ME_IPV6_HOST_CLASS_ID,           g_omci_me_ipv6_host_attr},
    {OMCI_ME_BRV6_PRE_ASSI_CLASS_ID,       g_omci_me_brv6_pre_assi_attr},
    {OMCI_ME_POE_CTRL_CLASS_ID,            g_omci_me_poe_ctrl_attr},
    {OMCI_ME_ONU3_G_CLASS_ID,              g_omci_me_onu3_g_attr},
    {OMCI_ME_ENHANCE_FEC_PM_CLASS_ID,      g_omci_me_enhance_fec_pm_attr},
    {OMCI_ME_ENHANCE_TC_PM_CLASS_ID,       g_omci_me_enhance_tc_pm_attr},
    {OMCI_ME_SYNC_E_OPER_CLASS_ID,         g_omci_me_sync_e_oper_attr},
    {OMCI_ME_PTP_CLASS_ID,                 g_omci_me_ptp_attr},
    {OMCI_ME_PTP_STATUS_CLASS_ID,          g_omci_me_ptp_status_attr},
    {OMCI_ME_ALCL_ONT_GENV2_CLASS_ID,      g_omci_me_alcl_ont_genv2_attr},
    {OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID,   g_omci_me_alcl_ds_tag_oper1_attr},
};


ca_omci_class_id_e ca_omci_me_get_inner_class(ca_omci_uint16_t standardClass)
{
    ca_omci_uint16_t idx = 0;

    for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        if (g_omci_me_map[idx].standardClassId == standardClass)
        {
            return g_omci_me_map[idx].classId;
        }
    }

    return OMCI_ME_CLASS_ID_MAX;
}


ca_omci_uint16_t ca_omci_me_get_standard_class(ca_omci_class_id_e innerClass)
{
    ca_omci_uint16_t idx = 0;

    for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        if (g_omci_me_map[idx].classId == innerClass)
        {
            return g_omci_me_map[idx].standardClassId;
        }
    }

    return OMCI_ME_CLASS_ID_MAX;
}


ca_omci_st_e ca_omci_me_class_map_adjust(ca_omci_uint16_t standardClass, ca_omci_class_id_e innerClass)
{
    ca_omci_uint16_t idx = 0;

    for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        if (g_omci_me_map[idx].standardClassId == standardClass)
        {
            g_omci_me_map[idx].classId = innerClass;
            return CA_OMCI_OK;
        }
    }

    return CA_OMCI_E;
}


const ca_omci_int8_t * ca_omci_me_desc_get(ca_omci_uint16_t classId)
{
    OMCI_CHECK_CLASSID_RET("UNKONW");
    return g_omci_me_list[classId].desc;
}


ca_omci_meclass_t *ca_omci_me_get(ca_omci_uint16_t classId)
{
    OMCI_CHECK_CLASSID_RET(NULL);
    return &g_omci_me_list[classId];
}


ca_omci_mebase_t *ca_omci_me_new(ca_omci_uint16_t classId)
{
    ca_omci_meclass_t *meclass = NULL;

    OMCI_CHECK_CLASSID_RET(NULL);

    meclass = &g_omci_me_list[classId];
    return meclass->newProcCb();
}


void ca_omci_me_delete(ca_omci_mebase_t *base)
{
    OMCI_PARAM_IS_NULL(base);

    if (base->amBmp)
    {
        OMCI_FREE(base->amBmp);
    }

    if (base->arcBmp)
    {
        OMCI_FREE(base->arcBmp);
    }

    ca_omci_linked_me_clear(base);
    OMCI_FREE(base);
}


ca_omci_st_e ca_omci_me_finish(void)
{
    ca_omci_uint32_t   i       = 0;
    ca_omci_meclass_t *meclass = NULL;
    ca_omci_mebase_t  *meInst  = NULL;

    for (i = 0; i < OMCI_ME_CLASS_ID_MAX; i++)
    {
        meclass = &g_omci_me_list[i];
        OMCIDBG("remove me [%s]", meclass->desc);
        meInst = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, NULL);

        while (NULL != meInst)
        {
            OMCIDBG("remove instance[%d]", meInst->instanceId);
            HASH_Remove(meclass->instHash, meInst);
            ca_omci_me_delete(meInst);
            meInst = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, NULL);
        }

        HASH_Destroy(meclass->instHash);
    }

    return CA_OMCI_OK;
}


ca_omci_uint32_t ca_omci_me_entry_num_get(ca_omci_uint16_t classId)
{
    ca_omci_meclass_t *meclass = NULL;

    meclass = ca_omci_me_get(classId);

    if (NULL == meclass)
    {
        return 0;
    }

    if (meclass->instHash && meclass->instHash->count)
    {
        return (ca_omci_uint32_t)meclass->instHash->count;
    }

    return 0;
}


ca_omci_mebase_t *ca_omci_me_entry_lookup(ca_omci_uint16_t classId, ca_omci_uint16_t instanceId)
{
    ca_omci_meclass_t *meclass = NULL;
    ca_omci_mebase_t  *meinst  = NULL;

    meclass = ca_omci_me_get(classId);

    if (NULL == meclass)
    {
        return NULL;
    }

    if (meclass->instHash && meclass->instHash->count)
    {
        meinst = HASH_Lookup(meclass->instHash, &instanceId);
        return meinst;
    }

    return NULL;
}


ca_omci_st_e ca_omci_me_entry_add(ca_omci_mebase_t *instance)
{
    ca_omci_meclass_t *meclass = NULL;
    ca_omci_mebase_t  *meinst  = NULL;

    OMCI_PARAM_NULL_RETURN(instance);

    meclass = ca_omci_me_get(instance->classId);
    if (NULL == meclass)
    {
        OMCIERR("can't support this class");
        return CA_OMCI_E;
    }

    /*insert the instance to the instance list */
    meinst = (ca_omci_mebase_t *)HASH_Lookup(meclass->instHash, &instance->instanceId);
    if (NULL != meinst)
    {
        OMCIERR("can't insert same instance for class[%s] instance[%d]", meclass->desc, instance->instanceId);
        return CA_OMCI_E;
    }

    HASH_Insert(meclass->instHash, instance);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_entry_delete(ca_omci_uint16_t classId, ca_omci_uint16_t instanceId)
{
    ca_omci_meclass_t *meclass = NULL;

    meclass = ca_omci_me_get(classId);
    OMCI_PARAM_NULL_RETURN(meclass);

    if (meclass->instHash && meclass->instHash->count)
    {
        HASH_RemoveByKey(meclass->instHash, &instanceId);
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_class_init(void)
{
    ca_omci_uint32_t   i       = 0;
    ca_omci_meclass_t *meclass = NULL;

    for (i = 0; i < OMCI_ME_CLASS_ID_MAX; i++)
    {
        meclass = &g_omci_me_list[i];
        meclass->initCb();
        meclass->instHash = HASH_Create(OMCI_INSTANCE_MAX, OFFSET(ca_omci_mebase_t, instanceId), OFFSET(ca_omci_mebase_t, next));
        if (NULL == meclass->instHash)
        {
            OMCIERR("No memory");
            return CA_OMCI_E_NO_MEMORY;
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_init(void)
{
    ca_omci_st_e ret = CA_OMCI_OK;

    ret = ca_omci_me_class_init();
    OMCI_CHECK_NOK_RET(ret, "Me hash init failed");

    /* Create all onu auto create MEs */
    ret = ca_omci_me_onu_auto_create_init();
    OMCI_CHECK_NOK_RET(ret, "Init onu Mes failed");

    return ret;
}

