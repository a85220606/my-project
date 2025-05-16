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
#include "omci_me_enhance_sec_ctl.h"
#include "omci_me_onu2_g.h"
#include "omci_mng_onuinfo.h"
#include "omci_timer.h"
#include "omci_mq.h"
#include "omci_mng_am.h"
#include "omci_hal.h"
#include "util_aes.h"
#include "omci_mic.h"
#ifdef CONFIG_CA_OMCI_SUPPORT_OTP
#include "cap_otp.h"
#endif

typedef struct
{
    ca_omci_uint8_t keyLen;
    ca_omci_uint8_t key[OMCI_ENHANCE_SEC_CTL_KEYFRAG];
}ca_omci_enhance_key_part_t;


#define OMCI_ENHANCE_SEC_CTL_KEY_SET        0
#define OMCI_ENHANCE_SEC_CTL_KEY_CLEAR      1
#define OMCI_ENHANCE_SEC_CTL_KEY_CLEAR_TBL  2
#define OMCI_ENHANCE_SEC_KEY_MAX_ROW_NUM    16
#define OMCI_ENHANCE_SEC_KEY_MSG_LEN        256
#define OMCI_ENHANCE_SEC_KEY_PSK_LEN        16
#define OMCI_ENHANCE_SEC_KEY_RSL_LEN        16
#define OMCI_ENHANCE_SEC_KEY_MSK_LEN        16
#define OMCI_ENHANCE_SEC_KEY_NEW_MSK_MSG    256
#define OMCI_ENHANCE_SEC_KEY_MSK_NAME_TAIL  16
#define OMCI_ENHANCE_SEC_KEY_MSK_NAME_MSG   (OMCI_ENHANCE_SEC_KEY_NEW_MSK_MSG + 16)
#define OMCI_ENHANCE_SEC_AUTH_RSL_TAIL_LEN  8


#define OMCI_ENHANCE_SEC_MSK_NAME_TAIL    {\
    0x31,0x41,0x59,0x26,0x53,0x58,0x97,0x93,\
    0x31,0x41,0x59,0x26,0x53,0x58,0x97,0x93}

#define OMCI_ENHANCE_SEC_AUTH_RSL_TAIL    {\
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}


static ca_omci_timer_t sg_omciMcKeyCheckTimer;
static ca_omci_me_handler_t sg_omci_me_enhance_sec_ctl_handlers = {0};
ca_omci_bool_t g_smaEnable = CA_OMCI_FALSE;

ca_omci_me_attr_t g_omci_me_enhance_sec_ctl_attr[OMCI_ME_ATTR12] =
{
    {OMCI_ME_ATTR1,  "olt_crypto_cap"},
    {OMCI_ME_ATTR2,  "olt_ram_chall_table"},
    {OMCI_ME_ATTR3,  "olt_chall_st"},
    {OMCI_ME_ATTR4,  "onu_sel_cry_cap"},
    {OMCI_ME_ATTR5,  "onu_ram_chall_table"},
    {OMCI_ME_ATTR6,  "onu_auth_rsl_table"},
    {OMCI_ME_ATTR7,  "olt_auth_rsl_table"},
    {OMCI_ME_ATTR8,  "olt_rsl_st"},
    {OMCI_ME_ATTR9,  "onu_auth_st"},
    {OMCI_ME_ATTR10, "master_sess_key"},
    {OMCI_ME_ATTR11, "broadcast_key"},
    {OMCI_ME_ATTR12, "effec_ket_len"},
};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
static void ca_omci_report_onu_random_tbl_avc(ca_omci_avc_report_t *pAvcData);
void ca_omci_enhance_key_update_mc_key(void);


ca_omci_uint32_t ca_omci_me_enhance_sec_ctl_attr_len_get(void)
{
    ca_omci_enhance_sec_ctl_t *me = NULL;
    ca_omci_uint32_t attrLen = 0;

    attrLen += sizeof(me->attr.oltCryptoCap);
    attrLen += sizeof(me->attr.oltChallSt);
    attrLen += sizeof(me->attr.onuSelCryCap);
    attrLen += sizeof(me->attr.oltRslSt);
    attrLen += sizeof(me->attr.onuAuthSt);
    attrLen += sizeof(me->attr.masterSessKey);
    attrLen += sizeof(me->attr.effecKetLen);
    return attrLen;
}


ca_omci_mebase_t *ca_omci_me_enhance_sec_ctl_new(void)
{
    ca_omci_enhance_sec_ctl_t *me = (ca_omci_enhance_sec_ctl_t *)omci_malloc(sizeof(ca_omci_enhance_sec_ctl_t));

    if (NULL == me)
    {
        OMCIERR("No memory");
        return NULL;
    }

    ca_omci_base_init(&me->meBase, OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID, &sg_omci_me_enhance_sec_ctl_handlers);

    /* This me need to mib upload */
    OMCI_GET_MIBCNT_NUM(enhance_sec_ctl);

    return (ca_omci_mebase_t *)me;
}


