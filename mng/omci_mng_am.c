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
#include "osal_cmn.h"
#include "osal_mutex.h"
#include "omci_common.h"
#include "omci_hash.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_me_onu_data.h"
#include "omci_mng_am.h"
#include "omci_codec_pkt.h"
#include "omci_me_enhance_sec_ctl.h"
#include "omci_mq.h"


static ca_omci_me_get_alarm_next_t *sg_omciAlarmBmps  = NULL;
static ca_omci_uint16_t             sg_omciAlarmNum   = 0;
static ca_omci_uint8_t              sg_omciAlarmSeq   = 0;
static ca_uint                      sg_omciAlarmMutex = 0;


extern void ca_omci_me_pptp_eth_link_st_change(ca_omci_uint16_t instance, ca_omci_uint32_t state);
extern ca_omci_st_e ca_omci_enhance_avc_report(ca_omci_mebase_t * me, ca_omci_uint16_t attrId, void * data);


static ca_omci_alarm_callback_table_t sg_alarmHandlerTbl[] =
{
    {CA_OMCI_ALARM_PPTP_ETH_LAN_LOS, ca_omci_me_pptp_eth_link_st_change},
};



void ca_omci_alarm_handle(void *pData)
{
    ca_omci_inner_msg_t    *pInnerMsg = (ca_omci_inner_msg_t *)pData;
    ca_omci_alarm_report_t *pAmData   = (ca_omci_alarm_report_t *)pInnerMsg->data;
    ca_omci_uint8_t         i         = 0;

    OMCI_PARAM_IS_NULL(pData);

    for (i = 0; i < sizeof(sg_alarmHandlerTbl)/sizeof(ca_omci_alarm_callback_table_t); i++)
    {
        if (sg_alarmHandlerTbl[i].alarmId == pAmData->alarmId)
        {
            sg_alarmHandlerTbl[i].func(pAmData->instanceId, pAmData->state);
            break;
        }
    }

    if (i >= sizeof(sg_alarmHandlerTbl)/sizeof(ca_omci_alarm_callback_table_t))
    {
        OMCICOERR("no function for alarm ID %d instance %d state %d",
                   pAmData->alarmId, pAmData->instanceId, pAmData->state);
    }
}


static ca_omci_avc_callback_table_t sg_avcHandlerTbl[] =
{
    {OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID, ca_omci_enhance_avc_report},
};


void ca_omci_avc_handle(void *pData)
{
    ca_omci_inner_msg_t  *pInnerMsg = (ca_omci_inner_msg_t *)pData;
    ca_omci_mebase_t     *meBase    = NULL;
    ca_omci_avc_report_t *pAvcData  = (ca_omci_avc_report_t *)pInnerMsg->data;
    ca_omci_uint8_t       i         = 0;

    OMCI_PARAM_IS_NULL(pData);
    meBase = ca_omci_me_entry_lookup(pAvcData->classId, pAvcData->instanceId);
    if (NULL == meBase)
    {
        OMCIMNGERR("Can't find instance for inner class %d instance %d", pAvcData->classId, pAvcData->instanceId);
        return;
    }

    for (i = 0; i < sizeof(sg_avcHandlerTbl)/sizeof(ca_omci_avc_callback_table_t); i++)
    {
        if (sg_avcHandlerTbl[i].classId == pAvcData->classId)
        {
            sg_avcHandlerTbl[i].func(meBase, pAvcData->attrNo, NULL);
            break;
        }
    }

    if (i >= sizeof(sg_avcHandlerTbl)/sizeof(ca_omci_avc_callback_table_t))
    {
        OMCICOERR("no function for inner class %d instance %d avc %d",
                   pAvcData->classId, pAvcData->instanceId, pAvcData->attrNo);
    }
}


ca_omci_uint8_t ca_omci_mng_alarm_seq_get(void)
{
    return sg_omciAlarmSeq;
}


static void ca_omci_mng_alarm_seq_reset(void)
{
    if (CA_OSAL_OK != ca_mutex_trylock(sg_omciAlarmMutex))
    {
        return;
    }

    sg_omciAlarmSeq = 0;
    ca_mutex_unlock(sg_omciAlarmMutex);
}


void ca_omci_mng_alarm_seq_inc(void)
{
    if (CA_OSAL_OK != ca_mutex_trylock(sg_omciAlarmMutex))
    {
        return;
    }

    sg_omciAlarmSeq++;
    if (0 == sg_omciAlarmSeq)
    {
        sg_omciAlarmSeq = 1;
    }
    ca_omci_alarm_dg_update();
    ca_mutex_unlock(sg_omciAlarmMutex);
}


