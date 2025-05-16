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

Copyright (c) 2023 by CA Incorporated
****************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <ctype.h>
#include "omci_mng_zephyr_cmd.h"
#include "osal_common.h"
#include "osal_file.h"
#include "omci_types.h"
#include "omci_common.h"
#include "omci_cfg_file.h"
#include "omci_mng_onuinfo.h"


extern const ca_omci_int8_t *g_omciLogLevelStr[];


static ca_omci_cfg_obj_t sg_omciDefCfgObj[OMCI_CFG_OBJ_MAX_NUM] =
{
    {"olt_type",       "ALCL"},
    {"psk",            "00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:00"},
    {"omcc_ver",       "B2"},
    {"pon_port_num",   "1"},
    {"pots_port_num",  "0"},
    {"battery_backup", "0"},
    {"ipv6_flag",      "0"},
    {"iphost_num",     "0"},
    {"veip_num",       "0"},
    {"eth_num",        "1"},
    {"pon_slot",       "1"},
    {"eth_slot",       "3"},
    {"iphost_slot",    "0"},
    {"pots_slot",      "0"},
    {"veip_slot",      "4"},
    {"eth_map",        "06"},
    {"remote_debug",   "0"},
    {"power_csv_mode", "1"},
    {"veip_independ",  "0"},
    {"gpon_ver",       "V1.0"},
    {"eqid",           "Neptune"},
    {"isp_type",       "COMMON"},
    {"loid",           "CORT12345678"},
    {"passwd",         "CORT1234567"},
    {"tod_rsp_time",   "35000"},
    {"log_level",      "error"},
    {"fec_mode",       "rs"},
};


void ca_omci_cfg_file_support_display(const struct shell *cli)
{
    ca_omci_uint8_t idx = 0;

    omci_sprint(cli, "OMCI cfg file support value name:\r\n");
    for (idx = 0; idx < OMCI_CFG_OBJ_MAX_NUM; idx++) {
        omci_sprint(cli, "    %s\r\n", sg_omciDefCfgObj[idx].name);
    }
}


void ca_omci_cfg_file_get_value(const struct shell *cli, char *pData)
{
    ca_omci_uint8_t idx = 0;

    for (idx = 0; idx < OMCI_CFG_OBJ_MAX_NUM; idx++) {
        if (0 == strcmp(pData, sg_omciDefCfgObj[idx].name)) {
            omci_sprint(cli, "%s = \"%s\"\r\n", sg_omciDefCfgObj[idx].name, sg_omciDefCfgObj[idx].value);
            break;
        }
    }

    if (OMCI_CFG_OBJ_MAX_NUM == idx) {
        return;
    }
}


