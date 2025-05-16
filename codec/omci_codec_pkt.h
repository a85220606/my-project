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
#ifndef __OMCI_CODEC_PKT_H__
#define __OMCI_CODEC_PKT_H__


#define OMCI_DB_FLAG(a)                     (a & 0x80)
#define OMCI_AR_FLAG(a)                     (a & 0x40)
#define OMCI_AK_FLAG(a)                     (a & 0x20)
#define OMCI_AR_BIT                         (1 << 6)
#define OMCI_DEF_INSTANCE_ID                0
#define OMCI_BASE_DEVICE_ID                 0x0a
#define OMCI_EXT_DEVICE_ID                  0x0b
#define OMCI_BASE_PKT_LEN                   48
#define OMCI_BASE_PKT_TAILER_OFFSET         42
#define OMCI_PKT_MIC_LEN                    4
#define OMCI_BASE_PKT_GEN_LEN               (OMCI_BASE_PKT_LEN - OMCI_PKT_MIC_LEN)
#define OMCI_BASE_PKT_CONT_LEN              32
#define OMCI_BASE_PKT_RSVD_LEN              2
#define OMCI_BASE_PKT_SDU_LEN               0x28
#define OMCI_EXT_PKT_MAX_LEN                1980
#define OMCI_PKT_MAX_LEN                    OMCI_EXT_PKT_MAX_LEN //support both ext and baseline, so use the big one
#define OMCI_SAVED_PKT_NUM                  2
#define OMCI_CREATE_RSP_MSG_LEN             3
#define OMCI_DELETE_RSP_MSG_LEN             1
#define OMCI_SET_RSP_MSG_LEN                5
#define OMCI_GET_RSP_MSG_BASE_LEN           3
#define OMCI_MIB_UPLOAD_RSP_MSG_BASE_LEN    2
#define OMCI_GET_ALL_AM_RSP_MSG_BASE_LEN    2
#define OMCI_ACTION_RSP_MSG_BASE_LEN        1
#define OMCI_EXT_RSP_SINGLE_ATTR_HDR_LEN    8
#define OMCI_MAX_GET_LEN                    25
#define OMCI_BASE_MIB_UPLOAD_NEXT_LEN_MAX   26
#define OMCI_MAX_ATTR_LEN                   28
#define OMCI_DELETE_CONT_LEN                0
#define OMCI_GET_CONT_LEN                   2
#define OMCI_ACTION_CONT_LEN                0
#define OMCI_MIB_UPLOAD_NEXT_CONT_LEN       2
#define OMCI_ALARM_RSP_CONT_LEN             29
#define OMCI_ALARM_ZERO_PADDING_LEN         3
#define OMCI_GET_ALL_AM_NEXT_CONT_LEN       2
#define OMCI_REBOOT_CONT_LEN                1
#define OMCI_PARALLEL_DL_SUPPORT            0xffff
#define OMCI_ST_DL_CONT_LEN                 2 //for now, not support parallel
#define OMCI_DL_SEC_CONT_LEN                2
#define OMCI_END_DL_CONT_LEN                1 //for now, not support parallel
#define OMCI_MSG_GET_NEXT_LEN               29
#define OMCI_MSG_EXT_GET_NEXT_LEN           1963
#define OMCI_GET_NEXT_RSP_MSG_BASE_LEN      3
#define OMCI_GET_NEXT_CONT_LEN              4
#define OMCI_SYNC_TIME_15_TICK_OK           0
#define OMCI_SYNC_TIME_DATE_15_TICK_OK      1
#define OMCI_SYNC_TIME_RSP_MSG_LEN          2
#define OMCI_SYNC_TIME_CONT_LEN             7
#define OMCI_TEST_CONT_LEN                  5
#define OMCI_TEST_SELF_TEST                 0xf
#define OMCI_TEST_RSL_CONT_LEN              17
#define OMCI_TEST_RSL_VOLTAGE               1
#define OMCI_TEST_RSL_RX_POWER              3
#define OMCI_TEST_RSL_MEAN_LAUNCH_POWER     5
#define OMCI_TEST_RSL_LASER_BIAS_CURRENT    9
#define OMCI_TEST_RSL_TEMPERATURE           12
#define OMCI_EXT_PKT_MAX_CONT_LEN           1966


