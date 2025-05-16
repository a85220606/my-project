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
#include "omci_timer.h"
#include "omci_me_ener_consu_pm.h"
#include "omci_mq.h"


static ca_omci_me_handler_t sg_omci_me_ener_consu_pm_handlers = {0};


ca_omci_me_attr_t g_omci_me_ener_consu_pm_attr[OMCI_ME_ATTR6] =
{
    {OMCI_ME_ATTR1, "interval_end_time"},
    {OMCI_ME_ATTR2, "thres_id"},
    {OMCI_ME_ATTR3, "doze_time"},
    {OMCI_ME_ATTR4, "cyclic_sleep_time"},
    {OMCI_ME_ATTR5, "watchful_sleep_time"},
    {OMCI_ME_ATTR6, "energy_consumed"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


ca_omci_uint32_t ca_omci_me_ener_consu_pm_attr_len_get(void)
{
    ca_omci_ener_consu_pm_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.intervalEndT);
    attrLen += sizeof(me->attr.thresId);
    attrLen += sizeof(me->attr.dozeTime);
    attrLen += sizeof(me->attr.cyclicSleepT);
    attrLen += sizeof(me->attr.watchfulSlT);
    attrLen += sizeof(me->attr.energyConsumed);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ener_consu_pm_new(void)
{
    ca_omci_ener_consu_pm_t *me = (ca_omci_ener_consu_pm_t *)omci_malloc(sizeof(ca_omci_ener_consu_pm_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ENER_CONSU_PM_CLASS_ID, &sg_omci_me_ener_consu_pm_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ener_consu_pm);

    return (ca_omci_mebase_t *)me;
}


void ca_omci_me_ener_consu_pm_timeout_handle(void *pData)
{
    ca_omci_uint16_t        *inst = (ca_omci_uint16_t *)pData;
    ca_omci_ener_consu_pm_t *me   = NULL;

    OMCI_ENTER;
    me = (ca_omci_ener_consu_pm_t *)ca_omci_me_entry_lookup(OMCI_ME_ENER_CONSU_PM_CLASS_ID, *inst);
    OMCI_PARAM_IS_NULL(me);

    //get static value from hal, and put these value in preAttr;
    //TODO:
    //ca_omci_hal_ener_consu_pm_pm_get(me->meBase.instanceId, value);

}


void ca_omci_me_ener_consu_pm_timeout(void *data)
{
    ca_omci_ener_consu_pm_t *me = (ca_omci_ener_consu_pm_t *)data;
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_PM_CHECK;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_uint16_t) + sizeof(ca_omci_uint16_t) + sizeof(ca_omci_timer_def_e);//pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, sizeof(ca_omci_timer_def_e));//cp timerDef
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e)), &me->meBase.classId, sizeof(ca_omci_uint16_t));//cp classId
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e) + sizeof(ca_omci_uint16_t)), &me->meBase.instanceId, sizeof(ca_omci_uint16_t));//cp instanceId
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


ca_omci_st_e ca_omci_me_ener_consu_pm_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT16(enerConsuPm->attr.thresId, "thresId", 2);

    //clear hal static number first
    //TODO:
    //ca_omci_hal_ener_consu_pm_pm_clear(me->meBase.instanceId);

    //start timer to collect data
    if (CA_INVALID_TIMER != enerConsuPm->timerId)
    {
        OMCIMNGERR("Can't create a timer twice for me ener_consu_pm");
        result->result = OMCI_MSG_RST_PARAM_ERR;
        return CA_OMCI_E_PARAM;
    }

    enerConsuPm->timerId = ca_circle_timer_add(OMCI_TIMER_PM_INTERVAL,
                                               ca_omci_me_ener_consu_pm_timeout,
                                               (void *)enerConsuPm);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ener_consu_pm_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ENER_CONSU_PM_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(16, enerConsuPm->attr.thresId, "thresId", 2);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ener_consu_pm_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ENER_CONSU_PM_GET_MASK_FULL);
    OMCI_GET_ATTR_UINT8(enerConsuPm->preAttr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT16(enerConsuPm->preAttr.thresId, 2);
    OMCI_GET_ATTR_UINT32(enerConsuPm->preAttr.dozeTime, 3);
    OMCI_GET_ATTR_UINT32(enerConsuPm->preAttr.cyclicSleepT, 4);
    OMCI_GET_ATTR_UINT32(enerConsuPm->preAttr.watchfulSlT, 5);
    OMCI_GET_ATTR_UINT32(enerConsuPm->preAttr.energyConsumed, 6);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ener_consu_pm_get_current(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ENER_CONSU_PM_GET_MASK_FULL);

    //TODO: call api to get current value

    OMCI_GET_ATTR_UINT8(enerConsuPm->attr.intervalEndT, 1);
    OMCI_GET_ATTR_UINT16(enerConsuPm->attr.thresId, 2);
    OMCI_GET_ATTR_UINT32(enerConsuPm->attr.dozeTime, 3);
    OMCI_GET_ATTR_UINT32(enerConsuPm->attr.cyclicSleepT, 4);
    OMCI_GET_ATTR_UINT32(enerConsuPm->attr.watchfulSlT, 5);
    OMCI_GET_ATTR_UINT32(enerConsuPm->attr.energyConsumed, 6);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ener_consu_pm_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    if (CA_INVALID_TIMER != enerConsuPm->timerId)
    {
        ca_timer_stop(enerConsuPm->timerId);
        ca_timer_del(enerConsuPm->timerId);
        enerConsuPm->timerId = CA_INVALID_TIMER;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ener_consu_pm_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ener_consu_pm_t *enerConsuPm = (ca_omci_ener_consu_pm_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(enerConsuPm);
    OMCI_UPLOAD_ATTR_SIZE(8, enerConsuPm->attr.intervalEndT, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, enerConsuPm->attr.thresId, 2);
    OMCI_UPLOAD_ATTR_SIZE(32, enerConsuPm->attr.dozeTime, 3);
    OMCI_UPLOAD_ATTR_SIZE(32, enerConsuPm->attr.cyclicSleepT, 4);
    OMCI_UPLOAD_ATTR_SIZE(32, enerConsuPm->attr.watchfulSlT, 5);
    OMCI_UPLOAD_ATTR_SIZE(32, enerConsuPm->attr.energyConsumed, 6);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_ener_consu_pm_init(void)
{
    sg_omci_me_ener_consu_pm_handlers.uploadProc = ca_omci_me_ener_consu_pm_upload;

    sg_omci_me_ener_consu_pm_handlers.createProc = ca_omci_me_ener_consu_pm_create;
    sg_omci_me_ener_consu_pm_handlers.deleteProc = ca_omci_me_ener_consu_pm_delete;
    sg_omci_me_ener_consu_pm_handlers.setProc    = ca_omci_me_ener_consu_pm_set;
    sg_omci_me_ener_consu_pm_handlers.getProc    = ca_omci_me_ener_consu_pm_get;
    sg_omci_me_ener_consu_pm_handlers.getCurProc = ca_omci_me_ener_consu_pm_get_current;
}