void ca_omci_cfg_file_set_value(const struct shell *cli, char *pData, char *pVal)
{
    ca_omci_uint8_t idx = 0;
    ca_omci_int32_t len = 0;

    len = strlen(pVal);
    if (OMCI_CFG_MAX_LINE <= len) {
        omci_sprint(cli, "Value[%s] len %d is too large \r\n", pVal, len);
        return;
    }

    if (0 == strcmp(pData, sg_omciDefCfgObj[OMCI_CFG_OLT_TYPE].name)) {
        if ((0 != OMCI_STRCMP((const char *)pVal, "CA"))
         && (0 != OMCI_STRCMP((const char *)pVal, "ALCL"))
         && (0 != OMCI_STRCMP((const char *)pVal, "SIM"))) {
            omci_sprint(cli, "Unsupport OLT type %s\r\n", pVal);
            omci_sprint(cli, "    OLT types are: CA/ALCL/SIM\r\n", pVal);
            return;
        }
    }

    if (0 == strcmp(pData, sg_omciDefCfgObj[OMCI_CFG_ISP_TYPE].name)) {
        if ((0 != OMCI_STRCMP((const char *)pVal, "COMMON"))
         && (0 != OMCI_STRCMP((const char *)pVal, "VERIZON"))
         && (0 != OMCI_STRCMP((const char *)pVal, "ORANGE"))
         && (0 != OMCI_STRCMP((const char *)pVal, "ATNT"))) {
            omci_sprint(cli, "Unsupport ISP type %s\r\n", pVal);
            omci_sprint(cli, "    ISP types are: COMMON/VERIZON/ORANGE/ATNT\r\n", pVal);
            return;
        }
    }

    if (0 == strcmp(pData, sg_omciDefCfgObj[OMCI_CFG_FEC_MODE].name)) {
        if ((0 != OMCI_STRCMP((const char *)pVal, "rs"))
         && (0 != OMCI_STRCMP((const char *)pVal, "base_r"))
         && (0 != OMCI_STRCMP((const char *)pVal, "disable"))) {
            omci_sprint(cli, "Unsupport FEC mode %s\r\n", pVal);
            omci_sprint(cli, "    FEC mode support rs/base_r/disable\r\n");
            return;
        }
    }

    for (idx = 0; idx < OMCI_CFG_OBJ_MAX_NUM; idx++) {
        if (0 == strcmp(pData, sg_omciDefCfgObj[idx].name)) {
            ca_omci_set_config_var(sg_omciDefCfgObj[idx].name, pVal, len);
            omci_sprint(cli, "Update %s = \"%s\"\r\n", sg_omciDefCfgObj[idx].name, pVal);
            break;
        }
    }

    if (OMCI_CFG_OBJ_MAX_NUM == idx) {
        omci_sprint(cli, "Unsupport value name %s\r\n", pData);
    }
}


void ca_omci_remove_trailing_chars(char *path, char c)
{
    size_t len;

    len = strlen(path);
    while (len > 0 && path[len-1] == c)
        path[--len] = '\0';
}


int ca_omci_get_key(char **line, char **key, char **value)
{
    char *linepos;
    char *temp;
    int   letter = 0;

    linepos = *line;
    if (!linepos)
    {
        return -1;
    }

    if (linepos[0] == '\0')
    {
        return -1;
    }

    /* get the key */
    *key = linepos;
    while (1)
    {
        linepos++;

        if (linepos[0] == '\0')
        {
            return -1;
        }

        letter = linepos[0];
        if (isspace(letter))
            break;

        if (linepos[0] == '=')
            break;

    }

    /* terminate key */
    linepos[0] = '\0';

    while (1)
    {
        linepos++;

        if (linepos[0] == '\0')
        {
            return -1;
        }

        letter = linepos[0];
        if (isspace(letter))
            continue;

        if (linepos[0] == '=')
            continue;

        break;
    }

    /* get the value*/
    if (linepos[0] == '"')
    {
        linepos++;
    }
    else
    {
        return -1;
    }
    *value = linepos;

    temp = strchr(linepos, '"');
    if (!temp)
    {
        return -1;
    }

    temp[0] = '\0';
    return 0;
}


void ca_omci_print_all_vars()
{
    ca_omci_uint8_t idx = 0;

    for (idx = 0; idx < OMCI_CFG_OBJ_MAX_NUM; idx++) {
        ca_printf("%s = \"%s\"\r\n", sg_omciDefCfgObj[idx].name, sg_omciDefCfgObj[idx].value);
    }
}