static ca_omci_bool_t ca_omci_alarm_raised(ca_omci_alarm_bmp_t *pAlarm)
{
    int idx = 0;

    for (idx = 0; idx < OMCI_STD_ALARM_BMP_LEN; idx++)
    {
        if (0 != pAlarm->stdAmBmp[idx])
        {
            return CA_OMCI_TRUE;
        }
    }

    for (idx = 0; idx < OMCI_ORG_ALARM_BMP_LEN; idx++)
    {
        if (0 != pAlarm->orgAmBmp[idx])
        {
            return CA_OMCI_TRUE;
        }
    }

    return CA_OMCI_FALSE;
}


void ca_omci_mng_get_all_alarm_next_ext(
    ca_omci_uint16_t  seqNum,
    ca_omci_uint8_t  *contents)
{
    ca_omci_uint16_t offset   = 0;
    ca_omci_uint16_t classId  = 0;
    ca_omci_uint16_t instance = 0;
    ca_omci_uint16_t idx      = 0;
    ca_omci_uint16_t pktLen   = 0;

    if ((NULL == sg_omciAlarmBmps) || (sg_omciAlarmNum < seqNum))
    {
        OMCIERR("can't find me for seq %d", seqNum);
        return;
    }

    //the first 2 bytes is the total len of this alarm
    offset += sizeof(ca_omci_uint16_t);

    //encode the real contents
    for (idx = 0; idx < sg_omciAlarmBmps[seqNum].entryNum; idx++)
    {
        classId = ca_omci_me_get_standard_class(sg_omciAlarmBmps[seqNum].entry[idx].classId);
        classId = ca_omci_htons(classId);
        OMCI_MEMCPY((contents + offset), &classId, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        instance = ca_omci_htons(sg_omciAlarmBmps[seqNum].entry[idx].instanceId);
        OMCI_MEMCPY((contents + offset), &instance, sizeof(ca_omci_uint16_t));
        offset += sizeof(ca_omci_uint16_t);
        OMCI_MEMCPY((contents + offset), sg_omciAlarmBmps[seqNum].entry[idx].alarmBmp, OMCI_ALARM_BMP_LEN);
        offset += OMCI_ALARM_BMP_LEN;
    }

    //save the packet len
    pktLen = offset - sizeof(ca_omci_uint16_t);
    if (OMCI_EXT_PKT_MAX_CONT_LEN <= pktLen)
    {
        //TODO: handle the packets longer than 1966
    }

    pktLen = ca_omci_htons(pktLen);
    OMCI_MEMCPY(contents, &pktLen, sizeof(ca_omci_uint16_t));
}


void ca_omci_mng_get_all_alarm_next_base(
    ca_omci_uint16_t  seqNum,
    ca_omci_uint8_t  *contents)
{
    ca_omci_uint16_t offset   = 0;
    ca_omci_uint16_t classId  = 0;
    ca_omci_uint16_t instance = 0;

    if ((NULL == sg_omciAlarmBmps) || (sg_omciAlarmNum < seqNum))
    {
        OMCIERR("can't find me for seq %d", seqNum);
        return;
    }
    classId = ca_omci_me_get_standard_class(sg_omciAlarmBmps[seqNum].entry[0].classId);
    classId = ca_omci_htons(classId);
    OMCI_MEMCPY((contents + offset), &classId, sizeof(ca_omci_uint16_t));
    offset += sizeof(ca_omci_uint16_t);
    instance = ca_omci_htons(sg_omciAlarmBmps[seqNum].entry[0].instanceId);
    OMCI_MEMCPY((contents + offset), &instance, sizeof(ca_omci_uint16_t));
    offset += sizeof(ca_omci_uint16_t);
    OMCI_MEMCPY((contents + offset), sg_omciAlarmBmps[seqNum].entry[0].alarmBmp, OMCI_ALARM_BMP_LEN);
}


void ca_omci_mng_get_all_alarm_next(
    ca_omci_uint16_t seqNum,
    ca_omci_bool_t   ext,
    ca_omci_uint8_t *contents)
{
    ca_omci_onu_data_t *onudata = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(contents);

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    OMCI_PARAM_IS_NULL(onudata);
    if (CA_OMCI_TRUE != ca_omci_next_valid(&onudata->alarmUploadNext, seqNum))
    {
        OMCIERR("Error seq number %d", seqNum);
        return;
    }

    if (CA_OMCI_TRUE == ext)
    {
        ca_omci_mng_get_all_alarm_next_ext(seqNum, contents);
        ca_omci_next_success(&onudata->alarmUploadNext);
        return;
    }

    ca_omci_mng_get_all_alarm_next_base(seqNum, contents);
    ca_omci_next_success(&onudata->alarmUploadNext);
}


ca_omci_uint16_t ca_omci_mng_get_all_alarms_num_get(ca_omci_bool_t ext)
{
    ca_omci_onu_data_t *onudata  = NULL;
    ca_omci_meclass_t  *me       = NULL;
    ca_omci_mebase_t   *meBase   = NULL;
    ca_omci_uint16_t    idx      = 0;
    ca_omci_uint16_t    seqNum   = 0;
    ca_omci_uint16_t    entryNum = 0;
    ca_omci_uint16_t    amIdx    = 0;
    ca_omci_bool_t      alarmOn  = CA_OMCI_FALSE;

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    if (NULL == onudata)
    {
        return 0;
    }

    //free the last result
    if (NULL != sg_omciAlarmBmps)
    {
        OMCI_FREE(sg_omciAlarmBmps);
    }

    if (CA_OMCI_TRUE == ext)
    {
        for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
        {
            me = ca_omci_me_get(idx);
            if (NULL == me)
            {
                continue;
            }

            alarmOn = CA_OMCI_FALSE;
            for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
                 meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
            {
                if ((NULL != meBase->amBmp) && (CA_OMCI_TRUE == ca_omci_alarm_raised(meBase->amBmp)))
                {
                    alarmOn = CA_OMCI_TRUE;
                }
            }

            if (CA_OMCI_TRUE == alarmOn)
            {
                seqNum++;
            }
        }

        if (0 == seqNum)
        {
            return 0;
        }

        //save alarm value
        sg_omciAlarmBmps = omci_malloc(sizeof(ca_omci_me_get_alarm_next_t) * seqNum);
        if (NULL == sg_omciAlarmBmps)
        {
            return 0;
        }

        amIdx = 0;
        for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
        {
            me = ca_omci_me_get(idx);
            if (NULL == me)
            {
                continue;
            }

            alarmOn  = CA_OMCI_FALSE;
            entryNum = 0;
            for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
                 meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
            {
                if ((NULL != meBase->amBmp) && (CA_OMCI_TRUE == ca_omci_alarm_raised(meBase->amBmp)))
                {
                    alarmOn = CA_OMCI_TRUE;
                    sg_omciAlarmBmps[amIdx].entry[entryNum].classId    = meBase->classId;
                    sg_omciAlarmBmps[amIdx].entry[entryNum].instanceId = meBase->instanceId;
                    OMCI_MEMCPY(sg_omciAlarmBmps[amIdx].entry[entryNum].alarmBmp,
                                meBase->amBmp->stdAmBmp, sizeof(meBase->amBmp->stdAmBmp));
                    OMCI_MEMCPY(&(sg_omciAlarmBmps[amIdx].entry[entryNum].alarmBmp[OMCI_ALARM_BMP_LEN - OMCI_ORG_ALARM_BMP_LEN]),
                                meBase->amBmp->orgAmBmp, sizeof(meBase->amBmp->orgAmBmp));
                    entryNum++;
                }
            }

            if (CA_OMCI_TRUE == alarmOn)
            {
                sg_omciAlarmBmps[amIdx].entryNum = entryNum;
                amIdx++;
            }
        }

        OMCIDBG("extern get all alarm seq number = %d saved idx %d", seqNum, amIdx);
        ca_omci_next_init(&onudata->alarmUploadNext, seqNum);
        sg_omciAlarmNum = seqNum;
        ca_omci_mng_alarm_seq_reset();
        return seqNum;
    }

    for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        me = ca_omci_me_get(idx);
        if (NULL == me)
        {
            continue;
        }

        for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
             meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
        {
            if ((NULL != meBase->amBmp) && (CA_OMCI_TRUE == ca_omci_alarm_raised(meBase->amBmp)))
            {
                seqNum++;
            }
        }
    }

    if (0 == seqNum)
    {
        return 0;
    }

    //save alarm value
    sg_omciAlarmBmps = omci_malloc(sizeof(ca_omci_me_get_alarm_next_t) * seqNum);
    if (NULL == sg_omciAlarmBmps)
    {
        return 0;
    }

    amIdx = 0;
    for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        me = ca_omci_me_get(idx);
        if (NULL == me)
        {
            continue;
        }

        for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
             meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
        {
            if ((NULL != meBase->amBmp) && (CA_OMCI_TRUE == ca_omci_alarm_raised(meBase->amBmp)))
            {
                sg_omciAlarmBmps[amIdx].entryNum            = 1;
                sg_omciAlarmBmps[amIdx].entry[0].classId    = meBase->classId;
                sg_omciAlarmBmps[amIdx].entry[0].instanceId = meBase->instanceId;
                OMCI_MEMCPY(sg_omciAlarmBmps[amIdx].entry[0].alarmBmp,
                            meBase->amBmp->stdAmBmp, sizeof(meBase->amBmp->stdAmBmp));
                OMCI_MEMCPY(&(sg_omciAlarmBmps[amIdx].entry[0].alarmBmp[OMCI_ALARM_BMP_LEN - OMCI_ORG_ALARM_BMP_LEN]),
                            meBase->amBmp->orgAmBmp, sizeof(meBase->amBmp->orgAmBmp));
                amIdx++;
            }
        }
    }

    OMCIDBG("get all alarm seq number = %d saved number %d", seqNum, amIdx);
    ca_omci_next_init(&onudata->alarmUploadNext, seqNum);
    sg_omciAlarmNum = seqNum;
    ca_omci_mng_alarm_seq_reset();
    return seqNum;
}