ca_omci_st_e ca_omci_me_enhance_sec_ctl_set(
ca_omci_mebase_t        *me,
ca_omci_uint16_t         inMask,
ca_omci_uint8_t         *attr,
ca_omci_me_set_result_t *result)
{
    ca_omci_enhance_sec_ctl_t      *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;
    ca_omci_enhance_sec_ctl_attr_t  newVal;
    ca_omci_uint16_t           offset   = 0;
    ca_omci_uint16_t           mask     = 0;
    ca_omci_uint8_t            idx      = 0;
    ca_omci_uint8_t            index     = 0;
    ca_omci_uint8_t            keyAction = 0;
    ca_omci_uint8_t            keyIdx    = 0;
    ca_omci_hal_pon_enc_key_t  keyVal    = {0};
    ca_omci_avc_report_t       avcAttr   = {0};

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_MEMSET(&newVal, 0, sizeof(ca_omci_enhance_sec_ctl_attr_t));
    OMCI_CHECK_ATTR_MASK(OMCI_ENHANCE_SEC_CTL_SET_MASK_FULL);
    OMCI_UPDATE_ATTR_UINT_T(8, enhanceSecCtl->attr.oltCryptoCap, "oltCryptoCap", OMCI_ENHANCE_SEC_CTL_OLT_CRYPTO_CAP, 1);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.oltRamChallTable[0].rowNum, "rowNum");
        OMCI_SET_TBL_UINT8_T(newVal.oltRamChallTable[0].content, "content", OMCI_ENHANCE_SEC_CTL_CONTENT);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR2, mask);

        if (0 == newVal.oltRamChallTable[0].rowNum)
        {
            OMCI_MEMSET(enhanceSecCtl->attr.oltRamChallTable, 0, sizeof(enhanceSecCtl->attr.oltRamChallTable));
        }
        else
        {
            for (index = 0; index < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; index++)
            {
                if ((CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[index].valid)
                 && (enhanceSecCtl->attr.oltRamChallTable[index].rowNum == newVal.oltRamChallTable[0].rowNum))
                {
                    OMCI_MEMCPY(enhanceSecCtl->attr.oltRamChallTable[index].content,
                                newVal.oltRamChallTable[0].content,
                                OMCI_ENHANCE_SEC_CTL_CONTENT);
                    break;
                }

                if (CA_OMCI_FALSE == enhanceSecCtl->attr.oltRamChallTable[index].valid)
                {
                    enhanceSecCtl->attr.oltRamChallTable[index].valid  = CA_OMCI_TRUE;
                    enhanceSecCtl->attr.oltRamChallTable[index].rowNum = newVal.oltRamChallTable[0].rowNum;
                    OMCI_MEMCPY(enhanceSecCtl->attr.oltRamChallTable[index].content,
                                newVal.oltRamChallTable[0].content,
                                OMCI_ENHANCE_SEC_CTL_CONTENT);
                    break;
                }
            }
        }
    }

    OMCI_UPDATE_ATTR_UINT(8, enhanceSecCtl->attr.oltChallSt, "oltChallSt", 3);
    if ((OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR3, mask))
     && (CA_OMCI_TRUE == enhanceSecCtl->attr.oltChallSt))
    {
        //report authentication status first
        avcAttr.classId    = enhanceSecCtl->meBase.classId;
        avcAttr.instanceId = enhanceSecCtl->meBase.instanceId;
        avcAttr.attrNo     = OMCI_ME_ATTR5;
        ca_omci_report_onu_random_tbl_avc(&avcAttr);

        //report ONU random challenge table
        avcAttr.classId    = enhanceSecCtl->meBase.classId;
        avcAttr.instanceId = enhanceSecCtl->meBase.instanceId;
        avcAttr.attrNo     = OMCI_ME_ATTR6;
        ca_omci_report_onu_random_tbl_avc(&avcAttr);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR7, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.oltAuthRslTable[0].rowNum, "rowNum");
        OMCI_SET_TBL_UINT8_T(newVal.oltAuthRslTable[0].content, "content", OMCI_ENHANCE_SEC_CTL_CONTENT);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
    }

    OMCI_UPDATE_ATTR_UINT(8, enhanceSecCtl->attr.oltRslSt, "oltRslSt", 8);
    if ((OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR8, mask))
     && (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRslSt))
    {
        //report ONU authentication status
        enhanceSecCtl->attr.onuAuthSt = 3;//success
        avcAttr.classId    = enhanceSecCtl->meBase.classId;
        avcAttr.instanceId = enhanceSecCtl->meBase.instanceId;
        avcAttr.attrNo     = OMCI_ME_ATTR9;
        ca_omci_report_onu_random_tbl_avc(&avcAttr);
    }

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, inMask))
    {
        OMCI_SET_TBL_UINT8(newVal.broadcastKey[0].rowCtl, "rowCtl");
        OMCI_SET_TBL_UINT8(newVal.broadcastKey[0].rowId, "rowId");
        OMCI_SET_TBL_UINT8_T(newVal.broadcastKey[0].keyFrag, "keyFrag", OMCI_ENHANCE_SEC_CTL_KEYFRAG);
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);

        keyAction = newVal.broadcastKey[0].rowCtl & 0x03; //two least significant bits
        keyIdx    = (newVal.broadcastKey[0].rowId & 0xc0) >> 6; //two most significant bits
        if (OMCI_ENHANCE_SEC_CTL_KEY_CLEAR_TBL == keyAction)
        {
            OMCIMNGDBG("reset both key and clear table");
            OMCI_MEMSET(enhanceSecCtl->attr.broadcastKey, 0, (sizeof(ca_omci_sec_key_tbl_t) * OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY));
            OMCI_MEMSET(&keyVal, 0, sizeof(ca_omci_hal_pon_enc_key_t));
            keyVal.keyFlag = OMCI_ENHANCE_SEC_CTL_KEY_CLEAR_TBL;
            ca_omci_hal_onu_mc_key_set((void *)&keyVal);
            return CA_OMCI_OK;
        }

        //for flash table, don't care keyidx
        if ((keyAction > OMCI_ENHANCE_SEC_CTL_KEY_CLEAR_TBL)
         || (keyIdx > 2) || (keyIdx < 1))
        {
            result->result = OMCI_MSG_RST_PARAM_ERR;
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, result->exeMask);
            return CA_OMCI_E;
        }

        if (OMCI_ENHANCE_SEC_CTL_KEY_CLEAR == keyAction)
        {
            OMCI_MEMSET(&keyVal, 0, sizeof(ca_omci_hal_pon_enc_key_t));
            keyVal.keyIdx  = keyIdx;
            keyVal.keyFlag = OMCI_ENHANCE_SEC_CTL_KEY_CLEAR;
            ca_omci_hal_onu_mc_key_set((void *)&keyVal);

            for (index = 0; index < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; index++)
            {
                if (keyIdx == ((enhanceSecCtl->attr.broadcastKey[index].rowId & 0xc0) >> 6))
                {
                    OMCI_MEMSET(&enhanceSecCtl->attr.broadcastKey[index], 0, sizeof(ca_omci_sec_key_tbl_t));
                }
            }
            OMCIMNGDBG("clear key[%d] and clear relate row", keyVal.keyIdx);
            return CA_OMCI_OK;
        }

        //if this value is not 0, mean the key is not whole, not 16 bytes key
        if (0 != (newVal.broadcastKey[0].rowCtl & 0xf0))
        {
            OMCIMNGDBG("Set key not complete[%d]", keyIdx);
            for (index = 0; index < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; index++)
            {
                if ((CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[index].valid)
                 && (newVal.broadcastKey[0].rowId == enhanceSecCtl->attr.broadcastKey[index].rowId)
                 && (newVal.broadcastKey[0].rowCtl == enhanceSecCtl->attr.broadcastKey[index].rowCtl))
                {
                    OMCI_MEMCPY(enhanceSecCtl->attr.broadcastKey[index].keyFrag, newVal.broadcastKey[0].keyFrag, OMCI_ENHANCE_SEC_CTL_KEYFRAG);
                    ca_omci_enhance_key_update_mc_key();
                    OMCIMNGDBG("Update key fragment");
                    return CA_OMCI_OK;
                }
            }

            for (index = 0; index < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; index++)
            {
                if (CA_OMCI_FALSE == enhanceSecCtl->attr.broadcastKey[index].valid)
                {
                    enhanceSecCtl->attr.broadcastKey[index].valid  = CA_OMCI_TRUE;
                    enhanceSecCtl->attr.broadcastKey[index].rowCtl = CA_OMCI_TRUE;
                    enhanceSecCtl->attr.broadcastKey[index].rowId  = CA_OMCI_TRUE;
                    OMCI_MEMCPY(enhanceSecCtl->attr.broadcastKey[index].keyFrag,
                                newVal.broadcastKey[0].keyFrag,
                                OMCI_ENHANCE_SEC_CTL_KEYFRAG);
                    ca_omci_enhance_key_update_mc_key();
                    OMCIMNGDBG("Update key fragment");
                    return CA_OMCI_OK;
                }
            }
            return CA_OMCI_OK;
        }

        OMCIMNGDBG("Set 16 bytes key [%d]", keyIdx);
        //check if olt configure key idx before
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; index++)
        {
            if ((CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[index].valid)
             && (keyIdx == ((enhanceSecCtl->attr.broadcastKey[index].rowId & 0xc0) >> 6)))
            {
                //the key is 16bytes, so could just configure it to hal
                OMCI_MEMSET(&keyVal, 0, sizeof(ca_omci_hal_pon_enc_key_t));
                keyVal.keyIdx  = keyIdx;
                keyVal.keyFlag = OMCI_ENHANCE_SEC_CTL_KEY_SET;
                keyVal.keyLen  = OMCI_HAL_PON_KEY_MAX_LEN;
                OMCI_MEMCPY(keyVal.key, newVal.broadcastKey[0].keyFrag, OMCI_HAL_PON_KEY_MAX_LEN);
                ca_omci_hal_onu_mc_key_set((void *)&keyVal);

                OMCIMNGDBG("Update key idx[%d]", keyIdx);
                enhanceSecCtl->attr.broadcastKey[index].rowId  = newVal.broadcastKey[0].rowId;
                enhanceSecCtl->attr.broadcastKey[index].rowCtl = newVal.broadcastKey[0].rowCtl;
                OMCI_MEMCPY(enhanceSecCtl->attr.broadcastKey[index].keyFrag, newVal.broadcastKey[0].keyFrag, OMCI_HAL_PON_KEY_MAX_LEN);
                return CA_OMCI_OK;
            }
        }

        //save the value in me table
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; index++)
        {
            if (CA_OMCI_FALSE == enhanceSecCtl->attr.broadcastKey[index].valid)
            {
                enhanceSecCtl->attr.broadcastKey[index].valid  = CA_OMCI_TRUE;
                enhanceSecCtl->attr.broadcastKey[index].rowCtl = newVal.broadcastKey[0].rowCtl;
                enhanceSecCtl->attr.broadcastKey[index].rowId  = newVal.broadcastKey[0].rowId;
                OMCI_MEMCPY(enhanceSecCtl->attr.broadcastKey[index].keyFrag,
                            newVal.broadcastKey[0].keyFrag,
                            OMCI_ENHANCE_SEC_CTL_KEYFRAG);
                OMCI_MEMSET(&keyVal, 0, sizeof(ca_omci_hal_pon_enc_key_t));
                keyVal.keyIdx  = keyIdx;
                keyVal.keyFlag = OMCI_ENHANCE_SEC_CTL_KEY_SET;
                keyVal.keyLen  = OMCI_HAL_PON_KEY_MAX_LEN;
                OMCI_MEMCPY(keyVal.key, newVal.broadcastKey[0].keyFrag, OMCI_HAL_PON_KEY_MAX_LEN);
                ca_omci_hal_onu_mc_key_set((void *)&keyVal);

                OMCIMNGDBG("Set key idx[%d]", keyIdx);
                return CA_OMCI_OK;
            }
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_enhance_sec_ctl_get(
    ca_omci_mebase_t        *me,
    ca_omci_uint8_t          ext,
    ca_omci_uint16_t         inMask,
    ca_omci_uint8_t         *attr,
    ca_omci_me_get_result_t *result)
{
    ca_omci_enhance_sec_ctl_t *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);
    OMCI_CHECK_ATTR_MASK(OMCI_ENHANCE_SEC_CTL_GET_MASK_FULL);

    //TODO: update value in me

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.oltRamChallTable[0]) - sizeof(enhanceSecCtl->attr.oltRamChallTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR2, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&enhanceSecCtl->oltRamChallTable, tblSeq);
    }
    OMCI_GET_ATTR_UINT8(enhanceSecCtl->attr.oltChallSt, 3);
    OMCI_GET_ATTR_UINT8(enhanceSecCtl->attr.onuSelCryCap, 4);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.onuRamChallTable[0]) - sizeof(enhanceSecCtl->attr.onuRamChallTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuRamChallTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&enhanceSecCtl->onuRamChallTable, tblSeq);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.onuAuthRslTable[0]) - sizeof(enhanceSecCtl->attr.onuAuthRslTable[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_ONU_AUTH_RSL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuAuthRslTable[tblIdx].valid)
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
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        OMCI_MEMCPY((attr + offset), &tblSize, sizeof(ca_omci_uint32_t));
        offset += sizeof(ca_omci_uint32_t);
        result->attrLen += sizeof(ca_omci_uint32_t);
        ca_omci_next_init(&enhanceSecCtl->onuAuthRslTable, tblSeq);
    }
    OMCI_GET_ATTR_UINT8(enhanceSecCtl->attr.oltRslSt, 8);
    OMCI_GET_ATTR_UINT8(enhanceSecCtl->attr.onuAuthSt, 9);
    OMCI_GET_ATTR_UINT8_T(enhanceSecCtl->attr.masterSessKey, OMCI_ENHANCE_SEC_CTL_MASTER_SESS_KEY, 10);
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, inMask))
    {
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.broadcastKey[0]) - sizeof(enhanceSecCtl->attr.broadcastKey[0].valid);
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint16_t tblSeq  = 0;

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[tblIdx].valid)
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
        ca_omci_next_init(&enhanceSecCtl->broadcastKey, tblSeq);
    }
    OMCI_GET_ATTR_UINT16(enhanceSecCtl->attr.effecKetLen, 12);

    result->mask    = mask;
    result->exeMask = 0;
    result->result  = OMCI_MSG_RST_SUCCESSFUL;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_enhance_sec_ctl_get_next(
    ca_omci_mebase_t            *me,
    ca_omci_uint8_t              ext,
    ca_omci_uint16_t             seqNum,
    ca_omci_uint16_t             mask,
    ca_omci_uint8_t             *attr,
    ca_omci_me_getnext_result_t *result)
{
    ca_omci_enhance_sec_ctl_t *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;

    (void)ext; //remove warning
    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(attr);
    OMCI_PARAM_NULL_RETURN(result);

    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR2, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.oltRamChallTable[tblIdx]) - sizeof(enhanceSecCtl->attr.oltRamChallTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&enhanceSecCtl->oltRamChallTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        if (NULL == tblAttr)
        {
            result->result = OMCI_MSG_RST_PROC_ERR;
            return CA_OMCI_E_PARAM;
        }
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT8(enhanceSecCtl->attr.oltRamChallTable[tblIdx].rowNum);
                OMCI_MEMCPY((tblAttr + offset), &enhanceSecCtl->attr.oltRamChallTable[tblIdx].content, sizeof(enhanceSecCtl->attr.oltRamChallTable[tblIdx].content));
                offset += sizeof(enhanceSecCtl->attr.oltRamChallTable[tblIdx].content);
            }
        }

        offset = seqNum * OMCI_MSG_GET_NEXT_LEN;
        sglLen = OMCI_MSG_GET_NEXT_LEN;
        if (ext)
        {
            offset = seqNum * OMCI_MSG_EXT_GET_NEXT_LEN;
            sglLen = OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        attLen = (tblSize - offset) > sglLen ? sglLen : (tblSize - offset);
        OMCI_MEMCPY(attr, (tblAttr + offset), attLen);
        OMCI_FREE(tblAttr);
        result->attrLen = attLen;
        result->result  = OMCI_MSG_RST_SUCCESSFUL;
        ca_omci_next_success(&enhanceSecCtl->oltRamChallTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR5, mask))
    {
        ca_omci_uint16_t offset  = 0;

        if (!ca_omci_next_valid(&enhanceSecCtl->onuRamChallTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        if (0 == seqNum)
        {
            OMCI_MEMCPY((attr + offset), enhanceSecCtl->attr.onuRamChallTable[0].content, OMCI_ENHANCE_SEC_CTL_CONTENT);
            offset += OMCI_ENHANCE_SEC_CTL_CONTENT;
            result->attrLen += OMCI_ENHANCE_SEC_CTL_CONTENT;
            OMCI_MEMCPY((attr + offset), enhanceSecCtl->attr.onuRamChallTable[1].content, 13);
            offset += 13;
            result->attrLen += 13;
        }
        else if (1 == seqNum)
        {
            OMCI_MEMCPY((attr + offset), enhanceSecCtl->attr.onuRamChallTable[1].content+13, 3);
            offset += 3;
            result->attrLen += 3;
            OMCI_MEMCPY((attr + offset), enhanceSecCtl->attr.onuRamChallTable[2].content, OMCI_ENHANCE_SEC_CTL_CONTENT);
            offset += OMCI_ENHANCE_SEC_CTL_CONTENT;
            result->attrLen += OMCI_ENHANCE_SEC_CTL_CONTENT;
        }

        result->result  = OMCI_MSG_RST_SUCCESSFUL;
        ca_omci_next_success(&enhanceSecCtl->onuRamChallTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR6, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.onuAuthRslTable[tblIdx]) - sizeof(enhanceSecCtl->attr.onuAuthRslTable[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&enhanceSecCtl->onuAuthRslTable, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_ONU_AUTH_RSL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuAuthRslTable[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        if (NULL == tblAttr)
        {
            result->result = OMCI_MSG_RST_PROC_ERR;
            return CA_OMCI_E_PARAM;
        }
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_ONU_AUTH_RSL_TABLE; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuAuthRslTable[tblIdx].valid)
            {
                OMCI_MEMCPY((tblAttr + offset), &enhanceSecCtl->attr.onuAuthRslTable[tblIdx].content, sizeof(enhanceSecCtl->attr.onuAuthRslTable[tblIdx].content));
                offset += sizeof(enhanceSecCtl->attr.onuAuthRslTable[tblIdx].content);
            }
        }

        offset = seqNum * OMCI_MSG_GET_NEXT_LEN;
        sglLen = OMCI_MSG_GET_NEXT_LEN;
        if (ext)
        {
            offset = seqNum * OMCI_MSG_EXT_GET_NEXT_LEN;
            sglLen = OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        attLen = (tblSize - offset) > sglLen ? sglLen : (tblSize - offset);
        OMCI_MEMCPY(attr, (tblAttr + offset), attLen);
        OMCI_FREE(tblAttr);
        result->attrLen = attLen;
        result->result  = OMCI_MSG_RST_SUCCESSFUL;
        ca_omci_next_success(&enhanceSecCtl->onuAuthRslTable);
    }
    if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR11, mask))
    {
        ca_omci_uint32_t tblIdx  = 0;
        ca_omci_uint32_t tblLen  = sizeof(enhanceSecCtl->attr.broadcastKey[tblIdx]) - sizeof(enhanceSecCtl->attr.broadcastKey[tblIdx].valid);
        ca_omci_uint16_t offset  = 0;
        ca_omci_uint32_t tblSize = 0;
        ca_omci_uint16_t sglLen  = 0;
        ca_omci_uint16_t attLen  = 0;
        ca_omci_uint8_t  *tblAttr = NULL;

        if (!ca_omci_next_valid(&enhanceSecCtl->broadcastKey, seqNum))
        {
            OMCIERR("invalid seq num %d", seqNum);
            result->result = OMCI_MSG_RST_PARAM_ERR;
            return CA_OMCI_E_PARAM;
        }

        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[tblIdx].valid)
            {
                tblSize += tblLen;
            }
        }

        tblAttr = omci_malloc(tblSize);
        if (NULL == tblAttr)
        {
            result->result = OMCI_MSG_RST_PROC_ERR;
            return CA_OMCI_E_PARAM;
        }
        offset  = 0;
        for (tblIdx = 0; tblIdx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; tblIdx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[tblIdx].valid)
            {
                OMCI_CP_TBL_UINT8(enhanceSecCtl->attr.broadcastKey[tblIdx].rowCtl);
                OMCI_CP_TBL_UINT8(enhanceSecCtl->attr.broadcastKey[tblIdx].rowId);
                OMCI_MEMCPY((tblAttr + offset), &enhanceSecCtl->attr.broadcastKey[tblIdx].keyFrag, sizeof(enhanceSecCtl->attr.broadcastKey[tblIdx].keyFrag));
                offset += sizeof(enhanceSecCtl->attr.broadcastKey[tblIdx].keyFrag);
            }
        }

        offset = seqNum * OMCI_MSG_GET_NEXT_LEN;
        sglLen = OMCI_MSG_GET_NEXT_LEN;
        if (ext)
        {
            offset = seqNum * OMCI_MSG_EXT_GET_NEXT_LEN;
            sglLen = OMCI_MSG_EXT_GET_NEXT_LEN;
        }

        attLen = (tblSize - offset) > sglLen ? sglLen : (tblSize - offset);
        OMCI_MEMCPY(attr, (tblAttr + offset), attLen);
        OMCI_FREE(tblAttr);
        result->attrLen = attLen;
        result->result  = OMCI_MSG_RST_SUCCESSFUL;
        ca_omci_next_success(&enhanceSecCtl->broadcastKey);
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_me_enhance_sec_ctl_upload(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   ext,
    ca_omci_uint8_t  *contents,
    ca_omci_uint16_t *contLen,
    ca_omci_uint16_t  seqNum)
{
    ca_omci_enhance_sec_ctl_t *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;
    ca_omci_uint16_t offset  = 0;
    ca_omci_uint16_t mask    = 0;
    ca_omci_uint32_t idx     = 0;

    OMCI_PARAM_NULL_RETURN(me);
    OMCI_PARAM_NULL_RETURN(contents);
    OMCI_PARAM_NULL_RETURN(contLen);

    OMCI_UPLOAD_HDR_ENCODE(enhanceSecCtl);
    if (CA_OMCI_TRUE == ext)
    {
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltCryptoCap, 1);
        for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; idx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.oltRamChallTable[idx].rowNum);
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.oltRamChallTable[idx].content);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR2, mask);
            }
        }
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltChallSt, 3);
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.onuSelCryCap, 4);
        for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; idx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuRamChallTable[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.onuRamChallTable[idx].content);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
            }
        }
        for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_ONU_AUTH_RSL_TABLE; idx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuAuthRslTable[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.onuAuthRslTable[idx].content);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
            }
        }
        for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_OLT_AUTH_RSL_TABLE; idx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltAuthRslTable[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.oltAuthRslTable[idx].rowNum);
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.oltAuthRslTable[idx].content);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR7, mask);
            }
        }
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltRslSt, 8);
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.onuAuthSt, 9);
        OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.masterSessKey, 10);
        for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; idx++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.broadcastKey[idx].valid)
            {
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.broadcastKey[idx].rowCtl);
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.broadcastKey[idx].rowId);
                OMCI_UPLOAD_TBL_UINT8(enhanceSecCtl->attr.broadcastKey[idx].keyFrag);
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR11, mask);
            }
        }
        OMCI_UPLOAD_ATTR_SIZE(16, enhanceSecCtl->attr.effecKetLen, 12);
        OMCI_UPLOAD_MASK_ENCODE;
    }
    else
    {
        if (0 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltCryptoCap, 1);
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltChallSt, 3);
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.onuSelCryCap, 4);
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.oltRslSt, 8);
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.onuAuthSt, 9);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
            return CA_OMCI_OK;
        }

        if (1 == seqNum)
        {
            OMCI_UPLOAD_ATTR_SIZE(8, enhanceSecCtl->attr.masterSessKey, 10);
            OMCI_UPLOAD_ATTR_SIZE(16, enhanceSecCtl->attr.effecKetLen, 12);
            OMCI_BASIC_UPLOAD_MASK_ENCODE;
        }
    }
    return CA_OMCI_OK;
}


