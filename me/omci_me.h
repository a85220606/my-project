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
#ifndef __OMCI_ME_H__
#define __OMCI_ME_H__


typedef enum
{
    OMCI_ME_CLASS_ID_MIN,
    OMCI_ME_ONU_DATA_CLASS_ID = OMCI_ME_CLASS_ID_MIN, /* 0 */
    OMCI_ME_CARDHOLDER_CLASS_ID,          /* 1 */
    OMCI_ME_CIRCUIT_PACK_CLASS_ID,        /* 2 */
    OMCI_ME_SOFT_IMAGE_CLASS_ID,          /* 3 */
    OMCI_ME_PPTP_ETH_CLASS_ID,            /* 4 */
    OMCI_ME_ETH_PM_HIS_CLASS_ID,          /* 5 */
    OMCI_ME_MAC_BRG_SRV_CLASS_ID,         /* 6 */
    OMCI_ME_MAC_BRG_CFG_CLASS_ID,         /* 7 */
    OMCI_ME_MAC_PORT_CFG_CLASS_ID,        /* 8 */
    OMCI_ME_MAC_PORT_DES_CLASS_ID,        /* 9 */
    OMCI_ME_MAC_PORT_FILTER_CLASS_ID,     /* 10 */
    OMCI_ME_MAC_PORT_BRG_CLASS_ID,        /* 11 */
    OMCI_ME_MAC_BRG_PM_CLASS_ID,          /* 12 */
    OMCI_ME_MAC_PORT_PM_CLASS_ID,         /* 13 */
    OMCI_ME_PPTP_POTS_CLASS_ID,           /* 14 */
    OMCI_ME_VOICE_SRV_PROF_CLASS_ID,      /* 15 */
    OMCI_ME_VLAN_TAG_OPER_CLASS_ID,       /* 16 */
    OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, /* 17 */
    OMCI_ME_VLAN_TAG_FILTER_CLASS_ID,     /* 18 */
    OMCI_ME_ETH_PM_HIS2_CLASS_ID,         /* 19 */
    OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID,      /* 20 */
    OMCI_ME_OLT_G_CLASS_ID,               /* 21 */
    OMCI_ME_ONU_POWER_SHED_CLASS_ID,      /* 22 */
    OMCI_ME_IP_HOST_CLASS_ID,             /* 23 */
    OMCI_ME_IP_HOST_PM_CLASS_ID,          /* 24 */
    OMCI_ME_TCP_UDP_CFG_CLASS_ID,         /* 25 */
    OMCI_ME_NET_ADDR_CLASS_ID,            /* 26 */
    OMCI_ME_VOIP_CFG_CLASS_ID,            /* 27 */
    OMCI_ME_VOIP_CTP_CLASS_ID,            /* 28 */
    OMCI_ME_CALL_CTL_PM_CLASS_ID,         /* 29 */
    OMCI_ME_VOIP_LINE_ST_CLASS_ID,        /* 30 */
    OMCI_ME_VOIP_MEDIA_PROF_CLASS_ID,     /* 31 */
    OMCI_ME_RTP_PROF_DATA_CLASS_ID,       /* 32 */
    OMCI_ME_RTP_PM_CLASS_ID,              /* 33 */
    OMCI_ME_NET_DIAL_PLAN_CLASS_ID,       /* 34 */
    OMCI_ME_VOIP_APP_SRV_CLASS_ID,        /* 35 */
    OMCI_ME_VOIP_ACCES_CODE_CLASS_ID,     /* 36 */
    OMCI_ME_AUTH_SCY_CLASS_ID,            /* 37 */
    OMCI_ME_SIP_CFG_PORTAL_CLASS_ID,      /* 38 */
    OMCI_ME_SIP_AGT_CFG_CLASS_ID,         /* 39 */
    OMCI_ME_SIP_AGT_PM_CLASS_ID,          /* 40 */
    OMCI_ME_SIP_CALL_INIT_PM_CLASS_ID,    /* 41 */
    OMCI_ME_SIP_USR_CLASS_ID,             /* 42 */
    OMCI_ME_MGC_CFG_PORTAL_CLASS_ID,      /* 43 */
    OMCI_ME_MGC_CFG_DATA_CLASS_ID,        /* 44 */
    OMCI_ME_MGC_PM_CLASS_ID,              /* 45 */
    OMCI_ME_LARGE_STRING_CLASS_ID,        /* 46 */
    OMCI_ME_ONU_REMOTE_DEBUG_CLASS_ID,    /* 47 */
    OMCI_ME_EQ_PROT_PROF_CLASS_ID,        /* 48 */
    OMCI_ME_EQ_EXT_PKG_CLASS_ID,          /* 49 */
    OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID,     /* 50 */
    OMCI_ME_ONU_G_CLASS_ID,               /* 51 */
    OMCI_ME_ONU2_G_CLASS_ID,              /* 52 */
    OMCI_ME_TCONT_CLASS_ID,               /* 53 */
    OMCI_ME_ANI_G_CLASS_ID,               /* 54 */
    OMCI_ME_UNI_G_CLASS_ID,               /* 55 */
    OMCI_ME_GEM_IWTP_CLASS_ID,            /* 56 */
    OMCI_ME_GEMPORT_PM_CLASS_ID,          /* 57 */
    OMCI_ME_GEM_NET_CTP_CLASS_ID,         /* 58 */
    OMCI_ME_GAL_ETH_PROF_CLASS_ID,        /* 59 */
    OMCI_ME_THRESHOLD1_CLASS_ID,          /* 60 */
    OMCI_ME_THRESHOLD2_CLASS_ID,          /* 61 */
    OMCI_ME_GAL_ETH_PM_CLASS_ID,          /* 62 */
    OMCI_ME_PRI_QUEUE_CLASS_ID,           /* 63 */
    OMCI_ME_TRAFFIC_SCHEG_CLASS_ID,       /* 64 */
    OMCI_ME_PROTECT_DATA_CLASS_ID,        /* 65 */
    OMCI_ME_TRAFFIC_DESCRIPT_CLASS_ID,    /* 66 */
    OMCI_ME_MULTI_GEM_IWTP_CLASS_ID,      /* 67 */
    OMCI_ME_OMCI_CLASS_ID,                /* 68 */
    OMCI_ME_DOT1X_PORT_EXT_PKG_CLASS_ID,  /* 69 */
    OMCI_ME_DOT1X_PM_HIS_CLASS_ID,        /* 70 */
    OMCI_ME_ETH_PM_HIS3_CLASS_ID,         /* 71 */
    OMCI_ME_PORT_MAP_PKG_CLASS_ID,        /* 72 */
    OMCI_ME_DOT1_RATE_LIMITER_CLASS_ID,   /* 73 */
    OMCI_ME_DOT1AG_DOMAIN_CLASS_ID,       /* 74 */
    OMCI_ME_DOT1AG_ASSO_CLASS_ID,         /* 75 */
    OMCI_ME_DOT1AG_MD_LEVEL_CLASS_ID,     /* 76 */
    OMCI_ME_DOT1AG_MEP_CLASS_ID,          /* 77 */
    OMCI_ME_DOT1AG_MEP_ST_CLASS_ID,       /* 78 */
    OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID,      /* 79 */
    OMCI_ME_DOT1AG_CFM_CLASS_ID,          /* 80 */
    OMCI_ME_DOT1AG_CHAS_INFO_CLASS_ID,    /* 81 */
    OMCI_ME_OCT_STR_CLASS_ID,             /* 82 */
    OMCI_ME_GEN_PUR_BUFF_CLASS_ID,        /* 83 */
    OMCI_ME_MULTICAST_OPER_PROF_CLASS_ID, /* 84 */
    OMCI_ME_MULTI_SUB_CFG_INFO_CLASS_ID,  /* 85 */
    OMCI_ME_MULTI_SUB_MONI_CLASS_ID,      /* 86 */
    OMCI_ME_FEC_PM_CLASS_ID,              /* 87 */
    OMCI_ME_FILE_TRANS_CTL_CLASS_ID,      /* 88 */
    OMCI_ME_ETH_FRM_PM_DS_CLASS_ID,       /* 89 */
    OMCI_ME_ETH_FRM_PM_US_CLASS_ID,       /* 90 */
    OMCI_ME_VEIP_CLASS_ID,                /* 91 */
    OMCI_ME_GEN_ST_PORTAL_CLASS_ID,       /* 92 */
    OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID,     /* 93 */
    OMCI_ME_ETH_FRAME_PM_CLASS_ID,        /* 94 */
    OMCI_ME_SNMP_CFG_CLASS_ID,            /* 95 */
    OMCI_ME_DYN_POWER_CTL_CLASS_ID,       /* 96 */
    OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID,     /* 97 */
    OMCI_ME_GPN_CTP_PM_CLASS_ID,          /* 98 */
    OMCI_ME_TCP_UDP_PM_CLASS_ID,          /* 99 */
    OMCI_ME_ENER_CONSU_PM_CLASS_ID,       /* 100 */
    OMCI_ME_XGPON_TC_PM_CLASS_ID,         /* 101 */
    OMCI_ME_XGPON_DS_MNG_PM_CLASS_ID,     /* 102 */
    OMCI_ME_XGPON_US_MNG_PM_CLASS_ID,     /* 103 */
    OMCI_ME_IPV6_HOST_CLASS_ID,           /* 104 */
    OMCI_ME_BRV6_PRE_ASSI_CLASS_ID,       /* 105 */
    OMCI_ME_POE_CTRL_CLASS_ID,            /* 106 */
    OMCI_ME_ONU3_G_CLASS_ID,              /* 107 */
    OMCI_ME_ENHANCE_FEC_PM_CLASS_ID,      /* 108 */
    OMCI_ME_ENHANCE_TC_PM_CLASS_ID,       /* 109 */
    OMCI_ME_SYNC_E_OPER_CLASS_ID,         /* 110 */
    OMCI_ME_PTP_CLASS_ID,                 /* 111 */
    OMCI_ME_PTP_STATUS_CLASS_ID,          /* 112 */
    OMCI_ME_ALCL_ONT_GENV2_CLASS_ID,      /* 113 */
    OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID,   /* 114 */
    OMCI_ME_CLASS_ID_MAX,
}ca_omci_class_id_e;