ca_omci_st_e ca_omci_alarm_id_update(
    ca_omci_mebase_t *me,
    ca_omci_uint32_t  alarmId,
    ca_omci_uint8_t   state)
{
    ca_omci_uint16_t pos    = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_PARAM_NULL_RETURN(me);

    if (NULL == me->amBmp)
    {
        OMCIERR("Can't report alarm for the me c[%d] i[0x%x]not support", me->classId, me->instanceId);
        return CA_OMCI_E;
    }

    if ((alarmId >= OMCI_ME_MAX_ALARM_ID)
     || ((alarmId >= OMCI_ME_ALARM_STD_BIT_MAX) && (alarmId < OMCI_ME_MIN_ORG_ALARM_ID)))
    {
        OMCIERR("alarm id error:%d c[%d] i[0x%x]", alarmId, me->classId, me->instanceId);
        return CA_OMCI_E;
    }

    /*update the org alarm*/
    if (alarmId >= OMCI_ME_MIN_ORG_ALARM_ID)
    {
        pos    = (alarmId - OMCI_ME_MIN_ORG_ALARM_ID) / OMCI_ME_ALARM_BIT_LEN;
        offset =  alarmId - (OMCI_ME_MIN_ORG_ALARM_ID - 1) - pos * OMCI_ME_ALARM_BIT_LEN;
        if (CA_OMCI_ALARM_RAISE == state)
        {
            OMCI_ALARM_BIT_SET(offset, me->amBmp->orgAmBmp[pos]);
        }
        else
        {
            OMCI_ALARM_BIT_CLR(offset, me->amBmp->orgAmBmp[pos]);
        }

        return CA_OMCI_OK;
    }

    /*update std alarm */
    pos    = alarmId / OMCI_ME_ALARM_BIT_LEN;
    offset = alarmId - pos * OMCI_ME_ALARM_BIT_LEN;
    if (CA_OMCI_ALARM_RAISE == state)
    {
        OMCI_ALARM_BIT_SET(offset, me->amBmp->stdAmBmp[pos]);
    }
    else
    {
        OMCI_ALARM_BIT_CLR(offset, me->amBmp->stdAmBmp[pos]);
    }

    return CA_OMCI_OK;
}


