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
#ifndef __OMCI_ME_VOICE_SRV_PROF_H__
#define __OMCI_ME_VOICE_SRV_PROF_H__


#define OMCI_VOICE_SRV_PROF_SET_MASK_FULL     65532
#define OMCI_VOICE_SRV_PROF_GET_MASK_FULL     65532

#define OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL 20
#define OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL   20
#define OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL 20
#define OMCI_VOICE_SRV_PROF_RING_EVENT_TBL   20

extern ca_omci_me_attr_t g_omci_me_voice_srv_prof_attr[OMCI_ME_ATTR14];


typedef enum
{
    VOICE_SRV_PROF_ANNOUNCE_TYPE_SILENCE        = 0x01,
    VOICE_SRV_PROF_ANNOUNCE_TYPE_REORDER_TONE   = 0x02,
    VOICE_SRV_PROF_ANNOUNCE_TYPE_FAST_BUST      = 0x03,
    VOICE_SRV_PROF_ANNOUNCE_TYPE_VOICE_ANNONUCE = 0x04,
    VOICE_SRV_PROF_ANNOUNCE_TYPE_NOT_SPEC       = 0xff,
}ca_omci_voice_srv_prof_announce_type_e;


typedef enum
{
    VOICE_SRV_PROF_TONE_EVENT_BUSY         = 1,
    VOICE_SRV_PROF_TONE_EVENT_CONFIRMATION = 2,
    VOICE_SRV_PROF_TONE_EVENT_DIAL         = 3,
    VOICE_SRV_PROF_TONE_EVENT_MSG_WAIT     = 4,
    VOICE_SRV_PROF_TONE_EVENT_OFFHOOK_WARN = 5,
    VOICE_SRV_PROF_TONE_EVENT_RINGBACK     = 6,
    VOICE_SRV_PROF_TONE_EVENT_REORDER      = 7,
    VOICE_SRV_PROF_TONE_EVENT_STUTTER_DIAL = 8,
    VOICE_SRV_PROF_TONE_EVENT_CALL_WAIT_1  = 9,
    VOICE_SRV_PROF_TONE_EVENT_CALL_WAIT_2  = 10,
    VOICE_SRV_PROF_TONE_EVENT_CALL_WAIT_3  = 11,
    VOICE_SRV_PROF_TONE_EVENT_CALL_WAIT_4  = 12,
    VOICE_SRV_PROF_TONE_EVENT_ALERT_SIGNAL = 13,
    VOICE_SRV_PROF_TONE_EVENT_SPECIAL_DIAL = 14,
    VOICE_SRV_PROF_TONE_EVENT_SPECIAL_INFO = 15,
    VOICE_SRV_PROF_TONE_EVENT_RELEASE      = 16,
    VOICE_SRV_PROF_TONE_EVENT_CONGESTION   = 17,
    VOICE_SRV_PROF_TONE_EVENT_USER_DEF_1   = 18,
    VOICE_SRV_PROF_TONE_EVENT_USER_DEF_2   = 19,
    VOICE_SRV_PROF_TONE_EVENT_USER_DEF_3   = 20,
    VOICE_SRV_PROF_TONE_EVENT_USER_DEF_4   = 21,
    VOICE_SRV_PROF_TONE_EVENT_INTRUSION    = 33,
    VOICE_SRV_PROF_TONE_EVENT_DEAD_TONE    = 34,
}ca_omci_voice_srv_prof_tone_event_e;


typedef enum
{
    VOICE_SRV_PROF_RING_EVENT_DEFAULT = 1,
    VOICE_SRV_PROF_RING_EVENT_SPLASH  = 2,
}ca_omci_voice_srv_prof_ring_event_e;


typedef struct
{
    ca_omci_bool_t   valid;
    ca_omci_uint8_t  index;
    ca_omci_uint8_t  toneOn;
    ca_omci_uint16_t frequency1;
    ca_omci_uint8_t  power1;
    ca_omci_uint16_t frequency2;
    ca_omci_uint8_t  power2;
    ca_omci_uint16_t frequency3;
    ca_omci_uint8_t  power3;
    ca_omci_uint16_t frequency4;
    ca_omci_uint8_t  power4;
    ca_omci_uint16_t modulFreq;
    ca_omci_uint8_t  modulPower;
    ca_omci_uint16_t duration;
    ca_omci_uint8_t  nextEntry;
}ca_omci_tone_pattern_table_t;


typedef struct
{
    ca_omci_bool_t   valid;
    ca_omci_uint8_t  event;
    ca_omci_uint8_t  tonePattern;
    ca_omci_uint16_t toneFile;
    ca_omci_uint8_t  toneFileRep;
    ca_omci_uint16_t reserved;
}ca_omci_tone_event_table_t;


typedef struct
{
    ca_omci_bool_t   valid;
    ca_omci_uint8_t  index;
    ca_omci_uint8_t  ringingOn;
    ca_omci_uint16_t duration;
    ca_omci_uint8_t  nextEntry;
}ca_omci_ring_pattern_table_t;


typedef struct
{
    ca_omci_bool_t   valid;
    ca_omci_uint8_t  event;
    ca_omci_uint8_t  ringPattern;
    ca_omci_uint16_t ringFile;
    ca_omci_uint8_t  ringFileRep;
    ca_omci_uint16_t ringText;
}ca_omci_ring_event_table_t;


typedef struct
{
    ca_omci_uint8_t              announceType;
    ca_omci_uint16_t             jitterTarget;
    ca_omci_uint16_t             maxJitterBuff;
    ca_omci_uint8_t              echoCancelInd;
    ca_omci_uint16_t             pstnProtVar;
    ca_omci_uint16_t             dtmfDigitLev;
    ca_omci_uint16_t             dtmfDigitDur;
    ca_omci_uint16_t             minHookFlashT;
    ca_omci_uint16_t             maxHookFlashT;
    ca_omci_tone_pattern_table_t tonePatternTbl[OMCI_VOICE_SRV_PROF_TONE_PATTERN_TBL];
    ca_omci_tone_event_table_t   toneEventTbl[OMCI_VOICE_SRV_PROF_TONE_EVENT_TBL];
    ca_omci_ring_pattern_table_t ringPatternTbl[OMCI_VOICE_SRV_PROF_RING_PATTERN_TBL];
    ca_omci_ring_event_table_t   ringEventTbl[OMCI_VOICE_SRV_PROF_RING_EVENT_TBL];
    ca_omci_uint16_t             networkPoint;
}ca_omci_voice_srv_prof_attr_t;


typedef struct
{
    ca_omci_mebase_t              meBase;
    ca_omci_next_t                tonePatternTbl;
    ca_omci_next_t                toneEventTbl;
    ca_omci_next_t                ringPatternTbl;
    ca_omci_next_t                ringEventTbl;
    ca_omci_voice_srv_prof_attr_t attr;
}ca_omci_voice_srv_prof_t;


void              ca_omci_me_voice_srv_prof_init(void);
ca_omci_mebase_t *ca_omci_me_voice_srv_prof_new (void);
#endif /* __OMCI_ME_VOICE_SRV_PROF_H__ */