void ca_omci_me_enhance_sec_ctl_display(ca_omci_mebase_t *me, void *data)
{
    const struct shell *cli = (const struct shell *)data;
    ca_omci_enhance_sec_ctl_t *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;
    ca_omci_uint32_t idx = 0;
    ca_omci_uint32_t i   = 0;

    OMCI_PARAM_IS_NULL(me);
    (void)data;

    omci_sprint(cli, "Me: enhanceSecCtl \n");
    omci_sprint(cli, "    Instance: 0x%x \n", me->instanceId);
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_OLT_CRYPTO_CAP; idx++)
    {
        omci_sprint(cli, "        oltCryptoCap[%d]: 0x%x \n", idx, enhanceSecCtl->attr.oltCryptoCap[idx]);
    }
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; idx++)
    {
        if (CA_OMCI_FALSE == enhanceSecCtl->attr.oltRamChallTable[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        oltRamChallTable[%d]:rowNum 0x%x\n", idx, enhanceSecCtl->attr.oltRamChallTable[idx].rowNum);
        for (i = 0; i < OMCI_ENHANCE_SEC_CTL_CONTENT; i++)
        {
            omci_sprint(cli, "        oltRamChallTable[%d].content[%d]: 0x%x \n", idx, i, enhanceSecCtl->attr.oltRamChallTable[idx].content[i]);
        }
    }
    omci_sprint(cli, "        oltChallSt:  0x%x\n", enhanceSecCtl->attr.oltChallSt);
    omci_sprint(cli, "        onuSelCryCap: 0x%x\n", enhanceSecCtl->attr.onuSelCryCap);
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; idx++)
    {
        if (CA_OMCI_FALSE == enhanceSecCtl->attr.onuRamChallTable[idx].valid)
        {
            continue;
        }
        for (i = 0; i < OMCI_ENHANCE_SEC_CTL_CONTENT; i++)
        {
            omci_sprint(cli, "        onuRamChallTable[%d].content[%d]: 0x%x \n", idx, i, enhanceSecCtl->attr.onuRamChallTable[idx].content[i]);
        }
    }
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_ONU_AUTH_RSL_TABLE; idx++)
    {
        if (CA_OMCI_FALSE == enhanceSecCtl->attr.onuAuthRslTable[idx].valid)
        {
            continue;
        }
        for (i = 0; i < OMCI_ENHANCE_SEC_CTL_CONTENT; i++)
        {
            omci_sprint(cli, "        onuAuthRslTable[%d].content[%d]: 0x%x \n", idx, i, enhanceSecCtl->attr.onuAuthRslTable[idx].content[i]);
        }
    }
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_OLT_AUTH_RSL_TABLE; idx++)
    {
        if (CA_OMCI_FALSE == enhanceSecCtl->attr.oltAuthRslTable[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        oltAuthRslTable[%d]:rowNum 0x%x\n", idx, enhanceSecCtl->attr.oltAuthRslTable[idx].rowNum);
        for (i = 0; i < OMCI_ENHANCE_SEC_CTL_CONTENT; i++)
        {
            omci_sprint(cli, "        oltAuthRslTable[%d].content[%d]: 0x%x \n", idx, i, enhanceSecCtl->attr.oltAuthRslTable[idx].content[i]);
        }
    }
    omci_sprint(cli, "        oltRslSt:    0x%x\n", enhanceSecCtl->attr.oltRslSt);
    omci_sprint(cli, "        onuAuthSt:   0x%x\n", enhanceSecCtl->attr.onuAuthSt);
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_MASTER_SESS_KEY; idx++)
    {
        omci_sprint(cli, "        masterSessKey[%d]: 0x%x \n", idx, enhanceSecCtl->attr.masterSessKey[idx]);
    }
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; idx++)
    {
        if (CA_OMCI_FALSE == enhanceSecCtl->attr.broadcastKey[idx].valid)
        {
            continue;
        }
        omci_sprint(cli, "        broadcastKey[%d]:rowCtl 0x%x\n", idx, enhanceSecCtl->attr.broadcastKey[idx].rowCtl);
        omci_sprint(cli, "        broadcastKey[%d]:rowId 0x%x\n", idx, enhanceSecCtl->attr.broadcastKey[idx].rowId);
        for (i = 0; i < OMCI_ENHANCE_SEC_CTL_KEYFRAG; i++)
        {
            omci_sprint(cli, "        broadcastKey[%d].keyFrag[%d]: 0x%x \n", idx, i, enhanceSecCtl->attr.broadcastKey[idx].keyFrag[i]);
        }
    }
    omci_sprint(cli, "        effecKetLen: 0x%x\n", enhanceSecCtl->attr.effecKetLen);
}


