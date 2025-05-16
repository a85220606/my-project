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
#include "omci_hash.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_me_onu_data.h"
#include "omci_mng_onuinfo.h"
#include "omci_mibmng.h"
#include "omci_crc32.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_timer.h"
#include "omci_cfg.h"
#include "omci_mq.h"
#include "osal_sem.h"


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


void ca_omci_mng_mibmng_upload_next_base(ca_omci_uint16_t seqNum, ca_omci_uint8_t *contents)
{
    /*to speed up the search*/
    static ca_omci_uint16_t    currClassId = 0;
    static ca_omci_mebase_t   *currMeBase  = NULL;
    static ca_omci_uint16_t    lastCnt     = 0;

    ca_omci_mebase_t   *meBase  = NULL;
    ca_omci_mebase_t   *startMe = NULL;
    ca_omci_meclass_t  *me      = NULL;
    ca_omci_bool_t      found   = CA_OMCI_FALSE;
    ca_omci_uint16_t    idx     = 0;
    ca_omci_uint16_t    attrLen = 0;

    /* If it is the first mibupload next */
    if (0 == seqNum)
    {
        currClassId = 0;
        currMeBase  = NULL;
        lastCnt     = 0;
    }

    /*hit an instance, it still in current instance*/
    if ((NULL != currMeBase) && ((currMeBase->mibUploadCnt + lastCnt) > seqNum))
    {
        meBase = currMeBase;
        found  = CA_OMCI_TRUE;
        OMCIDBG("report the last instance, class:%d, inst:%d \n", meBase->classId, meBase->instanceId);
    }
    else
    {
        startMe = currMeBase;

        for (idx = currClassId; (idx < OMCI_ME_CLASS_ID_MAX && (found == CA_OMCI_FALSE)); idx++)
        {
            me = ca_omci_me_get(idx);
            if (NULL == me)
            {
                continue;
            }

            for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, startMe);
                 meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
            {
                if (0 != meBase->mibUploadCnt)
                {
                    found       = CA_OMCI_TRUE;
                    currClassId = idx;
                    break;
                }
            }
            startMe = NULL;
        }
    }


    if (CA_OMCI_FALSE == found)
    {
        OMCIERR("can't find me for seq %d", seqNum);
        return;
    }

    if (NULL == meBase->handlers->uploadProc)
    {
        OMCIERR("me class[%d] instance [%d] don't support upload, cnt[%d]", meBase->classId, meBase->instanceId, meBase->mibUploadCnt);
        return;
    }

    /*move to next instance*/
    if (currMeBase != meBase && currMeBase != NULL)
    {
        lastCnt += currMeBase->mibUploadCnt;
        OMCIDBG("report next instance, class:%d, inst:%d\n", meBase->classId, meBase->instanceId);
    }

    currMeBase = meBase;
    OMCIDBG("report instance, class:%d, inst:%d seq:%d\n", meBase->classId, meBase->instanceId, (seqNum - lastCnt));
    meBase->handlers->uploadProc(meBase, CA_OMCI_FALSE, contents, &attrLen, (seqNum - lastCnt));
}


