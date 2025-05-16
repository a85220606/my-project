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
#ifndef __OMCI_ANALYSIS_FLOW_H__
#define __OMCI_ANALYSIS_FLOW_H__


#define CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM 32
#define CA_OMCI_DSCP_ENTRY_NUM            64


#define OMCI_CFG_FLOW_RETRY       (0)
#define OMCI_CFG_FLOW_UPDATE      (1)
#define OMCI_CFG_FLOW_OMCC_DOWN   (1 << 1)
#define OMCI_CFG_FLOW_UPDATE_VOIP (1 << 2)


#define OMCI_TCONT_SCHEDUL_NULL   (0)
#define OMCI_TCONT_SCHEDUL_SP     (1)
#define OMCI_TCONT_SCHEDUL_WRR    (2)


#define OMCI_CFG_STREAM_PORT_GEM     (1)
#define OMCI_CFG_STREAM_PORT_ETH     (2)
#define OMCI_CFG_STREAM_PORT_IPHOST  (3)
#define OMCI_CFG_STREAM_PORT_VEIP    (4)

#define OMCI_FILTER_PRI_NONE         (8)
#define OMCI_FILTER_PRI_DEF          (14)
#define OMCI_FILTER_PRI_IGNORE       (15)

#define OMCI_FILTER_VID_NONE         (4096)

#define OMCI_FILTER_TPID_DEI_NONE    (0) //000
#define OMCI_FILTER_TPID_DEI_8100    (4) //100
#define OMCI_FILTER_TPID_NOCARE_DEI  (5) //101
#define OMCI_FILTER_TPID_DEI_0       (6) //110
#define OMCI_FILTER_TPID_DEI_1       (7) //111

#define OMCI_FILTER_ETH_TYPE_NONE    (0)
#define OMCI_FILTER_ETH_TYPE_IPOE    (1)
#define OMCI_FILTER_ETH_TYPE_PPPOE   (2)
#define OMCI_FILTER_ETH_TYPE_ARP     (3)
#define OMCI_FILTER_ETH_TYPE_IPOE_V6 (4)

#define OMCI_TREATMENT_REMOVE_ZERO_TAG (0)
#define OMCI_TREATMENT_REMOVE_ONE_TAG  (1)
#define OMCI_TREATMENT_REMOVE_TWO_TAGS (2)
#define OMCI_TREATMENT_DISCARD         (3)

#define OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI  (8)
#define OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI  (9)
#define OMCI_TREATMENT_ADD_TAG_DSCP_2_PRI      (10)
#define OMCI_TREATMENT_NOT_ADD_TAG             (15)

#define OMCI_TREATMENT_COPY_VID_INNER    (4096)
#define OMCI_TREATMENT_COPY_VID_OUTER    (4097)

#define OMCI_TREATMENT_TPID_DEI_COPY_INNER     (0) //000
#define OMCI_TREATMENT_TPID_DEI_COPY_OUTER     (1) //001
#define OMCI_TREATMENT_TPID_SET_DEI_COPY_INNER (2) //010
#define OMCI_TREATMENT_TPID_SET_DEI_COPY_OUTER (3) //011
#define OMCI_TREATMENT_TPID_SET_8100           (4) //100
#define OMCI_TREATMENT_TPID_SET_88A8_DEI_0     (6) //110
#define OMCI_TREATMENT_TPID_SET_88A8_DEI_1     (7) //111


#define OMCI_FLOW_TYPE_GEM     (0)
#define OMCI_FLOW_TYPE_FILTER  (1)
#define OMCI_FLOW_TYPE_US_STR  (2)
#define OMCI_FLOW_TYPE_DS_STR  (3)


typedef enum
{
    CA_OMCI_UNTAG_FWD_DROP,
    CA_OMCI_UNTAG_FWD_TRANSPARENT,
    CA_OMCI_UNTAG_FWD_ADD_VID_PRI,
    CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI,
    CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_PRI,
    CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_INN_DSCP_PRI,
    CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_OUT_DSCP_PRI,
    CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_DSCP_PRI,
}ca_omci_untag_fwd_type_e;


typedef enum
{
    CA_OMCI_FILTER_NONE,           // Filter none, OMCI_FILTER_ETH_TYPE_NONE
    CA_OMCI_FILTER_IPOE_FRAMES,    // Filter IP,ethType == 0x0800, OMCI_FILTER_ETH_TYPE_IPOE
    CA_OMCI_FILTER_PPPOE_FRAMES,   // Filter PPPoE,ethType == 0x8863 or ethType == 0x8864, OMCI_FILTER_ETH_TYPE_PPPOE
    CA_OMCI_FILTER_ARP_FRAMES,     // Filter ARP,ethType == 0x0806, OMCI_FILTER_ETH_TYPE_ARP
    CA_OMCI_FILTER_IPOE_V6_FRAMES, // Filter IPv6, IPoE, ethType == 0x86dd, OMCI_FILTER_ETH_TYPE_IPOE_V6
}ca_omci_filter_eth_type_e;