void ca_omci_me_enhance_sec_ctl_init(void)
{
    sg_omci_me_enhance_sec_ctl_handlers.meDisplay  = ca_omci_me_enhance_sec_ctl_display;
    sg_omci_me_enhance_sec_ctl_handlers.uploadProc = ca_omci_me_enhance_sec_ctl_upload;

    sg_omci_me_enhance_sec_ctl_handlers.setProc     = ca_omci_me_enhance_sec_ctl_set;
    sg_omci_me_enhance_sec_ctl_handlers.getProc     = ca_omci_me_enhance_sec_ctl_get;
    sg_omci_me_enhance_sec_ctl_handlers.getNextProc = ca_omci_me_enhance_sec_ctl_get_next;
}


void ca_omci_key_update_timeout_handle(void *pData)
{
    ca_omci_enhance_key_part_t  keyPart[OMCI_ENHANCE_SEC_KEY_MAX_ROW_NUM];
    ca_omci_enhance_sec_ctl_t  *me      = NULL;
    ca_omci_uint8_t             idx     = 0;
    ca_omci_hal_pon_enc_key_t   keyVal  = {0};
    ca_omci_uint8_t             rowNum  = 0;
    ca_omci_uint8_t             row     = 0;
    ca_omci_uint8_t             keySize = 0;
    ca_omci_uint8_t             offset  = 0;

    (void)pData;

    me = (ca_omci_enhance_sec_ctl_t *)ca_omci_me_entry_lookup(OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID, 0);
    OMCI_PARAM_IS_NULL(me)

    OMCI_MEMSET(&keyVal,  0, sizeof(ca_omci_hal_pon_enc_key_t));
    OMCI_MEMSET(&keyPart, 0, sizeof(ca_omci_enhance_key_part_t) * OMCI_ENHANCE_SEC_KEY_MAX_ROW_NUM);
    keyVal.keyLen  = OMCI_HAL_PON_KEY_MAX_LEN;
    keyVal.keyFlag = OMCI_ENHANCE_SEC_CTL_KEY_SET;

    //combine the key to a whole one
    //there will be only one key in table, cause only one key is active in the same time
    for (idx = 0; idx < OMCI_ENHANCE_SEC_CTL_BROADCAST_KEY; idx++)
    {
        if ((CA_OMCI_TRUE == me->attr.broadcastKey[idx].valid)
         && (0 != (me->attr.broadcastKey[idx].rowCtl & 0xf)))
        {
            keyVal.keyIdx           = (me->attr.broadcastKey[idx].rowId & 0xc0) >> 6;
            rowNum                  = me->attr.broadcastKey[idx].rowId & 0xf;
            keyPart[rowNum].keyLen  = me->attr.broadcastKey[idx].rowCtl & 0xf;
            keySize                += keyPart[rowNum].keyLen;
            OMCI_MEMCPY(keyPart[rowNum].key, me->attr.broadcastKey[idx].keyFrag, keyPart[rowNum].keyLen);
            row++;
        }
    }

    if (keySize != OMCI_ENHANCE_SEC_CTL_KEYFRAG)
    {
        OMCIMNGERR("Error key size len");
        return;
    }

    for (idx = 0; idx < row; idx++)
    {
        OMCI_MEMCPY((keyVal.key + offset), keyPart[idx].key, keyPart[idx].keyLen);
        offset += keyPart[idx].keyLen;
    }
    ca_omci_hal_onu_mc_key_set((void *)&keyVal);
}


