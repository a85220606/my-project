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
#ifndef __OMCI_TYPES_H__
#define __OMCI_TYPES_H__
#include "ca_types.h"


#define CA_LITTLE_ENDIAN


#define OMCI_MAC_ADDR_LEN                (6)
#define OMCI_EQID_LEN                    (20)
#define OMCI_VENDOR_ID_LEN               (4)
#define OMCI_VERSION_LEN                 (14)
#define OMCI_SN_LEN                      (8)
#define OMCI_PRODUCT_CODE_LEN            (25)
#define OMCI_IMAGE_HASH_LEN              (16)
#define OMCI_ETH_EXPECT_TYPE_AUTOSENSE   (0)
#define OMCI_TIME_OF_DAY_LEN             (14)
#define OMCI_LARGE_STR_LEN               (25)
#define OMCI_DSCP_TO_PBIT_LEN            (24)
#define OMCI_LOID_LEN                    (24)
#define OMCI_LOID_PW_LEN                 (12)
#define OMCI_PORT_LIST_LEN               (16)
#define OMCI_MD_NAME_LEN                 (25)
#define OMCI_VLAN_LIST_LEN               (24)
#define OMCI_CCM_TABLE_LEN               (128)
#define OMCI_RMEP_TABLE_NUM              (16)
#define OMCI_CHASSIS_ID_LEN              (25)
#define OMCI_MNG_DOMAIN_LEN              (25)
#define OMCI_MNG_ADDR_LEN                (25)
#define OMCI_OCT_STR_PART_LEN            (25)
#define OMCI_IPV6_LEAD_LEN               (12)
#define OMCI_IGMP_MAX_TBL_NUM            (10)
#define OMCI_IGMP_LOST_TBL_NUM           (20)
#define OMCI_ONU_ID_LEN                  (25)
#define OMCI_IPV6_ADDR_LEN               (16)
#define OMCI_MULTI_IP_ADDR_LEN           (16)
#define OMCI_CTL_BLOCK_LEN               (16)
#define OMCI_PQ_NUM_PER_TCONT            (32)
#define OMCI_PQ_NUM_PER_UNI              (8)
#define OMCI_GEM_MAX_PRI_NUM             (8)
#define OMCI_VLAN_LIST_NUM               (12)
#define OMCI_ACS_URL_LEN                 (128)
#define OMCI_ACS_USERNAME_LEN            (25)
#define OMCI_ACS_PASSWD_LEN              (25)
#define OMCI_OLT_TYPE_STR_LEN            (32)
#define OMCI_H248_MGC_LEN                (128)
#define OMCI_H248_TERM_ID_BASE_LEN       (25)
#define OMCI_H248_MSG_ID_LEN             (128)
#define OMCI_ISP_TYPE_LEN                (20)
#define OMCI_ENCRYPTION_AES_128          (1)
#define OMCI_ENCRYPTION_NONE             (0)
#define OMCI_STR_SPACE                   (0x20)
#define OMCI_IPV4_ADDR_LEN               (4)
#define OMCI_PSK_LEN                     (16)
#define OMCI_VOIP_ACCESS_CODE_LEN        (5)


#define ca_omci_htons(x)    ca_htons(x)
#define ca_omci_htonl(x)    ca_htonl(x)
#define ca_omci_htonll(x)   ca_htonll(x)
#define ca_omci_ntohs(x)    ca_ntohs(x)
#define ca_omci_ntohl(x)    ca_ntohl(x)
#define ca_omci_ntohll(x)   ca_ntohll(x)


typedef ca_char         ca_omci_int8_t;
typedef ca_uint8        ca_omci_uint8_t;
typedef ca_boolean      ca_omci_bool_t;
typedef ca_uint16       ca_omci_uint16_t;
typedef ca_int16        ca_omci_int16_t;
typedef ca_uint32       ca_omci_uint32_t;
typedef ca_int32        ca_omci_int32_t;
typedef ca_uint64       ca_omci_uint64_t;
typedef ca_int64        ca_omci_int64_t;




typedef enum
{
    CA_OMCI_FALSE = 0,
    CA_OMCI_TRUE  = 1,
}ca_omci_flag_e;


typedef enum
{
    CA_OMCI_UNLOCK    = 0,
    CA_OMCI_LOCK      = 1,
    CA_OMCI_SHUT_DOWN = 2,
}ca_omci_admin_e;


typedef enum
{
    CA_OMCI_OPER_ENABLE  = 0,
    CA_OMCI_OPER_DISABLE = 1,
    CA_OMCI_OPER_UNKNOWN = 1,
}ca_omci_oper_state_e;


typedef enum
{
    CA_OMCI_POLICY_NULL  = 0,
    CA_OMCI_POLICY_SP    = 1,
    CA_OMCI_POLICY_WRR   = 2,
}ca_omci_policy_e;


typedef enum
{
    CA_OMCI_MHF_NONE     = 0,
    CA_OMCI_MHF_DEFAULT  = 1,
    CA_OMCI_MHF_EXPLICIT = 2,
    CA_OMCI_MHF_DEFER    = 3,
}ca_omci_mhf_e;


typedef enum
{
    CA_OMCI_SENDER_NONE        = 1,
    CA_OMCI_SENDER_CHASSIS     = 2,
    CA_OMCI_SENDER_MNG         = 3,
    CA_OMCI_SENDER_CHASSIS_MNG = 4,
    CA_OMCI_SENDER_DEFER       = 5,
}ca_omci_sender_e;


typedef enum
{
    CA_OMCI_L2_MAC_SRV = 0,
    CA_OMCI_L2_8021P   = 1,
}ca_omci_l2_type_e;


typedef enum
{
    MULTICAST_CFG_TBL_CTL_SET     = 1,
    MULTICAST_CFG_TBL_CTL_DEL     = 2,
    MULTICAST_CFG_TBL_CTL_DEL_ALL = 3,
}ca_omci_multicast_cfg_tbl_ctl_e;


typedef enum
{
    MULTICAST_ROW_PART_0 = 0,
    MULTICAST_ROW_PART_1 = 1,
    MULTICAST_ROW_PART_2 = 2,
}ca_omci_multicast_row_part_e;


typedef enum
{
    CA_OMCI_OK = 0,
    CA_OMCI_E_FORMAT,
    CA_OMCI_E_NO_MEMORY,
    CA_OMCI_E_PARAM,
    CA_OMCI_E_NOT_FOUND,
    CA_OMCI_E = -1,
}ca_omci_st_e;


typedef enum
{
    CA_OMCI_COMMON   = 0,
    CA_OMCI_CODEC    = 1,
    CA_OMCI_MNG      = 2,
    CA_OMCI_ANALYSIS = 3,
    CA_OMCI_HAL      = 4,
    CA_OMCI_AM       = 5,
    CA_OMCI_PM       = 6,
    CA_OMCI_UPG      = 7,
    CA_OMCI_M_MAX,
}ca_omci_module_e;
#endif /* __OMCI_TYPES_H__ */