#define OMCI_MSG_RST_SUCCESSFUL         0
#define OMCI_MSG_RST_PROC_ERR           1
#define OMCI_MSG_RST_NOT_SUPPORT        2
#define OMCI_MSG_RST_PARAM_ERR          3
#define OMCI_MSG_RST_UNKNOWN_ME         4
#define OMCI_MSG_RST_UNKNOWN_ME_INST    5
#define OMCI_MSG_RST_DEV_BUSY           6
#define OMCI_MSG_RST_INST_EXIST         7
#define OMCI_MSG_RST_ATTR_FAIL          9


#define OMCI_ME_ATTR1   1
#define OMCI_ME_ATTR2   2
#define OMCI_ME_ATTR3   3
#define OMCI_ME_ATTR4   4
#define OMCI_ME_ATTR5   5
#define OMCI_ME_ATTR6   6
#define OMCI_ME_ATTR7   7
#define OMCI_ME_ATTR8   8
#define OMCI_ME_ATTR9   9
#define OMCI_ME_ATTR10  10
#define OMCI_ME_ATTR11  11
#define OMCI_ME_ATTR12  12
#define OMCI_ME_ATTR13  13
#define OMCI_ME_ATTR14  14
#define OMCI_ME_ATTR15  15
#define OMCI_ME_ATTR16  16


typedef enum
{
    OMCI_MSG_TYPE_CREATE            = 4,
    OMCI_MSG_TYPE_DELETE            = 6,
    OMCI_MSG_TYPE_SET               = 8,
    OMCI_MSG_TYPE_GET               = 9,
    OMCI_MSG_TYPE_GET_ALL_ALM       = 11,
    OMCI_MSG_TYPE_GET_ALL_ALM_NEXT  = 12,
    OMCI_MSG_TYPE_MIB_UPLOAD        = 13,
    OMCI_MSG_TYPE_MIB_UPLOAD_NEXT   = 14,
    OMCI_MSG_TYPE_MIB_RESET         = 15,
    OMCI_MSG_TYPE_ALARM             = 16,
    OMCI_MSG_TYPE_ATTR_VALUE_CHANGE = 17,
    OMCI_MSG_TYPE_TEST              = 18,
    OMCI_MSG_TYPE_START_SW_DOWNLOAD = 19,
    OMCI_MSG_TYPE_DOWNLOAD_SEC      = 20,
    OMCI_MSG_TYPE_END_SW_DOWNLOAD   = 21,
    OMCI_MSG_TYPE_ACTIVATE_SW       = 22,
    OMCI_MSG_TYPE_COMMIT_SW         = 23,
    OMCI_MSG_TYPE_SYNCH_TIME        = 24,
    OMCI_MSG_TYPE_REBOOT            = 25,
    OMCI_MSG_TYPE_GET_NEXT          = 26,
    OMCI_MSG_TYPE_TEST_RESULT       = 27,
    OMCI_MSG_TYPE_GET_CURR_DATA     = 28,
    OMCI_MSG_TYPE_SET_TABLE         = 29,
    OMCI_MSG_TYPE_MAX,
}ca_omci_msg_type_e;


/* TO support both baseline msg and extern msg, msg header need to define */
typedef struct
{
    ca_omci_uint16_t     transId;
    ca_omci_uint8_t      msgType;
    ca_omci_uint8_t      devId;
    ca_omci_uint16_t     classId;
    ca_omci_uint16_t     instId;
}ca_omci_pkt_hdr_t;


