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
#include "omci_me_onu.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_hal.h"
#include "omci_mng_onuinfo.h"
#include "omci_me_onu_data.h"
#include "omci_me_onu_g.h"
#include "omci_me_onu2_g.h"
#include "omci_me_soft_image.h"
#include "omci_me_cardholder.h"
#include "omci_me_circuit_pack.h"
#include "omci_me_onu_power_shed.h"
#include "omci_me_port_map_pkg.h"
#include "omci_me_protect_data.h"
#include "omci_me_onu_remote_debug.h"
#include "omci_me_dyn_power_ctl.h"
#include "omci_me_ani_g.h"
#include "omci_me_tcont.h"
#include "omci_me_traffic_scheg.h"
#include "omci_me_pri_queue.h"
#include "omci_me_ip_host.h"
#include "omci_me_ipv6_host.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_veip.h"
#include "omci_me_uni_g.h"
#include "omci_me_mac_brg_cfg.h"
#include "omci_me_olt_g.h"
#include "omci_me_mac_port_des.h"
#include "omci_me_mac_port_filter.h"
#include "omci_me_mac_port_brg.h"
#include "omci_me_mac_port_filter_pre.h"
#include "omci_me_pptp_pots.h"
#include "omci_me_voip_cfg.h"
#include "omci_me_voip_line_st.h"
#include "omci_me_sip_cfg_portal.h"
#include "omci_me_port_map_pkg.h"
#include "omci_me_dot1ag_md_level.h"
#include "omci_me_dot1ag_mep_st.h"
#include "omci_me_dot1ag_mep_ccm.h"
#include "omci_me_dot1ag_cfm.h"
#include "omci_me_dot1ag_chas_info.h"
#include "omci_me_file_trans_ctl.h"
#include "omci_me_bbrtr69_mng_srv.h"
#include "omci_me_enhance_sec_ctl.h"
#include "omci_me_brv6_pre_assi.h"
#include "omci_me_onu3_g.h"
#include "omci_me_omci.h"
#include "omci_me_dot1x_port_ext_pkg.h"
#include "omci_me_alcl_ds_tag_oper1.h"
#include "omci_me_alcl_ont_genv2.h"
#include "omci_mng_upgrade.h"


ca_omci_mebase_t *ca_omci_me_pri_queue_auto_create(ca_omci_uint16_t instance, ca_omci_uint32_t relaPort);
void ca_omci_me_dot1x_port_ext_pkg_auto_create(ca_omci_uint16_t instance);
static ca_omci_uint8_t ca_omci_onu_card_type_get(ca_omci_uint8_t mode);