typedef enum
{
    CA_OMCI_SGL_TAG_FWD_DROP,
    CA_OMCI_SGL_TAG_FWD_TRANSPARENT,
    CA_OMCI_SGL_TAG_FWD_MD_TAG,
    CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP,
    CA_OMCI_SGL_TAG_FWD_ADD_TAG,
    CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP,
    CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_TAG,
    CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_VID_DSCP,
    CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_TAG,
    CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_VID_DSCP,
    CA_OMCI_SGL_TAG_FWD_REMOVE_TAG,
}ca_omci_sgl_tag_fwd_type_e;


typedef enum
{
    CA_OMCI_DBL_TAG_FWD_DROP,
    CA_OMCI_DBL_TAG_FWD_TRANSPARENT,
    CA_OMCI_DBL_TAG_FWD_MD_OTAG,
    CA_OMCI_DBL_TAG_FWD_MD_OVID,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OPRI,
    CA_OMCI_DBL_TAG_FWD_MD_ODSCP,
    CA_OMCI_DBL_TAG_FWD_MD_TAGS,
    CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID,
    CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID_IDSCP,
    CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG,
    CA_OMCI_DBL_TAG_FWD_MD_OTAG_IPRI,
    CA_OMCI_DBL_TAG_FWD_MD_OTAG_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_ITAG,
    CA_OMCI_DBL_TAG_FWD_MD_VIDS,
    CA_OMCI_DBL_TAG_FWD_MD_VIDS_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_IPRI,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_ITAG,
    CA_OMCI_DBL_TAG_FWD_MD_VIDS_ODSCP,
    CA_OMCI_DBL_TAG_FWD_MD_VIDS_DSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_IPRI,
    CA_OMCI_DBL_TAG_FWD_MD_OVID_DSCP,
    CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID,
    CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP,
    CA_OMCI_DBL_TAG_FWD_RM_TAGS,
    CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI,
    CA_OMCI_DBL_TAG_FWD_RM_ITAG,
    CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP,
    CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG,
    CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID,
    CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_PRIS,
    CA_OMCI_DBL_TAG_FWD_MD_OPRI_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_ITAG,
    CA_OMCI_DBL_TAG_FWD_MD_IVID,
    CA_OMCI_DBL_TAG_FWD_MD_IVID_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_IPRI,
    CA_OMCI_DBL_TAG_FWD_MD_IDSCP,
    CA_OMCI_DBL_TAG_FWD_MD_ODSCP_ITAG,
    CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IVID,
    CA_OMCI_DBL_TAG_FWD_MD_DSCP_IVID,
    CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IPRI,
    CA_OMCI_DBL_TAG_FWD_MD_DSCP,
    CA_OMCI_DBL_TAG_FWD_RM_OTAG,
    CA_OMCI_DBL_TAG_ADD_TAG,
}ca_omci_dbl_tag_fwd_type_e;


typedef enum
{
    CA_OMCI_POLICER_MODE_DISABLE,          /* disable */
    CA_OMCI_POLICER_MODE_SRTCM,            /* rfc2697 color-blind mode */
    CA_OMCI_POLICER_MODE_TRTCM,            /* rfc2698 color-blind mode */
    CA_OMCI_POLICER_MODE_TRTCMDS,          /* rfc4115 (cflag = 0) color-blind mode */
    CA_OMCI_POLICER_MODE_TRTCMDS_COUPLED,  /* rfc4115 (cflag = 1) color-blind mode */
    CA_OMCI_POLICER_MODE_RATE_ONLY         /* rate limiter mode */
}ca_omci_policer_mode_e;


typedef enum
{
    CA_OMCI_GEMFLOW_CFG_INIT,
    CA_OMCI_GEMFLOW_CFG_REMAIN,
    CA_OMCI_GEMFLOW_CFG_MODIFY,
    CA_OMCI_GEMFLOW_CFG_REMOVE,
}ca_omci_gemflow_cfg_e;


typedef enum
{
    CA_OMCI_ANALYSIS_CFG_INIT,
    CA_OMCI_ANALYSIS_CFG_REMAIN,
    CA_OMCI_ANALYSIS_CFG_MODIFY,
    CA_OMCI_ANALYSIS_CFG_REMOVE,
}ca_omci_analysis_cfg_e;