typedef struct
{
    ca_omci_uint16_t    standardClassId;
    ca_omci_class_id_e  classId;
}ca_omci_me_classid_map_t;


ca_omci_class_id_e ca_omci_me_get_inner_class(ca_omci_uint16_t standardClass);
ca_omci_uint16_t ca_omci_me_get_standard_class(ca_omci_class_id_e innerClass);
ca_omci_st_e ca_omci_me_class_map_adjust(ca_omci_uint16_t standardClass, ca_omci_class_id_e innerClass);
const ca_omci_int8_t * ca_omci_me_desc_get(ca_omci_uint16_t classId);
ca_omci_meclass_t *ca_omci_me_get(ca_omci_uint16_t classId);
ca_omci_mebase_t *ca_omci_me_new(ca_omci_uint16_t classId);
void ca_omci_me_delete(ca_omci_mebase_t *base);
ca_omci_st_e ca_omci_me_finish(void);
ca_omci_uint32_t ca_omci_me_entry_num_get(ca_omci_uint16_t classId);
ca_omci_mebase_t *ca_omci_me_entry_lookup(ca_omci_uint16_t classId, ca_omci_uint16_t instanceId);
ca_omci_st_e ca_omci_me_entry_add(ca_omci_mebase_t *instance);
ca_omci_st_e ca_omci_me_entry_delete(ca_omci_uint16_t classId, ca_omci_uint16_t instanceId);
ca_omci_st_e ca_omci_me_init(void);
#endif /* __OMCI_ME_H__ */

