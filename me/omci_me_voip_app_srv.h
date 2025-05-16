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
#ifndef __OMCI_ME_VOIP_APP_SRV_H__
#define __OMCI_ME_VOIP_APP_SRV_H__


#define OMCI_VOIP_APP_SRV_SET_MASK_FULL     65280
#define OMCI_VOIP_APP_SRV_GET_MASK_FULL     65280


extern ca_omci_me_attr_t g_omci_me_voip_app_srv_attr[OMCI_ME_ATTR8];


typedef enum
{
    VOIP_APP_SRV_CID_FEAT_DEF          = 0x00,
    VOIP_APP_SRV_CID_FEAT_CALLING_NUM  = 0x01,
    VOIP_APP_SRV_CID_FEAT_CALLING_NAME = 0x02,
    VOIP_APP_SRV_CID_FEAT_CID_BLOCKING = 0x04,
    VOIP_APP_SRV_CID_FEAT_CID_NUM      = 0x08,
    VOIP_APP_SRV_CID_FEAT_CID_NAME     = 0x10,
    VOIP_APP_SRV_CID_FEAT_ACR          = 0x20,
}ca_omci_voip_app_srv_cid_feat_e;


typedef enum
{
    VOIP_APP_SRV_CW_FEAT_DEF              = 0x00,
    VOIP_APP_SRV_CW_FEAT_CALL_WAITING     = 0x01,
    VOIP_APP_SRV_CW_FEAT_CID_ANNOUNCEMENT = 0x02,
}ca_omci_voip_app_srv_cw_feat_e;


typedef enum
{
    VOIP_APP_SRV_PROG_FEAT_DEF                 = 0x0000,
    VOIP_APP_SRV_PROG_FEAT_3WAY                = 0x0001,
    VOIP_APP_SRV_PROG_FEAT_CALL_TRANS          = 0x0002,
    VOIP_APP_SRV_PROG_FEAT_CALL_HOLD           = 0x0004,
    VOIP_APP_SRV_PROG_FEAT_CALL_PARK           = 0x0008,
    VOIP_APP_SRV_PROG_FEAT_DONT_DISTURB        = 0x0010,
    VOIP_APP_SRV_PROG_FEAT_FLASH_EMER_SRV_CALL = 0x0020,
    VOIP_APP_SRV_PROG_FEAT_EMER_SEV_HOLD       = 0x0040,
    VOIP_APP_SRV_PROG_FEAT_6WAY                = 0x0080,
}ca_omci_voip_app_srv_prog_feat_e;


typedef enum
{
    VOIP_APP_SRV_PRES_FEAT_DEF                = 0x0000,
    VOIP_APP_SRV_PRES_FEAT_MWI_SPLASH_RING    = 0x0001,
    VOIP_APP_SRV_PRES_FEAT_MWI_SPEC_DIAL_TONE = 0x0002,
    VOIP_APP_SRV_PRES_FEAT_MWI_VISUAL         = 0x0004,
    VOIP_APP_SRV_PRES_FEAT_CALL_FORWARD       = 0x0008,
}ca_omci_voip_app_srv_pres_feat_e;


typedef enum
{
    VOIP_APP_SRV_DC_FEAT_DEF      = 0x00,
    VOIP_APP_SRV_DC_FEAT_ENABLE   = 0x01,
    VOIP_APP_SRV_DC_FEAT_DT_DELAY = 0x02,
}ca_omci_voip_app_srv_dc_feat_e;


typedef struct
{
    ca_omci_uint8_t  cidFeature;
    ca_omci_uint8_t  callWaitFeature;
    ca_omci_uint16_t callProgFeature;
    ca_omci_uint16_t callPresFeature;
    ca_omci_uint8_t  direConnFeaturn;
    ca_omci_uint16_t direConnUp;
    ca_omci_uint16_t briLineAgtUp;
    ca_omci_uint16_t confFactUp;
}ca_omci_voip_app_srv_attr_t;


typedef struct
{
    ca_omci_mebase_t            meBase;
    ca_omci_voip_app_srv_attr_t attr;
}ca_omci_voip_app_srv_t;


void              ca_omci_me_voip_app_srv_init(void);
ca_omci_mebase_t *ca_omci_me_voip_app_srv_new (void);
#endif /* __OMCI_ME_VOIP_APP_SRV_H__ */