typedef struct
{
    ca_omci_uint32_t tcontId;
    ca_omci_uint32_t allocId;
    ca_omci_uint32_t policy;
    ca_omci_uint32_t wrrQue[OMCI_PQ_NUM_PER_TCONT];
    ca_omci_uint32_t schedId;
    ca_omci_uint32_t schedPoint;
    ca_omci_uint32_t schedPolicy;
    ca_omci_uint32_t schedWeight;
    ca_omci_uint32_t cfgFlag;
}__attribute__((__packed__))ca_omci_tcont_cfg_t;


typedef struct
{
    ca_omci_policer_mode_e mode;
    ca_omci_bool_t         pps;    /* True - PPS mode, false - BPS mode */
    ca_omci_uint32_t       cir;
    ca_omci_uint32_t       cbs;
    ca_omci_uint32_t       pir;
    ca_omci_uint32_t       pbs;
}__attribute__((__packed__))ca_omci_traffic_des_t;


typedef struct
{
    ca_omci_uint32_t      gemId;
    ca_omci_uint32_t      tcontId;
    ca_omci_uint32_t      allocId;
    ca_omci_uint32_t      direction;
    ca_omci_uint32_t      usQueue;
    ca_omci_uint32_t      usQueueWeight;
    ca_omci_uint32_t      usQueueDpcm;
    ca_omci_traffic_des_t usTrafficDes;
    ca_omci_uint32_t      dsQueue;
    ca_omci_uint32_t      dsQueueWeight;
    ca_omci_uint32_t      dsQueueDpcm;
    ca_omci_traffic_des_t dsTrafficDes;
    ca_omci_bool_t        aesState;
    ca_omci_uint32_t      aesKeyRing;
    ca_omci_uint8_t       gemPri;
    ca_omci_uint16_t      vidList[OMCI_VLAN_LIST_NUM];
    ca_omci_bool_t        mcFlag;
    ca_omci_bool_t        transUntag;
    ca_omci_gemflow_cfg_e cfgFlag;
}__attribute__((__packed__))ca_omci_gem_flow_t;


typedef struct
{
    ca_omci_uint16_t vid;
    ca_omci_uint16_t pri;
}__attribute__((__packed__))ca_omci_vlan_entry_t;


typedef struct
{
    ca_omci_bool_t            isValid;
    ca_omci_untag_fwd_type_e  fwdType;
    ca_omci_filter_eth_type_e etherTypeFilter;
    ca_omci_vlan_entry_t      innerVlanTreatment;
    ca_omci_vlan_entry_t      outerVlanTreatment;
    ca_omci_uint32_t          dsTagAction;
}__attribute__((__packed__))ca_omci_untag_filter_t;


typedef struct
{
    ca_omci_bool_t              isValid;
    ca_omci_sgl_tag_fwd_type_e  fwdType;
    ca_omci_vlan_entry_t        vlanFilter;
    ca_omci_filter_eth_type_e   etherTypeFilter;
    ca_omci_vlan_entry_t        innerVlanTreatment;
    ca_omci_vlan_entry_t        outerVlanTreatment;
    ca_omci_uint32_t            dsTagAction;
}__attribute__((__packed__))ca_omci_sgl_tag_filter_t;


typedef struct
{
    ca_omci_bool_t             isValid;
    ca_omci_dbl_tag_fwd_type_e fwdType;
    ca_omci_vlan_entry_t       innerVlanFilter;
    ca_omci_vlan_entry_t       outerVlanFilter;
    ca_omci_filter_eth_type_e  etherTypeFilter;
    ca_omci_vlan_entry_t       innerVlanTreatment;
    ca_omci_vlan_entry_t       outerVlanTreatment;
}__attribute__((__packed__))ca_omci_dbl_tag_filter_t;



typedef struct
{
    ca_omci_uint8_t filter;
    ca_omci_uint8_t treat;
}__attribute__((__packed__))ca_omci_tpid_t;


typedef struct
{
    ca_omci_uint32_t         gemId;
    ca_omci_uint32_t         uniPortId;
    ca_omci_uint32_t         portType;

    ca_omci_bool_t           filterFlag;
    ca_omci_uint8_t          filterPri;
    ca_omci_untag_filter_t   untagFilterIpoe;
    ca_omci_untag_filter_t   untagFilterIpoev6;
    ca_omci_untag_filter_t   untagFilterPppoe;
    ca_omci_untag_filter_t   untagFilterArp;
    ca_omci_untag_filter_t   untagFilterDef;
    ca_omci_sgl_tag_filter_t sglTagFilter[CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM];
    ca_omci_sgl_tag_filter_t sglTagFilterDef;
    ca_omci_dbl_tag_filter_t dblTagFilter[CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM];
    ca_omci_dbl_tag_filter_t dblTagFilterDef;
    ca_omci_uint32_t         dsMode;
    ca_omci_uint16_t         inputTpid;
    ca_omci_uint16_t         outputTpid;
    ca_omci_tpid_t           innerTpid;
    ca_omci_tpid_t           outerTpid;

    ca_omci_analysis_cfg_e   cfgFlag;
    ca_omci_uint32_t         tcontId;
    ca_omci_uint32_t         usQueue;
    ca_omci_uint32_t         dsQueue;
    ca_omci_uint32_t         dsQueueWeight;
    ca_omci_bool_t           broadcastFlag; //only hal need to use this
}__attribute__((__packed__))ca_omci_stream_cfg_t;


