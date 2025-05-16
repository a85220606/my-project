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
#include "omci_me_voice_srv_prof.h"
#include "omci_me_net_addr.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"


static ca_omci_me_handler_t sg_omci_me_voice_srv_prof_handlers = {0};


ca_omci_me_attr_t g_omci_me_voice_srv_prof_attr[OMCI_ME_ATTR14] =
{
    {OMCI_ME_ATTR1,  "announce_type"},
    {OMCI_ME_ATTR2,  "jitter_target"},
    {OMCI_ME_ATTR3,  "max_jitter_buff"},
    {OMCI_ME_ATTR4,  "echo_cancel_ind"},
    {OMCI_ME_ATTR5,  "pstn_prot_var"},
    {OMCI_ME_ATTR6,  "dtmf_digit_lev"},
    {OMCI_ME_ATTR7,  "dtmf_digit_dur"},
    {OMCI_ME_ATTR8,  "min_hook_flash_t"},
    {OMCI_ME_ATTR9,  "max_hook_flash_t"},
    {OMCI_ME_ATTR10, "tone_pattern_tbl"},
    {OMCI_ME_ATTR11, "tone_event_tbl"},
    {OMCI_ME_ATTR12, "ring_pattern_tbl"},
    {OMCI_ME_ATTR13, "ring_event_tbl"},
    {OMCI_ME_ATTR14, "network_point"},
};