void ca_omci_load_config_file_value(void *pData)
{
    ca_omci_onuinfo_obj_t *pObj = (ca_omci_onuinfo_obj_t *)pData;
    ca_omci_uint8_t tmpHex[OMCI_CFG_MAX_VALUE_LEN] = {0};
    ca_omci_uint8_t idx = 0;

    pObj->ponNum        = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_PON_PORT_NUM].value);
    pObj->potsNum       = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_POTS_PORT_NUM].value);
    pObj->batteryBackup = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_BATTERY_BACKUP].value);
    pObj->ipv6Flag      = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_IPV6_FLAG].value);
    pObj->iphostNum     = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_IPHOST_NUM].value);
    pObj->veipNum       = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_VEIP_NUM].value);
    pObj->ethNum        = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_ETH_NUM].value);
    pObj->ponSlot       = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_PON_SLOT].value);
    pObj->ethSlot       = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_ETH_SLOT].value);
    pObj->iphostSlot    = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_IPHOST_SLOT].value);
    pObj->potsSlot      = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_POTS_SLOT].value);
    pObj->veipSlot      = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_VEIP_SLOT].value);
    pObj->remoteDbg     = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_REMOTE_DEBUG].value);
    pObj->powerCsvMode  = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_POWER_CSV_MODE].value);
    pObj->veipIndepend  = ca_atoi(sg_omciDefCfgObj[OMCI_CFG_VEIP_INDEPEND].value);
    pObj->todRspTime    = ca_strtoll(sg_omciDefCfgObj[OMCI_CFG_TOD_RSP_TIME].value);

    if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_OLT_TYPE].value, "CA")){
        pObj->oltType = OMCI_OLT_TYPE_CA;
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_OLT_TYPE].value, "ALCL")){
        pObj->oltType = OMCI_OLT_TYPE_ALCL;
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_OLT_TYPE].value, "SIM")){
        pObj->oltType = OMCI_OLT_TYPE_SIM;
    } else {
        pObj->oltType = OMCI_OLT_TYPE_ALCL;
    }

    OMCI_MEMSET(tmpHex, 0, sizeof(tmpHex));
    if (CA_OSAL_OK == ca_str2hex(sg_omciDefCfgObj[OMCI_CFG_PSK].value, (char *)tmpHex, OMCI_PSK_LEN)) {
        OMCI_MEMCPY(pObj->psk, tmpHex, OMCI_PSK_LEN);
    }

    OMCI_MEMSET(tmpHex, 0, sizeof(tmpHex));
    if (CA_OSAL_OK == ca_str2hex(sg_omciDefCfgObj[OMCI_CFG_OMCC_VER].value, (char *)tmpHex, sizeof(ca_omci_uint8_t))) {
        OMCI_MEMCPY(&pObj->omccVersion, tmpHex, sizeof(ca_omci_uint8_t));
    }

    if ((0 < pObj->ethNum) && (pObj->ethNum < OMCI_MAX_PORT_NUM)) {
        OMCI_MEMSET(tmpHex, 0, sizeof(tmpHex));
        if (CA_OSAL_OK == ca_str2hex(sg_omciDefCfgObj[OMCI_CFG_ETH_MAP].value, (char *)tmpHex, pObj->ethNum)) {
            if (1 == pObj->ethNum) {
                OMCI_MEMCPY(&pObj->ethPortIdMap[0], tmpHex, sizeof(ca_omci_uint8_t));
            }else {
                for (idx = 0; idx < pObj->ethNum; idx++) {
                    pObj->ethPortIdMap[idx] = tmpHex[idx];
                }
            }
        }
    }

    if (strlen(sg_omciDefCfgObj[OMCI_CFG_GPON_VER].value) <= OMCI_VERSION_LEN) {
        OMCI_MEMCPY(pObj->version, sg_omciDefCfgObj[OMCI_CFG_GPON_VER].value, OMCI_VERSION_LEN);
    }

    if (strlen(sg_omciDefCfgObj[OMCI_CFG_EQID].value) <= OMCI_EQID_LEN) {
        OMCI_MEMCPY(pObj->eqId, sg_omciDefCfgObj[OMCI_CFG_EQID].value, OMCI_EQID_LEN);
    }

    if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_ISP_TYPE].value, "VERIZON")) {
        pObj->ispType = OMCI_ISP_TYPE_VERIZON;
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_ISP_TYPE].value, "ORANGE")) {
        pObj->ispType = OMCI_ISP_TYPE_ORANGE;
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_ISP_TYPE].value, "COMMON")) {
        pObj->ispType = OMCI_ISP_TYPE_COMMON;
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_ISP_TYPE].value, "ATNT")) {
        pObj->ispType = OMCI_ISP_TYPE_ATNT;
    } else {
        pObj->ispType = OMCI_ISP_TYPE_COMMON;
    }

    if (strlen(sg_omciDefCfgObj[OMCI_CFG_LOID].value) <= OMCI_LOID_LEN)
    {
        OMCI_MEMCPY(pObj->loid, sg_omciDefCfgObj[OMCI_CFG_LOID].value, OMCI_LOID_LEN);
    }

    if (strlen(sg_omciDefCfgObj[OMCI_CFG_PASSWD].value) <= OMCI_LOID_PW_LEN)
    {
        OMCI_MEMCPY(pObj->passwd, sg_omciDefCfgObj[OMCI_CFG_PASSWD].value, OMCI_LOID_PW_LEN);
    }

    for (idx = CA_OMCI_LOG_OFF; idx < CA_OMCI_LOG_LEVEL_MAX; idx++)
    {
        if (0 ==  OMCI_STRCMP(g_omciLogLevelStr[idx], sg_omciDefCfgObj[OMCI_CFG_LOG_LEVEL].value))
        {
            pObj->logLevel = idx;
            break;
        }
    }

    if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_FEC_MODE].value, "rs")) {
        pObj->fecMode = 1; /* CA_FEC_RS */
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_FEC_MODE].value, "base_r")) {
        pObj->fecMode = 2; /* CA_FEC_BASE_R */
    } else if (0 == OMCI_STRCMP((const char *)sg_omciDefCfgObj[OMCI_CFG_FEC_MODE].value, "disable")) {
        pObj->fecMode = 0; /*CA_FEC_DISABLE*/
    } else {
        pObj->fecMode = 1; /* CA_FEC_RS */
    }
}