/*
* For extern mib upload response msg flow,
* ONU should response all me instances for a class in one packets */
void ca_omci_mng_mibmng_upload_next_ext(
    ca_omci_uint16_t  seqNum,
    ca_omci_uint8_t  *contents)
{
    /*to speed up the search*/
    static ca_omci_uint16_t    currClassId = 0;
    static ca_omci_meclass_t  *currMe      = NULL;
    static ca_omci_mebase_t   *startMe     = NULL;

    ca_omci_mebase_t     *meBase     = NULL;
    ca_omci_meclass_t    *me         = NULL;
    ca_omci_uint16_t      singleLen  = 0;
    ca_omci_uint16_t      totalLen   = 0;
    ca_omci_uint16_t      idx        = 0;
    ca_omci_uint16_t      num        = 0;
    ca_omci_bool_t        flag       = CA_OMCI_FALSE;
    ca_omci_uint8_t      *cont       = NULL;
    ca_omci_uint8_t      *pCont      = NULL;

    /* If it is the first mibupload next */
    if (0 == seqNum)
    {
        currClassId = 0;
        currMe      = NULL;
        startMe     = NULL;
    }

    for (idx = currClassId; idx < OMCI_ME_CLASS_ID_MAX; idx++)
    {
        flag = CA_OMCI_FALSE;
        me   = ca_omci_me_get(idx);
        if ((NULL == me) || (0 == me->extMibUploadFlag))
        {
            continue;
        }

        //only get the exist MEs
        if (me->instHash->count >= 1)
        {
            flag = CA_OMCI_TRUE;
        }

        if (NULL != startMe)
        {
            break;
        }

        if ((me != currMe) && (CA_OMCI_TRUE == flag))
        {
            currMe = me;
            currClassId = idx;
            break;
        }
    }

    /* If this class need to report, report all MEs for this class */
    cont  = omci_malloc(2048);
    if (NULL == cont)
    {
        OMCIERR("upload next malloc failed\r\n");
        return;
    }

    pCont = cont;
    num   = 0;
    for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, startMe);
         meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
    {
        if (NULL == meBase->handlers->uploadProc)
        {
            OMCIERR("me class[%d] instance [%d] don't support upload, cnt[%d]", meBase->classId, meBase->instanceId, meBase->mibUploadCnt);
            continue;
        }
        singleLen = 0;
        meBase->handlers->uploadProc(meBase, CA_OMCI_TRUE, pCont, &singleLen, 0);
        pCont    += singleLen + OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN;
        totalLen += singleLen + OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN;
        if (totalLen <= OMCI_EXT_PKT_MAX_CONT_LEN)
        {
            num++;
            startMe = NULL;
        }
        else
        {
            startMe = meBase;
            printk("report %d single len is %d\r\n", num, singleLen);
            break;
        }
    }

    if (totalLen > OMCI_EXT_PKT_MAX_CONT_LEN)
    {
        totalLen = num * (singleLen + OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN);
        OMCI_MEMCPY(contents + sizeof(ca_omci_uint16_t), cont, totalLen);
        totalLen = ca_omci_htons(totalLen);
        OMCI_MEMCPY(contents, &totalLen, sizeof(ca_omci_uint16_t));
        return;
    }

    //TODO: NEED to support the multiple packets
    totalLen = (totalLen > OMCI_EXT_PKT_MAX_CONT_LEN) ? OMCI_EXT_PKT_MAX_CONT_LEN : totalLen;

    //copy the content
    OMCI_MEMCPY(contents + sizeof(ca_omci_uint16_t), cont, totalLen);

    totalLen = ca_omci_htons(totalLen);
    OMCI_MEMCPY(contents, &totalLen, sizeof(ca_omci_uint16_t));
    OMCI_FREE(cont);
}


void ca_omci_mng_mibmng_upload_next(
    ca_omci_uint16_t seqNum,
    ca_omci_bool_t   ext,
    ca_omci_uint8_t *contents)
{
    ca_omci_onu_data_t *onudata = NULL;

    OMCI_ENTER;
    OMCI_PARAM_IS_NULL(contents);

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    OMCI_PARAM_IS_NULL(onudata);
    if (CA_OMCI_TRUE != ca_omci_next_valid(&onudata->mibUploadNext, seqNum))
    {
        OMCIERR("Error seq number %d", seqNum);
        return;
    }

    if (CA_OMCI_TRUE == ext)
    {
        ca_omci_mng_mibmng_upload_next_ext(seqNum, contents);
        ca_omci_next_success(&onudata->mibUploadNext);
        return;
    }

    ca_omci_mng_mibmng_upload_next_base(seqNum, contents);
    ca_omci_next_success(&onudata->mibUploadNext);
}


