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
#include <zephyr.h>
#include "omci_common.h"
#include "util_aes.h"
#include "omci_hal.h"
#include "omci_mic.h"
#include "omci_mng_onuinfo.h"


ca_omci_uint8_t sg_ca_omci_ik[OMCI_IK_LEN]   = {0};
ca_omci_uint8_t sg_ca_omci_msk[OMCI_MSK_LEN] = {0};


ca_omci_uint8_t omci_def_msk_key[16] =
{
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55,
    0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55
};


ca_omci_uint8_t omci_sk_string[OMCI_SK_STR_LEN] = 
{
    0x53, 0x65, 0x73, 0x73, 
    0x69, 0x6f, 0x6e, 0x4b
};/*"SessionK"*/


ca_omci_uint8_t omci_ik_msg[OMCI_IK_MSG_LEN] =
{
    0x4f, 0x4d, 0x43, 0x49, 0x49, 0x6e, 0x74, 0x65,
    0x67, 0x72, 0x69, 0x74, 0x79, 0x4b, 0x65, 0x79
};/*"OMCIIntegrityKey"*/


void ca_omci_debug_print_omci_msk(void)
{
    ca_omci_uint8_t idx = 0;

    printk("omci msk is 0x");
    for (idx = 0; idx < OMCI_MSK_LEN; idx++)
    {
        printk("%02x", sg_ca_omci_msk[idx]);
    }
    printk("\n");

}


ca_omci_st_e ca_omci_gen_msk(void)
{
    ca_omci_uint8_t onuRegId[OMCI_ONU_REG_ID_LEN] = {0};
    ca_omci_uint8_t ponPort                       = 0;

    ca_omci_hal_onu_reg_id_get((void *)&ponPort, (void *)onuRegId);
    if (CA_E_OK != aes128_cmac_compute( omci_def_msk_key, onuRegId, OMCI_ONU_REG_ID_LEN, OMCI_MSK_LEN, sg_ca_omci_msk))
    {
        OMCIERR("calculate msk error now!");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


void ca_omci_update_omci_msk(ca_omci_uint8_t *pMsk)
{
    OMCI_MEMCPY(sg_ca_omci_msk, pMsk, OMCI_MSK_LEN);
    ca_omci_gen_omci_ik();

    //ca_omci_debug_print_omci_msk();
}


ca_omci_st_e ca_omci_gen_sk(ca_omci_uint8_t *sk)
{
    ca_omci_uint8_t       *pMsg                     = NULL;
    ca_omci_uint8_t        ponTag[OMCI_PON_TAG_LEN] = {0};
    ca_omci_onuinfo_obj_t *pObj                     = ca_omci_onuinfo_get();
    ca_omci_uint8_t        ponPort                  = 0;

    ca_omci_hal_pon_tag_get((void *)&ponPort, (void *)ponTag);

    pMsg = (ca_omci_uint8_t *)omci_malloc(OMCI_SK_MSG_LEN);
    if (NULL == pMsg)
    {
        OMCIERR("Can't malloc now!");
        return CA_OMCI_E_NO_MEMORY;
    }

    OMCI_MEMSET(pMsg, 0, OMCI_SK_MSG_LEN);
    OMCI_MEMCPY(pMsg, pObj->sn, OMCI_SN_LEN);
    OMCI_MEMCPY((pMsg + OMCI_SN_LEN), ponTag, OMCI_PON_TAG_LEN);
    OMCI_MEMCPY((pMsg + OMCI_SN_LEN + OMCI_PON_TAG_LEN), omci_sk_string, OMCI_SK_STR_LEN);

    if (CA_E_OK != aes128_cmac_compute( sg_ca_omci_msk, pMsg, OMCI_SK_MSG_LEN, OMCI_SK_LEN, sk))
    {
        OMCIERR("Calculate SK error!");
        OMCI_FREE(pMsg);
        return CA_OMCI_E_PARAM;
    }

    OMCI_FREE(pMsg);
    return CA_OMCI_OK;
}


void ca_omci_debug_print_omci_ik(void)
{
    ca_omci_uint8_t idx = 0;

    printk("omci ik is 0x");
    for (idx = 0; idx < OMCI_IK_LEN; idx++)
    {
        printk("%02x", sg_ca_omci_ik[idx]);
    }
    printk("\n");
}


ca_omci_st_e ca_omci_gen_omci_ik(void)
{
    ca_omci_uint8_t sk[OMCI_SK_LEN] = {0};
    
    if (CA_OMCI_OK != ca_omci_gen_sk(sk))
    {
        OMCIERR("Can't generate sk now!");
        return CA_OMCI_E_PARAM;
    }

    if (CA_E_OK != aes128_cmac_compute( sk, omci_ik_msg, OMCI_IK_MSG_LEN, OMCI_IK_LEN, sg_ca_omci_ik))
    {
        OMCIERR("Calculate omci ik error!");
        return CA_OMCI_E_PARAM;
    }

    //ca_omci_debug_print_omci_ik();
    return CA_OMCI_OK;
}


/* 0x01 for downstream and 0x02 for upstream */
ca_omci_uint32_t ca_omci_gen_mic(
    ca_omci_uint8_t   dir,
    ca_omci_uint8_t  *omciContent,
    ca_omci_uint32_t  omciContentLen)
{
    ca_omci_uint8_t *pMsg                  = NULL;
    ca_omci_uint8_t  mac[OMCI_DEF_MIC_LEN] = {0};
    ca_omci_uint32_t mic                   = 0;

    OMCI_ENTER;

    pMsg = (ca_omci_uint8_t *)omci_malloc(omciContentLen + 1);
    if (NULL == pMsg)
    {
        OMCIERR("Can't malloc now!");
        return 0;
    }
    
    OMCI_MEMSET(pMsg, 0, (omciContentLen + 1));
    OMCI_MEMCPY(pMsg, &dir, 1);
    OMCI_MEMCPY((pMsg + 1), omciContent, omciContentLen);

    if (CA_E_OK != aes128_cmac_compute( sg_ca_omci_ik, pMsg, (omciContentLen + 1), OMCI_DEF_MIC_LEN, mac))
    {
        OMCIERR("Calculate MIC error!");
        OMCI_FREE(pMsg);
        return 0;
    }

    mic = (mac[0] << 24) | (mac[1] << 16) | (mac[2] << 8) | mac[3];
    OMCIDBG("MIC 0x%02x%02x%02x%02x, mic[%x]", mac[0], mac[1], mac[2], mac[3], mic);
    OMCI_FREE(pMsg);
    return mic;
}


void ca_omci_gen_mic_init(void)
{
    ca_omci_onuinfo_obj_t *pObj = ca_omci_onuinfo_get();

    if (OMCI_PON_MODE_GPON == pObj->ponMacMode)
        return;

    ca_omci_onuinfo_sn_update();
    ca_omci_gen_msk();
    ca_omci_gen_omci_ik();
}

