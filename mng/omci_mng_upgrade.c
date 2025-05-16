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
#include "omci_me.h"
#include "omci_codec_pkt.h"
#include "omci_codec_attr.h"
#include "omci_timer.h"
#include "omci_mng_upgrade.h"
#include "omci_me_soft_image.h"
#include "omci_crc32.h"
#include "omci_hal.h"
#include "osal_timer.h"
#include "osal_common.h"
#include "osal_file.h"
#include "osal_cmn.h"
#include "omci_mq.h"
#include "ca_env_image.h"
#include <drivers/flash.h>


#if OMCI_UPGRADE_SUPPORT
#define MAX_FLASH_W_BLOCK   0x1000
#define OMCI_UPG_STACKSIZE  10240
#define UPG_IMG_INFO_OFFSET 0x20

struct k_thread omci_upg_thread;
K_THREAD_STACK_DEFINE(omci_upg_stack, OMCI_UPG_STACKSIZE);

static struct fs_file_t omci_zfp;
static struct fs_dirent omci_zEntry;
#endif


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


static ca_omci_me_swimage_ctrl_t sg_imageCtl[OMCI_IMG_NUM];
static ca_omci_img_write_st_e    sg_imageWriteSt = OMCI_IMG_WRITE_STATE_SUCCESS;
ca_omci_img_upg_fsm_handler_t    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_MAX][OMCI_IMG_UPG_EVENT_MAX];
ca_omci_mgr_img_upg_ctx_t        img_upg_glb_ctx;

ca_omci_int8_t *ca_omic_mgr_img_upg_state_str[] =
{
    [OMCI_IMG_UPG_STATE_S0]    = "S0",
    [OMCI_IMG_UPG_STATE_0S1]   = "0S1",
    [OMCI_IMG_UPG_STATE_0S2]   = "0S2",
    [OMCI_IMG_UPG_STATE_0S2_1] = "0S2_1",
    [OMCI_IMG_UPG_STATE_0S2_2] = "0S2_2",
    [OMCI_IMG_UPG_STATE_0S3]   = "0S3",
    [OMCI_IMG_UPG_STATE_0S4]   = "0S4",
    [OMCI_IMG_UPG_STATE_1S1]   = "1S1",
    [OMCI_IMG_UPG_STATE_1S2]   = "1S2",
    [OMCI_IMG_UPG_STATE_1S2_1] = "1S2_1",
    [OMCI_IMG_UPG_STATE_1S2_2] = "1S2_2",
    [OMCI_IMG_UPG_STATE_1S3]   = "1S3",
    [OMCI_IMG_UPG_STATE_1S4]   = "1S4",
    [OMCI_IMG_UPG_STATE_MAX]   = "SMAX",
};

ca_omci_int8_t *ca_omic_mgr_img_upg_event_str[] =
{
    [OMCI_IMG_UPG_EVENT_REBOOT]     = "reboot",
    [OMCI_IMG_UPG_EVENT_START_DW_0] = "DL_0",
    [OMCI_IMG_UPG_EVENT_START_DW_1] = "DL_1",
    [OMCI_IMG_UPG_EVENT_DL_SECT_0]  = "DL_SECT_0",
    [OMCI_IMG_UPG_EVENT_DL_SECT_1]  = "DL_SECT_1",
    [OMCI_IMG_UPG_EVENT_DL_END_0]   = "DL_END_0",
    [OMCI_IMG_UPG_EVENT_DL_END_1]   = "DL_END_1",
    [OMCI_IMG_UPG_EVENT_ACTIVE_0]   = "ACTIVE_0",
    [OMCI_IMG_UPG_EVENT_ACTIVE_1]   = "ACTIVE_1",
    [OMCI_IMG_UPG_EVENT_COMMIT_0]   = "COMMIT_0",
    [OMCI_IMG_UPG_EVENT_COMMIT_1]   = "COMMIT_1",
    [OMCI_IMG_UPG_EVENT_MAX]        = "EMAX",
};


/****************************************************************
* For function ca_omci_me_soft_image_auto_create create 2 soft image MEs
* Instance 0 and 1 are used now, So could use inst Id to get ctl directly
* If change the instance ID in function above, need to change this function too!
*****************************************************************/
static ca_omci_me_swimage_ctrl_t *ca_omci_mng_img_ctl_get(ca_omci_uint16_t instId)
{
    if ((0 !=  instId) && (1 != instId))
    {
        return NULL;
    }

    return &sg_imageCtl[instId];
}

static ca_omci_mgr_img_upg_ctx_t *ca_omci_mgr_img_upg_glb_ctx_get(void)
{
    return &img_upg_glb_ctx;
}

int ca_omci_mng_img_open(ca_omci_uint16_t instId)
{
#if OMCI_UPGRADE_SUPPORT
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;
    imgCtl = ca_omci_mng_img_ctl_get(instId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for InstId %d", instId);
        return -1;
    }

    fs_file_t_init(&omci_zfp);
    imgCtl->file = fs_open(&omci_zfp, OMCI_IMG_FILE, FS_O_CREATE | FS_O_RDWR);
    if(imgCtl->file != 0)
    {
        OMCIUPGERR("open file %s failed", OMCI_IMG_FILE);
        return -1;
    }
#endif
    return 0;
}

int ca_omci_mng_img_close(ca_omci_uint16_t instId)
{
#if OMCI_UPGRADE_SUPPORT
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;

    imgCtl = ca_omci_mng_img_ctl_get(instId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for InstId %d", instId);
        return -1;
    }

    fs_close(&omci_zfp);
    imgCtl->file = -1;
#endif
    return 0;
}

int ca_omci_mng_img_write_exe(ca_omci_uint16_t instId)
{
#if OMCI_UPGRADE_SUPPORT
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;
    ca_omci_uint32_t          len  = 0;
    volatile ca_omci_uint32_t write_len = 0;

    imgCtl = ca_omci_mng_img_ctl_get(instId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for InstId %d", instId);
        return -1;
    }

    if(imgCtl->file != 0)
    {
        OMCIUPGERR("file not open, can't write file");
        return -1;
    }

    for(;;)
    {
        if((len = fs_write(&omci_zfp, imgCtl->win.buf+write_len,  imgCtl->win.bufLen-write_len)) <=0)
        {
            //perror("write");
            //OMCIUPGERR("write len %d", len);
            break;
        }
        write_len += len;
    }
#endif
    return 0;
}

void ca_omci_mgr_img_offset_exe(ca_omci_me_swimage_ctrl_t *imgCtl)
{
#if OMCI_UPGRADE_SUPPORT
    ca_omci_int32_t ret = 0;

    OMCIUPGDBG("imgCtl->commitLen %d imgCtl->pre_commitLen %d", imgCtl->commitLen, imgCtl->pre_commitLen);
    imgCtl->commitLen = imgCtl->pre_commitLen;
    ret = fs_seek(&omci_zfp, imgCtl->pre_commitLen, FS_SEEK_SET);
    if(ret != 0)
    {
        OMCIUPGERR("img offset exe err");
        return;
    }
#endif
    return;
}

static void ca_omci_mng_img_write(ca_omci_uint16_t instId)
{
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;

    OMCI_ENTER;

    imgCtl = ca_omci_mng_img_ctl_get(instId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        return;
    }

    /* restart download timer */
    ca_omci_download_timer_stop();
    ca_omci_download_timer_start();

    /* restart led ctrl timer */
    ca_omci_img_led_timer_stop();
    ca_omci_img_led_timer_start();

    /* LED blink */
    // need to call functions for led blink

    // need to call functions to write file block
    ca_omci_mng_img_write_exe(instId);

}