void ca_omci_key_update_timeout(void *pData)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_KEY_CHECK;

    (void)pData;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_timer_def_e);//no pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, msg.hdr.dataLen);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


void ca_omci_enhance_key_update_mc_key(void)
{
    OMCI_ENTER;

    if (CA_INVALID_TIMER == sg_omciMcKeyCheckTimer.timerId)
    {
        OMCIMNGDBG("first update key timer start");
        sg_omciMcKeyCheckTimer.timerId  = ca_timer_add(OMCI_TIMER_KEY_CHECK,
                                                       ca_omci_key_update_timeout,
                                                      (void *)NULL);
        sg_omciMcKeyCheckTimer.timerFlag = CA_OMCI_TRUE;
        return;
    }

    if (CA_OMCI_TRUE == sg_omciMcKeyCheckTimer.timerFlag)
    {
        OMCIMNGDBG("Refresh update key timer");
        ca_timer_stop(sg_omciMcKeyCheckTimer.timerId);
        ca_timer_start(sg_omciMcKeyCheckTimer.timerId);
    }
}


ca_omci_st_e ca_omci_enhance_avc_report(
    ca_omci_mebase_t *me,
    ca_omci_uint16_t  attrId,
    void *data)
{
    ca_omci_enhance_sec_ctl_t *enhanceSecCtl = (ca_omci_enhance_sec_ctl_t *)me;
#ifndef CONFIG_CA_OMCI_SUPPORT_OTP
    ca_omci_onuinfo_obj_t     *pOnuInfo      = ca_omci_onuinfo_get();
#endif
    ca_omci_uint16_t           mask          = 0;
    ca_omci_pkt_avc_t          avcAttr       = {0};
    ca_omci_uint16_t           index         = 0;
    ca_omci_uint32_t           attrLen       = 0;
    ca_omci_uint16_t           offset        = 0;
    ca_omci_uint16_t           number        = 0;
    ca_omci_uint8_t            pskMsg[OMCI_ENHANCE_SEC_KEY_MSG_LEN];
    ca_omci_uint8_t            rsl[OMCI_ENHANCE_SEC_KEY_RSL_LEN];
    ca_omci_uint8_t            psk[OMCI_ENHANCE_SEC_KEY_PSK_LEN];
    ca_omci_uint8_t            onuRslTail[OMCI_ENHANCE_SEC_AUTH_RSL_TAIL_LEN] = OMCI_ENHANCE_SEC_AUTH_RSL_TAIL;
    ca_omci_uint8_t            msk[OMCI_ENHANCE_SEC_KEY_MSK_LEN];
    ca_omci_uint8_t            mskMsg[OMCI_ENHANCE_SEC_KEY_NEW_MSK_MSG];
    ca_omci_uint8_t            mskNameMsg[OMCI_ENHANCE_SEC_KEY_MSK_NAME_MSG];
    ca_omci_uint8_t            mskName[OMCI_ENHANCE_SEC_KEY_MSK_LEN];
    ca_omci_uint8_t            mskNameTail[OMCI_ENHANCE_SEC_KEY_MSK_NAME_TAIL] = OMCI_ENHANCE_SEC_MSK_NAME_TAIL;

    (void)data;

    if (OMCI_ME_ATTR5 == attrId)
    {
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; index++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuRamChallTable[index].valid)
            {
                attrLen += OMCI_ENHANCE_SEC_CTL_CONTENT;
            }
        }

        OMCI_MEMSET(&avcAttr, 0, sizeof(ca_omci_pkt_avc_t));
        avcAttr.attrNum         = 1;
        avcAttr.attr[0].pAttr   = omci_malloc(sizeof(ca_omci_uint32_t));
        avcAttr.attr[0].attrLen = sizeof(ca_omci_uint32_t);
        attrLen                 = ca_omci_htonl(attrLen);
        OMCI_MEMCPY(avcAttr.attr[0].pAttr, &attrLen, sizeof(ca_omci_uint32_t));
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR5, mask);
        omci_codec_encode_avc(me, mask, &avcAttr);
        OMCI_FREE(avcAttr.attr[0].pAttr);
        return CA_OMCI_OK;
    }

    if (OMCI_ME_ATTR6 == attrId)
    {
        attrLen = OMCI_ENHANCE_SEC_CTL_CONTENT;

        OMCI_MEMSET(&avcAttr, 0, sizeof(ca_omci_pkt_avc_t));
        avcAttr.attrNum         = 1;
        avcAttr.attr[0].pAttr   = omci_malloc(sizeof(ca_omci_uint32_t));
        avcAttr.attr[0].attrLen = sizeof(ca_omci_uint32_t);
        attrLen                 = ca_omci_htonl(attrLen);
        OMCI_MEMCPY(avcAttr.attr[0].pAttr, &attrLen, sizeof(ca_omci_uint32_t));
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR6, mask);
        omci_codec_encode_avc(me, mask, &avcAttr);
        OMCI_FREE(avcAttr.attr[0].pAttr);

        OMCI_MEMSET(pskMsg, 0, OMCI_ENHANCE_SEC_KEY_MSG_LEN);
        OMCI_MEMSET(psk,    0, OMCI_ENHANCE_SEC_KEY_PSK_LEN);
        OMCI_MEMSET(rsl,    0, OMCI_ENHANCE_SEC_KEY_RSL_LEN);

        pskMsg[0] = 1;
        offset    = 1;
        number    = 0;
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; index++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[index].valid)
            {
                OMCI_MEMCPY(pskMsg + offset + number * OMCI_ENHANCE_SEC_CTL_CONTENT,
                            enhanceSecCtl->attr.oltRamChallTable[index].content,
                            OMCI_ENHANCE_SEC_CTL_CONTENT);
                number++;
            }
        }

        offset += number * OMCI_ENHANCE_SEC_CTL_CONTENT;
        number  = 0;
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; index++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuRamChallTable[index].valid)
            {
                OMCI_MEMCPY(pskMsg + offset + number * OMCI_ENHANCE_SEC_CTL_CONTENT,
                            enhanceSecCtl->attr.onuRamChallTable[index].content,
                            OMCI_ENHANCE_SEC_CTL_CONTENT);
                number++;
            }
        }
        offset += number * OMCI_ENHANCE_SEC_CTL_CONTENT;
        OMCI_MEMCPY(pskMsg + offset, onuRslTail, OMCI_ENHANCE_SEC_AUTH_RSL_TAIL_LEN);
        offset += OMCI_ENHANCE_SEC_AUTH_RSL_TAIL_LEN;

        //Get PSK
