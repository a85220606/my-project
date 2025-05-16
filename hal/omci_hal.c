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
#include <math.h>
#include <shell/shell.h>
#include <shell/shell_uart.h>
#include "omci_common.h"
#include "omci_mq.h"
#include "omci_hal.h"
#include "omci_mic.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_mng_onuinfo.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_me.h"
#include "omci_mng_am.h"
#include "omci_codec_pkt.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_fec_pm.h"
#include "omci_me_eth_frm_pm_us.h"
#include "omci_me_eth_pm_his.h"
#include "omci_me_eth_frm_pm_ds.h"
#include "omci_me_gpn_ctp_pm.h"
#include "omci_me_xgpon_tc_pm.h"
#include "omci_me_xgpon_ds_mng_pm.h"
#include "omci_me_xgpon_us_mng_pm.h"
#include "omci_me_enhance_fec_pm.h"
#include "omci_me_enhance_tc_pm.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_ext_vlan_tag_op.h"
#include "omci_me_pri_queue.h"
#include "omci_hal.h"
#include "omci_pm.h"
#include "omci_cfg.h"
#include "osal_common.h"
#include "gpon.h"
#include "xgpon.h"
#include "ca_event.h"
#include "classifier.h"
#include "pkt_tx.h"
#include "pkt_rx.h"
#include "eth_port.h"
#include "port.h"
#include "xgpon_aes.h"
#include "queue.h"
#include "special_packet.h"
#include "ptp.h"
#include "omci_me_alcl_ds_tag_oper1.h"
#include "sys.h"
#include "omci_me_dyn_power_ctl.h"
#include "vlan.h"


#define NOT_USED(x) ((void)(x))
#define OMCI_AAL_LPORT_ETH_WAN   0x07 //can't use AAL_LPORT_ETH_WAN
#define OMCI_AAL_LPORT_ETH_NI3   0x03 //can't use AAL_LPORT_ETH_NI3
#define OMCI_AAL_LPORT_ETH_NI6   0x06 //can't use AAL_LPORT_ETH_NI6
#define OMCI_AAL_LPORT_CPU0      0x10 //can't use AAL_LPORT_CPU_0
#define OMCI_AAL_LPORT_L3_LAN    0x19 //can't use AAL_LPORT_L3_LAN
#define OMCI_HAL_WAN_PORT        CA_PORT_ID(CA_PORT_TYPE_GPON,     OMCI_AAL_LPORT_ETH_WAN)
#define OMCI_HAL_UNI_PORT        CA_PORT_ID(CA_PORT_TYPE_ETHERNET, OMCI_AAL_LPORT_ETH_NI6)
#define OMCI_HAL_VEIP_SRC_PORT   CA_PORT_ID(CA_PORT_TYPE_ETHERNET, OMCI_AAL_LPORT_ETH_NI6)
#define OMCI_HAL_CPU_PORT        CA_PORT_ID(CA_PORT_TYPE_CPU,      OMCI_AAL_LPORT_CPU0)
#define QOS_FOR_WAN              0 //AAL_L2_QOS_MAP_PROFILE_FOR_WAN
#define QOS_FOR_LAN              1 //AAL_L2_QOS_MAP_PROFILE_FOR_LAN
#define OPM_DEVICE_ADDR_1        0xA2
#define OPM_DEVICE_OFFSET        0x60
#define PLOAMD_REBOOT_ONU_LEN    48
#define PLOAMD_BROADCAST_ONU_ID  0x3ff
#define PLOAMD_REBOOT_CFG_VAL    3


#define HAL_CHECK_TCL\
     do {\
            ca_omci_onuinfo_obj_t *pInfo = ca_omci_onuinfo_get();\
            if (3 == pInfo->onuMode)\
            {\
                return CA_OMCI_OK;\
            }\
        }while(0)


static ca_omci_hal_gem_flow_map_t sg_halGemMap[OMCI_HAL_MAX_GEM_NUM];
static ca_omci_hal_rule_map_t     sg_halRuleMap[OMCI_HAL_MAX_FLOW_NUM];
static ca_omci_hal_rule_map_t     sg_halFilterMap[OMCI_HAL_MAX_FLOW_NUM];
static ca_omci_uint8_t            sg_queueUsedNum[OMCI_HAL_MAX_QUEUE_NUM];
static ca_omci_uint8_t            sg_halRebootCfg[PLOAMD_REBOOT_CFG_VAL] = {0};
static ca_omci_bool_t             sg_halRebootCfgFlag = CA_OMCI_FALSE;
static ca_omci_uint8_t            sg_dscpMapTbl[64] = {0};


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);
extern ca_status_t cap_i2c_read(ca_device_id_t device_id, ca_uint8_t slave_addr, ca_uint8_t slave_offset, ca_uint16_t data_len, ca_uint8_t *p_rx_data);


ca_omci_st_e ca_omci_hal_tpid_set(ca_omci_stream_cfg_t *pUsFlow);


#if DESCRIPTION("ONU functions")
ca_omci_st_e ca_omci_hal_onu_reboot(void *in)
{
    ca_omci_uint8_t *flag = (ca_omci_uint8_t *)in;

    OMCIHALDBG("reboot onu flag %d", *flag);
    ca_reset(0, CA_SYS_RESET_MODE_COLD);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_mc_key_set(void *in)
{
    ca_omci_hal_pon_enc_key_t   *key     = (ca_omci_hal_pon_enc_key_t *)in;
    ca_ngp2_encryption_key_id_t  keyIdx  = 0;
    ca_status_t                  ret     = CA_E_OK;

    OMCIHALDBG("cfg onu key idx   %d", key->keyIdx);
    OMCIHALDBG("cfg onu key admin %d", key->keyFlag);
    OMCIHALDBG("cfg onu key len   %d", key->keyLen);
    OMCIHALDBG("cfg onu key 0x%02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x",
               key->key[0], key->key[1], key->key[2], key->key[3],
               key->key[4], key->key[5], key->key[6], key->key[7],
               key->key[8], key->key[9], key->key[10], key->key[11],
               key->key[12], key->key[13], key->key[14], key->key[15]);

    //our key id is 0 and 1, omci have key idx 1 or 2
    keyIdx  = (key->keyIdx == 1) ? CA_NGP2_KEY_INDEX_0 : CA_NGP2_KEY_INDEX_1;
    ret     = ca_ngp2_port_mc_encryption_key_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, keyIdx, key->keyFlag, key->keyLen, key->key);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set pon mc enc failed ret 0x%x", ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_pon_info_get(void *in, void *out)
{
    ca_omci_hal_pon_info_t *ponInfo  = (ca_omci_hal_pon_info_t *)out;
    ca_uint32_t             len      = 10;
    ca_uint8_t              data[10] = {0}; /*data size same as the len*/
    ca_omci_int16_t         temp     = 0;
    ca_omci_uint16_t        volt     = 0;
    ca_omci_uint16_t        txBias   = 0;
    ca_omci_int16_t         txPwr    = 0;
    ca_omci_int16_t         rxPwr    = 0;

    if (CA_E_OK != cap_i2c_read(OMCI_HAL_DEV_ID, OPM_DEVICE_ADDR_1, OPM_DEVICE_OFFSET, len, data)) {
        OMCIHALWARN("failed to read pon info");
        return CA_OMCI_E;
    }

    /*Convert value to the right unit*/
    temp   = data[0] * 256; /* 1/256 degree C */
    volt   = ((data[2] << 8) + data[3]) * 0.05; /* 20mV */
    txBias = ((data[4] << 8) + data[5]); /* 2uA */
    txPwr  = (10 * log10(((data[6] << 8) + data[7]) * 0.0001)) * 500; /* 0.002dB */
    rxPwr  = (10 * log10(((data[8] << 8) + data[9]) * 0.0001)) * 500; /* 0.002dB */

    ponInfo->temperature       = (ca_omci_uint16_t)temp;
    ponInfo->voltage           = volt;
    ponInfo->laserBiasCurrent  = txBias;
    ponInfo->minLaunchPower    = (ca_omci_uint16_t)txPwr;
    ponInfo->rxPower           = (ca_omci_uint16_t)rxPwr;
    ponInfo->minLaunchPowerDbm = (ca_omci_uint16_t)txPwr + 30 * 500; //convert to dbm
    ponInfo->rxPowerDbm        = (ca_omci_uint16_t)rxPwr + 30 * 500;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_reg_id_get(void *in, void *out)
{
    ca_omci_uint8_t  regId[OMCI_ONU_REG_ID_LEN] = {0};
    ca_status_t      ret  = CA_E_OK;

    (void)in;

    ret = ca_ngp2_port_registration_id_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, regId);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Can't get reg id for onu ret[%d]", ret);
        return CA_OMCI_E;
    }

    OMCI_MEMCPY(out, regId, OMCI_ONU_REG_ID_LEN);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_pon_tag_get(void *in, void *out)
{
    ca_omci_uint8_t  ponTag[OMCI_PON_TAG_LEN] = {0};
    ca_status_t      ret  = CA_E_OK;

    (void)in;

    ret = ca_gpon_port_pon_tag_get(OMCI_HAL_DEV_ID, OMCI_PON_TAG_LEN, ponTag);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Can't get pon tag for onu ret[%d]", ret);
        return CA_OMCI_E;
    }

    OMCI_MEMCPY(out, ponTag, OMCI_PON_TAG_LEN);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_msk_set(void *pData)
{
    ca_omci_uint8_t *msk = (ca_omci_uint8_t *)pData;
    ca_status_t      ret = CA_E_OK;

    OMCIHALDBG("set onu msk to 0x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                msk[0], msk[1], msk[2], msk[3], msk[4], msk[5], msk[6], msk[7],
                msk[8], msk[9], msk[10], msk[11], msk[12], msk[13], msk[14], msk[15]);
    ret = ca_gpon_port_msk_update(OMCI_HAL_DEV_ID, msk);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Can't set msk ret[%d]", ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_tod_set(void *pData)
{
    ca_omci_hal_onu_tod_cfg_t *onuTodCfg  = (ca_omci_hal_onu_tod_cfg_t *)pData;
    ca_ptp_set_trigger_t       trigger    = {0};
    ca_ptp_local_clock_t       localClock = {0};
    ca_status_t                ret        = CA_E_OK;
    ca_uint32_t                rem        = 0;
    ca_uint32_t                nsec       = 0;
    ca_uint32_t                onuEqd     = 0;
    ca_uint64_t                offnsec    = 0;
    ca_uint64_t                eqdTime    = 0;
    ca_uint64_t                sec        = 0;

    /* Get eqdTime */
    ret = ca_gpon_port_onu_eqd_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &onuEqd);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get EQD of ONU failed! ret = 0x%x", ret);
        return CA_OMCI_E;
    }
    eqdTime = onuEqd;
    eqdTime = (eqdTime * 100000) / 248832;

    /* Caculate TrecevN */
    ca_div((eqdTime * 500153), 1000000, &offnsec, &rem);
    ca_div(offnsec, 1000000000, &sec, &nsec);

    onuTodCfg->second -= sec;
    if(onuTodCfg->nanoSec < nsec)
    {
        onuTodCfg->second  -= 1;
        onuTodCfg->nanoSec = 1000000000 - nsec + onuTodCfg->nanoSec;
    } else {
        onuTodCfg->nanoSec -= nsec;
    }

    if (onuTodCfg->nanoSec > 1000000000)
    {
        onuTodCfg->second  += 1;
        onuTodCfg->nanoSec -= 1000000000;
    }

    localClock.sec         = onuTodCfg->second;
    localClock.nsec        = onuTodCfg->nanoSec;
    trigger.mask.start_sfc = 1;
    trigger.start_sfc      = onuTodCfg->seqNum;

    OMCIHALDBG("Olt update TOD Seq Number %lld", trigger.start_sfc);
    OMCIHALDBG("Second %llu, Nano Second %u", localClock.sec, localClock.nsec);

    HAL_CHECK_TCL;

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_ptp_local_clock_set(OMCI_HAL_DEV_ID, &trigger, &localClock);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Local clock update failed! ret = 0x%x", ret);
        OMCIHALERR("Olt update TOD Seq Number %lld", trigger.start_sfc);
        OMCIHALERR("Second %lld, Nano Second %d", localClock.sec, localClock.nsec);
        return CA_OMCI_E;
    }
#endif
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_dg_alarm_set(
    void *pData,
    void *pLen)
{
    ca_omci_uint8_t             *pkt    = (ca_omci_uint8_t *)pData;
    ca_omci_uint16_t            *pktLen = (ca_omci_uint16_t *)pLen;
    ca_status_t                  ret    = CA_E_OK;
    ca_gpon_dying_gasp_config_t  dgCfg  = {0};

    dgCfg.mask.omci_dying_gasp_pdu_valid = 1;
    dgCfg.omci_buffer_length             = *pktLen;
    OMCI_MEMCPY(dgCfg.omci_buffer, pkt, *pktLen);

    ret = ca_gpon_port_dying_gasp_set(OMCI_HAL_DEV_ID, 0, &dgCfg);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Can't set dying gasp for onu ret[%d]", ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_onu_pwmng_set(
    void *pData)
{
    ca_omci_dyn_power_ctl_attr_t          *pCfg = (ca_omci_dyn_power_ctl_attr_t *)pData;
    ca_gpon_power_management_parameters_t  cfg  = {0};
    ca_status_t                            ret  = CA_E_OK;

    ret = ca_gpon_port_power_mgmt_param_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &cfg);
    if (CA_E_OK != ret) {
        OMCIHALERR("Get pwmn param failed ret[%d]", ret);
        return CA_OMCI_E;
    }

    /* Need to disable the cfg and update the whole value*/
    if ((0 != pCfg->powerReduMngMode) && (SLEEP_MODE_OFF != cfg.mode)){
        cfg.mask.mode = 1;
        cfg.mode      = SLEEP_MODE_OFF;
        (void) ca_gpon_port_power_mgmt_param_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &cfg);
        ca_udelay(100);
    }

    cfg.mask.mode      = 1;
    cfg.mask.ilowpower = 1;
    cfg.mask.iaware    = 1;
    cfg.mask.irxoff    = 1;
    cfg.mask.ihold     = 1;
    cfg.mode      = (0 == pCfg->powerReduMngMode) ? SLEEP_MODE_OFF : SLEEP_MODE_DOZE;
    cfg.ilowpower = pCfg->maxSleepInterval;
    cfg.iaware    = pCfg->minAwareInterval;
    cfg.irxoff    = pCfg->maxRxOffInterval;
    cfg.ihold     = pCfg->minActHeldInterval;
    ret = ca_gpon_port_power_mgmt_param_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &cfg);
    if (CA_E_OK != ret){
        OMCIHALERR("Set pwmn param failed ret[%d]", ret);
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}

#endif


#if DESCRIPTION("tool functions")
ca_port_id_t ca_omci_hal_get_port_id(ca_omci_uint8_t ponPort)
{
    //should add some convert for port as define in CA API
    return OMCI_HAL_WAN_PORT;
}


void ca_omci_hal_dbg_print_gemflow(ca_omci_gem_flow_t *pGemFlow)
{
    OMCIHALDBG("gemId %d",             pGemFlow->gemId);
    OMCIHALDBG("tcontId %d",           pGemFlow->tcontId);
    OMCIHALDBG("allocId %d",           pGemFlow->allocId);
    OMCIHALDBG("direction %d",         pGemFlow->direction);
    OMCIHALDBG("usQueue %d",           pGemFlow->usQueue);
    OMCIHALDBG("usQueueWeight %d",     pGemFlow->usQueueWeight);
    OMCIHALDBG("usTrafficDes mode %d", pGemFlow->usTrafficDes.mode);
    OMCIHALDBG("usTrafficDes pps %d",  pGemFlow->usTrafficDes.pps);
    OMCIHALDBG("usTrafficDes cir %d",  pGemFlow->usTrafficDes.cir);
    OMCIHALDBG("usTrafficDes cbs %d",  pGemFlow->usTrafficDes.cbs);
    OMCIHALDBG("usTrafficDes pir %d",  pGemFlow->usTrafficDes.pir);
    OMCIHALDBG("usTrafficDes pbs %d",  pGemFlow->usTrafficDes.pbs);
    OMCIHALDBG("dsQueue %d",           pGemFlow->dsQueue);
    OMCIHALDBG("dsQueueWeight %d",     pGemFlow->dsQueueWeight);
    OMCIHALDBG("dsTrafficDes mode %d", pGemFlow->dsTrafficDes.mode);
    OMCIHALDBG("dsTrafficDes pps %d",  pGemFlow->dsTrafficDes.pps);
    OMCIHALDBG("dsTrafficDes cir %d",  pGemFlow->dsTrafficDes.cir);
    OMCIHALDBG("dsTrafficDes cbs %d",  pGemFlow->dsTrafficDes.cbs);
    OMCIHALDBG("dsTrafficDes pir %d",  pGemFlow->dsTrafficDes.pir);
    OMCIHALDBG("dsTrafficDes pbs %d",  pGemFlow->dsTrafficDes.pbs);
    OMCIHALDBG("aesState %d",          pGemFlow->aesState);
    OMCIHALDBG("aesKeyRing %d",        pGemFlow->aesKeyRing);
    OMCIHALDBG("gemPri 0x%x",          pGemFlow->gemPri);
}


void ca_omci_hal_dbg_print_tcont(ca_omci_tcont_cfg_t *pTcCfg)
{
    ca_omci_uint8_t idx = 0;

    OMCIHALDBG("tcontId %d", pTcCfg->tcontId);
    OMCIHALDBG("allocId %d", pTcCfg->allocId);
    OMCIHALDBG("policy %d",  pTcCfg->policy);
    for (idx = 0; idx < OMCI_PQ_NUM_PER_TCONT; idx++)
    {
        OMCIHALDBG("wrrQue[%d] %d", idx, pTcCfg->wrrQue[idx]);
    }
    OMCIHALDBG("schedId %d",     pTcCfg->schedId);
    OMCIHALDBG("schedPoint %d",  pTcCfg->schedPoint);
    OMCIHALDBG("schedPolicy %d", pTcCfg->schedPolicy);
    OMCIHALDBG("schedWeight %d", pTcCfg->schedWeight);
}


void ca_omci_hal_dbg_print_flow(ca_omci_stream_cfg_t *pData)
{
    ca_omci_uint8_t idx = 0;

    OMCIHALDBG("gemId %d",       pData->gemId);
    OMCIHALDBG("uniPortId %d",   pData->uniPortId);
    OMCIHALDBG("portType %d",    pData->portType);
    OMCIHALDBG("filterFlag %d",  pData->filterFlag);
    OMCIHALDBG("dsMode %d",      pData->dsMode);

    if (CA_OMCI_TRUE == pData->untagFilterIpoe.isValid)
    {
        OMCIHALDBG("untagFilterIpoe fwdType %d",                pData->untagFilterIpoe.fwdType);
        OMCIHALDBG("untagFilterIpoe etherTypeFilter %d",        pData->untagFilterIpoe.etherTypeFilter);
        OMCIHALDBG("untagFilterIpoe innerVlanTreatment vid %d", pData->untagFilterIpoe.innerVlanTreatment.vid);
        OMCIHALDBG("untagFilterIpoe innerVlanTreatment pri %d", pData->untagFilterIpoe.innerVlanTreatment.pri);
        OMCIHALDBG("untagFilterIpoe outerVlanTreatment vid %d", pData->untagFilterIpoe.outerVlanTreatment.vid);
        OMCIHALDBG("untagFilterIpoe outerVlanTreatment pri %d", pData->untagFilterIpoe.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->untagFilterIpoev6.isValid)
    {
        OMCIHALDBG("untagFilterIpoev6 fwdType %d",                pData->untagFilterIpoev6.fwdType);
        OMCIHALDBG("untagFilterIpoev6 etherTypeFilter %d",        pData->untagFilterIpoev6.etherTypeFilter);
        OMCIHALDBG("untagFilterIpoev6 innerVlanTreatment vid %d", pData->untagFilterIpoev6.innerVlanTreatment.vid);
        OMCIHALDBG("untagFilterIpoev6 innerVlanTreatment pri %d", pData->untagFilterIpoev6.innerVlanTreatment.pri);
        OMCIHALDBG("untagFilterIpoev6 outerVlanTreatment vid %d", pData->untagFilterIpoev6.outerVlanTreatment.vid);
        OMCIHALDBG("untagFilterIpoev6 outerVlanTreatment pri %d", pData->untagFilterIpoev6.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->untagFilterPppoe.isValid)
    {
        OMCIHALDBG("untagFilterPppoe fwdType %d",                pData->untagFilterPppoe.fwdType);
        OMCIHALDBG("untagFilterPppoe etherTypeFilter %d",        pData->untagFilterPppoe.etherTypeFilter);
        OMCIHALDBG("untagFilterPppoe innerVlanTreatment vid %d", pData->untagFilterPppoe.innerVlanTreatment.vid);
        OMCIHALDBG("untagFilterPppoe innerVlanTreatment pri %d", pData->untagFilterPppoe.innerVlanTreatment.pri);
        OMCIHALDBG("untagFilterPppoe outerVlanTreatment vid %d", pData->untagFilterPppoe.outerVlanTreatment.vid);
        OMCIHALDBG("untagFilterPppoe outerVlanTreatment pri %d", pData->untagFilterPppoe.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->untagFilterArp.isValid)
    {
        OMCIHALDBG("untagFilterArp fwdType %d",                pData->untagFilterArp.fwdType);
        OMCIHALDBG("untagFilterArp etherTypeFilter %d",        pData->untagFilterArp.etherTypeFilter);
        OMCIHALDBG("untagFilterArp innerVlanTreatment vid %d", pData->untagFilterArp.innerVlanTreatment.vid);
        OMCIHALDBG("untagFilterArp innerVlanTreatment pri %d", pData->untagFilterArp.innerVlanTreatment.pri);
        OMCIHALDBG("untagFilterArp outerVlanTreatment vid %d", pData->untagFilterArp.outerVlanTreatment.vid);
        OMCIHALDBG("untagFilterArp outerVlanTreatment pri %d", pData->untagFilterArp.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->untagFilterDef.isValid)
    {
        OMCIHALDBG("untagFilterDef fwdType %d",                pData->untagFilterDef.fwdType);
        OMCIHALDBG("untagFilterDef etherTypeFilter %d",        pData->untagFilterDef.etherTypeFilter);
        OMCIHALDBG("untagFilterDef innerVlanTreatment vid %d", pData->untagFilterDef.innerVlanTreatment.vid);
        OMCIHALDBG("untagFilterDef innerVlanTreatment pri %d", pData->untagFilterDef.innerVlanTreatment.pri);
        OMCIHALDBG("untagFilterDef outerVlanTreatment vid %d", pData->untagFilterDef.outerVlanTreatment.vid);
        OMCIHALDBG("untagFilterDef outerVlanTreatment pri %d", pData->untagFilterDef.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->sglTagFilterDef.isValid)
    {
        OMCIHALDBG("sglTagFilterDef fwdType %d",                pData->sglTagFilterDef.fwdType);
        OMCIHALDBG("sglTagFilterDef etherTypeFilter %d",        pData->sglTagFilterDef.etherTypeFilter);
        OMCIHALDBG("sglTagFilterDef vlanFilter vid %d",         pData->sglTagFilterDef.vlanFilter.vid);
        OMCIHALDBG("sglTagFilterDef vlanFilter pri %d",         pData->sglTagFilterDef.vlanFilter.pri);
        OMCIHALDBG("sglTagFilterDef innerVlanTreatment vid %d", pData->sglTagFilterDef.innerVlanTreatment.vid);
        OMCIHALDBG("sglTagFilterDef innerVlanTreatment pri %d", pData->sglTagFilterDef.innerVlanTreatment.pri);
        OMCIHALDBG("sglTagFilterDef outerVlanTreatment vid %d", pData->sglTagFilterDef.outerVlanTreatment.vid);
        OMCIHALDBG("sglTagFilterDef outerVlanTreatment pri %d", pData->sglTagFilterDef.outerVlanTreatment.pri);
    }

    if (CA_OMCI_TRUE == pData->dblTagFilterDef.isValid)
    {
        OMCIHALDBG("dblTagFilterDef fwdType %d",                pData->dblTagFilterDef.fwdType);
        OMCIHALDBG("dblTagFilterDef etherTypeFilter %d",        pData->dblTagFilterDef.etherTypeFilter);
        OMCIHALDBG("dblTagFilterDef innerVlanFilter vid %d",    pData->dblTagFilterDef.innerVlanTreatment.vid);
        OMCIHALDBG("dblTagFilterDef innerVlanFilter pri %d",    pData->dblTagFilterDef.innerVlanTreatment.pri);
        OMCIHALDBG("dblTagFilterDef outerVlanFilter vid %d",    pData->dblTagFilterDef.outerVlanFilter.vid);
        OMCIHALDBG("dblTagFilterDef outerVlanFilter pri %d",    pData->dblTagFilterDef.outerVlanFilter.pri);
        OMCIHALDBG("dblTagFilterDef innerVlanTreatment vid %d", pData->dblTagFilterDef.innerVlanTreatment.vid);
        OMCIHALDBG("dblTagFilterDef innerVlanTreatment pri %d", pData->dblTagFilterDef.innerVlanTreatment.pri);
        OMCIHALDBG("dblTagFilterDef outerVlanTreatment vid %d", pData->dblTagFilterDef.outerVlanTreatment.vid);
        OMCIHALDBG("dblTagFilterDef outerVlanTreatment pri %d", pData->dblTagFilterDef.outerVlanTreatment.pri);
    }

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        if (CA_OMCI_TRUE == pData->sglTagFilter[idx].isValid)
        {
            OMCIHALDBG("sglTagFilter rule %d fwdType %d",                idx, pData->sglTagFilter[idx].fwdType);
            OMCIHALDBG("sglTagFilter rule %d etherTypeFilter %d",        idx, pData->sglTagFilter[idx].etherTypeFilter);
            OMCIHALDBG("sglTagFilter rule %d vlanFilter vid %d",         idx, pData->sglTagFilter[idx].vlanFilter.vid);
            OMCIHALDBG("sglTagFilter rule %d vlanFilter pri %d",         idx, pData->sglTagFilter[idx].vlanFilter.pri);
            OMCIHALDBG("sglTagFilter rule %d innerVlanTreatment vid %d", idx, pData->sglTagFilter[idx].innerVlanTreatment.vid);
            OMCIHALDBG("sglTagFilter rule %d innerVlanTreatment pri %d", idx, pData->sglTagFilter[idx].innerVlanTreatment.pri);
            OMCIHALDBG("sglTagFilter rule %d outerVlanTreatment vid %d", idx, pData->sglTagFilter[idx].outerVlanTreatment.vid);
            OMCIHALDBG("sglTagFilter rule %d outerVlanTreatment pri %d", idx, pData->sglTagFilter[idx].outerVlanTreatment.pri);
        }

        if (CA_OMCI_TRUE == pData->dblTagFilter[idx].isValid)
        {
            OMCIHALDBG("dblTagFilter rule %d fwdType %d",                idx, pData->dblTagFilter[idx].fwdType);
            OMCIHALDBG("dblTagFilter rule %d etherTypeFilter %d",        idx, pData->dblTagFilter[idx].etherTypeFilter);
            OMCIHALDBG("dblTagFilter rule %d innerVlanFilter vid %d",    idx, pData->dblTagFilter[idx].innerVlanTreatment.vid);
            OMCIHALDBG("dblTagFilter rule %d innerVlanFilter pri %d",    idx, pData->dblTagFilter[idx].innerVlanTreatment.pri);
            OMCIHALDBG("dblTagFilter rule %d outerVlanFilter vid %d",    idx, pData->dblTagFilter[idx].outerVlanFilter.vid);
            OMCIHALDBG("dblTagFilter rule %d outerVlanFilter pri %d",    idx, pData->dblTagFilter[idx].outerVlanFilter.pri);
            OMCIHALDBG("dblTagFilter rule %d innerVlanTreatment vid %d", idx, pData->dblTagFilter[idx].innerVlanTreatment.vid);
            OMCIHALDBG("dblTagFilter rule %d innerVlanTreatment pri %d", idx, pData->dblTagFilter[idx].innerVlanTreatment.pri);
            OMCIHALDBG("dblTagFilter rule %d outerVlanTreatment vid %d", idx, pData->dblTagFilter[idx].outerVlanTreatment.vid);
            OMCIHALDBG("dblTagFilter rule %d outerVlanTreatment pri %d", idx, pData->dblTagFilter[idx].outerVlanTreatment.pri);
        }
    }
}


ca_boolean_t ca_omci_hal_eth_autoneg_get(ca_omci_uint8_t mode)
{
    if ((PPTP_ETH_AUTO_DETEC_CFG_AUTO_AUTO == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_AUTO_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_AUTO == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_AUTO_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_AUTO == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_AUTO == mode))
    {
        OMCIHALDBG("Enable auto nego");
        return CA_OMCI_TRUE;
    }

    OMCIHALDBG("Disable auto nego");
    return CA_OMCI_FALSE;
}


ca_uint32_t ca_omci_hal_eth_speed_get(ca_omci_uint8_t mode)
{
    if ((PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_AUTO == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_HALF == mode))
    {
        OMCIHALDBG("Set speed to 10M");
        return CA_PORT_SPEED_10M;
    }

    if ((PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_AUTO == mode))
    {
        OMCIHALDBG("Set speed to 100M");
        return CA_PORT_SPEED_100M;
    }

    if ((PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_AUTO == mode))
    {
        OMCIHALDBG("Set speed to 1G");
        return CA_PORT_SPEED_1G;
    }

    if (PPTP_ETH_AUTO_DETEC_CFG_10G_ONLY_FULL == mode)
    {
        OMCIHALDBG("Set speed to 10G");
        return CA_PORT_SPEED_SFI_10G;
    }

    if (PPTP_ETH_AUTO_DETEC_CFG_2G5_ONLY_FULL == mode)
    {
        OMCIHALDBG("Set speed to 2.5G");
        return CA_PORT_SPEED_2_5G;
    }

    if (PPTP_ETH_AUTO_DETEC_CFG_5G_ONLY_FULL == mode)
    {
        OMCIHALDBG("Set speed to 5G");
        return CA_PORT_SPEED_5G;
    }

    if (PPTP_ETH_AUTO_DETEC_CFG_25G_ONLY_FULL == mode)
    {
        OMCIHALDBG("Set speed to 25G");
        return CA_PORT_SPEED_SFI_25G;
    }

    if (PPTP_ETH_AUTO_DETEC_CFG_AUTO_AUTO == mode)
    {
        OMCIHALDBG("Set speed to AUTO");
        return CA_PORT_SPEED_SFI_AUTO;
    }

    OMCIHALDBG("No need to set speed");
    return CA_PORT_SPEED_INVALID;
}


ca_omci_st_e ca_omci_hal_eth_duplex_get(
    ca_omci_uint8_t       mode,
    ca_eth_port_duplex_t *pDuplex)
{
    if ((PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_HALF == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_AUTO_HALF == mode))
    {
        OMCIHALDBG("Set duplex half");
        *pDuplex = CA_PORT_DUPLEX_HALF;
        return CA_OMCI_OK;
    }

    if ((PPTP_ETH_AUTO_DETEC_CFG_10M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_100M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_1000M_ONLY_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_AUTO_FULL == mode)
     || (PPTP_ETH_AUTO_DETEC_CFG_10G_ONLY_FULL == mode))
    {
        OMCIHALDBG("Set duplex full");
        *pDuplex = CA_PORT_DUPLEX_FULL;
        return CA_OMCI_OK;
    }

    OMCIHALDBG("No need to set duplex");
    return CA_OMCI_E;
}


ca_omci_uint16_t ca_omci_hal_get_eth_type(ca_omci_filter_eth_type_e ethType)
{
    if (ethType == CA_OMCI_FILTER_IPOE_FRAMES) return OMCI_HAL_ETH_TYPE_IPOE;
    if (ethType == CA_OMCI_FILTER_PPPOE_FRAMES) return OMCI_HAL_ETH_TYPE_PPPOE;
    if (ethType == CA_OMCI_FILTER_ARP_FRAMES) return OMCI_HAL_ETH_TYPE_ARP;
    if (ethType == CA_OMCI_FILTER_IPOE_V6_FRAMES) return OMCI_HAL_ETH_TYPE_IPOEV6;

    return OMCI_HAL_ETH_TYPE_DEF;
}
#endif


#if DESCRIPTION("Flow to classifier idx map functions")
ca_omci_uint32_t *ca_omci_hal_flow_classifier_idx_get(
    ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            return sg_halRuleMap[index].classifierIdx;
        }
    }

    return 0;
}


ca_omci_st_e ca_omci_hal_flow_classifier_idx_save(
    ca_omci_stream_cfg_t *pStream,
    ca_omci_uint32_t      idx)
{
    ca_omci_uint16_t index   = 0;
    ca_omci_uint8_t  ruleIdx = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW_US; ruleIdx++)
            {
                if (0 == sg_halRuleMap[index].classifierIdx[ruleIdx])
                {
                    sg_halRuleMap[index].classifierIdx[ruleIdx] = idx;
                    break;
                }
            }

            if (OMCI_HAL_MAX_RULE_PER_FLOW_US == ruleIdx)
            {
                OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                           pStream->gemId, pStream->uniPortId, pStream->portType);
                return CA_OMCI_E;
            }
            break;
        }
    }

    //never store this flow before, make a new record
    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
        {
            if (0 == sg_halRuleMap[index].gemId)
            {
                sg_halRuleMap[index].gemId            = pStream->gemId;
                sg_halRuleMap[index].uniPortId        = pStream->uniPortId;
                sg_halRuleMap[index].portType         = pStream->portType;
                sg_halRuleMap[index].classifierIdx[0] = idx;
                break;
            }
        }
    }

    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                   pStream->gemId, pStream->uniPortId, pStream->portType);
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_uint32_t *ca_omci_hal_flow_ds_classifier_idx_get(
    ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            return sg_halRuleMap[index].dsClassifierIdx;
        }
    }

    return NULL;
}


