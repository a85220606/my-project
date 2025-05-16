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
#ifndef __OMCI_HAL_H__
#define __OMCI_HAL_H__


#define OMCI_HAL_MAX_GEM_NUM       (256) //gem 0-7 can't be used
#define OMCI_HAL_DEV_ID            (0)
#define OMCI_HAL_PKT_HEADER_LEN    (16)
#define OMCI_HAL_PKT_ADDR_LEN      (6)
#define OMCI_HAL_DST_PORT_ID       (7)
#define OMCI_HAL_TCONT_RESET_ALLOC (0xffff)
#define OMCI_HAL_ETH_TYPE_IPOE     (0x0800)
#define OMCI_HAL_ETH_TYPE_PPPOE    (0x8863)
#define OMCI_HAL_ETH_TYPE_PPPOE2   (0x8864)
#define OMCI_HAL_ETH_TYPE_ARP      (0x0806)
#define OMCI_HAL_ETH_TYPE_IPOEV6   (0x86DD)
#define OMCI_HAL_ETH_TYPE_DEF      (0)
#define OMCI_HAL_MAX_RULE_PER_FLOW_US (160)
#define OMCI_HAL_MAX_RULE_PER_FLOW (48)
#define OMCI_HAL_MAX_FLOW_NUM      (256)
#define OMCI_HAL_MAX_PRI_PER_FLOW  (8)
#define OMCI_HAL_MAX_PORT_MEM_NUM  (9) // 1 pon port and 8 eth port
#define OMCI_HAL_MAX_QUEUE_NUM     (256) //queue 0-7 can't be used
#define OMCI_HAL_PON_KEY_MAX_LEN   16


/*********************************
*  For pm type, there will be diff pon type
*  But for OMCI stack, there is no diff
*  Need to add eth relate type
*  No need to define struct match type
*  CA API struct match OMCI define
**********************************/
typedef enum
{
    OMCI_PM_GAL_ETH      = 0,
    OMCI_PM_ETH_FRM_US   = 1, //Ethernet frame performance monitoring history data upstream (G.988: 9.3.30)
    OMCI_PM_ETH          = 2, //Ethernet performance monitoring history data  (G.988: 9.5.2)
    OMCI_PM_ETH_FRM_DS   = 3, //Ethernet frame performance monitoring history data downstream (G.988: 9.3.31)
    OMCI_PM_FEC          = 4, //FEC performance monitoring history data (G.988: 9.2.9)
    OMCI_PM_GEM_NTP      = 5, //GEM port network CTP performance monitoring history data (G.988:9.2.13)
    OMCI_PM_XGPON_TC     = 6, //XG-PON TC performance monitoring history data (G.988:9.2.15)
    OMCI_PM_XGPON_DS_MNG = 7, //XG-PON downstream management performance monitoring history data (G.988:9.2.16)
    OMCI_PM_XGPON_US_MNG = 8, //XG-PON upstream management performance monitoring history data  (G.988:9.2.17)
    OMCI_PM_FEC_ENH      = 9, //Enhanced FEC performance monitoring history data (G.988: 9.2.22)
    OMCI_PM_XGPON_TC_ENH = 10,//Enhanced XG-PON TC performance monitoring history data (G.988:9.2.23)
    OMCI_PM_MAX,
}ca_omci_hal_pm_type_e;


typedef struct
{
    ca_omci_uint32_t      port;
    ca_omci_hal_pm_type_e type;
    ca_omci_uint32_t      dataLen;
}ca_omci_hal_pm_cfg_t;


typedef struct
{
    ca_omci_hal_pm_type_e pmType;
    void (*func)(void *in, void *out);
}ca_omci_hal_pm_cfg_cb_t;


typedef struct
{
    ca_omci_uint16_t gemId;
    ca_omci_uint16_t uniPortId;
    ca_omci_uint32_t portType;
    ca_omci_uint32_t classifierIdx[OMCI_HAL_MAX_RULE_PER_FLOW_US];
    ca_omci_uint32_t dsClassifierIdx[OMCI_HAL_MAX_RULE_PER_FLOW];
    ca_omci_uint32_t defClassifierIdx;
}ca_omci_hal_rule_map_t;


typedef struct
{
    ca_omci_uint16_t gemportIdx;
    ca_omci_bool_t   admin;
}ca_omci_hal_gem_ctl_t;


typedef struct
{
    ca_omci_uint16_t      gemportId;
    ca_omci_hal_gem_ctl_t gemCtl[OMCI_HAL_MAX_PRI_PER_FLOW];
}ca_omci_hal_gem_flow_map_t;


