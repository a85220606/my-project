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
#include "omci_me_ani_g.h"
#include "omci_timer.h"
#include "omci_hal.h"
#include "omci_mq.h"
#include "omci_mng_onuinfo.h"
#include "omci_mng_am.h"


static ca_omci_me_handler_t sg_omci_me_ani_g_handlers = {0};
static ca_uint32 sg_anigTestTimer     = CA_INVALID_TIMER;
static ca_uint32 sg_anigOpmCheckTimer = CA_INVALID_TIMER;
static ca_omci_bool_t sg_anigRxLowFlag  = CA_OMCI_FALSE;
static ca_omci_bool_t sg_anigRxHighFlag = CA_OMCI_FALSE;
static ca_omci_bool_t sg_anigTxLowFlag  = CA_OMCI_FALSE;
static ca_omci_bool_t sg_anigTxHighFlag = CA_OMCI_FALSE;


ca_omci_me_attr_t g_omci_me_ani_g_attr[OMCI_ME_ATTR16] =
{
    {OMCI_ME_ATTR1,  "sr_ind"},
    {OMCI_ME_ATTR2,  "total_tcont_num"},
    {OMCI_ME_ATTR3,  "gem_block_len"},
    {OMCI_ME_ATTR4,  "pb_dba_report"},
    {OMCI_ME_ATTR5,  "deprecated"},
    {OMCI_ME_ATTR6,  "sf_thres"},
    {OMCI_ME_ATTR7,  "sd_thres"},
    {OMCI_ME_ATTR8,  "arc"},
    {OMCI_ME_ATTR9,  "arc_interval"},
    {OMCI_ME_ATTR10, "optical_level"},
    {OMCI_ME_ATTR11, "low_opt_thres"},
    {OMCI_ME_ATTR12, "up_opt_thres"},
    {OMCI_ME_ATTR13, "onu_res_time"},
    {OMCI_ME_ATTR14, "tx_opt_level"},
    {OMCI_ME_ATTR15, "low_tx_power_thres"},
    {OMCI_ME_ATTR16, "up_tx_power_thres"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


ca_omci_uint32_t ca_omci_me_ani_g_attr_len_get(void)
{
    ca_omci_ani_g_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.srInd);
    attrLen += sizeof(me->attr.totalTcontNum);
    attrLen += sizeof(me->attr.gemBlockLen);
    attrLen += sizeof(me->attr.pbDbaReport);
    attrLen += sizeof(me->attr.deprecated);
    attrLen += sizeof(me->attr.sfThres);
    attrLen += sizeof(me->attr.sdThres);
    attrLen += sizeof(me->attr.arc);
    attrLen += sizeof(me->attr.arcInterval);
    attrLen += sizeof(me->attr.opticalLevel);
    attrLen += sizeof(me->attr.lowOptThres);
    attrLen += sizeof(me->attr.upOptThres);
    attrLen += sizeof(me->attr.onuResTime);
    attrLen += sizeof(me->attr.txOptLevel);
    attrLen += sizeof(me->attr.lowTxPowerThres);
    attrLen += sizeof(me->attr.upTxPowerThres);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_ani_g_new(void)
{
    ca_omci_ani_g_t *me = (ca_omci_ani_g_t *)omci_malloc(sizeof(ca_omci_ani_g_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ANI_G_CLASS_ID, &sg_omci_me_ani_g_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(ani_g);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_ani_g_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_ani_g_t  *anig   = (ca_omci_ani_g_t *)me;
    ca_omci_uint16_t  offset = 0;
    ca_omci_uint16_t  mask   = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ANI_G_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT(16, anig->attr.gemBlockLen, "gemBlockLen", 3);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.sfThres, "sfThres", 6);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.sdThres, "sdThres", 7);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.arc, "arc", 8);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.arcInterval, "arcInterval", 9);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.lowOptThres, "lowOptThres", 11);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.upOptThres, "upOptThres", 12);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.lowTxPowerThres, "lowTxPowerThres", 15);
    OMCI_UPDATE_ATTR_UINT(8, anig->attr.upTxPowerThres, "upTxPowerThres", 16);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ani_g_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_ani_g_t        *anig    = (ca_omci_ani_g_t *)me;
    ca_omci_uint16_t        offset  = 0;
    ca_omci_uint16_t        mask    = 0;
    ca_omci_uint8_t         ponPort = 0;
    ca_omci_hal_pon_info_t  ponInfo = {0};

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ANI_G_GET_MASK_FULL);

    //get ani_g parameters
    ca_omci_hal_pon_info_get((void *)&ponPort, (void *)&ponInfo);
    anig->attr.opticalLevel = ponInfo.rxPowerDbm;
    anig->attr.txOptLevel   = ponInfo.minLaunchPowerDbm;//dBuW to dBm

    OMCI_GET_ATTR_UINT8(anig->attr.srInd, 1);
    OMCI_GET_ATTR_UINT16(anig->attr.totalTcontNum, 2);
    OMCI_GET_ATTR_UINT16(anig->attr.gemBlockLen, 3);
    OMCI_GET_ATTR_UINT8(anig->attr.pbDbaReport, 4);
    OMCI_GET_ATTR_UINT8(anig->attr.deprecated, 5);
    OMCI_GET_ATTR_UINT8(anig->attr.sfThres, 6);
    OMCI_GET_ATTR_UINT8(anig->attr.sdThres, 7);
    OMCI_GET_ATTR_UINT8(anig->attr.arc, 8);
    OMCI_GET_ATTR_UINT8(anig->attr.arcInterval, 9);
    OMCI_GET_ATTR_UINT16(anig->attr.opticalLevel, 10);
    OMCI_GET_ATTR_UINT8(anig->attr.lowOptThres, 11);
    OMCI_GET_ATTR_UINT8(anig->attr.upOptThres, 12);
    OMCI_GET_ATTR_UINT16(anig->attr.onuResTime, 13);
    OMCI_GET_ATTR_UINT16(anig->attr.txOptLevel, 14);
    OMCI_GET_ATTR_UINT8(anig->attr.lowTxPowerThres, 15);
    OMCI_GET_ATTR_UINT8(anig->attr.upTxPowerThres, 16);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_ani_g_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_ani_g_t *anig = (ca_omci_ani_g_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(anig);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.srInd, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, anig->attr.totalTcontNum, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, anig->attr.gemBlockLen, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.pbDbaReport, 4);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.deprecated, 5);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.sfThres, 6);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.sdThres, 7);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.arc, 8);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.arcInterval, 9);
    OMCI_UPLOAD_ATTR_SIZE(16, anig->attr.opticalLevel, 10);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.lowOptThres, 11);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.upOptThres, 12);
    OMCI_UPLOAD_ATTR_SIZE(16, anig->attr.onuResTime, 13);
    OMCI_UPLOAD_ATTR_SIZE(16, anig->attr.txOptLevel, 14);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.lowTxPowerThres, 15);
    OMCI_UPLOAD_ATTR_SIZE(8, anig->attr.upTxPowerThres, 16);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_ani_g_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_ani_g_t *anig = (ca_omci_ani_g_t *)me;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: anig \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    omci_sprint(cli, "        srInd:       0x%x\n", anig->attr.srInd);
    omci_sprint(cli, "        totalTcontNum: 0x%x\n", anig->attr.totalTcontNum);
    omci_sprint(cli, "        gemBlockLen: 0x%x\n", anig->attr.gemBlockLen);
    omci_sprint(cli, "        pbDbaReport: 0x%x\n", anig->attr.pbDbaReport);
    omci_sprint(cli, "        deprecated:  0x%x\n", anig->attr.deprecated);
    omci_sprint(cli, "        sfThres:     0x%x\n", anig->attr.sfThres);
    omci_sprint(cli, "        sdThres:     0x%x\n", anig->attr.sdThres);
    omci_sprint(cli, "        arc:         0x%x\n", anig->attr.arc);
    omci_sprint(cli, "        arcInterval: 0x%x\n", anig->attr.arcInterval);
    omci_sprint(cli, "        opticalLevel: 0x%x\n", anig->attr.opticalLevel);
    omci_sprint(cli, "        lowOptThres: 0x%x\n", anig->attr.lowOptThres);
    omci_sprint(cli, "        upOptThres:  0x%x\n", anig->attr.upOptThres);
    omci_sprint(cli, "        onuResTime:  0x%x\n", anig->attr.onuResTime);
    omci_sprint(cli, "        txOptLevel:  0x%x\n", anig->attr.txOptLevel);
    omci_sprint(cli, "        lowTxPowerThres: 0x%x\n", anig->attr.lowTxPowerThres);
    omci_sprint(cli, "        upTxPowerThres: 0x%x\n", anig->attr.upTxPowerThres);
}