#if DESCRIPTION("ALCL me")
void ca_omci_me_alcl_ds_tag_oper1_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_alcl_ds_tag_oper1_t *me = NULL;

    me = (ca_omci_alcl_ds_tag_oper1_t *)ca_omci_me_new(OMCI_ME_ALCL_DS_TAG_OPER1_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId   = instance;
    me->attr.uniDefBehavior = ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PASS_TAG;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_alcl_ont_genv2_auto_create(void)
{
    ca_omci_alcl_ont_genv2_t *me = NULL;

    me = (ca_omci_alcl_ont_genv2_t *)ca_omci_me_new(OMCI_ME_ALCL_ONT_GENV2_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId         = 0;
    me->attr.igmpSnoopEnable      = ALCL_ONT_GENV2_IGMP_SNOOP_ENABLE_IPOE;
    me->attr.forceDataStorCmd     = 0;
    me->attr.timeLastDataStorOper = 0;
    me->attr.craftAdminIndi       = ALCL_ONT_GENV2_CRAFT_ENABLE;
    me->attr.ethPtpEnableIndi     = ALCL_ONT_GENV2_INDICATOR_DISABLE;
    me->attr.stMcastVlanTransIndi = ALCL_ONT_GENV2_INDICATOR_DISABLE;
    me->attr.aluMeCapBitmap       = 0x10080;//now support 65296 and 65305
    me->attr.flexPqCap            = 1; //Flexible
    me->attr.slid_cap             = ALCL_ONT_GENV2_SLID_DISABLE;
    me->attr.aluSpeBhvField       = 1;
    //me->attr.aluMeCapBitmapP2     = 0;
    //me->attr.ypSn;

    ca_omci_me_entry_add(&me->meBase);
}
#endif


#if DESCRIPTION("9.13  Miscellaneous services")
void ca_omci_me_enhance_sec_ctl_auto_create(void)
{
    ca_omci_enhance_sec_ctl_t *me = NULL;
    ca_omci_uint8_t content0[OMCI_ENHANCE_SEC_CTL_CONTENT] = {0x5a,0x78,0x2d,0xc2,0x70,0xe2,0x2a,0xeb,
                                                              0x89,0x3c,0xa6,0x4b,0x3f,0x88,0x16,0x29};
    ca_omci_uint8_t content1[OMCI_ENHANCE_SEC_CTL_CONTENT] = {0x1a,0x15,0x6c,0x9f,0xa1,0xeb,0xc2,0x84,
                                                              0x93,0x28,0x7e,0xa0,0xff,0xb2,0x2b,0xbd};
    ca_omci_uint8_t content2[OMCI_ENHANCE_SEC_CTL_CONTENT] = {0x97,0xa9,0xbf,0x2c,0x83,0x3a,0xc5,0x2c,
                                                              0xf8,0x41,0xa6,0x94,0x42,0x5e,0x9e,0x80};

    me = (ca_omci_enhance_sec_ctl_t *)ca_omci_me_new(OMCI_ME_ENHANCE_SEC_CTL_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    me->attr.onuRamChallTable[0].valid = CA_OMCI_TRUE;
    me->attr.onuRamChallTable[1].valid = CA_OMCI_TRUE;
    me->attr.onuRamChallTable[2].valid = CA_OMCI_TRUE;
    OMCI_MEMCPY(me->attr.onuRamChallTable[0].content, content0, OMCI_ENHANCE_SEC_CTL_CONTENT);
    OMCI_MEMCPY(me->attr.onuRamChallTable[1].content, content1, OMCI_ENHANCE_SEC_CTL_CONTENT);
    OMCI_MEMCPY(me->attr.onuRamChallTable[2].content, content2, OMCI_ENHANCE_SEC_CTL_CONTENT);
    me->attr.onuSelCryCap = 1;

    //me->attr.oltCryptoCap = ;
    //me->attr.oltRamChallTable = ;
    //me->attr.oltChallSt = ;
    //me->attr.onuRamChallTable = ;
    //me->attr.onuAuthRslTable = ;
    //me->attr.oltAuthRslTable = ;
    //me->attr.oltRslSt = ;
    //me->attr.onuAuthSt = ;
    //me->attr.masterSessKey = ;
    //me->attr.broadcastKey = ;
    //me->attr.effecKetLen = ;
    ca_omci_me_entry_add(&me->meBase);
}
#endif


#if DESCRIPTION("9.12 General purpose MEs")
/* create this me when you have tr69 configure */
void ca_omci_me_bbrtr69_mng_srv_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_bbrtr69_mng_srv_t *me = NULL;

    me = (ca_omci_bbrtr69_mng_srv_t *)ca_omci_me_new(OMCI_ME_BBRTR69_MNG_SRV_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.admin        = CA_OMCI_LOCK;
    //me->attr.acsNetAddr = ;
    //me->attr.assocTag = ;
    ca_omci_me_entry_add(&me->meBase);
}


/*  if ONU supports out-of-band file transfer need to create this me */
void ca_omci_me_file_trans_ctl_auto_create(void)
{
    ca_omci_file_trans_ctl_t *me = NULL;

    me = (ca_omci_file_trans_ctl_t *)ca_omci_me_new(OMCI_ME_FILE_TRANS_CTL_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    //me->attr.supTransProt = ;
    //me->attr.fileType = ;
    //me->attr.fileInstance = ;
    //me->attr.localFileNamePoint = ;
    //me->attr.netAddrPoint = ;
    //me->attr.fileTransTrigger = ;
    //me->attr.fileTransSt = ;
    //me->attr.gemIwtpPoint = ;
    //me->attr.vlan = ;
    //me->attr.fileSize = ;
    //me->attr.direcListTbl = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_olt_g_auto_create(void)
{
    ca_omci_olt_g_t      *me  = NULL;
    ca_omci_uint8_t       idx = 0;

    me = (ca_omci_olt_g_t *)ca_omci_me_new(OMCI_ME_OLT_G_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    me->attr.timeOfDay[0] = OMCI_STR_SPACE;
    me->attr.timeOfDay[1] = OMCI_STR_SPACE;
    me->attr.timeOfDay[2] = OMCI_STR_SPACE;
    me->attr.timeOfDay[3] = OMCI_STR_SPACE;

    for (idx = 0; idx < OMCI_VENDOR_ID_LEN; idx++)
    {
        me->attr.oltVendorId[idx] = OMCI_STR_SPACE;
    }

    for (idx = 0; idx < OMCI_EQID_LEN; idx++)
    {
        me->attr.eqId[idx] = OMCI_STR_SPACE;
    }

    for (idx = 0; idx < OMCI_VERSION_LEN; idx++)
    {
        me->attr.version[idx] = 0x30;
    }

    ca_omci_me_entry_add(&me->meBase);
}


//veip only used when cap is not only OMCI
void ca_omci_me_uni_g_auto_create(
    ca_omci_uint16_t instance,
    ca_omci_uint8_t  cap,
    ca_omci_uint16_t veip)
{
    ca_omci_uni_g_t *me = NULL;

    me = (ca_omci_uni_g_t *)ca_omci_me_new(OMCI_ME_UNI_G_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.admin        = CA_OMCI_UNLOCK;
    me->attr.mngCap       = cap;
    if (UNI_G_MNG_CAP_OMCI != me->attr.mngCap)
    {
        me->attr.nonOmciMngId = veip;
    }

    //There is no need for OMCI to create large string
    //Could wait olt to create is and the set this relay agent option point to it
    //me->attr.relayAgtOp   = 65534;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_omci_auto_create(void)
{
    ca_omci_omci_t   *me  = NULL;
    ca_omci_uint32_t  i   = 0;
    ca_omci_uint16_t  cls = 0;

    me = (ca_omci_omci_t *)ca_omci_me_new(OMCI_ME_OMCI_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    for (i = OMCI_ME_ONU_DATA_CLASS_ID; i < OMCI_ME_CLASS_ID_MAX; i++){
        cls = ca_omci_me_get_standard_class(i);
        me->attr.meTypeTbl[i] = ca_omci_htons(cls);
    }

    me->attr.msgTypeTbl[0]  = OMCI_MSG_TYPE_CREATE;
    me->attr.msgTypeTbl[1]  = OMCI_MSG_TYPE_DELETE;
    me->attr.msgTypeTbl[2]  = OMCI_MSG_TYPE_SET;
    me->attr.msgTypeTbl[3]  = OMCI_MSG_TYPE_GET;
    me->attr.msgTypeTbl[4]  = OMCI_MSG_TYPE_GET_ALL_ALM;
    me->attr.msgTypeTbl[5]  = OMCI_MSG_TYPE_GET_ALL_ALM_NEXT;
    me->attr.msgTypeTbl[6]  = OMCI_MSG_TYPE_MIB_UPLOAD;
    me->attr.msgTypeTbl[7]  = OMCI_MSG_TYPE_MIB_UPLOAD_NEXT;
    me->attr.msgTypeTbl[8]  = OMCI_MSG_TYPE_MIB_RESET;
    me->attr.msgTypeTbl[9]  = OMCI_MSG_TYPE_ALARM;
    me->attr.msgTypeTbl[10] = OMCI_MSG_TYPE_ATTR_VALUE_CHANGE;
    me->attr.msgTypeTbl[11] = OMCI_MSG_TYPE_TEST;
    me->attr.msgTypeTbl[12] = OMCI_MSG_TYPE_START_SW_DOWNLOAD;
    me->attr.msgTypeTbl[13] = OMCI_MSG_TYPE_DOWNLOAD_SEC;
    me->attr.msgTypeTbl[14] = OMCI_MSG_TYPE_END_SW_DOWNLOAD;
    me->attr.msgTypeTbl[15] = OMCI_MSG_TYPE_ACTIVATE_SW;
    me->attr.msgTypeTbl[16] = OMCI_MSG_TYPE_COMMIT_SW;
    me->attr.msgTypeTbl[17] = OMCI_MSG_TYPE_SYNCH_TIME;
    me->attr.msgTypeTbl[18] = OMCI_MSG_TYPE_REBOOT;
    me->attr.msgTypeTbl[19] = OMCI_MSG_TYPE_GET_NEXT;
    me->attr.msgTypeTbl[20] = OMCI_MSG_TYPE_TEST_RESULT;
    me->attr.msgTypeTbl[21] = OMCI_MSG_TYPE_GET_CURR_DATA;
    me->attr.msgTypeTbl[22] = OMCI_MSG_TYPE_SET_TABLE;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_gen_auto_create(void)
{
    ca_omci_me_olt_g_auto_create();
    ca_omci_me_omci_auto_create();
}
#endif


#if DESCRIPTION("9.9  Voice services")
/* This me only create when non-omci sip support, so not call it now*/
void ca_omci_me_sip_cfg_portal_auto_create(void)
{
    ca_omci_sip_cfg_portal_t *me = NULL;

    me = (ca_omci_sip_cfg_portal_t *)ca_omci_me_new(OMCI_ME_SIP_CFG_PORTAL_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_voip_line_st_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_voip_line_st_t *me = NULL;

    me = (ca_omci_voip_line_st_t *)ca_omci_me_new(OMCI_ME_VOIP_LINE_ST_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    //me->attr.codecUse = ;
    //me->attr.serverSt = ;
    //me->attr.sessionType = ;
    //me->attr.call1PktPeriod = ;
    //me->attr.call2PktPeriod = ;
    //me->attr.call1DstAddr = ;
    //me->attr.call2DstAddr = ;
    //me->attr.lineState = ;
    //me->attr.emCallStatus = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_pptp_pots_auto_create(void)
{
    ca_omci_pptp_pots_t   *me         = NULL;
    ca_omci_mebase_t      *meBase     = NULL;
    ca_omci_onuinfo_obj_t *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_uint32_t       idx        = 0;
    ca_omci_uint8_t        pqIdx      = 0;
    ca_omci_uint16_t       pqInst     = 0;
    ca_omci_uint32_t       pqRelaPort = 0;
    ca_omci_uint16_t       slotNum    = 0;
    ca_omci_uint16_t       slotNo     = 0;
    ca_omci_uint8_t        portNum    = 0;
    ca_omci_uint8_t        portIdx    = 0;

    if (0 == onuinfo->potsNum)
    {
        OMCIDBG("no need to create pptp_pots");
        return;
    }

    slotNum = onuinfo->potsNum / 8 + ((onuinfo->potsNum % 8) ? 1 : 0);
    portNum = 8;
    for (idx = 0; idx < slotNum; idx++)
    {
        if ((idx + 1) * portNum > onuinfo->potsNum)
        {
            portNum = onuinfo->potsNum - (idx * portNum);
        }
        slotNo = (onuinfo->potsSlot & 0xff) + idx;
        for (portIdx = 0; portIdx < portNum; portIdx++)
        {
            me = (ca_omci_pptp_pots_t *)ca_omci_me_new(OMCI_ME_PPTP_POTS_CLASS_ID);
            OMCI_PARAM_IS_NULL(me);

            me->meBase.instanceId     = ((slotNo & 0xff) << 8) | (portIdx + 1);
            me->attr.admin            = CA_OMCI_UNLOCK;
            me->attr.operState        = CA_OMCI_OPER_ENABLE;
            me->attr.hookState        = PPTP_POTS_HOOK_STATE_ON; //should get from hal
            //me->attr.arc              = ;
            //me->attr.arcInterval      = ;
            //me->attr.impedance        = ;
            //me->attr.txPath           = ;
            //me->attr.rxGain           = ;
            //me->attr.txGain           = ;
            //me->attr.potsHoldoverTime = ;
            //me->attr.nominalFeedVol   = ;
            ca_omci_me_entry_add(&me->meBase);
            ca_omci_me_voip_line_st_auto_create(me->meBase.instanceId);

            for (pqIdx = 0; pqIdx < OMCI_PQ_NUM_PER_UNI; pqIdx++)
            {
                pqInst     = ((slotNo & 0xf) << 12) | (portIdx * OMCI_PQ_NUM_PER_UNI + pqIdx);
                pqRelaPort = (me->meBase.instanceId << 16) | pqIdx;
                meBase     = ca_omci_me_pri_queue_auto_create(pqInst, pqRelaPort);
                if (NULL != meBase)
                {
                    ca_omci_linked_me_add(meBase, &me->meBase);
                }
            }
        }
    }
}


void ca_omci_me_voip_cfg_auto_create(void)
{
    ca_omci_voip_cfg_t    *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();

    if (0 == onuinfo->potsNum)
    {
        OMCIDBG("no need to create voip_cfg");
        return;
    }

    me = (ca_omci_voip_cfg_t *)ca_omci_me_new(OMCI_ME_VOIP_CFG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId    = 0;
    me->attr.availSigProto   = (VOIP_CFG_AVAIL_PROTO_SIP_BIT | VOIP_CFG_AVAIL_PROTO_H248_BIT);
    me->attr.sigProtoUsed    = VOIP_CFG_USED_PROTO_SIP;
    me->attr.availVoipMethod = VOIP_CFG_CFG_METHOD_OMCI;
    me->attr.voipMethodUsed  = VOIP_CFG_USED_METHOD_OMCI;
    me->attr.voipAddrPoint   = 0xFFFF;
    me->attr.voipCfgState    = VOIP_CFG_CFG_ST_INACTIVE;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_voice_auto_create(void)
{
    ca_omci_me_pptp_pots_auto_create();
    ca_omci_me_voip_cfg_auto_create();
    ca_omci_me_sip_cfg_portal_auto_create();
}
#endif


#if DESCRIPTION("9.5 Ethernet services")
//need to seprate the eth uni type for GE FE or other
void ca_omci_me_pptp_eth_auto_create(void)
{
    ca_omci_pptp_eth_t    *me         = NULL;
    ca_omci_mebase_t      *meBase     = NULL;
    ca_omci_onuinfo_obj_t *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_uint32_t       idx        = 0;
    ca_omci_uint8_t        pqIdx      = 0;
    ca_omci_uint16_t       pqInst     = 0;
    ca_omci_uint32_t       pqRelaPort = 0;
    ca_omci_uint16_t       slotNum    = 0;
    ca_omci_uint16_t       slotNo     = 0;
    ca_omci_uint8_t        portNum    = 0;
    ca_omci_uint8_t        portIdx    = 0;
    ca_omci_uint16_t       portInst   = 0;
    ca_omci_uint8_t        mode       = 0;
    ca_omci_uint8_t        cardMode   = 0;

    if (0 == onuinfo->ethNum)
    {
        OMCIDBG("no need to create pptp_eth");
        return;
    }

    /*Both Neptune SFP and SFU have only 1 UNI and need to get the port type before init cardhold and UNI MEs*/
    portInst = ((onuinfo->ethSlot & 0xff) << 8) | 1;
    ca_omci_hal_eth_mode_get((void *)&portInst, (void *)&mode);

    if ((OMCI_PON_MODE_XGSPON == onuinfo->ponMacMode)
     && (PPTP_ETH_CFG_IND_25G_ETH_FULL == mode)
     && (OMCI_OLT_TYPE_ALCL == onuinfo->oltType)){
        mode = PPTP_ETH_CFG_IND_10G_ETH_FULL;
        ca_omci_hal_eth_mode_set((void *)&portInst, PPTP_ETH_AUTO_DETEC_CFG_10G_ONLY_FULL);
    }

    if (PPTP_ETH_CFG_IND_25G_ETH_FULL == mode) {
        cardMode = CARDHOLDER_UNIT_TYPE_25G_OPTICAL;
    }
    if (PPTP_ETH_CFG_IND_GE_FULL == mode) {
        cardMode = CARDHOLDER_UNIT_TYPE_10_100_1000_BT;
    }
    if (PPTP_ETH_CFG_IND_10G_ETH_FULL == mode) {
        cardMode = CARDHOLDER_UNIT_TYPE_10G_ETH;
    }

    if (0 < onuinfo->ethNum)
    {
        slotNum = onuinfo->ethNum / 8 + ((onuinfo->ethNum % 8) ? 1 : 0);
        portNum = 8;
        for (idx = 0; idx < slotNum; idx++)
        {
            if ((idx + 1) * portNum > onuinfo->ethNum)
            {
                portNum = onuinfo->ethNum - (idx * portNum);
            }
            slotNo = (onuinfo->ethSlot & 0xff) + idx;
            for (portIdx = 0; portIdx < portNum; portIdx++)
            {
                me = (ca_omci_pptp_eth_t *)ca_omci_me_new(OMCI_ME_PPTP_ETH_CLASS_ID);
                OMCI_PARAM_IS_NULL(me);

                me->meBase.instanceId   = ((slotNo & 0xff) << 8) | (portIdx + 1);
                me->attr.expectType     = ca_omci_onu_card_type_get(mode);
                me->attr.sensedType     = cardMode;
                me->attr.autoDetectCfg  = PPTP_ETH_AUTO_DETEC_CFG_AUTO_AUTO;
                me->attr.ethLoopbackCfg = 0;
                me->attr.admin          = CA_OMCI_UNLOCK;
                me->attr.operState      = CA_OMCI_OPER_ENABLE;
                me->attr.maxFrameSize   = 1518;
                me->attr.dteDceInd      = 0;
                //me->attr.cfgInd         = ;
                //me->attr.pauseTime      = ;
                //me->attr.brgIpInd       = ;
                //me->attr.arc            = ;
                //me->attr.arcInterval    = ;
                //me->attr.pppoeFilter    = ;
                //me->attr.powerCtrl      = ;
                ca_omci_me_entry_add(&me->meBase);

                /* create implicitly linked uni-g for every pptp me */
                ca_omci_me_uni_g_auto_create(me->meBase.instanceId, UNI_G_MNG_CAP_OMCI, 0);

                /* create implicitly linked dot1x_port_ext_pkg for every pptp me */
                ca_omci_me_dot1x_port_ext_pkg_auto_create(me->meBase.instanceId);

                for (pqIdx = 0; pqIdx < OMCI_PQ_NUM_PER_UNI; pqIdx++)
                {
                    pqInst     = ((slotNo & 0xf) << 12) | (portIdx * OMCI_PQ_NUM_PER_UNI + pqIdx);
                    pqRelaPort = (me->meBase.instanceId << 16) | pqIdx;
                    meBase     = ca_omci_me_pri_queue_auto_create(pqInst, pqRelaPort);
                    if (NULL != meBase)
                    {
                        ca_omci_linked_me_add(meBase, &me->meBase);
                    }
                }

                if ((OMCI_OLT_TYPE_ALCL == onuinfo->oltType) && (OMCI_ISP_TYPE_ATNT != onuinfo->ispType)){
                    ca_omci_me_alcl_ds_tag_oper1_auto_create(me->meBase.instanceId);
                }
            }
        }
    }
}


void ca_omci_me_veip_auto_create(void)
{
    ca_omci_veip_t        *me         = NULL;
    ca_omci_mebase_t      *meBase     = NULL;
    ca_omci_onuinfo_obj_t *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx        = 0;
    ca_omci_uint8_t        pqIdx      = 0;
    ca_omci_uint16_t       pqInst     = 0;
    ca_omci_uint32_t       pqRelaPort = 0;

    if (0 == onuinfo->veipNum)
    {
        OMCIDBG("no need to create veip");
        return;
    }

    for (idx = 0; idx < onuinfo->veipNum; idx++)
    {
        me = (ca_omci_veip_t *)ca_omci_me_new(OMCI_ME_VEIP_CLASS_ID);
        OMCI_PARAM_IS_NULL(me);

        me->meBase.instanceId    = ((onuinfo->veipSlot & 0xff) << 8) | (idx + 1);
        me->attr.admin           = CA_OMCI_UNLOCK;
        me->attr.operState       = CA_OMCI_OPER_ENABLE;
        //me->attr.interdomainName = ;
        //me->attr.tcpUdpPointer   = ;
        //me->attr.ianaAssignPort  = ;
        ca_omci_me_entry_add(&me->meBase);

        /* create implicitly linked uni-g for every pptp me */
        ca_omci_me_uni_g_auto_create(me->meBase.instanceId,
                                     UNI_G_MNG_CAP_NON_OMCI,
                                     me->meBase.instanceId);

        /* create implicitly linked BBF tr069 for every veip me */
        ca_omci_me_bbrtr69_mng_srv_auto_create(me->meBase.instanceId);

        for (pqIdx = 0; pqIdx < OMCI_PQ_NUM_PER_UNI; pqIdx++)
        {
            pqInst     = ((onuinfo->veipSlot & 0xf) << 12) | (idx * OMCI_PQ_NUM_PER_UNI + pqIdx);
            pqRelaPort = (me->meBase.instanceId << 16) | pqIdx;
            meBase     = ca_omci_me_pri_queue_auto_create(pqInst, pqRelaPort);
            if (NULL != meBase)
            {
                ca_omci_linked_me_add(meBase, &me->meBase);
            }
        }

        if ((OMCI_OLT_TYPE_ALCL == onuinfo->oltType) && (OMCI_ISP_TYPE_ATNT != onuinfo->ispType)){
            ca_omci_me_alcl_ds_tag_oper1_auto_create(me->meBase.instanceId);
        }
    }
}


void ca_omci_me_eth_auto_create(void)
{
    ca_omci_me_pptp_eth_auto_create();
    ca_omci_me_veip_auto_create();
}
#endif


#if DESCRIPTION("9.4 Layer 3 data services ")
/* This me link to mac brg port cfg data */
void ca_omci_me_brv6_pre_assi_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_brv6_pre_assi_t *me = NULL;

    me = (ca_omci_brv6_pre_assi_t *)ca_omci_me_new(OMCI_ME_BRV6_PRE_ASSI_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    //me->attr.icmpv6ErrMsgProcess = ;
    //me->attr.icmpv6InfoMsgProcess = ;
    //me->attr.routeSolicProcess = ;
    //me->attr.routeAdverProcess = ;
    //me->attr.nbSolicProcess = ;
    //me->attr.nbAdverProcess = ;
    //me->attr.redirectProcess = ;
    //me->attr.mcLsnQueryProcess = ;
    //me->attr.unknownIcmpv6Process = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_brv6_pre_assi_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_BRV6_PRE_ASSI_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_BRV6_PRE_ASSI_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_ip_host_auto_create(void)
{
    ca_omci_ip_host_t     *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx     = 0;

    if (0 == onuinfo->iphostNum)
    {
        OMCIDBG("no need to create ip_host");
        return;
    }

    for (idx = 0; idx < onuinfo->iphostNum; idx++)
    {
        me = (ca_omci_ip_host_t *)ca_omci_me_new(OMCI_ME_IP_HOST_CLASS_ID);
        OMCI_PARAM_IS_NULL(me);

        me->meBase.instanceId = idx;

        OMCI_MEMCPY(me->attr.macAddr, onuinfo->mac, OMCI_MAC_ADDR_LEN);//set to mac addr base first
        me->attr.macAddr[5] = me->attr.macAddr[5] + idx;

        // the below value should get from iphost relate interface!
        //me->attr.ipOption = ;
        //me->attr.onuId = ;
        //me->attr.ipAddr = ;
        //me->attr.mask = ;
        //me->attr.gateway = ;
        //me->attr.priDns = ;
        //me->attr.sndDns = ;
        //me->attr.curAddr = ;
        //me->attr.curMask = ;
        //me->attr.curGateway = ;
        //me->attr.curPriDns = ;
        //me->attr.cur2ndDns = ;
        //me->attr.domainName = ;
        //me->attr.hostName = ;
        //me->attr.relayAgentP = ;

        ca_omci_me_entry_add(&me->meBase);
    }
}


void ca_omci_me_ipv6_host_auto_create(void)
{
    ca_omci_ipv6_host_t   *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx     = 0;

    if ((0 == onuinfo->iphostNum) || (CA_OMCI_FALSE == onuinfo->ipv6Flag))
    {
        OMCIERR("no need to create ipv6_host");
        return;
    }

    for (idx = 0; idx < onuinfo->iphostNum; idx++)
    {
        me = (ca_omci_ipv6_host_t *)ca_omci_me_new(OMCI_ME_IPV6_HOST_CLASS_ID);
        OMCI_PARAM_IS_NULL(me);

        me->meBase.instanceId = idx;
        //me->attr.ipOption = ;
        //me->attr.macAddr = ;
        //me->attr.onuId = ;
        //me->attr.ipv6LinkLocalAddr = ;
        //me->attr.ipv6Addr = ;
        //me->attr.defRouter = ;
        //me->attr.priDns = ;
        //me->attr.sndDns = ;
        //me->attr.currAddrTable = ;
        //me->attr.currDefRouterTable = ;
        //me->attr.currDnsTable = ;
        //me->attr.duid = ;
        //me->attr.onLinkPrefix = ;
        //me->attr.currOnLinkPreTable = ;
        //me->attr.relayAgentOption = ;
        ca_omci_me_entry_add(&me->meBase);
    }
}


void ca_omci_me_layer3_auto_create(void)
{
    ca_omci_me_ip_host_auto_create();
}
#endif

#if DESCRIPTION("9.3 Layer 2 data services ")
/* some layer 2 MEs will auto create while OLT create relate ME */
/* mac brg cfg link to mac brg srv */
void ca_omci_me_mac_brg_cfg_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_mac_brg_cfg_t *me = NULL;

    me = (ca_omci_mac_brg_cfg_t *)ca_omci_me_new(OMCI_ME_MAC_BRG_CFG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_mac_brg_cfg_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_CFG_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_BRG_CFG_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


/* below mes link to mac brg port cfg */
void ca_omci_me_mac_port_des_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_mac_port_des_t *me = NULL;

    me = (ca_omci_mac_port_des_t *)ca_omci_me_new(OMCI_ME_MAC_PORT_DES_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_mac_port_des_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_DES_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_PORT_DES_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_mac_port_filter_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_mac_port_filter_t *me = NULL;

    me = (ca_omci_mac_port_filter_t *)ca_omci_me_new(OMCI_ME_MAC_PORT_FILTER_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.macFilterTbl[0].valid = CA_OMCI_TRUE;
    me->attr.macFilterTbl[0].index  = 1;
    me->attr.macFilterTbl[0].filter = 1;
    me->attr.macFilterTbl[0].macAddr[0] = 0;
    me->attr.macFilterTbl[0].macAddr[1] = 0x11;
    me->attr.macFilterTbl[0].macAddr[2] = 0x22;
    me->attr.macFilterTbl[0].macAddr[3] = 0x33;
    me->attr.macFilterTbl[0].macAddr[4] = 0x44;
    me->attr.macFilterTbl[0].macAddr[5] = 0x55;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_mac_port_filter_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_FILTER_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_PORT_FILTER_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_mac_port_brg_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_mac_port_brg_t *me = NULL;

    me = (ca_omci_mac_port_brg_t *)ca_omci_me_new(OMCI_ME_MAC_PORT_BRG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_mac_port_brg_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_BRG_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_PORT_BRG_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_mac_port_filter_pre_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_mac_port_filter_pre_t *me = NULL;

    me = (ca_omci_mac_port_filter_pre_t *)ca_omci_me_new(OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_mac_port_filter_pre_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


/* Below MES relate to dot1ag service */
/* These 2 mes may link to mac brg srv or ieee 802.1p need call it when support 802.1ag */
void ca_omci_me_dot1ag_md_level_auto_create(ca_omci_uint16_t instance, ca_omci_uint8_t l2Type)
{
    ca_omci_dot1ag_md_level_t *me = NULL;

    me = (ca_omci_dot1ag_md_level_t *)ca_omci_me_new(OMCI_ME_DOT1AG_MD_LEVEL_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.l2Type       = l2Type;
    //me->attr.catchallLevel = ;
    //me->attr.catchallMhfCreate = ;
    //me->attr.catchallSenderIdPerm = ;
    //me->attr.defMdLevelTbl = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dot1ag_md_level_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_MAC_PORT_FILTER_PRE_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_dot1ag_cfm_auto_create(ca_omci_uint16_t instance, ca_omci_uint8_t l2Type)
{
    ca_omci_dot1ag_cfm_t *me = NULL;

    me = (ca_omci_dot1ag_cfm_t *)ca_omci_me_new(OMCI_ME_DOT1AG_CFM_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.l2Type       = l2Type;
    //me->attr.mpStTbl = ;
    //me->attr.cfgErrListTbl = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dot1ag_cfm_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_DOT1AG_CFM_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_DOT1AG_CFM_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


/* These below mes link to dot1ag mep */
void ca_omci_me_dot1ag_mep_st_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_dot1ag_mep_st_t *me = NULL;

    me = (ca_omci_dot1ag_mep_st_t *)ca_omci_me_new(OMCI_ME_DOT1AG_MEP_ST_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    //me->attr.mepMacAddr = ;
    //me->attr.faultNotiGenSt = ;
    //me->attr.hiPriDefectObs = ;
    //me->attr.currDefect = ;
    //me->attr.lastRxErrCcmTbl = ;
    //me->attr.lastRxCconCcmTbl = ;
    //me->attr.oosCcmCnt = ;
    //me->attr.unexpLtrCnt = ;
    //me->attr.lbrTxCnt = ;
    //me->attr.nextLpbTransId = ;
    //me->attr.nextLinkTransId = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dot1ag_mep_st_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_DOT1AG_MEP_ST_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_DOT1AG_MEP_ST_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


void ca_omci_me_dot1ag_mep_ccm_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_dot1ag_mep_ccm_t *me = NULL;

    me = (ca_omci_dot1ag_mep_ccm_t *)ca_omci_me_new(OMCI_ME_DOT1AG_MEP_CCM_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    //me->attr.rmepDbTbl1 = ;
    //me->attr.rmepDbTbl2 = ;
    //me->attr.rmepDbTbl3 = ;
    //me->attr.rmepDbTbl4 = ;
    //me->attr.rmepDbTbl5 = ;
    //me->attr.rmepDbTbl6 = ;
    //me->attr.rmepDbTbl7 = ;
    //me->attr.rmepDbTbl8 = ;
    //me->attr.rmepDbTbl9 = ;
    //me->attr.rmepDbTbl10 = ;
    //me->attr.rmepDbTbl11 = ;
    //me->attr.rmepDbTbl12 = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dot1ag_mep_ccm_delete(ca_omci_uint16_t instance)
{
    ca_omci_mebase_t *meBase = ca_omci_me_entry_lookup(OMCI_ME_DOT1AG_MEP_ST_CLASS_ID, instance);

    ca_omci_me_entry_delete(OMCI_ME_DOT1AG_MEP_ST_CLASS_ID, instance);
    ca_omci_me_delete(meBase);
}


/* create when support this function, need to find a place to call this functions */
void ca_omci_me_dot1ag_chas_info_auto_create(void)
{
    ca_omci_dot1ag_chas_info_t *me = NULL;

    me = (ca_omci_dot1ag_chas_info_t *)ca_omci_me_new(OMCI_ME_DOT1AG_CHAS_INFO_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    //me->attr.chasIdLen = ;
    //me->attr.chasIdSubType = ;
    //me->attr.chasId1 = ;
    //me->attr.chasId2 = ;
    //me->attr.mngAddrDomainLen = ;
    //me->attr.mngAddrDomain1 = ;
    //me->attr.mngAddrDomain2 = ;
    //me->attr.mngAddrLen = ;
    //me->attr.mngAddr1 = ;
    //me->attr.mngAddr2 = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dot1x_port_ext_pkg_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_dot1x_port_ext_pkg_t *me = NULL;

    me = (ca_omci_dot1x_port_ext_pkg_t *)ca_omci_me_new(OMCI_ME_DOT1X_PORT_EXT_PKG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = instance;
    me->attr.actReg       = 3;

    ca_omci_me_entry_add(&me->meBase);
}
#endif


#if DESCRIPTION("9.2 ANI management, traffic management ")
void ca_omci_me_ani_g_auto_create(void)
{
    ca_omci_ani_g_t       *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx     = 0;
    ca_omci_uint8_t        ponPort = 0;
    ca_omci_hal_pon_cap_t  ponCap;

    for (idx = 0; idx < onuinfo->ponNum; idx++)
    {
        me = (ca_omci_ani_g_t *)ca_omci_me_new(OMCI_ME_ANI_G_CLASS_ID);
        OMCI_PARAM_IS_NULL(me);
        OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
        ponPort = idx + 1;

        if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
        {
            OMCIERR("can't get cap for pon");
            ca_omci_me_delete(&me->meBase);
            return;
        }

        me->meBase.instanceId    = ((onuinfo->ponSlot & 0xff) << 8) | (idx + 1);
        me->attr.srInd           = CA_OMCI_TRUE;
        me->attr.totalTcontNum   = ponCap.maxTcNum;
        me->attr.gemBlockLen     = OMCI_ANIG_GEM_BLOCK_LEN_DEF;
        me->attr.pbDbaReport     = ponCap.reportMode;
        me->attr.sfThres         = 5;
        me->attr.sdThres         = 9;
        me->attr.arc             = 0;
        me->attr.arcInterval     = 0;
        me->attr.opticalLevel    = 0xd80c;
        me->attr.lowOptThres     = 0xff;
        me->attr.upOptThres      = 0xff;
        me->attr.onuResTime      = onuinfo->todRspTime;
        me->attr.txOptLevel      = 0xce3;
        me->attr.lowTxPowerThres = 0x81;
        me->attr.upTxPowerThres  = 0x81;

        ca_omci_me_entry_add(&me->meBase);
    }
}


ca_omci_mebase_t *ca_omci_me_pri_queue_auto_create(
    ca_omci_uint16_t instance,
    ca_omci_uint32_t relaPort)
{
    ca_omci_pri_queue_t  *me = NULL;

    me = (ca_omci_pri_queue_t *)ca_omci_me_new(OMCI_ME_PRI_QUEUE_CLASS_ID);
    if (NULL == me)
    {
        return NULL;
    }

    me->meBase.instanceId         = instance;
    me->attr.queueCfgOpt          = 0;
    me->attr.maxQueueSize         = 0x2fd0;
    me->attr.allocQueueSize       = 0x100; //TODO add get function in hal
    //me->attr.disCntResetItv       = ;
    //me->attr.buffOverflowDisThres = ;
    me->attr.relatePort           = relaPort;
    //me->attr.trafSchedPoint       = trafSchedPoint;
    me->attr.weight               = 1;
    //me->attr.bpOper               = ;
    //me->attr.bpTime               = ;
    //me->attr.bpOccurQueueThres    = ;
    //me->attr.bpClearQueueThres    = ;
    //me->attr.pktDropQueueThres    = ;
    //me->attr.pkgDropMaxp          = ;
    //me->attr.queueDropWQ          = ;
    //me->attr.colorMark            = ;

    ca_omci_me_entry_add(&me->meBase);
    return &me->meBase;
}


ca_omci_mebase_t *ca_omci_me_traffic_scheg_auto_create(ca_omci_uint16_t instance)
{
    ca_omci_traffic_scheg_t *me = NULL;

    me = (ca_omci_traffic_scheg_t *)ca_omci_me_new(OMCI_ME_TRAFFIC_SCHEG_CLASS_ID);
    if (NULL == me)
    {
        return NULL;
    }

    me->meBase.instanceId   = instance;
    me->attr.tcontPoint     = instance;
    me->attr.policy         = CA_OMCI_POLICY_WRR;
    me->attr.priWeight      = 0;

    ca_omci_me_entry_add(&me->meBase);
    return &me->meBase;
}

void ca_omci_me_tcont_auto_create(void)
{
    ca_omci_tcont_t       *me         = NULL;
    ca_omci_mebase_t      *meBase     = NULL;
    ca_omci_mebase_t      *tcScheMe   = NULL;
    ca_omci_onuinfo_obj_t *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx        = 0;
    ca_omci_uint8_t        tcIdx      = 0;
    ca_omci_uint8_t        pqIdx      = 0;
    ca_omci_uint16_t       pqInst     = 0;
    ca_omci_uint32_t       pqRelaPort = 0;
    ca_omci_uint8_t        ponPort    = 0;
    ca_omci_hal_pon_cap_t  ponCap;


    for (idx = 0; idx < onuinfo->ponNum; idx++)
    {
        OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
        ponPort = idx + 1;

        if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
        {
            OMCIERR("can't get cap for pon");
            ca_omci_me_delete(&me->meBase);
            return;
        }

        for (tcIdx = 0; tcIdx < ponCap.maxTcNum; tcIdx++)
        {
            me = (ca_omci_tcont_t *)ca_omci_me_new(OMCI_ME_TCONT_CLASS_ID);
            OMCI_PARAM_IS_NULL(me);

            me->meBase.instanceId = ((onuinfo->ponSlot & 0xff) << 8) | tcIdx;
            me->attr.allocId      = 0xffff;
            me->attr.policy       = CA_OMCI_POLICY_SP;
            me->attr.deprecated   = 1;

            if (OMCI_PON_MODE_GPON == onuinfo->ponMacMode)
            {
                me->attr.allocId = 0xff;
            }

            ca_omci_me_entry_add(&me->meBase);

            for (pqIdx = 0; pqIdx < ponCap.maxQueuePerTc; pqIdx++)
            {
                pqInst     = 0x8000 | (tcIdx * ponCap.maxQueuePerTc + pqIdx);
                pqRelaPort = (me->meBase.instanceId << 16) | pqIdx;
                meBase     = ca_omci_me_pri_queue_auto_create(pqInst, pqRelaPort);
                if (NULL != meBase)
                {
                    ca_omci_linked_me_add(meBase, &me->meBase);
                }
            }

            tcScheMe = ca_omci_me_traffic_scheg_auto_create(me->meBase.instanceId);
            if (NULL != tcScheMe)
            {
                ca_omci_linked_me_add(tcScheMe, &me->meBase);
            }
        }
    }

    onuinfo->maxQueuePerTc = ponCap.maxQueuePerTc;
}


void ca_omci_me_ani_auto_create(void)
{
    ca_omci_me_ani_g_auto_create();
    ca_omci_me_tcont_auto_create();
}
#endif


#if DESCRIPTION("9.1 Equipment management")
void ca_omci_me_onu_g_auto_create(void)
{
    ca_omci_onu_g_t       *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        alclVer[OMCI_VERSION_LEN] = ALCL_ONUG_VERSION_DEF;

    me = (ca_omci_onu_g_t *)ca_omci_me_new(OMCI_ME_ONU_G_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    OMCI_MEMCPY(me->attr.vendorId, onuinfo->vendorId, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(me->attr.version,  alclVer,           OMCI_VERSION_LEN);
    OMCI_MEMCPY(me->attr.sn,       onuinfo->sn,       OMCI_SN_LEN);
    OMCI_MEMCPY(me->attr.loid,     onuinfo->loid,     OMCI_LOID_LEN);
    OMCI_MEMCPY(me->attr.loidPwd,  onuinfo->passwd,   OMCI_LOID_PW_LEN);
    me->attr.trafficMngOption = ONU_G_TRAF_OP_PRI_RATE;
    me->attr.batteryBackup    = onuinfo->batteryBackup;
    me->attr.onuSurvTime      = 200;
    me->attr.creStatus        = ONU_G_CRE_ST_INIT;
    me->attr.operState        = CA_OMCI_OPER_DISABLE;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_onu2_g_auto_create(void)
{
    ca_omci_onu2_g_t      *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        ponPort = 1;
    ca_omci_hal_pon_cap_t  ponCap;

    me = (ca_omci_onu2_g_t *)ca_omci_me_new(OMCI_ME_ONU2_G_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);
    OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));

    if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
    {
        OMCIERR("can't get cap for pon");
        ca_omci_me_delete(&me->meBase);
        return;
    }

    me->meBase.instanceId = 0;
    OMCI_MEMCPY(me->attr.eqId, onuinfo->eqId, OMCI_EQID_LEN);
    me->attr.omccVer              = onuinfo->omccVersion;
    me->attr.vendorProductCode    = 0; //TBD may be diff for diff olt
    me->attr.securityCapab        = OMCI_ENCRYPTION_AES_128;
    me->attr.securityMode         = OMCI_ENCRYPTION_AES_128;
    me->attr.totalPqNum           = ponCap.maxQueueNum;
    me->attr.totalTrafficSchedNum = ponCap.maxTcNum;
    me->attr.totalGemNum          = ponCap.maxGemNum;
    me->attr.sysUpTime            = 0;//should use a function , to add later
    me->attr.connectCap           = 0x7F;
    me->attr.qosCfgFlex           = 0;
    me->attr.pqScaleFactor        = 1;
    me->attr.deprecated           = 1;

    if ((32 == onuinfo->maxQueuePerTc) && (OMCI_OLT_TYPE_ALCL == onuinfo->oltType)) {
        me->attr.qosCfgFlex = 0x16;
    }

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_onu3_g_auto_create(void)
{
    ca_omci_onu3_g_t *me = NULL;

    me = (ca_omci_onu3_g_t *)ca_omci_me_new(OMCI_ME_ONU3_G_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId      = 0;
    me->attr.flashMemPmVal     = 0; //the more, the older
    me->attr.lastResetReason   = ONU3_G_RESTART_REASON_SW_INIT;
    me->attr.stSnapshotTotal   = 10;
    me->attr.stSnapshotValid   = 2;
    me->attr.snapAction        = 0;
    me->attr.nextStSnapshotIdx = 0;
    me->attr.resetAction       = 0;
    OMCI_STRCPY((char *)me->attr.stSnapshotRcTbl,  "This is a test");
    OMCI_STRCPY((char *)me->attr.stSnapshotRecent, "This is a test");

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_onu_data_auto_create(void)
{
    ca_omci_onu_data_t *me = NULL;

    me = (ca_omci_onu_data_t *)ca_omci_me_new(OMCI_ME_ONU_DATA_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    me->attr.mibDataSync  = 0;

    ca_omci_me_entry_add(&me->meBase);
}

extern ca_omci_int32_t ca_omci_mng_me_sw_img_init(ca_omci_soft_image_t *swImage);
extern void ca_omci_mgr_img_upg_state_update(
    ca_omci_soft_image_t *swImg,
    ca_omci_soft_image_t *another_swImg);

void ca_omci_me_soft_image_auto_create(void)
{
    ca_omci_soft_image_t *me      = NULL;
    ca_omci_soft_image_t *swImage = NULL;

    me      = (ca_omci_soft_image_t *)ca_omci_me_new(OMCI_ME_SOFT_IMAGE_CLASS_ID);
    swImage = (ca_omci_soft_image_t *)ca_omci_me_new(OMCI_ME_SOFT_IMAGE_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(swImage);

    // there are 2 software image for each onu
    me->meBase.instanceId = 0x0000;

    OMCI_STRCPY((char *)me->attr.version, "V1.0.0");//todo need to get version
    me->attr.isCommit = 1;
    me->attr.isActive = 1;
    me->attr.isValid  = 1;
    //me->attr.productCode = ; // todo
    //me->attr.imageHash = ;   // todo
    ca_omci_me_entry_add(&me->meBase);


    swImage->meBase.instanceId = 0x0001;

    OMCI_STRCPY((char *)swImage->attr.version, "V1.0.0");//todo need to get version
    swImage->attr.isCommit = 0;
    swImage->attr.isActive = 0;
    swImage->attr.isValid  = 1;
    //swImage->attr.productCode = ; // todo
    //swImage->attr.imageHash = ;   // todo
    ca_omci_me_entry_add(&swImage->meBase);

#ifdef CA_OMCI_USE_PLAT_API
    ca_omci_mng_me_sw_img_init(me);
    ca_omci_mng_me_sw_img_init(swImage);
#endif

    ca_omci_mgr_img_upg_state_update(me, swImage);
}


/* all card holder me need to relate to pon port ! need to add !*/
void ca_omci_create_pots_cardholder(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_cardholder_t *me  = NULL;
    ca_omci_uint8_t       idx = 0;

    me = (ca_omci_cardholder_t *)ca_omci_me_new(OMCI_ME_CARDHOLDER_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId   = 0x100 | (slotNo & 0xff);
    me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_POTS;
    me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_POTS; //need a function to get support xgpon

    for (idx = 0; idx < OMCI_EQID_LEN; idx++)
    {
        me->attr.actualEqId[idx] = OMCI_STR_SPACE;
        me->attr.expectEqId[idx] = OMCI_STR_SPACE;
    }

    ca_omci_me_entry_add(&me->meBase);
}

void ca_omci_create_veip_cardholder(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_cardholder_t *me      = NULL;
    ca_omci_uint8_t       idx     = 0;

    me = (ca_omci_cardholder_t *)ca_omci_me_new(OMCI_ME_CARDHOLDER_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId   = 0x100 | (slotNo & 0xff);
    me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_VEIP;
    me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_VEIP; //need a function to get support xgpon

    for (idx = 0; idx < OMCI_EQID_LEN; idx++)
    {
        me->attr.actualEqId[idx] = OMCI_STR_SPACE;
        me->attr.expectEqId[idx] = OMCI_STR_SPACE;
    }

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_eth_cardholder(
    ca_omci_uint16_t slotNo,
    ca_omci_uint8_t  portNum,
    ca_omci_uint8_t  type)
{
    ca_omci_cardholder_t *me  = NULL;
    ca_omci_uint8_t       idx = 0;

    me = (ca_omci_cardholder_t *)ca_omci_me_new(OMCI_ME_CARDHOLDER_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId   = 0x100 | (slotNo & 0xff);
    me->attr.actualUnitType = type;
    me->attr.exceptUnitType = type;

    for (idx = 0; idx < OMCI_EQID_LEN; idx++)
    {
        me->attr.actualEqId[idx] = OMCI_STR_SPACE;
        me->attr.expectEqId[idx] = OMCI_STR_SPACE;
    }

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_pon_cardholder(ca_omci_uint16_t slotNo)
{
    ca_omci_cardholder_t  *me      = NULL;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx     = 0;

    me = (ca_omci_cardholder_t *)ca_omci_me_new(OMCI_ME_CARDHOLDER_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId   = 0x100 | (slotNo & 0xff);
    me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_1244;
    me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_1244; //need a function to get support xgpon

    if (OMCI_PON_MODE_XGPON1 == onuinfo->ponMacMode)
    {
        me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_XGPON_10G_2488;
        me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_XGPON_10G_2488;
    }
    if ((OMCI_PON_MODE_XGSPON == onuinfo->ponMacMode)
     || (OMCI_PON_MODE_NGPON2 == onuinfo->ponMacMode))
    {
        me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_XGPON_10G_10;
        me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_XGPON_10G_10;
    }

    if (OMCI_PON_MODE_25G10G == onuinfo->ponMacMode)
    {
        me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_25GPON_25G_10;
        me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_25GPON_25G_10;
    }

    if (OMCI_PON_MODE_25GPON == onuinfo->ponMacMode)
    {
        me->attr.actualUnitType = CARDHOLDER_UNIT_TYPE_25GPON_25G_25;
        me->attr.exceptUnitType = CARDHOLDER_UNIT_TYPE_25GPON_25G_25;
    }

    for (idx = 0; idx < OMCI_EQID_LEN; idx++)
    {
        me->attr.actualEqId[idx] = OMCI_STR_SPACE;
        me->attr.expectEqId[idx] = OMCI_STR_SPACE;
    }

    /*no need for now, maybe to add later*/
    //me->attr.exceptPortNum  = ;
    //me->attr.protecProfPoint = ;
    //me->attr.invokeProtectSwitch = ;
    //me->attr.arc = ;
    //me->attr.arcInterval = ;

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_pots_circuit_pack(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_circuit_pack_t *me      = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t         ponPort = 1;
    ca_omci_hal_pon_cap_t   ponCap;
    ca_omci_uint8_t         version[OMCI_VERSION_LEN] = ALCL_ONUG_VERSION_DEF;

    OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
    if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
    {
        OMCIERR("can't get cap for pon");
        return;
    }

    me = (ca_omci_circuit_pack_t *)ca_omci_me_new(OMCI_ME_CIRCUIT_PACK_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId        = 0x100 | (slotNo & 0xff);
    me->attr.type                = CARDHOLDER_UNIT_TYPE_POTS;
    me->attr.portNum             = portNum;
    me->attr.admin               = 0;
    me->attr.tcontBufNum         = ponCap.maxTcNum;
    me->attr.priQueueNUm         = 8;
    me->attr.trafficSchedulerNum = 8;
    OMCI_MEMCPY(me->attr.sn,       onuinfo->sn,       OMCI_SN_LEN);
    OMCI_MEMCPY(me->attr.vendorId, onuinfo->vendorId, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(me->attr.version,  version,           OMCI_VERSION_LEN);
    OMCI_MEMCPY(me->attr.eqId,     onuinfo->eqId,     OMCI_EQID_LEN);

    ca_omci_me_entry_add(&me->meBase);
}

void ca_omci_create_veip_circuit_pack(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_circuit_pack_t *me      = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t         ponPort = 1;
    ca_omci_hal_pon_cap_t   ponCap;
    ca_omci_uint8_t         version[OMCI_VERSION_LEN] = ALCL_ONUG_VERSION_DEF;

    OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
    if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
    {
        OMCIERR("can't get cap for pon");
        return;
    }

    me = (ca_omci_circuit_pack_t *)ca_omci_me_new(OMCI_ME_CIRCUIT_PACK_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId        = 0x100 | (slotNo & 0xff);
    me->attr.type                = CARDHOLDER_UNIT_TYPE_VEIP;
    me->attr.portNum             = portNum;
    me->attr.admin               = 0;
    me->attr.tcontBufNum         = ponCap.maxTcNum;
    me->attr.priQueueNUm         = 8;
    me->attr.trafficSchedulerNum = 8;
    OMCI_MEMCPY(me->attr.sn,       onuinfo->sn,       OMCI_SN_LEN);
    OMCI_MEMCPY(me->attr.vendorId, onuinfo->vendorId, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(me->attr.version,  version,           OMCI_VERSION_LEN);
    OMCI_MEMCPY(me->attr.eqId,     onuinfo->eqId,     OMCI_EQID_LEN);

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_eth_circuit_pack(
    ca_omci_uint16_t slotNo,
    ca_omci_uint8_t  portNum,
    ca_omci_uint8_t  type)
{
    ca_omci_circuit_pack_t *me      = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t         ponPort = 1;
    ca_omci_hal_pon_cap_t   ponCap;
    ca_omci_uint8_t         version[OMCI_VERSION_LEN] = ALCL_ONUG_VERSION_DEF;

    OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
    if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
    {
        OMCIERR("can't get cap for pon");
        return;
    }

    me = (ca_omci_circuit_pack_t *)ca_omci_me_new(OMCI_ME_CIRCUIT_PACK_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId        = 0x100 | (slotNo & 0xff);
    me->attr.type                = type;
    me->attr.portNum             = portNum;
    me->attr.admin               = 0;
    me->attr.tcontBufNum         = ponCap.maxTcNum;
    me->attr.priQueueNUm         = 8;
    me->attr.trafficSchedulerNum = 8;
    OMCI_MEMCPY(me->attr.sn,       onuinfo->sn,       OMCI_SN_LEN);
    OMCI_MEMCPY(me->attr.vendorId, onuinfo->vendorId, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(me->attr.version,  version,           OMCI_VERSION_LEN);
    OMCI_MEMCPY(me->attr.eqId,     onuinfo->eqId,     OMCI_EQID_LEN);

    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_pon_circuit_pack(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_circuit_pack_t *me      = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t         ponPort = 1;
    ca_omci_hal_pon_cap_t   ponCap;
    ca_omci_uint8_t         version[OMCI_VERSION_LEN] = ALCL_ONUG_VERSION_DEF;

    OMCI_MEMSET(&ponCap, 0, sizeof(ponCap));
    if (CA_OMCI_OK != ca_omci_hal_pon_cap_get(&ponPort, &ponCap))
    {
        OMCIERR("can't get cap for pon");
        return;
    }

    me = (ca_omci_circuit_pack_t *)ca_omci_me_new(OMCI_ME_CIRCUIT_PACK_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId        = 0x100 | (slotNo & 0xff);
    me->attr.type                = CARDHOLDER_UNIT_TYPE_GPON_ASYM_2488_1244;
    me->attr.portNum             = portNum;
    me->attr.admin               = 0;
    me->attr.tcontBufNum         = ponCap.maxTcNum;
    me->attr.priQueueNUm         = 8;
    me->attr.trafficSchedulerNum = 8;
    OMCI_MEMCPY(me->attr.sn,       onuinfo->sn,       OMCI_SN_LEN);
    OMCI_MEMCPY(me->attr.vendorId, onuinfo->vendorId, OMCI_VENDOR_ID_LEN);
    OMCI_MEMCPY(me->attr.version,  version,           OMCI_VERSION_LEN);
    OMCI_MEMCPY(me->attr.eqId,     onuinfo->eqId,     OMCI_EQID_LEN);

    if (OMCI_PON_MODE_XGPON1 == onuinfo->ponMacMode)
    {
        me->attr.type = CARDHOLDER_UNIT_TYPE_XGPON_10G_2488;
    }
    if ((OMCI_PON_MODE_XGSPON == onuinfo->ponMacMode)
     || (OMCI_PON_MODE_NGPON2 == onuinfo->ponMacMode))
    {
        me->attr.type = CARDHOLDER_UNIT_TYPE_XGPON_10G_10;
    }
    if (OMCI_PON_MODE_25G10G == onuinfo->ponMacMode)
    {
        me->attr.type = CARDHOLDER_UNIT_TYPE_25GPON_25G_10;
    }
    if (OMCI_PON_MODE_25GPON == onuinfo->ponMacMode)
    {
        me->attr.type = CARDHOLDER_UNIT_TYPE_25GPON_25G_25;
    }

    //me->attr.operState = ;
    //me->attr.brdIpInd = ;
    //me->attr.cardCfg = ;
    //me->attr.powerShedOverride = ;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_create_port_map_pkg(ca_omci_uint16_t slotNo, ca_omci_uint8_t portNum)
{
    ca_omci_port_map_pkg_t *me = NULL;

    me = (ca_omci_port_map_pkg_t *)ca_omci_me_new(OMCI_ME_PORT_MAP_PKG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0x100 | (slotNo & 0xff);
    me->attr.maxPorts     = portNum;
    //me->attr.portList1 = ;
    //me->attr.portList2 = ;
    //me->attr.portList3 = ;
    //me->attr.portList4 = ;
    //me->attr.portList5 = ;
    //me->attr.portList6 = ;
    //me->attr.portList7 = ;
    //me->attr.portList8 = ;
    //me->attr.combPortTable = ;
    ca_omci_me_entry_add(&me->meBase);
}


static ca_omci_uint8_t ca_omci_onu_card_type_get(ca_omci_uint8_t mode)
{
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();

    if (PPTP_ETH_CFG_IND_GE_FULL == mode){
        return CARDHOLDER_UNIT_TYPE_10_100_1000_BT;
    }

    if (PPTP_ETH_CFG_IND_10G_ETH_FULL == mode){
        return CARDHOLDER_UNIT_TYPE_10G_ETH;
    }

    if (PPTP_ETH_CFG_IND_25G_ETH_FULL == mode){
        if (OMCI_OLT_TYPE_ALCL == onuinfo->oltType){
            return CARDHOLDER_UNIT_TYPE_GB_ETH;
        }
        return CARDHOLDER_UNIT_TYPE_25G_OPTICAL;
    }

    return CARDHOLDER_UNIT_TYPE_25G_OPTICAL;
}

void ca_omci_onu_card_create(void)
{
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_omci_uint16_t       slotNum = 0;
    ca_omci_uint16_t       slotNo  = 0;
    ca_omci_uint8_t        portNum = 0;
    ca_omci_uint32_t       idx     = 0;
    ca_omci_uint16_t       inst    = 0;
    ca_omci_uint8_t        mode    = 0;
    ca_omci_uint8_t        type    = 0;

    //create pon card holder, for now there will be 2 pon max for onu
    if (0 < onuinfo->ponNum)
    {
        ca_omci_create_pon_cardholder(onuinfo->ponSlot);
        ca_omci_create_pon_circuit_pack(onuinfo->ponSlot, onuinfo->ponNum);
        ca_omci_create_port_map_pkg(onuinfo->ponSlot, onuinfo->ponNum);
    }

    //create eth card holder, for some olt every 8 port need a cardhold and circuit pack
    if (0 < onuinfo->ethNum)
    {
        /*Both Neptune SFP and SFU have only 1 UNI and need to get the port type before init cardhold and UNI MEs*/
        inst = ((onuinfo->ethSlot & 0xff) << 8) | 1;
        ca_omci_hal_eth_mode_get((void *)&inst, (void *)&mode);

        /*By default now ONU will auto detect UNI link state for UNI, add a workaround here to always report 25G UNI cardhold now*/
        /*There is a alarm even ONU report the card type correctly*/
        if (0 == OMCI_STRCMP(onuinfo->version, "V0.9")){
            mode = PPTP_ETH_CFG_IND_25G_ETH_FULL;
        }

        if ((OMCI_PON_MODE_XGSPON == onuinfo->ponMacMode)
         && (OMCI_OLT_TYPE_ALCL == onuinfo->oltType)){
            mode = PPTP_ETH_CFG_IND_10G_ETH_FULL;
        }

        type = ca_omci_onu_card_type_get(mode);

        //for some olt, 8 port to 1 slot
        slotNum = onuinfo->ethNum / 8 + ((onuinfo->ethNum % 8) ? 1 : 0);
        portNum = 8;
        for (idx = 0; idx < slotNum; idx++)
        {
            if ((idx + 1) * portNum > onuinfo->ethNum)
            {
                portNum = onuinfo->ethNum - (idx * portNum);
            }
            slotNo = (onuinfo->ethSlot & 0xff) + idx;
            ca_omci_create_eth_cardholder(slotNo, portNum, type);
            ca_omci_create_eth_circuit_pack(slotNo, portNum, type);
            ca_omci_create_port_map_pkg(slotNo, portNum);
        }
    }

    //create veip card holder, for CTC define veip only need one
    // if veip used independently of a cardholder and circuit pack
    // no need to create cardholder and circuit path
    if ((0 < onuinfo->veipNum) && (CA_OMCI_FALSE == onuinfo->veipIndepend))
    {
        ca_omci_create_veip_cardholder(onuinfo->veipSlot, portNum);
        ca_omci_create_veip_circuit_pack(onuinfo->veipSlot, onuinfo->veipNum);
        ca_omci_create_port_map_pkg(onuinfo->veipSlot, onuinfo->veipNum);
    }

    //create voip card holder
    if (0 < onuinfo->potsNum)
    {
        //for some olt, 8 port to 1 slot
        slotNum = onuinfo->potsNum / 8 + ((onuinfo->potsNum % 8) ? 1 : 0);
        portNum = 8;
        for (idx = 0; idx < slotNum; idx++)
        {
            if ((idx + 1) * portNum > onuinfo->potsNum)
            {
                portNum = onuinfo->potsNum - (idx * portNum);
            }
            slotNo = (onuinfo->potsSlot & 0xff) + idx;
            ca_omci_create_pots_cardholder(slotNo, portNum);
            ca_omci_create_pots_circuit_pack(slotNo, portNum);
            ca_omci_create_port_map_pkg(slotNo, portNum);
        }
    }
}


void ca_omci_me_onu_power_shed_auto_create(void)
{
    ca_omci_onu_power_shed_t *me      = NULL;
    ca_omci_onuinfo_obj_t    *onuinfo = ca_omci_onuinfo_get();

    if (CA_OMCI_FALSE == onuinfo->batteryBackup)
    {
        OMCIDBG("no need to create onu_power_shed");
        return;
    }

    me = (ca_omci_onu_power_shed_t *)ca_omci_me_new(OMCI_ME_ONU_POWER_SHED_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    //me->attr.restorePowerTimeResetInterval = ;
    //me->attr.dataShedInterval = ;
    //me->attr.voideShedInterval = ;
    //me->attr.vedioOverlayShedInterval = ;
    //me->attr.dslShedInterval = ;
    //me->attr.atmShedInterval = ;
    //me->attr.cesShedInterval = ;
    //me->attr.frameShedInterval = ;
    //me->attr.sdhShedInterval = ;
    //me->attr.shedStatus = ;

    ca_omci_me_entry_add(&me->meBase);
}


/* for ANI-G use only now */
void ca_omci_me_protect_data_auto_create(void)
{
    ca_omci_protect_data_t *me      = NULL;
    ca_omci_onuinfo_obj_t  *onuinfo = ca_omci_onuinfo_get();

    if (onuinfo->ponNum != 2)
    {
        OMCIDBG("no need to create protect_data pon port %d", onuinfo->ponNum);
        return;
    }

    me = (ca_omci_protect_data_t *)ca_omci_me_new(OMCI_ME_PROTECT_DATA_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId      = 0;
    me->attr.workAnigPoint     = ((onuinfo->ponSlot & 0xff) << 8) | 1;// default active pon 1
    me->attr.protAnigPoint     = ((onuinfo->ponSlot & 0xff) << 8) | 2;;
    me->attr.protType          = PROTECT_DATA_PROT_TYPE_PLUS;
    me->attr.revertive_ind     = 1;
    me->attr.wait_restore_time = 3;
    me->attr.sw_guard_time     = 3000;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_onu_remote_debug_auto_create(void)
{
    ca_omci_onu_remote_debug_t *me      = NULL;
    ca_omci_onuinfo_obj_t      *onuinfo = ca_omci_onuinfo_get();

    if (CA_OMCI_FALSE == onuinfo->remoteDbg)
    {
        OMCIDBG("no need to create onu_remote_debug");
        return;
    }

    me = (ca_omci_onu_remote_debug_t *)ca_omci_me_new(OMCI_ME_ONU_REMOTE_DEBUG_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId = 0;
    me->attr.cmdFormat    = 0;
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_dyn_power_ctl_auto_create(void)
{
    ca_omci_dyn_power_ctl_t *me      = NULL;
    ca_omci_onuinfo_obj_t   *onuinfo = ca_omci_onuinfo_get();

    if (CA_OMCI_FALSE == onuinfo->powerCsvMode)
    {
        OMCIDBG("no need to create dyn_power_ctl");
        return;
    }

    me = (ca_omci_dyn_power_ctl_t *)ca_omci_me_new(OMCI_ME_DYN_POWER_CTL_CLASS_ID);
    OMCI_PARAM_IS_NULL(me);

    me->meBase.instanceId     = 0;
    me->attr.powerReduMngCap  = 4;
    me->attr.missConsBurstThr = 4;

    /* measured in units of 125-microsecond frames, should hook API to get the actual value, 0 means no delay, so just report hard value now */
    me->attr.itransinit      = 160; /*20ms,  CA Timer min length is 10ms*/
    me->attr.itxinit         = 80;  /*10ms,  CA Timer min length is 10ms*/
    ca_omci_me_entry_add(&me->meBase);
}


void ca_omci_me_eq_mng_auto_create(void)
{
    ca_omci_me_onu_g_auto_create();
    ca_omci_me_onu2_g_auto_create();
    ca_omci_me_onu3_g_auto_create();
    ca_omci_me_onu_data_auto_create();
    ca_omci_me_soft_image_auto_create();
    ca_omci_onu_card_create();
    ca_omci_me_onu_power_shed_auto_create();
    ca_omci_me_protect_data_auto_create();
    ca_omci_me_onu_remote_debug_auto_create();
    ca_omci_me_dyn_power_ctl_auto_create();
}
#endif


ca_omci_st_e ca_omci_me_onu_auto_create_init(void)
{
    ca_omci_me_eq_mng_auto_create();
    ca_omci_me_ani_auto_create();
    ca_omci_me_eth_auto_create();
    ca_omci_me_layer3_auto_create();
    ca_omci_me_gen_auto_create();
    ca_omci_me_voice_auto_create();
    ca_omci_me_enhance_sec_ctl_auto_create();
    return CA_OMCI_OK;
}

