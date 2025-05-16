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
#ifndef __OMCI_ME_MULTICAST_OPER_PROF_H__
#define __OMCI_ME_MULTICAST_OPER_PROF_H__


#define OMCI_MULTICAST_OPER_PROF_SET_MASK_FULL     65407
#define OMCI_MULTICAST_OPER_PROF_GET_MASK_FULL     65535

#define OMCI_MULTICAST_OPER_PROF_RESERVED   10
#define OMCI_MULTICAST_OPER_PROF_TBLCTLDATA 24

extern ca_omci_me_attr_t g_omci_me_multicast_oper_prof_attr[OMCI_ME_ATTR16];


typedef enum
{
    MULTICAST_OPER_PROF_IGMP_V1     = 1,
    MULTICAST_OPER_PROF_IGMP_V2     = 2,
    MULTICAST_OPER_PROF_IGMP_V3     = 3,
    MULTICAST_OPER_PROF_IGMP_MLD_V1 = 16,
    MULTICAST_OPER_PROF_IGMP_MLD_V2 = 17,
}ca_omci_multicast_oper_prof_igmp_e;


typedef enum
{
    MULTICAST_OPER_PROF_US_TAG_CTL_TRANSPARENT = 0,
    MULTICAST_OPER_PROF_US_TAG_CTL_ADD_TCI     = 1,
    MULTICAST_OPER_PROF_US_TAG_CTL_REPLACE_TCI = 2,
    MULTICAST_OPER_PROF_US_TAG_CTL_REPLACE_VID = 3,
}ca_omci_multicast_oper_prof_us_tag_ctl_e;


typedef struct
{
#ifdef CA_LITTLE_ENDIAN
    ca_omci_uint16_t rowKey:10;
    ca_omci_uint16_t test:1;
    ca_omci_uint16_t rowPartId:3;
    ca_omci_uint16_t setCtl:2;
#else
    ca_omci_uint16_t setCtl:2;
    ca_omci_uint16_t rowPartId:3;
    ca_omci_uint16_t test:1;
    ca_omci_uint16_t rowKey:10;
#endif
}ca_omci_acl_ctl_t;


typedef struct
{
    ca_omci_uint16_t tblCtl;
    union
    {
        struct
        {
            ca_omci_uint16_t gemId;
            ca_omci_uint16_t aniVlan;
            ca_omci_uint32_t srcIp;
            ca_omci_uint32_t dstIpRangeSt;
            ca_omci_uint32_t dstIpRangeEnd;
            ca_omci_uint32_t imGroupBw;
            ca_omci_uint16_t reserved;
        }row_part_0;
        struct
        {
            ca_omci_uint8_t  srcIpv6AddrLeadByte[OMCI_IPV6_LEAD_LEN];
            ca_omci_uint16_t previewLen;
            ca_omci_uint16_t previewRpTime;
            ca_omci_uint16_t previewRpCnt;
            ca_omci_uint16_t previewResetTime;
            ca_omci_uint16_t reserved;
        }row_part_1;
        struct
        {
            ca_omci_uint8_t  dstIpv6AddrLeadByte[OMCI_IPV6_LEAD_LEN];
            ca_omci_uint8_t  reserved[OMCI_MULTICAST_OPER_PROF_RESERVED];
        }row_part_2;
    }u;
}__attribute__((__packed__))ca_omci_acl_tbl_info_t;


typedef struct
{
    ca_omci_bool_t  valid;
    ca_omci_uint8_t tblCtlData[OMCI_MULTICAST_OPER_PROF_TBLCTLDATA];
}ca_omci_acl_tbl_t;


typedef struct
{
    ca_omci_bool_t   valid;
    ca_omci_uint16_t vlanId;
    ca_omci_uint32_t srcIpAddr;
    ca_omci_uint32_t dstMultiIp;
}ca_omci_lost_tbl_t;


typedef struct
{
    ca_omci_uint8_t  ctl;
    ca_omci_uint16_t tci;
}ca_omci_ds_tci_t;


typedef struct
{
    ca_omci_uint8_t    igmpVer;
    ca_omci_uint8_t    igmpFunc;
    ca_omci_uint8_t    immediLeave;
    ca_omci_uint16_t   usTci;
    ca_omci_uint8_t    usTagCtl;
    ca_omci_uint32_t   usRate;
    ca_omci_acl_tbl_t  dynamicTbl[OMCI_IGMP_MAX_TBL_NUM];
    ca_omci_acl_tbl_t  staticTbl[OMCI_IGMP_MAX_TBL_NUM];
    ca_omci_lost_tbl_t lostTbl[OMCI_IGMP_LOST_TBL_NUM];
    ca_omci_uint8_t    robust;
    ca_omci_uint32_t   queryIpAddr;
    ca_omci_uint32_t   queryItv;
    ca_omci_uint32_t   queryMaxRspT;
    ca_omci_uint32_t   lastMbQueryItv;
    ca_omci_uint8_t    unauthJoinReqBehave;
    ca_omci_ds_tci_t   dsTci;
}ca_omci_multicast_oper_prof_attr_t;


typedef struct
{
    ca_omci_mebase_t                   meBase;
    ca_omci_next_t                     dynamicTbl;
    ca_omci_next_t                     staticTbl;
    ca_omci_next_t                     lostTbl;
    ca_omci_multicast_oper_prof_attr_t attr;
}ca_omci_multicast_oper_prof_t;


void              ca_omci_me_multicast_oper_prof_init(void);
ca_omci_mebase_t *ca_omci_me_multicast_oper_prof_new (void);
#endif /* __OMCI_ME_MULTICAST_OPER_PROF_H__ */