void ca_omci_me_ani_g_init(void)
{
    sg_omci_me_ani_g_handlers.meDisplay  = ca_omci_me_ani_g_display;
    sg_omci_me_ani_g_handlers.uploadProc = ca_omci_me_ani_g_upload;

    sg_omci_me_ani_g_handlers.setProc    = ca_omci_me_ani_g_set;
    sg_omci_me_ani_g_handlers.getProc    = ca_omci_me_ani_g_get;
}


void ca_omci_me_ani_g_test_timer_handler(void *data)
{
    ca_omci_uint8_t         ponPort = 0;
    ca_omci_hal_pon_info_t  ponInfo = {0};
    ca_omci_mebase_t       *me      = (ca_omci_mebase_t *)data;

    if (NULL == me)
    {
        OMCIMNGERR("instance is NULL");
        return;
    }

    //remove timer
    sg_anigTestTimer = CA_INVALID_TIMER;

    //get ani_g parameters
    ca_omci_hal_pon_info_get((void *)&ponPort, (void *)&ponInfo);

    //send out ani_g test result
    omci_codec_encode_test_rsl(me, (void *)&ponInfo);
}


void ca_omci_me_ani_g_test_timeout(void *data)
{
    ca_omci_inner_msg_t  msg;
    ca_omci_timer_def_e  timerDef = CA_OMCI_TIMER_ME_ANIG_TEST;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_next_t) + sizeof(ca_omci_timer_def_e);//pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef,sizeof(ca_omci_timer_def_e)); //cp timer def
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e)), data, sizeof(ca_omci_mebase_t));//cp pData
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