void ca_omci_load_config_file(void *pData)
{
    struct fs_file_t zfp;
    struct fs_dirent zEntry;
    char  line[OMCI_CFG_MAX_LINE + 2];
    char  buf[OMCI_CFG_MAX_FILE];
    char *linepos;
    char *variable;
    char *value;
    size_t count;
    int lineno = 0;
    int retval = 0;
    int err    = 0;
    int read   = 0;
    int idx    = 0;
    int start  = 0;
    int offset = 0;

    fs_file_t_init(&zfp);

    /*When file is not exist, just return*/
    if (0 != fs_stat(OMCI_CFG_FILE, &zEntry)) {
        ca_printf("No omci.conf, just use default value\r\n");
        /*Update value depend on local default value*/
        ca_omci_load_config_file_value(pData);
        return;
    }

    err = fs_open(&zfp, OMCI_CFG_FILE, FS_O_READ);
    if (err < 0) {
        ca_printf("Failed to open %s (%d)\r\n", OMCI_CFG_FILE, err);
        return;
    }

    read = fs_read(&zfp, buf, OMCI_CFG_MAX_FILE);
    if (read <= 0) {
        ca_printf("Failed to read from file %s (err: %d)\r\n", OMCI_CFG_FILE, read);
        fs_close(&zfp);
        return;
    }

    lineno = 0;
    start  = 0;
    offset = 0;
    for (idx = 0; idx < read; idx++) {
        if (buf[idx] == '\n') {
            lineno++;
            count = idx - start;
            start = idx;

            if ((count == 0) || (count > OMCI_CFG_MAX_LINE)){
                ca_printf("skip line because of line count%d for file %s, line %d\r\n", count, OMCI_CFG_FILE, lineno);
                continue;
            }

            /*skip \n*/
            memset(line, 0, sizeof(line));
            memcpy(line, (buf + offset), (1 == lineno) ? count : (count - 1));
            offset = idx + 1;

            /* Add \0 at last */
            line[count] = '\0';
            linepos = line;
            retval  = ca_omci_get_key(&linepos, &variable, &value);
            if (retval != 0) {
                ca_printf("error parsing %s, line %d:%d\r\n", OMCI_CFG_FILE, lineno, (int)(linepos-line));
                continue;
            }

            if (strlen(variable) > OMCI_CFG_MAX_NAME_LEN) {
                ca_printf("var name to long %s, line %d:%d\r\n", OMCI_CFG_FILE, lineno, (int)(linepos-line));
                continue;
            }

            if (strlen(value) > OMCI_CFG_MAX_VALUE_LEN) {
                ca_printf("value to long %s, line %d:%d\r\n", OMCI_CFG_FILE, lineno, (int)(linepos-line));
                continue;
            }

            if (lineno > OMCI_CFG_OBJ_MAX_NUM)
            {
                ca_printf("value to long %s, line %d:%d\r\n", OMCI_CFG_FILE, lineno, (int)(linepos-line));
                break;
            }

            if (0 != strcmp(sg_omciDefCfgObj[lineno - 1].name, variable)) {
                ca_printf("Unmatch var name %s, line %d\r\n", variable, lineno);
                break;
            }

            ca_omci_remove_trailing_chars(value, '/');
            strncpy(sg_omciDefCfgObj[lineno - 1].value, value, sizeof(sg_omciDefCfgObj[lineno - 1].value));
            continue;
        }
    }

    fs_close(&zfp);
    if (lineno  != OMCI_CFG_OBJ_MAX_NUM){
        ca_printf("Warning: please run fs rm lfs1/omci.conf for cfg file update\r\n");
    }

    /*Update value depend on config file*/
    ca_omci_load_config_file_value(pData);
}


