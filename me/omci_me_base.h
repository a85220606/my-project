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
#ifndef __OMCI_ME_BASE_H__
#define __OMCI_ME_BASE_H__
#include "omci_hash.h"


#define OMCI_INSTANCE_MAX           32
#define OMCI_STD_ALARM_BMP_LEN      26
#define OMCI_ORG_ALARM_BMP_LEN      2
#define OMCI_ALARM_BMP_LEN          28
#define OMCI_ALARM_ENTRY_MAX        64
#define OMCI_MAX_ATTR_NUM           16
#define OMCI_ME_NULL_POINTER        0xFFFF
#define OMCI_ME_MAX_ALARM_ID        224
#define OMCI_ME_MIN_ORG_ALARM_ID    208
#define OMCI_ME_ALARM_BIT_LEN       8
#define OMCI_ME_ALARM_STD_BIT_MAX   16


typedef struct
{
    ca_omci_uint8_t     result;
    ca_omci_uint16_t    exeMask;
}ca_omci_me_create_result_t;

typedef struct
{
    ca_omci_uint8_t     result;
    ca_omci_uint16_t    optMask;
    ca_omci_uint16_t    exeMask;
}ca_omci_me_set_result_t;

typedef struct
{
    ca_omci_uint8_t     result;
    ca_omci_uint16_t    mask;
    ca_omci_uint16_t    optMask;
    ca_omci_uint16_t    exeMask;
    ca_omci_uint16_t    attrLen;
}ca_omci_me_get_result_t;

typedef struct
{
    ca_omci_uint16_t classId;
    ca_omci_uint16_t instanceId;
    ca_omci_uint8_t  alarmBmp[OMCI_ALARM_BMP_LEN];
}ca_omci_me_get_alarm_entry_t;

typedef struct
{
    ca_omci_uint16_t              entryNum;
    ca_omci_me_get_alarm_entry_t  entry[OMCI_ALARM_ENTRY_MAX];
}ca_omci_me_get_alarm_next_t;


typedef struct
{
    ca_omci_uint8_t  result;
    ca_omci_uint8_t  rslInfo;
}ca_omci_me_sync_time_rsl_t;


typedef struct
{
    ca_omci_uint8_t  result;
    ca_omci_uint16_t attrLen;
}ca_omci_me_getnext_result_t;

typedef struct
{
    ca_omci_uint8_t     result;
    ca_omci_uint8_t     windowSize;
}ca_omci_me_st_dl_rsl_t;

typedef struct
{
    ca_omci_uint8_t     isLast;
    ca_omci_uint8_t     sectionNum;
    ca_omci_uint16_t    dataLen;
    ca_omci_uint8_t    *data;
}ca_omci_me_dl_sec_t;


//bit 16 to 207 Reserved 
typedef struct
{
    ca_omci_uint8_t  stdAmBmp[OMCI_STD_ALARM_BMP_LEN]; //for standard use 2 bytes
    ca_omci_uint8_t  orgAmBmp[OMCI_ORG_ALARM_BMP_LEN]; //for Vendor-specific use 2 bytes
}ca_omci_alarm_bmp_t;


typedef struct ca_omci_me_link
{
    ca_omci_uint16_t   classId;
    ca_omci_uint16_t   instanceId;
    struct ca_omci_me_link *next;
}ca_omci_me_link_t;


typedef struct ca_omci_mebase
{
    ca_omci_uint16_t           transId;
    ca_omci_uint16_t           classId;
    ca_omci_uint16_t           instanceId;
    ca_omci_uint16_t           mibUploadCnt;
    ca_omci_alarm_bmp_t       *amBmp;
    ca_omci_alarm_bmp_t       *arcBmp; /*refer g988 A.1.4.3*/
    struct ca_omci_me_handler *handlers;
    ca_omci_me_link_t         *linkedMeHeader;
    ca_omci_uint32_t           linkedCount;
    struct ca_omci_mebase     *next;
}ca_omci_mebase_t;


