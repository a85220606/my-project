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
#include "omci_me_eq_prot_prof.h"


static ca_omci_me_handler_t sg_omci_me_eq_prot_prof_handlers = {0};


ca_omci_me_attr_t g_omci_me_eq_prot_prof_attr[OMCI_ME_ATTR14] =
{
    {OMCI_ME_ATTR1,  "prot_slot_1"},
    {OMCI_ME_ATTR2,  "prot_slot_2"},
    {OMCI_ME_ATTR3,  "work_slot_1"},
    {OMCI_ME_ATTR4,  "work_slot_2"},
    {OMCI_ME_ATTR5,  "work_slot_3"},
    {OMCI_ME_ATTR6,  "work_slot_4"},
    {OMCI_ME_ATTR7,  "work_slot_5"},
    {OMCI_ME_ATTR8,  "work_slot_6"},
    {OMCI_ME_ATTR9,  "work_slot_7"},
    {OMCI_ME_ATTR10, "work_slot_8"},
    {OMCI_ME_ATTR11, "prot_status_1"},
    {OMCI_ME_ATTR12, "prot_status_2"},
    {OMCI_ME_ATTR13, "revertive_ind"},
    {OMCI_ME_ATTR14, "wait_restore_time"},
};


ca_omci_uint32_t ca_omci_me_eq_prot_prof_attr_len_get(void)
{
    ca_omci_eq_prot_prof_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.protSlot1);
    attrLen += sizeof(me->attr.protSlot2);
    attrLen += sizeof(me->attr.workSlot1);
    attrLen += sizeof(me->attr.workSlot2);
    attrLen += sizeof(me->attr.workSlot3);
    attrLen += sizeof(me->attr.workSlot4);
    attrLen += sizeof(me->attr.workSlot5);
    attrLen += sizeof(me->attr.workSlot6);
    attrLen += sizeof(me->attr.workSlot7);
    attrLen += sizeof(me->attr.workSlot8);
    attrLen += sizeof(me->attr.protStatus1);
    attrLen += sizeof(me->attr.protStatus2);
    attrLen += sizeof(me->attr.revertiveInd);
    attrLen += sizeof(me->attr.waitRestoreT);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_eq_prot_prof_new(void)
{
    ca_omci_eq_prot_prof_t *me = (ca_omci_eq_prot_prof_t *)omci_malloc(sizeof(ca_omci_eq_prot_prof_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_EQ_PROT_PROF_CLASS_ID, &sg_omci_me_eq_prot_prof_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(eq_prot_prof);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_eq_prot_prof_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_eq_prot_prof_t *eqProtProf = (ca_omci_eq_prot_prof_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(eqProtProf->attr.protSlot1, "protSlot1", 1);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.protSlot2, "protSlot2", 2);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot1, "workSlot1", 3);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot2, "workSlot2", 4);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot3, "workSlot3", 5);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot4, "workSlot4", 6);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot5, "workSlot5", 7);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot6, "workSlot6", 8);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot7, "workSlot7", 9);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.workSlot8, "workSlot8", 10);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.revertiveInd, "revertiveInd", 13);
    OMCI_SET_ATTR_UINT8(eqProtProf->attr.waitRestoreT, "waitRestoreT", 14);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eq_prot_prof_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_eq_prot_prof_t *eqProtProf = (ca_omci_eq_prot_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_EQ_PROT_PROF_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.protSlot1, "protSlot1", 1);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.protSlot2, "protSlot2", 2);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot1, "workSlot1", 3);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot2, "workSlot2", 4);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot3, "workSlot3", 5);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot4, "workSlot4", 6);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot5, "workSlot5", 7);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot6, "workSlot6", 8);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot7, "workSlot7", 9);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.workSlot8, "workSlot8", 10);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.revertiveInd, "revertiveInd", 13);
    OMCI_UPDATE_ATTR_UINT(8, eqProtProf->attr.waitRestoreT, "waitRestoreT", 14);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eq_prot_prof_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_eq_prot_prof_t *eqProtProf = (ca_omci_eq_prot_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_EQ_PROT_PROF_GET_MASK_FULL);

    OMCI_GET_ATTR_UINT8(eqProtProf->attr.protSlot1, 1);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.protSlot2, 2);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot1, 3);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot2, 4);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot3, 5);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot4, 6);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot5, 7);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot6, 8);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot7, 9);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.workSlot8, 10);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.protStatus1, 11);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.protStatus2, 12);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.revertiveInd, 13);
    OMCI_GET_ATTR_UINT8(eqProtProf->attr.waitRestoreT, 14);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eq_prot_prof_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_eq_prot_prof_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_eq_prot_prof_t *eqProtProf = (ca_omci_eq_prot_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(eqProtProf);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.protSlot1, 1);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.protSlot2, 2);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot1, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot2, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot3, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot4, 6);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot5, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot6, 8);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot7, 9);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.workSlot8, 10);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.protStatus1, 11);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.protStatus2, 12);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.revertiveInd, 13);
    OMCI_UPLOAD_ATTR_SIZE(8, eqProtProf->attr.waitRestoreT, 14);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_eq_prot_prof_init(void)
{
    sg_omci_me_eq_prot_prof_handlers.uploadProc = ca_omci_me_eq_prot_prof_upload;

    sg_omci_me_eq_prot_prof_handlers.createProc = ca_omci_me_eq_prot_prof_create;
    sg_omci_me_eq_prot_prof_handlers.deleteProc = ca_omci_me_eq_prot_prof_delete;
    sg_omci_me_eq_prot_prof_handlers.setProc    = ca_omci_me_eq_prot_prof_set;
    sg_omci_me_eq_prot_prof_handlers.getProc    = ca_omci_me_eq_prot_prof_get;
}