typedef struct
{
    ca_omci_uint32_t seqNum;
    ca_omci_uint64_t second;    //only 6 bytes used
    ca_omci_uint32_t nanoSec;
}ca_omci_hal_onu_tod_cfg_t;


typedef struct
{
    ca_omci_uint32_t keyIdx;
    ca_omci_uint32_t keyFlag;
    ca_omci_uint8_t  keyLen;
    ca_omci_uint8_t  key[OMCI_HAL_PON_KEY_MAX_LEN];
}ca_omci_hal_pon_enc_key_t;


typedef struct
{
    ca_omci_uint32_t maxTcNum;
    ca_omci_uint32_t maxGemNum;
    ca_omci_uint32_t maxQueueNum;
    ca_omci_uint32_t reportMode;
    ca_omci_uint32_t maxQueuePerTc;
}ca_omci_hal_pon_cap_t;


typedef struct
{
    ca_omci_uint16_t voltage;
    ca_omci_uint16_t rxPower;
    ca_omci_uint16_t minLaunchPower;
    ca_omci_uint16_t laserBiasCurrent;
    ca_omci_uint16_t temperature;
    ca_omci_uint16_t rxPowerDbm;
    ca_omci_uint16_t minLaunchPowerDbm;
}ca_omci_hal_pon_info_t;


typedef struct
{
    ca_omci_uint32_t   dsMode;
    ca_omci_st_e     (*untagFunc)(void *data, void *rule, ca_omci_uint16_t type);
    ca_omci_st_e     (*sglTagFunc)(void *data, void *rule);
    ca_omci_st_e     (*dblTagFunc)(void *data, void *rule);
}ca_omci_hal_ds_flow_cb_t;


ca_omci_st_e ca_omci_hal_eth_st_report(void *pPort, void *pState);
ca_omci_st_e ca_omci_hal_pon_link_st(void *pPort, void *pState);
ca_omci_st_e ca_omci_hal_pm_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_pm_clear(void *in);
ca_omci_st_e ca_omci_hal_pon_cap_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_onu_reboot(void *in);
ca_omci_st_e ca_omci_hal_onu_mc_key_set(void *in);
ca_omci_st_e ca_omci_hal_pon_info_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_onu_reg_id_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_pon_tag_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_onu_msk_set(void *pData);
ca_omci_st_e ca_omci_hal_onu_tod_set(void *pData);
ca_omci_st_e ca_omci_hal_onu_dg_alarm_set(void *pData,void *pLen);
ca_omci_st_e ca_omci_hal_onu_pwmng_set(void * pData);
ca_omci_st_e ca_omci_hal_eth_mode_set(void *in, ca_omci_uint8_t mode);
ca_omci_st_e ca_omci_hal_eth_mode_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_eth_lb_set(void *in, ca_omci_uint8_t loopback);
ca_omci_st_e ca_omci_hal_eth_admin_set(void *in, ca_omci_uint8_t admin);
ca_omci_st_e ca_omci_hal_eth_max_frame_set(void *in, ca_omci_uint16_t size);
ca_omci_st_e ca_omci_hal_eth_state_get(void *in, void *out);
ca_omci_st_e ca_omci_hal_tcont_reset(void *in, void *pData);
ca_omci_st_e ca_omci_hal_tcont_set(void *in, void *pData);
ca_omci_st_e ca_omci_hal_tcont_restore(void *in, void *pData);
ca_omci_st_e ca_omci_hal_gemflow_add(void *in);
ca_omci_st_e ca_omci_hal_gemflow_del(void *in);
ca_omci_st_e ca_omci_hal_gem_keyring_set(void *pData, void *keyRing);
ca_omci_st_e ca_omci_hal_dscp_map_set(ca_omci_uint8_t *pData);
ca_omci_st_e ca_omci_hal_us_flow_add(void *in);
ca_omci_st_e ca_omci_hal_us_flow_del(void *in);
ca_omci_st_e ca_omci_hal_filter_flow_add(void *pData);
ca_omci_st_e ca_omci_hal_filter_flow_del(void *pData);
ca_omci_st_e ca_omci_hal_ds_ivs_flow_add(void *in);
ca_omci_st_e ca_omci_hal_ds_ivs_flow_del(void *in);
ca_omci_st_e ca_omci_hal_pkt_tx(void *in, ca_omci_uint32_t len);
ca_omci_st_e ca_omci_hal_pkt_mic_gen(void *in, ca_omci_uint32_t len, ca_omci_uint8_t dir, ca_omci_uint32_t *mic);
ca_omci_st_e ca_omci_hal_init(void);
#endif /* __OMCI_OMCI_HAL_HAL_H__ */