typedef ca_omci_st_e (ca_omci_create_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t *attr, ca_omci_me_create_result_t *result);
typedef ca_omci_st_e (ca_omci_set_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint16_t inMask, ca_omci_uint8_t *attr, ca_omci_me_set_result_t *result);
typedef ca_omci_st_e (ca_omci_get_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t ext, ca_omci_uint16_t inMask, ca_omci_uint8_t *attr, ca_omci_me_get_result_t *result);
typedef ca_omci_st_e (ca_omci_get_alarm_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t mode, ca_omci_uint8_t ext, ca_omci_uint16_t *nextCnt);
typedef ca_omci_st_e (ca_omci_action_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t *result);
typedef ca_omci_st_e (ca_omci_get_alarm_next_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint16_t seqNum, ca_omci_uint8_t ext, ca_omci_me_get_alarm_next_t *alarmEntry);
typedef ca_omci_st_e (ca_omci_test_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t ext, ca_omci_uint8_t *attr, ca_omci_uint8_t *result);
typedef ca_omci_st_e (ca_omci_sync_time_proc_cb)(ca_omci_mebase_t *me, ca_omci_bool_t flag);
typedef ca_omci_st_e (ca_omci_start_download_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t windowSize, ca_omci_uint32_t  imageSize, ca_omci_me_st_dl_rsl_t *result);
typedef ca_omci_st_e (ca_omci_download_proc_cb)(ca_omci_mebase_t *me, ca_omci_me_dl_sec_t *section, ca_omci_uint8_t *result);
typedef ca_omci_st_e (ca_omci_end_download_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint32_t crc, ca_omci_uint32_t imgSize, ca_omci_uint8_t *result);
typedef ca_omci_st_e (ca_omci_getnext_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t ext, ca_omci_uint16_t seqNum, ca_omci_uint16_t mask, ca_omci_uint8_t *attr, ca_omci_me_getnext_result_t *result);
typedef ca_omci_st_e (ca_omci_table_set_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint16_t inMask, ca_omci_uint16_t attrLen, ca_omci_uint8_t *attr, ca_omci_uint8_t *result);
typedef ca_omci_st_e (ca_omci_upload_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t ext, ca_omci_uint8_t *contents, ca_omci_uint16_t *contLen,ca_omci_uint16_t seqNum);
typedef ca_omci_st_e (ca_omci_alarm_report_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint8_t action, ca_omci_uint32_t id);
typedef ca_omci_st_e (ca_omci_avc_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint16_t attrId, void *data);
typedef ca_omci_st_e (ca_omci_mib_save_proc_cb)(ca_omci_mebase_t *me, void *pData);
typedef ca_omci_st_e (ca_omci_mib_restore_proc_cb)(ca_omci_mebase_t *me, ca_omci_uint32_t *len, void *pData);
typedef void (ca_omci_me_display_proc_cb)(ca_omci_mebase_t *me, void *data);


/*G.988 standard operation*/
typedef struct ca_omci_me_handler
{
    ca_omci_create_proc_cb           *createProc;
    ca_omci_set_proc_cb              *setProc;
    ca_omci_get_proc_cb              *getProc;
    ca_omci_action_proc_cb           *deleteProc;
    ca_omci_test_proc_cb             *testProc;
    ca_omci_sync_time_proc_cb        *syncTimeProc;
    ca_omci_getnext_proc_cb          *getNextProc;
    ca_omci_get_proc_cb              *getCurProc;
    ca_omci_table_set_proc_cb        *tableSetProc;
    ca_omci_upload_proc_cb           *uploadProc;
    ca_omci_me_display_proc_cb       *meDisplay;
    ca_omci_mib_save_proc_cb         *mibSave;
    ca_omci_mib_restore_proc_cb      *mibRestore;
} ca_omci_me_handler_t;


typedef void (ca_omci_me_init_cb)(void);
typedef ca_omci_mebase_t *(ca_omci_me_new_proc_cb)(void);


typedef struct
{
    ca_omci_uint16_t        classId;
    ca_omci_uint16_t        extMibUploadFlag; //default is 0, set it when auto-create
    ca_omci_int8_t         *desc;
    ca_omci_me_init_cb     *initCb;
    ca_omci_me_new_proc_cb *newProcCb;
    HASH_TABLE             *instHash;
}ca_omci_meclass_t;


typedef struct
{
    ca_omci_uint16_t  attrIdx;
    ca_omci_int8_t   *name;
}ca_omci_me_attr_t;


typedef struct
{
    ca_omci_uint16_t   classId;
    ca_omci_me_attr_t *ptAttr;
}ca_omci_me_dscpt_t;


void            ca_omci_base_init(ca_omci_mebase_t *me, ca_omci_uint16_t classId, ca_omci_me_handler_t *handlers);
ca_omci_st_e    ca_omci_linked_me_add(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe);
ca_omci_st_e    ca_omci_linked_me_delete(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe);
ca_omci_st_e    ca_omci_linked_me_clear(ca_omci_mebase_t *pMe);
ca_omci_st_e    ca_omci_me_relation_switch(ca_omci_mebase_t *me, ca_omci_uint16_t classId, ca_omci_uint16_t newInst, ca_omci_uint16_t oldInst);
ca_omci_bool_t  ca_omci_me_linked(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe);
ca_omci_bool_t  ca_omci_me_null_pointer(ca_omci_uint16_t instanceId);
ca_omci_uint8_t ca_omci_linked_me_count_get(ca_omci_mebase_t *pMe);


#endif /* __OMCI_ME_BASE_H__ */