ca_omci_uint32_t ca_omci_me_voice_srv_prof_attr_len_get(void)
{
    ca_omci_voice_srv_prof_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.announceType);
    attrLen += sizeof(me->attr.jitterTarget);
    attrLen += sizeof(me->attr.maxJitterBuff);
    attrLen += sizeof(me->attr.echoCancelInd);
    attrLen += sizeof(me->attr.pstnProtVar);
    attrLen += sizeof(me->attr.dtmfDigitLev);
    attrLen += sizeof(me->attr.dtmfDigitDur);
    attrLen += sizeof(me->attr.minHookFlashT);
    attrLen += sizeof(me->attr.maxHookFlashT);
    attrLen += sizeof(me->attr.networkPoint);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_voice_srv_prof_new(void)
{
    ca_omci_voice_srv_prof_t *me = (ca_omci_voice_srv_prof_t *)omci_malloc(sizeof(ca_omci_voice_srv_prof_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_VOICE_SRV_PROF_CLASS_ID, &sg_omci_me_voice_srv_prof_handlers);

    /* This me need to init alarm report struct */
    me->meBase.amBmp = omci_malloc(sizeof(ca_omci_alarm_bmp_t));
    if (NULL == me->meBase.amBmp)
    {
        OMCIERR("No memory for alarm");
        OMCI_FREE(me);
        return NULL;
    }

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(voice_srv_prof);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_voice_srv_prof_create(
    ca_omci_mebase_t           *me,
    ca_omci_uint8_t            *attr,
    ca_omci_me_create_result_t *result)
{
    ca_omci_voice_srv_prof_t *voiceSrvProf = (ca_omci_voice_srv_prof_t *)me;
    ca_omci_uint16_t mask   = 0;
    ca_omci_uint16_t offset = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    OMCI_SET_ATTR_UINT8(voiceSrvProf->attr.announceType, "announceType", 1);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.jitterTarget, "jitterTarget", 2);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.maxJitterBuff, "maxJitterBuff", 3);
    OMCI_SET_ATTR_UINT8(voiceSrvProf->attr.echoCancelInd, "echoCancelInd", 4);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.pstnProtVar, "pstnProtVar", 5);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.dtmfDigitLev, "dtmfDigitLev", 6);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.dtmfDigitDur, "dtmfDigitDur", 7);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.minHookFlashT, "minHookFlashT", 8);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.maxHookFlashT, "maxHookFlashT", 9);
    OMCI_SET_ATTR_UINT16(voiceSrvProf->attr.networkPoint, "networkPoint", 14);
    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voice_srv_prof_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_voice_srv_prof_t      *voiceSrvProf = (ca_omci_voice_srv_prof_t *)me;
    ca_omci_voice_srv_prof_attr_t  newVal;
    ca_omci_uint16_t  offset  = 0;
    ca_omci_uint16_t  mask    = inMask;
    ca_omci_mebase_t *netAddr = NULL;
    ca_omci_uint16_t  idx     = 0;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_voice_srv_prof_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_VOICE_SRV_PROF_SET_MASK_FULL);

    //remove old link
    netAddr = ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, voiceSrvProf->attr.networkPoint);
    OMCI_RM_LINK_ME(voiceSrvProf, netAddr);

    OMCI_UPDATE_ATTR_UINT(8, voiceSrvProf->attr.announceType, "announceType", 1);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.jitterTarget, "jitterTarget", 2);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.maxJitterBuff, "maxJitterBuff", 3);
    OMCI_UPDATE_ATTR_UINT(8, voiceSrvProf->attr.echoCancelInd, "echoCancelInd", 4);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.pstnProtVar, "pstnProtVar", 5);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.dtmfDigitLev, "dtmfDigitLev", 6);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.dtmfDigitDur, "dtmfDigitDur", 7);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.minHookFlashT, "minHookFlashT", 8);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.maxHookFlashT, "maxHookFlashT", 9);
    OMCI_UPDATE_ATTR_UINT(16, voiceSrvProf->attr.networkPoint, "networkPoint", 14);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, mask))
    {
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].index, "index");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].toneOn, "toneOn");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].frequency1, "frequency1");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].power1, "power1");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].frequency2, "frequency2");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].power2, "power2");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].frequency3, "frequency3");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].power3, "power3");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].frequency4, "frequency4");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].power4, "power4");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].modulFreq, "modulFreq");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].modulPower, "modulPower");
        OMCI_SET_TBL_UINT16(newVal.tonePatternTbl[0].duration, "duration");
        OMCI_SET_TBL_UINT8(newVal.tonePatternTbl[0].nextEntry, "nextEntry");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);

        for (idx = 0; idx < OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL; idx++)
        {
            if (CA_OMCI_FALSE == voiceSrvProf->attr.tonePatternTbl[idx].valid)
            {
                voiceSrvProf->attr.tonePatternTbl[idx].index      = newVal.tonePatternTbl[0].index;
                voiceSrvProf->attr.tonePatternTbl[idx].toneOn     = newVal.tonePatternTbl[0].toneOn;
                voiceSrvProf->attr.tonePatternTbl[idx].frequency1 = newVal.tonePatternTbl[0].frequency1;
                voiceSrvProf->attr.tonePatternTbl[idx].power1     = newVal.tonePatternTbl[0].power1;
                voiceSrvProf->attr.tonePatternTbl[idx].frequency2 = newVal.tonePatternTbl[0].frequency2;
                voiceSrvProf->attr.tonePatternTbl[idx].power2     = newVal.tonePatternTbl[0].power2;
                voiceSrvProf->attr.tonePatternTbl[idx].frequency3 = newVal.tonePatternTbl[0].frequency3;
                voiceSrvProf->attr.tonePatternTbl[idx].power3     = newVal.tonePatternTbl[0].power3;
                voiceSrvProf->attr.tonePatternTbl[idx].frequency4 = newVal.tonePatternTbl[0].frequency4;
                voiceSrvProf->attr.tonePatternTbl[idx].power4     = newVal.tonePatternTbl[0].power4;
                voiceSrvProf->attr.tonePatternTbl[idx].modulFreq  = newVal.tonePatternTbl[0].modulFreq;
                voiceSrvProf->attr.tonePatternTbl[idx].modulPower = newVal.tonePatternTbl[0].modulPower;
                voiceSrvProf->attr.tonePatternTbl[idx].duration   = newVal.tonePatternTbl[0].duration;
                voiceSrvProf->attr.tonePatternTbl[idx].nextEntry  = newVal.tonePatternTbl[0].nextEntry;
                voiceSrvProf->attr.tonePatternTbl[idx].valid      = CA_OMCI_TRUE;
                break;
            }
            if (OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL == idx)
            {
                OMCIWARN("table tonePatternTbl for me voice_srv_prof is full");
            }
        }
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, mask))
    {
        OMCI_SET_TBL_UINT8(newVal.toneEventTbl[0].event, "event");
        OMCI_SET_TBL_UINT8(newVal.toneEventTbl[0].tonePattern, "tonePattern");
        OMCI_SET_TBL_UINT16(newVal.toneEventTbl[0].toneFile, "toneFile");
        OMCI_SET_TBL_UINT8(newVal.toneEventTbl[0].toneFileRep, "toneFileRep");
        OMCI_SET_TBL_UINT16(newVal.toneEventTbl[0].reserved, "reserved");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);

        for (idx = 0; idx < OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL; idx++)
        {
            if (CA_OMCI_FALSE == voiceSrvProf->attr.toneEventTbl[idx].valid)
            {
                voiceSrvProf->attr.toneEventTbl[idx].event       = newVal.toneEventTbl[0].event;
                voiceSrvProf->attr.toneEventTbl[idx].tonePattern = newVal.toneEventTbl[0].tonePattern;
                voiceSrvProf->attr.toneEventTbl[idx].toneFile    = newVal.toneEventTbl[0].toneFile;
                voiceSrvProf->attr.toneEventTbl[idx].toneFileRep = newVal.toneEventTbl[0].toneFileRep;
                voiceSrvProf->attr.toneEventTbl[idx].reserved    = newVal.toneEventTbl[0].reserved;
                voiceSrvProf->attr.toneEventTbl[idx].valid       = CA_OMCI_TRUE;
                break;
            }
            if (OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL == idx)
            {
                OMCIWARN("table toneEventTbl for me voice_srv_prof is full");
            }
        }
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, mask))
    {
        OMCI_SET_TBL_UINT8(newVal.ringPatternTbl[0].index, "index");
        OMCI_SET_TBL_UINT8(newVal.ringPatternTbl[0].ringingOn, "ringingOn");
        OMCI_SET_TBL_UINT16(newVal.ringPatternTbl[0].duration, "duration");
        OMCI_SET_TBL_UINT8(newVal.ringPatternTbl[0].nextEntry, "nextEntry");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);

        for (idx = 0; idx < OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL; idx++)
        {
            if (CA_OMCI_FALSE == voiceSrvProf->attr.ringPatternTbl[idx].valid)
            {
                voiceSrvProf->attr.ringPatternTbl[idx].index     = newVal.ringPatternTbl[0].index;
                voiceSrvProf->attr.ringPatternTbl[idx].ringingOn = newVal.ringPatternTbl[0].ringingOn;
                voiceSrvProf->attr.ringPatternTbl[idx].duration  = newVal.ringPatternTbl[0].duration;
                voiceSrvProf->attr.ringPatternTbl[idx].nextEntry = newVal.ringPatternTbl[0].nextEntry;
                voiceSrvProf->attr.ringPatternTbl[idx].valid     = CA_OMCI_TRUE;
                break;
            }
            if (OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL == idx)
            {
                OMCIWARN("table ringPatternTbl for me voice_srv_prof is full");
            }
        }
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR13, mask))
    {
        OMCI_SET_TBL_UINT8(newVal.ringEventTbl[0].event, "event");
        OMCI_SET_TBL_UINT8(newVal.ringEventTbl[0].ringPattern, "ringPattern");
        OMCI_SET_TBL_UINT16(newVal.ringEventTbl[0].ringFile, "ringFile");
        OMCI_SET_TBL_UINT8(newVal.ringEventTbl[0].ringFileRep, "ringFileRep");
        OMCI_SET_TBL_UINT16(newVal.ringEventTbl[0].ringText, "ringText");
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);

        for (idx = 0; idx < OMCI_VOICE_SRV_PROF_RING_EVENT_TBL; idx++)
        {
            if (CA_OMCI_FALSE == voiceSrvProf->attr.ringEventTbl[idx].valid)
            {
                voiceSrvProf->attr.ringEventTbl[idx].event       = newVal.ringEventTbl[0].event;
                voiceSrvProf->attr.ringEventTbl[idx].ringPattern = newVal.ringEventTbl[0].ringPattern;
                voiceSrvProf->attr.ringEventTbl[idx].ringFile    = newVal.ringEventTbl[0].ringFile;
                voiceSrvProf->attr.ringEventTbl[idx].ringFileRep = newVal.ringEventTbl[0].ringFileRep;
                voiceSrvProf->attr.ringEventTbl[idx].ringText    = newVal.ringEventTbl[0].ringText;
                voiceSrvProf->attr.ringEventTbl[idx].valid       = CA_OMCI_TRUE;
                break;
            }
            if (OMCI_VOICE_SRV_PROF_RING_EVENT_TBL == idx)
            {
                OMCIWARN("table ringEventTbl for me voice_srv_prof is full");
            }
        }
    }

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voice_srv_prof_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_voice_srv_prof_t *voiceSrvProf = (ca_omci_voice_srv_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_VOICE_SRV_PROF_GET_MASK_FULL);

    //TODO: update value in me

    OMCI_GET_ATTR_UINT8(voiceSrvProf->attr.announceType, 1);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.jitterTarget, 2);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.maxJitterBuff, 3);
    OMCI_GET_ATTR_UINT8(voiceSrvProf->attr.echoCancelInd, 4);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.pstnProtVar, 5);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.dtmfDigitLev, 6);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.dtmfDigitDur, 7);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.minHookFlashT, 8);
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.maxHookFlashT, 9);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR10, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(voiceSrvProf->attr.tonePatternTbl[0]) - sizeof(voiceSrvProf->attr.tonePatternTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == voiceSrvProf->attr.tonePatternTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblSeq = (tblSize + OMCI_MSG_GET_NEXT_LEN - 1) / OMCI_MSG_GET_NEXT_LEN;
        if (CA_OMCI_TRUE == ext)
        {
            tblSeq = (tblSize + OMCI_MSG_EXT_GET_NEXT_LEN - 1) / OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        tblSize = ca_omci_htonl(tblSize);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&voiceSrvProf->tonePatternTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(voiceSrvProf->attr.toneEventTbl[0]) - sizeof(voiceSrvProf->attr.toneEventTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == voiceSrvProf->attr.toneEventTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblSeq = (tblSize + OMCI_MSG_GET_NEXT_LEN - 1) / OMCI_MSG_GET_NEXT_LEN;
        if (CA_OMCI_TRUE == ext)
        {
            tblSeq = (tblSize + OMCI_MSG_EXT_GET_NEXT_LEN - 1) / OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        tblSize = ca_omci_htonl(tblSize);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&voiceSrvProf->toneEventTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR12, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(voiceSrvProf->attr.ringPatternTbl[0]) - sizeof(voiceSrvProf->attr.ringPatternTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == voiceSrvProf->attr.ringPatternTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblSeq = (tblSize + OMCI_MSG_GET_NEXT_LEN - 1) / OMCI_MSG_GET_NEXT_LEN;
        if (CA_OMCI_TRUE == ext)
        {
            tblSeq = (tblSize + OMCI_MSG_EXT_GET_NEXT_LEN - 1) / OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        tblSize = ca_omci_htonl(tblSize);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&voiceSrvProf->ringPatternTbl, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR13, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(voiceSrvProf->attr.ringEventTbl[0]) - sizeof(voiceSrvProf->attr.ringEventTbl[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_VOICE_SRV_PROF_RING_EVENT_TBL; tblIdx++)
        {
            if (CA_OMCI_TRUE == voiceSrvProf->attr.ringEventTbl[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblSeq = (tblSize + OMCI_MSG_GET_NEXT_LEN - 1) / OMCI_MSG_GET_NEXT_LEN;
        if (CA_OMCI_TRUE == ext)
        {
            tblSeq = (tblSize + OMCI_MSG_EXT_GET_NEXT_LEN - 1) / OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        tblSize = ca_omci_htonl(tblSize);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&voiceSrvProf->ringEventTbl, tblSeq);
    }
    OMCI_GET_ATTR_UINT16(voiceSrvProf->attr.networkPoint, 14);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voice_srv_prof_delete(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *result)
{
    ca_omci_voice_srv_prof_t *voiceSrvProf = (ca_omci_voice_srv_prof_t *)me;
    ca_omci_mebase_t         *netAddr      = NULL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(result);

    netAddr = ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, voiceSrvProf->attr.networkPoint);
    OMCI_RM_LINK_ME(voiceSrvProf, netAddr);

    ca_omci_update_flow(OMCI_CFG_FLOW_UPDATE_VOIP);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_voice_srv_prof_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_voice_srv_prof_t *voiceSrvProf = (ca_omci_voice_srv_prof_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    (void)seqNum; //remove warning
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(voiceSrvProf);
    OMCI_UPLOAD_ATTR_SIZE(8, voiceSrvProf->attr.announceType, 1);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.jitterTarget, 2);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.maxJitterBuff, 3);
    OMCI_UPLOAD_ATTR_SIZE(8, voiceSrvProf->attr.echoCancelInd, 4);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.pstnProtVar, 5);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.dtmfDigitLev, 6);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.dtmfDigitDur, 7);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.minHookFlashT, 8);
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.maxHookFlashT, 9);
    for (idx = 0; idx < OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL; idx++)
    {
        if (CA_OMCI_TRUE == voiceSrvProf->attr.tonePatternTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].index);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].toneOn);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].frequency1);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].power1);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].frequency2);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].power2);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].frequency3);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].power3);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].frequency4);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].power4);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].modulFreq);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].modulPower);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.tonePatternTbl[idx].duration);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.tonePatternTbl[idx].nextEntry);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR10, mask);
        }
    }
    for (idx = 0; idx < OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL; idx++)
    {
        if (CA_OMCI_TRUE == voiceSrvProf->attr.toneEventTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.toneEventTbl[idx].event);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.toneEventTbl[idx].tonePattern);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.toneEventTbl[idx].toneFile);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.toneEventTbl[idx].toneFileRep);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.toneEventTbl[idx].reserved);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
        }
    }
    for (idx = 0; idx < OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL; idx++)
    {
        if (CA_OMCI_TRUE == voiceSrvProf->attr.ringPatternTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringPatternTbl[idx].index);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringPatternTbl[idx].ringingOn);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.ringPatternTbl[idx].duration);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringPatternTbl[idx].nextEntry);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR12, mask);
        }
    }
    for (idx = 0; idx < OMCI_VOICE_SRV_PROF_RING_EVENT_TBL; idx++)
    {
        if (CA_OMCI_TRUE == voiceSrvProf->attr.ringEventTbl[idx].valid)
        {
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringEventTbl[idx].event);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringEventTbl[idx].ringPattern);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.ringEventTbl[idx].ringFile);
            OMCI_UPLOAD_TBL_UINT8(voiceSrvProf->attr.ringEventTbl[idx].ringFileRep);
            OMCI_UPLOAD_TBL_UINT16(voiceSrvProf->attr.ringEventTbl[idx].ringText);
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR13, mask);
        }
    }
    OMCI_UPLOAD_ATTR_SIZE(16, voiceSrvProf->attr.networkPoint, 14);
    OMCI_UPLOAD_MASK_ENCODE;
    return CA_OMCI_OK;
}