static void ca_omci_mng_img_clean(void)
{
#if OMCI_UPGRADE_SUPPORT
    OMCI_ENTER;
    ca_omci_int32_t ret = 0;

    if (0 != fs_stat(OMCI_IMG_FILE, &omci_zEntry)) {
        OMCIUPGDBG("file %s is not exist", OMCI_IMG_FILE);
        return;
    }

    // need to call functions for clean the image in flash
    ret = fs_unlink(OMCI_IMG_FILE);
    if(ret != 0)
    {
        OMCIUPGDBG("remove file err");
        perror("remove");
    }
#endif
}

ca_omci_int32_t ca_omci_mng_img_upgrade(void)
{
#if OMCI_UPGRADE_SUPPORT
    const struct device *flash_dev;
    uint32_t         pageAddr = 0x420000;
    uint32_t         size     = 0x200000;
    int              ret      = 0;
    ca_omci_uint8_t  active   = 0;
    char            *buf      = NULL;
    int              fileS    = 0;
    ca_omci_uint8_t  ver[64]  = {0};
    uint32_t         imgIdx   = 1;
    uint32_t         offset   = 0;

    /* Default treat img1 as backup part, if img 0 is not active, it means img0 is backup part*/
    ret = CA_IMG_ACTIVE_GET(0, &active);
    if (0 != ret) {
        OMCIUPGERR("Can't upgrade img for unknow active %d part, ret%d", active, ret);
        return CA_OMCI_E;
    }

    if (0 == active){
        pageAddr = 0x120000;
        imgIdx = 0;
    }

    buf = ca_malloc(MAX_FLASH_W_BLOCK);
    if (!buf) {
        OMCIUPGERR("Malloc buffer for image failed");
        return CA_OMCI_E;
    }

    flash_dev = device_get_binding("SPI_NOR");
    ret       = flash_erase(flash_dev, pageAddr, size);
    if (ret) {
        OMCIUPGERR("Erase failed, code %d", ret);
    } else {
        ca_printf("Erase done\r\n");
    }

    fs_file_t_init(&omci_zfp);
    if (0 != fs_open(&omci_zfp, OMCI_IMG_FILE, FS_O_READ)) {
        OMCIUPGERR("Open img file failed");
        ca_free(buf);
        return CA_OMCI_E;
    }

    memset(buf, 0, MAX_FLASH_W_BLOCK);
    fileS = fs_read(&omci_zfp, buf, MAX_FLASH_W_BLOCK);
    if (fileS <= 0) {
        OMCIUPGERR("Read header failed");
        ca_free(buf);
        return CA_OMCI_E;
    }

    memcpy(ver, (buf + UPG_IMG_INFO_OFFSET), OMCI_IMG_VERSION_LEN);

    /*Adjust to the beginning of the file, and then to read the whole file*/
    fs_seek(&omci_zfp, 0, FS_SEEK_SET);
    while (1) {
        memset(buf, 0, MAX_FLASH_W_BLOCK);
        fileS = fs_read(&omci_zfp, buf, MAX_FLASH_W_BLOCK);
        if (fileS == MAX_FLASH_W_BLOCK) {
            ret = flash_write(flash_dev, (pageAddr + (offset * MAX_FLASH_W_BLOCK)), buf, MAX_FLASH_W_BLOCK);
            if (ret) {
                OMCIUPGERR("Write failed, code %d", ret);
                ca_free(buf);
                return CA_OMCI_E;
            }
            offset++;
        }
        else if ((fileS < MAX_FLASH_W_BLOCK) && (fileS >= 0)){
            ret = flash_write(flash_dev, (pageAddr + (offset * MAX_FLASH_W_BLOCK)), buf, MAX_FLASH_W_BLOCK);
            if (ret) {
                OMCIUPGERR("Write failed, code %d", ret);
                ca_free(buf);
                return CA_OMCI_E;
            }
            OMCIUPGDBG("Write the last block size %d", fileS);
            break;
        }
        else{
            OMCIUPGERR("Size err %d", fileS);
            break;
        }
    }
    ca_free(buf);
    fs_close(&omci_zfp);
    ca_printf("Write S%d done\r\n", imgIdx);
    CA_IMG_EXECUTE_SET(imgIdx, 0);
    CA_IMG_VALID_SET(imgIdx, 1);

    OMCIUPGDBG("Upgrade version %s", ver);
    CA_IMG_VERSION_SET(imgIdx, ver);
#endif
    return 0;
}


void ca_omci_mgr_img_upg_done(void)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_IMG_UPG;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_timer_def_e);//no pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, msg.hdr.dataLen);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}

#if OMCI_UPGRADE_SUPPORT
void ca_omci_mng_img_end_entry(void *p1, void *p2, void *p3)
{
    (void)p1;
    (void)p2;
    (void)p3;

    sg_imageWriteSt = OMCI_IMG_WRITE_STATE_WRITING;

    /* write image */
    ca_omci_mng_img_upgrade();

    sg_imageWriteSt = OMCI_IMG_WRITE_STATE_SUCCESS;

    //report download end here
    ca_omci_mgr_img_upg_done();
}
#endif

void ca_omci_mgr_img_upg_done_handle(void *pData)
{
    ca_omci_mgr_img_upg_ctx_t *img_upg_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    (void)pData;

    OMCIUPGDBG("pre upg state :%s\r\n", ca_omic_mgr_img_upg_state_str[img_upg_ctx->upg_pre_state]);
    OMCIUPGDBG("    upg state :%s\r\n", ca_omic_mgr_img_upg_state_str[img_upg_ctx->upg_state]);
    OMCIUPGDBG("    upg state :%s\r\n", ca_omic_mgr_img_upg_event_str[img_upg_ctx->upg_event]);

    img_upg_ctx->upg_pre_state = img_upg_ctx->upg_state;
    if(OMCI_IMG_UPG_STATE_0S2_1 == img_upg_ctx->upg_pre_state)
    {
        img_upg_ctx->upg_state = OMCI_IMG_UPG_STATE_0S2_2;
        return;
    }

    if(OMCI_IMG_UPG_STATE_1S2_1 == img_upg_ctx->upg_pre_state)
    {
        img_upg_ctx->upg_state = OMCI_IMG_UPG_STATE_1S2_2;
    }

    return;
}


void ca_omci_mgr_img_upg_done_timeout(void *pData)
{
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_IMG_UPG;
    ca_omci_inner_msg_t msg;

    (void)pData;

    if (OMCI_IMG_WRITE_STATE_SUCCESS == sg_imageWriteSt)
        return;

    OMCIERR("ca.upgrade can't finish in one minute");

    OMCI_MEMSET(&msg, 0, sizeof(msg));
    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_timer_def_e);//no pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef, msg.hdr.dataLen);
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


static ca_omci_int32_t ca_omci_mgr_img_upg_done_timer_del(ca_omci_mebase_t *me)
{
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;

    if(!me)
        return -1;

    imgCtl = ca_omci_mng_img_ctl_get(me->instanceId);
    if(imgCtl->timerId != CA_INVALID_TIMER)
    {
        ca_timer_del(imgCtl->timerId);
        imgCtl->timerId = CA_INVALID_TIMER;
    }

    return 0;
}

static ca_omci_st_e ca_omci_mng_img_end(
    ca_omci_uint16_t imgId,
    ca_omci_int32_t  file
)
{
#if OMCI_UPGRADE_SUPPORT
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;
    k_tid_t                    upgTid;

    (void)file;

    imgCtl = ca_omci_mng_img_ctl_get(imgId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        return CA_OMCI_E;
    }

    /* delete download and led ctl timer */
    ca_omci_download_timer_del();
    ca_omci_img_led_timer_del();
    ca_omci_mng_img_close(imgId);
    imgCtl->crc = ca_omci_file_cal_crc32(OMCI_IMG_FILE);

    upgTid = k_thread_create(&omci_upg_thread, omci_upg_stack, OMCI_UPG_STACKSIZE,
                              ca_omci_mng_img_end_entry, NULL, NULL, NULL, -1, K_USER, K_NO_WAIT);
    k_thread_name_set(upgTid, "omci_upg_stack");

    //start a timer to reset the state match when ca.upgrade hang
    imgCtl->timerId = ca_timer_add(OMCI_TIMER_IMG_UPG,
                                   ca_omci_mgr_img_upg_done_timeout,
                                   (void *)NULL);
#endif
    return CA_OMCI_OK;
}

