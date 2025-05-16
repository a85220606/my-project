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
#include "osal_timer.h"
#include "omci_timer.h"
#include "omci_mq.h"
#include "omci_mng_onuinfo.h"


static ca_uint32       sg_omciCfgTimerId = CA_INVALID_TIMER;
static ca_omci_timer_t sg_omciDlTimer;
static ca_omci_timer_t sg_omciImgLedTimer;
static ca_omci_timer_t sg_omciImgActTimer;


extern void ca_omci_cfg_timeout_handle(void *pData);
extern void ca_omci_mng_dl_timeout(void *data);
extern void ca_omci_mng_dl_timeout_handle(void *pData);
extern void ca_omci_mng_img_led_timeout(void *data);
extern void ca_omci_mng_img_led_timeout_handle(void *pData);
extern void ca_omci_mng_img_act_timeout(void *data);
extern void ca_omci_mng_img_act_timeout_handle(void *pData);
extern void ca_omci_next_timer_handler(void *data);
extern void ca_omci_me_ani_g_test_timer_handler(void *data);
extern void ca_omci_key_update_timeout_handle(void *pData);
extern void ca_omci_me_pptp_eth_link_st_timeout_handler(void *data);
extern void ca_omci_mgr_img_upg_done_handle(void *pData);
extern void ca_omci_pm_timeout_handler(void *pData);
extern void ca_omci_me_ani_g_opm_check_timeout_handler(void * data);
extern void ca_omci_cfg_timeout_handle(void * pData);
extern void ca_omci_cfg_timeout(void * data);


void ca_omci_cfg_timer_stop(void)
{
    ca_timer_stop(sg_omciCfgTimerId);
}


void ca_omci_cfg_timer_start(void)
{
    ca_omci_onuinfo_obj_t *onuInfo        = ca_omci_onuinfo_get();
    ca_int32_t             cfgTimerLength = OMCI_TIMER_CFG_FLOW;

    OMCI_ENTER;
    if (OMCI_OLT_TYPE_SIM == onuInfo->oltType)
    {
        //use a shorter timer, the gpon xpert will send downstream packets right after finish provision service
        cfgTimerLength = 1500;
    }

    if (CA_INVALID_TIMER == sg_omciCfgTimerId)
    {
        sg_omciCfgTimerId = ca_circle_timer_add(cfgTimerLength,
                                                ca_omci_cfg_timeout,
                                               (void *)NULL);
        OMCIDBG("cfg timer start %d", sg_omciCfgTimerId);
        return;
    }

    //refresh timer to 3 seconds
    OMCIDBG("refresh timer to %d ", cfgTimerLength);
    ca_timer_start(sg_omciCfgTimerId);
}


void ca_omci_download_timer_id_reset(void)
{
    sg_omciDlTimer.timerId = CA_INVALID_TIMER;
}


void ca_omci_download_timer_del(void)
{
    ca_timer_del(sg_omciDlTimer.timerId);
    sg_omciDlTimer.timerId = CA_INVALID_TIMER;
}


void ca_omci_download_timer_start(void)
{
    OMCI_ENTER;

    if (CA_INVALID_TIMER == sg_omciDlTimer.timerId)
    {
        OMCIDBG("start download timer");
        sg_omciDlTimer.timerId = ca_timer_add(OMCI_TIMER_SWIMG_DOWNLOAD,
                                              ca_omci_mng_dl_timeout,
                                             (void *)NULL);
        sg_omciDlTimer.timerFlag = CA_OMCI_TRUE;
        return;
    }

    ca_timer_start(sg_omciDlTimer.timerId);

    return;
}


void ca_omci_download_timer_stop(void)
{
    if (CA_INVALID_TIMER != sg_omciDlTimer.timerId)
    {
        ca_timer_stop(sg_omciDlTimer.timerId);
    }
}


void ca_omci_img_led_timer_id_reset(void)
{
    sg_omciImgLedTimer.timerId = CA_INVALID_TIMER;
}


void ca_omci_img_led_timer_del(void)
{
    ca_timer_del(sg_omciImgLedTimer.timerId);
    sg_omciImgLedTimer.timerId = CA_INVALID_TIMER;
}


