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
#include <zephyr.h>
#include "omci_common.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_codec_pkt.h"
#include "omci_me.h"
#include "omci_log.h"


void ca_omci_pkt_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen);


ca_omci_bool_t ca_omci_pkg_rx_info_print = CA_OMCI_FALSE;
ca_omci_bool_t ca_omci_pkg_tx_info_print = CA_OMCI_FALSE;
ca_omci_bool_t ca_omci_pkg_tx_hex_print  = CA_OMCI_FALSE;
ca_omci_bool_t ca_omci_pkg_rx_hex_print  = CA_OMCI_FALSE;


static char *s_pOmciCmdStr[OMCI_MSG_TYPE_MAX] = 
{
    "Unknown Action",  "Unknown Action",         "Unknown Action",         "Unknown Action",          "Create",
    "Unknown Action",  "Delete",                 "Unknown Action",         "Set",                     "Get",
    "Unknown Action",  "Get all alarms",         "Get all alarms next",    "MIB upload",              "MIB upload next",
    "MIB reset",       "Alarm",                  "Attribute value change", "Test",                    "Start sw download",
    "Download section","End sw download",        "Activate sw",            "Commit sw",               "Synchronize time",
    "Reboot",          "Get next",               "Test result",            "Get current data",        "Set table"
};


void ca_omci_pkg_info_rx_debug_set(ca_omci_bool_t admin)
{
    ca_omci_pkg_rx_info_print = admin;
}


void ca_omci_pkg_info_tx_debug_set(ca_omci_bool_t admin)
{
    ca_omci_pkg_tx_info_print = admin;
}


void ca_omci_pkg_hex_rx_debug_set(ca_omci_bool_t admin)
{
    ca_omci_pkg_rx_hex_print = admin;
}


void ca_omci_pkg_hex_tx_debug_set(ca_omci_bool_t admin)
{
    ca_omci_pkg_tx_hex_print = admin;
}


void ca_omci_pkg_print_header(ca_omci_uint8_t *pBuf)
{
    ca_omci_pkt_hdr_t  *hdr      = (ca_omci_pkt_hdr_t *)pBuf;
    ca_omci_meclass_t  *meclass  = NULL;
    ca_omci_uint32_t    msgType  = 0;
    ca_omci_uint16_t    classId  = 0;
    ca_omci_class_id_e  innerCls = OMCI_ME_CLASS_ID_MIN;

    msgType = hdr->msgType & 0x1f;

    printk("---------------- Header ----------------\n");
    printk("Transaction Id = 0x%04x\n", ca_omci_ntohs(hdr->transId));
    if (msgType >= OMCI_MSG_TYPE_MAX)
    {
        printk("Message type   = %s(%d) DB=%d AR=%d AK=%d\n",
               "Unknown Action", msgType,
               (OMCI_DB_FLAG(hdr->msgType) != 0),
               (OMCI_AR_FLAG(hdr->msgType) != 0),
               (OMCI_AK_FLAG(hdr->msgType) != 0));
    }
    else
    {
        printk("Message type   = %s(%d) DB=%d AR=%d AK=%d\n",
               s_pOmciCmdStr[msgType], msgType,
               (OMCI_DB_FLAG(hdr->msgType) != 0),
               (OMCI_AR_FLAG(hdr->msgType) != 0),
               (OMCI_AK_FLAG(hdr->msgType) != 0));
    }
    printk("Device id      = 0x%02x\n", hdr->devId);

    classId  = ca_omci_ntohs(hdr->classId);
    innerCls = ca_omci_me_get_inner_class(classId);
    meclass  = ca_omci_me_get(innerCls);
    if (NULL == meclass)
    {
        printk("Class id       = %d [0x%04x]\n", classId, classId);
        printk("Class desc     = Unknown class\n");
    }
    else
    {
        printk("Class id       = %d [0x%04x]\n", classId, classId);
        printk("Class desc     = %s\n", meclass->desc);
    }
    printk("Instance id    = 0x%02x\n", ca_omci_ntohs(hdr->instId));
    printk("----------------------------------------\n");
}


void ca_omci_pkt_rx_info_print(ca_omci_uint8_t *pBuf , ca_omci_uint32_t len)
{
    if ((CA_OMCI_FALSE == ca_omci_pkg_rx_info_print) || (NULL == pBuf))
    {
        return;
    }

    /* print the detail info for omci receive packet */

    printk("\n++++++++++OMCI Request Message++++++++++\n");
    ca_omci_pkg_print_header(pBuf);
    printk("---------------- Content ------------------");
    ca_omci_pkt_hex_print(pBuf, len);
}


void ca_omci_pkt_tx_info_print(ca_omci_uint8_t *pBuf , ca_omci_uint32_t len)
{
    if (CA_OMCI_FALSE == ca_omci_pkg_tx_info_print)
    {
        return;
    }

    printk("\n----------OMCI Response Message----------\n");

    /* print the detail info for omci transmit packet */
    ca_omci_pkg_print_header(pBuf);
    printk("---------------- Content ------------------");
    ca_omci_pkt_hex_print(pBuf, len);
}


void ca_omci_pkt_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen)
{
    ca_omci_uint32_t i = 0;
    ca_omci_uint32_t j = 0;
    ca_omci_uint32_t k = 0;
    ca_omci_uint8_t  strbuf[16];
    ca_omci_uint32_t remainLen = 0;

    for (i = 0 ; i < pktLen ; i++)
    {
        if (i % 16 == 0)
        {
            j = 0;
            printk("\r\n%08x: ", (ca_omci_uint32_t)(i));
            OMCI_MEMSET(strbuf , 0 , sizeof(strbuf));
            remainLen = pktLen - i;
        }

        strbuf[i % 16] =  *((ca_omci_uint8_t *)(pkt + i));
        printk("%02x ", *((ca_omci_uint8_t *)(pkt + i)));

        if (++j == 8)
        {
            printk(" ");
        }

        if (remainLen == j && (remainLen % 16) != 0)
        {
            if (j < 8)
            {
                printk(" ");
            }

            for ( ; j < 16; j++)
            {
                printk("   ");
            }
        }

        if (j == 16)
        {
            remainLen = 0;
            printk("; ");

            for (k = 0 ; k < 16 ; k++)
            {
                if (strbuf[k] >= ' ' && strbuf[k] <= '~')
                {
                    printk("%c", strbuf[k]);
                }
                else
                {
                    printk(".");
                }
            }
        }
    }

    printk("\r\n\n");
    return;
}


void ca_omci_pkt_tx_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen)
{
    if (CA_OMCI_FALSE == ca_omci_pkg_tx_hex_print)
    {
        return;
    }

    printk("\r\n=====================================[tx]=====================================");
    ca_omci_pkt_hex_print(pkt, pktLen);
}


void ca_omci_pkt_rx_hex_print(ca_omci_uint8_t *pkt, ca_omci_uint16_t pktLen)
{
    if (CA_OMCI_FALSE == ca_omci_pkg_rx_hex_print)
    {
        return;
    }

    printk("\r\n=====================================[rx]=====================================");
    ca_omci_pkt_hex_print(pkt, pktLen);
}

