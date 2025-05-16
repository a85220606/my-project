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
#include "omci_mng_onuinfo.h"
#include "osal_cmn.h"
#include "util_misc.h"
#include "scfg.h"
#include "omci_cfg_file.h"


static ca_omci_onuinfo_obj_t sg_omciOnuinfoObj = {0};


void ca_omci_test_rslt_handle(void *pData)
{
    OMCI_PARAM_IS_NULL(pData)
}


ca_omci_uint32_t ca_omci_olt_type_get(void)
{
    return sg_omciOnuinfoObj.oltType;
}


ca_omci_onuinfo_obj_t *ca_omci_onuinfo_get(void)
{
    return &sg_omciOnuinfoObj;
}


void ca_omci_onuinfo_cfg(ca_omci_onuinfo_obj_t *pObj)
{
    OMCI_PARAM_IS_NULL(pObj);
    OMCI_MEMCPY(&sg_omciOnuinfoObj, pObj, sizeof(sg_omciOnuinfoObj));
}


void ca_omci_onuinfo_sn_convert(ca_omci_onuinfo_obj_t *pObj, ca_uint32_t vsSn)
{
    //first four bytes are an ASCII-encoded four-letter vendor ID
    OMCI_MEMCPY(pObj->sn, pObj->vendorId, OMCI_VENDOR_ID_LEN);

    // The second four bytes  are  a  binary  encoded  serial  number
    pObj->sn[4] = (vsSn & 0xff000000) >> 24;
    pObj->sn[5] = (vsSn & 0xff0000) >> 16;
    pObj->sn[6] = (vsSn & 0xff00) >> 8;
    pObj->sn[7] = vsSn & 0xff;
}


void ca_omci_onuinfo_sn_update(void)
{
    ca_omci_onuinfo_obj_t *pObj = ca_omci_onuinfo_get();
    ca_uint32              vsSn = 0;
    ca_uint8_t             vendor[OMCI_VENDOR_ID_LEN] = {0};

    if (CA_E_OK == ca_scfg_read(0, CFG_ID_PON_VENDOR_ID, OMCI_VENDOR_ID_LEN, vendor))
    {
        OMCI_MEMCPY(pObj->vendorId, vendor, OMCI_VENDOR_ID_LEN);
    }

    if (CA_E_OK == ca_scfg_read(0, CFG_ID_PON_VSSN, sizeof(ca_uint32), &vsSn))
    {
        ca_omci_onuinfo_sn_convert(pObj, vsSn);
    }
}


void ca_omci_onuinfo_scfg_get(ca_omci_onuinfo_obj_t *pObj)
{
    ca_uint8_t  mac[OMCI_MAC_ADDR_LEN] = {0};
    ca_uint8_t  ponMacMode             = 0;
    ca_uint32_t onuMode                = 0;

    if (CA_E_OK == ca_scfg_read(0, CFG_ID_ETH0_MAC_ADDR, OMCI_MAC_ADDR_LEN, mac))
    {
        OMCI_MEMCPY(pObj->mac, mac, OMCI_MAC_ADDR_LEN);
    }

    if (CA_E_OK == ca_scfg_read(0, CFG_ID_PON_MAC_MODE, sizeof(ca_uint8), &ponMacMode))
    {
        pObj->ponMacMode = ponMacMode;
    }

    if (CA_E_OK == ca_scfg_read(0, CFG_ID_GPON_ONU_MODE, sizeof(ca_uint32), &onuMode))
    {
        pObj->onuMode = onuMode;
    }
}


void ca_omci_onuinfo_cfg_get(ca_omci_onuinfo_obj_t *pObj)
{
    pObj->ipv4Addr       = OMCI_DEF_LOCAL_IP;
    pObj->ipv4Mask       = OMCI_DEF_LOCAL_IP_MASK;
    pObj->ipv4Gw         = OMCI_DEF_LOCAL_IP_MASK;
    pObj->ponMacMode     = OMCI_PON_MODE_25GPON;
    pObj->maxQueuePerTc  = 8; /*by default 8 queue per 1 tcont*/
    pObj->onuMode        = 0;
    pObj->mac[0]         = 0;
    pObj->mac[1]         = 0x13;
    pObj->mac[2]         = 0x25;
    pObj->mac[3]         = 0;
    pObj->mac[4]         = 0;
    pObj->mac[5]         = 0x01; /*00:13:25:00:00:01*/

    OMCI_MEMCPY(pObj->vendorId, OMCI_DEF_VENDOR_ID, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(pObj->sn,       OMCI_DEF_SN,        OMCI_SN_LEN);

    /* get hardware relate value from scfg file */
    ca_omci_onuinfo_scfg_get(pObj);

    /* load omci cfg file */
    ca_omci_load_config_file((void *)pObj);
}


void ca_omci_onuinfo_init(void)
{
    ca_omci_onuinfo_obj_t cfg = {0};

    OMCI_MEMSET(&cfg, 0, sizeof(ca_omci_onuinfo_obj_t));

    ca_omci_onuinfo_cfg_get(&cfg);
    ca_omci_onuinfo_cfg(&cfg);
    ca_omci_onuinfo_sn_update();
}