// go through all me to get the total numbers for baseline msg or extern msg
ca_omci_uint16_t ca_omci_mng_mibmng_seq_num_get(ca_omci_bool_t ext)
{
    ca_omci_mebase_t   *meBase  = NULL;
    ca_omci_onu_data_t *onudata = NULL;
    ca_omci_meclass_t  *me      = NULL;
    ca_omci_uint16_t    idx     = 0;
    ca_omci_uint16_t    seqNum  = 0;
    ca_omci_bool_t      flag    = CA_OMCI_FALSE;
    ca_omci_uint16_t    singleLen  = 0;
    ca_omci_uint16_t    totalLen   = 0;
    ca_omci_uint16_t    num        = 0;
    ca_omci_uint8_t    *pCont      = NULL;

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    if (NULL == onudata)
    {
        return 0;
    }

    if (CA_OMCI_TRUE == ext)
    {
        for (idx = 0; idx < OMCI_ME_CLASS_ID_MAX; idx++)
        {
            flag = CA_OMCI_FALSE;
            me   = ca_omci_me_get(idx);
            if ((NULL == me) || (0 == me->extMibUploadFlag))
            {
                continue;
            }

            num      = 1;
            totalLen = 0;
            pCont    = omci_malloc(2048);
            if (NULL == pCont)
            {
                OMCIERR("upload get seq Num malloc failed\r\n");
                return 0;
            }

            for (meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
                 meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, meBase))
            {
                flag = CA_OMCI_TRUE;
                if (NULL != meBase->handlers->uploadProc)
                {
                    singleLen = 0;
                    meBase->handlers->uploadProc(meBase, CA_OMCI_TRUE, pCont, &singleLen, 0);
                    pCont    += singleLen + OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN;
                    totalLen += singleLen + OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN;
                    if (totalLen > OMCI_EXT_PKT_MAX_CONT_LEN * num)
                    {
                        num++;
                    }
                }
            }
            OMCI_FREE(pCont);

            if (CA_OMCI_TRUE == flag)
            {
                seqNum += num;
            }
        }

        OMCIDBG("extern mib upload seq number = %d ", seqNum);
        ca_omci_next_init(&onudata->mibUploadNext, seqNum);
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
            seqNum += meBase->mibUploadCnt;
        }
    }

    OMCIDBG("baseline mib upload seq number = %d ", seqNum);
    ca_omci_next_init(&onudata->mibUploadNext, seqNum);
    return seqNum;
}


void ca_omci_mibdata_inc(void)
{
    ca_omci_onu_data_t *onudata = NULL;

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    OMCI_PARAM_IS_NULL(onudata);

    onudata->attr.mibDataSync++;
    if (0 == onudata->attr.mibDataSync)
    {
        onudata->attr.mibDataSync = 1;
    }
}


void ca_omci_mng_next_finish(void)
{
    ca_omci_onu_data_t *onudata = NULL;

    onudata = (ca_omci_onu_data_t *)ca_omci_me_entry_lookup(OMCI_ME_ONU_DATA_CLASS_ID, OMCI_DEF_INSTANCE_ID);
    OMCI_PARAM_IS_NULL(onudata);

    ca_omci_next_finish(&onudata->mibUploadNext);
    ca_omci_next_finish(&onudata->alarmUploadNext);
}


ca_omci_uint8_t ca_omci_mng_mib_reset(void)
{
    ca_omci_meclass_t  *me      = NULL;
    ca_omci_mebase_t   *meBase  = NULL;
    ca_omci_uint32_t   i         = 0;
    ca_omci_uint8_t    rsl       = OMCI_MSG_RST_SUCCESSFUL;

    OMCI_ENTER;

    //remove all the unfinished timer when mib reset
    ca_omci_mng_next_finish();

    for (i = 0; i < OMCI_ME_CLASS_ID_MAX; i++)
    {
        me = ca_omci_me_get(i);

        /* copy from old stack */
        if (NULL == me)
        {
            continue;
        }

        meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
        while (NULL != meBase)
        {
            /* If it is PM ME, call delete func to release resource cp from old stack */
            if ((OMCI_ME_ETH_PM_HIS_CLASS_ID == meBase->classId)
             || (OMCI_ME_MAC_BRG_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_MAC_PORT_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_ETH_PM_HIS2_CLASS_ID == meBase->classId)
             || (OMCI_ME_IP_HOST_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_CALL_CTL_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_RTP_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_SIP_AGT_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_SIP_CALL_INIT_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_GAL_ETH_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_ETH_PM_HIS3_CLASS_ID == meBase->classId)
             || (OMCI_ME_FEC_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_ETH_FRM_PM_DS_CLASS_ID == meBase->classId)
             || (OMCI_ME_ETH_FRM_PM_US_CLASS_ID == meBase->classId)
             || (OMCI_ME_ETH_FRAME_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_GPN_CTP_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_TCP_UDP_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_ENER_CONSU_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_GEMPORT_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_MGC_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_XGPON_DS_MNG_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_XGPON_TC_PM_CLASS_ID == meBase->classId)
             || (OMCI_ME_DOT1X_PM_HIS_CLASS_ID == meBase->classId))
            {
                if (NULL != meBase->handlers->deleteProc)
                {
                    meBase->handlers->deleteProc(meBase, &rsl);
                }
            }

            HASH_Remove(me->instHash, meBase);
            ca_omci_me_delete(meBase);
            meBase = (ca_omci_mebase_t *)HASH_GetNext(me->instHash, NULL);
        }

        HASH_Destroy(me->instHash);
        me->instHash = NULL;
    }

    ca_omci_me_init();

    //remove the flow
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE);

    OMCIERR("Warning: Mib reset");
    return rsl;
}

