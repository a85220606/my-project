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
#ifndef __OMCI_MNG_UPGRADE_H__
#define __OMCI_MNG_UPGRADE_H__


#define OMCI_MSG_IMG_DATA_LEN    31
#define OMCI_IMG_NUM             2
#define OMCI_IMG_FILE            "/lfs1/upgrade.img"
#define OMCI_IMG_VERSION_LEN     14
#define OMCI_IMG_MAX_WIN_SIZE    254

#ifndef __X86_DEMO__
#if !defined(CONFIG_CA_OMCI_FPGA)
#define CA_OMCI_USE_PLAT_API
#endif
#endif

#ifdef CA_OMCI_USE_PLAT_API
#define OMCI_UPGRADE_SUPPORT     1
#define OMCI_IMG_MAX_BUF_LEN     7905//baseline buf len is 31 * 255; extern buf len is 1965 * 255

#define CA_IMG_ACTIVE_SET(img_idx, flag) \
        ca_img_active_set(img_idx, flag)

#define CA_IMG_ACTIVE_GET(img_idx, pFlag) \
        ca_img_active_get(img_idx, pFlag)

#define CA_IMG_COMMIT_SET(img_idx, flag) \
        ca_img_committed_set(img_idx, flag)

#define CA_IMG_COMMIT_GET(img_idx, pFlag) \
        ca_img_committed_get(img_idx, pFlag)

#define CA_IMG_EXECUTE_SET(img_idx, flag) \
        ca_img_executed_set(img_idx, flag)

#define CA_IMG_VALID_SET(img_idx, flag) \
        ca_img_valid_set(img_idx, flag)

#define CA_IMG_VALID_GET(img_idx, pFlag) \
        ca_img_valid_get(img_idx, pFlag)

#define CA_IMG_VERSION_GET(img_idx, pInfo)\
        ca_img_version_get(img_idx, pInfo)

#define CA_IMG_VERSION_SET(img_idx, pInfo)\
        ca_img_version_set(img_idx, pInfo)
#else
#define OMCI_UPGRADE_SUPPORT     0
#define OMCI_IMG_MAX_BUF_LEN     0
#define CA_IMG_ACTIVE_SET(img_idx, flag)   0
#define CA_IMG_ACTIVE_GET(img_idx, pFlag)  0
#define CA_IMG_COMMIT_SET(img_idx, flag)   0
#define CA_IMG_COMMIT_GET(img_idx, pFlag)  0
#define CA_IMG_EXECUTE_SET(img_idx, flag)  0
#define CA_IMG_VALID_SET(img_idx, flag)    0
#define CA_IMG_VALID_GET(img_idx, pFlag)   0
#define CA_IMG_VERSION_GET(img_idx, pInfo) 0
#endif

#define CA_OMCI_IMG_GET_ACTICE(x) \
        ((x)->attr.isActive)

#define CA_OMCI_IMG_GET_COMMIT(x) \
        ((x)->attr.isCommit)

#define CA_OMCI_IMG_GET_VALID(x) \
        ((x)->attr.isValid)

typedef enum
{
    OMCI_IMG_WRITE_STATE_SUCCESS = 0,
    OMCI_IMG_WRITE_STATE_WRITING,
    OMCI_IMG_WRITE_STATE_FAIL,
}ca_omci_img_write_st_e;

typedef enum{
    OMCI_IMG_UPG_STATE_S0,
    OMCI_IMG_UPG_STATE_0S1,
    OMCI_IMG_UPG_STATE_0S2,
    OMCI_IMG_UPG_STATE_0S2_1,//start upg
    OMCI_IMG_UPG_STATE_0S2_2,//upg done
    OMCI_IMG_UPG_STATE_0S3,
    OMCI_IMG_UPG_STATE_0S4,
    OMCI_IMG_UPG_STATE_1S1,
    OMCI_IMG_UPG_STATE_1S2,
    OMCI_IMG_UPG_STATE_1S2_1,//start upg
    OMCI_IMG_UPG_STATE_1S2_2,//upg done
    OMCI_IMG_UPG_STATE_1S3,
    OMCI_IMG_UPG_STATE_1S4,
    OMCI_IMG_UPG_STATE_MAX,
}ca_omci_img_upg_state_e;

typedef enum{
    OMCI_IMG_UPG_EVENT_REBOOT,
    OMCI_IMG_UPG_EVENT_START_DW_0,
    OMCI_IMG_UPG_EVENT_START_DW_1,
    OMCI_IMG_UPG_EVENT_DL_SECT_0,
    OMCI_IMG_UPG_EVENT_DL_SECT_1,
    OMCI_IMG_UPG_EVENT_DL_END_0,
    OMCI_IMG_UPG_EVENT_DL_END_1,
    OMCI_IMG_UPG_EVENT_ACTIVE_0,
    OMCI_IMG_UPG_EVENT_ACTIVE_1,
    OMCI_IMG_UPG_EVENT_COMMIT_0,
    OMCI_IMG_UPG_EVENT_COMMIT_1,
    OMCI_IMG_UPG_EVENT_MAX,
}ca_omci_img_upg_event_e;

typedef ca_omci_int32_t (*ca_omci_img_upg_fsm_handler_t) (ca_omci_mebase_t *, void *, void *);

typedef struct
{
    ca_omci_uint8_t             windowSize;
    ca_omci_uint8_t             sectionNum;
    ca_omci_uint8_t             error;
    ca_omci_uint32_t            crc;
    ca_omci_uint8_t             buf[OMCI_IMG_MAX_BUF_LEN];
    ca_omci_uint32_t            bufLen;
    ca_omci_uint32_t            writeLen;
}ca_omci_swimage_window_t;


typedef struct
{
    ca_omci_uint32_t         imgSize;
    ca_omci_uint32_t         crc;
    ca_omci_uint32_t         pre_commitLen;
    ca_omci_uint32_t         commitLen;
    ca_omci_swimage_window_t win;
    ca_omci_int32_t          file;
    ca_uint32                timerId;
}ca_omci_me_swimage_ctrl_t;

typedef struct
{
    ca_omci_int32_t upg_state;
    ca_omci_int32_t upg_pre_state;
    ca_omci_int32_t upg_event;
}ca_omci_mgr_img_upg_ctx_t;

void ca_omci_mng_st_dl_process(ca_omci_mebase_t *me, ca_omci_uint8_t *contents, ca_omci_me_st_dl_rsl_t *rsl);
void ca_omci_mng_dl_sec_process(ca_omci_mebase_t *me, ca_omci_me_dl_sec_t *secInfo, ca_omci_uint8_t *rsl);
void ca_omci_mng_end_dl_process(ca_omci_mebase_t *me, ca_omci_uint8_t *contents, ca_omci_uint8_t *rsl);
void ca_omci_mng_img_active_process(ca_omci_mebase_t * me,ca_omci_uint8_t flag,ca_omci_uint8_t * rsl);
void ca_omci_mng_img_commit_process(ca_omci_mebase_t * me,ca_omci_uint8_t * rsl);
void ca_omci_mng_sw_img_init(void);
void ca_omci_mng_me_sw_ver_upg(void *pData);
#endif /* __OMCI_MNG_UPGRADE_H__ */