#ifdef CONFIG_CA_OMCI_SUPPORT_OTP
        cap_otp_omci_psk_get(0, psk);
        OMCIMNGDBG("Get PSK 0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                    psk[0], psk[1], psk[2], psk[3], psk[4], psk[5], psk[6], psk[7], psk[8],
                    psk[9], psk[10], psk[11], psk[12], psk[13], psk[14], psk[15]);
#else
        OMCI_MEMCPY(psk, pOnuInfo->psk, OMCI_ENHANCE_SEC_KEY_PSK_LEN);
#endif

        //calculate the onu auth result
        (void) aes128_cmac_compute( psk, pskMsg, offset, OMCI_ENHANCE_SEC_KEY_RSL_LEN, rsl);
        OMCI_MEMCPY(enhanceSecCtl->attr.onuAuthRslTable[0].content, rsl, OMCI_ENHANCE_SEC_KEY_RSL_LEN);
        enhanceSecCtl->attr.onuAuthRslTable[0].valid = CA_OMCI_TRUE;
        return CA_OMCI_OK;
    }

    if (OMCI_ME_ATTR9 == attrId)
    {
        //update OMCI MSK first then report this AVC
        //generate the new msk and msk name;
        OMCI_MEMSET(msk,        0, OMCI_ENHANCE_SEC_KEY_MSK_LEN);
        OMCI_MEMSET(mskName,    0, OMCI_ENHANCE_SEC_KEY_MSK_LEN);
        OMCI_MEMSET(mskMsg,     0, OMCI_ENHANCE_SEC_KEY_NEW_MSK_MSG);
        OMCI_MEMSET(mskNameMsg, 0, OMCI_ENHANCE_SEC_KEY_MSK_NAME_MSG);
        OMCI_MEMSET(psk,        0, OMCI_ENHANCE_SEC_KEY_PSK_LEN);

        //Get PSK
#ifdef CONFIG_CA_OMCI_SUPPORT_OTP
        cap_otp_omci_psk_get(0, psk);
        OMCIMNGDBG("Get PSK 0x%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x",
                    psk[0], psk[1], psk[2], psk[3], psk[4], psk[5], psk[6], psk[7], psk[8],
                    psk[9], psk[10], psk[11], psk[12], psk[13], psk[14], psk[15]);
#else
        OMCI_MEMCPY(psk, pOnuInfo->psk, OMCI_ENHANCE_SEC_KEY_PSK_LEN);
#endif

        //calculate msk
        number = 0;
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_OLT_RAM_CHALL_TABLE; index++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.oltRamChallTable[index].valid)
            {
                OMCI_MEMCPY(mskMsg + number * OMCI_ENHANCE_SEC_CTL_CONTENT,
                            enhanceSecCtl->attr.oltRamChallTable[index].content,
                            OMCI_ENHANCE_SEC_CTL_CONTENT);
                number++;
            }
        }
        offset = number * OMCI_ENHANCE_SEC_CTL_CONTENT;

        number = 0;
        for (index = 0; index < OMCI_ENHANCE_SEC_CTL_ONU_RAM_CHALL_TABLE; index++)
        {
            if (CA_OMCI_TRUE == enhanceSecCtl->attr.onuRamChallTable[index].valid)
            {
                OMCI_MEMCPY(mskMsg + offset + number * OMCI_ENHANCE_SEC_CTL_CONTENT,
                            enhanceSecCtl->attr.onuRamChallTable[index].content,
                            OMCI_ENHANCE_SEC_CTL_CONTENT);
                number++;
            }
        }
        offset += number * OMCI_ENHANCE_SEC_CTL_CONTENT;//total len

        (void) aes128_cmac_compute( psk, mskMsg, offset, OMCI_ENHANCE_SEC_KEY_MSK_LEN, msk);
        ca_omci_update_omci_msk(msk);
        ca_omci_alarm_dg_update();

        //calculate msk name
        OMCI_MEMCPY(mskNameMsg, mskMsg, offset);
        OMCI_MEMCPY(mskNameMsg + offset, mskNameTail, OMCI_ENHANCE_SEC_KEY_MSK_NAME_TAIL);
        (void) aes128_cmac_compute( psk, mskNameMsg, (offset + OMCI_ENHANCE_SEC_KEY_MSK_NAME_TAIL), OMCI_ENHANCE_SEC_KEY_MSK_LEN, mskName);
        OMCI_MEMCPY(enhanceSecCtl->attr.masterSessKey, mskName, OMCI_ENHANCE_SEC_CTL_MASTER_SESS_KEY);

        //set ploam msk flag
        (void) ca_omci_hal_onu_msk_set((void *)&msk);

        OMCI_MEMSET(&avcAttr, 0, sizeof(ca_omci_pkt_avc_t));
        avcAttr.attrNum         = 1;
        avcAttr.attr[0].pAttr   = omci_malloc(sizeof(ca_omci_uint8_t));
        avcAttr.attr[0].attrLen = sizeof(ca_omci_uint8_t);
        OMCI_MEMCPY(avcAttr.attr[0].pAttr, &enhanceSecCtl->attr.onuAuthSt, sizeof(ca_omci_uint8_t));
        OMCI_MASK_ATTR_SET(OMCI_ME_ATTR9, mask);
        omci_codec_encode_avc(me, mask, &avcAttr);
        OMCI_FREE(avcAttr.attr[0].pAttr);
        //Update MSK flag here
        g_smaEnable = CA_OMCI_TRUE;
        return CA_OMCI_OK;
    }

    return CA_OMCI_OK;
}

static void ca_omci_report_onu_random_tbl_avc(
    ca_omci_avc_report_t *pAvcData)
{
    ca_omci_inner_msg_t    msg;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();

    if (onuinfo->omccVersion < ONU2_G_OMCC_VER_988_AMD2_E)
    {
        OMCIMNGWARN("Don't report the AVC to OLT for OMCC version is 0x%x", onuinfo->omccVersion);
        return;
    }

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_AVC;
    msg.hdr.dataLen = sizeof(ca_omci_avc_report_t);
    OMCI_MEMCPY(msg.data, pAvcData, sizeof(ca_omci_avc_report_t));

    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIMNGERR("send to inner mq error");
    }
}


