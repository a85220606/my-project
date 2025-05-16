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
#ifndef __OMCI_ME_CARDHOLDER_H__
#define __OMCI_ME_CARDHOLDER_H__


#define OMCI_CARDHOLDER_SET_MASK_FULL     29568
#define OMCI_CARDHOLDER_GET_MASK_FULL     65408


extern ca_omci_me_attr_t g_omci_me_cardholder_attr[OMCI_ME_ATTR9];


typedef enum
{
    CARDHOLDER_UNIT_TYPE_NO_LIM               = 0,
    CARDHOLDER_UNIT_TYPE_ATM_015              = 1,
    CARDHOLDER_UNIT_TYPE_ATM_020              = 2,
    CARDHOLDER_UNIT_TYPE_ATM_063              = 3,
    CARDHOLDER_UNIT_TYPE_ATM_063_U            = 4,
    CARDHOLDER_UNIT_TYPE_ATM_080              = 5,
    CARDHOLDER_UNIT_TYPE_ATM_250              = 6,
    CARDHOLDER_UNIT_TYPE_ATM_340              = 7,
    CARDHOLDER_UNIT_TYPE_ATM_450              = 8,
    CARDHOLDER_UNIT_TYPE_ATM_450_340          = 9,
    CARDHOLDER_UNIT_TYPE_ATM_STM1_SMF         = 10,
    CARDHOLDER_UNIT_TYPE_ATM_STM1_MMF         = 11,
    CARDHOLDER_UNIT_TYPE_ATM_STM1_UTP         = 12,
    CARDHOLDER_UNIT_TYPE_CBR_DS1              = 13,
    CARDHOLDER_UNIT_TYPE_CBR_E1               = 14,
    CARDHOLDER_UNIT_TYPE_CBR_J2               = 15,
    CARDHOLDER_UNIT_TYPE_CBR_DS1_E1           = 16,
    CARDHOLDER_UNIT_TYPE_CBR_DS1_E1_J1        = 17,
    CARDHOLDER_UNIT_TYPE_CBR_J2_U             = 18,
    CARDHOLDER_UNIT_TYPE_CBR_192K             = 19,
    CARDHOLDER_UNIT_TYPE_CBR_DS3              = 20,
    CARDHOLDER_UNIT_TYPE_CBR_E3               = 21,
    CARDHOLDER_UNIT_TYPE_10_BT                = 22,
    CARDHOLDER_UNIT_TYPE_100_BT               = 23,
    CARDHOLDER_UNIT_TYPE_10_100_BT            = 24,
    CARDHOLDER_UNIT_TYPE_TOKEN_RING           = 25,
    CARDHOLDER_UNIT_TYPE_FDDI                 = 26,
    CARDHOLDER_UNIT_TYPE_FRAME_RELAY          = 27,
    CARDHOLDER_UNIT_TYPE_CBR_J1               = 28,
    CARDHOLDER_UNIT_TYPE_ATM_OC3_SMF          = 29,
    CARDHOLDER_UNIT_TYPE_ATM_OC3_MMF          = 30,
    CARDHOLDER_UNIT_TYPE_ATM_OC3_UTP          = 31,
    CARDHOLDER_UNIT_TYPE_POTS                 = 32,
    CARDHOLDER_UNIT_TYPE_ISDN_BRI             = 33,
    CARDHOLDER_UNIT_TYPE_GB_ETH               = 34,
    CARDHOLDER_UNIT_TYPE_XDSL                 = 35,
    CARDHOLDER_UNIT_TYPE_SHDSL                = 36,
    CARDHOLDER_UNIT_TYPE_VDSL                 = 37,
    CARDHOLDER_UNIT_TYPE_VIDEO                = 38,
    CARDHOLDER_UNIT_TYPE_LCT                  = 39,
    CARDHOLDER_UNIT_TYPE_802_11               = 40,
    CARDHOLDER_UNIT_TYPE_XDSL_POTS            = 41,
    CARDHOLDER_UNIT_TYPE_VDSL_POTS            = 42,
    CARDHOLDER_UNIT_TYPE_COM_EQUIP            = 43,
    CARDHOLDER_UNIT_TYPE_COMB_VEDIO_UNI_PON   = 44,
    CARDHOLDER_UNIT_TYPE_MIX_SVC_EQP          = 45,
    CARDHOLDER_UNIT_TYPE_MOCA                 = 46,
    CARDHOLDER_UNIT_TYPE_10_100_1000_BT       = 47,
    CARDHOLDER_UNIT_TYPE_VEIP                 = 48,
    CARDHOLDER_UNIT_TYPE_10G_ETH              = 49,
    CARDHOLDER_UNIT_TYPE_2G5_ETH              = 50,
    CARDHOLDER_UNIT_TYPE_5G_ETH               = 51,
    CARDHOLDER_UNIT_TYPE_25G_ETH              = 52,
    CARDHOLDER_UNIT_TYPE_40G_ETH              = 53,
    CARDHOLDER_UNIT_TYPE_1000BASE_LX          = 54,
    CARDHOLDER_UNIT_TYPE_1000BASE_SX          = 55,
    CARDHOLDER_UNIT_TYPE_10G_SR               = 56,
    CARDHOLDER_UNIT_TYPE_10G_LX4              = 57,
    CARDHOLDER_UNIT_TYPE_10G_LRM              = 58,
    CARDHOLDER_UNIT_TYPE_10G_LR               = 59,
    CARDHOLDER_UNIT_TYPE_10G_ER               = 60,
    CARDHOLDER_UNIT_TYPE_10G_SW               = 61,
    CARDHOLDER_UNIT_TYPE_10G_LW               = 62,
    CARDHOLDER_UNIT_TYPE_10G_EW               = 63,
    CARDHOLDER_UNIT_TYPE_25G_SR               = 64,
    CARDHOLDER_UNIT_TYPE_40G_SR4              = 65,
    CARDHOLDER_UNIT_TYPE_40G_LR4              = 66,
    CARDHOLDER_UNIT_TYPE_40G_ER4              = 67,
    CARDHOLDER_UNIT_TYPE_10G_MULTI_RATE       = 68,
    CARDHOLDER_UNIT_TYPE_10G_OPTICAL          = 69,
    CARDHOLDER_UNIT_TYPE_40G_OPTICAL          = 70,
    CARDHOLDER_UNIT_TYPE_GFAST                = 71,
    CARDHOLDER_UNIT_TYPE_25G_OPTICAL          = 72,
    CARDHOLDER_UNIT_TYPE_50G_OPTICAL          = 73,
    CARDHOLDER_UNIT_TYPE_25G_LR               = 74,
    CARDHOLDER_UNIT_TYPE_25G_ER               = 75,
    CARDHOLDER_UNIT_TYPE_50G_SR               = 76,
    CARDHOLDER_UNIT_TYPE_50G_FR               = 77,
    CARDHOLDER_UNIT_TYPE_50G_LR               = 78,
    CARDHOLDER_UNIT_TYPE_50G_ER               = 79,
    CARDHOLDER_UNIT_TYPE_10G_BR10             = 80,
    CARDHOLDER_UNIT_TYPE_10G_BR20             = 81,
    CARDHOLDER_UNIT_TYPE_10G_BR40             = 82,
    CARDHOLDER_UNIT_TYPE_25G_BR10             = 83,
    CARDHOLDER_UNIT_TYPE_25G_BR20             = 84,
    CARDHOLDER_UNIT_TYPE_25G_BR40             = 85,
    CARDHOLDER_UNIT_TYPE_50G_BR10             = 86,
    CARDHOLDER_UNIT_TYPE_50G_BR20             = 87,
    CARDHOLDER_UNIT_TYPE_50G_BR40             = 88,
    CARDHOLDER_UNIT_TYPE_25GPON_25G_10        = 222,
    CARDHOLDER_UNIT_TYPE_25GPON_25G_25        = 223,
    CARDHOLDER_UNIT_TYPE_HSP50G50             = 227,
    CARDHOLDER_UNIT_TYPE_HSP50G25             = 228,
    CARDHOLDER_UNIT_TYPE_HSP50G12             = 229,
    CARDHOLDER_UNIT_TYPE_MULTI_PON            = 230,
    CARDHOLDER_UNIT_TYPE_PTP_WDM_PON_CLS1     = 231,
    CARDHOLDER_UNIT_TYPE_PTP_WDM_PON_CLS2     = 232,
    CARDHOLDER_UNIT_TYPE_PTP_WDM_PON_CLS3     = 233,
    CARDHOLDER_UNIT_TYPE_TWMP10G2             = 234,
    CARDHOLDER_UNIT_TYPE_TWDM10G10            = 235,
    CARDHOLDER_UNIT_TYPE_TWDM2G2              = 236,
    CARDHOLDER_UNIT_TYPE_XGPON_10G_2488       = 237,
    CARDHOLDER_UNIT_TYPE_XGPON_10G_10         = 238,
    CARDHOLDER_UNIT_TYPE_MIDS_PON_EXT_UNI     = 239,
    CARDHOLDER_UNIT_TYPE_MIDS_PON_EXT_ANI     = 240,
    CARDHOLDER_UNIT_TYPE_MIDS_PON_EXT_US_OPT  = 241,
    CARDHOLDER_UNIT_TYPE_MIDS_PON_2488_DS_OPT = 242,
    CARDHOLDER_UNIT_TYPE_GPON_ASYM_1244_155   = 243,
    CARDHOLDER_UNIT_TYPE_GPON_ASYM_1244_622   = 244,
    CARDHOLDER_UNIT_TYPE_PON_SYM_1244         = 245,
    CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_155   = 246,
    CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_622   = 247,
    CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_1244  = 248,
    CARDHOLDER_UNIT_TYPE_PON_SYM_2488         = 249,
    CARDHOLDER_UNIT_TYPE_PON_ASYM_1244_155    = 250,
    CARDHOLDER_UNIT_TYPE_PON_ASYM_1244_622    = 251,
    CARDHOLDER_UNIT_TYPE_PON_SYM_622          = 252,
    CARDHOLDER_UNIT_TYPE_PON_SYM_155          = 253,
    CARDHOLDER_UNIT_TYPE_PON_ASYM_155_622     = 254,
    CARDHOLDER_UNIT_TYPE_PNP                  = 255,
}ca_omci_cardholder_unit_type_e;


typedef struct
{
    ca_omci_uint8_t actualUnitType;
    ca_omci_uint8_t exceptUnitType;
    ca_omci_uint8_t exceptPortNum;
    ca_omci_uint8_t expectEqId[OMCI_EQID_LEN];
    ca_omci_uint8_t actualEqId[OMCI_EQID_LEN];
    ca_omci_uint8_t protecProfPoint;
    ca_omci_uint8_t invokeProtectSwitch;
    ca_omci_uint8_t arc;
    ca_omci_uint8_t arcInterval;
}ca_omci_cardholder_attr_t;


typedef struct
{
    ca_omci_mebase_t          meBase;
    ca_omci_cardholder_attr_t attr;
}ca_omci_cardholder_t;


void              ca_omci_me_cardholder_init(void);
ca_omci_mebase_t *ca_omci_me_cardholder_new (void);
#endif /* __OMCI_ME_CARDHOLDER_H__ */