typedef struct
{
    ca_omci_pkt_hdr_t   hdr;
    ca_omci_uint8_t     pdu[OMCI_BASE_PKT_CONT_LEN];
    ca_omci_uint8_t     rsvd[OMCI_BASE_PKT_RSVD_LEN];
    ca_omci_uint16_t    sduLen;
    ca_omci_uint32_t    crc;
}ca_omci_base_pkt_t;


typedef struct
{
    ca_omci_pkt_hdr_t   hdr;
    ca_omci_uint16_t    len;
}ca_omci_ext_pkt_hdr_t;


typedef struct
{
    void             *pAttr;
    ca_omci_uint16_t  attrLen;
}ca_omci_avc_attr_t;


typedef struct
{
    ca_omci_uint8_t     attrNum;
    ca_omci_avc_attr_t  attr[OMCI_ME_ATTR16];
}ca_omci_pkt_avc_t;


typedef struct
{
    ca_omci_uint16_t classId;
    ca_omci_uint8_t (*func)(ca_omci_uint8_t *);
}ca_omci_decode_test_cb_t;


typedef struct
{
    ca_omci_uint32_t msgType;
    void (*func)(ca_omci_uint8_t *);
}ca_omci_codec_decode_cb_t;


ca_omci_st_e ca_omci_pkt_tx(ca_omci_uint8_t *buffer, ca_omci_bool_t ext);
ca_omci_st_e ca_omci_pkt_tx_for_cli(ca_omci_uint8_t *buffer, ca_omci_bool_t ext);
ca_omci_uint8_t *ca_omci_pkt_tx_hdr_build(ca_omci_uint8_t *inPdu);
void omci_codec_encode_create_rsp(ca_omci_uint8_t *pkt, ca_omci_me_create_result_t *rsl);
void omci_codec_encode_delete_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl);
void omci_codec_encode_set_rsp(ca_omci_uint8_t *pkt, ca_omci_me_set_result_t *rsl);
void omci_codec_encode_get_rsp(ca_omci_uint8_t *outPdu, ca_omci_me_get_result_t *rsl);
void omci_codec_encode_mib_upload_rsp(ca_omci_uint8_t *pkt, ca_omci_uint16_t seqNum);
void omci_codec_encode_get_all_alarms_rsp(ca_omci_uint8_t *pkt, ca_omci_uint16_t seqNum);
void omci_codec_encode_action_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl);
void omci_codec_encode_start_sw_dl_rsp(ca_omci_uint8_t *pkt, ca_omci_me_st_dl_rsl_t *rsl);
void omci_codec_encode_dl_sec_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl, ca_omci_me_dl_sec_t *sec);
void omci_codec_encode_end_sw_dl_rsp(ca_omci_uint8_t *pkt, ca_omci_uint8_t rsl);
void omci_codec_encode_get_next_rsp(ca_omci_uint8_t *outPdu, ca_omci_me_getnext_result_t *rsl);
void omci_codec_encode_get_curr_rsp(ca_omci_uint8_t *outPdu, ca_omci_me_get_result_t *rsl);
void omci_codec_encode_sync_time_rsp(ca_omci_uint8_t *pkt, ca_omci_me_sync_time_rsl_t *rsl);
void omci_codec_encode_dg_alarm(ca_omci_mebase_t *me);
void omci_codec_encode_alarm(ca_omci_mebase_t *me);
void omci_codec_encode_avc(ca_omci_mebase_t * me,ca_omci_uint16_t mask,ca_omci_pkt_avc_t * pAvcAttr);
void omci_codec_encode_test_rsl(ca_omci_mebase_t *me, void *pData);
ca_omci_st_e ca_omci_decode_pkt(ca_omci_uint8_t *pkt);
void ca_omci_pkg_info_rx_debug_set(ca_omci_bool_t admin);
void ca_omci_pkg_info_tx_debug_set(ca_omci_bool_t admin);
void ca_omci_pkg_hex_rx_debug_set(ca_omci_bool_t admin);
void ca_omci_pkg_hex_tx_debug_set(ca_omci_bool_t admin);
#endif /* __OMCI_CODEC_PKT_H__ */

