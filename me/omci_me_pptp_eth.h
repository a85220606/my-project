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
#ifndef __OMCI_ME_PPTP_ETH_H__
#define __OMCI_ME_PPTP_ETH_H__


#define OMCI_PPTP_ETH_SET_MASK_FULL     47614
#define OMCI_PPTP_ETH_GET_MASK_FULL     65534


extern ca_omci_me_attr_t g_omci_me_pptp_eth_attr[OMCI_ME_ATTR15];


typedef enum
{
    PPTP_ETH_AUTO_DETEC_CFG_AUTO_AUTO       = 0x00,
    PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_FULL   = 0x01,
    PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_FULL  = 0x02,
    PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_FULL = 0x03,
    PPTP_ETH_AUTO_DETEC_CFG_AUTO_FULL       = 0x04,
    PPTP_ETH_AUTO_DETEC_CFG_10G_ONLY_FULL   = 0x05,
    PPTP_ETH_AUTO_DETEC_CFG_2G5_ONLY_FULL   = 0x06,
    PPTP_ETH_AUTO_DETEC_CFG_5G_ONLY_FULL    = 0x07,
    PPTP_ETH_AUTO_DETEC_CFG_25G_ONLY_FULL   = 0x08,
    PPTP_ETH_AUTO_DETEC_CFG_40G_ONLY_FULL   = 0x09,
    PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_AUTO   = 0x10,
    PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_HALF   = 0x11,
    PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_HALF  = 0x12,
    PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_HALF = 0x13,
    PPTP_ETH_AUTO_DETEC_CFG_AUTO_HALF       = 0x14,
    PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_AUTO = 0x20,
    PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_AUTO  = 0x30,
}ca_omci_pptp_eth_auto_detec_cfg_e;


typedef enum
{
    PPTP_ETH_CFG_IND_10BASE_T_FULL  = 0x01,
    PPTP_ETH_CFG_IND_100BASE_T_FULL = 0x02,
    PPTP_ETH_CFG_IND_GE_FULL        = 0x03,
    PPTP_ETH_CFG_IND_10G_ETH_FULL   = 0x04,
    PPTP_ETH_CFG_IND_2G5_ETH_FULL   = 0x05,
    PPTP_ETH_CFG_IND_5G_ETH_FULL    = 0x06,
    PPTP_ETH_CFG_IND_25G_ETH_FULL   = 0x07,
    PPTP_ETH_CFG_IND_40G_ETH_FULL   = 0x08,
    PPTP_ETH_CFG_IND_10BASE_T_HALF  = 0x11,
    PPTP_ETH_CFG_IND_100BASE_T_HALF = 0x12,
    PPTP_ETH_CFG_IND_GE_HALF        = 0x13,
}ca_omci_pptp_eth_cfg_ind_e;


typedef enum
{
    PPTP_ETH_DTE_DCE_IND_DCE_MDI_X   = 0,
    PPTP_ETH_DTE_DCE_IND_DTE_MDI     = 1,
    PPTP_ETH_DTE_DCE_IND_AUTO_SELECT = 2,
}ca_omci_pptp_eth_dte_dce_ind_e;


typedef struct
{
    ca_omci_uint8_t  expectType;
    ca_omci_uint8_t  sensedType;
    ca_omci_uint8_t  autoDetectCfg;
    ca_omci_uint8_t  ethLoopbackCfg;
    ca_omci_uint8_t  admin;
    ca_omci_uint8_t  operState;
    ca_omci_uint8_t  cfgInd;
    ca_omci_uint16_t maxFrameSize;
    ca_omci_uint8_t  dteDceInd;
    ca_omci_uint16_t pauseTime;
    ca_omci_uint16_t brgIpInd;
    ca_omci_uint8_t  arc;
    ca_omci_uint8_t  arcInterval;
    ca_omci_uint8_t  pppoeFilter;
    ca_omci_uint8_t  powerCtrl;
}ca_omci_pptp_eth_attr_t;


typedef struct
{
    ca_omci_mebase_t        meBase;
    ca_omci_pptp_eth_attr_t attr;
}ca_omci_pptp_eth_t;


void              ca_omci_me_pptp_eth_init(void);
ca_omci_mebase_t *ca_omci_me_pptp_eth_new (void);
#endif /* __OMCI_ME_PPTP_ETH_H__ */