ca_omci_st_e ca_omci_get_config_var(ca_omci_int8_t *varName, ca_omci_int8_t *value, ca_omci_int32_t *len)
{
    ca_omci_int32_t length = 0;
    ca_omci_int32_t i      = 0;

    for(i = 0; i < OMCI_CFG_OBJ_MAX_NUM; i++)
    {
        if (strcasecmp(sg_omciDefCfgObj[i].name, varName) == 0)
        {
            length = strlen(sg_omciDefCfgObj[i].value);
            *len   = length;
            strncpy(value, sg_omciDefCfgObj[i].value, length);
            return CA_OMCI_OK;
        }
    }

    ca_printf("get %s failed\r\n", varName);
    return CA_OMCI_E;
}


ca_omci_st_e ca_omci_set_config_var(ca_omci_int8_t *varName, ca_omci_int8_t *value, ca_omci_int32_t len)
{
    struct fs_file_t zfp;
    ca_omci_int32_t  i    = 0;
    char            *buf  = NULL;
    ca_omci_int32_t  bufL = 0;

    for(i = 0; i < OMCI_CFG_OBJ_MAX_NUM; i++)
    {
        if (strcasecmp(sg_omciDefCfgObj[i].name, varName) == 0)
        {
            OMCI_MEMSET(sg_omciDefCfgObj[i].value, 0, sizeof(sg_omciDefCfgObj[i].value));
            strncpy(sg_omciDefCfgObj[i].value, value, len);
        }
    }

    /*Always create a file after update any value */
    fs_file_t_init(&zfp);
    if (0 != fs_open(&zfp, OMCI_CFG_FILE, FS_O_CREATE)) {
        ca_printf("Create omci cfg file failed\r\n");
        return CA_OMCI_E;
    }
    fs_truncate(&zfp, 0);

    buf = ca_malloc(OMCI_CFG_MAX_FILE);
    if (!buf) {
        fs_close(&zfp);
        return CA_OMCI_E;
    }

    for(i = 0; i < OMCI_CFG_OBJ_MAX_NUM; i++){
        bufL = sprintf(buf, "%s=\"%s\"\n", sg_omciDefCfgObj[i].name, sg_omciDefCfgObj[i].value);
        fs_write(&zfp, buf, bufL);
    }

    fs_close(&zfp);
    ca_free(buf);
    return CA_OMCI_OK;
}


