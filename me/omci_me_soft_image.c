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
#include "omci_mng_zephyr_cmd.h"
#include "omci_common.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_me_soft_image.h"
#include "omci_mng_upgrade.h"


static ca_omci_me_handler_t sg_omci_me_soft_image_handlers = {0};


ca_omci_me_attr_t g_omci_me_soft_image_attr[OMCI_ME_ATTR6] =
{
    {OMCI_ME_ATTR1, "version"},
    {OMCI_ME_ATTR2, "is_commit"},
    {OMCI_ME_ATTR3, "is_active"},
    {OMCI_ME_ATTR4, "is_valid"},
    {OMCI_ME_ATTR5, "product_code"},
    {OMCI_ME_ATTR6, "image_hash"},
};


ca_omci_uint32_t ca_omci_me_soft_image_attr_len_get(void)
{
    ca_omci_soft_image_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.version);
    attrLen += sizeof(me->attr.isCommit);
    attrLen += sizeof(me->attr.isActive);
    attrLen += sizeof(me->attr.isValid);
    attrLen += sizeof(me->attr.productCode);
    attrLen += sizeof(me->attr.imageHash);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_soft_image_new(void)
{
    ca_omci_soft_image_t *me = (ca_omci_soft_image_t *)omci_malloc(sizeof(ca_omci_soft_image_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_SOFT_IMAGE_CLASS_ID, &sg_omci_me_soft_image_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(soft_image);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_soft_image_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_soft_image_t *softImage = (ca_omci_soft_image_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = inMask;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_SOFT_IMAGE_GET_MASK_FULL);

    //update value in me
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR1, mask))
    {
        //check weather to get the new image or not
        ca_omci_mng_me_sw_ver_upg((void *)me);
    }

    OMCI_GET_ATTR_UINT8_T(softImage->attr.version, OMCI_VERSION_LEN, 1);
    OMCI_GET_ATTR_UINT8(softImage->attr.isCommit, 2);
    OMCI_GET_ATTR_UINT8(softImage->attr.isActive, 3);
    OMCI_GET_ATTR_UINT8(softImage->attr.isValid, 4);
    OMCI_GET_ATTR_UINT8_T(softImage->attr.productCode, OMCI_PRODUCT_CODE_LEN, 5);
    OMCI_GET_ATTR_UINT8_T(softImage->attr.imageHash, OMCI_IMAGE_HASH_LEN, 6);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_soft_image_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_soft_image_t *softImage = (ca_omci_soft_image_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(softImage);
    if (CA_OMCI_TRUE == ext)
    {
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.version, 1);
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isCommit, 2);
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isActive, 3);
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isValid, 4);
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.productCode, 5);
        OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.imageHash, 6);
        OMCI_UPLOAD_MASK_ENCODE;
    }
    else
    {
        if (0 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.version, 1);
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isCommit, 2);
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isActive, 3);
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.isValid, 4);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        if (1 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.productCode, 5);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        if (2 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, softImage->attr.imageHash, 6);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
        }
    }
    return CA_OMCI_OK;
}


void ca_omci_me_soft_image_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_soft_image_t *softImage = (ca_omci_soft_image_t *)me;
    ca_omci_uint32_t idx = 0;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(data);

    omci_sprint(cli, "Me: softImage \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        version:     %s \n", softImage->attr.version);
    omci_sprint(cli, "        isCommit:    0x%x\n", softImage->attr.isCommit);
    omci_sprint(cli, "        isActive:    0x%x\n", softImage->attr.isActive);
    omci_sprint(cli, "        isValid:     0x%x\n", softImage->attr.isValid);
    for (idx = 0; idx < OMCI_PRODUCT_CODE_LEN; idx++)
    {
        omci_sprint(cli, "        productCode[%d]: 0x%x \n", idx, softImage->attr.productCode[idx]);
    }
    for (idx = 0; idx < OMCI_IMAGE_HASH_LEN; idx++)
    {
        omci_sprint(cli, "        imageHash[%d]: 0x%x \n", idx, softImage->attr.imageHash[idx]);
    }
}



void ca_omci_me_soft_image_init(void)
{
    sg_omci_me_soft_image_handlers.uploadProc = ca_omci_me_soft_image_upload;
    sg_omci_me_soft_image_handlers.meDisplay  = ca_omci_me_soft_image_display;
    sg_omci_me_soft_image_handlers.getProc    = ca_omci_me_soft_image_get;
}