typedef struct
{
    ca_omci_uint32_t gemId;
    ca_omci_uint32_t uniPortId;
    ca_omci_uint32_t portType;
    ca_omci_uint32_t usQueueDpcm;
    ca_omci_uint8_t  priBit;
    ca_omci_uint16_t vidList[OMCI_VLAN_LIST_NUM];
    ca_omci_uint8_t  direction;
    ca_omci_uint16_t inputTpid;
    ca_omci_uint16_t outputTpid;
    ca_omci_bool_t   externVlanFlag;
    ca_omci_uint32_t dsQueue;
    ca_omci_uint32_t dsQueueWeight;
    ca_omci_uint32_t dsQueueDpcm;

    ca_omci_analysis_cfg_e   cfgFlag;
}__attribute__((__packed__))ca_omci_filter_stream_cfg_t;



typedef struct
{
    omci_avl_table_t *tcCfg;
    omci_avl_table_t *gemFlow;
    omci_avl_table_t *usFlow;
    omci_avl_table_t *dsFlow;
    omci_avl_table_t *filterFlow;
}ca_omci_flow_t;


typedef struct
{
    ca_omci_uint16_t uniId;
    ca_omci_uint8_t  ipOpt;
    ca_omci_uint32_t ipAddr;
    ca_omci_uint32_t mask;
    ca_omci_uint32_t gateway;
    ca_omci_uint32_t priDns;
    ca_omci_uint32_t secDns;
}ca_omci_net_if_t;


typedef struct
{
    ca_omci_uint8_t  part[OMCI_LARGE_STR_LEN];
}ca_omci_large_str_part_t;


typedef struct
{
    ca_omci_uint32_t localIp;
    ca_omci_uint8_t  usrname[OMCI_ACS_USERNAME_LEN];
    ca_omci_uint8_t  passwd[OMCI_ACS_PASSWD_LEN];
    ca_omci_uint8_t  url[OMCI_ACS_URL_LEN];
}ca_omci_acs_cfg_t;


typedef struct
{
    ca_omci_uint8_t  usrname[OMCI_ACS_USERNAME_LEN];
    ca_omci_uint8_t  passwd[OMCI_ACS_PASSWD_LEN];
    ca_omci_uint8_t  url[OMCI_ACS_URL_LEN];
}ca_omci_non_omci_voip_t;


typedef struct
{
    ca_omci_uint32_t potsId;//bit map
    ca_omci_uint8_t  priMgc[OMCI_H248_MGC_LEN];
    ca_omci_uint8_t  sndMgc[OMCI_H248_MGC_LEN];
    ca_omci_net_if_t voipIphost;
    ca_omci_uint8_t  version;
    ca_omci_uint8_t  msgFormat;//this value should match the port in mgc config(H248 stack should check)
    ca_omci_uint16_t maxRetryTime;
    ca_omci_uint16_t maxRetryAttem;
    ca_omci_uint16_t srvChangeDelay;
    ca_omci_uint8_t  termIdBase[OMCI_H248_TERM_ID_BASE_LEN];
    ca_omci_uint32_t softswitch;
    ca_omci_uint8_t  msgId[OMCI_H248_MSG_ID_LEN];
}ca_omci_h248_voip_t;


typedef struct
{
    ca_omci_flow_t          flow;
    ca_omci_net_if_t        iphost;
    ca_omci_acs_cfg_t       acs;
    ca_omci_non_omci_voip_t nonOmciVoip;
    ca_omci_h248_voip_t     h248Voip;
    //pots or other value
}ca_omci_analyse_flow_t;


void ca_omci_update_flow(ca_omci_uint32_t flag);
void ca_omci_analysis_flow_cfg_tbl_finish(void);
void ca_omci_analysis_flow_cfg_tbl_init(void);
void ca_omci_analysis_tcont_reset(void);
void ca_omci_analysis_tcont_cfg(void);
void ca_omci_analysis_flow_cfg(void);
void ca_omci_analysis_flow_remove(void);
void ca_omci_analysis_filter_stream_cfg(void);
void ca_omci_analysis_filter_stream_remove(void);
void ca_omci_analysis_stream_cfg(void);
void ca_omci_analysis_stream_remove(void);
void ca_omci_analysis_tbl_realease(void);
void ca_omci_analysis_tr69_cfg(void);
#endif /* __OMCI_ANALYSIS_FLOW_H__ */