void ca_omci_img_led_timer_start(void)
{
    OMCI_ENTER;

    if (CA_INVALID_TIMER == sg_omciImgLedTimer.timerId)
    {
        OMCIDBG("start image led timer");
        sg_omciImgLedTimer.timerId = ca_timer_add(OMCI_TIMER_IMG_LED_CTRL,
                                                  ca_omci_mng_img_led_timeout,
                                                 (void *)NULL);
        sg_omciImgLedTimer.timerFlag = CA_OMCI_TRUE;
        return;
    }

    ca_timer_start(sg_omciImgLedTimer.timerId);
    return;
}


void ca_omci_img_led_timer_stop(void)
{
    if (CA_INVALID_TIMER != sg_omciImgLedTimer.timerId)
    {
        ca_timer_stop(sg_omciImgLedTimer.timerId);
    }
}


void ca_omci_active_img_timer_start(ca_omci_uint8_t flag)
{
    OMCI_ENTER;

    if (CA_INVALID_TIMER == sg_omciImgActTimer.timerId)
    {
        OMCIDBG("start image active timer");
        sg_omciImgActTimer.timerId = ca_timer_add(OMCI_TIMER_IMG_ACTIVE,
                                                  ca_omci_mng_img_act_timeout,
                                                 (void *)&flag);
        sg_omciImgActTimer.timerFlag = CA_OMCI_TRUE;
        return;
    }
}


static ca_omci_timer_cb_t sg_omciTimerHandleCb[] = 
{
    {CA_OMCI_TIMER_NEXT,                ca_omci_next_timer_handler},
    {CA_OMCI_TIMER_CFG,                 ca_omci_cfg_timeout_handle},
    {CA_OMCI_TIMER_IMG_UPG,             ca_omci_mgr_img_upg_done_handle},
    {CA_OMCI_TIMER_SWIMG_DOWNLOAD,      ca_omci_mng_dl_timeout_handle},
    {CA_OMCI_TIMER_IMG_LED_CTRL,        ca_omci_mng_img_led_timeout_handle},
    {CA_OMCI_TIMER_IMG_ACTIVE,          ca_omci_mng_img_act_timeout_handle},
    {CA_OMCI_TIMER_ME_PPTP_ETH_ARC_IVL, ca_omci_me_pptp_eth_link_st_timeout_handler},
    {CA_OMCI_TIMER_KEY_CHECK,           ca_omci_key_update_timeout_handle},
    {CA_OMCI_TIMER_ME_ANIG_TEST,        ca_omci_me_ani_g_test_timer_handler},
    {CA_OMCI_TIMER_ME_ANIG_OPM_CHECK,   ca_omci_me_ani_g_opm_check_timeout_handler},
    {CA_OMCI_TIMER_PM_CHECK,            ca_omci_pm_timeout_handler},
};


void ca_omci_timeout_handle(void *pData)
{
    ca_omci_inner_msg_t *pInnerMsg = (ca_omci_inner_msg_t *)pData;
    ca_omci_timer_def_e  type      = 0;
    ca_omci_uint8_t      i         = 0;
    ca_omci_uint8_t      data[2048];

    OMCI_PARAM_IS_NULL(pData);
    OMCI_MEMSET(data, 0, 2048);

    OMCI_MEMCPY(&type, pInnerMsg->data, sizeof(ca_omci_timer_def_e));
    OMCI_MEMCPY(data, (pInnerMsg->data + sizeof(ca_omci_timer_def_e)), (pInnerMsg->hdr.dataLen - sizeof(ca_omci_timer_def_e)));

    for (i = 0; i < sizeof(sg_omciTimerHandleCb)/sizeof(ca_omci_timer_handle_t); i++)
    {
        if (sg_omciTimerHandleCb[i].timerDef == type)
        {
            sg_omciTimerHandleCb[i].func(data);
            break;
        }
    }

    if (i >= sizeof(sg_omciTimerHandleCb)/sizeof(ca_omci_timer_handle_t))
    {
        OMCIHALERR("no function for timer(%d)", type);
    }
}