void ca_omci_me_voice_srv_prof_init(void)
{
    sg_omci_me_voice_srv_prof_handlers.uploadProc = ca_omci_me_voice_srv_prof_upload;

    sg_omci_me_voice_srv_prof_handlers.createProc = ca_omci_me_voice_srv_prof_create;
    sg_omci_me_voice_srv_prof_handlers.deleteProc = ca_omci_me_voice_srv_prof_delete;
    sg_omci_me_voice_srv_prof_handlers.setProc    = ca_omci_me_voice_srv_prof_set;
    sg_omci_me_voice_srv_prof_handlers.getProc    = ca_omci_me_voice_srv_prof_get;
}


void ca_omci_me_voice_srv_prof_link(void)
{
    ca_omci_meclass_t        *meClass = NULL;
    ca_omci_mebase_t         *meInst  = NULL;
    ca_omci_voice_srv_prof_t *me      = NULL;
    ca_omci_net_addr_t       *netAddr = NULL;

    meClass = ca_omci_me_get(OMCI_ME_VOICE_SRV_PROF_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        return;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        me      = (ca_omci_voice_srv_prof_t *)meInst;
        netAddr = (ca_omci_net_addr_t *)ca_omci_me_entry_lookup(OMCI_ME_NET_ADDR_CLASS_ID, me->attr.networkPoint);
        OMCI_ME_LINK(me, netAddr);
    }
}