ca_omci_st_e ca_omci_hal_flow_ds_classifier_idx_save(
    ca_omci_stream_cfg_t *pStream,
    ca_omci_uint32_t      idx)
{
    ca_omci_uint16_t  index   = 0;
    ca_omci_uint8_t   ruleIdx = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW; ruleIdx++)
            {
                if (0 == sg_halRuleMap[index].dsClassifierIdx[ruleIdx])
                {
                    sg_halRuleMap[index].dsClassifierIdx[ruleIdx] = idx;
                    break;
                }
            }
            if (OMCI_HAL_MAX_RULE_PER_FLOW == ruleIdx)
            {
                OMCIHALERR("can't store so many rules for gem %d port 0x%x type %d",
                           pStream->gemId, pStream->uniPortId, pStream->portType);
                return CA_OMCI_E;
            }
            break;
        }
    }

    //never store this flow before, make a new record
    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
        {
            if (0 == sg_halRuleMap[index].gemId)
            {
                sg_halRuleMap[index].gemId            = pStream->gemId;
                sg_halRuleMap[index].uniPortId        = pStream->uniPortId;
                sg_halRuleMap[index].portType         = pStream->portType;
                sg_halRuleMap[index].dsClassifierIdx[0] = idx;
                break;
            }
        }
    }

    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                   pStream->gemId, pStream->uniPortId, pStream->portType);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_bool_t ca_omci_hal_flow_def_classifier_idx_check(ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            if (0 != sg_halRuleMap[index].defClassifierIdx)
            {
                return CA_OMCI_TRUE;
            }
        }
    }

    return CA_OMCI_FALSE;
}


ca_omci_uint32_t ca_omci_hal_flow_def_classifier_idx_get(ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            return sg_halRuleMap[index].defClassifierIdx;
        }
    }

    return 0;
}


void ca_omci_hal_flow_def_classifier_idx_release(ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            sg_halRuleMap[index].defClassifierIdx = 0;
        }
    }
}


ca_omci_st_e ca_omci_hal_flow_def_classifier_idx_save(
    ca_omci_stream_cfg_t *pStream,
    ca_omci_uint32_t      idx)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            if (0 == sg_halRuleMap[index].defClassifierIdx)
            {
                sg_halRuleMap[index].defClassifierIdx = idx;
                return CA_OMCI_OK;
            }

        }
    }

    //never store this flow before, make a new record
    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
        {
            if (0 == sg_halRuleMap[index].gemId)
            {
                sg_halRuleMap[index].gemId            = pStream->gemId;
                sg_halRuleMap[index].uniPortId        = pStream->uniPortId;
                sg_halRuleMap[index].portType         = pStream->portType;
                sg_halRuleMap[index].defClassifierIdx = idx;
                return CA_OMCI_OK;
            }
        }
    }

    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                   pStream->gemId, pStream->uniPortId, pStream->portType);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


void ca_omci_hal_flow_classifier_tbl_release(
    ca_omci_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halRuleMap[index].gemId == pStream->gemId)
         && (sg_halRuleMap[index].uniPortId == pStream->uniPortId)
         && (sg_halRuleMap[index].portType == pStream->portType))
        {
            sg_halRuleMap[index].gemId     = 0;
            sg_halRuleMap[index].uniPortId = 0;
            sg_halRuleMap[index].portType  = 0;
            break;
        }
    }
}


ca_omci_uint32_t *ca_omci_hal_filter_classifier_idx_get(
    ca_omci_filter_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halFilterMap[index].gemId == pStream->gemId)
         && (sg_halFilterMap[index].uniPortId == pStream->uniPortId)
         && (sg_halFilterMap[index].portType == pStream->portType))
        {
            return sg_halFilterMap[index].classifierIdx;
        }
    }

    return NULL;
}


ca_omci_st_e ca_omci_hal_filter_classifier_idx_save(
    ca_omci_filter_stream_cfg_t *pStream,
    ca_omci_uint32_t             idx)
{
    ca_omci_uint16_t  index   = 0;
    ca_omci_uint8_t   ruleIdx = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halFilterMap[index].gemId == pStream->gemId)
         && (sg_halFilterMap[index].uniPortId == pStream->uniPortId)
         && (sg_halFilterMap[index].portType == pStream->portType))
        {
            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW_US; ruleIdx++)
            {
                if (0 == sg_halFilterMap[index].classifierIdx[ruleIdx])
                {
                    sg_halFilterMap[index].classifierIdx[ruleIdx] = idx;
                    break;
                }
            }

            if (OMCI_HAL_MAX_RULE_PER_FLOW_US == ruleIdx)
            {
                OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                           pStream->gemId, pStream->uniPortId, pStream->portType);
                return CA_OMCI_E;
            }
            break;
        }
    }

    //never store this flow before, make a new record
    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
        {
            if (0 == sg_halFilterMap[index].gemId)
            {
                sg_halFilterMap[index].gemId            = pStream->gemId;
                sg_halFilterMap[index].uniPortId        = pStream->uniPortId;
                sg_halFilterMap[index].portType         = pStream->portType;
                sg_halFilterMap[index].classifierIdx[0] = idx;
                break;
            }
        }
    }

    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                   pStream->gemId, pStream->uniPortId, pStream->portType);
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_uint32_t *ca_omci_hal_ds_filter_classifier_idx_get(
    ca_omci_filter_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halFilterMap[index].gemId == pStream->gemId)
         && (sg_halFilterMap[index].uniPortId == pStream->uniPortId)
         && (sg_halFilterMap[index].portType == pStream->portType))
        {
            return sg_halFilterMap[index].dsClassifierIdx;
        }
    }

    return NULL;
}


ca_omci_st_e ca_omci_hal_ds_filter_classifier_idx_save(
    ca_omci_filter_stream_cfg_t *pStream,
    ca_omci_uint32_t             idx)
{
    ca_omci_uint16_t  index   = 0;
    ca_omci_uint8_t   ruleIdx = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halFilterMap[index].gemId == pStream->gemId)
         && (sg_halFilterMap[index].uniPortId == pStream->uniPortId)
         && (sg_halFilterMap[index].portType == pStream->portType))
        {
            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW; ruleIdx++)
            {
                if (0 == sg_halFilterMap[index].dsClassifierIdx[ruleIdx])
                {
                    sg_halFilterMap[index].dsClassifierIdx[ruleIdx] = idx;
                    break;
                }
            }

            if (OMCI_HAL_MAX_RULE_PER_FLOW == ruleIdx)
            {
                OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                           pStream->gemId, pStream->uniPortId, pStream->portType);
                return CA_OMCI_E;
            }
            break;
        }
    }

    //never store this flow before, make a new record
    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
        {
            if (0 == sg_halFilterMap[index].gemId)
            {
                sg_halFilterMap[index].gemId              = pStream->gemId;
                sg_halFilterMap[index].uniPortId          = pStream->uniPortId;
                sg_halFilterMap[index].portType           = pStream->portType;
                sg_halFilterMap[index].dsClassifierIdx[0] = idx;
                break;
            }
        }
    }

    if (OMCI_HAL_MAX_FLOW_NUM == index)
    {
        OMCIHALERR("can't store so many rules for gem %d port %d type %d",
                   pStream->gemId, pStream->uniPortId, pStream->portType);
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_filter_classifier_tbl_release(
    ca_omci_filter_stream_cfg_t *pStream)
{
    ca_omci_uint16_t  index  = 0;

    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++)
    {
        if ((sg_halFilterMap[index].gemId == pStream->gemId)
         && (sg_halFilterMap[index].uniPortId == pStream->uniPortId)
         && (sg_halFilterMap[index].portType == pStream->portType))
        {
            sg_halFilterMap[index].gemId     = 0;
            sg_halFilterMap[index].uniPortId = 0;
            sg_halFilterMap[index].portType  = 0;
            break;
        }
    }

    return CA_OMCI_OK;
}


void ca_omci_hal_flow_cls_show(const struct shell *cli)
{
    ca_omci_uint16_t index   = 0;
    ca_omci_uint16_t ruleIdx = 0;
    ca_omci_uint32_t number  = 0;

    shell_print(cli, "======================= Stream CLSTBL =========================");
    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++) {
        if (0 != sg_halRuleMap[index].gemId) {
            if (0 != sg_halRuleMap[index].defClassifierIdx) {
                shell_print(cli, "    Gemport Id[%d] uniPortInstance[0x%x] PortType[%d] defcls[%d]",
                                sg_halRuleMap[index].gemId, sg_halRuleMap[index].uniPortId,
                                sg_halRuleMap[index].portType, sg_halRuleMap[index].defClassifierIdx);
            }

            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW_US; ruleIdx++) {
                if (0 != sg_halRuleMap[index].classifierIdx[ruleIdx]) {
                    shell_print(cli, "    Gemport Id[%d] uniPortInstance[0x%x] PortType[%d] uscls[%d]",
                                    sg_halRuleMap[index].gemId, sg_halRuleMap[index].uniPortId,
                                    sg_halRuleMap[index].portType, sg_halRuleMap[index].classifierIdx[ruleIdx]);
                    number++;
                }
            }

            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW; ruleIdx++) {
                if (0 != sg_halRuleMap[index].dsClassifierIdx[ruleIdx]) {
                    shell_print(cli, "    Gemport Id[%d] uniPortInstance[0x%x] PortType[%d] dscls[%d]",
                                    sg_halRuleMap[index].gemId, sg_halRuleMap[index].uniPortId, sg_halRuleMap[index].portType,
                                    sg_halRuleMap[index].dsClassifierIdx[ruleIdx]);
                    number++;
                }
            }
        }
    }

    if (0 == number) {
        shell_print(cli, "    No Stream Classifier configure now");
    }

    number = 0;
    shell_print(cli, "======================= Filter CLSTBL =========================");
    for (index = 0; index < OMCI_HAL_MAX_FLOW_NUM; index++) {
        if (0 != sg_halFilterMap[index].gemId) {
            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW_US; ruleIdx++) {
                if (0 != sg_halFilterMap[index].classifierIdx[ruleIdx]) {
                    shell_print(cli, "    Gemport Id[%d] uniPortInstance[0x%x] PortType[%d] classidx[%d]",
                                    sg_halFilterMap[index].gemId, sg_halFilterMap[index].uniPortId, sg_halFilterMap[index].portType,
                                    sg_halFilterMap[index].classifierIdx[ruleIdx]);
                    number++;
                }
            }

            for (ruleIdx = 0; ruleIdx < OMCI_HAL_MAX_RULE_PER_FLOW; ruleIdx++) {
                if (0 != sg_halFilterMap[index].dsClassifierIdx[ruleIdx]) {
                    shell_print(cli, "    Gemport Id[%d] uniPortInstance[0x%x] PortType[%d] dsclassidx [%d]",
                                    sg_halFilterMap[index].gemId, sg_halFilterMap[index].uniPortId, sg_halFilterMap[index].portType,
                                    sg_halFilterMap[index].dsClassifierIdx[ruleIdx]);
                    number++;
                }
            }
        }
    }

    if (0 == number) {
        shell_print(cli, "    No Filter Classifier configure now");
    }
}

#endif


#if DESCRIPTION("Get actual Id from entityID")
/* The input tcontId is the entityID OMCI set, so get the real tcont in CA API*/
/* Tcont entityID calculate like ((onuinfo->ponSlot & 0xff) << 8) | tcIdx */
/* So tcIdx will be the last 1 byte for Tcont entityID */
/* If change tcont entityID rule, need to change this function too!*/
ca_uint32_t ca_omci_hal_tcont_idx_get(ca_omci_uint32_t tcontId)
{
    ca_uint32_t tcIdx = 0;

    tcIdx = (tcontId & 0xff) + 1;
    OMCIHALDBG("Real tcont index [%d]", tcIdx);

    return tcIdx;
}


/* The input pqId is the entityID OMCI set, so get the real pqIdx in CA API*/
/* upstream priority queue entityID calculate like  */
/* 0x8000 | (tcIdx * maxQueuePerTc + pqIdx) */
/* If change upstream priority queue entityID rule, need to change this function too!*/
ca_uint32_t ca_omci_hal_us_pq_idx_get(ca_omci_uint32_t tcIdx, ca_omci_uint32_t pqId)
{
    ca_uint32_t            pqIdx   = 0;
    ca_uint32_t            uspq    = 0;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();

    pqIdx = (onuinfo->maxQueuePerTc - 1) - ((pqId & 0x7fff) - (((tcIdx - 1) & 0xff) * onuinfo->maxQueuePerTc));
    uspq  = pqIdx + (((tcIdx - 1) & 0xff) * onuinfo->maxQueuePerTc);//for hal use 0-247
    OMCIHALDBG("Real us priority queue index [%d], pqId [%d]", pqIdx, uspq);

    return uspq;
}


/* The input pqId is the entityID OMCI set, so get the real pqIdx in CA API*/
/* downstream priority queue entityID calculate like almost the same for diff uni type */
/* ((onuinfo->veipSlot & 0xf) << 12) | (idx * OMCI_PQ_NUM_PER_UNI + pqIdx) */
/* ((onuinfo->geSlot & 0xf) << 12) | (idx * OMCI_PQ_NUM_PER_UNI + pqIdx) */
/* If change downstream priority queue entityID rule, need to change this function too!*/
ca_uint32_t ca_omci_hal_ds_pq_idx_get(ca_omci_uint32_t pqId)
{
    ca_uint32_t pqIdx = 0;

    pqIdx = 7 - ((pqId & 0xfff) % OMCI_PQ_NUM_PER_UNI);
    OMCIHALDBG("Real ds priority queue index [%d]", pqIdx);

    return pqIdx;
}


/* When priority queues are created by ONU, the priority of each queue were defined */
/* pri mached the queue idx of each queue */
/* pqRelaPort = (me->meBase.instanceId << 16) | pqIdx */
/* If change priority queue relate port attr rule, need to change this function too!*/
ca_uint32_t ca_omci_hal_us_pq_pri_get(ca_omci_uint32_t tcIdx, ca_omci_uint32_t pqId)
{
    ca_uint32_t            pqIdx   = 0;
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();

    pqIdx = (onuinfo->maxQueuePerTc - 1) - ((pqId & 0x7fff) - (((tcIdx - 1) & 0xff) * onuinfo->maxQueuePerTc));
    OMCIHALDBG("priority queue pri [%d]", pqIdx);

    return pqIdx;
}


/* The input eth port is the portId, so get the instance id */
/* Eth entityID calculate in function ca_omci_me_pptp_eth_auto_create */
/* If change eth entityID rule, need to change this function too!*/
/* In CA API, portId is 4 bytes long, the first 2 bytes is port type */
/* And the last 2 bytes are port Idx, range is 0-7 for OMCI shouldn't check this range */
ca_omci_uint16_t ca_omci_hal_eth_instance_get(ca_uint32_t portId)
{
    ca_omci_onuinfo_obj_t *onuinfo    = ca_omci_onuinfo_get();
    ca_omci_uint16_t       instanceId = 0;
    ca_omci_uint16_t       portNo     = 0;
    ca_omci_uint16_t       slotNo     = 0;
    ca_omci_uint16_t       idx        = 0;

    portNo = PORT_ID(portId);
    for (idx = 0; idx < onuinfo->ethNum; idx++)
    {
        if (portNo == onuinfo->ethPortIdMap[idx])
        {
            slotNo     = ((idx + 1) / 8) + (((idx + 1)% 8) ? 1 : 0) - 1;
            instanceId = ((slotNo + onuinfo->ethSlot) << 8) + ((idx + 1) % 8);
            OMCIHALDBG("Real port Idx [%d] portId[0x%x]", (idx + 1), instanceId);
            return instanceId;
        }
    }

    OMCIHALERR("Can't find port [0x%x]", portId);
    return 0;
}


/* The input eth port is the entityID OMCI set, so get the real port id */
/* Eth entityID calculate in function ca_omci_me_pptp_eth_auto_create */
/* If change eth entityID rule, need to change this function too!*/
/* In CA API, src port shoule be AAL_LPORT_ETH_NI0 to AAL_LPORT_ETH_NI7 */
/* Equal to 0-7 */
ca_uint32_t ca_omci_hal_eth_port_get(ca_omci_uint16_t portInst)
{
    ca_omci_onuinfo_obj_t *onuinfo = ca_omci_onuinfo_get();
    ca_uint32_t            portIdx = 0;
    ca_uint32_t            portId  = 0;
    ca_uint32_t            slotNo  = 0;

    if (onuinfo->ethNum > 8) {
        OMCIHALERR("Can't support so many port, return XGE port 1");
        return OMCI_HAL_UNI_PORT;
    }

    slotNo = ((portInst & 0xff00) >> 8);
    if (slotNo == onuinfo->ethSlot)
    {
        portIdx = ((((portInst & 0xff00) >> 8) - onuinfo->ethSlot) * 8) + (portInst & 0xff);
        portId  = portIdx - 1;//CA_PORT_ID_NI0 -7

        if (portId >= onuinfo->ethNum)
        {
            OMCIHALERR("Don't have more than %d ETH ports, return to ETH port 6", onuinfo->ethNum);
            return OMCI_HAL_UNI_PORT;
        }

        return CA_PORT_ID(CA_PORT_TYPE_ETHERNET, onuinfo->ethPortIdMap[portId]);
    }

    OMCIHALERR("SlotNo %d don't match, but still return port 0x30006", onuinfo->ethSlot);
    return OMCI_HAL_UNI_PORT;
}
#endif


#if DESCRIPTION("Gemport Id and Gemport Index map manage functions")
ca_omci_st_e ca_omci_hal_gem_status_get(ca_omci_uint16_t gemId, ca_omci_bool_t *pAdmin)
{
    ca_omci_uint16_t index = 0;
    ca_omci_uint16_t idx   = 0;

    for (index = 0; index < OMCI_HAL_MAX_GEM_NUM; index++)
    {
        if (gemId == sg_halGemMap[index].gemportId)
        {
            for (idx = 0; idx < OMCI_HAL_MAX_PRI_PER_FLOW; idx++)
            {
                if (0 != sg_halGemMap[index].gemCtl[idx].gemportIdx)
                {
                    *pAdmin = sg_halGemMap[index].gemCtl[idx].admin;
                    OMCIHALDBG("find gem Id[%d] gemIdx[0x%x] admin[%d]", gemId, sg_halGemMap[index].gemCtl[idx].gemportIdx, *pAdmin);
                    return CA_OMCI_OK;
                }
            }
        }
    }

    OMCIHALWARN("can't find gem index for Id[%d]", gemId);
    return CA_OMCI_E_PARAM;
}