void ca_omci_alarm_report(
    ca_omci_mebase_t *me,
    ca_omci_uint32_t  alarmId,
    ca_omci_uint8_t   state)
{
    OMCI_PARAM_IS_NULL(me);

    if (CA_OMCI_OK != ca_omci_alarm_id_update(me, alarmId, state))
    {
        return;
    }

    OMCIDBG("report alarm for me innerclass %d, instance 0x%x alarmId %d",
             me->classId, me->instanceId, alarmId);
    omci_codec_encode_alarm(me);
}


//omci need to update dying gasp alarm when
//1. alarm seq number increase
//2. OMCC link up
//3. MIC update
void ca_omci_alarm_dg_update(void)
{
    ca_omci_mebase_t *me = ca_omci_me_entry_lookup(OMCI_ME_ONU_G_CLASS_ID, 0);

    OMCI_PARAM_IS_NULL(me);

    if (CA_OMCI_OK != ca_omci_alarm_id_update(me, OMCI_ME_ATTR7, CA_OMCI_ALARM_RAISE))
    {
        return;
    }
    omci_codec_encode_dg_alarm(me);

    //release the alarm bit after prepare the packet
    ca_omci_alarm_id_update(me, OMCI_ME_ATTR7, CA_OMCI_ALARM_CLEAR);
}


void ca_omci_alarm_init(void)
{
    //this mutex is used for alarm seq number
    if (CA_OMCI_OK != ca_mutex_init(&sg_omciAlarmMutex, "omci_alarm_seq", 0))
    {
        OMCIALERR("initialize omci alarm seq mutex failed");
    }
}