static ca_omci_st_e ca_omci_mng_img_start(
    ca_omci_uint16_t  instId,
    ca_omci_int32_t  *pFile)
{
    OMCI_PARAM_NULL_RETURN(pFile)

    if (*pFile != -1)
    {
        ca_omci_mng_img_end(instId, *pFile);
    }

    // need to call functions for start to download the image

    ca_omci_download_timer_start();
    ca_omci_img_led_timer_start();

    /* LED blink */
    // need to call functions for blink led
    ca_omci_mng_img_close(instId);
    ca_omci_mng_img_clean();
    ca_omci_mng_img_open(instId);

    return CA_OMCI_OK;
}

static void ca_omci_mng_img_clt_win_reset(ca_omci_me_swimage_ctrl_t *imgCtl)
{
    OMCI_PARAM_IS_NULL(imgCtl);

    imgCtl->win.sectionNum = 0xff;
    imgCtl->win.crc        = 0;
    imgCtl->win.error      = 0;
    imgCtl->win.bufLen     = 0;
    imgCtl->win.writeLen   = 0;
    OMCI_MEMSET(imgCtl->win.buf, 0x00, OMCI_IMG_MAX_BUF_LEN);
}

void ca_omci_mgr_img_upg_state_update(
    ca_omci_soft_image_t *swImg,
    ca_omci_soft_image_t *another_swImg)
{
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    if(!(swImg->meBase.instanceId))
    {
        if(CA_OMCI_IMG_GET_ACTICE(swImg) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
            (!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (!CA_OMCI_IMG_GET_VALID(another_swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S1;
            goto out;
        }

        if(CA_OMCI_IMG_GET_ACTICE(swImg) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
            (!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (CA_OMCI_IMG_GET_VALID(another_swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S3;
            goto out;
        }

        if((!CA_OMCI_IMG_GET_ACTICE(swImg)) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
            (CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (CA_OMCI_IMG_GET_VALID(another_swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S4;
            goto out;
        }

        if(CA_OMCI_IMG_GET_ACTICE(another_swImg) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
            (!CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (!CA_OMCI_IMG_GET_VALID(swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S1;
            goto out;
        }

        if(CA_OMCI_IMG_GET_ACTICE(another_swImg) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
            (!CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (CA_OMCI_IMG_GET_VALID(swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S3;
            goto out;
        }

        if((!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
            (CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (CA_OMCI_IMG_GET_VALID(swImg)) )
        {
            img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S4;
            goto out;
        }

        OMCIUPGERR("uncalculate the state");
        goto out;
    }

    if(CA_OMCI_IMG_GET_ACTICE(swImg) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
        (!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (!CA_OMCI_IMG_GET_VALID(another_swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S1;
        goto out;
    }

    if(CA_OMCI_IMG_GET_ACTICE(swImg) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
        (!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (CA_OMCI_IMG_GET_VALID(another_swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S3;
        goto out;
    }

    if((!CA_OMCI_IMG_GET_ACTICE(swImg)) && CA_OMCI_IMG_GET_COMMIT(swImg) && CA_OMCI_IMG_GET_VALID(swImg) &&
        (CA_OMCI_IMG_GET_ACTICE(another_swImg)) && (!CA_OMCI_IMG_GET_COMMIT(another_swImg)) && (CA_OMCI_IMG_GET_VALID(another_swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_1S4;
        goto out;
    }

    if(CA_OMCI_IMG_GET_ACTICE(another_swImg) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
        (!CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (!CA_OMCI_IMG_GET_VALID(swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S1;
        goto out;
    }

    if(CA_OMCI_IMG_GET_ACTICE(another_swImg) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
        (!CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (CA_OMCI_IMG_GET_VALID(swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S3;
        goto out;
    }

    if((!CA_OMCI_IMG_GET_ACTICE(another_swImg)) && CA_OMCI_IMG_GET_COMMIT(another_swImg) && CA_OMCI_IMG_GET_VALID(another_swImg) &&
        (CA_OMCI_IMG_GET_ACTICE(swImg)) && (!CA_OMCI_IMG_GET_COMMIT(swImg)) && (CA_OMCI_IMG_GET_VALID(swImg)) )
    {
        img_up_ctx->upg_state = OMCI_IMG_UPG_STATE_0S4;
        goto out;
    }

out:
    OMCIUPGDBG("img upg state %d", img_up_ctx->upg_state);
    return;
}

static ca_omci_st_e ca_omci_mng_img_active(ca_omci_uint16_t imgId, ca_omci_uint8_t flag)
{
    (void)imgId;

    //need to call function for active image
    ca_omci_active_img_timer_start(flag);

    return CA_OMCI_OK;
}

void ca_omci_mng_dl_timeout_handle(void *pData)
{
    (void)pData;

    OMCIUPGDBG("Image download canceled because of timer expire");
    ca_omci_mng_img_clean();
    ca_omci_download_timer_id_reset();
}


void ca_omci_mng_dl_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e  timerDef = CA_OMCI_TIMER_SWIMG_DOWNLOAD;

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


void ca_omci_mng_img_led_timeout_handle(void *pData)
{
    /* LED end blink */
    // need to call functions for blink led end
    (void)pData;

    ca_omci_img_led_timer_id_reset();
}


void ca_omci_mng_img_led_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_IMG_LED_CTRL;

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


void ca_omci_mng_img_act_timeout_handle(void *pData)
{
    ca_omci_hal_onu_reboot(pData);
}


void ca_omci_mng_img_act_timeout(void *data)
{
    ca_omci_inner_msg_t msg;
    ca_omci_timer_def_e timerDef = CA_OMCI_TIMER_IMG_ACTIVE;

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


void ca_omci_mng_st_dl_process(
    ca_omci_mebase_t       *me,
    ca_omci_uint8_t        *contents,
    ca_omci_me_st_dl_rsl_t *rsl)
{
    ca_omci_soft_image_t      *swImage    = (ca_omci_soft_image_t *)me;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(contents);
    OMCI_PARAM_IS_NULL(rsl);

    if(swImage->meBase.instanceId)
    {
        img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_START_DW_1](me, (void *)contents, (void *)rsl);
        return;
    }

    img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_START_DW_0](me, (void *)contents, (void *)rsl);
    return;
}

void ca_omci_mng_dl_sec_process(
    ca_omci_mebase_t    *me,
    ca_omci_me_dl_sec_t *secInfo,
    ca_omci_uint8_t     *rsl)
{
    ca_omci_soft_image_t      *swImage    = (ca_omci_soft_image_t *)me;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(secInfo);
    OMCI_PARAM_IS_NULL(rsl);

    if(swImage->meBase.instanceId)
    {
        img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_DL_SECT_1](me, (void *)secInfo, (void *)rsl);
        return;
    }

    img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_DL_SECT_0](me, (void *)secInfo, (void *)rsl);
    return;
}

void ca_omci_mng_end_dl_process(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *contents,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage    = (ca_omci_soft_image_t *)me;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(contents);
    OMCI_PARAM_IS_NULL(rsl);

    if(swImage->meBase.instanceId)
    {
        img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_DL_END_1](me, (void *)contents, (void *)rsl);
        return;
    }

    img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_DL_END_0](me, (void *)contents, (void *)rsl);
    return;
}

void ca_omci_mng_img_active_process(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t   flag,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage    = (ca_omci_soft_image_t *)me;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(rsl);

    if(swImage->meBase.instanceId)
    {
        img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_ACTIVE_1](me, &flag, rsl);
        return;
    }

    img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_ACTIVE_0](me, &flag, rsl);
    return;
}

void ca_omci_mng_img_commit_process(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage    = (ca_omci_soft_image_t *)me;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(rsl);

    if(swImage->meBase.instanceId)
    {
        img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_COMMIT_1](me, NULL, rsl);
        return;
    }

    img_upg_fsm_handler[img_up_ctx->upg_state][OMCI_IMG_UPG_EVENT_COMMIT_0](me, NULL, rsl);
    return;
}


void ca_omci_mng_me_sw_ver_upg(void *pData)
{
    ca_omci_soft_image_t      *swImage     = (ca_omci_soft_image_t *)pData;
    ca_omci_mgr_img_upg_ctx_t *img_upg_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t            version[64];
    ca_omci_int32_t            ret = 0;

    if ((OMCI_IMG_UPG_STATE_0S2_2 != img_upg_ctx->upg_state)
     && (OMCI_IMG_UPG_STATE_1S2_2 != img_upg_ctx->upg_state))
    {
        return;
    }

    if (!swImage)
    {
        OMCIUPGERR("sw img me is NULL");
        return;
    }

    memset(version, 0x0, sizeof(version));
    ret = CA_IMG_VERSION_GET(swImage->meBase.instanceId, version);
    if(ret != CA_E_OK)
    {
        OMCIUPGERR("img info get error InstanceId %d ret %d", swImage->meBase.instanceId, ret);
        return;
    }

    memcpy(swImage->attr.version, version, OMCI_IMG_VERSION_LEN);
    OMCIUPGDBG("Instance %d version [%s]", swImage->meBase.instanceId, swImage->attr.version);
}


ca_omci_int32_t ca_omci_mng_me_sw_img_init(ca_omci_soft_image_t *swImage)
{
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;
    ca_omci_int32_t            ret    = 0;
    ca_omci_uint8_t            active = 0;
    ca_omci_uint8_t            commit = 0;
    ca_omci_uint8_t            valid  = 0;
    ca_omci_uint8_t            ver[64] = {0};

    if(!swImage)
    {
        OMCIUPGERR("sw img me is NULL");
        return -1;
    }

    ret += CA_IMG_ACTIVE_GET(swImage->meBase.instanceId, &active);
    ret += CA_IMG_COMMIT_GET(swImage->meBase.instanceId, &commit);
    ret += CA_IMG_VALID_GET(swImage->meBase.instanceId, &valid);
    ret += CA_IMG_VERSION_GET(swImage->meBase.instanceId, ver);
    if(ret != CA_E_OK)
    {
        OMCIUPGERR("img info get error InstanceId %d ret %d", swImage->meBase.instanceId, ret);
        return -1;
    }

    imgCtl = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    if(!imgCtl)
    {
        OMCIUPGERR("can't get img ctl for me instanceId %d", swImage->meBase.instanceId);
        return -1;
    }

    swImage->attr.isActive = active;
    swImage->attr.isCommit = commit;
    swImage->attr.isValid  = valid;
    memcpy(swImage->attr.version, ver, OMCI_IMG_VERSION_LEN);
    OMCIUPGDBG("Instance %d isActive %d isCommit %d isValid %d version %s", swImage->meBase.instanceId,
                swImage->attr.isActive, swImage->attr.isCommit, swImage->attr.isValid, swImage->attr.version);
    return 0;
}

/*Need to clean the upgrade state when OMCC link down*/
void ca_omci_mng_upgrade_cancle(void)
{
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;

    ca_omci_download_timer_stop();
    ca_omci_img_led_timer_stop();

    imgCtl = ca_omci_mng_img_ctl_get(0);
    if (NULL != imgCtl) {
        if (-1 != imgCtl->file) {
            ca_omci_mng_img_close(0);
            imgCtl->file = -1;
        }

        if (CA_INVALID_TIMER != imgCtl->timerId) {
            ca_timer_stop(imgCtl->timerId);
            imgCtl->timerId = CA_INVALID_TIMER;
        }
    }

    imgCtl = ca_omci_mng_img_ctl_get(1);
    if (NULL != imgCtl) {
        if (-1 != imgCtl->file){
            ca_omci_mng_img_close(1);
            imgCtl->file = -1;
        }

        if (CA_INVALID_TIMER != imgCtl->timerId) {
            ca_timer_stop(imgCtl->timerId);
            imgCtl->timerId = CA_INVALID_TIMER;
        }
    }

    ca_omci_mng_img_clean();
}

void ca_omci_mgr_start_dl_img(
    ca_omci_soft_image_t   *swImage,
    ca_omci_uint8_t        *contents,
    ca_omci_me_st_dl_rsl_t *rsl)
{
    ca_omci_me_swimage_ctrl_t *imgCtl   = NULL;
    ca_omci_uint8_t            imgWin   = 0;
    ca_omci_uint8_t            win      = OMCI_IMG_MAX_WIN_SIZE;
    ca_omci_uint32_t           imgSize  = 0;
    ca_omci_uint32_t           offset   = 0;

    OMCI_PARAM_IS_NULL(swImage);
    OMCI_PARAM_IS_NULL(contents);
    OMCI_PARAM_IS_NULL(rsl);

    imgCtl = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        rsl->result = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    imgWin = *(contents);
    offset += sizeof(ca_omci_uint8_t);
    OMCI_MEMCPY(&imgSize, contents + offset, sizeof(ca_omci_uint32_t));
    offset += sizeof(ca_omci_uint32_t);
    imgSize = ca_omci_ntohl(imgSize);

    imgCtl->imgSize   = imgSize;
    imgCtl->crc       = 0;
    imgCtl->commitLen = 0;
    imgCtl->pre_commitLen = 0;

    OMCIUPGDBG("imgWin %d imgSize %d\r\n", imgWin, imgSize);
    win = win > imgWin ? imgWin : win;
    rsl->windowSize = win;

    /* packet window fields is the windows -1 */
    imgCtl->win.windowSize = win + 1;
    imgCtl->win.sectionNum = 0xFF;
    ca_omci_mng_img_clt_win_reset(imgCtl);
    OMCIUPGDBG("nego windows size = %d \n", imgCtl->win.windowSize);

    if (CA_OMCI_OK != ca_omci_mng_img_start(swImage->meBase.instanceId,
                                           &imgCtl->file))
    {
        OMCIUPGERR("init the flash failed");
        rsl->result = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    swImage->attr.isValid = 0;
    rsl->result           = OMCI_MSG_RST_SUCCESSFUL;
}

void ca_omci_mgr_dl_sec(
    ca_omci_mebase_t    *me,
    ca_omci_me_dl_sec_t *secInfo,
    ca_omci_uint8_t     *rsl)
{
    ca_omci_uint32_t          totalWriteLen = 0;
    ca_omci_uint8_t           expectSeq     = 0;
    ca_omci_me_swimage_ctrl_t *imgCtl        = NULL;
    ca_omci_uint16_t          instId;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(secInfo);
    OMCI_PARAM_IS_NULL(rsl);

    instId = me->instanceId;
    imgCtl = ca_omci_mng_img_ctl_get(instId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        *rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    /*error happen before*/
    if (imgCtl->win.error )
    {
        OMCIUPGERR("Error happen in prev, so ignore this pkt");
        goto Error;
    }

    expectSeq = imgCtl->win.sectionNum + 1;

    //printk("recv sectionNUm %d expectNum %d \r\n", secInfo->sectionNum, expectSeq);
    /*check the section num*/
    if (secInfo->sectionNum != expectSeq)
    {
        OMCIUPGERR("wrong section num: %d, expect:%d", secInfo->sectionNum, expectSeq);
        if((expectSeq == 0) && (secInfo->sectionNum == (imgCtl->win.windowSize - 1)))
        {
            OMCIUPGDBG("set file offset");
            ca_omci_mgr_img_offset_exe(imgCtl);
        }
        goto Error;
    }
    else if (secInfo->sectionNum >= imgCtl->win.windowSize)
    {
        OMCIUPGERR("section > win size :section num %d , window size:%d", secInfo->sectionNum, imgCtl->win.windowSize);
        goto Error;
    }

    imgCtl->win.sectionNum = secInfo->sectionNum;

    totalWriteLen = imgCtl->commitLen + secInfo->dataLen + imgCtl->win.writeLen + imgCtl->win.bufLen;

    /*make sure the recv len is not large than the image size*/
    if (totalWriteLen > imgCtl->imgSize + OMCI_MSG_IMG_DATA_LEN)
    {
        OMCIUPGERR("the recv len %d is greater the the image size %d", totalWriteLen, imgCtl->imgSize);
        goto Error;
    }

    /*cal the last pkt real len*/
    if (totalWriteLen  > imgCtl->imgSize)
    {
        OMCIUPGDBG("totalWriteLen=%d commitLen %d datalen %d writelen %d buflen%d imgSize %d",
                totalWriteLen, imgCtl->commitLen, secInfo->dataLen, imgCtl->win.writeLen,
                imgCtl->win.bufLen, imgCtl->imgSize);
        secInfo->dataLen = imgCtl->imgSize - (totalWriteLen - secInfo->dataLen);
        OMCIUPGDBG("last pkt len = %d", secInfo->dataLen);
    }

    if (imgCtl->win.bufLen + secInfo->dataLen > OMCI_IMG_MAX_BUF_LEN)
    {
        OMCIUPGERR("buf size than the window buf size");
        goto Error;
    }

    /*copy the buf*/
    OMCI_MEMCPY(imgCtl->win.buf + imgCtl->win.bufLen, secInfo->data, secInfo->dataLen);
    imgCtl->win.bufLen += secInfo->dataLen;

    if (secInfo->isLast)
    {
        /* don't check the write status,
        * we will use the verify to verify the image write success or not */
        ca_omci_mng_img_write(instId);

        imgCtl->win.writeLen += imgCtl->win.bufLen;
        OMCI_MEMSET(imgCtl->win.buf, 0, OMCI_IMG_MAX_BUF_LEN);

        /*commit this window*/
        imgCtl->crc           = imgCtl->win.crc;
        imgCtl->pre_commitLen = imgCtl->commitLen;
        imgCtl->commitLen     += imgCtl->win.bufLen;

        OMCIUPGDBG("imgCtl->commitLen %d imgCtl->pre_commitLen %d", imgCtl->commitLen, imgCtl->pre_commitLen);

        /*reset the window to wait next window*/
        ca_omci_mng_img_clt_win_reset(imgCtl);
        *rsl= OMCI_MSG_RST_SUCCESSFUL;
    }
    return;

Error:
    /*in the mid has error, set the error*/
    *rsl = OMCI_MSG_RST_PROC_ERR;

    if (expectSeq != 0)
    {
        imgCtl->win.error = 1;
    }

    if (secInfo->isLast)
    {
        /* reset window and wait start this window again */
        ca_omci_mng_img_clt_win_reset(imgCtl);
    }

    return;
}

void ca_omci_mgr_end_dl_start_upgrade(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *contents,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage = (ca_omci_soft_image_t *)me;
    ca_omci_me_swimage_ctrl_t *imgCtl  = NULL;
    ca_omci_uint32_t           crc     = 0;
    ca_omci_uint32_t           imgSize = 0;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(contents);
    OMCI_PARAM_IS_NULL(rsl);

    imgCtl = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        *rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    OMCI_MEMCPY(&crc, contents, sizeof(ca_omci_uint32_t));
    OMCI_MEMCPY(&imgSize, (contents + sizeof(ca_omci_uint32_t)), sizeof(ca_omci_uint32_t));
    crc         = ca_omci_ntohl(crc);
    imgSize     = ca_omci_ntohl(imgSize);

    ca_omci_mng_img_close(swImage->meBase.instanceId);
    imgCtl->crc = ca_omci_file_cal_crc32(OMCI_IMG_FILE);

    OMCIUPGDBG("crc = %#x, img size = %d", crc, imgSize);

    /*reset the download status */
    if (0 == imgSize)
    {
        OMCIUPGDBG("reset the download status");
        imgCtl->imgSize        = 0;
        imgCtl->crc            = 0;
        imgCtl->commitLen      = 0;
        imgCtl->win.windowSize = 0;

        ca_omci_mng_img_clt_win_reset(imgCtl);
        *rsl = OMCI_MSG_RST_SUCCESSFUL;
        return;
    }

    if (crc != imgCtl->crc || imgSize != imgCtl->imgSize)
    {
        OMCIUPGERR("CRC or image size not match,cal crc:%#x , img size:%d, recv crc:%#x, img_size :%d",
                   imgCtl->crc, imgCtl->imgSize, crc, imgSize);
        *rsl = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    if (CA_OMCI_OK != ca_omci_mng_img_end(me->instanceId, imgCtl->file))
    {
        OMCIUPGERR("end write image fail");
        *rsl = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    *rsl = OMCI_MSG_RST_DEV_BUSY;
    return;
}

void ca_omci_mgr_end_dl(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *contents,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage = (ca_omci_soft_image_t *)me;
    ca_omci_me_swimage_ctrl_t *imgCtl  = NULL;
    ca_omci_uint32_t           crc     = 0;
    ca_omci_uint32_t           imgSize = 0;
    ca_omci_int32_t            ret     = 0;
    ca_omci_uint8_t            valid   = 0;
    ca_omci_uint8_t            version[64] = {0};

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(contents);
    OMCI_PARAM_IS_NULL(rsl);

    imgCtl = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    if (NULL == imgCtl)
    {
        OMCIUPGERR("Can't get image ctl for ME");
        *rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    OMCI_MEMCPY(&crc, contents, sizeof(ca_omci_uint32_t));
    OMCI_MEMCPY(&imgSize, (contents + sizeof(ca_omci_uint32_t)), sizeof(ca_omci_uint32_t));
    crc     = ca_omci_ntohl(crc);
    imgSize = ca_omci_ntohl(imgSize);

    OMCIUPGDBG("crc = %#x, img size = %d", crc, imgSize);

    /*reset the download status */
    if (0 == imgSize)
    {
        OMCIUPGDBG("reset the download status");
        imgCtl->imgSize        = 0;
        imgCtl->crc            = 0;
        imgCtl->commitLen      = 0;
        imgCtl->win.windowSize = 0;

        ca_omci_mng_img_clt_win_reset(imgCtl);
        *rsl = OMCI_MSG_RST_SUCCESSFUL;
        return;
    }

    ret += CA_IMG_VERSION_GET(swImage->meBase.instanceId, version);
    ret += CA_IMG_VALID_GET(swImage->meBase.instanceId, &valid);
    if(ret != CA_E_OK)
    {
        OMCIUPGERR("img info get error InstanceId %d ret %d", swImage->meBase.instanceId, ret);
        *rsl = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    OMCIUPGDBG("get img Instance %d info valid flag %d", swImage->meBase.instanceId, valid);
    swImage->attr.isValid = valid;
    if(swImage->attr.isValid)
    {
        memcpy(swImage->attr.version, version, OMCI_IMG_VERSION_LEN);
        *rsl = OMCI_MSG_RST_SUCCESSFUL;
        return;
    }

    OMCIUPGERR("img upgrade err");
    *rsl = OMCI_MSG_RST_PROC_ERR;
    return;
}

void ca_omci_mgr_img_env_debug(void)
{
    ca_omci_uint8_t active0 = 0;
    ca_omci_uint8_t active1 = 0;
    ca_omci_uint8_t commit0 = 0;
    ca_omci_uint8_t commit1 = 0;
    ca_omci_uint8_t valid0  = 0;
    ca_omci_uint8_t valid1  = 0;
    ca_omci_int32_t ret     = 0;

    ret += CA_IMG_ACTIVE_GET(0, &active0);
    ret += CA_IMG_COMMIT_GET(0, &commit0);
    ret += CA_IMG_VALID_GET(0, &valid0);
    if(ret)
    {
        OMCIUPGDBG("img 0 info get err");
        return;
    }

    ret += CA_IMG_ACTIVE_GET(1, &active1);
    ret += CA_IMG_COMMIT_GET(1, &commit1);
    ret += CA_IMG_VALID_GET(1, &valid1);
    if(ret)
    {
        OMCIUPGDBG("img 1 info get err");
        return;
    }

    OMCIUPGDBG("Image 0 isActive %d isCommit %d isValid %d", active0, commit0, valid0);
    OMCIUPGDBG("Image 1 isActive %d isCommit %d isValid %d", active1, commit1, valid1);
    return ;
}

ca_omci_int32_t ca_omci_mgr_img_upg_active_env_set(
    ca_omci_soft_image_t  *swImage0,
    ca_omci_int32_t       flag0,
    ca_omci_soft_image_t  *swImage1,
    ca_omci_int32_t       flag1)
{
    ca_omci_int32_t ret     = 0;
    ca_omci_uint8_t active0 = 0;

    (void)flag1;

    ret = CA_IMG_ACTIVE_GET(swImage0->meBase.instanceId, &active0);
    if (ret)
    {
        OMCIUPGERR("img Instance=%d info get err from mtd", swImage0->meBase.instanceId);
        return -1;
    }

    ret = CA_IMG_ACTIVE_SET(swImage0->meBase.instanceId, flag0);
    if (ret)
    {
        OMCIUPGERR("img Instance=%d active env set err", swImage0->meBase.instanceId);
        return -1;
    }

    ret = CA_IMG_ACTIVE_SET(swImage1->meBase.instanceId, flag1);
    if (ret)
    {
        OMCIUPGERR("img Instance=%d active env set err", swImage1->meBase.instanceId);
        if(active0 != flag0)
        {
            ret = CA_IMG_ACTIVE_SET(swImage0->meBase.instanceId, active0);
            if(ret)
            {
                OMCIUPGERR("fatal err restore img Instance %d active env error", swImage0->meBase.instanceId);
            }
        }

        return -1;
    }

    ca_omci_mgr_img_env_debug();
    return 0;
}

ca_omci_int32_t ca_omci_mgr_img_upg_commit_env_set(
    ca_omci_soft_image_t  *swImage0,
    ca_omci_int32_t       flag0,
    ca_omci_soft_image_t  *swImage1,
    ca_omci_int32_t       flag1)
{
    ca_omci_int32_t ret = 0;
    ca_omci_uint8_t commit0 = 0;

    (void)flag1;

    ret = CA_IMG_COMMIT_GET(swImage0->meBase.instanceId, &commit0);
    if (ret)
    {
        OMCIUPGERR("img Instance=%d info get err from mtd", swImage0->meBase.instanceId);
        return -1;
    }

    ret = CA_IMG_COMMIT_SET(swImage0->meBase.instanceId, flag0);
    if (ret)
    {
        OMCIUPGERR("fatal err img Instance=%d commit env set err", swImage0->meBase.instanceId);
        return -1;
    }

    ret = CA_IMG_COMMIT_SET(swImage1->meBase.instanceId, flag1);
    if (ret)
    {
        OMCIUPGERR("fatal err img Instance=%d commit env set err", swImage1->meBase.instanceId);

        if(commit0 != flag0)
        {
            ret = CA_IMG_COMMIT_SET(swImage0->meBase.instanceId, commit0);
            if (ret)
            {
                OMCIUPGERR("fatal err restore img Instance %d commit env error", swImage0->meBase.instanceId);
            }
        }
        return -1;
    }

    return 0;
}

ca_omci_int32_t ca_omci_mgr_img_upg_execute_env_set(
    ca_omci_soft_image_t  *swImage0,
    ca_omci_int32_t       flag0,
    ca_omci_soft_image_t  *swImage1,
    ca_omci_int32_t       flag1)
{
    ca_omci_int32_t ret = 0;

    (void)flag0;
    (void)flag1;

    ret = CA_IMG_EXECUTE_SET(swImage0->meBase.instanceId, flag0);
    if(ret)
    {
        OMCIUPGERR("fatal err img Instance=%d execute env set err", swImage0->meBase.instanceId);
        return -1;
    }

    ret = CA_IMG_EXECUTE_SET(swImage1->meBase.instanceId, flag1);
    if(ret)
    {
        OMCIUPGERR("fatal err img Instance=%d execute env set err", swImage1->meBase.instanceId);
        return -1;
    }

    return 0;
}


void ca_omci_mng_upg_load_image_set(ca_omci_uint8_t flag)
{
    ca_omci_soft_image_t *swImg0 = NULL;
    ca_omci_soft_image_t *swImg1 = NULL;

    swImg0 = (ca_omci_soft_image_t *)ca_omci_me_entry_lookup(OMCI_ME_SOFT_IMAGE_CLASS_ID, 0x0000);
    swImg1 = (ca_omci_soft_image_t *)ca_omci_me_entry_lookup(OMCI_ME_SOFT_IMAGE_CLASS_ID, 0x0001);
    if ((NULL == swImg0) || (NULL == swImg1)) {
        return;
    }

    /*Load and execute the image that is not currently committed*/
    if (flag){
        if (0 == swImg0->attr.isCommit){
            ca_omci_mgr_img_upg_active_env_set(swImg0, 1, swImg1, 0);
            ca_omci_mgr_img_upg_execute_env_set(swImg0, 0, swImg1, 0);
            return;
        }
        if (0 == swImg1->attr.isCommit){
            ca_omci_mgr_img_upg_active_env_set(swImg1, 1, swImg0, 0);
            ca_omci_mgr_img_upg_execute_env_set(swImg1, 0, swImg0, 0);
            return;
        }
        return;
    }

    /*Load and execute the image that is currently committed*/
    if (1 == swImg0->attr.isCommit){
        ca_omci_mgr_img_upg_active_env_set(swImg0, 1, swImg1, 0);
        ca_omci_mgr_img_upg_execute_env_set(swImg0, 0, swImg1, 0);
        return;
    }

    if (1 == swImg1->attr.isCommit){
        ca_omci_mgr_img_upg_active_env_set(swImg1, 1, swImg0, 0);
        ca_omci_mgr_img_upg_execute_env_set(swImg1, 0, swImg0, 0);
        return;
    }
}


void ca_omci_mgr_img_upg_active(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  flag,
    ca_omci_uint8_t  *rsl)
{
    OMCIUPGDBG("enter");
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_soft_image_t      *otherImg = NULL;
    ca_omci_me_swimage_ctrl_t *imgCtl   = NULL;
    ca_omci_me_swimage_ctrl_t *otherCtl = NULL;
    ca_omci_uint16_t          instId    = 0;
    ca_omci_int32_t           ret       = 0;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(rsl);

    instId   = swImage->meBase.instanceId == 0 ? 1 : 0;
    otherImg = (ca_omci_soft_image_t *)ca_omci_me_entry_lookup(swImage->meBase.classId, instId);
    if (NULL == otherImg)
    {
        *rsl = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    imgCtl   = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    otherCtl = ca_omci_mng_img_ctl_get(otherImg->meBase.instanceId);
    if ((NULL == imgCtl) || (NULL == otherCtl))
    {
        OMCIUPGERR("Can't get image ctl for ME");
        *rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    ret = ca_omci_mgr_img_upg_active_env_set(swImage, 1, otherImg, 0);
    if(ret)
    {
        *rsl = OMCI_MSG_RST_PROC_ERR;
        OMCIUPGERR("img active env set failed");
        return;
    }
    ca_omci_mgr_img_upg_execute_env_set(swImage, 0, otherImg, 0);

    if (CA_OMCI_OK != ca_omci_mng_img_active(me->instanceId, flag))
    {
        *rsl = OMCI_MSG_RST_PROC_ERR;
        OMCIUPGERR("active image id %d failed", me->instanceId);
        return;
    }

    swImage->attr.isActive  = 1;
    otherImg->attr.isActive = 0;
    *rsl = OMCI_MSG_RST_SUCCESSFUL;

    return;
}

void ca_omci_mgr_img_upg_commit(
    ca_omci_mebase_t *me,
    ca_omci_uint8_t  *rsl)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_soft_image_t      *otherImg = NULL;
    ca_omci_me_swimage_ctrl_t *imgCtl   = NULL;
    ca_omci_me_swimage_ctrl_t *otherCtl = NULL;
    ca_omci_uint16_t           instId   = 0;
    ca_omci_int32_t            ret      = 0;

    OMCI_PARAM_IS_NULL(me);
    OMCI_PARAM_IS_NULL(rsl);

    instId   = swImage->meBase.instanceId == 0 ? 1 : 0;
    otherImg = (ca_omci_soft_image_t *)ca_omci_me_entry_lookup(swImage->meBase.classId, instId);
    if (NULL == otherImg)
    {
        *rsl = OMCI_MSG_RST_PROC_ERR;
        return;
    }

    imgCtl   = ca_omci_mng_img_ctl_get(swImage->meBase.instanceId);
    otherCtl = ca_omci_mng_img_ctl_get(otherImg->meBase.instanceId);
    if ((NULL == imgCtl) || (NULL == otherCtl))
    {
        OMCIUPGERR("Can't get image ctl for ME");
        *rsl = OMCI_MSG_RST_UNKNOWN_ME_INST;
        return;
    }

    ret = ca_omci_mgr_img_upg_commit_env_set(swImage, 1, otherImg, 0);
    if(ret)
    {
        *rsl = OMCI_MSG_RST_PROC_ERR;
        OMCIUPGERR("img active env set failed");
        return;
    }

    ca_omci_mgr_img_upg_execute_env_set(swImage, 0, otherImg, 0);

    swImage->attr.isCommit  = 1;
    otherImg->attr.isCommit = 0;
    *rsl = OMCI_MSG_RST_SUCCESSFUL;

    return;
}

ca_omci_int32_t ca_omci_mgr_img_upg_fsm_err_handler(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;
    (void)resp;

    OMCIUPGERR("unsupport state %d pre state %d event %d Indstance %d\r\n",
                img_upg_glb_ctx.upg_state,
                img_upg_glb_ctx.upg_pre_state,
                img_upg_glb_ctx.upg_event,
                me->instanceId);
    return 0;
}

ca_omci_int32_t ca_omci_mgr_img_upg_fsm_dflt_handler(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;
    (void)resp;

    OMCIUPGDBG("donothing state %d me Indstance %d\r\n", img_upg_glb_ctx.upg_state, me->instanceId);
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s1_e_start_dl_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl        = (ca_omci_me_st_dl_rsl_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCIUPGDBG("enter");

    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    if(rsl->result != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 0s1 event start dl 1 err");
        return -1;
    }

    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_START_DW_1;
    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S2;

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s2_e_start_dl_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl      = (ca_omci_me_st_dl_rsl_t *)resp;

    OMCIUPGDBG("enter");
    ca_omci_mng_img_close(swImage->meBase.instanceId);
    ca_omci_mng_img_clean();
    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s2_e_dl_sect_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_me_dl_sec_t *secInfo = (ca_omci_me_dl_sec_t *)data;
    ca_omci_uint8_t     *rsl     = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_dl_sec(me, secInfo, rsl);

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s2_e_dl_end_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    ca_omci_mgr_end_dl_start_upgrade(me, contents, rsl);

    if((*rsl) != OMCI_MSG_RST_DEV_BUSY)
    {
        img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
        img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_1;
        img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S1;
        ca_omci_mgr_img_upg_done_timer_del(me);
        OMCIUPGDBG("state 0s2 event dl end 1 err resp %d", *rsl);
        return 0;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S2_1;

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s2_1_e_dl_end_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)me;
    (void)data;

    OMCIUPGDBG("enter");
    ca_omci_uint8_t *rsl = (ca_omci_uint8_t *)resp;

    *rsl = OMCI_MSG_RST_DEV_BUSY;
    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s2_2_e_dl_end_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    ca_omci_mgr_end_dl(me, contents, rsl);

    if((*rsl) != OMCI_MSG_RST_SUCCESSFUL)
    {
        img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
        img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_1;
        img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S1;
        OMCIUPGDBG("state 0s2_1 event dl end 1 err resp %d", *rsl);
        return 0;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S3;

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s3_e_active_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *flag       = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_active(me, *flag, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 0s3 event active 1 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_ACTIVE_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S4;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s3_e_commit_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;

    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_commit(me, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 0s3 event commit 1 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_COMMIT_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S4;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s3_e_start_dl_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl        = (ca_omci_me_st_dl_rsl_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCIUPGDBG("enter");
    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    if(rsl->result != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGDBG("state 0s3 event start dl 1 err");
        return -1;
    }

    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_START_DW_1;
    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S2;

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s4_e_active_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *flag       = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_active(me, *flag, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 0s4 event active 0 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_ACTIVE_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S3;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_0s4_e_commit1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;

    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_commit(me, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 0s4 event commit 1 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_COMMIT_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S3;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s1_e_start_dw_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl        = (ca_omci_me_st_dl_rsl_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCIUPGDBG("enter");
    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    if(rsl->result != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s1 event start dl 0 err");
        return -1;
    }

    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_START_DW_0;
    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S2;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s2_e_start_dw_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl        = (ca_omci_me_st_dl_rsl_t *)resp;

    OMCIUPGDBG("enter");
    ca_omci_mng_img_close(swImage->meBase.instanceId);
    ca_omci_mng_img_clean();
    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s2_e_dl_sect_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_me_dl_sec_t *secInfo = (ca_omci_me_dl_sec_t *)data;
    ca_omci_uint8_t     *rsl     = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_dl_sec(me, secInfo, rsl);

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s2_e_dl_end_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    ca_omci_mgr_end_dl_start_upgrade(me, contents, rsl);

    if((*rsl) != OMCI_MSG_RST_DEV_BUSY)
    {
        img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
        img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_0;
        img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S1;
        ca_omci_mgr_img_upg_done_timer_del(me);
        OMCIUPGDBG("state 1s2 event dl end 0 err resp %d", *rsl);
        return 0;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S2_1;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s2_1_e_dl_end_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)me;
    (void)data;

    OMCIUPGDBG("enter");
    ca_omci_uint8_t *rsl = (ca_omci_uint8_t *)resp;

    (*rsl) = OMCI_MSG_RST_DEV_BUSY;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s2_2_e_dl_end_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    ca_omci_mgr_end_dl(me, contents, rsl);

    if((*rsl) != OMCI_MSG_RST_SUCCESSFUL)
    {
        img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
        img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_0;
        img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S1;
        OMCIUPGDBG("state 1s2_1 event dl end 0 err resp %d", *rsl);
        return 0;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_DL_END_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S3;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s3_e_active_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *flag       = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_active(me, *flag, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s3 event active 0 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_ACTIVE_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S4;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s3_e_commit_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;

    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_commit(me, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s3 event commit 0 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_COMMIT_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S4;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s3_e_start_dl_0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    ca_omci_soft_image_t      *swImage  = (ca_omci_soft_image_t *)me;
    ca_omci_uint8_t           *contents   = (ca_omci_uint8_t *)data;
    ca_omci_me_st_dl_rsl_t    *rsl        = (ca_omci_me_st_dl_rsl_t *)resp;
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();

    OMCIUPGDBG("enter");
    ca_omci_mgr_start_dl_img(swImage, contents, rsl);

    if(rsl->result != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s3 event start dl 0 err");
        return -1;
    }

    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_START_DW_0;
    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S2;

    OMCIUPGDBG("leave");
    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s4_e_active_1(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *flag       = (ca_omci_uint8_t *)data;
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_active(me, *flag, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s4 event active 1 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_ACTIVE_1;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_1S3;

    OMCIUPGDBG("leave");

    return 0;
}

ca_omci_int32_t ca_omci_img_upg_fsm_s_1s4_e_commit0(
    ca_omci_mebase_t *me,  void *data, void *resp)
{
    (void)data;

    OMCIUPGDBG("enter");

    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_uint8_t           *rsl        = (ca_omci_uint8_t *)resp;

    ca_omci_mgr_img_upg_commit(me, rsl);

    if(*rsl != OMCI_MSG_RST_SUCCESSFUL)
    {
        OMCIUPGERR("state 1s4 event commit 0 err");
        return -1;
    }

    img_up_ctx->upg_pre_state = img_up_ctx->upg_state;
    img_up_ctx->upg_event     = OMCI_IMG_UPG_EVENT_COMMIT_0;
    img_up_ctx->upg_state     = OMCI_IMG_UPG_STATE_0S3;

    OMCIUPGDBG("leave");

    return 0;
}

void ca_omci_mgr_img_upg_fsm_handle_init(void)
{
    ca_omci_int32_t state;
    ca_omci_int32_t event;

    for(state = 0; state < OMCI_IMG_UPG_STATE_MAX; state++)
    {
        for(event = 0; event < OMCI_IMG_UPG_EVENT_MAX; event++)
        {
            img_upg_fsm_handler[state][event] = ca_omci_mgr_img_upg_fsm_err_handler;
        }
    }

    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S1][OMCI_IMG_UPG_EVENT_START_DW_1] = ca_omci_img_upg_fsm_s_0s1_e_start_dl_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S2][OMCI_IMG_UPG_EVENT_START_DW_1] = ca_omci_img_upg_fsm_s_0s2_e_start_dl_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S2][OMCI_IMG_UPG_EVENT_DL_SECT_1]  = ca_omci_img_upg_fsm_s_0s2_e_dl_sect_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S2][OMCI_IMG_UPG_EVENT_DL_END_1]   = ca_omci_img_upg_fsm_s_0s2_e_dl_end_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S2_1][OMCI_IMG_UPG_EVENT_DL_END_1] = ca_omci_img_upg_fsm_s_0s2_1_e_dl_end_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S2_2][OMCI_IMG_UPG_EVENT_DL_END_1] = ca_omci_img_upg_fsm_s_0s2_2_e_dl_end_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S3][OMCI_IMG_UPG_EVENT_ACTIVE_1]   = ca_omci_img_upg_fsm_s_0s3_e_active_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S3][OMCI_IMG_UPG_EVENT_COMMIT_1]   = ca_omci_img_upg_fsm_s_0s3_e_commit_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S3][OMCI_IMG_UPG_EVENT_START_DW_1] = ca_omci_img_upg_fsm_s_0s3_e_start_dl_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S4][OMCI_IMG_UPG_EVENT_ACTIVE_0]   = ca_omci_img_upg_fsm_s_0s4_e_active_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_0S4][OMCI_IMG_UPG_EVENT_COMMIT_1]   = ca_omci_img_upg_fsm_s_0s4_e_commit1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S1][OMCI_IMG_UPG_EVENT_START_DW_0] = ca_omci_img_upg_fsm_s_1s1_e_start_dw_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S2][OMCI_IMG_UPG_EVENT_START_DW_0] = ca_omci_img_upg_fsm_s_1s2_e_start_dw_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S2][OMCI_IMG_UPG_EVENT_DL_SECT_0]  = ca_omci_img_upg_fsm_s_1s2_e_dl_sect_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S2][OMCI_IMG_UPG_EVENT_DL_END_0]   = ca_omci_img_upg_fsm_s_1s2_e_dl_end_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S2_1][OMCI_IMG_UPG_EVENT_DL_END_0] = ca_omci_img_upg_fsm_s_1s2_1_e_dl_end_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S2_2][OMCI_IMG_UPG_EVENT_DL_END_0] = ca_omci_img_upg_fsm_s_1s2_2_e_dl_end_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S3][OMCI_IMG_UPG_EVENT_ACTIVE_0]   = ca_omci_img_upg_fsm_s_1s3_e_active_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S3][OMCI_IMG_UPG_EVENT_COMMIT_0]   = ca_omci_img_upg_fsm_s_1s3_e_commit_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S3][OMCI_IMG_UPG_EVENT_START_DW_0] = ca_omci_img_upg_fsm_s_1s3_e_start_dl_0;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S4][OMCI_IMG_UPG_EVENT_ACTIVE_1]   = ca_omci_img_upg_fsm_s_1s4_e_active_1;
    img_upg_fsm_handler[OMCI_IMG_UPG_STATE_1S4][OMCI_IMG_UPG_EVENT_COMMIT_0]   = ca_omci_img_upg_fsm_s_1s4_e_commit0;

}

void ca_omci_mng_sw_img_init(void)
{
    ca_omci_me_swimage_ctrl_t *imgCtl = NULL;
    ca_omci_uint32_t          idx     = 0;

    OMCI_MEMSET(sg_imageCtl, 0, (sizeof(ca_omci_me_swimage_ctrl_t) * OMCI_IMG_NUM));

    for(idx = 0; idx < OMCI_IMG_NUM; idx++)
    {
        imgCtl   = ca_omci_mng_img_ctl_get(idx);
        ca_omci_mng_img_clt_win_reset(imgCtl);
        imgCtl->file    = -1;
        imgCtl->timerId = CA_INVALID_TIMER;
    }

    ca_omci_mgr_img_upg_fsm_handle_init();
    img_upg_glb_ctx.upg_event     = OMCI_IMG_UPG_EVENT_REBOOT;
    img_upg_glb_ctx.upg_pre_state = OMCI_IMG_UPG_STATE_S0;
    img_upg_glb_ctx.upg_state     = OMCI_IMG_UPG_STATE_S0;

    return;
}

void ca_omci_mgr_img_upg_show(const struct shell *cli)
{
    ca_omci_mgr_img_upg_ctx_t *img_up_ctx = ca_omci_mgr_img_upg_glb_ctx_get();
    ca_omci_me_swimage_ctrl_t *imgCtl     = NULL;
    omci_sprint(cli, "========================img upg info=================\r\n");

    omci_sprint(cli, "pre upg state :%s\r\n", ca_omic_mgr_img_upg_state_str[img_up_ctx->upg_pre_state]);
    omci_sprint(cli, "    upg state :%s\r\n", ca_omic_mgr_img_upg_state_str[img_up_ctx->upg_state]);
    omci_sprint(cli, "    upg state :%s\r\n", ca_omic_mgr_img_upg_event_str[img_up_ctx->upg_event]);
    imgCtl = ca_omci_mng_img_ctl_get(0);
    omci_sprint(cli, "SW image0 imgSize    :%d\r\n", imgCtl->imgSize);
    omci_sprint(cli, "SW image0 commitSize :%d\r\n", imgCtl->commitLen);
    omci_sprint(cli, "SW image0 crc32      :%u\r\n", imgCtl->crc);
    if(imgCtl->imgSize)
    {
        omci_sprint(cli, "SW image0 DL progress :%.0f%\r\n", ((float)(imgCtl->commitLen)/(imgCtl->imgSize)*100));
    }

    imgCtl = ca_omci_mng_img_ctl_get(1);
    omci_sprint(cli, "SW image1 imgSize    :%d\r\n", imgCtl->imgSize);
    omci_sprint(cli, "SW image1 commitSize :%d\r\n", imgCtl->commitLen);
    omci_sprint(cli, "SW image1 crc32      :%u\r\n", imgCtl->crc);
    if(imgCtl->imgSize)
    {
        omci_sprint(cli, "SW image1 DL progress :%.0f%\r\n", ((float)(imgCtl->commitLen)/(imgCtl->imgSize)*100));
    }

    omci_sprint(cli, "=====================================================\r\n");

    return;
}