void ca_omci_me_ani_g_test(ca_omci_mebase_t *me)
{
    //start timer to collect data
    if (CA_INVALID_TIMER != sg_anigTestTimer)
    {
        OMCIMNGERR("Can't create a timer twice to test ani_g");
        return;
    }

    sg_anigTestTimer = ca_timer_add(OMCI_TIMER_TEST_INTERVAL,
                                    ca_omci_me_ani_g_test_timeout,
                                    me);
}


void ca_omci_me_ani_g_opm_check_timeout_handler(void *data)
{
    ca_omci_uint8_t         ponPort = 0;
    ca_omci_hal_pon_info_t  ponInfo = {0};
    ca_omci_ani_g_t        *aniG    = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t         idx     = 0;
    ca_omci_uint16_t        instance = 0;

    (void)data;

    //now there is only one pon port, but when use dual pon or other mode, must check all pon port
    for (idx = 0; idx < onuinfo->ponNum; idx++)
    {
        instance = ((onuinfo->ponSlot & 0xff) << 8) | (idx + 1);
        aniG = (ca_omci_ani_g_t *)ca_omci_me_entry_lookup(OMCI_ME_ANI_G_CLASS_ID, instance);
        if (NULL == aniG)
            continue;

        ponPort = idx;
        ca_omci_hal_pon_info_get((void *)&ponPort, (void *)&ponInfo);

        aniG->attr.opticalLevel = ponInfo.rxPowerDbm;
        aniG->attr.txOptLevel   = ponInfo.minLaunchPowerDbm;//dBuW to dBm

        //compare the value to the Thres
        //compare the rx power
        if (0xff != aniG->attr.lowOptThres)
        {
            if (((0 - aniG->attr.lowOptThres * 250) > (aniG->attr.opticalLevel - 30 * 500))
             && (CA_OMCI_FALSE == sg_anigRxLowFlag))
            {
                sg_anigRxLowFlag = CA_OMCI_TRUE;
                ca_omci_alarm_report(&aniG->meBase, 0, CA_OMCI_ALARM_RAISE);
            }
            else if (((0 - aniG->attr.lowOptThres * 250) < (aniG->attr.opticalLevel - 30 * 500))
             && (CA_OMCI_TRUE == sg_anigRxLowFlag))
            {
                sg_anigRxLowFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, 0, CA_OMCI_ALARM_CLEAR);
            }
        }
        else
        {
            //clear the alarm when reset the thres
            if (CA_OMCI_TRUE == sg_anigRxLowFlag)
            {
                sg_anigRxLowFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, 0, CA_OMCI_ALARM_CLEAR);
            }
        }

        if (0xff != aniG->attr.upOptThres)
        {
            if (((0 - aniG->attr.upOptThres * 250) < (aniG->attr.opticalLevel - 30 * 500))
             && (CA_OMCI_FALSE == sg_anigRxHighFlag))
            {
                sg_anigRxHighFlag = CA_OMCI_TRUE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR1, CA_OMCI_ALARM_RAISE);
            }
            else if (((0 - aniG->attr.upOptThres * 250) > (aniG->attr.opticalLevel - 30 * 500))
             && (CA_OMCI_TRUE == sg_anigRxHighFlag))
            {
                sg_anigRxHighFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR1, CA_OMCI_ALARM_CLEAR);
            }
        }
        else
        {
            if (CA_OMCI_TRUE == sg_anigRxHighFlag)
            {
                sg_anigRxHighFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR1, CA_OMCI_ALARM_CLEAR);
            }
        }

        //compare the tx power
        if (0x81 != aniG->attr.lowTxPowerThres)
        {
            if (((aniG->attr.lowTxPowerThres * 250) > aniG->attr.txOptLevel)
             && (CA_OMCI_FALSE == sg_anigTxLowFlag))
            {
                sg_anigTxLowFlag = CA_OMCI_TRUE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR4, CA_OMCI_ALARM_RAISE);
            }
            else if (((aniG->attr.lowTxPowerThres * 250) < aniG->attr.txOptLevel)
             && (CA_OMCI_TRUE == sg_anigTxLowFlag))
            {
                sg_anigTxLowFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR4, CA_OMCI_ALARM_CLEAR);
            }
        }
        else
        {
            if (CA_OMCI_TRUE == sg_anigTxLowFlag)
            {
                sg_anigTxLowFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR4, CA_OMCI_ALARM_CLEAR);
            }
        }

        if (0x81 != aniG->attr.upTxPowerThres)
        {
            if (((aniG->attr.upTxPowerThres * 250) < aniG->attr.txOptLevel)
             && (CA_OMCI_FALSE == sg_anigTxHighFlag))
            {
                sg_anigTxHighFlag = CA_OMCI_TRUE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR5, CA_OMCI_ALARM_RAISE);
            }
            else if (((aniG->attr.upTxPowerThres * 250) > aniG->attr.txOptLevel)
             && (CA_OMCI_TRUE == sg_anigTxHighFlag))
            {
                sg_anigTxHighFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR5, CA_OMCI_ALARM_CLEAR);
            }
        }
        else
        {
            if (CA_OMCI_TRUE == sg_anigTxHighFlag)
            {
                sg_anigTxHighFlag = CA_OMCI_FALSE;
                ca_omci_alarm_report(&aniG->meBase, OMCI_ME_ATTR5, CA_OMCI_ALARM_CLEAR);
            }
        }

        //compare the bias
        //TODO: for diff vendor may have diff threshold
    }
}


void ca_omci_me_ani_g_opm_check_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_ME_ANIG_OPM_CHECK;

    (void)data;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_timer_def_e);//no pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, msg.hdr.dataLen);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


void ca_omci_me_ani_g_opm_check(void)
{
    if (CA_INVALID_TIMER != sg_anigOpmCheckTimer)
    {
        return;
    }

    sg_anigOpmCheckTimer = ca_circle_timer_add(OMCI_TIMER_OPM_CHECK,
                                               ca_omci_me_ani_g_opm_check_timeout,
                                               NULL);
}


void ca_omci_me_ani_g_opm_check_stop(void)
{
    if (CA_INVALID_TIMER != sg_anigOpmCheckTimer)
    {
        ca_timer_stop(sg_anigOpmCheckTimer);
        ca_timer_del(sg_anigOpmCheckTimer);
        sg_anigOpmCheckTimer = CA_INVALID_TIMER;
    }
}

