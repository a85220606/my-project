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
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_me_onu_power_shed.h"


static ca_omci_me_handler_t sg_omci_me_onu_power_shed_handlers = {0};


ca_omci_me_attr_t g_omci_me_onu_power_shed_attr[OMCI_ME_ATTR11] =
{
    {OMCI_ME_ATTR1,  "rpt_reset_itv"},
    {OMCI_ME_ATTR2,  "data_shed_itv"},
    {OMCI_ME_ATTR3,  "voice_shed_itv"},
    {OMCI_ME_ATTR4,  "video_ol_shed_itv"},
    {OMCI_ME_ATTR5,  "video_rt_shed_itv"},
    {OMCI_ME_ATTR6,  "dsl_shed_itv"},
    {OMCI_ME_ATTR7,  "atm_shed_itv"},
    {OMCI_ME_ATTR8,  "ces_shed_itv"},
    {OMCI_ME_ATTR9,  "frame_shed_itv"},
    {OMCI_ME_ATTR10, "sdh_shed_itv"},
    {OMCI_ME_ATTR11, "shed_status"},
};


ca_omci_uint32_t ca_omci_me_onu_power_shed_attr_len_get(void)
{
    ca_omci_onu_power_shed_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.restorePowerTimeResetInterval);
    attrLen += sizeof(me->attr.dataShedInterval);
    attrLen += sizeof(me->attr.voideShedInterval);
    attrLen += sizeof(me->attr.vedioOverlayShedInterval);
    attrLen += sizeof(me->attr.vedioReturnShedInterval);
    attrLen += sizeof(me->attr.dslShedInterval);
    attrLen += sizeof(me->attr.atmShedInterval);
    attrLen += sizeof(me->attr.cesShedInterval);
    attrLen += sizeof(me->attr.frameShedInterval);
    attrLen += sizeof(me->attr.sdhShedInterval);
    attrLen += sizeof(me->attr.shedStatus);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_onu_power_shed_new(void)
{
    ca_omci_onu_power_shed_t *me = (ca_omci_onu_power_shed_t *)omci_malloc(sizeof(ca_omci_onu_power_shed_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ONU_POWER_SHED_CLASS_ID, &sg_omci_me_onu_power_shed_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(onu_power_shed);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_onu_power_shed_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_onu_power_shed_t *onuPowerShed = (ca_omci_onu_power_shed_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ONU_POWER_SHED_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.restorePowerTimeResetInterval, "restorePowerTimeResetInterval", 1);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.dataShedInterval, "dataShedInterval", 2);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.voideShedInterval, "voideShedInterval", 3);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.vedioOverlayShedInterval, "vedioOverlayShedInterval", 4);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.vedioReturnShedInterval, "vedioReturnShedInterval", 5);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.dslShedInterval, "dslShedInterval", 6);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.atmShedInterval, "atmShedInterval", 7);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.cesShedInterval, "cesShedInterval", 8);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.frameShedInterval, "frameShedInterval", 9);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.sdhShedInterval, "sdhShedInterval", 10);
    OMCI_UPDATE_ATTR_UINT(16, onuPowerShed->attr.shedStatus, "shedStatus", 11);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_onu_power_shed_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_onu_power_shed_t *onuPowerShed = (ca_omci_onu_power_shed_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ONU_POWER_SHED_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.restorePowerTimeResetInterval, 1);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.dataShedInterval, 2);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.voideShedInterval, 3);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.vedioOverlayShedInterval, 4);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.vedioReturnShedInterval, 5);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.dslShedInterval, 6);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.atmShedInterval, 7);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.cesShedInterval, 8);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.frameShedInterval, 9);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.sdhShedInterval, 10);
    OMCI_GET_ATTR_UINT16(onuPowerShed->attr.shedStatus, 11);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_onu_power_shed_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_onu_power_shed_t *onuPowerShed = (ca_omci_onu_power_shed_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(onuPowerShed);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.restorePowerTimeResetInterval, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.dataShedInterval, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.voideShedInterval, 3);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.vedioOverlayShedInterval, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.vedioReturnShedInterval, 5);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.dslShedInterval, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.atmShedInterval, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.cesShedInterval, 8);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.frameShedInterval, 9);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.sdhShedInterval, 10);
    OMCI_UPLOAD_ATTR_SIZE(16, onuPowerShed->attr.shedStatus, 11);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_onu_power_shed_init(void)
{
    sg_omci_me_onu_power_shed_handlers.uploadProc = ca_omci_me_onu_power_shed_upload;

    sg_omci_me_onu_power_shed_handlers.setProc    = ca_omci_me_onu_power_shed_set;
    sg_omci_me_onu_power_shed_handlers.getProc    = ca_omci_me_onu_power_shed_get;
}