void ca_omci_hal_gem_release_gem_idx(ca_omci_uint16_t gemId)
{
    ca_omci_uint16_t idx   = 0;

    for (idx = 0; idx < OMCI_HAL_MAX_GEM_NUM; idx++)
    {
        if (gemId == sg_halGemMap[idx].gemportId)
        {
            OMCI_MEMSET(sg_halGemMap[idx].gemCtl, 0, sizeof(ca_omci_hal_gem_ctl_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
            sg_halGemMap[idx].gemportId = 0;
            OMCIHALDBG("release all gem index for Id[%d]", gemId);
            return;
        }
    }

    OMCIHALERR("can't find gem index for Id[%d]", gemId);
}


ca_omci_uint16_t ca_omci_hal_gem_get_idx_from_id(ca_omci_uint16_t gemId, ca_uint16_t *idxGroup)
{
    ca_omci_uint16_t gemNum = 0;
    ca_omci_uint16_t index  = 0;
    ca_omci_uint16_t idx    = 0;

    for (index = 0; index < OMCI_HAL_MAX_GEM_NUM; index++)
    {
        if (gemId == sg_halGemMap[index].gemportId)
        {
            for (idx = 0; idx < OMCI_HAL_MAX_PRI_PER_FLOW; idx++)
            {
                if (0 != sg_halGemMap[index].gemCtl[idx].gemportIdx)
                {
                    idxGroup[idx] = sg_halGemMap[index].gemCtl[idx].gemportIdx;
                    OMCIHALDBG("find gem index[0x%x] Id[%d]", idxGroup[idx], gemId);
                    gemNum++;
                }
            }
        }
    }

    OMCIHALDBG("Total Num[%d] for gem Id[%d]", gemNum, gemId);
    return gemNum;
}


ca_omci_st_e ca_omci_hal_gem_idx_save(
    ca_omci_gem_flow_t *pGem,
    ca_omci_uint16_t    gemIdx)
{
    ca_omci_uint16_t idx   = 0;
    ca_omci_uint16_t index = 0;

    OMCIHALDBG("Save gemportIdx[0x%x] for gemportId[%d]", gemIdx, pGem->gemId);

    for (idx = 0; idx < OMCI_HAL_MAX_GEM_NUM; idx++)
    {
        if (pGem->gemId == sg_halGemMap[idx].gemportId)
        {
            for (index = 0; index < OMCI_HAL_MAX_PRI_PER_FLOW; index++)
            {
                if (gemIdx == sg_halGemMap[idx].gemCtl[index].gemportIdx)
                {
                    OMCIHALDBG("Already add this gemportIdx[0x%x] for gemportId[%d]",
                                gemIdx, pGem->gemId);
                    return CA_OMCI_OK;
                }
            }
            if (OMCI_HAL_MAX_PRI_PER_FLOW == index)
            {
                for (index = 0; index < OMCI_HAL_MAX_PRI_PER_FLOW; index++)
                {
                    if (0 == sg_halGemMap[idx].gemCtl[index].gemportIdx)
                    {
                        sg_halGemMap[idx].gemCtl[index].gemportIdx = gemIdx;
                        sg_halGemMap[idx].gemCtl[index].admin      = CA_OMCI_TRUE;
                        OMCIHALDBG("Save gemportIdx[0x%x] for gemportId[%d]",
                                    gemIdx, pGem->gemId);
                        return CA_OMCI_OK;
                    }
                }
            }
        }
    }

    if (OMCI_HAL_MAX_GEM_NUM == idx)
    {
        for (idx = 0; idx < OMCI_HAL_MAX_GEM_NUM; idx++)
        {
            if (0 == sg_halGemMap[idx].gemportId)
            {
                sg_halGemMap[idx].gemportId = pGem->gemId;
                for (index = 0; index < OMCI_HAL_MAX_PRI_PER_FLOW; index++)
                {
                    if (0 == sg_halGemMap[idx].gemCtl[index].gemportIdx)
                    {
                        sg_halGemMap[idx].gemCtl[index].gemportIdx = gemIdx;
                        sg_halGemMap[idx].gemCtl[index].admin      = CA_OMCI_TRUE;
                        OMCIHALDBG("Save gemportIdx[0x%x] for gemportId[%d]",
                                    gemIdx, pGem->gemId);
                        return CA_OMCI_OK;
                    }
                }
            }
        }
    }

    OMCIHALERR("Can't Save gemportIdx[0x%x] for gemportId[%d]", gemIdx, pGem->gemId);
    return CA_OMCI_E_PARAM;
}


void ca_omci_hal_gemflow_map_show(const struct shell *cli)
{
    ca_omci_uint16_t  index = 0;
    ca_omci_uint16_t  num   = 0;
    ca_omci_uint16_t  idx = 0;

    for (index = 0; index < OMCI_HAL_MAX_GEM_NUM; index++) {
        if (0 != sg_halGemMap[index].gemportId) {
            for (idx = 0; idx < OMCI_HAL_MAX_PRI_PER_FLOW; idx++) {
                if (CA_OMCI_TRUE == sg_halGemMap[index].gemCtl[idx].admin) {
                    num++;
                }
            }
        }
    }

    if (0 == num) {
        shell_print(cli, "No gemport configure now, please check after OLT configure");
        return;
    }

    shell_print(cli, "======================= Total Flow %d =========================", num);
    for (index = 0; index < OMCI_HAL_MAX_GEM_NUM; index++) {
        if (0 != sg_halGemMap[index].gemportId) {
            for (idx = 0; idx < OMCI_HAL_MAX_PRI_PER_FLOW; idx++) {
                if (CA_OMCI_TRUE == sg_halGemMap[index].gemCtl[idx].admin) {
                    shell_print(cli, "Gemport ID -- [%-4d], Gemport Idx -- [0x%x]", sg_halGemMap[index].gemportId, sg_halGemMap[index].gemCtl[idx].gemportIdx);
                }
            }
        }
    }
}
#endif


#if DESCRIPTION("Get functions:CA API hook layer functions")
ca_omci_st_e ca_omci_hal_pon_cap_get(void *in, void *out)
{
    ca_omci_uint8_t           *ponPort = (ca_omci_uint8_t *)in;
    ca_omci_hal_pon_cap_t     *ponCap  = (ca_omci_hal_pon_cap_t *)out;
    ca_port_id_t               portId  = 0;
    ca_gpon_port_capability_t  capability;

    OMCI_PARAM_NULL_RETURN(in);
    OMCI_PARAM_NULL_RETURN(out);
    OMCI_MEMSET(&capability, 0, sizeof(capability));

    portId = ca_omci_hal_get_port_id(*ponPort);
    if (CA_E_OK != ca_gpon_port_capability_get(OMCI_HAL_DEV_ID, portId, &capability))
    {
        OMCIHALERR("can't get pon port capability");
        return CA_OMCI_E;
    }

    //for Tcont 0 is for OMCC only, so need to adjust the value as below
    ponCap->maxTcNum      = capability.max_tcont_number - 1; //remove omcc tcont 0
    ponCap->maxGemNum     = capability.max_gem_number - capability.queues_per_tcont;
    ponCap->maxQueueNum   = capability.max_queue_number - capability.queues_per_tcont;
    ponCap->reportMode    = capability.report_mode;
    ponCap->maxQueuePerTc = capability.queues_per_tcont;
    OMCIHALDBG("Get capability for pon port [%d][0x%x] tcont[%d], gem[%d], queue[%d], mode[%d], queue number per tcont[%d]",
              *ponPort, portId,ponCap->maxTcNum, ponCap->maxGemNum,
               ponCap->maxQueueNum, ponCap->reportMode, ponCap->maxQueuePerTc);
    if (OMCI_PQ_NUM_PER_TCONT < ponCap->maxQueuePerTc)
    {
        OMCIHALERR("Queue number is larger than 32!");
        ponCap->maxQueuePerTc = OMCI_PQ_NUM_PER_TCONT;
    }

    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("Eth port relate:CA API hook layer functions")
ca_omci_st_e ca_omci_hal_eth_mode_set(void *in, ca_omci_uint8_t mode)
{
    ca_omci_uint16_t      *portInst = (ca_omci_uint16_t *)in;
    ca_uint32_t            portId   = 0;
    ca_eth_port_speed_t    speed    = CA_PORT_SPEED_INVALID;
    ca_omci_onuinfo_obj_t *onuinfo  = ca_omci_onuinfo_get();
    ca_eth_port_fec_t      encoder  = CA_FEC_DISABLE;
    ca_eth_port_fec_t      decoder  = CA_FEC_DISABLE;

    OMCIHALDBG("set eth port[0x%x] mode [%d]", *portInst, mode);
    portId = ca_omci_hal_eth_port_get(*portInst);
    /* 0x48 means disable 25G fec, and 0x8 means enable 25G fec */
    if ((OMCI_OLT_TYPE_ALCL == onuinfo->oltType)
     && (PPTP_ETH_AUTO_DETEC_CFG_25G_ONLY_FULL == (PPTP_ETH_AUTO_DETEC_CFG_25G_ONLY_FULL & mode))) {
        encoder = (0x48 == mode) ? CA_FEC_DISABLE : onuinfo->fecMode;
        mode    = (0x48 == mode) ? PPTP_ETH_AUTO_DETEC_CFG_25G_ONLY_FULL : mode;
        (void) ca_eth_port_fec_enable_set(OMCI_HAL_DEV_ID, portId, encoder, decoder);
        OMCIHALDBG("Set eth 0x%x fec mode %d", portId, encoder);
    }

    speed = ca_omci_hal_eth_speed_get(mode);
    /* Only support UNI speed set, Duplex is not support for 25G UNI now*/
    if (CA_PORT_SPEED_INVALID != speed)
    {
        if (CA_E_OK != ca_eth_port_speed_set(OMCI_HAL_DEV_ID, portId, speed))
        {
            OMCIHALERR("Set eth speed failed");
            return CA_OMCI_E;
        }
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_eth_mode_get(void *in, void *out)
{
    ca_omci_uint16_t    *portInst = (ca_omci_uint16_t *)in;
    ca_omci_uint8_t     *mode     = (ca_omci_uint8_t *)out;
    ca_uint32_t          portId   = 0;
    ca_eth_port_speed_t  speed    = CA_PORT_SPEED_INVALID;
    ca_status_t          ret      = CA_E_OK;

    portId  = ca_omci_hal_eth_port_get(*portInst);
    ret     = ca_eth_port_speed_get(OMCI_HAL_DEV_ID, portId, &speed);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get port[0x%x] speed failed", portId);
        return CA_OMCI_E;
    }

    if (CA_PORT_SPEED_SFI_25G == speed)
    {
        *mode = PPTP_ETH_CFG_IND_25G_ETH_FULL;
    }

    if (CA_PORT_SPEED_5G == speed)
    {
        *mode = PPTP_ETH_CFG_IND_5G_ETH_FULL;
    }

    if (CA_PORT_SPEED_2_5G == speed)
    {
        *mode = PPTP_ETH_CFG_IND_2G5_ETH_FULL;
    }

    if (CA_PORT_SPEED_SFI_10G == speed)
    {
        *mode = PPTP_ETH_CFG_IND_10G_ETH_FULL;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_eth_lb_set(void *in, ca_omci_uint8_t loopback)
{
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_omci_uint16_t *portInst = (ca_omci_uint16_t *)in;
    ca_uint32_t       portId   = 0;
    ca_boolean_t      mode     = FALSE;

    HAL_CHECK_TCL;
    OMCIHALDBG("set eth port[0x%x] loopback [%d]", *portInst, loopback);

    portId = ca_omci_hal_eth_port_get(*portInst);
    if (0 != loopback)
    {
        mode = TRUE;
    }

    if (CA_E_OK != ca_gpon_port_gem_port_uni_loopback_enable_set(OMCI_HAL_DEV_ID, portId, 0, 0, mode, mode))
    {
        OMCIHALERR("call ca_gpon_port_gem_port_uni_loopback_set failed");
        return CA_OMCI_E;
    }
#endif
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_eth_admin_set(void *in, ca_omci_uint8_t admin)
{
    ca_omci_uint16_t *portInst = (ca_omci_uint16_t *)in;
    ca_uint32_t       portId   = 0;
    ca_boolean_t      enable   = CA_OMCI_FALSE;
    ca_boolean_t      oriFlag  = CA_OMCI_FALSE;
    ca_status_t       ret      = CA_E_OK;

    HAL_CHECK_TCL;
    OMCIHALDBG("set eth port[0x%x] admin [%s]", *portInst, ((admin == CA_OMCI_LOCK) ? "lock" : "unlock"));
    portId = ca_omci_hal_eth_port_get(*portInst);
    enable = (admin == CA_OMCI_LOCK) ? CA_OMCI_FALSE : CA_OMCI_TRUE;
    ret    = ca_port_enable_get(OMCI_HAL_DEV_ID, portId, &oriFlag);
    if ((CA_E_OK == ret) && (enable != oriFlag))
    {
        OMCIHALDBG("set eth port[0x%x] admin [%s]", *portInst, ((admin == CA_OMCI_LOCK) ? "lock" : "unlock"));
        ret = ca_port_enable_set(OMCI_HAL_DEV_ID, portId, enable);
        if (CA_E_OK != ret)
        {
            OMCIHALERR("set eth port[0x%x] admin [%s] failed [%d]",
                       *portInst, ((admin == CA_OMCI_LOCK) ? "lock" : "unlock"), ret);
            return CA_OMCI_E;
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_eth_max_frame_set(void *in, ca_omci_uint16_t size)
{
    ca_omci_uint16_t *portInst  = (ca_omci_uint16_t *)in;
    ca_uint32_t       portId    = 0;
    ca_uint32_t       frameSize = size;
    ca_status_t       ret       = CA_E_OK;

    HAL_CHECK_TCL;
    OMCIHALDBG("set eth port[0x%x] max frame size [%d]", *portInst, size);
    portId = ca_omci_hal_eth_port_get(*portInst);
    ret    = ca_port_max_frame_size_set(OMCI_HAL_DEV_ID, portId, frameSize, 1);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set eth port[0x%x] max frame size[%d] failed ret[%d]", portId, frameSize, ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_eth_state_get(void *in, void *out)
{
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_omci_uint16_t          *portInst  = (ca_omci_uint16_t *)in;
    ca_omci_bool_t            *linkState = (ca_omci_bool_t *)out;
    ca_port_id_t               portId    = 0;
    ca_eth_port_link_status_t  status    = CA_PORT_LINK_DOWN;
    ca_status_t                ret       = CA_E_OK;

    HAL_CHECK_TCL;
    portId = ca_omci_hal_eth_port_get(*portInst);
    ret    = ca_eth_port_link_status_get(OMCI_HAL_DEV_ID, portId, &status);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get port 0x%x ca_port 0x%x link status failed!", *portInst, portId);
        return CA_OMCI_E;
    }

    *linkState = (CA_PORT_LINK_DOWN == status) ? CA_OMCI_FALSE : CA_OMCI_TRUE;
    OMCIHALDBG("Get port 0x%x link status %s!", *portInst, (CA_OMCI_FALSE == *linkState) ? "down" : "up");
#endif
    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("Gemflow functions:CA API hook layer functions")
ca_omci_st_e ca_omci_hal_tcont_reset(void *in, void *pData)
{
    ca_omci_uint8_t                *portNo  = (ca_omci_uint8_t *)in;
    ca_omci_tcont_cfg_t            *val     = (ca_omci_tcont_cfg_t *)pData;
    ca_uint32_t                     tcIdx   = 0;
    ca_gpon_port_tcont_scheduler_t  tcCfg   = {0};
    ca_status_t                     ret     = CA_E_OK;

    OMCIHALDBG("reset pon[%d] tcont", *portNo);
    //ca_omci_hal_dbg_print_tcont(val);

    OMCI_MEMSET(&tcCfg, 0, sizeof(ca_gpon_port_tcont_scheduler_t));
    tcIdx              = ca_omci_hal_tcont_idx_get(val->tcontId);
    tcCfg.mode         = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_SP;
    tcCfg.sp_queue_num = OMCI_PQ_NUM_PER_TCONT;

    ret = ca_gpon_port_tcont_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, val->allocId,
                                 tcIdx, CA_OMCI_FALSE, &tcCfg);

    if (CA_E_OK != ret)
    {
        OMCIHALERR("reset tcont failed");
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_tcont_set(void *in, void *pData)
{
    ca_omci_uint8_t                *portNo  = (ca_omci_uint8_t *)in;
    ca_omci_tcont_cfg_t            *val     = (ca_omci_tcont_cfg_t *)pData;
    ca_uint32_t                     tcIdx   = 0;
    ca_uint32_t                     idx     = 0;
    ca_gpon_port_tcont_scheduler_t  tcCfg   = {0};
    ca_omci_onuinfo_obj_t          *onuinfo = ca_omci_onuinfo_get();

    OMCIHALDBG("cfg pon[%d] tcont", *portNo);
    //ca_omci_hal_dbg_print_tcont(val);

    OMCI_MEMSET(&tcCfg, 0, sizeof(ca_gpon_port_tcont_scheduler_t));
    tcIdx = ca_omci_hal_tcont_idx_get(val->tcontId);

    //check wrrQue value not policy, ALCL OLT don't set tcont policy
    //Other OLT will set the mode, and WRR value too
    //It's OK for OMCI to check WRR value not the policy
    for (idx = 0; idx < onuinfo->maxQueuePerTc; idx++)
    {
        tcCfg.weight_queue[idx] = (1 == val->wrrQue[idx]) ? 0 : val->wrrQue[idx];

        if (1 == val->wrrQue[idx])
        {
            tcCfg.sp_queue_num++;
        }
    }

    if (onuinfo->maxQueuePerTc == tcCfg.sp_queue_num)
    {
        tcCfg.mode         = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_SP;
        tcCfg.sp_queue_num = onuinfo->maxQueuePerTc;
    }
    else if (0 == tcCfg.sp_queue_num)
    {
        tcCfg.mode = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_WRR;
    }
    else
    {
        tcCfg.mode = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_SP_WRR;
    }

    if (CA_E_OK != ca_gpon_port_tcont_set(OMCI_HAL_DEV_ID,
                                          OMCI_HAL_WAN_PORT,
                                          val->allocId,
                                          tcIdx,
                                          CA_OMCI_TRUE,
                                          &tcCfg))
    {
        OMCIHALERR("set tcont failed");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_tcont_restore(void *in, void *pData)
{
    ca_omci_uint8_t                *portNo  = (ca_omci_uint8_t *)in;
    ca_omci_tcont_cfg_t            *val     = (ca_omci_tcont_cfg_t *)pData;
    ca_uint32_t                     tcIdx   = 0;
    ca_uint32_t                     idx     = 0;
    ca_gpon_port_tcont_scheduler_t  tcCfg   = {0};
    ca_omci_onuinfo_obj_t          *onuinfo = ca_omci_onuinfo_get();
    ca_uint32_t                     oriTcId = 0;
    ca_boolean_t                    tcFlag  = CA_OMCI_FALSE;
    ca_gpon_port_tcont_scheduler_t  oriCfg  = {0};

    OMCIHALDBG("cfg pon[%d] tcont", *portNo);
    //ca_omci_hal_dbg_print_tcont(val);

    OMCI_MEMSET(&tcCfg, 0, sizeof(ca_gpon_port_tcont_scheduler_t));
    tcIdx = ca_omci_hal_tcont_idx_get(val->tcontId);

    //check wrrQue value not policy, ALCL OLT don't set tcont policy
    //Other OLT will set the mode, and WRR value too
    //It's OK for OMCI to check WRR value not the policy
    for (idx = 0; idx < onuinfo->maxQueuePerTc; idx++)
    {
        tcCfg.weight_queue[idx] = (1 == val->wrrQue[idx]) ? 0 : val->wrrQue[idx];

        if (1 == val->wrrQue[idx])
        {
            tcCfg.sp_queue_num++;
        }
    }

    if (onuinfo->maxQueuePerTc == tcCfg.sp_queue_num)
    {
        tcCfg.mode         = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_SP;
        tcCfg.sp_queue_num = onuinfo->maxQueuePerTc;
    }
    else if (0 == tcCfg.sp_queue_num)
    {
        tcCfg.mode = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_WRR;
    }
    else
    {
        tcCfg.mode = CA_GPON_PORT_TCONT_QUEUE_SCHEDULER_SP_WRR;
    }

    ca_gpon_port_tcont_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, val->allocId, &oriTcId, &tcFlag, &oriCfg);
    if (CA_OMCI_TRUE == tcFlag)
        return CA_OMCI_OK;

    if (CA_E_OK != ca_gpon_port_tcont_set(OMCI_HAL_DEV_ID,
                                          OMCI_HAL_WAN_PORT,
                                          val->allocId,
                                          tcIdx,
                                          CA_OMCI_TRUE,
                                          &tcCfg))
    {
        OMCIHALERR("set tcont failed");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_gemflow_add(void *in)
{
    ca_omci_onuinfo_obj_t   *onuinfo  = ca_omci_onuinfo_get();
    ca_omci_gem_flow_t      *pGemFlow = (ca_omci_gem_flow_t *)in;
    ca_gpon_port_gem_port_t  gemCfg   = {0};
    ca_uint32_t              pri      = 0;
    ca_uint32_t              queueId  = 0;
    ca_uint32_t              weight   = 0;
    ca_status_t              ret      = CA_E_OK;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(pGemFlow);
    //ca_omci_hal_dbg_print_gemflow(pGemFlow);

    OMCI_MEMSET(&gemCfg, 0, sizeof(ca_gpon_port_gem_port_t));
    //gemCfg.valid        = CA_OMCI_TRUE;
    gemCfg.gem_port_id  = pGemFlow->gemId;
    gemCfg.tcont_id     = ca_omci_hal_tcont_idx_get(pGemFlow->tcontId);
    gemCfg.direction    = pGemFlow->direction - 1;//omci is 1-3, hal is 0 -2
    gemCfg.up_queue     = ca_omci_hal_us_pq_idx_get(gemCfg.tcont_id, pGemFlow->usQueue);
    gemCfg.dn_queue     = ca_omci_hal_ds_pq_idx_get(pGemFlow->dsQueue);
    gemCfg.aes_state    = pGemFlow->aesState;
    gemCfg.aes_key_ring = pGemFlow->aesKeyRing;

    /*Check usTrafficDes and dsTrafficDes*/
    if ((0 != pGemFlow->usTrafficDes.cir) || (0 != pGemFlow->usTrafficDes.cbs)
     || (0 != pGemFlow->usTrafficDes.pir) || (0 != pGemFlow->usTrafficDes.pbs)){
        gemCfg.up_traffic_descriptor.mode = CA_POLICER_MODE_RATE_ONLY;
        gemCfg.up_traffic_descriptor.cir  = pGemFlow->usTrafficDes.cir * 8 / 1000;
        gemCfg.up_traffic_descriptor.cbs  = pGemFlow->usTrafficDes.cbs * 8 / 1000;
        gemCfg.up_traffic_descriptor.pir  = pGemFlow->usTrafficDes.pir * 8 / 1000;
        gemCfg.up_traffic_descriptor.pbs  = pGemFlow->usTrafficDes.pbs * 8 / 1000;
        OMCIHALDBG("USTrafficDes %d cir %d cbs %d pir %d pbs %d", gemCfg.up_traffic_descriptor.mode,
                   gemCfg.up_traffic_descriptor.cir, gemCfg.up_traffic_descriptor.cbs,
                   gemCfg.up_traffic_descriptor.pir, gemCfg.up_traffic_descriptor.pbs);
    }

    if ((0 != pGemFlow->dsTrafficDes.cir) || (0 != pGemFlow->dsTrafficDes.cbs)
     || (0 != pGemFlow->dsTrafficDes.pir) || (0 != pGemFlow->dsTrafficDes.pbs)){
        gemCfg.dn_traffic_descriptor.mode = CA_POLICER_MODE_RATE_ONLY;
        gemCfg.dn_traffic_descriptor.cir  = pGemFlow->dsTrafficDes.cir * 8 / 1000;
        gemCfg.dn_traffic_descriptor.cbs  = pGemFlow->dsTrafficDes.cbs * 8 / 1000;
        gemCfg.dn_traffic_descriptor.pir  = pGemFlow->dsTrafficDes.pir * 8 / 1000;
        gemCfg.dn_traffic_descriptor.pbs  = pGemFlow->dsTrafficDes.pbs * 8 / 1000;
        OMCIHALDBG("DSTrafficDes %d cir %d cbs %d pir %d pbs %d", gemCfg.dn_traffic_descriptor.mode,
                   gemCfg.dn_traffic_descriptor.cir, gemCfg.dn_traffic_descriptor.cbs,
                   gemCfg.dn_traffic_descriptor.pir, gemCfg.dn_traffic_descriptor.pbs);
    }

    if ((OMCI_OLT_TYPE_ALCL == onuinfo->oltType) && (OMCI_ISP_TYPE_ORANGE != onuinfo->ispType)) {
        OMCIHALDBG("Don't enable us aes for ALCL");
        gemCfg.aes_key_ring = (pGemFlow->aesKeyRing == GEM_NET_CTP_ENCRYPT_UNICAST_BI) ? CA_GPON_PORT_GEM_PORT_ENC_UC_DS_ONLY : pGemFlow->aesKeyRing;
    }

    if (GEM_NET_CTP_DIR_DS == pGemFlow->direction) {
        gemCfg.tcont_id                   = 0;
        gemCfg.traffic_type_fwd_control   = (CA_OMCI_TRUE == pGemFlow->mcFlag) ? 0x6 : 0;  /* bit2 multicast GEM, UMC drop by default */
        gemCfg.up_traffic_descriptor.mode = CA_POLICER_MODE_DISABLE;
    }

    pri     = ca_omci_hal_us_pq_pri_get(gemCfg.tcont_id, pGemFlow->usQueue);
    queueId = ca_omci_hal_us_pq_idx_get(gemCfg.tcont_id, pGemFlow->usQueue);
    if (pGemFlow->usQueue == pGemFlow->tcontId)
    {
        //rate mode, use highest priority queue
        pri      = onuinfo->maxQueuePerTc - 1;
        queueId  = onuinfo->maxQueuePerTc - 1 + (((gemCfg.tcont_id - 1) & 0xff) * onuinfo->maxQueuePerTc);
    }

    //traffic scheduler entity id equals to tcont entity id
    //set scheduler to ffff to not cfg this function yet, need to change it later
    weight = (1 == pGemFlow->usQueueWeight) ? 0 : pGemFlow->usQueueWeight;
    if (queueId < OMCI_HAL_MAX_QUEUE_NUM && GEM_NET_CTP_DIR_DS != pGemFlow->direction) {
        if (0 == sg_queueUsedNum[queueId]) {
            ret = ca_gpon_port_upstream_queue_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, gemCfg.up_queue, CA_OMCI_TRUE,
                                                  gemCfg.tcont_id, 0xffff, weight, pri);
            if ((CA_E_OK != ret) && (CA_E_EXISTS != ret)) {
                OMCIHALERR("set up stream queue failed for tcont[%d] queue[%d] ret[%d]",
                            gemCfg.tcont_id, queueId, ret);
                return CA_OMCI_E;
            }
        }
        sg_queueUsedNum[queueId]++;
    }

    ret = ca_gpon_port_gem_port_add(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &gemCfg);
    if (CA_E_OK != ret) {
        OMCIHALERR("add gemflow for gemport[%d] dir[%d] tcont[%d] us-queue[%d] failed ret[%d]",
                    gemCfg.gem_port_id, gemCfg.direction, gemCfg.tcont_id,
                    gemCfg.up_queue, ret);
        return CA_OMCI_E;
    }

    //save the gemport index
    ca_omci_hal_gem_idx_save(pGemFlow, gemCfg.gem_index);
    ca_mdelay(1);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_gemflow_del(void *in)
{
    ca_omci_gem_flow_t *pGemFlow = (ca_omci_gem_flow_t *)in;
    ca_uint32_t         tcontId  = 0;
    ca_uint32_t         usQueue  = 0;
    ca_uint32_t         pri      = 0;
    ca_status_t         ret      = CA_E_OK;
    ca_uint16_t         idx      = 0;
    ca_uint32_t         weight   = 0;
    ca_uint16_t         gemNum   = 0;
    ca_uint16_t         gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(pGemFlow);
    //ca_omci_hal_dbg_print_gemflow(pGemFlow);

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
    gemNum = ca_omci_hal_gem_get_idx_from_id(pGemFlow->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    //then remove gemport
    ret = ca_gpon_port_gem_port_delete(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, gemIdx[idx]);
    if ((CA_E_OK != ret) && (CA_E_NOT_FOUND != ret))
    {
        OMCIHALERR("del gemflow[%d] failed ret[%d]", pGemFlow->gemId, ret);
        return CA_OMCI_E;
    }

    ca_omci_hal_gem_release_gem_idx(pGemFlow->gemId);

    //remove pri queue first
    if (GEM_NET_CTP_DIR_DS != pGemFlow->direction)
    {
        tcontId = ca_omci_hal_tcont_idx_get(pGemFlow->tcontId);
        usQueue = ca_omci_hal_us_pq_idx_get(tcontId, pGemFlow->usQueue);
        pri     = ca_omci_hal_us_pq_pri_get(tcontId, pGemFlow->usQueue);

        ca_mdelay(1);
        if (usQueue < OMCI_HAL_MAX_QUEUE_NUM)
        {
            if (0 != sg_queueUsedNum[usQueue])
                sg_queueUsedNum[usQueue]--;

            if (0 == sg_queueUsedNum[usQueue])
            {
                weight = (1 == pGemFlow->usQueueWeight) ? 0 : pGemFlow->usQueueWeight;
                ret    = ca_gpon_port_upstream_queue_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, usQueue, CA_OMCI_FALSE,
                                                         tcontId, 0xffff, weight, pri);
                if ((CA_E_OK != ret) && (CA_E_NOT_FOUND != ret))
                {
                    OMCIHALERR("release up stream queue failed for tcont[%d] queue[%d] ret[%d]",
                                tcontId, usQueue, ret);
                }
            }
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_gem_keyring_set(void *pData, void *keyRing)
{
    ca_omci_onuinfo_obj_t   *onuinfo   = ca_omci_onuinfo_get();
    ca_omci_uint16_t        *gemportId = (ca_omci_uint16_t *)pData;
    ca_omci_uint8_t         *aesDir    = (ca_omci_uint8_t *)keyRing;
    ca_omci_uint8_t          idx       = 0;
    ca_omci_uint16_t         gemNum    = 0;
    ca_gpon_port_gem_port_t  gemCfg    = {0};
    ca_status_t              ret       = CA_E_OK;
    ca_uint16_t              gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
    gemNum = ca_omci_hal_gem_get_idx_from_id(*gemportId, gemIdx);
    if (0 == gemNum)
    {
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    gemCfg.gem_index = gemIdx[idx];
    ret = ca_gpon_port_gem_port_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &gemCfg);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get gemId[0x%x] idx[0x%x] ret[%d]", *gemportId, gemIdx[idx], ret);
        return CA_OMCI_E;
    }

    if (*aesDir == gemCfg.aes_key_ring)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Set gemId[0x%x] idx[0x%x] key ring[%d]", *gemportId, gemIdx[idx], *aesDir);
    gemCfg.aes_key_ring = *aesDir;
    if ((OMCI_OLT_TYPE_ALCL == onuinfo->oltType) && (OMCI_ISP_TYPE_ORANGE != onuinfo->ispType))
    {
        OMCIHALDBG("Don't enable us aes for ALCL");
        gemCfg.aes_key_ring = (gemCfg.aes_key_ring == GEM_NET_CTP_ENCRYPT_UNICAST_BI) ? CA_GPON_PORT_GEM_PORT_ENC_UC_DS_ONLY : gemCfg.aes_key_ring;
    }

    ret = ca_gpon_port_gem_port_set(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &gemCfg);
    if (CA_E_OK != ret)
    {
        OMCIHALDBG("Set gemId[0x%x] idx[0x%x] key ring[%d] ret %d", *gemportId, gemIdx[idx], *aesDir, ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("Stream relate functions")
/*Just update the dscp map table in hal layer and use for classifier create*/
ca_omci_st_e ca_omci_hal_dscp_map_set(ca_omci_uint8_t *pData)
{
    ca_uint8_t idx = 0;

    for (idx = 0; idx < 8; idx++){
        sg_dscpMapTbl[idx * 8 + 0] = (pData[idx * 3 + 0] >> 5) & 0x07;
        sg_dscpMapTbl[idx * 8 + 1] = (pData[idx * 3 + 0] >> 2) & 0x07;
        sg_dscpMapTbl[idx * 8 + 2] = ((pData[idx * 3 + 0] & 3) << 1) | (pData[idx * 3 + 1] >> 7);
        sg_dscpMapTbl[idx * 8 + 3] = (pData[idx * 3 + 1] >> 4) & 0x07;
        sg_dscpMapTbl[idx * 8 + 4] = (pData[idx * 3 + 1] >> 1) & 0x07;
        sg_dscpMapTbl[idx * 8 + 5] = ((pData[idx * 3 + 1] & 1) << 2) | (pData[idx * 3 + 2] >> 6);
        sg_dscpMapTbl[idx * 8 + 6] = (pData[idx * 3 + 2] >> 3) & 0x07;
        sg_dscpMapTbl[idx * 8 + 7] = (pData[idx * 3 + 2] >> 0) & 0x07;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_queue_set(
    ca_omci_stream_cfg_t *pData,
    ca_omci_bool_t        admin)
{
    ca_port_id_t              dstPort = 0;
    ca_status_t               ret     = CA_E_OK;
    ca_queue_schedule_mode_t  qMode   = CA_SCHEDULE_STRICT_PRIORITY;
    ca_queue_weights_t        weights = {0};
    ca_uint32_t               pdIdx   = 0;
    ca_omci_uint8_t           idx     = 0;

    if (OMCI_CFG_STREAM_PORT_ETH != pData->portType)
    {
        OMCIHALWARN("Don't set downstream queue when port type is not eth");
        return CA_OMCI_OK;
    }

    dstPort = ca_omci_hal_eth_port_get(pData->uniPortId);
    pdIdx   = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    ret     = ca_queue_schedule_get(OMCI_HAL_DEV_ID, dstPort, &qMode, &weights);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("ca_queue_schedule_get failed for port 0x%x, ret %d", dstPort, ret);
        return CA_OMCI_E;
    }

    if (CA_OMCI_FALSE == admin)
    {
        if (CA_SCHEDULE_STRICT_PRIORITY == qMode)
        {
            return CA_OMCI_OK;
        }

        ret = ca_queue_schedule_set(OMCI_HAL_DEV_ID, dstPort, qMode, &weights);
        if (CA_E_OK != ret)
        {
            OMCIHALERR("reset port 0x%x dsQueue failed ret %d", dstPort, ret);
            return CA_OMCI_E;
        }

        OMCIHALDBG("reset port 0x%x dsQueue", dstPort);
        return CA_OMCI_OK;
    }

    qMode                  = (1 != pData->dsQueueWeight) ? CA_SCHEDULE_DEFICIT_WEIGHT_ROUND_ROBIN : qMode;
    weights.weights[pdIdx] = (1 == pData->dsQueueWeight) ? 0 : pData->dsQueueWeight;
    OMCIHALDBG("Set port 0x%x pdIdx[%d] mode %s weight[%d]",
               dstPort, pdIdx, qMode == CA_SCHEDULE_STRICT_PRIORITY ? "SP" : "WRR", weights.weights[pdIdx]);

    if (0 != weights.weights[pdIdx])
    {
        //check the weights before this pq
        for (idx = 0; idx < pdIdx; idx++)
        {
            weights.weights[idx] = (0 == weights.weights[idx]) ? 1 : weights.weights[idx];
             OMCIHALDBG("Update pqIdx %d weight to 1 it must be set later", idx);
        }
    }

    ret = ca_queue_schedule_set(OMCI_HAL_DEV_ID, dstPort, qMode, &weights);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set port 0x%x pdIdx[%d] mode %s weight[%d] failed ret %d",
                   dstPort, pdIdx, qMode == CA_SCHEDULE_STRICT_PRIORITY ? "SP" : "WRR",
                   weights.weights[pdIdx], ret);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_classifier_reset(ca_omci_stream_cfg_t *pUsFlow)
{
    ca_omci_uint32_t *ruleIdx = NULL;
    ca_omci_uint8_t   idx     = 0;

    ruleIdx = ca_omci_hal_flow_classifier_idx_get(pUsFlow);
    if (0 == ruleIdx)
    {
        OMCIWARN("don't remove not exist flow");
        return CA_OMCI_OK;
    }

    for (idx = 0; idx < OMCI_HAL_MAX_RULE_PER_FLOW_US; idx++)
    {
        if (0 == ruleIdx[idx])
        {
            continue;
        }
#if !defined(CONFIG_CA_OMCI_FPGA)
        if (CA_E_OK != ca_classifier_rule_delete(OMCI_HAL_DEV_ID, ruleIdx[idx]))
        {
            OMCIHALERR("remove classifier failed");
        }
#endif
        OMCIHALDBG("remove rule %d for gem %d, uni %d, type %d",
                    ruleIdx[idx], pUsFlow->gemId, pUsFlow->uniPortId, pUsFlow->portType);
        ruleIdx[idx] = 0;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_untag_del(
    ca_omci_stream_cfg_t   *pUsFlow,
    ca_omci_untag_filter_t *pUntag,
    ca_omci_uint16_t        type)
{
    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    ca_omci_hal_us_flow_classifier_reset(pUsFlow);
    OMCIHALDBG("delete untag class rule");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_sgltag_del(
    ca_omci_stream_cfg_t     *pUsFlow,
    ca_omci_sgl_tag_filter_t *pSgltag)
{
    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //reset all classifier rules
    ca_omci_hal_us_flow_classifier_reset(pUsFlow);
    OMCIHALDBG("delete single tag class rule");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_dbltag_del(
    ca_omci_stream_cfg_t     *pUsFlow,
    ca_omci_dbl_tag_filter_t *pDbltag)
{
    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //reset all classifier rules
    ca_omci_hal_us_flow_classifier_reset(pUsFlow);
    OMCIHALDBG("delete double tag class rule");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_classifier_reset(ca_omci_stream_cfg_t *pData)
{
    ca_omci_uint32_t *ruleIdx = NULL;
    ca_omci_uint8_t   idx     = 0;

    ruleIdx = ca_omci_hal_flow_ds_classifier_idx_get(pData);
    if (NULL == ruleIdx)
    {
        OMCIWARN("No classifier to remove");
        return CA_OMCI_OK;
    }

    for (idx = 0; idx < OMCI_HAL_MAX_RULE_PER_FLOW; idx++)
    {
        if (0 == ruleIdx[idx])
        {
            continue;
        }
#if !defined(CONFIG_CA_OMCI_FPGA)
        if (CA_E_OK != ca_classifier_rule_delete(OMCI_HAL_DEV_ID, ruleIdx[idx]))
        {
           OMCIHALERR("remove classifier failed");
        }
#endif
        OMCIHALDBG("remove rule %d for gem %d, uni %d, type %d",
                    ruleIdx[idx], pData->gemId, pData->uniPortId, pData->portType);

        ruleIdx[idx] = 0;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_alcl_def_untag(
    ca_omci_stream_cfg_t   *pData,
    ca_omci_untag_filter_t *pUntag)
{
    ca_classifier_key_t       key       = {0};
    ca_classifier_key_mask_t  keyMask   = {0};
    ca_classifier_action_t    action    = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority  = 1;
#endif
    ca_uint32_t               ruleIdx   = 0;
    ca_status_t               ret       = CA_E_OK;
    ca_uint16_t               idx       = 0;
    ca_uint16_t               gemNum    = 0;
    ca_omci_onuinfo_obj_t    *onuInfo   = ca_omci_onuinfo_get();
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];
    ca_omci_uint8_t           mac[OMCI_MAC_ADDR_LEN];

    OMCI_MEMSET(&key,       0, sizeof(key));
    OMCI_MEMSET(&keyMask,   0, sizeof(keyMask));
    OMCI_MEMSET(&action,    0, sizeof(action));
    OMCI_MEMSET(gemIdx,     0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
    OMCI_MEMSET(mac,        0, sizeof(ca_omci_uint8_t) * OMCI_MAC_ADDR_LEN);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pData->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                        = OMCI_HAL_WAN_PORT;
    key.key_handle.gem_index            = gemIdx[idx];
    key.handle_type                     = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    keyMask.src_port                    = 1;
    keyMask.l2_mask.vlan_count          = 1;
    keyMask.l2                          = 1;
    keyMask.key_handle                  = 1;
    action.forward                      = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                    = ca_omci_hal_eth_port_get(pData->uniPortId);
    action.options.priority             = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    action.options.masks.priority       = 1;

    if (OMCI_CFG_STREAM_PORT_IPHOST == pData->portType)
    {
        //base mac + port id is the eth MAC
        OMCI_MEMCPY(mac, onuInfo->mac, OMCI_MAC_ADDR_LEN);
        mac[5] = mac[5] + pData->uniPortId;

        //OMCI_MEMCPY(&key.l2.mac_da.mac_max, mac, OMCI_MAC_ADDR_LEN);
        //OMCI_MEMCPY(&key.l2.mac_da.mac_min, mac, OMCI_MAC_ADDR_LEN);
        //keyMask.l2_mask.mac_da = 1;
        action.forward         = CA_CLASSIFIER_FORWARD_PORT;
        action.dest.port       = OMCI_HAL_CPU_PORT + pData->uniPortId;
    }

    if (CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pUntag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pUntag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pUntag->innerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pUntag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        action.options.priority             = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;
        OMCIHALDBG("Remove vid[%d] pri[%d]", key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri);
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = 0;
        key.l2.vlan_otag.vlan_max.pri       = 0;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        OMCIHALDBG("Remove vid[%d]", key.l2.vlan_otag.vlan_min.vid);
    }

    if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PRI_TAG == pUntag->dsTagAction)
    {
        action.options.outer_vlan_act = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid      = 0;
        OMCIHALDBG("ALCL need to swap PVID to PTAG");
    }

    if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PASS_TAG == pUntag->dsTagAction)
    {
        if (0 == key.l2.vlan_otag.vlan_min.vid)
        {
            OMCIHALDBG("Don't know the original vlan, just ignore this rule");
            return CA_OMCI_OK;
        }
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_NOP;
        action.options.masks.outer_vlan_act = 0;
    }

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set untag failed ret[%d] to port[0x%x]", ret, key.dest_port);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
    {
        OMCIHALERR("can't save idx for ds stream");
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t        type)
{
    ca_omci_stream_cfg_t     *pData    = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t   *pUntag   = (ca_omci_untag_filter_t *)rule;
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 1;
#endif
    ca_uint32_t               ruleIdx  = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    /* For Some OLT, default rule only have 2 kinds, drop or add tag.
          Don't need to seprate it to diff tags */
    if (OMCI_HAL_ETH_TYPE_DEF == type)
    {
        if (CA_OMCI_UNTAG_FWD_DROP == pUntag->fwdType)
        {
            //set drop all packet in default drop set, so just return
             return CA_OMCI_OK;
        }

        if ((CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType)
         || (CA_OMCI_UNTAG_FWD_TRANSPARENT == pUntag->fwdType)
         || (CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType))
        {
            //set pvid to the port
            return ca_omci_hal_ds_alcl_def_untag(pData, pUntag);
        }

        return CA_OMCI_OK;
    }

    OMCI_MEMSET(&key,       0, sizeof(key));
    OMCI_MEMSET(&keyMask,   0, sizeof(keyMask));
    OMCI_MEMSET(&action,    0, sizeof(action));
    OMCI_MEMSET(gemIdx,     0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pData->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                  = OMCI_HAL_WAN_PORT;
    key.handle_type               = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    key.l2.vlan_count             = 0;
    key.key_handle.gem_index      = gemIdx[idx];
    keyMask.src_port              = 1;
    keyMask.l2_mask.vlan_count    = 1;
    keyMask.l2                    = 1;
    keyMask.key_handle            = 1;
    action.forward                = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port              = ca_omci_hal_eth_port_get(pData->uniPortId);
    action.options.priority       = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    action.options.masks.priority = 1;

    if (OMCI_HAL_ETH_TYPE_DEF != type)
    {
        key.l2.ethertype          = type;
        keyMask.l2_mask.ethertype = 1;
    }
    OMCIHALDBG("Set untag ds-stream for gem[0x%x] filter eth[0x%x]",
                gemIdx[idx], key.l2.ethertype);

    if (CA_OMCI_UNTAG_FWD_TRANSPARENT == pUntag->fwdType)
    {
        OMCIHALDBG("Transparent");
    }
    else if (CA_OMCI_UNTAG_FWD_DROP == pUntag->fwdType)
    {
        action.forward = CA_CLASSIFIER_FORWARD_DENY;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType)
    {
        if (0 != pData->filterPri && 0xff != pData->filterPri)
        {
            if (0 == ((1 << pUntag->innerVlanTreatment.pri) &  pData->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pUntag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pUntag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pUntag->innerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pUntag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        action.options.priority             = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;

        OMCIHALDBG("Remove vid[%d] pri[%d]", key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri);
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_PRI == pUntag->fwdType)
    {
        key.l2.vlan_count                   = 2;
        key.l2.vlan_otag.vlan_min.vid       = pUntag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pUntag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pUntag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pUntag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.inner_vlan_act = 1;
        action.options.priority             = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;
        OMCIHALDBG("Rm ovid[%d] opri[%d] ivid[%d] ipri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri);
    }
    else
    {
        if (CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType)
        {
            key.l2.vlan_count                   = 1;
            key.l2.vlan_otag.vlan_min.vid       = pUntag->innerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pUntag->innerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;

            OMCIHALDBG("Rm vid[%d] pri-dscp", key.l2.vlan_otag.vlan_min.vid);
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_INN_DSCP_PRI == pUntag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pUntag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pUntag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_min.pri       = pUntag->outerVlanTreatment.pri;
            key.l2.vlan_otag.vlan_max.pri       = pUntag->outerVlanTreatment.pri;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Rm ovid[%d] opri[%d] ivid[%d] ipri-dscp",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                        key.l2.vlan_itag.vlan_min.vid);
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_OUT_DSCP_PRI == pUntag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pUntag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pUntag->outerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Rm ovid[%d] opri-dscp ivid[%d] ipri[%d]",
                        key.l2.vlan_otag.vlan_min.vid,
                        key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri);
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_DSCP_PRI == pUntag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pUntag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pUntag->outerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.inner_vlan_act = 1;
            OMCIHALDBG("Rm ovid[%d] opri-dscp ivid[%d] ipri-dscp",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_itag.vlan_min.vid);
        }
    }

    if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PRI_TAG == pUntag->dsTagAction)
    {
        action.options.outer_vlan_act = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid      = 0;
        OMCIHALDBG("ALCL need to swap PVID to PTAG");
    }

    if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PASS_TAG == pUntag->dsTagAction)
    {
        if (0 == key.l2.vlan_otag.vlan_min.vid)
        {
            OMCIHALDBG("Don't know the original vlan, just ignore this rule");
            return CA_OMCI_OK;
        }
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_NOP;
        action.options.masks.outer_vlan_act = 0;
    }

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set untag filter eth[0x%x] failed ret[%d] to port[0x%x]",
                   type, ret, key.dest_port);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
    {
        OMCIHALERR("can't save idx for ds stream");
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_untag_del(
    void             *data,
    void             *rule,
    ca_omci_uint16_t        type)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t   *pUntag    = (ca_omci_untag_filter_t *)rule;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    ca_omci_hal_ds_flow_classifier_reset(pData);
    OMCIHALDBG("delete untag filter eth %d ds class rule", type);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData    = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag  = (ca_omci_sgl_tag_filter_t *)rule;
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 1;
#endif
    ca_uint32_t               ruleIdx  = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pData->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                       = OMCI_HAL_WAN_PORT;
    key.handle_type                    = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    key.key_handle.gem_index           = gemIdx[idx];
    key.l2.vlan_otag.vlan_min.vid      = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;
    key.l2.vlan_otag.vlan_max.vid      = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;
    key.l2.vlan_otag.vlan_min.pri      = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : pSgltag->vlanFilter.pri;
    key.l2.vlan_otag.vlan_max.pri      = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : pSgltag->vlanFilter.pri;
    key.l2.vlan_count                  = 1;
    keyMask.src_port                   = 1;
    keyMask.key_handle                 = 1;
    keyMask.l2_mask.vlan_count         = 1;
    keyMask.l2_mask.vlan_otag          = 1;
    keyMask.l2_mask.vlan_otag_mask.vid = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : 1;
    keyMask.l2_mask.vlan_otag_mask.pri = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : 1;
    keyMask.l2                         = 1;
    action.forward                     = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                   = ca_omci_hal_eth_port_get(pData->uniPortId);
    action.options.priority            = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    action.options.masks.priority      = 1;

    if ((OMCI_FILTER_TPID_DEI_8100 == pData->innerTpid.filter)
     || ((OMCI_FILTER_TPID_NOCARE_DEI ==pData->innerTpid.filter) && (0x8100 == pData->inputTpid)))
    {
        OMCIHALDBG("Filter 0x8100");

        if ((OMCI_TREATMENT_TPID_SET_88A8_DEI_0 == pData->innerTpid.treat)
         || ((0x88A8 == pData->outputTpid) && (OMCI_TREATMENT_TPID_SET_DEI_COPY_INNER == pData->innerTpid.treat)))
        {
            OMCIHALDBG("Swap downstream tpid to 0x8100");
            action.options.outer_tpid       = 0x8100;
            action.options.masks.outer_tpid = 1;
        }
    }

    if (CA_OMCI_FILTER_NONE != pSgltag->etherTypeFilter)
    {
        key.l2.ethertype          = ca_omci_hal_get_eth_type(pSgltag->etherTypeFilter);
        keyMask.l2_mask.ethertype = 1;
    }

    OMCIHALDBG("DS single Tag match vid[%d] pri[%d] to port[0x%x] Filter eth[0x%x]",
                key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                key.key_handle.gem_index, key.l2.ethertype);

    if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
    {
        keyMask.l2_mask.vlan_otag = 0;
        OMCIHALDBG("Just filter single tag, no vid or pri specific");
    }

    if (CA_OMCI_SGL_TAG_FWD_TRANSPARENT == pSgltag->fwdType)
    {
        OMCIHALDBG("Transparent");
    }
    else if (CA_OMCI_SGL_TAG_FWD_DROP == pSgltag->fwdType)
    {
        action.forward = CA_CLASSIFIER_FORWARD_DENY;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_SGL_TAG_FWD_ADD_TAG == pSgltag->fwdType)
    {
        if ((0 != pData->filterPri && 0xff != pData->filterPri)
        && (pSgltag->outerVlanTreatment.pri <= 7))
        {
            if (0 == ((1 << pSgltag->outerVlanTreatment.pri) &  pData->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        key.l2.vlan_count                   = 2;
        key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pSgltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pSgltag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;

        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->outerVlanTreatment.pri)
         && (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
        {
            key.l2.vlan_otag.vlan_min.pri      = 0;
            key.l2.vlan_otag.vlan_max.pri      = 0;
            keyMask.l2_mask.vlan_otag_mask.pri = 0;
        }

        if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
        {
            keyMask.l2_mask.vlan_itag = 0;
            OMCIHALDBG("Don't filter inner tag");
        }

        if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE <= pSgltag->vlanFilter.pri)
         && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->outerVlanTreatment.pri)
         && (0 != pData->filterPri && 0xff != pData->filterPri))
        {
            for (idx = 0; idx < 8; idx++)
            {
                //if map more than 1 dot1p value, need to add more classifier rules here
                if ((1 << idx) & pData->filterPri)
                    break;
            }

            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;//use the map dot1p as key and action tag key
            key.l2.vlan_otag.vlan_min.pri       = idx;
            key.l2.vlan_otag.vlan_max.pri       = idx;
        }

        if (1 == keyMask.l2_mask.vlan_otag_mask.pri) {
            action.options.priority = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;
        }

        OMCIHALDBG("Filter ivid[%d] ipri[%d] ovid[%d] opri[%d] Remove out tag",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri);
    }
    else if (CA_OMCI_SGL_TAG_FWD_MD_TAG == pSgltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pSgltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pSgltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pSgltag->innerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pSgltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
        action.options.outer_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;

        if (OMCI_TREATMENT_COPY_VID_INNER == action.options.outer_vid)
        {
            key.l2.vlan_otag.vlan_min.vid       = 0;
            key.l2.vlan_otag.vlan_max.vid       = 0;
            keyMask.l2_mask.vlan_otag_mask.vid  = 0;
            action.options.masks.outer_vlan_act = 0;
            action.options.outer_vid            = 0;
            action.options.outer_vlan_act       = 0;
            OMCIHALDBG("Only Modify pri");
        }

        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->innerVlanTreatment.pri)
        {
            key.l2.vlan_otag.vlan_min.pri      = 0;
            key.l2.vlan_otag.vlan_max.pri      = 0;
            keyMask.l2_mask.vlan_otag_mask.pri = 0;
            action.options.outer_dot1p         = 0;
            action.options.masks.outer_dot1p   = 0;
            OMCIHALDBG("Only Modify vid");
        }
        OMCIHALDBG("Modify vid[%d] pri[%d] to vid[%d] pri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);

        if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_RM_OUTER_TAG == pSgltag->dsTagAction)
        {
            action.options.outer_vlan_act    = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_dot1p = 0;
            action.options.outer_vid         = 0;
        }

        if (ALCL_DS_TAG_OPER1_BEHAVIOR_TYPE_PRI_TAG == pSgltag->dsTagAction)
        {
            action.options.outer_vid         = 0;
            action.options.outer_dot1p       = 0;
            action.options.masks.outer_dot1p = 0;
            OMCIHALDBG("Only Modify vid to 0");
        }

        if (0 != keyMask.l2_mask.vlan_otag_mask.pri) {
            action.options.priority = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;
        }

        if (0 != pData->filterPri && 0xff != pData->filterPri)
        {
            //for downstream, should match the key value
            if (1 == action.options.masks.outer_dot1p)
            {
                if (0 == ((1 << key.l2.vlan_otag.vlan_max.pri) &  pData->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
            else
            {
                for (idx = 0; idx < 8; idx++)
                {
                    if ((1 << idx) & pData->filterPri)
                    {
                        OMCIHALDBG("map pbit %d to gemport", idx);
                        action.options.outer_dot1p         = idx;
                        action.options.masks.outer_dot1p   = 1;
                        key.l2.vlan_otag.vlan_min.pri      = idx;
                        key.l2.vlan_otag.vlan_max.pri      = idx;
                        keyMask.l2_mask.vlan_otag_mask.pri = 1;
                        action.options.priority            = (0 == pData->dsQueue) ? idx : action.options.priority;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                        if (CA_E_OK == ret)
                        {
                            if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for ds stream");
                            }
                        }

                        if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
                        {
                            OMCIHALDBG("Filter eth type 0x8864 too!");
                            key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK == ret)
                            {
                                if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for ds stream");
                                }
                            }

                        }
                    }
                }
                return CA_OMCI_OK;
            }
        }
    }
    else if (CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_TAG == pSgltag->fwdType)
    {
        key.l2.vlan_count                   = 2;
        key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pSgltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pSgltag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.outer_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
        action.options.outer_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1; //can't support now

        //this only happen in ds mode 3!
        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->innerVlanTreatment.pri)
        {
            action.options.outer_dot1p          = 0;
            action.options.masks.outer_dot1p    = 0;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            keyMask.l2_mask.vlan_itag_mask.pri  = 0;
            OMCIHALDBG("Only Modify vid");
        }

        if (1 == keyMask.l2_mask.vlan_otag_mask.pri) {
            action.options.priority = (0 == pData->dsQueue) ? key.l2.vlan_otag.vlan_max.pri : action.options.priority;
        }

        OMCIHALDBG("Filter ivid[%d] ipri[%d] ovid[%d] opri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri);
        OMCIHALDBG("Modify vid[%d] pri[%d] Remove out tag",
                   action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_SGL_TAG_FWD_REMOVE_TAG == pSgltag->fwdType)
    {
        key.l2.vlan_count                   = 0;
        keyMask.l2_mask.vlan_otag           = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
        action.options.outer_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : pSgltag->vlanFilter.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;

        OMCIHALDBG("Add out Tag vid [%d] pri [%d]", action.options.outer_vid, action.options.outer_dot1p);
    }
    else
    {
        if (CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP == pSgltag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            OMCIHALDBG("Filter ivid[%d] ipri[%d] ovid[%d] remove out tag",
                        key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                        key.l2.vlan_otag.vlan_min.vid);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP == pSgltag->fwdType)
        {
            key.l2.vlan_otag.vlan_min.vid       = pSgltag->innerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pSgltag->innerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
            action.options.outer_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            OMCIHALDBG("Modify vid[%d] pri[%d]",
                       action.options.outer_vid, action.options.outer_dot1p);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_VID_DSCP == pSgltag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
            action.options.inner_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1; //can't support now
            OMCIHALDBG("Modify vid[%d] pri[%d] Remove vid[%d] pri all",
                       action.options.inner_vid, action.options.inner_dot1p,
                       key.l2.vlan_otag.vlan_min.vid);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_TAG == pSgltag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_min.pri       = pSgltag->outerVlanTreatment.pri;
            key.l2.vlan_otag.vlan_max.pri       = pSgltag->outerVlanTreatment.pri;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
            action.options.inner_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1; //can't support now

            OMCIHALDBG("Modify vid[%d] pri all Remove vid[%d] pri[%d]",
                       action.options.inner_vid,
                       key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_VID_DSCP == pSgltag->fwdType)
        {
            key.l2.vlan_count                   = 2;
            key.l2.vlan_otag.vlan_min.vid       = pSgltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pSgltag->outerVlanTreatment.vid;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? pSgltag->innerVlanTreatment.vid : pSgltag->vlanFilter.vid;//can't get the orignal id, so use the treatment value
            action.options.inner_dot1p          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? pSgltag->innerVlanTreatment.pri : pSgltag->vlanFilter.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1; //can't support now

            OMCIHALDBG("Modify vid[%d] pri all Remove vid[%d] all",
                       action.options.inner_vid, key.l2.vlan_otag.vlan_min.vid);
        }
    }
#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set single filter eth[0x%x] failed ret[%d] from port[0x%x]",
                   key.l2.ethertype, ret, key.src_port);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
    {
        OMCIHALERR("can't save idx for ds stream");
        return CA_OMCI_E;
    }

    if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
    {
        OMCIHALDBG("Filter eth type 0x8864 too!");
        key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALDBG("Set Single Tag match vid[%d] pri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                        key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
            return CA_OMCI_E;
        }

        if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
        {
            OMCIHALERR("can't save idx for ds stream");
            return CA_OMCI_E;
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_sgltag_del(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag   = (ca_omci_sgl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    ca_omci_hal_ds_flow_classifier_reset(pData);
    OMCIHALDBG("delete single tag ds class rule");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData    = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag  = (ca_omci_dbl_tag_filter_t *)rule;
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 1;
#endif
    ca_uint32_t               ruleIdx  = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pData->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                           = OMCI_HAL_WAN_PORT;
    key.handle_type                        = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    key.key_handle.gem_index               = gemIdx[idx];
    key.l2.vlan_otag.vlan_min.vid          = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : pDbltag->outerVlanFilter.vid;
    key.l2.vlan_otag.vlan_max.vid          = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : pDbltag->outerVlanFilter.vid;
    key.l2.vlan_otag.vlan_min.pri          = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : pDbltag->outerVlanFilter.pri;
    key.l2.vlan_otag.vlan_max.pri          = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : pDbltag->outerVlanFilter.pri;
    key.l2.vlan_count                      = 2;
    keyMask.src_port                       = 1;
    keyMask.key_handle                     = 1;
    keyMask.l2                             = 1;
    keyMask.l2_mask.vlan_count             = 1;
    keyMask.l2_mask.vlan_otag              = 1;
    keyMask.l2_mask.vlan_otag_mask.vid     = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
    keyMask.l2_mask.vlan_otag_mask.pri     = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                       = ca_omci_hal_eth_port_get(pData->uniPortId);
    action.options.priority                = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    action.options.masks.priority          = 1;

    if ((OMCI_FILTER_TPID_DEI_8100 == pData->innerTpid.filter)
     || ((OMCI_FILTER_TPID_NOCARE_DEI ==pData->innerTpid.filter) && (0x8100 == pData->inputTpid)))
    {
        OMCIHALDBG("Filter 0x8100");

        if ((OMCI_TREATMENT_TPID_SET_88A8_DEI_0 == pData->innerTpid.treat)
         || ((0x88A8 == pData->outputTpid) && (OMCI_TREATMENT_TPID_SET_DEI_COPY_INNER == pData->innerTpid.treat)))
        {
            OMCIHALDBG("Swap downstream tpid to 0x8100");
            action.options.outer_tpid       = 0x8100;
            action.options.masks.outer_tpid = 1;
        }
    }

    if (CA_OMCI_FILTER_NONE != pDbltag->etherTypeFilter)
    {
        key.l2.ethertype          = ca_omci_hal_get_eth_type(pDbltag->etherTypeFilter);
        keyMask.l2_mask.ethertype = 1;
    }

    OMCIHALDBG("DS double Tag match ovid[%d] opri[%d] ivid[%d] ipri[%d] for gemIdx[0x%x] Filter eth[0x%x]",
                key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                key.key_handle.gem_index, key.l2.ethertype);

    if ((OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri))
    {
        keyMask.l2_mask.vlan_otag = 0;
        OMCIHALDBG("Filter outer tag, no vid or pri specific");
    }

    if ((OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri))
    {
        keyMask.l2_mask.vlan_itag = 0;
        OMCIHALDBG("Filter inner tag, no vid or pri specific");
    }

    if (CA_OMCI_DBL_TAG_FWD_TRANSPARENT == pDbltag->fwdType)
    {
        OMCIHALDBG("Transparent");
    }
    else if (CA_OMCI_DBL_TAG_FWD_DROP == pDbltag->fwdType)
    {
        action.forward = CA_CLASSIFIER_FORWARD_DENY;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_DBL_TAG_ADD_TAG == pDbltag->fwdType)
    {
        if ((0 != pData->filterPri && 0xff != pData->filterPri)
         && (pDbltag->outerVlanTreatment.pri <= 7))
        {
            if (0 == ((1 << pDbltag->outerVlanTreatment.pri) &  pData->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        OMCIHALDBG("Remove outer tag");
        key.l2.vlan_count                   = 3;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag           = 1;
        keyMask.l2_mask.vlan_otag_mask.vid  = 1;
        keyMask.l2_mask.vlan_otag_mask.pri  = 1;

        if ((OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE <= pDbltag->outerVlanFilter.pri))
        {
            key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
            key.l2.vlan_otag.vlan_min.pri       = 0;
            key.l2.vlan_otag.vlan_max.pri       = 0;
            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.vid  = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.outer_vlan_act = 1;
        }

        if ((OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri)
         && (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE <= pDbltag->outerVlanFilter.pri)
         && (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == pDbltag->outerVlanTreatment.pri)
         && (0 != pData->filterPri && 0xff != pData->filterPri))
        {
            for (idx = 0; idx < 8; idx++)
            {
                //if map more than 1 dot1p value, need to add more classifier rules here
                if ((1 << idx) & pData->filterPri)
                    break;
            }

            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;//use the map dot1p as key and action tag key
            key.l2.vlan_otag.vlan_min.pri       = idx;
            key.l2.vlan_otag.vlan_max.pri       = idx;
        }
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify out tag vid[%d] pri[%d] to vid[%d] pri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify out tag vid[%d] to vid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid);

        if (0 != pData->filterPri && 0xff != pData->filterPri)
        {
            //for downstream, should match the key value
            if (1 == action.options.masks.outer_dot1p)
            {
                if (0 == ((1 << key.l2.vlan_otag.vlan_max.pri) &  pData->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
            else
            {
                for (idx = 0; idx < 8; idx++)
                {
                    if ((1 << idx) & pData->filterPri)
                    {
                        OMCIHALDBG("map pbit %d to gemport", idx);
                        action.options.outer_dot1p         = idx;
                        action.options.masks.outer_dot1p   = 1;
                        key.l2.vlan_otag.vlan_min.pri      = idx;
                        key.l2.vlan_otag.vlan_max.pri      = idx;
                        keyMask.l2_mask.vlan_otag_mask.pri = 1;
                        action.options.priority            = (0 == pData->dsQueue) ? idx : action.options.priority;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                        if (CA_E_OK == ret)
                        {
                            if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for ds stream");
                            }
                        }

                        if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
                        {
                            OMCIHALDBG("Filter eth type 0x8864 too!");
                            key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK == ret)
                            {
                                if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for ds stream");
                                }
                            }

                        }
                    }
                }
                return CA_OMCI_OK;
            }
        }
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify out tag vid[%d] to vid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify out tag vid[%d] to vid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_DSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify out tag vid[%d] to vid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify out tag pri[%d] to pri[%d]",
                    key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_TAGS == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ipri[%d] to ipri[%d]", key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ipri[%d] to ipri[%d]", key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d] Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d] ",
                    action.options.outer_vid, action.options.outer_dot1p,
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IPRI == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ipri[%d] to ipri[%d]", key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ipri[%d] to ipri[%d]", key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] opri[%d] to ovid[%d] opri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    action.options.outer_vid, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d] ivid[%d] to ivid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_DSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d] ivid[%d] to ivid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d] ivid[%d] to ivid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_ODSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d] ivid[%d] to ivid[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_IPRI == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d]  ipri[%d] to ipri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_IPRI == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->outerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ovid[%d] to ovid[%d]  ipri[%d] to ipri[%d]",
                    key.l2.vlan_otag.vlan_min.vid, action.options.outer_vid,
                    key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d] Modify ivid[%d] to ivid[%d]",
                    action.options.outer_vid, action.options.outer_dot1p,
                    key.l2.vlan_otag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d] Modify ivid[%d] to ivid[%d]",
                    action.options.outer_vid, action.options.outer_dot1p,
                    key.l2.vlan_otag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAGS == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 0;
        keyMask.l2_mask.vlan_otag_mask.vid  = 0;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        keyMask.l2_mask.vlan_otag           = 0;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d] ivid[%d] to ivid[%d]",
                    action.options.outer_vid, action.options.outer_dot1p,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        key.l2.vlan_otag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d] Modify ipri[%d] to ipri[%d]",
                    action.options.outer_vid, action.options.outer_dot1p,
                    key.l2.vlan_otag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ivid[%d] ipri[%d]",
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ivid[%d] ipri[%d]",
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] to opri[%d]", key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] to opri[%d]", key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] to ivid[%d]", key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] to opri[%d]", key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p);
        OMCIHALDBG("Modify ivid[%d] to ivid[%d]", key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_PRIS == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ipri[%d] to opri[%d] ipri[%d]",
                    key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p,
                    key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP == pDbltag->fwdType)
    {
        keyMask.l2_mask.vlan_otag_mask.pri = 0;
        action.options.outer_dot1p         = pDbltag->outerVlanFilter.pri;
        action.options.masks.outer_dot1p   = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d]", action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_otag.vlan_min.pri       = pDbltag->outerVlanTreatment.pri;
        key.l2.vlan_otag.vlan_max.pri       = pDbltag->outerVlanTreatment.pri;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] to opri[%d] ipri[%d]",
                    key.l2.vlan_otag.vlan_min.pri, action.options.outer_dot1p,
                    action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IVID == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ivid[%d] to ivid[%d]",
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IVID_IDSCP == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify ivid[%d] to ivid[%d]",
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IPRI == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.pri    = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri    = pDbltag->innerVlanTreatment.pri;
        action.options.inner_dot1p       = pDbltag->innerVlanFilter.pri;
        //action.options.masks.inner_dot1p = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ipri[%d] to ipri[%d]",
                    key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IDSCP == pDbltag->fwdType)
    {
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        //action.options.masks.inner_dot1p = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify ipri[%d]", action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_ITAG == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        //action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ivid[%d] ipri[%d] to ivid[%d] ipri[%d]",
                    action.options.outer_dot1p,
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IVID == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ivid[%d] to ivid[%d]",
                    action.options.outer_dot1p,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_DSCP_IVID == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.vid       = pDbltag->innerVlanTreatment.vid;
        key.l2.vlan_itag.vlan_max.vid       = pDbltag->innerVlanTreatment.vid;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanFilter.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        action.options.masks.inner_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ipri [%d] ivid[%d] to ivid[%d]",
                    action.options.outer_dot1p, action.options.inner_dot1p,
                    key.l2.vlan_itag.vlan_min.vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IPRI == pDbltag->fwdType)
    {
        key.l2.vlan_itag.vlan_min.pri       = pDbltag->innerVlanTreatment.pri;
        key.l2.vlan_itag.vlan_max.pri       = pDbltag->innerVlanTreatment.pri;
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ipri [%d] to ipri[%d]",
                    action.options.outer_dot1p, key.l2.vlan_itag.vlan_min.pri, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_DSCP == pDbltag->fwdType)
    {
        keyMask.l2_mask.vlan_otag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.inner_dot1p          = pDbltag->innerVlanFilter.pri;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
        //action.options.masks.inner_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Modify opri[%d] ipri [%d] ",
                    action.options.outer_dot1p, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_OTAG == pDbltag->fwdType)
    {
        key.l2.vlan_count                   = 1;
        keyMask.l2_mask.vlan_itag_mask.vid  = 0;
        keyMask.l2_mask.vlan_itag_mask.pri  = 0;
        keyMask.l2_mask.vlan_itag           = 0;
        key.l2.vlan_otag.vlan_min.vid       = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : pDbltag->innerVlanFilter.vid;
        key.l2.vlan_otag.vlan_max.vid       = (OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid) ? 0 : pDbltag->innerVlanFilter.vid;
        key.l2.vlan_otag.vlan_min.pri       = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : pDbltag->innerVlanFilter.pri;
        key.l2.vlan_otag.vlan_max.pri       = (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri) ? 0 : pDbltag->innerVlanFilter.pri;
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pDbltag->outerVlanFilter.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanFilter.pri;
        action.options.masks.outer_vlan_act = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
        action.options.masks.outer_dot1p    = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;

        OMCIHALDBG("Add ovid[%d] opri[%d]",
                    action.options.outer_vid, action.options.outer_dot1p);
    }
    else
    {
        OMCIHALERR("Unsupport type %d", pDbltag->fwdType);
        return CA_OMCI_E;
    }
#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set Double Tag match ovid[%d] opri[%d] ivid[%d] ipri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
    {
        OMCIHALERR("can't save idx for ds stream");
        return CA_OMCI_E;
    }

    if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
    {
        OMCIHALDBG("Filter eth type 0x8864 too!");
        key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALDBG("Set Double Tag match ovid[%d] opri[%d] ivid[%d] ipri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                        key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                        key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
            return CA_OMCI_E;
        }

        if (CA_OMCI_OK != ca_omci_hal_flow_ds_classifier_idx_save(pData, ruleIdx))
        {
            OMCIHALERR("can't save idx for ds stream");
            return CA_OMCI_E;
        }
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_inverse_dbltag_del(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag   = (ca_omci_dbl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    ca_omci_hal_ds_flow_classifier_reset(pData);
    OMCIHALDBG("delete double tag ds class rule");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_def_set(
    ca_omci_stream_cfg_t *pData,
    ca_omci_bool_t flag)
{
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 0;
#endif
    ca_uint32_t               ruleIdx  = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE == ca_omci_hal_flow_def_classifier_idx_check(pData))
    {
        OMCIHALDBG("Already Set default rule for gemport %d, %s",
                    pData->gemId, (CA_OMCI_TRUE == flag) ? "pass" : "drop");
        return CA_OMCI_OK;
    }

    OMCI_MEMSET(&key,     0, sizeof(key));
    OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
    OMCI_MEMSET(&action,  0, sizeof(action));
    OMCI_MEMSET(gemIdx,   0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pData->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port             = OMCI_HAL_WAN_PORT;
    key.key_handle.gem_index = gemIdx[idx];
    key.handle_type          = CA_KEY_HANDLE_TYPE_GEM_INDEX;//default rules should be created in L2 classifier table
    keyMask.src_port         = 1;
    keyMask.key_handle       = 1;
    action.forward           = (CA_OMCI_TRUE == flag) ? CA_CLASSIFIER_FORWARD_PORT : CA_CLASSIFIER_FORWARD_DENY;
    action.dest.port         = (CA_OMCI_TRUE == flag) ? ca_omci_hal_eth_port_get(pData->uniPortId) : 0;
    action.options.priority  = ca_omci_hal_ds_pq_idx_get(pData->dsQueue);
    action.options.masks.priority = 1;

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set default rule for gemport %d, idx 0x%x, %s failed ret %d",
                    pData->gemId, gemIdx[idx], (CA_OMCI_TRUE == flag) ? "pass" : "drop", ret);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_def_classifier_idx_save(pData, ruleIdx))
    {
        OMCIHALERR("can't save idx for ds stream");
        return CA_OMCI_E;
    }

    OMCIHALDBG("Set default rule for gemport %d, idx 0x%x, %s",
                pData->gemId, gemIdx[idx], (CA_OMCI_TRUE == flag) ? "pass" : "drop");
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_flow_def_reset(ca_omci_stream_cfg_t *pData)
{
    ca_omci_uint32_t idx = 0;

    idx = ca_omci_hal_flow_def_classifier_idx_get(pData);

    if (0 != idx)
    {
        OMCIHALDBG("Release default rule for gemport %d", pData->gemId);
#if !defined(CONFIG_CA_OMCI_FPGA)
        if (CA_E_OK != ca_classifier_rule_delete(OMCI_HAL_DEV_ID, idx))
        {
            OMCIHALERR("Release rule %d for gemport %d failed", idx, pData->gemId);
        }
#endif
        ca_omci_hal_flow_def_classifier_idx_release(pData);
        return CA_OMCI_OK;
    }

    OMCIHALDBG("No need to release default rule for gemport %d", pData->gemId);
    return CA_OMCI_OK;
}


/* Regardless of the filter rules, discard all downstream traffic. */
ca_omci_st_e ca_omci_hal_ds_flow_dis_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t   *pUntag    = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream untag discard");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


ca_omci_st_e ca_omci_hal_ds_flow_dis_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag   = (ca_omci_sgl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream single tag discard");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


ca_omci_st_e ca_omci_hal_ds_flow_dis_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData     = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag   = (ca_omci_dbl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream double tag discard");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


/* Filter on P-bit only. On a match, perform the inverse P-bit operation
     only; pass the VID through. If no match is found, discard the frame. */
ca_omci_st_e ca_omci_hal_ds_flow_pbit_dis_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default discard rule
    OMCIHALDBG("Downstream untag discard for mode 7");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


ca_omci_st_e ca_omci_hal_ds_flow_pbit_dis_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;
    ca_omci_sgl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri)
    {
        OMCIHALDBG("Unmatch single tag rule discard for mode 7");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match single tag rule inverse for mode 7");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.vlanFilter.vid         = OMCI_FILTER_VID_NONE;
    ruleTbl.innerVlanTreatment.vid = OMCI_TREATMENT_COPY_VID_INNER;

    return ca_omci_hal_ds_flow_inverse_sgltag(data, &ruleTbl);
}


ca_omci_st_e ca_omci_hal_ds_flow_pbit_dis_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;
    ca_omci_dbl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri)
    {
        OMCIHALDBG("Unmatch double tag rule discard for mode 7");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match double tag rule inverse for mode 7");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.innerVlanFilter.vid = OMCI_FILTER_VID_NONE;
    ruleTbl.outerVlanFilter.vid = OMCI_FILTER_VID_NONE;
    return ca_omci_hal_ds_flow_inverse_dbltag(data, &ruleTbl);
}


/* Filter on VID. On a match, perform the inverse operation on the VID only;
    pass the P bits through. If no match is found, discard the frame.  */
ca_omci_st_e ca_omci_hal_ds_flow_vid_dis_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default pass rule
    OMCIHALDBG("Downstream untag discard for mode 6");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


ca_omci_st_e ca_omci_hal_ds_flow_vid_dis_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;
    ca_omci_sgl_tag_filter_t  ruleTbl = {0};
    ca_omci_stream_cfg_t      stream  = {0};

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
    {
        OMCIHALDBG("Unmatch single tag rule discard for mode 6");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match single tag rule inverse for mode 6");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));
    OMCI_MEMCPY(&stream,  pData, sizeof(stream));

    ruleTbl.vlanFilter.pri         = OMCI_FILTER_PRI_NONE;
    ruleTbl.innerVlanTreatment.pri = OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI;
    ruleTbl.outerVlanTreatment.pri = OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI;
    stream.filterPri               = 0;
    return ca_omci_hal_ds_flow_inverse_sgltag((void *)&stream, (void *)&ruleTbl);
}


ca_omci_st_e ca_omci_hal_ds_flow_vid_dis_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;
    ca_omci_dbl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
    {
        OMCIHALDBG("Unmatch double tag rule discard for mode 6");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match double tag rule inverse for mode 6");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.innerVlanFilter.pri = OMCI_FILTER_PRI_NONE;
    ruleTbl.outerVlanFilter.pri = OMCI_FILTER_PRI_NONE;
    return ca_omci_hal_ds_flow_inverse_dbltag(data, &ruleTbl);
}


/* Filter  on  VID  and  P-bit  value.  On  a  match,  perform  the  inverse operation  on
    both  the  VID  and  P-bit  value.  If  no  match  is  found,  discard the frame.*/
ca_omci_st_e ca_omci_hal_ds_flow_tag_dis_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default discard rule
    OMCIHALDBG("Downstream untag discard for mode 5");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
}


ca_omci_st_e ca_omci_hal_ds_flow_tag_dis_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
     || (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
    {
        OMCIHALDBG("Unmatch single tag rule discard for mode 5");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match single tag rule inverse for mode 5");
    return ca_omci_hal_ds_flow_inverse_sgltag(data, rule);
}


ca_omci_st_e ca_omci_hal_ds_flow_tag_dis_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just discard
    if ((OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
     || (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri))
    {
        OMCIHALDBG("Unmatch double tag rule discard for mode 5");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    }

    OMCIHALDBG("match double tag rule inverse for mode 5");
    return ca_omci_hal_ds_flow_inverse_dbltag(data, rule);
}


/* Filter on P-bit only. On a match, perform the inverse P-bit operation only;
     pass the VID through. If no match is found, forward the frame unmodified. */
ca_omci_st_e ca_omci_hal_ds_flow_pbit_pass_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default pass rule
    OMCIHALDBG("Downstream untag pass for mode 4");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_ds_flow_pbit_pass_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;
    ca_omci_sgl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri)
    {
        OMCIHALDBG("Unmatch single tag rule pass for mode 4");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match single tag rule inverse and pass vid");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.vlanFilter.vid         = OMCI_FILTER_VID_NONE;
    ruleTbl.innerVlanTreatment.vid = OMCI_TREATMENT_COPY_VID_INNER;

    return ca_omci_hal_ds_flow_inverse_sgltag(data, &ruleTbl);
}


ca_omci_st_e ca_omci_hal_ds_flow_pbit_pass_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;
    ca_omci_dbl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri)
    {
        OMCIHALDBG("Unmatch double tag rule pass for mode 4");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match double tag rule inverse and pass vid");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.innerVlanFilter.vid = OMCI_FILTER_VID_NONE;
    ruleTbl.outerVlanFilter.vid = OMCI_FILTER_VID_NONE;
    return ca_omci_hal_ds_flow_inverse_dbltag(data, &ruleTbl);
}


/*  Filter on VID only. On a match, perform the inverse VID operation only;
     pass the P bits through. If no match is found, forward the frame unmodified. */
ca_omci_st_e ca_omci_hal_ds_flow_vid_pass_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default pass rule
    OMCIHALDBG("Downstream untag pass for mode 3");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_ds_flow_vid_pass_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;
    ca_omci_sgl_tag_filter_t  ruleTbl = {0};
    ca_omci_stream_cfg_t      stream  = {0};

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
    {
        OMCIHALDBG("Unmatch single tag rule pass for mode 3");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match single tag rule inverse and pass pbit");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));
    OMCI_MEMCPY(&stream,  pData, sizeof(stream));

    ruleTbl.vlanFilter.pri         = OMCI_FILTER_PRI_NONE;
    ruleTbl.innerVlanTreatment.pri = OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI;
    ruleTbl.outerVlanTreatment.pri = OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI;
    stream.filterPri               = 0;

    return ca_omci_hal_ds_flow_inverse_sgltag((void *)&stream, (void *)&ruleTbl);
}


ca_omci_st_e ca_omci_hal_ds_flow_vid_pass_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;
    ca_omci_dbl_tag_filter_t  ruleTbl = {0};

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
    {
        OMCIHALDBG("Unmatch double tag rule pass for mode 3");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match double tag rule inverse and pass pbit");
    OMCI_MEMCPY(&ruleTbl, rule, sizeof(ruleTbl));

    ruleTbl.innerVlanFilter.pri = OMCI_FILTER_PRI_NONE;
    ruleTbl.outerVlanFilter.pri = OMCI_FILTER_PRI_NONE;
    return ca_omci_hal_ds_flow_inverse_dbltag(data, &ruleTbl);
}


/* Filter  on  VID  and  P-bit  value.  On  a  match,  perform  the  inverse operation  on
    both  the  VID  and  P-bit  value.  If  no  match  is  found,  forward the frame unmodified. */
ca_omci_st_e ca_omci_hal_ds_flow_tag_pass_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    //untag rule only need to create default pass rule
    OMCIHALDBG("Downstream untag pass for mode 2");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_ds_flow_tag_pass_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
     || (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
    {
        OMCIHALDBG("Unmatch single tag rule pass for mode 2");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match single tag rule inverse for mode 2");
    return ca_omci_hal_ds_flow_inverse_sgltag(data, rule);
}


ca_omci_st_e ca_omci_hal_ds_flow_tag_pass_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    //unmatch just pass
    if ((OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
     || (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri))
    {
        OMCIHALDBG("Unmatch double tag rule pass for mode 2");
        return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
    }

    OMCIHALDBG("match double tag rule inverse for mode 2");
    return ca_omci_hal_ds_flow_inverse_dbltag(data, rule);
}


/* Regardless  of  the  filter  rules,  no  operation  is  performed  in  the
    downstream  direction.  All  downstream  frames  are  forwarded unmodified. */
ca_omci_st_e ca_omci_hal_ds_flow_fw_untag(
    void             *data,
    void             *rule,
    ca_omci_uint16_t  type)
{
    ca_omci_stream_cfg_t   *pData  = (ca_omci_stream_cfg_t *)data;
    ca_omci_untag_filter_t *pUntag = (ca_omci_untag_filter_t *)rule;

    (void)type;

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream untag pass");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_ds_flow_fw_sgltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_sgl_tag_filter_t *pSgltag = (ca_omci_sgl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream single tag pass");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_ds_flow_fw_dbltag(void *data, void *rule)
{
    ca_omci_stream_cfg_t     *pData   = (ca_omci_stream_cfg_t *)data;
    ca_omci_dbl_tag_filter_t *pDbltag = (ca_omci_dbl_tag_filter_t *)rule;

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    OMCIHALDBG("Downstream double tag pass");
    return ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_TRUE);
}


ca_omci_st_e ca_omci_hal_filter_flow_classifier_reset(
    ca_omci_filter_stream_cfg_t *pUsFlow)
{
    ca_omci_uint32_t *ruleIdx = NULL;
    ca_omci_uint8_t   idx     = 0;
    ca_status_t       ret     = CA_E_OK;

    ruleIdx = ca_omci_hal_filter_classifier_idx_get(pUsFlow);
    //remove the us
    if (NULL != ruleIdx)
    {
        for (idx = 0; idx < OMCI_HAL_MAX_RULE_PER_FLOW_US; idx++)
        {
            if (0 == ruleIdx[idx])
            {
                continue;
            }
#if !defined(CONFIG_CA_OMCI_FPGA)
            ret = ca_classifier_rule_delete(OMCI_HAL_DEV_ID, ruleIdx[idx]);
#endif
            if (CA_E_OK != ret)
            {
                OMCIHALERR("remove classifier failed ret[%d] for index[%d]", ret, ruleIdx[idx]);
            }

            OMCIHALDBG("remove rule %d for gem %d, uni %d, type %d",
                        ruleIdx[idx], pUsFlow->gemId, pUsFlow->uniPortId, pUsFlow->portType);
            ruleIdx[idx] = 0;
        }
    }

    //remove the ds
    ruleIdx = ca_omci_hal_ds_filter_classifier_idx_get(pUsFlow);
    if (NULL != ruleIdx)
    {
        for (idx = 0; idx < OMCI_HAL_MAX_RULE_PER_FLOW; idx++)
        {
            if (0 == ruleIdx[idx])
            {
                continue;
            }
#if !defined(CONFIG_CA_OMCI_FPGA)
            ret = ca_classifier_rule_delete(OMCI_HAL_DEV_ID, ruleIdx[idx]);
#endif
            if (CA_E_OK != ret)
            {
                OMCIHALERR("remove classifier failed ret[%d] for index[%d]", ret, ruleIdx[idx]);
            }

            OMCIHALDBG("remove rule %d for gem %d, uni %d, type %d",
                        ruleIdx[idx], pUsFlow->gemId, pUsFlow->uniPortId, pUsFlow->portType);
            ruleIdx[idx] = 0;
        }
    }

    return ca_omci_hal_filter_classifier_tbl_release(pUsFlow);
}


ca_omci_st_e ca_omci_hal_def_drop_set(ca_omci_stream_cfg_t *pStream)
{
    ca_classifier_action_t  defAct  = {0};
    ca_uint32_t                     srcPort = 0;
    ca_status_t                     ret     = CA_E_OK;

    OMCI_MEMSET(&defAct, 0, sizeof(defAct));

    if (OMCI_CFG_STREAM_PORT_ETH == pStream->portType)
    {
        srcPort = ca_omci_hal_eth_port_get(pStream->uniPortId);

        defAct.forward = CA_CLASSIFIER_FORWARD_DENY;
        //ret = ca_classifier_port_default_action_set(OMCI_HAL_DEV_ID, srcPort, &defAct);
        if (CA_E_OK != ret)
        {
            OMCIHALERR("Set def action drop failed ret[%d] for port[0x%x]", ret, srcPort);
            return CA_OMCI_E;
        }

        OMCIHALDBG("Set port[0x%x] def action drop", srcPort);
    }

    if (OMCI_CFG_STREAM_PORT_VEIP == pStream->portType)
    {
        defAct.forward = CA_CLASSIFIER_FORWARD_DENY;
        //ret = ca_classifier_port_default_action_set(OMCI_HAL_DEV_ID, OMCI_HAL_VEIP_SRC_PORT, &defAct);
        if (CA_E_OK != ret)
        {
            OMCIHALERR("Set def action drop failed ret[%d] for port[0x%x]", ret, OMCI_HAL_VEIP_SRC_PORT);
            return CA_OMCI_E;
        }

        OMCIHALDBG("Set wanport[0x%x] def action drop", OMCI_HAL_VEIP_SRC_PORT);
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_alcl_def_untag(
    ca_omci_stream_cfg_t   *pUsFlow,
    ca_omci_untag_filter_t *pUntag)
{
    ca_classifier_key_t        key      = {0};
    ca_classifier_key_mask_t   keyMask  = {0};
    ca_classifier_action_t     action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t                priority = 0;
#endif
    ca_uint32_t                srcPort  = OMCI_HAL_VEIP_SRC_PORT;
    ca_uint32_t                ruleIdx  = 0;
    ca_status_t                ret      = CA_E_OK;
    ca_uint32_t                uniPort  = OMCI_HAL_WAN_PORT;
    ca_uint16_t                idx      = 0;
    ca_uint16_t                gemNum   = 0;
    ca_omci_onuinfo_obj_t     *onuInfo  = ca_omci_onuinfo_get();
    ca_omci_uint8_t            index    = 0;
    ca_uint16_t                gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];
    ca_omci_uint8_t            mac[OMCI_MAC_ADDR_LEN];
    ca_omci_uint8_t            dscpIdx  = 0;

    if (OMCI_CFG_STREAM_PORT_ETH == pUsFlow->portType)
    {
        srcPort = ca_omci_hal_eth_port_get(pUsFlow->uniPortId);
        uniPort = srcPort;
    }

    OMCI_MEMSET(&key,       0, sizeof(key));
    OMCI_MEMSET(&keyMask,   0, sizeof(keyMask));
    OMCI_MEMSET(&action,    0, sizeof(action));
    OMCI_MEMSET(gemIdx,     0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
    OMCI_MEMSET(mac,        0, sizeof(ca_omci_uint8_t) * OMCI_MAC_ADDR_LEN);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pUsFlow->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    if (OMCI_CFG_STREAM_PORT_IPHOST == pUsFlow->portType)
    {
        //base mac + port id is the eth MAC
        //eth0-6, source port shoule be 0x40010, only eth7 use 0x40011
        srcPort = (7 == pUsFlow->uniPortId) ? (OMCI_HAL_CPU_PORT + 1) : OMCI_HAL_CPU_PORT;
        uniPort = srcPort;

        OMCI_MEMCPY(mac, onuInfo->mac, OMCI_MAC_ADDR_LEN);
        mac[5] = mac[5] + pUsFlow->uniPortId;

        OMCI_MEMCPY(&key.l2.mac_sa.mac_max, mac, OMCI_MAC_ADDR_LEN);
        OMCI_MEMCPY(&key.l2.mac_sa.mac_min, mac, OMCI_MAC_ADDR_LEN);
        keyMask.l2_mask.mac_sa = 1;
    }

    key.src_port                           = srcPort;
    key.l2.vlan_count                      = 0;
    keyMask.l2                             = 1;
    keyMask.l2_mask.vlan_count             = 1;
    keyMask.src_port                       = 1;
    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                       = OMCI_HAL_WAN_PORT;
    action.options.action_handle.gem_index = gemIdx[idx];
    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    action.options.masks.action_handle     = 1;

    /* Check OMCI gemport index cos value, don't use the action.options.priority value */
    action.options.masks.use_action_handle_priority = 1;

    if (CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType) {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pUntag->innerVlanTreatment.vid;
        action.options.outer_dot1p          = pUntag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType) {
        if (0 != pUsFlow->filterPri) {
            /* map to different gemport TODO fib_tx_cos need to modify depend on max queue per tcont */
            for (index = 0; index < OMCI_HAL_MAX_PRI_PER_FLOW; index++) {
                if ((1 << index) & pUsFlow->filterPri) {
                    /* Get dscp value based on dot1p Map*/
                    for (dscpIdx = 0; dscpIdx < 64; dscpIdx++) {
                        if (index == sg_dscpMapTbl[dscpIdx]){
                            key.ip.dscp          = dscpIdx;
                            keyMask.ip_mask.dscp = 1;
                            keyMask.ip           = 1;

                            action.options.outer_vlan_act                   = CA_CLASSIFIER_VLAN_ACTION_PUSH;
                            action.options.outer_vid                        = pUntag->innerVlanTreatment.vid;
                            action.options.outer_dot1p                      = index;
                            action.options.masks.outer_vlan_act             = 1;
                            action.options.masks.outer_dot1p                = 1;

#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK != ret)
                            {
                                OMCIHALERR("Add Tag for port[0x%x] failed ret[%d] to gemIdx[0x%x]",
                                            key.src_port, ret, action.options.action_handle.gem_index);
                                return CA_OMCI_E;
                            }

                            if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for us stream");
                                return CA_OMCI_E;
                            }
                        }
                    }
                }
            }
        }
        else {
            OMCIHALWARN("can't set classifier rule filter pbit 0x%x", pUsFlow->filterPri);
        }
        return CA_OMCI_OK;
    }
#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Add Tag for port[0x%x] failed ret[%d] to gemIdx[0x%x]",
                    key.src_port, ret, action.options.action_handle.gem_index);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
    {
        OMCIHALERR("can't save idx for us stream");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_untag(
    ca_omci_stream_cfg_t   *pUsFlow,
    ca_omci_untag_filter_t *pUntag,
    ca_omci_uint16_t        type)
{
    ca_omci_onuinfo_obj_t     *onuinfo  = ca_omci_onuinfo_get();
    ca_classifier_key_t        key      = {0};
    ca_classifier_key_mask_t   keyMask  = {0};
    ca_classifier_action_t     action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t                priority = 1;
#endif
    ca_uint32_t                srcPort  = OMCI_HAL_VEIP_SRC_PORT;
    ca_uint32_t                ruleIdx  = 0;
    ca_uint32_t                vid      = 0;
    ca_status_t                ret      = CA_E_OK;
    ca_uint32_t                uniPort  = OMCI_HAL_WAN_PORT;
    ca_uint16_t                idx      = 0;
    ca_uint16_t                gemNum   = 0;
    ca_omci_uint8_t            index    = 0;
    ca_omci_uint8_t            dscpIdx  = 0;
    ca_uint16_t                gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (OMCI_CFG_STREAM_PORT_ETH == pUsFlow->portType)
    {
        srcPort = ca_omci_hal_eth_port_get(pUsFlow->uniPortId);
        uniPort = srcPort;
    }

    if (CA_OMCI_TRUE != pUntag->isValid)
    {
        return CA_OMCI_OK;
    }

    /* For Some OLT, default rule only have 2 kinds, drop or add tag.
          Don't need to seprate it to diff tags */
    if ((OMCI_HAL_ETH_TYPE_DEF == type) && (OMCI_OLT_TYPE_ALCL == onuinfo->oltType))
    {
        if (CA_OMCI_UNTAG_FWD_DROP == pUntag->fwdType)
        {
            //set drop all packet in default drop set, so just return
            return CA_OMCI_OK;
        }

        if ((CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType)
         || (CA_OMCI_UNTAG_FWD_TRANSPARENT == pUntag->fwdType)
         || (CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType))
        {
            //set pvid to the port
            ca_omci_hal_alcl_def_untag(pUsFlow, pUntag);
        }

        return CA_OMCI_OK;
    }

    OMCI_MEMSET(&key,       0, sizeof(key));
    OMCI_MEMSET(&keyMask,   0, sizeof(keyMask));
    OMCI_MEMSET(&action,    0, sizeof(action));
    OMCI_MEMSET(gemIdx,     0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pUsFlow->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    //TODO: may need to add loop to configure all, for now, there will be only one
    key.src_port                           = srcPort;
    key.l2.vlan_count                      = 0;
    keyMask.src_port                       = 1;
    keyMask.l2_mask.vlan_count             = 1;
    keyMask.l2                             = 1;
    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                       = OMCI_HAL_WAN_PORT;
    action.options.action_handle.gem_index = gemIdx[idx];
    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    action.options.masks.action_handle     = 1;

    //Check OMCI gemport index cos value, don't use the action.options.priority value
    action.options.masks.use_action_handle_priority = 1;

    if (OMCI_HAL_ETH_TYPE_DEF != type)
    {
        key.l2.ethertype          = type;
        keyMask.l2_mask.ethertype = 1;
    }
    OMCIHALDBG("Set untag stream from port[0x%x] to gemIdx[0x%x] filter eth[0x%x]",
                srcPort, action.options.action_handle.gem_index, key.l2.ethertype);

    /*Set TPID for TR247/TR255*/
    if (OMCI_OLT_TYPE_SIM == onuinfo->oltType){
        if ((0x88a8 == pUsFlow->outputTpid)
         && (OMCI_TREATMENT_TPID_SET_88A8_DEI_0 == pUsFlow->innerTpid.treat)) {
            action.options.outer_tpid       = 0x88a8;
            action.options.masks.outer_tpid = 1;
        }
    }

    if (CA_OMCI_UNTAG_FWD_TRANSPARENT == pUntag->fwdType)
    {
        OMCIHALDBG("Transparent");
    }
    else if (CA_OMCI_UNTAG_FWD_DROP == pUntag->fwdType)
    {
        action.forward                         = CA_CLASSIFIER_FORWARD_DENY;
        action.dest.port                       = 0;
        action.options.action_handle.gem_index = 0;
        action.options.masks.action_handle     = 0;
        action.options.handle_type             = 0;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_VID_PRI == pUntag->fwdType)
    {
        if (0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri)
        {
            if (0 == ((1 << pUntag->innerVlanTreatment.pri) &  pUsFlow->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pUntag->innerVlanTreatment.vid;
        action.options.outer_dot1p          = pUntag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pUntag->innerVlanTreatment.vid;
        OMCIHALDBG("Add vid[%d] pri[%d]", action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_PRI == pUntag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pUntag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pUntag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.inner_vid            = pUntag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pUntag->innerVlanTreatment.pri;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1; //not support now
        vid                                 = pUntag->outerVlanTreatment.vid;
        OMCIHALDBG("Add ovid[%d] opri[%d] ivid[%d] ipri[%d]",
                    action.options.outer_vid, action.options.outer_dot1p,
                    action.options.inner_vid, action.options.inner_dot1p);
    }
    else
    {
        //all relate to DSCP, so need to make sure it filter IP packet
        //type should be ip only
        if ((CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType)
         && ((OMCI_HAL_ETH_TYPE_PPPOE == type) || (OMCI_HAL_ETH_TYPE_ARP == type) || (OMCI_HAL_ETH_TYPE_PPPOE2 == type))) {
            OMCIHALDBG("Skip PPPOE and ARP for DSCP untag rule, it's wrong");
            return CA_OMCI_OK;
        }

        if ((CA_OMCI_UNTAG_FWD_ADD_VID_DSCP_PRI == pUntag->fwdType)
         && (OMCI_HAL_ETH_TYPE_PPPOE != type) && (OMCI_HAL_ETH_TYPE_ARP != type) && (OMCI_HAL_ETH_TYPE_PPPOE2 != type)) {
            if (0 != pUsFlow->filterPri) {
                /* map to different gemport TODO fib_tx_cos need to modify depend on max queue per tcont */
                for (index = 0; index < OMCI_HAL_MAX_PRI_PER_FLOW; index++) {
                    if ((1 << index) & pUsFlow->filterPri) {
                        /* Get dscp value based on dot1p Map*/
                        for (dscpIdx = 0; dscpIdx < 64; dscpIdx++) {
                            if (index == sg_dscpMapTbl[dscpIdx]){
                                key.ip.dscp          = dscpIdx;
                                keyMask.ip_mask.dscp = 1;
                                keyMask.ip           = 1;

                                action.options.outer_vlan_act                   = CA_CLASSIFIER_VLAN_ACTION_PUSH;
                                action.options.outer_vid                        = pUntag->innerVlanTreatment.vid;
                                action.options.outer_dot1p                      = index;
                                action.options.masks.outer_vlan_act             = 1;
                                action.options.masks.outer_dot1p                = 1;

#if !defined(CONFIG_CA_OMCI_FPGA)
                                ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                                if (CA_E_OK != ret)
                                {
                                    OMCIHALERR("Add Tag for port[0x%x] failed ret[%d] to gemIdx[0x%x]",
                                                key.src_port, ret, action.options.action_handle.gem_index);
                                    return CA_OMCI_E;
                                }

                                if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for us stream");
                                    return CA_OMCI_E;
                                }
                            }
                        }
                    }
                }
            }
            else {
                OMCIHALWARN("can't set classifier rule filter pbit 0x%x", pUsFlow->filterPri);
            }
            return CA_OMCI_OK;
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_INN_DSCP_PRI == pUntag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pUntag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = pUntag->outerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.inner_vid            = pUntag->innerVlanTreatment.vid;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pUntag->outerVlanTreatment.vid;
            OMCIHALDBG("Add ovid[%d] opri[%d] ivid[%d] ipri-dscp",
                        action.options.outer_vid, action.options.outer_dot1p,
                        action.options.inner_vid);
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_OUT_DSCP_PRI == pUntag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pUntag->outerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.inner_vid            = pUntag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = pUntag->innerVlanTreatment.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1; //not support now
            vid                                 = pUntag->outerVlanTreatment.vid;
            OMCIHALDBG("Add ovid[%d] opri-dscp ivid[%d] ipri[%d]",
                        action.options.outer_vid,
                        action.options.inner_vid, action.options.inner_dot1p);
        }
        if (CA_OMCI_UNTAG_FWD_ADD_BOTH_VID_DSCP_PRI == pUntag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pUntag->outerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.inner_vid            = pUntag->innerVlanTreatment.vid;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pUntag->outerVlanTreatment.vid;
            OMCIHALDBG("Add ovid[%d] opri-dscp ivid[%d] ipri-dscp",
                        action.options.outer_vid, action.options.inner_vid);
        }
    }

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set untag filter eth[0x%x] failed ret[%d] for port[0x%x] to gemIdx[0x%x]",
                   type, ret, key.src_port, action.options.action_handle.gem_index);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
    {
        OMCIHALERR("can't save idx for us stream");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_sgltag(
    ca_omci_stream_cfg_t     *pUsFlow,
    ca_omci_sgl_tag_filter_t *pSgltag)
{
    ca_omci_onuinfo_obj_t    *onuinfo  = ca_omci_onuinfo_get();
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 0;
#endif
    ca_uint32_t               srcPort  = OMCI_HAL_VEIP_SRC_PORT;
    ca_uint32_t               ruleIdx  = 0;
    ca_uint32_t               vid      = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint32_t               uniPort  = OMCI_HAL_WAN_PORT;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE != pSgltag->isValid)
    {
        return CA_OMCI_OK;
    }

    if (OMCI_CFG_STREAM_PORT_ETH == pUsFlow->portType)
    {
        srcPort = ca_omci_hal_eth_port_get(pUsFlow->uniPortId);
        uniPort = srcPort;
    }

    OMCI_MEMSET(&key,     0, sizeof(key));
    OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
    OMCI_MEMSET(&action,  0, sizeof(action));
    OMCI_MEMSET(gemIdx,   0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pUsFlow->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                           = srcPort;
    key.l2.vlan_otag.vlan_min.vid          = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;
    key.l2.vlan_otag.vlan_max.vid          = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;
    key.l2.vlan_otag.vlan_min.pri          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : pSgltag->vlanFilter.pri;
    key.l2.vlan_otag.vlan_max.pri          = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : pSgltag->vlanFilter.pri;
    key.l2.vlan_count                      = 1;
    keyMask.src_port                       = 1;
    keyMask.l2_mask.vlan_count             = 1;
    keyMask.l2_mask.vlan_otag              = 1;
    keyMask.l2_mask.vlan_otag_mask.vid     = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : 1;
    keyMask.l2_mask.vlan_otag_mask.pri     = (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri) ? 0 : 1;
    keyMask.l2                             = 1;
    action.options.action_handle.gem_index = gemIdx[idx];
    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    action.options.masks.action_handle     = 1;
    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                       = OMCI_HAL_WAN_PORT;
    vid                                    = (OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid) ? 0 : pSgltag->vlanFilter.vid;

    //Check OMCI gemport index cos value, don't use the action.options.priority value
    action.options.masks.use_action_handle_priority = 1;

    if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
    {
        keyMask.l2_mask.vlan_otag = 0;
        OMCIHALDBG("Just filter single tag, no vid or pri specific");
    }

    if (CA_OMCI_FILTER_NONE != pSgltag->etherTypeFilter)
    {
        key.l2.ethertype          = ca_omci_hal_get_eth_type(pSgltag->etherTypeFilter);
        keyMask.l2_mask.ethertype = 1;
    }

    OMCIHALDBG("Single Tag match vid[%d] pri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]",
                key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                key.src_port, action.options.action_handle.gem_index, key.l2.ethertype);

    if (OMCI_OLT_TYPE_SIM == onuinfo->oltType){
        if ((0x88a8 == pUsFlow->outputTpid)
         && (OMCI_TREATMENT_TPID_SET_88A8_DEI_0 == pUsFlow->innerTpid.treat)) {
            action.options.outer_tpid       = 0x88a8;
            action.options.masks.outer_tpid = 1;
        }
    }

    if (CA_OMCI_SGL_TAG_FWD_TRANSPARENT == pSgltag->fwdType)
    {
        OMCIHALDBG("Transparent");

        if (0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri)
        {
            // set pbit when don't care filter pbit and don't do pbit treatment
            if ((0 == keyMask.l2_mask.vlan_otag_mask.pri)
             && (0 == action.options.masks.outer_dot1p))
            {
                for (idx = 0; idx < 8; idx++)
                {
                    if ((1 << idx) & pUsFlow->filterPri)
                    {
                        keyMask.l2_mask.vlan_otag          = 1;
                        key.l2.vlan_otag.vlan_min.pri      = idx;
                        key.l2.vlan_otag.vlan_max.pri      = idx;
                        keyMask.l2_mask.vlan_otag_mask.pri = 1;
                        action.options.outer_dot1p         = idx;
                        action.options.masks.outer_dot1p   = 1;
                        OMCIHALDBG("map pbit %d to gemport", idx);
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                        if (CA_E_OK == ret)
                        {
                            if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for us stream");
                            }
                        }

                        if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
                        {
                            OMCIHALDBG("Filter eth type 0x8864 too!");
                            key.l2.ethertype   = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK == ret)
                            {
                                if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for us stream");
                                }
                            }

                        }
                    }
                }
                return CA_OMCI_OK;
            }

            //only set the match one for filter or treat match
            if (1 == action.options.masks.outer_dot1p)
            {
                if (0 == ((1 << action.options.outer_dot1p) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
            else
            {
                if (0 == ((1 << key.l2.vlan_otag.vlan_max.pri) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
        }
    }
    else if (CA_OMCI_SGL_TAG_FWD_DROP == pSgltag->fwdType)
    {
        action.forward                         = CA_CLASSIFIER_FORWARD_DENY;
        action.options.action_handle.gem_index = 0;
        action.options.handle_type             = 0;
        action.options.masks.action_handle     = 0;
        action.dest.port                       = 0;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_SGL_TAG_FWD_ADD_TAG == pSgltag->fwdType)
    {
        if ((0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri)
         && (pSgltag->outerVlanTreatment.pri <= 7))
        {
            if (0 == ((1 << pSgltag->outerVlanTreatment.pri) &  pUsFlow->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pSgltag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pSgltag->outerVlanTreatment.vid;

        if ((OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->outerVlanTreatment.pri)
         && (OMCI_FILTER_PRI_NONE == pSgltag->vlanFilter.pri))
        {
            action.options.outer_dot1p       = 0;
            action.options.masks.outer_dot1p = 0;
        }

        if ((OMCI_FILTER_VID_NONE == pSgltag->vlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE <= pSgltag->vlanFilter.pri)
         && (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->outerVlanTreatment.pri)
         && (0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri))
        {
            for (idx = 0; idx < 8; idx++)
            {
                //if map more than 1 dot1p value, need to add more classifier rules here
                if ((1 << idx) & pUsFlow->filterPri)
                    break;
            }

            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;//use the map dot1p as key and action tag key
            key.l2.vlan_otag.vlan_min.pri       = idx;
            key.l2.vlan_otag.vlan_max.pri       = idx;
            action.options.outer_dot1p          = idx;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
        }

        OMCIHALDBG("Add vid[%d] pri[%d]", action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_SGL_TAG_FWD_MD_TAG == pSgltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pSgltag->innerVlanTreatment.vid;
        action.options.outer_dot1p          = pSgltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pSgltag->innerVlanTreatment.vid;

        if (OMCI_TREATMENT_ADD_TAG_COPY_INNER_PRI == pSgltag->innerVlanTreatment.pri)
        {
            action.options.outer_dot1p         = 0;
            action.options.masks.outer_dot1p   = 0;
            keyMask.l2_mask.vlan_otag_mask.pri = 0;
            OMCIHALDBG("Only Modify vid");
        }
        OMCIHALDBG("Modify vid[%d] pri[%d]", action.options.outer_vid, action.options.outer_dot1p);

        if (0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri)
        {
            // set pbit when don't care filter pbit and don't do pbit treatment
            if ((0 == keyMask.l2_mask.vlan_otag_mask.pri)
             && (0 == action.options.masks.outer_dot1p))
            {
                for (idx = 0; idx < 8; idx++)
                {
                    if ((1 << idx) & pUsFlow->filterPri)
                    {
                        key.l2.vlan_otag.vlan_min.pri      = idx;
                        key.l2.vlan_otag.vlan_max.pri      = idx;
                        keyMask.l2_mask.vlan_otag_mask.pri = 1;
                        action.options.outer_dot1p         = idx;
                        action.options.masks.outer_dot1p   = 1;
                        OMCIHALDBG("map pbit %d to gemport", idx);
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                        if (CA_E_OK == ret)
                        {
                            if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for us stream");
                            }
                        }

                        if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
                        {
                            OMCIHALDBG("Filter eth type 0x8864 too!");
                            key.l2.ethertype   = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK == ret)
                            {
                                if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for us stream");
                                }
                            }

                        }
                    }
                }
                return CA_OMCI_OK;
            }

            //only set the match one for filter or treat match
            if (1 == action.options.masks.outer_dot1p)
            {
                if (0 == ((1 << action.options.outer_dot1p) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
            else
            {
                if (0 == ((1 << key.l2.vlan_otag.vlan_max.pri) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
        }
    }
    else if (CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_TAG == pSgltag->fwdType)
    {
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pSgltag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pSgltag->innerVlanTreatment.pri;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1; //can't support now
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pSgltag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pSgltag->outerVlanTreatment.vid;
        OMCIHALDBG("Modify vid[%d] pri[%d] Add vid[%d] pri[%d]",
                   action.options.inner_vid, action.options.inner_dot1p,
                   action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_SGL_TAG_FWD_REMOVE_TAG == pSgltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;

        OMCIHALDBG("Remove out Tag");
    }
    else
    {
        //set dscp for all other relate dscp rules
        if (CA_OMCI_SGL_TAG_FWD_ADD_VID_DSCP == pSgltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = 0;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 0;
            vid                                 = pSgltag->outerVlanTreatment.vid;
            OMCIHALDBG("Add vid[%d] pri-dscp", action.options.outer_vid);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP == pSgltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pSgltag->innerVlanTreatment.vid;
            action.options.outer_dot1p          = 0;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 0;
            vid                                 = pSgltag->innerVlanTreatment.vid;
            OMCIHALDBG("Modify vid[%d] pri-dscp", action.options.outer_vid);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_TAG_ADD_VID_DSCP == pSgltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pSgltag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = pSgltag->innerVlanTreatment.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1; //can't support now
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = 0;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 0;
            vid                                 = pSgltag->outerVlanTreatment.vid;
            OMCIHALDBG("Modify vid[%d] pri[%d] Add vid[%d] pri-dscp",
                       action.options.inner_vid, action.options.inner_dot1p,
                       action.options.outer_vid);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_TAG == pSgltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pSgltag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = 0;
            action.options.masks.inner_vlan_act = 1;
            action.options.masks.inner_dot1p    = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = pSgltag->outerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            vid                                 = pSgltag->outerVlanTreatment.vid;
            OMCIHALDBG("Modify vid[%d] pri-dscp Add vid[%d] pri[%d]",
                       action.options.inner_vid,
                       action.options.outer_vid, action.options.outer_dot1p);
        }
        if (CA_OMCI_SGL_TAG_FWD_MD_VID_DSCP_ADD_VID_DSCP == pSgltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pSgltag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = 0;
            action.options.masks.inner_vlan_act = 1;
            action.options.masks.inner_dot1p    = 0;
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
            action.options.outer_vid            = pSgltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = 0;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 0;
            vid                                 = pSgltag->outerVlanTreatment.vid;
            OMCIHALDBG("Modify vid[%d] pri-dscp Add vid[%d] pri-dscp",
                       action.options.inner_vid, action.options.outer_vid);
        }
    }

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set Single Tag match vid[%d] pri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
    {
        OMCIHALERR("can't save idx for us stream");
        return CA_OMCI_E;
    }

    if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
    {
        OMCIHALDBG("Filter eth type 0x8864 too!");
        key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALDBG("Set Single Tag match vid[%d] pri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                        key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
            return CA_OMCI_E;
        }

        if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
        {
            OMCIHALERR("can't save idx for us stream");
            return CA_OMCI_E;
        }
    }

    //ret = ca_qos_config_set(OMCI_HAL_DEV_ID, uniPort, &config);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("set qos ctl for port[0x%x] failed ret[%d]", uniPort, ret);
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_us_flow_dbltag(
    ca_omci_stream_cfg_t     *pUsFlow,
    ca_omci_dbl_tag_filter_t *pDbltag)
{
    ca_classifier_key_t       key      = {0};
    ca_classifier_key_mask_t  keyMask  = {0};
    ca_classifier_action_t    action   = {0};
#if !defined(CONFIG_CA_OMCI_FPGA)
    ca_uint32_t               priority = 0;
#endif
    ca_uint32_t               srcPort  = OMCI_HAL_VEIP_SRC_PORT;
    ca_uint32_t               ruleIdx  = 0;
    ca_uint32_t               vid      = 0;
    ca_status_t               ret      = CA_E_OK;
    ca_uint32_t               uniPort  = OMCI_HAL_WAN_PORT;
    ca_uint16_t               idx      = 0;
    ca_uint16_t               gemNum   = 0;
    ca_uint16_t               gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    if (CA_OMCI_TRUE != pDbltag->isValid)
    {
        return CA_OMCI_OK;
    }

    if (OMCI_CFG_STREAM_PORT_ETH == pUsFlow->portType)
    {
        srcPort = ca_omci_hal_eth_port_get(pUsFlow->uniPortId);
        uniPort = srcPort;
    }

    OMCI_MEMSET(&key,     0, sizeof(key));
    OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
    OMCI_MEMSET(&action,  0, sizeof(action));
    OMCI_MEMSET(gemIdx,   0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pUsFlow->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    key.src_port                           = srcPort;
    key.l2.vlan_otag.vlan_min.vid          = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : pDbltag->outerVlanFilter.vid;
    key.l2.vlan_otag.vlan_max.vid          = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : pDbltag->outerVlanFilter.vid;
    key.l2.vlan_otag.vlan_min.pri          = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : pDbltag->outerVlanFilter.pri;
    key.l2.vlan_otag.vlan_max.pri          = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : pDbltag->outerVlanFilter.pri;
    key.l2.vlan_count                      = 2;
    keyMask.src_port                       = 1;
    keyMask.l2_mask.vlan_count             = 1;
    keyMask.l2_mask.vlan_otag              = 1;
    keyMask.l2_mask.vlan_otag_mask.vid     = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : 1;
    keyMask.l2_mask.vlan_otag_mask.pri     = (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri) ? 0 : 1;
    keyMask.l2                             = 1;
    action.options.action_handle.gem_index = gemIdx[idx];
    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
    action.options.masks.action_handle     = 1;
    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
    action.dest.port                       = OMCI_HAL_WAN_PORT;
    vid                                    = (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid) ? 0 : pDbltag->outerVlanFilter.vid;

    //Check OMCI gemport index cos value, don't use the action.options.priority value
    action.options.masks.use_action_handle_priority = 1;

    if ((OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri))
    {
        keyMask.l2_mask.vlan_itag = 0;
        OMCIHALDBG("Inner vid and pri don't specific");
    }

    if ((OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
     && (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri))
    {
        keyMask.l2_mask.vlan_otag = 0;
        OMCIHALDBG("Outer vid and pri don't specific");
    }

    if (CA_OMCI_FILTER_NONE != pDbltag->etherTypeFilter)
    {
        key.l2.ethertype          = ca_omci_hal_get_eth_type(pDbltag->etherTypeFilter);
        keyMask.l2_mask.ethertype = 1;
    }

    OMCIHALDBG("Double Tag match ovid[%d] opri[%d] ivid [%d] ipri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]",
                key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                key.src_port, action.options.action_handle.gem_index, key.l2.ethertype);

    if (CA_OMCI_DBL_TAG_FWD_TRANSPARENT == pDbltag->fwdType)
    {
        OMCIHALDBG("Transparent");
    }
    else if (CA_OMCI_DBL_TAG_FWD_DROP == pDbltag->fwdType)
    {
        action.forward                         = CA_CLASSIFIER_FORWARD_DENY;
        action.options.action_handle.gem_index = 0;
        action.options.masks.action_handle     = 0;
        action.dest.port                       = 0;
        action.options.handle_type             = 0;
        OMCIHALDBG("Drop");
    }
    else if (CA_OMCI_DBL_TAG_ADD_TAG == pDbltag->fwdType)
    {
        if ((0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri)
         && (pDbltag->outerVlanTreatment.pri <= 7))
        {
            if (0 == ((1 << pDbltag->outerVlanTreatment.pri) &  pUsFlow->filterPri))
            {
                OMCIHALDBG("skip the unmatch one");
                return CA_OMCI_OK;
            }
        }

        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_PUSH;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        if ((OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == pDbltag->outerVlanTreatment.pri)
         && (OMCI_FILTER_PRI_NONE == pDbltag->outerVlanFilter.pri))
        {
            action.options.outer_dot1p       = 0;
            action.options.masks.outer_dot1p = 0;
        }

        if ((OMCI_FILTER_VID_NONE == pDbltag->innerVlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE == pDbltag->innerVlanFilter.pri)
         && (OMCI_FILTER_VID_NONE == pDbltag->outerVlanFilter.vid)
         && (OMCI_FILTER_PRI_NONE <= pDbltag->outerVlanFilter.pri)
         && (OMCI_TREATMENT_ADD_TAG_COPY_OUTER_PRI == pDbltag->outerVlanTreatment.pri)
         && (0 != pUsFlow->filterPri && 0xff != pUsFlow->filterPri))
        {
            for (idx = 0; idx < 8; idx++)
            {
                //if map more than 1 dot1p value, need to add more classifier rules here
                if ((1 << idx) & pUsFlow->filterPri)
                    break;
            }

            keyMask.l2_mask.vlan_otag           = 1;
            keyMask.l2_mask.vlan_otag_mask.pri  = 1;//use the map dot1p as key and action tag key
            key.l2.vlan_otag.vlan_min.pri       = idx;
            key.l2.vlan_otag.vlan_max.pri       = idx;
            action.options.outer_dot1p          = idx;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
        }

        OMCIHALDBG("Add vid[%d] pri[%d]",
                   action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_OTAG == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        vid                                 = pDbltag->innerVlanFilter.vid;

        OMCIHALDBG("Remove otag");
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] opri[%d]",
                   action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.masks.outer_vlan_act = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] only", action.options.outer_vid);

        //if the pbit filter is not empty, need to set every pbit for this rule
        if ((0 != pUsFlow->filterPri) && (0xff != pUsFlow->filterPri))
        {
            // set pbit when don't care filter pbit and don't do pbit treatment
            if ((0 == keyMask.l2_mask.vlan_otag_mask.pri)
             && (0 == action.options.masks.outer_dot1p))
            {
                for (idx = 0; idx < 8; idx++)
                {
                    if ((1 << idx) & pUsFlow->filterPri)
                    {
                        key.l2.vlan_otag.vlan_min.pri      = idx;
                        key.l2.vlan_otag.vlan_max.pri      = idx;
                        keyMask.l2_mask.vlan_otag_mask.pri = 1;
                        action.options.outer_dot1p         = idx;
                        action.options.masks.outer_dot1p   = 1;
                        OMCIHALDBG("map pbit %d to gemport", idx);
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                        if (CA_E_OK == ret)
                        {
                            if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                            {
                                OMCIHALERR("can't save idx for us stream");
                            }
                        }

                        if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
                        {
                            OMCIHALDBG("Filter eth type 0x8864 too!");
                            key.l2.ethertype   = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
                            if (CA_E_OK == ret)
                            {
                                if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
                                {
                                    OMCIHALERR("can't save idx for us stream");
                                }
                            }

                        }
                    }
                }
                return CA_OMCI_OK;
            }

            //only set the match one for filter or treat match
            if (1 == action.options.masks.outer_dot1p)
            {
                if (0 == ((1 << action.options.outer_dot1p) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }
            else
            {
                if (0 == ((1 << key.l2.vlan_otag.vlan_max.pri) &  pUsFlow->filterPri))
                {
                    OMCIHALDBG("skip the unmatch one");
                    return CA_OMCI_OK;
                }
            }

            return CA_OMCI_OK;
        }
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;

        OMCIHALDBG("Change opri[%d] only", action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_TAGS == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] opri[%d] ivid[%d] ipri[%d]",
                   action.options.outer_vid, action.options.outer_dot1p,
                   action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] opri[%d] ivid[%d]",
                   action.options.outer_vid, action.options.outer_dot1p,
                   action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_TAG == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.outer_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        vid                                 = pDbltag->innerVlanTreatment.vid;

        OMCIHALDBG("Remove otag change vid[%d] pri[%d]",
                    action.options.outer_vid, action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IPRI == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] opri[%d] ipri[%d]",
                   action.options.outer_vid, action.options.outer_dot1p,
                   action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ITAG == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] ivid[%d] ipri[%d]",
                   action.options.outer_vid, action.options.inner_vid,
                   action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] ivid[%d]",
                   action.options.outer_vid, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_IPRI == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;
        vid                                 = pDbltag->outerVlanTreatment.vid;

        OMCIHALDBG("Change ovid[%d] ipri[%d]",
                   action.options.outer_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.outer_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        vid                                 = pDbltag->innerVlanTreatment.vid;

        OMCIHALDBG("Remove otag change vid[%d]", action.options.outer_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAGS == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        vid                                 = 0;

        OMCIHALDBG("Remove both tags");
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_PRI == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.outer_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.inner_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;

        vid = pDbltag->innerVlanFilter.vid;
        OMCIHALDBG("Remove otag change pri[%d]", action.options.outer_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_RM_ITAG == pDbltag->fwdType)
    {
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
        action.options.masks.inner_vlan_act = 1;

        OMCIHALDBG("Remove inner tag");
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_ITAG == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;

        OMCIHALDBG("Change opri[%d] ivid[%d] ipri[%d]",
                   action.options.outer_dot1p,
                   action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;

        OMCIHALDBG("Change opri[%d] ivid[%d]",
                   action.options.outer_dot1p, action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_PRIS == pDbltag->fwdType)
    {
        action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.outer_vlan_act = 1;
        action.options.masks.outer_dot1p    = 1;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;

        OMCIHALDBG("Change opri[%d] ipri[%d]",
                   action.options.outer_dot1p, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_ITAG == pDbltag->fwdType)
    {
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;

        OMCIHALDBG("Change ivid[%d] ipri[%d]",
                   action.options.inner_vid, action.options.inner_dot1p);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IVID == pDbltag->fwdType)
    {
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
        action.options.masks.inner_vlan_act = 1;

        OMCIHALDBG("Change ivid[%d]", action.options.inner_vid);
    }
    else if (CA_OMCI_DBL_TAG_FWD_MD_IPRI == pDbltag->fwdType)
    {
        action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
        action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
        action.options.masks.inner_vlan_act = 1;
        //action.options.masks.inner_dot1p    = 1;

        OMCIHALDBG("Change ipri[%d]", action.options.inner_dot1p);
    }
    else
    {
        if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri map dscp", action.options.outer_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP == pDbltag->fwdType)
        {
            OMCIHALDBG("Change opri map dscp");
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IVID_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri[%d] ivid[%d] ipri map dscp",
                       action.options.outer_vid, action.options.outer_dot1p,
                       action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OTAG_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri[%d] ipri map dscp",
                       action.options.outer_vid, action.options.outer_dot1p);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] ivid[%d] ipri map dscp",
                       action.options.outer_vid, action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] ipri map dscp", action.options.outer_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_ITAG == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri map dscp ivid[%d] ipri[%d]",
                       action.options.outer_vid,
                       action.options.inner_vid, action.options.inner_dot1p);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_ODSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri map dscp ivid[%d]",
                       action.options.outer_vid, action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_VIDS_DSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] ivid[%d] pri map dscp",
                       action.options.outer_vid, action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_ODSCP_IPRI == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] opri map dscp ipri[%d]",
                       action.options.outer_vid, action.options.inner_dot1p);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OVID_DSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_vid            = pDbltag->outerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            vid                                 = pDbltag->outerVlanTreatment.vid;

            OMCIHALDBG("Change ovid[%d] pri map dscp", action.options.outer_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_MD_VID_DSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.outer_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.inner_vlan_act = 1;
            vid                                 = pDbltag->innerVlanTreatment.vid;

            OMCIHALDBG("Remove tag and change vid[%d] pri map dscp", action.options.outer_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_RM_TAG_DSCP == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_POP;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Remove tag and change pri map dscp");
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_IVID_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Change opri[%d] ivid[%d] ipri map dscp",
                       action.options.outer_dot1p, action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_OPRI_IDSCP == pDbltag->fwdType)
        {
            action.options.outer_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.outer_dot1p          = pDbltag->outerVlanTreatment.pri;
            action.options.masks.outer_vlan_act = 1;
            action.options.masks.outer_dot1p    = 1;

            OMCIHALDBG("Change opri[%d] ipri map dscp", action.options.outer_dot1p);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_IVID_IDSCP == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Change ivid[%d] ipri map dscp", action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_IDSCP == pDbltag->fwdType)
        {
            OMCIHALDBG("Change ipri map dscp");
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_ITAG == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1;

            OMCIHALDBG("Change opri map dscp ivid[%d] ipri[%d]",
                       action.options.inner_vid, action.options.inner_dot1p);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IVID == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Change opri map dscp ivid[%d]", action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_DSCP_IVID == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_vid            = pDbltag->innerVlanTreatment.vid;
            action.options.masks.inner_vlan_act = 1;

            OMCIHALDBG("Change ivid[%d] pri map dscp", action.options.inner_vid);
        }
        else if (CA_OMCI_DBL_TAG_FWD_MD_ODSCP_IPRI == pDbltag->fwdType)
        {
            action.options.inner_vlan_act       = CA_CLASSIFIER_VLAN_ACTION_SWAP;
            action.options.inner_dot1p          = pDbltag->innerVlanTreatment.pri;
            action.options.masks.inner_vlan_act = 1;
            //action.options.masks.inner_dot1p    = 1;

            OMCIHALDBG("Change opri map dscp ipri[%d]", action.options.inner_dot1p);
        }
        else
        {
            //if (CA_OMCI_DBL_TAG_FWD_MD_DSCP == pDbltag->fwdType)
            OMCIHALDBG("Change pri map dscp");
        }
    }

#if !defined(CONFIG_CA_OMCI_FPGA)
    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Set Double Tag match ovid[%d] opri[%d] ivid[%d] ipri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                    key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                    key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                    key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
        return CA_OMCI_E;
    }

    if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
    {
        OMCIHALERR("can't save idx for us stream");
        return CA_OMCI_E;
    }

    if (OMCI_HAL_ETH_TYPE_PPPOE == key.l2.ethertype)
    {
        OMCIHALDBG("Filter eth type 0x8864 too!");
        key.l2.ethertype = OMCI_HAL_ETH_TYPE_PPPOE2;
#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &ruleIdx);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALERR("Set Double Tag match ovid[%d] opri[%d] ivid[%d] ipri[%d] from port[0x%x] to gemIdx[0x%x] Filter eth[0x%x]failed ret[%d] ",
                        key.l2.vlan_otag.vlan_min.vid, key.l2.vlan_otag.vlan_min.pri,
                        key.l2.vlan_itag.vlan_min.vid, key.l2.vlan_itag.vlan_min.pri,
                        key.src_port, action.options.action_handle.gem_index, key.l2.ethertype, ret);
            return CA_OMCI_E;
        }

        if (CA_OMCI_OK != ca_omci_hal_flow_classifier_idx_save(pUsFlow, ruleIdx))
        {
            OMCIHALERR("can't save idx for us stream");
            return CA_OMCI_E;
        }
    }
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_filter_flow_add(void *pData)
{
    ca_omci_filter_stream_cfg_t   *pFilter  = (ca_omci_filter_stream_cfg_t *)pData;
    ca_port_id_t                   portId   = OMCI_HAL_VEIP_SRC_PORT;
    ca_omci_uint32_t               vidIdx   = 0;
    ca_omci_uint32_t               priIdx   = 0;
    ca_uint32_t                    priority = 0;
    ca_uint32_t                    index    = 0;
    ca_status_t                    ret      = CA_E_OK;
    ca_port_id_t                   wanPort  = OMCI_HAL_WAN_PORT;
    ca_classifier_key_t            key      = {0};
    ca_classifier_key_mask_t       keyMask  = {0};
    ca_classifier_action_t         action   = {0};
    ca_classifier_key_t            dsKey     = {0};
    ca_classifier_key_mask_t       dsKeyMask = {0};
    ca_classifier_action_t         dsAction  = {0};
    ca_uint16_t                    idx       = 0;
    ca_uint16_t                    gemNum    = 0;
    ca_omci_stream_cfg_t           flow      = {0};
    ca_uint16_t                    gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    HAL_CHECK_TCL;

    if (GEM_NET_CTP_DIR_DS == pFilter->direction)
    {
        OMCIHALDBG("Don't add ds filter rule now, add later");
        return CA_OMCI_OK;
    }

    if (CA_OMCI_TRUE == pFilter->externVlanFlag)
    {
        OMCIHALWARN("Don't add filter rule only use extern vlan table result");
        return CA_OMCI_OK;
    }

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);

    gemNum = ca_omci_hal_gem_get_idx_from_id(pFilter->gemId, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return CA_OMCI_E;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    if (OMCI_CFG_STREAM_PORT_ETH == pFilter->portType)
    {
        portId = ca_omci_hal_eth_port_get(pFilter->uniPortId);
    }

    //set TPID first and ds Queue
    flow.inputTpid     = pFilter->inputTpid;
    flow.outputTpid    = pFilter->outputTpid;
    flow.uniPortId     = pFilter->uniPortId;
    flow.portType      = pFilter->portType;
    flow.dsQueue       = pFilter->dsQueue;
    flow.dsQueueWeight = pFilter->dsQueueWeight;
    flow.gemId         = pFilter->gemId;
    ca_omci_hal_tpid_set(&flow);

    if (CA_OMCI_OK != ca_omci_hal_ds_queue_set(&flow, CA_OMCI_TRUE))
    {
        OMCIHALERR("Set ds queue failed");
    }

    //No filter on vid or pri
    if ((0xff == pFilter->priBit || 0 == pFilter->priBit) && (0 == pFilter->vidList[0]))
    {
        OMCI_MEMSET(&key,     0, sizeof(key));
        OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
        OMCI_MEMSET(&action,  0, sizeof(action));
        OMCI_MEMSET(&dsKey,     0, sizeof(dsKey));
        OMCI_MEMSET(&dsKeyMask, 0, sizeof(dsKeyMask));
        OMCI_MEMSET(&dsAction,  0, sizeof(dsAction));
        priority = 0;
        index    = 0;

        //us
        key.src_port                           = portId;
        keyMask.src_port                       = 1;
        action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
        action.dest.port                       = wanPort;
        action.options.action_handle.gem_index = gemIdx[idx];
        action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
        action.options.masks.action_handle     = 1;

        //Check OMCI gemport index cos value, don't use the action.options.priority value
        action.options.masks.use_action_handle_priority = 1;

#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALERR("add classifier failed ret[%d] key src port [0x%x] dest port[0x%x] gemIdx[0x%x]",
                        ret, key.src_port, action.dest.port, gemIdx[idx]);
            return CA_OMCI_E;
        }

        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
        {
            OMCIHALERR("can't save idx for us stream");
            return CA_OMCI_E;
        }

        //ds
        if (OMCI_CFG_STREAM_PORT_ETH == pFilter->portType)
        {
            index                           = 0;
            dsKey.src_port                  = OMCI_HAL_WAN_PORT;
            dsKey.key_handle.gem_index      = gemIdx[idx];
            dsKey.handle_type               = CA_KEY_HANDLE_TYPE_GEM_INDEX;
            dsKeyMask.src_port              = 1;
            dsKeyMask.key_handle            = 1;
            dsAction.forward                = CA_CLASSIFIER_FORWARD_PORT;
            dsAction.dest.port              = ca_omci_hal_eth_port_get(pFilter->uniPortId);
            dsAction.options.priority       = ca_omci_hal_ds_pq_idx_get(pFilter->dsQueue);
            dsAction.options.masks.priority = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
            if (CA_E_OK != ret)
            {
                OMCIHALERR("add ds classifier failed ret[%d] key src port [0x%x] dest port[0x%x] gemIdx[0x%x]",
                            ret, dsKey.src_port, dsAction.dest.port, gemIdx[idx]);
                return CA_OMCI_E;
            }

            if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
            {
                OMCIHALERR("can't save idx for ds stream");
                return CA_OMCI_E;
            }
        }
        return ret;
    }

    // if filter pri
    if (0xff != pFilter->priBit && 0 != pFilter->priBit)
    {
        //don't filter vlan, just filter priority
        if (0 == pFilter->vidList[0])
        {
            for (priIdx = 0; priIdx < OMCI_HAL_MAX_PRI_PER_FLOW; priIdx++)
            {
                if ((1 << priIdx) & pFilter->priBit)
                {
                    OMCI_MEMSET(&key,     0, sizeof(key));
                    OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
                    OMCI_MEMSET(&action,  0, sizeof(action));
                    priority = 0;
                    index    = 0;

                    key.l2.vlan_otag.vlan_min.pri          = priIdx;
                    key.l2.vlan_otag.vlan_max.pri          = priIdx;
                    key.src_port                           = portId;
                    keyMask.src_port                       = 1;
                    keyMask.l2                             = 1;
                    keyMask.l2_mask.vlan_otag              = 1;
                    keyMask.l2_mask.vlan_otag_mask.pri     = 1;
                    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
                    action.dest.port                       = wanPort;
                    action.options.action_handle.gem_index = gemIdx[idx];
                    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
                    action.options.masks.action_handle     = 1;
                    OMCIHALDBG("add classifier key otag pri[%d] src port[0x%x] dest port[0x%x] flow[0x%x] ",
                               key.l2.vlan_otag.vlan_min.pri, key.src_port, action.dest.port, gemIdx[idx]);

                    //Check OMCI gemport index cos value, don't use the action.options.priority value
                    action.options.masks.use_action_handle_priority = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                    ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
                    if (CA_E_OK != ret)
                    {
                        OMCIHALERR("add classifier failed ret[%d] key otag pri[%d] src port[0x%x] dest port[0x%x] flow[0x%x] priority[%d]",
                                    ret, key.l2.vlan_otag.vlan_min.pri, key.src_port, action.dest.port, gemIdx[idx], action.options.priority);
                        return CA_OMCI_E;
                    }

                    if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
                    {
                        OMCIHALERR("can't save idx for us stream");
                        return CA_OMCI_E;
                    }

                    //ds
                    if (OMCI_CFG_STREAM_PORT_ETH == pFilter->portType)
                    {
                        index                                = 0;
                        dsKey.src_port                       = OMCI_HAL_WAN_PORT;
                        dsKey.key_handle.gem_index           = gemIdx[idx];
                        dsKey.handle_type                    = CA_KEY_HANDLE_TYPE_GEM_INDEX;
                        dsKey.l2.vlan_otag.vlan_min.pri      = priIdx;
                        dsKey.l2.vlan_otag.vlan_max.pri      = priIdx;
                        dsKeyMask.src_port                   = 1;
                        dsKeyMask.key_handle                 = 1;
                        dsKeyMask.l2                         = 1;
                        dsKeyMask.l2_mask.vlan_otag          = 1;
                        dsKeyMask.l2_mask.vlan_otag_mask.pri = 1;
                        dsAction.forward                     = CA_CLASSIFIER_FORWARD_PORT;
                        dsAction.dest.port                   = ca_omci_hal_eth_port_get(pFilter->uniPortId);
                        dsAction.options.priority            = ca_omci_hal_ds_pq_idx_get(pFilter->dsQueue);
                        dsAction.options.masks.priority      = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
                        if (CA_E_OK != ret)
                        {
                            OMCIHALERR("add ds classifier failed ret[%d] key src port [0x%x] dest port[0x%x] gemIdx[0x%x]",
                                        ret, dsKey.src_port, dsAction.dest.port, gemIdx[idx]);
                            return CA_OMCI_E;
                        }
                        if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
                        {
                            OMCIHALERR("can't save idx for ds stream");
                            return CA_OMCI_E;
                        }
                    }
                }
            }
            return CA_OMCI_OK;
        }

        //filter pri + vid
        for (priIdx = 0; priIdx < OMCI_HAL_MAX_PRI_PER_FLOW; priIdx++)
        {
            if ((1 << priIdx) & pFilter->priBit)
            {
                for (vidIdx = 0; vidIdx < OMCI_VLAN_LIST_NUM; vidIdx++)
                {
                    if (0 == pFilter->vidList[vidIdx])
                    {
                        break;
                    }

                    OMCI_MEMSET(&key,     0, sizeof(key));
                    OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
                    OMCI_MEMSET(&action,  0, sizeof(action));
                    priority = 0;
                    index    = 0;

                    key.l2.vlan_otag.vlan_min.pri          = priIdx;
                    key.l2.vlan_otag.vlan_max.pri          = priIdx;
                    key.l2.vlan_otag.vlan_min.vid          = pFilter->vidList[vidIdx];
                    key.l2.vlan_otag.vlan_max.vid          = pFilter->vidList[vidIdx];
                    key.src_port                           = portId;
                    keyMask.src_port                       = 1;
                    keyMask.l2                             = 1;
                    keyMask.l2_mask.vlan_otag              = 1;
                    keyMask.l2_mask.vlan_otag_mask.pri     = 1;
                    keyMask.l2_mask.vlan_otag_mask.vid     = 1;
                    action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
                    action.dest.port                       = wanPort;
                    action.options.action_handle.gem_index = gemIdx[idx];
                    action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
                    action.options.masks.action_handle     = 1;

                    //Check OMCI gemport index cos value, don't use the action.options.priority value
                    action.options.masks.use_action_handle_priority = 1;

                    if (PRI_QUEUE_COLOR_MARK_DEI == pFilter->usQueueDpcm)
                    {
                        //add two classifier rules for 1 pbit
                        key.l2.vlan_otag.vlan_min.dei      = 1;
                        key.l2.vlan_otag.vlan_max.dei      = 1;
                        keyMask.l2_mask.vlan_otag_mask.dei = 1;
                        action.options.mark                = 1;
                        action.options.masks.mark          = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
                        if (CA_E_OK != ret)
                        {
                            OMCIHALERR("add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                        ret, key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx], action.options.priority);
                            continue;
                        }
                        OMCIHALDBG("add classifier dei 1 mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                    key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx],
                                    action.options.priority, index);

                        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
                        {
                            OMCIHALERR("can't save idx for us stream");
                            continue;
                        }

                        key.l2.vlan_otag.vlan_min.dei      = 0;
                        key.l2.vlan_otag.vlan_max.dei      = 0;
                        keyMask.l2_mask.vlan_otag_mask.dei = 1;
                        action.options.mark                = 0;
                        action.options.masks.mark          = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
                        if (CA_E_OK != ret)
                        {
                            OMCIHALERR("add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                        ret, key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx], action.options.priority);
                            continue;
                        }
                        OMCIHALDBG("add classifier dei 0 no mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                    key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx],
                                    action.options.priority, index);

                        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
                        {
                            OMCIHALERR("can't save idx for us stream");
                            continue;
                        }
                    }
                    else if ((PRI_QUEUE_COLOR_MARK_PCP_6P2D == pFilter->usQueueDpcm)
                         && ((2 == priIdx) || (4 == priIdx)))
                    {
                        //only set pbit 2 and 4 for TR247 case
                        //add two classifier rules for 1 pbit
                        action.options.mark       = 1;
                        action.options.masks.mark = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
                        if (CA_E_OK != ret)
                        {
                            OMCIHALERR("add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                        ret, key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx], action.options.priority);
                            continue;
                        }
                        OMCIHALDBG("add classifier mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                    key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx],
                                    action.options.priority, index);

                        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
                        {
                            OMCIHALERR("can't save idx for us stream");
                            continue;
                        }
                    }
                    else
                    {
#if !defined(CONFIG_CA_OMCI_FPGA)
                        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
                        if (CA_E_OK != ret)
                        {
                            OMCIHALERR("add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                        ret, key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx], action.options.priority);
                            continue;
                        }
                        OMCIHALDBG("add classifier key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                    key.l2.vlan_otag.vlan_min.pri, key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx],
                                    action.options.priority, index);

                        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
                        {
                            OMCIHALERR("can't save idx for us stream");
                            continue;
                        }
                    }

                    //ds
                    if (OMCI_CFG_STREAM_PORT_ETH == pFilter->portType)
                    {
                        index                                = 0;
                        dsKey.src_port                       = OMCI_HAL_WAN_PORT;
                        dsKey.key_handle.gem_index           = gemIdx[idx];
                        dsKey.handle_type                    = CA_KEY_HANDLE_TYPE_GEM_INDEX;
                        dsKey.l2.vlan_otag.vlan_min.pri      = priIdx;
                        dsKey.l2.vlan_otag.vlan_max.pri      = priIdx;
                        dsKey.l2.vlan_otag.vlan_min.vid      = pFilter->vidList[vidIdx];
                        dsKey.l2.vlan_otag.vlan_max.vid      = pFilter->vidList[vidIdx];
                        dsKeyMask.src_port                   = 1;
                        dsKeyMask.key_handle                 = 1;
                        dsKeyMask.l2                         = 1;
                        dsKeyMask.l2_mask.vlan_otag          = 1;
                        dsKeyMask.l2_mask.vlan_otag_mask.pri = 1;
                        dsKeyMask.l2_mask.vlan_otag_mask.vid = 1;
                        dsAction.forward                     = CA_CLASSIFIER_FORWARD_PORT;
                        dsAction.dest.port                   = ca_omci_hal_eth_port_get(pFilter->uniPortId);
                        dsAction.options.priority            = ca_omci_hal_ds_pq_idx_get(pFilter->dsQueue);
                        dsAction.options.masks.priority      = 1;

                        if (PRI_QUEUE_COLOR_MARK_DEI == pFilter->dsQueueDpcm)
                        {
                            //add two classifier rules for 1 pbit
                            dsKey.l2.vlan_otag.vlan_min.dei      = 1;
                            dsKey.l2.vlan_otag.vlan_max.dei      = 1;
                            dsKeyMask.l2_mask.vlan_otag_mask.dei = 1;
                            dsAction.options.mark                = 1;
                            dsAction.options.masks.mark          = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
                            if (CA_E_OK != ret)
                            {
                                OMCIHALERR("ds add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                            ret, dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx], dsAction.options.priority);
                                continue;
                            }
                            OMCIHALDBG("ds add classifier dei 1 mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                        dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx],
                                        dsAction.options.priority, index);

                            if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
                            {
                                OMCIHALERR("can't save idx for ds stream");
                                continue;
                            }

                            dsKey.l2.vlan_otag.vlan_min.dei      = 0;
                            dsKey.l2.vlan_otag.vlan_max.dei      = 0;
                            dsKeyMask.l2_mask.vlan_otag_mask.dei = 1;
                            dsAction.options.mark                = 0;
                            dsAction.options.masks.mark          = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
                            if (CA_E_OK != ret)
                            {
                                OMCIHALERR("ds add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                            ret, dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx], dsAction.options.priority);
                                continue;
                            }
                            OMCIHALDBG("ds add classifier dei 0 no mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                        dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx],
                                        dsAction.options.priority, index);

                            if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
                            {
                                OMCIHALERR("can't save idx for us stream");
                                continue;
                            }
                        }
                        else if ((PRI_QUEUE_COLOR_MARK_PCP_6P2D == pFilter->dsQueueDpcm)
                             && ((2 == priIdx) || (4 == priIdx)))
                        {
                            //only set pbit 2 and 4 for TR247 case
                            //add two classifier rules for 1 pbit
                            dsAction.options.mark       = 1;
                            dsAction.options.masks.mark = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
                            if (CA_E_OK != ret)
                            {
                                OMCIHALERR("ds add classifier failed ret[%d] key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d]",
                                            ret, dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx], dsAction.options.priority);
                                continue;
                            }
                            OMCIHALDBG("ds add classifier mark key otag pri[%d] vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                                        dsKey.l2.vlan_otag.vlan_min.pri, dsKey.l2.vlan_otag.vlan_min.vid, dsKey.src_port, dsAction.dest.port, gemIdx[idx],
                                        dsAction.options.priority, index);

                            if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
                            {
                                OMCIHALERR("can't save idx for us stream");
                                continue;
                            }
                        }
                        else
                        {
#if !defined(CONFIG_CA_OMCI_FPGA)
                            ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &dsKey, &dsKeyMask, &dsAction, &index);
#endif
                            if (CA_E_OK != ret)
                            {
                                OMCIHALERR("add ds classifier failed ret[%d] key src port [0x%x] dest port[0x%x] gemIdx[0x%x]",
                                            ret, dsKey.src_port, dsAction.dest.port, gemIdx[idx]);
                                return CA_OMCI_E;
                            }
                            if (CA_OMCI_OK != ca_omci_hal_ds_filter_classifier_idx_save(pFilter, index))
                            {
                                OMCIHALERR("can't save idx for ds stream");
                                return CA_OMCI_E;
                            }
                        }
                    }
                }
            }
        }
        return CA_OMCI_OK;
    }

    //if only filter vlan create vlan and set vlan port
    for (vidIdx = 0; vidIdx < OMCI_VLAN_LIST_NUM; vidIdx++)
    {
        if (0 == pFilter->vidList[vidIdx])
        {
            break;
        }

        OMCI_MEMSET(&key,     0, sizeof(key));
        OMCI_MEMSET(&keyMask, 0, sizeof(keyMask));
        OMCI_MEMSET(&action,  0, sizeof(action));
        priority = 0;
        index    = 0;

        key.l2.vlan_otag.vlan_min.vid          = pFilter->vidList[vidIdx];
        key.l2.vlan_otag.vlan_max.vid          = pFilter->vidList[vidIdx];
        key.src_port                           = portId;
        keyMask.src_port                       = 1;
        keyMask.l2                             = 1;
        keyMask.l2_mask.vlan_otag              = 1;
        keyMask.l2_mask.vlan_otag_mask.vid     = 1;
        action.forward                         = CA_CLASSIFIER_FORWARD_PORT;
        action.dest.port                       = wanPort;
        action.options.action_handle.gem_index = gemIdx[idx];
        action.options.handle_type             = CA_KEY_HANDLE_TYPE_GEM_INDEX;
        action.options.masks.action_handle     = 1;

        //Check OMCI gemport index cos value, don't use the action.options.priority value
        action.options.masks.use_action_handle_priority = 1;
#if !defined(CONFIG_CA_OMCI_FPGA)
        ret = ca_classifier_rule_add(OMCI_HAL_DEV_ID, priority, &key, &keyMask, &action, &index);
#endif
        if (CA_E_OK != ret)
        {
            OMCIHALERR("add classifier failed ret[%d] key vid[%d] dest port[0x%x]",
                        ret, key.l2.vlan_otag.vlan_min.vid, action.dest.port);
            continue;
        }
        OMCIHALDBG("add classifier key otag vid [%d] src port [0x%x] dest port[0x%x] gemIdx[0x%x] priority[%d] index[%d]",
                    key.l2.vlan_otag.vlan_min.vid, key.src_port, action.dest.port, gemIdx[idx], priority, index);

        if (CA_OMCI_OK != ca_omci_hal_filter_classifier_idx_save(pFilter, index))
        {
            OMCIHALERR("can't save idx for us stream");
            continue;
        }
    }

    return CA_OMCI_OK;
}


static ca_omci_hal_ds_flow_cb_t sg_halDsFlowAddCb[] =
{
    {EXT_VLAN_TAG_OP_DS_MODE_INVERSE,              ca_omci_hal_ds_flow_inverse_untag,
     ca_omci_hal_ds_flow_inverse_sgltag,        ca_omci_hal_ds_flow_inverse_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FW_UNMODIFY,          ca_omci_hal_ds_flow_fw_untag,
     ca_omci_hal_ds_flow_fw_sgltag,             ca_omci_hal_ds_flow_fw_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PBIT_PASS, ca_omci_hal_ds_flow_tag_pass_untag,
     ca_omci_hal_ds_flow_tag_pass_sgltag,       ca_omci_hal_ds_flow_tag_pass_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PASS,      ca_omci_hal_ds_flow_vid_pass_untag,
     ca_omci_hal_ds_flow_vid_pass_sgltag,       ca_omci_hal_ds_flow_vid_pass_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_PBIT_PASS,     ca_omci_hal_ds_flow_pbit_pass_untag,
     ca_omci_hal_ds_flow_pbit_pass_sgltag,      ca_omci_hal_ds_flow_pbit_pass_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PBIT_DIS,  ca_omci_hal_ds_flow_tag_dis_untag,
     ca_omci_hal_ds_flow_tag_dis_sgltag,        ca_omci_hal_ds_flow_tag_dis_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_DIS,       ca_omci_hal_ds_flow_vid_dis_untag,
     ca_omci_hal_ds_flow_vid_dis_sgltag,        ca_omci_hal_ds_flow_vid_dis_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_PBIT_DIS,      ca_omci_hal_ds_flow_pbit_dis_untag,
     ca_omci_hal_ds_flow_pbit_dis_sgltag,       ca_omci_hal_ds_flow_pbit_dis_dbltag},
    {EXT_VLAN_TAG_OP_DS_MODE_DISCARD_ALL,          ca_omci_hal_ds_flow_dis_untag,
     ca_omci_hal_ds_flow_dis_sgltag,            ca_omci_hal_ds_flow_dis_dbltag},
};


//now all use classifier rule, so just reset all classifier is enough
static ca_omci_hal_ds_flow_cb_t sg_halDsFlowDelCb[] =
{
    {EXT_VLAN_TAG_OP_DS_MODE_INVERSE,              ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FW_UNMODIFY,          ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PBIT_PASS, ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PASS,      ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_PBIT_PASS,     ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_PBIT_DIS,  ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_VID_DIS,       ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_FILTER_PBIT_DIS,      ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
    {EXT_VLAN_TAG_OP_DS_MODE_DISCARD_ALL,          ca_omci_hal_ds_flow_inverse_untag_del,
     ca_omci_hal_ds_flow_inverse_sgltag_del,    ca_omci_hal_ds_flow_inverse_dbltag_del},
};


ca_omci_st_e ca_omci_hal_def_ds_flow_add(ca_omci_stream_cfg_t *pData)
{
    ca_omci_uint8_t i   = 0;
    ca_omci_uint8_t idx = 0;

    //set downstream queue
    if (CA_OMCI_OK != ca_omci_hal_ds_queue_set(pData, CA_OMCI_TRUE))
    {
        OMCIHALERR("Set ds queue failed");
    }

    for (i = 0; i < sizeof(sg_halDsFlowAddCb)/sizeof(ca_omci_hal_ds_flow_cb_t); i++)
    {
        if (sg_halDsFlowAddCb[i].dsMode == pData->dsMode)
        {
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);
            sg_halDsFlowAddCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterDef,    OMCI_HAL_ETH_TYPE_DEF);

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                sg_halDsFlowAddCb[i].sglTagFunc((void *)pData, (void *)&pData->sglTagFilter[idx]);
                sg_halDsFlowAddCb[i].dblTagFunc((void *)pData, (void *)&pData->dblTagFilter[idx]);
            }

            //Ignore single tag default drop and double tag default drop rule
            if (CA_OMCI_SGL_TAG_FWD_DROP != pData->sglTagFilterDef.fwdType)
            {
                sg_halDsFlowAddCb[i].sglTagFunc((void *)pData, (void *)&pData->sglTagFilterDef);
            }

            if (CA_OMCI_DBL_TAG_FWD_DROP != pData->dblTagFilterDef.fwdType)
            {
                sg_halDsFlowAddCb[i].dblTagFunc((void *)pData, (void *)&pData->dblTagFilterDef);
            }

            if (EXT_VLAN_TAG_OP_DS_MODE_INVERSE == pData->dsMode)
            {
                ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
            }

            return CA_OMCI_OK;
        }
    }

    if (i >= sizeof(sg_halDsFlowAddCb)/sizeof(ca_omci_hal_ds_flow_cb_t))
    {
        OMCIHALERR("Unsupport ds mode[%d] for port[0x%x] type[%d] gem[%d]",
                    pData->dsMode, pData->uniPortId, pData->portType, pData->gemId);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_def_ds_flow_del(ca_omci_stream_cfg_t *pData)
{
    ca_omci_uint8_t i   = 0;
    ca_omci_uint8_t idx = 0;

    //set downstream queue
    if (CA_OMCI_OK != ca_omci_hal_ds_queue_set(pData, CA_OMCI_FALSE))
    {
        OMCIHALERR("Reset ds queue failed");
    }

    for (i = 0; i < sizeof(sg_halDsFlowDelCb)/sizeof(ca_omci_hal_ds_flow_cb_t); i++)
    {
        if (sg_halDsFlowDelCb[i].dsMode == pData->dsMode)
        {
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);
            sg_halDsFlowDelCb[i].untagFunc((void *)pData, (void *)&pData->untagFilterDef,    OMCI_HAL_ETH_TYPE_DEF);

            for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
            {
                sg_halDsFlowDelCb[i].sglTagFunc((void *)pData, (void *)&pData->sglTagFilter[idx]);
                sg_halDsFlowDelCb[i].dblTagFunc((void *)pData, (void *)&pData->dblTagFilter[idx]);
            }

            //Ignore single tag default drop and double tag default drop rule
            if (CA_OMCI_SGL_TAG_FWD_DROP != pData->sglTagFilterDef.fwdType)
            {
                sg_halDsFlowDelCb[i].sglTagFunc((void *)pData, (void *)&pData->sglTagFilterDef);
            }

            if (CA_OMCI_DBL_TAG_FWD_DROP != pData->dblTagFilterDef.fwdType)
            {
                sg_halDsFlowDelCb[i].dblTagFunc((void *)pData, (void *)&pData->dblTagFilterDef);
            }

            //release the default rule
            ca_omci_hal_ds_flow_def_reset(pData);
            return CA_OMCI_OK;
        }
    }

    if (i >= sizeof(sg_halDsFlowDelCb)/sizeof(ca_omci_hal_ds_flow_cb_t))
    {
        OMCIHALERR("Unsupport ds mode[%d] for port[0x%x] type[%d] gem[%d]",
                    pData->dsMode, pData->uniPortId, pData->portType, pData->gemId);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_port_outer_tpid_set(
    ca_port_id_t   portId,
    ca_uint32_t    idx,
    ca_omci_bool_t admin)
{
    return CA_OMCI_OK;
}



ca_omci_st_e ca_omci_hal_port_inner_tpid_set(
    ca_port_id_t   portId,
    ca_omci_bool_t admin)
{
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_tpid_set(ca_omci_stream_cfg_t *pUsFlow)
{
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_tpid_reset(ca_omci_stream_cfg_t *pUsFlow)
{
    return CA_OMCI_OK;
}



ca_omci_st_e ca_omci_hal_us_flow_add(void *in)
{
    ca_omci_stream_cfg_t *pUsFlow = (ca_omci_stream_cfg_t *)in;
    ca_omci_uint8_t       idx     = 0;

    HAL_CHECK_TCL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(pUsFlow);
    //ca_omci_hal_dbg_print_flow(pUsFlow);

    if ((OMCI_CFG_STREAM_PORT_ETH != pUsFlow->portType)
     && (OMCI_CFG_STREAM_PORT_VEIP != pUsFlow->portType)
     && (OMCI_CFG_STREAM_PORT_IPHOST != pUsFlow->portType))
    {
        return CA_OMCI_OK;
    }

    //set TPID first
    ca_omci_hal_tpid_set(pUsFlow);

    //if any rules add, make default drop
    ca_omci_hal_def_drop_set(pUsFlow);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);
    ca_omci_hal_us_flow_untag(pUsFlow, &pUsFlow->untagFilterDef,    OMCI_HAL_ETH_TYPE_DEF);

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        ca_omci_hal_us_flow_sgltag(pUsFlow, &pUsFlow->sglTagFilter[idx]);
        ca_omci_hal_us_flow_dbltag(pUsFlow, &pUsFlow->dblTagFilter[idx]);
    }

    //Ignore single tag default drop and double tag default drop rule
    if (CA_OMCI_SGL_TAG_FWD_DROP != pUsFlow->sglTagFilterDef.fwdType)
    {
        ca_omci_hal_us_flow_sgltag(pUsFlow, &pUsFlow->sglTagFilterDef);
    }

    if (CA_OMCI_DBL_TAG_FWD_DROP != pUsFlow->dblTagFilterDef.fwdType)
    {
        ca_omci_hal_us_flow_dbltag(pUsFlow, &pUsFlow->dblTagFilterDef);
    }

    ca_omci_hal_def_ds_flow_add(pUsFlow);
    return CA_OMCI_OK;
}



ca_omci_st_e ca_omci_hal_us_flow_del(void *in)
{
    ca_omci_stream_cfg_t *pUsFlow = (ca_omci_stream_cfg_t *)in;
    ca_omci_uint8_t       idx     = 0;

    HAL_CHECK_TCL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(pUsFlow);
    //ca_omci_hal_dbg_print_flow(pUsFlow);

    if ((OMCI_CFG_STREAM_PORT_ETH != pUsFlow->portType)
     && (OMCI_CFG_STREAM_PORT_VEIP != pUsFlow->portType))
    {
        return CA_OMCI_OK;
    }

    //when remove the flow, need to reset the tpid too
    ca_omci_hal_tpid_reset(pUsFlow);

    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);
    ca_omci_hal_us_flow_untag_del(pUsFlow, &pUsFlow->untagFilterDef,    OMCI_HAL_ETH_TYPE_DEF);

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        ca_omci_hal_us_flow_sgltag_del(pUsFlow, &pUsFlow->sglTagFilter[idx]);
        ca_omci_hal_us_flow_dbltag_del(pUsFlow, &pUsFlow->dblTagFilter[idx]);
    }
    ca_omci_hal_us_flow_sgltag_del(pUsFlow, &pUsFlow->sglTagFilterDef);
    ca_omci_hal_us_flow_dbltag_del(pUsFlow, &pUsFlow->dblTagFilterDef);

    ca_omci_hal_def_ds_flow_del(pUsFlow);
    ca_omci_hal_flow_classifier_tbl_release(pUsFlow);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_filter_flow_del(void *pData)
{
    ca_omci_filter_stream_cfg_t *pFilter = (ca_omci_filter_stream_cfg_t *)pData;
    ca_omci_stream_cfg_t         flow    = {0};

    HAL_CHECK_TCL;

    if (CA_OMCI_TRUE == pFilter->externVlanFlag)
    {
        OMCIHALWARN("Don't need to delete filter rule");
        return CA_OMCI_OK;
    }

    //reset TPID first
    flow.inputTpid     = pFilter->inputTpid;
    flow.outputTpid    = pFilter->outputTpid;
    flow.uniPortId     = pFilter->uniPortId;
    flow.portType      = pFilter->portType;
    flow.dsQueue       = pFilter->dsQueue;
    flow.dsQueueWeight = pFilter->dsQueueWeight;
    ca_omci_hal_tpid_reset(&flow);

    if (CA_OMCI_OK != ca_omci_hal_ds_queue_set(&flow, CA_OMCI_FALSE))
    {
        OMCIHALERR("Set ds queue failed");
    }

    if (CA_OMCI_OK != ca_omci_hal_filter_flow_classifier_reset(pFilter))
    {
        OMCIHALERR("remove filter flow classifier failed");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


//for ds ivs mode, it only set the downstream only gemport rules
ca_omci_st_e ca_omci_hal_ds_ivs_flow_add(void *in)
{
    ca_omci_stream_cfg_t *pData = (ca_omci_stream_cfg_t *)in;
    ca_omci_uint8_t       idx   = 0;

    HAL_CHECK_TCL;

    OMCI_ENTER;
    OMCI_PARAM_NULL_RETURN(pData);

    //broad cast downstream flow need to set forward to L2FE
    pData->broadcastFlag = CA_OMCI_TRUE;

    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);
    ca_omci_hal_ds_flow_inverse_untag((void *)pData, (void *)&pData->untagFilterDef,    OMCI_HAL_ETH_TYPE_DEF);

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        ca_omci_hal_ds_flow_inverse_sgltag((void *)pData, (void *)&pData->sglTagFilter[idx]);
        ca_omci_hal_ds_flow_inverse_dbltag((void *)pData, (void *)&pData->dblTagFilter[idx]);
    }

    //Ignore single tag default drop and double tag default drop rule
    if (CA_OMCI_SGL_TAG_FWD_DROP != pData->sglTagFilterDef.fwdType)
    {
        ca_omci_hal_ds_flow_inverse_sgltag((void *)pData, (void *)&pData->sglTagFilterDef);
    }

    if (CA_OMCI_DBL_TAG_FWD_DROP != pData->dblTagFilterDef.fwdType)
    {
        ca_omci_hal_ds_flow_inverse_dbltag((void *)pData, (void *)&pData->dblTagFilterDef);
    }

    ca_omci_hal_ds_flow_def_set(pData, CA_OMCI_FALSE);
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_ds_ivs_flow_del(void *in)
{
    ca_omci_stream_cfg_t *pData = (ca_omci_stream_cfg_t *)in;
    ca_omci_uint8_t       idx   = 0;

    HAL_CHECK_TCL;

    ca_omci_hal_ds_flow_inverse_untag_del((void *)pData, (void *)&pData->untagFilterIpoe,   OMCI_HAL_ETH_TYPE_IPOE);
    ca_omci_hal_ds_flow_inverse_untag_del((void *)pData, (void *)&pData->untagFilterIpoev6, OMCI_HAL_ETH_TYPE_IPOEV6);
    ca_omci_hal_ds_flow_inverse_untag_del((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE);
    ca_omci_hal_ds_flow_inverse_untag_del((void *)pData, (void *)&pData->untagFilterPppoe,  OMCI_HAL_ETH_TYPE_PPPOE2);
    ca_omci_hal_ds_flow_inverse_untag_del((void *)pData, (void *)&pData->untagFilterArp,    OMCI_HAL_ETH_TYPE_ARP);

    for (idx = 0; idx < CA_OMCI_VLAN_FILTER_ENTRY_MAX_NUM; idx++)
    {
        ca_omci_hal_ds_flow_inverse_sgltag_del((void *)pData, (void *)&pData->sglTagFilter[idx]);
        ca_omci_hal_ds_flow_inverse_dbltag_del((void *)pData, (void *)&pData->dblTagFilter[idx]);
    }

    ca_omci_hal_ds_flow_def_reset(pData);
    ca_omci_hal_flow_classifier_tbl_release(pData);
    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("Packets relate:CA API hook layer functions")
#if defined(CONFIG_CA_OMCI_FPGA)
static ca_mac_addr_t sg_da = {0x00, 0x13, 0x25, 0x00, 0x00, 0x00};
static ca_mac_addr_t sg_sa = {0x00, 0x13, 0x25, 0x00, 0x00, 0x01};
#endif

/* could do nothing but record a log while transport packet to pon mac */
ca_omci_st_e ca_omci_hal_pkt_tx(void *in, ca_omci_uint32_t len)
{
    ca_omci_uint8_t *pkt = (ca_omci_uint8_t *)in;
    ca_status_t      ret = CA_E_OK;
    ca_pkt_t         halPkt;

    OMCI_ENTER;

    OMCI_PARAM_NULL_RETURN(pkt);
    if (0 == len)
    {
        OMCIHALERR("receive error packet");
        return CA_OMCI_E_PARAM;
    }

    OMCI_MEMSET(&halPkt, 0, sizeof(halPkt));
    halPkt.block_count    = 1;
    halPkt.priority       = 7;
    halPkt.device_id      = OMCI_HAL_DEV_ID;
    halPkt.src_port_id    = OMCI_HAL_CPU_PORT;
    halPkt.dst_port_id    = OMCI_HAL_WAN_PORT;
    halPkt.pkt_type       = CA_PKT_TYPE_OMCI;

#if defined(CONFIG_CA_OMCI_FPGA)
    halPkt.pkt_len        = len + 16;
    halPkt.pkt_data       = (ca_pkt_block_t *)omci_malloc(sizeof(ca_pkt_block_t));
    if (NULL == halPkt.pkt_data)
    {
        OMCIHALERR("halPkt.pkt_data malloc failed");
        return CA_OMCI_E_NO_MEMORY;
    }

    halPkt.pkt_data->len  = len + 16;
    halPkt.pkt_data->data = (ca_uint8_t *)omci_malloc(len + 16);
    if (NULL == halPkt.pkt_data->data)
    {
        OMCIHALERR("halPkt.pkt_data->data malloc failed");
        OMCI_FREE(halPkt.pkt_data);
        return CA_OMCI_E_NO_MEMORY;
    }

    halPkt.flags          = CA_TX_BYPASS_FWD_ENGINE;
    OMCI_MEMCPY(halPkt.pkt_data->data, sg_da, CA_ETH_ADDR_LEN);
    OMCI_MEMCPY((halPkt.pkt_data->data + CA_ETH_ADDR_LEN), sg_sa, CA_ETH_ADDR_LEN);
    halPkt.pkt_data->data[12] = 0xff;
    halPkt.pkt_data->data[13] = 0xf1;
    halPkt.pkt_data->data[14] = len;

    OMCI_MEMCPY((halPkt.pkt_data->data + 16), pkt, len);
#else
    halPkt.pkt_len        = len;
    halPkt.pkt_data       = (ca_pkt_block_t *)omci_malloc(sizeof(ca_pkt_block_t));
    if (NULL == halPkt.pkt_data)
    {
        OMCIHALERR("halPkt.pkt_data malloc failed");
        return CA_OMCI_E_NO_MEMORY;
    }

    halPkt.pkt_data->len  = len;
    halPkt.pkt_data->data = (ca_uint8_t *)omci_malloc(len);
    halPkt.flags          = CA_TX_BYPASS_FWD_ENGINE;
    if (NULL == halPkt.pkt_data->data)
    {
        OMCIHALERR("halPkt.pkt_data->data malloc failed");
        OMCI_FREE(halPkt.pkt_data);
        return CA_OMCI_E_NO_MEMORY;
    }

    OMCI_MEMCPY(halPkt.pkt_data->data, pkt, len);
#endif

    //printk("\r\n=====================================[tx]=====================================");
    //ca_omci_pkt_hex_print(halPkt.pkt_data->data, halPkt.pkt_len);

    ret = ca_tx(OMCI_HAL_DEV_ID, &halPkt);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("send pkt to pon mac error ret = [%d]", ret);
    }

    OMCI_FREE(halPkt.pkt_data->data);
    OMCI_FREE(halPkt.pkt_data);
    OMCI_LEAVE;
    return CA_OMCI_OK;
}

ca_omci_st_e ca_omci_hal_pkt_mic_gen(
    void             *in,
    ca_omci_uint32_t  len,
    ca_omci_uint8_t   dir,
    ca_omci_uint32_t *mic)
{
    ca_omci_uint8_t *pkt                      = (ca_omci_uint8_t *)in;
    ca_omci_uint8_t  calMic[OMCI_DEF_MIC_LEN] = {0};

    if (CA_E_OK != ca_generate_omci_mic(0, dir, pkt, len, OMCI_DEF_MIC_LEN, calMic))
    {
        OMCIHALERR("calculate mic error ");
        return CA_OMCI_E;
    }

    OMCI_MEMCPY(mic, calMic, OMCI_DEF_MIC_LEN);
    *mic = ca_omci_ntohl(*mic);
    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("PM relate functions")
void ca_omci_hal_pm_eth_frm_us_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t         *pIn    = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_eth_frm_pm_us_attr_t *pData  = (ca_omci_eth_frm_pm_us_attr_t *)out;
    ca_eth_port_stats_ex_t        data   = {0};
    ca_port_id_t                  portId = 0;
    ca_status_t                   ret    = CA_E_OK;

    portId = ca_omci_hal_eth_port_get(pIn->port);
    ret    = ca_eth_port_stats_get_ex(OMCI_HAL_DEV_ID, portId, pIn->dataLen, &data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get port[0x%x] stat failed[%d]", portId, ret);
        return;
    }

    pData->dropEvent       = 0;
    pData->octets          = data.rx_good_octets;
    pData->packets         = (data.rx_uc_frames + data.rx_bc_frames + data.rx_mc_frames);
    pData->broadcastPacket = data.rx_bc_frames;
    pData->muticastPacket  = data.rx_mc_frames;
    pData->crcErrPacket    = data.rx_fcs_error_frames;
    pData->undersizePacket = data.rx_undersize_frames;
    pData->oversizePacket  = data.rx_oversize_frames;
    pData->packetOct64     = data.rx_64_frames;
    pData->packetOct127    = data.rx_65_127_frames;
    pData->packetOct255    = data.rx_128_255_frames;
    pData->packetOct511    = data.rx_256_511_frames;
    pData->packetOct1023   = data.rx_512_1023_frames;
    pData->packetOct1518   = data.rx_1024_1518_frames;

    OMCIHALDBG("Get eth frame us pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    dropEvent[%d]",       pData->dropEvent);
    OMCIHALDBG("    octets[%d]",          pData->octets);
    OMCIHALDBG("    packets[%d]",         pData->packets);
    OMCIHALDBG("    broadcastPacket[%d]", pData->broadcastPacket);
    OMCIHALDBG("    muticastPacket[%d]",  pData->muticastPacket);
    OMCIHALDBG("    crcErrPacket[%d]",    pData->crcErrPacket);
    OMCIHALDBG("    undersizePacket[%d]", pData->undersizePacket);
    OMCIHALDBG("    oversizePacket[%d]",  pData->oversizePacket);
    OMCIHALDBG("    packetOct64[%d]",     pData->packetOct64);
    OMCIHALDBG("    packetOct127[%d]",    pData->packetOct127);
    OMCIHALDBG("    packetOct255[%d]",    pData->packetOct255);
    OMCIHALDBG("    packetOct511[%d]",    pData->packetOct511);
    OMCIHALDBG("    packetOct1023[%d]",   pData->packetOct1023);
    OMCIHALDBG("    packetOct1518[%d]",   pData->packetOct1518);
}


void ca_omci_hal_pm_eth_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t      *pIn    = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_eth_pm_his_attr_t *pData  = (ca_omci_eth_pm_his_attr_t *)out;
    ca_eth_port_stats_ex_t     data   = {0};
    ca_port_id_t               portId = 0;
    ca_status_t                ret    = CA_E_OK;

    portId = ca_omci_hal_eth_port_get(pIn->port);
    ret    = ca_eth_port_stats_get_ex(OMCI_HAL_DEV_ID, portId, pIn->dataLen, &data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get port[0x%x] stat failed[%d]", portId, ret);
        return;
    }

    pData->fcsErr              = data.rx_fcs_error_frames;
    pData->excessCollCnt       = data.phy_stats.frames_with_excessive_deferral;
    pData->lateCollCnt         = data.phy_stats.late_collisions;
    pData->frameTooLong        = data.rx_oversize_frames;
    pData->rxBuffOverflow      = data.rx_buffer_overflow;
    pData->txBuffOverflow      = data.tx_buffer_overflow;
    pData->singleCollFrameCnt  = data.phy_stats.single_collision_frames;
    pData->multiCollFrameCnt   = data.phy_stats.multiple_collision_frames;
    //pData->sqeCnt              = data.phy_stats.;
    pData->deferredTxCnt       = data.phy_stats.frames_with_deferredXmissions;
    pData->internalMacTxErrCnt = data.phy_stats.frames_lost_duetoint_mac_xmit_error;
    pData->carrierSenseErrCnt  = data.phy_stats.symbol_error_during_carrier;
    pData->alignErrCnt         = data.phy_stats.alignmentErrors;
    pData->internalMacRxErrCnt = data.phy_stats.frames_lost_duetoint_mac_rcv_error;

    OMCIHALDBG("Get eth pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    fcsErr[%d]",              pData->fcsErr);
    OMCIHALDBG("    excessCollCnt[%d]",       pData->excessCollCnt);
    OMCIHALDBG("    lateCollCnt[%d]",         pData->lateCollCnt);
    OMCIHALDBG("    frameTooLong[%d]",        pData->frameTooLong);
    OMCIHALDBG("    rxBuffOverflow[%d]",      pData->rxBuffOverflow);
    OMCIHALDBG("    txBuffOverflow[%d]",      pData->txBuffOverflow);
    OMCIHALDBG("    singleCollFrameCnt[%d]",  pData->singleCollFrameCnt);
    OMCIHALDBG("    multiCollFrameCnt[%d]",   pData->multiCollFrameCnt);
    OMCIHALDBG("    sqeCnt[%d]",              pData->sqeCnt);
    OMCIHALDBG("    deferredTxCnt[%d]",       pData->deferredTxCnt);
    OMCIHALDBG("    internalMacTxErrCnt[%d]", pData->internalMacTxErrCnt);
    OMCIHALDBG("    carrierSenseErrCnt[%d]",  pData->carrierSenseErrCnt);
    OMCIHALDBG("    alignErrCnt[%d]",         pData->alignErrCnt);
    OMCIHALDBG("    internalMacRxErrCnt[%d]", pData->internalMacRxErrCnt);
}


void ca_omci_hal_pm_eth_frm_ds_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t         *pIn    = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_eth_frm_pm_ds_attr_t *pData  = (ca_omci_eth_frm_pm_ds_attr_t *)out;
    ca_eth_port_stats_ex_t        data   = {0};
    ca_port_id_t                  portId = 0;
    ca_status_t                   ret    = CA_E_OK;

    portId = ca_omci_hal_eth_port_get(pIn->port);
    ret    = ca_eth_port_stats_get_ex(OMCI_HAL_DEV_ID, portId, pIn->dataLen, &data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get port[0x%x] stat failed[%d]", portId, ret);
        return;
    }

    pData->dropEvent      = 0;
    pData->octets         = data.tx_good_octets;
    pData->frames         = (data.tx_uc_frames + data.tx_bc_frames + data.tx_mc_frames);
    pData->broadcastFrame = data.tx_bc_frames;
    pData->muticastFrame  = data.tx_mc_frames;
    pData->crcErrFrame    = 0;
    pData->undersizeFrame = 0;
    pData->oversizeFrame  = 0;
    pData->frameOct64     = data.tx_64_frames;
    pData->frameOct127    = data.tx_65_127_frames;
    pData->frameOct255    = data.tx_128_255_frames;
    pData->frameOct511    = data.tx_256_511_frames;
    pData->frameOct1023   = data.tx_512_1023_frames;
    pData->frameOct1518   = data.tx_1024_1518_frames;

    OMCIHALDBG("Get eth frame ds pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    dropEvent[%ld]",       pData->dropEvent);
    OMCIHALDBG("    octets[%ld]",          pData->octets);
    OMCIHALDBG("    packets[%ld]",         pData->frames);
    OMCIHALDBG("    broadcastPacket[%ld]", pData->broadcastFrame);
    OMCIHALDBG("    muticastPacket[%ld]",  pData->muticastFrame);
    OMCIHALDBG("    crcErrPacket[%ld]",    pData->crcErrFrame);
    OMCIHALDBG("    undersizePacket[%ld]", pData->undersizeFrame);
    OMCIHALDBG("    oversizePacket[%ld]",  pData->oversizeFrame);
    OMCIHALDBG("    packetOct64[%ld]",     pData->frameOct64);
    OMCIHALDBG("    packetOct127[%ld]",    pData->frameOct127);
    OMCIHALDBG("    packetOct255[%ld]",    pData->frameOct255);
    OMCIHALDBG("    packetOct511[%ld]",    pData->frameOct511);
    OMCIHALDBG("    packetOct1023[%ld]",   pData->frameOct1023);
    OMCIHALDBG("    packetOct1518[%ld]",   pData->frameOct1518);
}


void ca_omci_hal_pm_fec_get(void *in, void *out)
{
    ca_omci_onuinfo_obj_t     *onuinfo  = ca_omci_onuinfo_get();
    ca_omci_hal_pm_cfg_t      *pIn      = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_fec_pm_attr_t     *pData    = (ca_omci_fec_pm_attr_t *)out;
    ca_gpon_port_ldpc_stats_t  stats    = {0};
    ca_status_t                ret      = CA_E_OK;
    ca_gpon_port_fec_stats_t   fecStats = {0};

    if ((OMCI_PON_MODE_25G10G == onuinfo->ponMacMode)
     || (OMCI_PON_MODE_25GPON == onuinfo->ponMacMode)){
        /*This API can't use read keep mode, so just use read clear for any case*/
        ret = ca_gpon_port_ldpc_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, CA_OMCI_TRUE, &stats);
        if (CA_E_OK != ret){
            OMCIHALERR("Get counter failed!");
            return;
        }

        pData->corrByte       = stats.corrected_byte_count;
        pData->corrCodeWord   = stats.corrected_codeword_count;
        pData->uncorrCodeWord = stats.uncorrected_codeword_count;
        pData->totalCodeWord  = stats.total_codeword_count;
    } else {
        ret = ca_gpon_port_fec_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &fecStats);
        if (CA_E_OK != ret){
            OMCIHALERR("Get counter failed!");
            return;
        }

        pData->corrByte       = fecStats.corrected_bytes;
        pData->corrCodeWord   = fecStats.corrected_codewords;
        pData->uncorrCodeWord = fecStats.uncorrectable_codewords;
        pData->totalCodeWord  = fecStats.total_codewords;
    }

    OMCIHALDBG("Get fec pm!");
    OMCIHALDBG("    corrByte[%d]",       pData->corrByte);
    OMCIHALDBG("    corrCodeWord[%d]",   pData->corrCodeWord);
    OMCIHALDBG("    uncorrCodeWord[%d]", pData->uncorrCodeWord);
    OMCIHALDBG("    totalCodeWord[%d]",  pData->totalCodeWord);
    OMCIHALDBG("    fecSeconds[%d]",     pData->fecSeconds);
}


void ca_omci_hal_pm_gem_ntp_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t             *pIn    = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_gpn_ctp_pm_attr_t        *pData  = (ca_omci_gpn_ctp_pm_attr_t *)out;
    ca_status_t                       ret    = CA_E_OK;
    ca_uint16_t                       idx    = 0;
    ca_uint16_t                       gemNum = 0;
    ca_gpon_port_gem_port_direction_t dir    = CA_GPON_PORT_GEM_PORT_DIRECTION_BI;
    ca_gpon_port_gem_stats_ex_t       gemSt  = {0};
    ca_uint16_t                       gemIdx[OMCI_HAL_MAX_PRI_PER_FLOW];

    OMCI_MEMSET(gemIdx, 0, sizeof(ca_uint16_t) * OMCI_HAL_MAX_PRI_PER_FLOW);
    gemNum = ca_omci_hal_gem_get_idx_from_id(pIn->port, gemIdx);
    if (0 == gemNum)
    {
        OMCIHALERR("can't set stream for a uncfg gem");
        return;
    }

    //Usually there is only one gemIdx for a gemId, if there are more than one gemIdx, we need to add more than 1 rule!
    for (idx = 0; idx < gemNum; idx++)
    {
        if (0 != gemIdx[idx])
        {
            break;
        }
    }

    ret = ca_gpon_port_gem_counters_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, gemIdx[idx], dir, &gemSt);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get Gem port[%d] stat failed[%d]", pIn->port, ret);
        return;
    }

    pData->txGemFrame    = gemSt.tx_gem_frames;
    pData->rxGemFrame    = gemSt.rx_gem_frames;
    pData->rxPayloadByte = gemSt.rx_gem_payload;
    pData->txPayloadByte = gemSt.tx_gem_payload;
    pData->encryptKeyErr = 0;

    OMCIHALDBG("Get Gem %d NTP pm! read clear flag %d", pIn->port, pIn->dataLen);
    OMCIHALDBG("    txGemFrame[%d]",      pData->txGemFrame);
    OMCIHALDBG("    rxGemFrame[%d]",      pData->rxGemFrame);
    OMCIHALDBG("    rxPayloadByte[%lld]", pData->rxPayloadByte);
    OMCIHALDBG("    txPayloadByte[%lld]", pData->txPayloadByte);
    OMCIHALDBG("    encryptKeyErr[%d]",   pData->encryptKeyErr);
}



void ca_omci_hal_pm_xgpon_tc_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t       *pIn   = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_xgpon_tc_pm_attr_t *pData = (ca_omci_xgpon_tc_pm_attr_t *)out;
    ca_phy_los_pm_t             data  = {0};
    ca_gpon_port_stats_t        stats = {0};
    ca_status_t                 ret   = CA_E_OK;

    (void)in;
    ret = ca_gpon_port_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &stats);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get gpon port stat failed[%d]", ret);
        return;
    }

    ret = ca_ngp2_port_pm_counters_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, CA_COUNTERS_PHY_LOS_PM_T, sizeof(ca_phy_los_pm_t), (ca_uint8_t *)&data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get gpon port los failed[%d]", ret);
        return;
    }

    pData->psbdHecErrCnt   = stats.downstream_psbd_hec_err_count;
    pData->xgtcHecErrCnt   = stats.downstream_xgtc_hec_err_count;
    pData->unknownProfCnt  = stats.downstream_grant_unknown_pf_count;
    pData->txXgemFrame     = stats.upstream_tx_gem_block_count;
    pData->fragXgemFrame   = stats.upstream_tx_gem_fragment_count;
    pData->xgemHecLostWord = stats.downstream_rx_gem_lost_fragment_count;
    pData->xgemKeyErr      = stats.downstream_rx_gem_key_err_count;
    pData->xgemHecErr      = stats.downstream_rx_gem_hec_err_count;
    //pData->txXgemBytes     = ;
    //pData->rxXgemBytes     = ;
    pData->lodsCnt         = data.total_lods_event_count;
    pData->lodsRestoreCnt  = data.lods_restored_in_discretionary_twdm_ch + data.lods_restored_in_operating_twdm_ch + data.lods_restored_in_protection_twdm_ch;
    pData->onuReactByLods  = data.lods_reactivations;

    OMCIHALDBG("Get XGPON TC pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    psbdHecErrCnt[%d]",   pData->psbdHecErrCnt);
    OMCIHALDBG("    xgtcHecErrCnt[%d]",   pData->xgtcHecErrCnt);
    OMCIHALDBG("    unknownProfCnt[%d]",  pData->unknownProfCnt);
    OMCIHALDBG("    txXgemFrame[%d]",     pData->txXgemFrame);
    OMCIHALDBG("    fragXgemFrame[%d]",   pData->fragXgemFrame);
    OMCIHALDBG("    xgemHecLostWord[%d]", pData->xgemHecLostWord);
    OMCIHALDBG("    xgemKeyErr[%d]",      pData->xgemKeyErr);
    OMCIHALDBG("    xgemHecErr[%d]",      pData->xgemHecErr);
    OMCIHALDBG("    txXgemBytes[%lld]",   pData->txXgemBytes);
    OMCIHALDBG("    rxXgemBytes[%lld]",   pData->rxXgemBytes);
    OMCIHALDBG("    lodsCnt[%d]",         pData->lodsCnt);
    OMCIHALDBG("    lodsRestoreCnt[%d]",  pData->lodsRestoreCnt);
    OMCIHALDBG("    onuReactByLods[%d]",  pData->onuReactByLods);
}


void ca_omci_hal_pm_xgpon_ds_mng_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t               *pIn   = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_xgpon_ds_mng_pm_attr_t     *pData = (ca_omci_xgpon_ds_mng_pm_attr_t *)out;
    ca_gpon_port_ploam_omci_counters_t  data  = {0};
    ca_status_t                         ret   = CA_E_OK;

    ret = ca_gpon_port_ploam_omci_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get xgpon ds mng pm failed[%d]", ret);
        return;
    }

    pData->ploamMicErr        = data.ploam_mic_err;
    pData->dsPloamMsg         = data.ploam_ds_rx;
    pData->rxProfMsg          = data.ploam_burst_profile;
    pData->rxRangingTMsg      = data.ploam_ranging_time;
    pData->rxDeactiveOnuIdMsg = data.ploam_deactive_onu;
    pData->rxDisableSnMsg     = data.ploam_disable_sn;
    pData->rxReqRegMsg        = data.ploam_request_registration;
    pData->rxAssignAllocIdMsg = data.ploam_assign_allocid;
    pData->rxKeyCtlMsg        = data.ploam_key_ctrl;
    pData->rxSleepAllowMsg    = data.ploam_sleep_allow;
    pData->rxBaseOmciMsgCnt   = data.omci_baseline_rx;
    pData->rxExtOmciMsgCnt    = data.omci_extended_rx;
    pData->rxAssignOnuIdMsg   = data.ploam_assign_onu_id;
    pData->omciMicErr         = 0; //omci should handle itself

    OMCIHALDBG("Get xgpon ds mng pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    ploamMicErr[%d]",        pData->ploamMicErr);
    OMCIHALDBG("    dsPloamMsg[%d]",         pData->dsPloamMsg);
    OMCIHALDBG("    rxProfMsg[%d]",          pData->rxProfMsg);
    OMCIHALDBG("    rxRangingTMsg[%d]",      pData->rxRangingTMsg);
    OMCIHALDBG("    rxDeactiveOnuIdMsg[%d]", pData->rxDeactiveOnuIdMsg);
    OMCIHALDBG("    rxDisableSnMsg[%d]",     pData->rxDisableSnMsg);
    OMCIHALDBG("    rxReqRegMsg[%d]",        pData->rxReqRegMsg);
    OMCIHALDBG("    rxAssignAllocIdMsg[%d]", pData->rxAssignAllocIdMsg);
    OMCIHALDBG("    rxKeyCtlMsg[%d]",        pData->rxKeyCtlMsg);
    OMCIHALDBG("    rxSleepAllowMsg[%d]",    pData->rxSleepAllowMsg);
    OMCIHALDBG("    rxBaseOmciMsgCnt[%d]",   pData->rxBaseOmciMsgCnt);
    OMCIHALDBG("    rxExtOmciMsgCnt[%d]",    pData->rxExtOmciMsgCnt);
    OMCIHALDBG("    rxAssignOnuIdMsg[%d]",   pData->rxAssignOnuIdMsg);
    OMCIHALDBG("    omciMicErr[%d]",         pData->omciMicErr);
}


void ca_omci_hal_pm_xgpon_us_mng_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t               *pIn   = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_xgpon_us_mng_pm_attr_t     *pData = (ca_omci_xgpon_us_mng_pm_attr_t *)out;
    ca_gpon_port_ploam_omci_counters_t  data  = {0};
    ca_status_t                         ret   = CA_E_OK;

    ret = ca_gpon_port_ploam_omci_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get xgpon ds mng pm failed[%d]", ret);
        return;
    }

    pData->usPloamMsg        = data.ploam_us_tx;
    pData->txSnMsg           = data.ploam_sn_onu;
    pData->txRegistrationMsg = data.ploam_registration;
    pData->txKeyReportMsg    = data.ploam_key_report;
    pData->txAckMsg          = data.ploam_ack;
    pData->txSleepReqMsg     = data.ploam_sleep_request;

    OMCIHALDBG("Get xgpon ds mng pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    usPloamMsg[%d]",        pData->usPloamMsg);
    OMCIHALDBG("    txSnMsg[%d]",           pData->txSnMsg);
    OMCIHALDBG("    txRegistrationMsg[%d]", pData->txRegistrationMsg);
    OMCIHALDBG("    txKeyReportMsg[%d]",    pData->txKeyReportMsg);
    OMCIHALDBG("    txAckMsg[%d]",          pData->txAckMsg);
    OMCIHALDBG("    txSleepReqMsg[%d]",     pData->txSleepReqMsg);
}



void ca_omci_hal_pm_fec_enhanced_get(void *in, void *out)
{
    ca_omci_onuinfo_obj_t         *onuinfo  = ca_omci_onuinfo_get();
    ca_omci_hal_pm_cfg_t          *pIn      = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_enhance_fec_pm_attr_t *pData    = (ca_omci_enhance_fec_pm_attr_t *)out;
    ca_gpon_port_ldpc_stats_t      stats    = {0};
    ca_status_t                    ret      = CA_E_OK;
    ca_gpon_port_fec_stats_t       fecStats = {0};

    if ((OMCI_PON_MODE_25G10G == onuinfo->ponMacMode)
     || (OMCI_PON_MODE_25GPON == onuinfo->ponMacMode)){
        /*This API can't use read keep mode, so just use read clear for any case*/
        ret = ca_gpon_port_ldpc_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, CA_OMCI_TRUE, &stats);
        if (CA_E_OK != ret){
            OMCIHALERR("Get counter failed!");
            return;
        }

        pData->corrByte       = stats.corrected_byte_count;
        pData->corrCodeWord   = stats.corrected_codeword_count;
        pData->uncorrCodeWord = stats.uncorrected_codeword_count;
        pData->totalCodeWord  = stats.total_codeword_count;
    } else {
        ret = ca_gpon_port_fec_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &fecStats);
        if (CA_E_OK != ret){
            OMCIHALERR("Get counter failed!");
            return;
        }

        pData->corrByte       = fecStats.corrected_bytes;
        pData->corrCodeWord   = fecStats.corrected_codewords;
        pData->uncorrCodeWord = fecStats.uncorrectable_codewords;
        pData->totalCodeWord  = fecStats.total_codewords;
    }

    OMCIHALDBG("Get fec pm!");
    OMCIHALDBG("    corrByte[%lld]",       pData->corrByte);
    OMCIHALDBG("    corrCodeWord[%lld]",   pData->corrCodeWord);
    OMCIHALDBG("    uncorrCodeWord[%lld]", pData->uncorrCodeWord);
    OMCIHALDBG("    totalCodeWord[%lld]",  pData->totalCodeWord);
}


void ca_omci_hal_pm_xgpon_tc_enhanced_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t         *pIn   = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_enhance_tc_pm_attr_t *pData = (ca_omci_enhance_tc_pm_attr_t *)out;
    ca_phy_los_pm_t               data  = {0};
    ca_gpon_port_stats_t          stats = {0};
    ca_status_t                   ret   = CA_E_OK;

    (void)in;
    ret = ca_gpon_port_stats_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, pIn->dataLen, &stats);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get gpon port stat failed[%d]", ret);
        return;
    }

    ret = ca_ngp2_port_pm_counters_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, CA_COUNTERS_PHY_LOS_PM_T, sizeof(ca_phy_los_pm_t), (ca_uint8_t *)&data);
    if (CA_E_OK != ret)
    {
        OMCIHALERR("Get gpon port los failed[%d]", ret);
        return;
    }

    pData->psbdHecErrCnt   = stats.downstream_psbd_hec_err_count;
    pData->xgtcHecErrCnt   = stats.downstream_xgtc_hec_err_count;
    pData->unknownProfCnt  = stats.downstream_grant_unknown_pf_count;
    pData->txXgemFrame     = stats.upstream_tx_gem_block_count;
    pData->fragXgemFrame   = stats.upstream_tx_gem_fragment_count;
    pData->xgemHecLostWord = stats.downstream_rx_gem_lost_fragment_count;
    pData->xgemKeyErr      = stats.downstream_rx_gem_key_err_count;
    pData->xgemHecErr      = stats.downstream_rx_gem_hec_err_count;
    //pData->txXgemBytes     = ;
    //pData->rxXgemBytes     = ;
    pData->lodsCnt         = data.total_lods_event_count;
    pData->lodsRestoreCnt  = data.lods_restored_in_discretionary_twdm_ch + data.lods_restored_in_operating_twdm_ch + data.lods_restored_in_protection_twdm_ch;
    pData->onuReactByLods  = data.lods_reactivations;

    OMCIHALDBG("Get XGPON TC pm! read clear flag %d", pIn->dataLen);
    OMCIHALDBG("    psbdHecErrCnt[%d]",     pData->psbdHecErrCnt);
    OMCIHALDBG("    xgtcHecErrCnt[%d]",     pData->xgtcHecErrCnt);
    OMCIHALDBG("    unknownProfCnt[%d]",    pData->unknownProfCnt);
    OMCIHALDBG("    txXgemFrame[%lld]",     pData->txXgemFrame);
    OMCIHALDBG("    fragXgemFrame[%lld]",   pData->fragXgemFrame);
    OMCIHALDBG("    xgemHecLostWord[%lld]", pData->xgemHecLostWord);
    OMCIHALDBG("    xgemKeyErr[%lld]",      pData->xgemKeyErr);
    OMCIHALDBG("    xgemHecErr[%lld]",      pData->xgemHecErr);
    OMCIHALDBG("    txXgemBytes[%lld]",     pData->txXgemBytes);
    OMCIHALDBG("    rxXgemBytes[%lld]",     pData->rxXgemBytes);
    OMCIHALDBG("    lodsCnt[%d]",           pData->lodsCnt);
    OMCIHALDBG("    lodsRestoreCnt[%d]",    pData->lodsRestoreCnt);
    OMCIHALDBG("    onuReactByLods[%d]",    pData->onuReactByLods);
}


static ca_omci_hal_pm_cfg_cb_t sg_halSfuPmCb[] =
{
    {OMCI_PM_ETH_FRM_US,   ca_omci_hal_pm_eth_frm_us_get},
    {OMCI_PM_ETH,          ca_omci_hal_pm_eth_get},
    {OMCI_PM_ETH_FRM_DS,   ca_omci_hal_pm_eth_frm_ds_get},
    {OMCI_PM_FEC,          ca_omci_hal_pm_fec_get},
    {OMCI_PM_GEM_NTP,      ca_omci_hal_pm_gem_ntp_get},
    {OMCI_PM_XGPON_TC,     ca_omci_hal_pm_xgpon_tc_get},
    {OMCI_PM_XGPON_DS_MNG, ca_omci_hal_pm_xgpon_ds_mng_get},
    {OMCI_PM_XGPON_US_MNG, ca_omci_hal_pm_xgpon_us_mng_get},
    {OMCI_PM_FEC_ENH,      ca_omci_hal_pm_fec_enhanced_get},
    {OMCI_PM_XGPON_TC_ENH, ca_omci_hal_pm_xgpon_tc_enhanced_get},
};


ca_omci_st_e ca_omci_hal_pm_get(void *in, void *out)
{
    ca_omci_hal_pm_cfg_t *cfg = (ca_omci_hal_pm_cfg_t *)in;
    ca_omci_uint8_t       i   = 0;

    HAL_CHECK_TCL;

    for (i = 0; i < sizeof(sg_halSfuPmCb)/sizeof(ca_omci_hal_pm_cfg_cb_t); i++)
    {
        if (sg_halSfuPmCb[i].pmType == cfg->type)
        {
            sg_halSfuPmCb[i].func(in, out);
            break;
        }
    }

    if (i >= sizeof(sg_halSfuPmCb)/sizeof(ca_omci_hal_pm_cfg_cb_t))
    {
        OMCIHALERR("no function for pm type(%d)", cfg->type);
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_pm_clear(void *in)
{
    //don't clear the PM now, TODO
    (void)in;
    return CA_OMCI_OK;
}
#endif


#if DESCRIPTION("All event handlers. Include pkt and some events")
ca_int32_t ca_omci_hal_ploam_handler(
    CA_IN   ca_device_id_t  device_id,
    CA_OUT  ca_pkt_t    *pkt,
    CA_IN   void*        cookie)
{
    ca_omci_inner_msg_t        msg     = {0};
    ca_uint32_t                onuId   = 0;
    ca_uint16_t                rxOnuId = 0;
    ca_omci_onuinfo_obj_t     *onuinfo = ca_omci_onuinfo_get();
    ca_uint16_t                idx     = 0;
    ca_gpon_port_onu_states_t  state   = CA_NGP2_ONU_ACT_STATE_O1_1;

    (void)device_id;
    (void)cookie;
    OMCI_PARAM_NULL_RETURN(pkt);
    OMCI_PARAM_NULL_RETURN(pkt->pkt_data);
    OMCI_PARAM_NULL_RETURN(pkt->pkt_data->data);

    if ((pkt->pkt_type != CA_PKT_TYPE_PLOAM)
     || (PLOAMD_REBOOT_ONU_LEN != pkt->pkt_data->len))
    {
        OMCIHALERR("receive error packet type%d len %d", pkt->pkt_type, pkt->pkt_data->len);
        return CA_RX_NOT_HANDLED;
    }

    /* No need to handle other ploamd except reboot ONU */
    if (0x1d != pkt->pkt_data->data[2]) {
        return CA_RX_NOT_HANDLED;
    }

    /* Only handle broadcast ONUID and matched ONUID */
    OMCI_MEMCPY(&rxOnuId, pkt->pkt_data->data, sizeof(rxOnuId));
    rxOnuId = ca_omci_ntohs(rxOnuId);

    (void) ca_gpon_port_onu_id_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &onuId);
    if ((PLOAMD_BROADCAST_ONU_ID != rxOnuId) && (onuId != rxOnuId)) {
        return CA_RX_NOT_HANDLED;
    }

    /*Check ONU sn and vendor ID for broadcast ploamd*/
    if (PLOAMD_BROADCAST_ONU_ID == rxOnuId) {
        for (idx = 0; idx < OMCI_SN_LEN; idx++) {
            if (0 != pkt->pkt_data->data[4 +  idx]) {
                break;
            }
        }

        if (OMCI_SN_LEN != idx) {
            for (idx = 0; idx < OMCI_SN_LEN; idx++) {
                if (pkt->pkt_data->data[4 +  idx] != onuinfo->sn[idx]) {
                    return CA_RX_NOT_HANDLED;
                }
            }
        }
    }

    /*Check ONU state, only reboot when ONU in state O1, O2-3*/
    if (1 == pkt->pkt_data->data[14]){
        (void) ca_gpon_port_onu_state_get(OMCI_HAL_DEV_ID, OMCI_HAL_WAN_PORT, &state);
        if ((CA_NGP2_ONU_ACT_STATE_O1_1 != state)
         && (CA_NGP2_ONU_ACT_STATE_O1_2 != state)
         && (CA_NGP2_ONU_ACT_STATE_O2_3 != state)) {
             /*Save a flag and only reset that value */
             sg_halRebootCfg[0]  = pkt->pkt_data->data[12];/*Reboot depth*/
             sg_halRebootCfg[1]  = pkt->pkt_data->data[13];/*Reboot image*/
             sg_halRebootCfg[2]  = pkt->pkt_data->data[15];/*Flags*/
             sg_halRebootCfgFlag = CA_OMCI_TRUE;
             return CA_RX_NOT_HANDLED;
        }
    }

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    OMCI_MEMCPY(msg.data, pkt->pkt_data->data, pkt->pkt_data->len);
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_PLOAMD_REBOOT;
    msg.hdr.dataLen = pkt->pkt_data->len;
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIHALERR("send to inner mq error");
        return CA_RX_NOT_HANDLED;
    }

    return CA_RX_HANDLED;
}


/* while receive packet from PON mac send to omci MQ */
ca_int32_t ca_omci_hal_pkt_handler(
    CA_IN   ca_device_id_t  device_id,
    CA_OUT  ca_pkt_t    *pkt,
    CA_IN   void*        cookie)
{
    ca_omci_inner_msg_t   msg;

    (void)device_id;
    (void)cookie;
    OMCI_PARAM_NULL_RETURN(pkt);
    OMCI_PARAM_NULL_RETURN(pkt->pkt_data);
    OMCI_PARAM_NULL_RETURN(pkt->pkt_data->data);

    if ((pkt->pkt_type != CA_PKT_TYPE_OMCI)
     || (0 == pkt->pkt_data->len)
     || (pkt->pkt_data->len > OMCI_MQ_MSG_MAX_LEN))
    {
        OMCIHALERR("receive error packet type%d len %d", pkt->pkt_type, pkt->pkt_data->len);
        return CA_RX_NOT_HANDLED;
    }

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    OMCI_MEMCPY(msg.data, pkt->pkt_data->data, pkt->pkt_data->len);
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_PKT;
    msg.hdr.dataLen = pkt->pkt_data->len;

    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIHALERR("send to inner mq error");
        return CA_RX_NOT_HANDLED;
    }

    return CA_RX_HANDLED;
}


ca_omci_st_e ca_omci_hal_eth_st_report(void *pPort, void *pState)
{
    ca_uint32_t            *portId   = (ca_uint32_t *)pPort;
    ca_uint32_t            *link     = (ca_uint32_t *)pState;
    ca_omci_uint16_t        instance = 0;
    ca_omci_alarm_report_t *pData    = NULL;
    ca_omci_inner_msg_t     msg;

    HAL_CHECK_TCL;

    OMCI_PARAM_NULL_RETURN(pPort);
    OMCI_PARAM_NULL_RETURN(pState);
    instance = ca_omci_hal_eth_instance_get(*portId);
    if (0 == instance){
        OMCIHALERR("OMCI have no such UNI instance");
        return CA_OMCI_OK;
    }

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    pData             = (ca_omci_alarm_report_t *)&msg.data;
    pData->alarmId    = CA_OMCI_ALARM_PPTP_ETH_LAN_LOS;
    pData->instanceId = instance;
    pData->state      = *link;
    msg.hdr.msgType   = OMCI_INNER_MSG_TYPE_ALARM;
    msg.hdr.dataLen   = sizeof(ca_omci_alarm_report_t);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIHALERR("send to inner mq error");
        return CA_OMCI_E;
    }

    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_hal_pon_link_st(void *pPort, void *pState)
{
    ca_omci_bool_t      *linkSt   = (ca_omci_bool_t *)pState;
    ca_omci_inner_msg_t  msg;

    OMCI_PARAM_NULL_RETURN(pPort);
    OMCI_PARAM_NULL_RETURN(pState);

    OMCIHALDBG("Pon Link state change to %s", (*linkSt == CA_OMCI_TRUE) ? "up" : "down");
    OMCI_MEMSET(&msg, 0, sizeof(msg));
    OMCI_MEMCPY(msg.data, linkSt, sizeof(ca_omci_bool_t));
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_OMCC_STATE;
    msg.hdr.dataLen = sizeof(ca_omci_bool_t);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIHALERR("send to inner mq error");
        return CA_OMCI_E;
    }
    return CA_OMCI_OK;
}



ca_uint32_t ca_omci_hal_eth_st_change(
    ca_device_id_t  device_id,
    ca_uint32_t     event_id,
    ca_void_t      *event,
    ca_uint32_t     len,
    ca_void_t      *cookie
)
{
    ca_event_eth_port_link_t *pData  = (ca_event_eth_port_link_t *)event;
    ca_uint32_t               status = CA_OMCI_FALSE;

    (void)device_id;
    (void)event_id;
    (void)len;
    (void)cookie;

    OMCIHALDBG("port %x, link status %s", pData->port_id, (CA_PORT_LINK_UP == pData->status) ? "up" : "down");

    status = (CA_PORT_LINK_UP == pData->status) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    ca_omci_hal_eth_st_report((void *)&pData->port_id, (void *)&status);

    return CA_OMCI_OK;
}


ca_uint32_t ca_omci_hal_omcc_st_change(
    ca_device_id_t  device_id,
    ca_uint32_t     event_id,
    ca_void_t      *event,
    ca_uint32_t     len,
    ca_void_t      *cookie
)
{
    ca_event_omcc_link_t *pData  = (ca_event_omcc_link_t *)event;
    ca_omci_bool_t        status = CA_OMCI_FALSE;

    (void)device_id;
    (void)event_id;
    (void)len;
    (void)cookie;

    printk("------------- OMCC link %s \r\n", (CA_OMCC_LINK_UP == pData->status) ? "up" : "down");
    status = (CA_OMCC_LINK_UP == pData->status) ? CA_OMCI_TRUE : CA_OMCI_FALSE;
    ca_omci_hal_pon_link_st((void *)&pData->port_id, (void *)&status);

    return CA_OMCI_OK;
}


ca_uint32_t ca_omci_hal_gpon_st_change(
    ca_device_id_t  device_id,
    ca_uint32_t     event_id,
    ca_void_t      *event,
    ca_uint32_t     len,
    ca_void_t      *cookie)
{
    ca_event_gpon_state_change_t *pData = (ca_event_gpon_state_change_t *)event;
    ca_omci_inner_msg_t           msg   = {0};

    (void)device_id;
    (void)event_id;
    (void)len;
    (void)cookie;

    if (CA_OMCI_TRUE == sg_halRebootCfgFlag) {
        if ((CA_NGP2_ONU_ACT_STATE_O1_1 == pData->current_state)
         || (CA_NGP2_ONU_ACT_STATE_O1_2 == pData->current_state)
         || (CA_NGP2_ONU_ACT_STATE_O2_3 == pData->current_state)) {

             OMCI_MEMSET(&msg, 0, sizeof(msg));
             msg.hdr.msgType = OMCI_INNER_MSG_TYPE_PLOAMD_REBOOT;
             msg.hdr.dataLen = PLOAMD_REBOOT_ONU_LEN;
             msg.data[12]    = sg_halRebootCfg[0];
             msg.data[13]    = sg_halRebootCfg[1];
             msg.data[15]    = sg_halRebootCfg[2];
             if (CA_OMCI_OK != ca_omci_mq_send(&msg))
             {
                 OMCIHALERR("send to inner mq error");
                 return 0;
             }

             sg_halRebootCfgFlag = CA_OMCI_FALSE;
             return 0;
        }
    }

    return 0;
}
#endif


#if DESCRIPTION("Init functions")
void ca_omci_hal_data_init(void)
{
    OMCI_MEMSET(sg_queueUsedNum, 0, OMCI_HAL_MAX_QUEUE_NUM);
    OMCI_MEMSET(sg_halGemMap, 0, (sizeof(ca_omci_hal_gem_flow_map_t) * OMCI_HAL_MAX_GEM_NUM));
    OMCI_MEMSET(sg_halRuleMap, 0, (sizeof(ca_omci_hal_rule_map_t) * OMCI_HAL_MAX_FLOW_NUM));
    OMCI_MEMSET(sg_halFilterMap, 0, (sizeof(ca_omci_hal_rule_map_t) * OMCI_HAL_MAX_FLOW_NUM));
}


void ca_omci_hal_event_register(void)
{
    ca_rx_register(OMCI_HAL_DEV_ID, CA_PKT_TYPE_PLOAM, (ca_rx_cb)ca_omci_hal_ploam_handler, 0, NULL, 0);
    ca_rx_register(OMCI_HAL_DEV_ID, CA_PKT_TYPE_OMCI,  (ca_rx_cb)ca_omci_hal_pkt_handler,   0, NULL, 0);

    ca_event_register(OMCI_HAL_DEV_ID, CA_EVENT_ETH_PORT_LINK,      ca_omci_hal_eth_st_change,  NULL);
    ca_event_register(OMCI_HAL_DEV_ID, CA_EVENT_OMCC_LINK,          ca_omci_hal_omcc_st_change, NULL);
    ca_event_register(OMCI_HAL_DEV_ID, CA_EVENT_GPON_STATE_CHANGED, ca_omci_hal_gpon_st_change, NULL);
}


void ca_omci_hal_tpid_init(void)
{
    ca_omci_onuinfo_obj_t *onuinfo   = ca_omci_onuinfo_get();
    ca_uint32_t            itpidNum  = 0;
    ca_uint32_t            itpids[8] = {0};
    ca_uint32_t            otpidNum  = 0;
    ca_uint32_t            otpids[8] = {0};

    if (OMCI_OLT_TYPE_SIM != onuinfo->oltType)
        return;

    /*By default, 0x8100 bound to both UNI and WAN*/
    /*Remove it and then add TPID 0x8100,0x88a8, that's the API logic*/
    (void) ca_l2_vlan_outer_tpid_delete(OMCI_HAL_DEV_ID, OMCI_HAL_UNI_PORT, 0);
    (void) ca_l2_vlan_inner_tpid_delete(OMCI_HAL_DEV_ID, OMCI_HAL_UNI_PORT, 0);

    itpidNum  = 2;
    otpidNum  = 2;
    itpids[0] = 0x8100;
    itpids[1] = 0x88a8;
    otpids[0] = 0x8100;
    otpids[1] = 0x88a8;
    (void) ca_l2_vlan_tpid_set(OMCI_HAL_DEV_ID, itpids, itpidNum, otpids, otpidNum);
    (void) ca_l2_vlan_inner_tpid_add(OMCI_HAL_DEV_ID, OMCI_HAL_UNI_PORT, 0);
    (void) ca_l2_vlan_outer_tpid_add(OMCI_HAL_DEV_ID, OMCI_HAL_UNI_PORT, 0);
    (void) ca_l2_vlan_outer_tpid_add(OMCI_HAL_DEV_ID, OMCI_HAL_UNI_PORT, 1);
}


ca_omci_st_e ca_omci_hal_init(void)
{
    ca_omci_hal_event_register();
    ca_omci_hal_data_init();
    ca_omci_hal_tpid_init();
    return CA_OMCI_OK;
}
#endif
