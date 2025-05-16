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

Copyright (c) 2022 by CA Incorporated
****************************************************************************/
#include "omci_mng_zephyr_cmd.h"
#include "omci_common.h"
#include "omci_me_base.h"
#include "omci_me_next.h"
#include "omci_codec_pkt.h"
#include "omci_me_gem_iwtp.h"
#include "omci_me_gem_net_ctp.h"
#include "omci_me_ieee_dot1p_srv.h"
#include "omci_me_mac_brg_srv.h"
#include "omci_me_pptp_eth.h"
#include "omci_me_gal_eth_prof.h"
#include "omci_me_tcont.h"
#include "omci_me_ext_vlan_tag_op.h"
#include "omci_me_mac_port_cfg.h"
#include "omci_me_multi_gem_iwtp.h"
#include "omci_me_vlan_tag_filter.h"
#include "omci_me.h"
#include "omci_hal.h"
#include "omci_avl.h"
#include "omci_analysis_flow.h"
#include "omci_mq.h"
#include "omci_mng_onuinfo.h"


#define CA_OMCI_CHECK_CONTINUE(param)\
     if (param)\
        {\
            continue;\
        }


extern const ca_omci_int8_t *g_omciLogLevelStr[];
extern const ca_omci_int8_t *g_omciLogModuleStr[];


static ca_omci_int8_t *sg_FlowDirStr[] =
{
    "Unknown",
    "US",      // US
    "DS",      // DS
    "BI"       // US + DS
};


static ca_omci_int8_t *sg_ConnModeStr[] =
{
    "Unknown",
    "Bridge",
    "802.1p",
    "Bridge + 802.1p",
};


static ca_omci_int8_t *sg_StreamMapStr[] =
{
    "Mac port",
    "802.1p",
    "PPTP eth",
    "Iphost",
    "PPTP xDsl",
    "Gem Iwtp",
    "Multicast Gem Iwtp",
    "PPTP MoCA",
    "Reserved",
    "Eth flow",
    "VEIP",
    "MPLS",
};


static ca_omci_int8_t *sg_StreamFilterStr[] =
{
    "No filter",
    "  IPoE   ",
    "  PPPoE  ",
    "  ARP    ",
    "IPv6 IpoE",
};


static ca_omci_int8_t *sg_StreamMacPortStr[] =
{
    "unknown",
    "PPTP eth",
    "IW VCC TP",
    "802.1p",
    "Iphost",
    "Gem IwTP",
    "Multicast Gem Iwtp",
    "PPTP XDSL P1",
    "PPTP VDSL",
    "Eth flow",
    "Reserved",
    "VEIP",
    "PPTP MoCA"
};


static ca_omci_int8_t *sg_StreamDsModeStr[] =
{
    "Inverse us rules",
    "Forwarded unmodified",
    "Filter & Inverse on VID & Pbit, Pass unmatch",
    "Filter & Inverse on VID, Pass Pbits, Pass unmatch",
    "Filter & Inverse on Pbit, Pass VID, Pass unmatch",
    "Filter & Inverse on VID & Pbit, Discard unmatch",
    "Filter & Inverse on VID, Pass Pbits, Discard unmatch",
    "Filter & Inverse on Pbit, Pass VID, Discard unmatch",
    "Discard all"
};


static ca_omci_int8_t *sg_OltTypeMapStr[] =
{
    "UNKNOWN",
    "CA",  //OMCI_OLT_TYPE_CA
    "ALCL",     //OMCI_OLT_TYPE_ALCL
    "SIM",
    "UNKNOWN",  //OMCI_OLT_TYPE_UNKNOWN
};


static ca_omci_int8_t *sg_IspTypeMapStr[] =
{
    "COMMON",   //OMCI_ISP_TYPE_COMMON
    "VERIZON",  //OMCI_ISP_TYPE_VERIZON
    "ORANGE",   //OMCI_ISP_TYPE_ORANGE
    "ATNT",     //OMCI_ISP_TYPE_ATNT
    "UNKNOW",   //OMCI_ISP_TYPE_UNKNOWN
};


static ca_omci_int8_t *sg_FecModeMapStr[] =
{
    "disable", //CA_DISABLE_FEC
    "base_r",  //CA_BASE_R_FEC
    "rs",      //CA_RS_FEC
};


extern void ca_omci_analysis_flow_tbl_show(const struct shell *cli, ca_omci_uint8_t flowType);
extern void ca_omci_hal_flow_cls_show(const struct shell *cli);
extern void ca_omci_hal_gemflow_map_show(const struct shell *cli);
extern ca_omci_st_e ca_omci_decode_pkt(ca_omci_uint8_t *pkt);
extern ca_omci_uint32_t ca_omci_gen_mic(ca_omci_uint8_t dir, ca_omci_uint8_t *omciContent, ca_omci_uint32_t omciContentLen);
extern void ca_omci_cfg_file_support_display(const struct shell *cli);
extern void ca_omci_cfg_file_get_value(const struct shell *cli, char *pData);
extern void ca_omci_cfg_file_set_value(const struct shell *cli, char *pData, char *pVal);
extern void ca_omci_mgr_img_upg_show(const struct shell *cli);


static ca_omci_uint8_t ca_omci_cli_get_gem_pri_map(ca_omci_uint16_t instanceId, ca_omci_ieee_dot1p_srv_t *pMe)
{
    ca_omci_uint8_t pri = 0 ;

    if (instanceId == pMe->attr.iwtpPri0)
    {
        pri = pri | (1 << 0);
    }
    if (instanceId == pMe->attr.iwtpPri1)
    {
        pri = pri | (1 << 1);
    }
    if (instanceId == pMe->attr.iwtpPri2)
    {
        pri = pri | (1 << 2);
    }
    if (instanceId == pMe->attr.iwtpPri3)
    {
        pri = pri | (1 << 3);
    }
    if (instanceId == pMe->attr.iwtpPri4)
    {
        pri = pri | (1 << 4);
    }
    if (instanceId == pMe->attr.iwtpPri5)
    {
        pri = pri | (1 << 5);
    }
    if (instanceId == pMe->attr.iwtpPri6)
    {
        pri = pri | (1 << 6);
    }
    if (instanceId == pMe->attr.iwtpPri7)
    {
        pri = pri | (1 << 7);
    }

    return pri;
}


static int ca_omci_cli_show_stream(const struct shell *cli)
{
    ca_omci_meclass_t          *meClass     = NULL;
    ca_omci_mebase_t           *meInst      = NULL;
    ca_omci_ext_vlan_tag_op_t  *extvlan     = NULL;
    ca_omci_filter_inner_t     *infilter    = NULL;
    ca_omci_filter_outter_t    *outfilter   = NULL;
    ca_omci_treat_outter_t     *outtreat    = NULL;
    ca_omci_treat_inner_t      *intreat     = NULL;
    ca_omci_mac_port_cfg_t     *brgportcfg  = NULL;
    ca_omci_int8_t              ethType[20] = {0};
    ca_omci_uint8_t             idx         = 0;
    ca_uint8_t                  dscpMap[64] = {0};

    meClass = ca_omci_me_get(OMCI_ME_EXT_VLAN_TAG_OP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        shell_print(cli, "no extern vlan is created now !");
        return 0;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        extvlan = (ca_omci_ext_vlan_tag_op_t *)meInst;
        if (EXT_VLAN_TAG_OP_ASSOC_TYPE_RESERVED == extvlan->attr.assocType)
        {
            continue;
        }

        if (EXT_VLAN_TAG_OP_ASSOC_TYPE_MAC_PORT == extvlan->attr.assocType)
        {
            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_PORT_CFG_CLASS_ID, extvlan->attr.assocPoint));
            if (NULL == brgportcfg)
            {
                shell_print(cli, "EVTOCD[0x%04X] Link to %s: 0x%x -------> Nothing",
                            extvlan->meBase.instanceId,
                            sg_StreamMapStr[extvlan->attr.assocType],
                            extvlan->attr.assocPoint);
            }
            else
            {
                shell_print(cli, "EVTOCD[0x%04X] Link to %s: 0x%x -------> %s:0x%x",
                            extvlan->meBase.instanceId,
                            sg_StreamMapStr[extvlan->attr.assocType],
                            extvlan->attr.assocPoint,
                            sg_StreamMacPortStr[brgportcfg->attr.tpType],
                            brgportcfg->attr.tpPointer);
            }
        }
        else
        {
            shell_print(cli, "EVTOCD[0x%04X] Link to %s: ------->instance 0x%x",
                        extvlan->meBase.instanceId,
                        sg_StreamMapStr[extvlan->attr.assocType],
                        extvlan->attr.assocPoint);
        }
        shell_print(cli, "    input TPID 0x%x output TPID 0x%x", extvlan->attr.inputTpid, extvlan->attr.outputTpid);
        shell_print(cli, "    dsMode:%s", sg_StreamDsModeStr[extvlan->attr.dsMode]);
        shell_print(cli, "    -------------------------------------------------------------------");
        shell_print(cli, "    |               Filter              |          Treatment          |");
        shell_print(cli, "    |-----------------------------------|-----------------------------|");
        shell_print(cli, "    |    Outer   |         Inner        | R |   Outer    |    Inner   |");
        shell_print(cli, "    |------------|----------------------| E |------------|------------|");
        shell_print(cli, "    | PRI | VID  | PRI | VID  | Filter  | M | PRI | VID  | PRI | VID  |");
        shell_print(cli, "    |-----|------|-----|------|---------|---|-----|------|-----|------|");

        for (idx = 0; idx < OMCI_EXT_VLAN_TAG_OP_RX_VLAN_TBL; idx++)
        {
            if (CA_OMCI_TRUE == extvlan->attr.rxVlanTbl[idx].valid)
            {
                outfilter = (ca_omci_filter_outter_t *)(&(extvlan->attr.rxVlanTbl[idx].filterOutter));
                infilter  = (ca_omci_filter_inner_t *)(&(extvlan->attr.rxVlanTbl[idx].filterInner));
                outtreat  = (ca_omci_treat_outter_t *)(&(extvlan->attr.rxVlanTbl[idx].treatOutter));
                intreat   = (ca_omci_treat_inner_t *)(&(extvlan->attr.rxVlanTbl[idx].treatInner));

                if (infilter->filterEthType > 4)
                {
                    strcpy(ethType, " unknown ");
                }
                else
                {
                    strcpy(ethType, sg_StreamFilterStr[infilter->filterEthType]);
                }

                if (0 != outtreat->pad){
                    shell_print(cli, "    | %3d | %4d | %3d | %4d |%s| %d | %3d | %4d | %3d | %4d |0x%x",
                                outfilter->filterOutPri,
                                outfilter->filterOutVid,
                                infilter->filterInnPri,
                                infilter->filterInnVid,
                                ethType,
                                outtreat->trearTag2Remove,
                                outtreat->treatOutPri,
                                outtreat->treatOutVid,
                                intreat->treatInnPri,
                                intreat->treatInnVid, outtreat->pad);
                } else {
                    shell_print(cli, "    | %3d | %4d | %3d | %4d |%s| %d | %3d | %4d | %3d | %4d |",
                                outfilter->filterOutPri,
                                outfilter->filterOutVid,
                                infilter->filterInnPri,
                                infilter->filterInnVid,
                                ethType,
                                outtreat->trearTag2Remove,
                                outtreat->treatOutPri,
                                outtreat->treatOutVid,
                                intreat->treatInnPri,
                                intreat->treatInnVid);
                }
            }
        }
        shell_print(cli, "    |-----|------|-----|------|---------|---|-----|------|-----|------|");

        for (idx = 0; idx < 8; idx++){
            dscpMap[idx * 8 + 0] = (extvlan->attr.dscp2Pbit[idx * 3 + 0] >> 5) & 0x07;
            dscpMap[idx * 8 + 1] = (extvlan->attr.dscp2Pbit[idx * 3 + 0] >> 2) & 0x07;
            dscpMap[idx * 8 + 2] = ((extvlan->attr.dscp2Pbit[idx * 3 + 0] & 3) << 1) | (extvlan->attr.dscp2Pbit[idx * 3 + 1] >> 7);
            dscpMap[idx * 8 + 3] = (extvlan->attr.dscp2Pbit[idx * 3 + 1] >> 4) & 0x07;
            dscpMap[idx * 8 + 4] = (extvlan->attr.dscp2Pbit[idx * 3 + 1] >> 1) & 0x07;
            dscpMap[idx * 8 + 5] = ((extvlan->attr.dscp2Pbit[idx * 3 + 1] & 1) << 2) | (extvlan->attr.dscp2Pbit[idx * 3 + 2] >> 6);
            dscpMap[idx * 8 + 6] = (extvlan->attr.dscp2Pbit[idx * 3 + 2] >> 3) & 0x07;
            dscpMap[idx * 8 + 7] = (extvlan->attr.dscp2Pbit[idx * 3 + 2] >> 0) & 0x07;
        }

        shell_print(cli, "--------------------------------- DSCP to Pbit Map -------------------------------------");
        shell_print(cli, "| DSCP | 0  | 1  | 2  | 3  | 4  | 5  | 6  | 7  | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |");
        shell_print(cli, "| pBit | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  |",
                        dscpMap[0], dscpMap[1], dscpMap[2], dscpMap[3], dscpMap[4], dscpMap[5], dscpMap[6], dscpMap[7],
                        dscpMap[8], dscpMap[9], dscpMap[10], dscpMap[11], dscpMap[12], dscpMap[13], dscpMap[14], dscpMap[15]);
        shell_print(cli, "| DSCP | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |");
        shell_print(cli, "| pBit | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  |",
                        dscpMap[16], dscpMap[17], dscpMap[18], dscpMap[19], dscpMap[20], dscpMap[21], dscpMap[22], dscpMap[23],
                        dscpMap[24], dscpMap[25], dscpMap[26], dscpMap[27], dscpMap[28], dscpMap[29], dscpMap[30], dscpMap[31]);
        shell_print(cli, "| DSCP | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |");
        shell_print(cli, "| pBit | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  |",
                        dscpMap[32], dscpMap[33], dscpMap[34], dscpMap[35], dscpMap[36], dscpMap[37], dscpMap[38], dscpMap[39],
                        dscpMap[40], dscpMap[41], dscpMap[42], dscpMap[43], dscpMap[44], dscpMap[45], dscpMap[46], dscpMap[47]);
        shell_print(cli, "| DSCP | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |");
        shell_print(cli, "| pBit | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  | %d  |",
                        dscpMap[48], dscpMap[49], dscpMap[50], dscpMap[51], dscpMap[52], dscpMap[53], dscpMap[54], dscpMap[55],
                        dscpMap[56], dscpMap[57], dscpMap[58], dscpMap[59], dscpMap[60], dscpMap[61], dscpMap[62], dscpMap[63]);
        shell_print(cli, "----------------------------------------------------------------------------------------");
    }
    return 0;
}



static int ca_omci_cli_show_connection(const struct shell *cli)
{
    ca_omci_meclass_t         *meClass    = NULL;
    ca_omci_mebase_t          *meInst     = NULL;
    ca_omci_me_link_t         *link       = NULL;
    ca_omci_gem_net_ctp_t     *gpnctp     = NULL;
    ca_omci_tcont_t           *tcont      = NULL;
    ca_omci_gem_iwtp_t        *gemiwtp    = NULL;
    ca_omci_mac_brg_srv_t     *macbrg     = NULL;
    ca_omci_mac_port_cfg_t    *brgportcfg = NULL;
    ca_omci_ieee_dot1p_srv_t  *ieee8021p  = NULL;
    ca_omci_me_link_t         *linkbrgp   = NULL;
    ca_omci_multi_gem_iwtp_t  *multiGem   = NULL;
    ca_omci_vlan_tag_filter_t *vlanfilter = NULL;
    ca_omci_uint8_t            connMode   = 0;
    ca_omci_uint8_t            pri        = 0;
    ca_omci_uint8_t            idx        = 0;
    ca_omci_uint16_t           vid        = 0;

    meClass = ca_omci_me_get(OMCI_ME_GEM_NET_CTP_CLASS_ID);
    if ((meClass == NULL) || (meClass->instHash == NULL))
    {
        shell_print(cli, "no gemport is created now !");
        return 0;
    }

    for (meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, NULL);
         meInst; meInst = (ca_omci_mebase_t *) HASH_GetNext(meClass->instHash, meInst))
    {
        gpnctp = (ca_omci_gem_net_ctp_t *)meInst;

        /* if can't find the tcont , the flow is not complete */
        if (GEM_NET_CTP_DIR_DS != gpnctp->attr.direction)
        {
            tcont  = (ca_omci_tcont_t *)(ca_omci_me_entry_lookup(OMCI_ME_TCONT_CLASS_ID, gpnctp->attr.tcontPoint));
            CA_OMCI_CHECK_CONTINUE(NULL == tcont);
        }

        /* If gemport necwork ctp is not linked, the flow is not complete */
        CA_OMCI_CHECK_CONTINUE(1 != meInst->linkedCount);

        link = meInst->linkedMeHeader;
        CA_OMCI_CHECK_CONTINUE(NULL == link);

        if (OMCI_ME_GEM_IWTP_CLASS_ID == link->classId)
        {
            gemiwtp = (ca_omci_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            CA_OMCI_CHECK_CONTINUE(NULL == gemiwtp);

            /* If gemport iwtp is not linked, the flow is not complete
                                    Get ANI side of vlan filter */
            CA_OMCI_CHECK_CONTINUE(0 == gemiwtp->meBase.linkedCount);
            link = gemiwtp->meBase.linkedMeHeader;
            while (link)
            {
                /* find the mac port cfg data me only */
                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
                {
                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                    if (NULL != brgportcfg)
                    {
                        vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                    }
                }

                link = link->next;
            }

            /* If gem iwtp linked by mac bridge srv this flow is bridge mode */
            if (GEM_IWTP_IW_OPTION_MAC_BRG_LAN == gemiwtp->attr.iwOption)
            {
                connMode = 1; // Bridge
                macbrg   = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));

                /* If mac bridge srv is null, the flow is not complete */
                CA_OMCI_CHECK_CONTINUE(NULL == macbrg);

                if (NULL == tcont)
                {
                    shell_print(cli, "GEM:%5d  | (%s, TCONT NULL)", gpnctp->attr.portId, sg_FlowDirStr[gpnctp->attr.direction]);
                }
                else
                {
                    shell_print(cli, "GEM:%5d  | (%s, TCONT:0x%X, allocId:%d)",
                                   gpnctp->attr.portId,
                                   sg_FlowDirStr[gpnctp->attr.direction],
                                   tcont->meBase.instanceId,
                                   tcont->attr.allocId);
                }

                shell_print(cli, "           --%s", sg_ConnModeStr[connMode]);
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    omci_sprint(cli, "           --Ani Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vid = 0;
                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[(idx * 2) + 1];
                        omci_sprint(cli, " %d", vid);
                    }
                    omci_sprint(cli, "\r\n");
                }

                if (macbrg->meBase.linkedCount)
                {
                    link = macbrg->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        /* Get the bridge mode UNI side */
                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link       = link->next;
                        CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                        if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                         && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                        {
                            continue;
                        }

                        vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                        if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                        {
                            omci_sprint(cli, "           --Uni Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                            for (idx = 0; idx < vlanfilter->attr.number; idx++)
                            {
                                vid = 0;
                                vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                                omci_sprint(cli, " %d", vid);
                            }
                            omci_sprint(cli, "\r\n");
                        }

                        if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                        {
                            shell_print(cli, "             |--Eth   :0x%04X", brgportcfg->attr.tpPointer);
                        }
                        if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                        {
                            shell_print(cli, "             |--Iphost:0x%04X", brgportcfg->attr.tpPointer);
                        }
                        if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                        {
                            shell_print(cli, "             |--Veip  :0x%04X", brgportcfg->attr.tpPointer);
                        }
                    }
                }
                continue;
            }

            if (GEM_IWTP_IW_OPTION_8021P == gemiwtp->attr.iwOption)
            {
                ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID, gemiwtp->attr.srvProfPoint));
                CA_OMCI_CHECK_CONTINUE(NULL == ieee8021p);

                pri = ca_omci_cli_get_gem_pri_map(gemiwtp->meBase.instanceId, ieee8021p);
                /* when 802.1p links to Bridge, the mode is 802.1p + Bridge*/
                if (IEEE_DOT1P_SRV_TP_TYPE_BRIDGE == ieee8021p->attr.tpType)
                {
                    connMode = 3; // Bridge + 802.1p

                    if (ieee8021p->meBase.linkedCount)
                    {
                        link = ieee8021p->meBase.linkedMeHeader;
                        while (link)
                        {
                            /* find the mac port cfg data me only */
                            if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                            {
                                link = link->next;
                                continue;
                            }

                            /* Get the bridge for 802.1p, there is only one bridge for a 802.1p */
                            brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                            link       = link->next;
                            CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                            macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                            CA_OMCI_CHECK_CONTINUE(NULL == macbrg);

                            if (NULL == tcont)
                            {
                                shell_print(cli, "GEM:%5d  | (%s, TCONT NULL)", gpnctp->attr.portId, sg_FlowDirStr[gpnctp->attr.direction]);
                            }
                            else
                            {
                                shell_print(cli, "GEM:%5d  | (%s, TCONT:0x%X, allocId:%d)",
                                               gpnctp->attr.portId,
                                               sg_FlowDirStr[gpnctp->attr.direction],
                                               tcont->meBase.instanceId,
                                               tcont->attr.allocId);
                            }

                            shell_print(cli, "           --%s:0x%x", sg_ConnModeStr[connMode], ieee8021p->meBase.instanceId);
                            shell_print(cli, "           --Map Pri 0x%x", pri);

                            vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                            if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                            {
                                omci_sprint(cli, "           --Ani Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                                for (idx = 0; idx < vlanfilter->attr.number; idx++)
                                {
                                    vid = 0;
                                    vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                                    omci_sprint(cli, " %d", vid);
                                }
                                omci_sprint(cli, "\r\n");
                            }

                            if (macbrg->meBase.linkedCount)
                            {
                                linkbrgp = macbrg->meBase.linkedMeHeader;
                                while (linkbrgp)
                                {
                                    /* find the mac port cfg data me only */
                                    if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                                    {
                                        linkbrgp = linkbrgp->next;
                                        continue;
                                    }

                                    /* Get the bridge mode UNI side */
                                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                                    linkbrgp   = linkbrgp->next;
                                    CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                                    if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                                     && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                                     && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                                    {
                                        continue;
                                    }

                                    vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                                    if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                                    {
                                        omci_sprint(cli, "           --Uni Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                                        for (idx = 0; idx < vlanfilter->attr.number; idx++)
                                        {
                                            vid = 0;
                                            vid = (vlanfilter->attr.vlanList[idx] << 8) | vlanfilter->attr.vlanList[idx + 1];
                                            omci_sprint(cli, " %d", vid);
                                        }
                                        omci_sprint(cli, "\r\n");
                                    }

                                    if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                                    {
                                        shell_print(cli, "             |--Eth   :0x%04X", brgportcfg->attr.tpPointer);
                                    }
                                    if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                                    {
                                        shell_print(cli, "             |--Iphost:0x%04X", brgportcfg->attr.tpPointer);
                                    }
                                    if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                                    {
                                        shell_print(cli, "             |--Veip  :0x%04X", brgportcfg->attr.tpPointer);
                                    }
                                }
                            }
                            break;
                        }
                    }
                    continue;
                }

                connMode = 2; // 802.1p
                if (NULL == tcont)
                {
                    shell_print(cli, "GEM:%5d  | (%s, TCONT NULL)", gpnctp->attr.portId, sg_FlowDirStr[gpnctp->attr.direction]);
                }
                else
                {
                    shell_print(cli, "GEM:%5d  | (%s, TCONT:0x%X, allocId:%d)",
                                   gpnctp->attr.portId,
                                   sg_FlowDirStr[gpnctp->attr.direction],
                                   tcont->meBase.instanceId,
                                   tcont->attr.allocId);
                }

                shell_print(cli, "           --%s:0x%x", sg_ConnModeStr[connMode], ieee8021p->meBase.instanceId);
                shell_print(cli, "           --Map Pri 0x%x", pri);

                if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
                 && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
                 && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
                {
                    continue;
                }

                if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
                {
                    shell_print(cli, "             |--Eth   :0x%04X", ieee8021p->attr.tpPointer);
                }
                if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
                {
                    shell_print(cli, "             |--Iphost:0x%04X", ieee8021p->attr.tpPointer);
                }
                if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
                {
                    shell_print(cli, "             |--Veip  :0x%04X", ieee8021p->attr.tpPointer);
                }
                continue;
            }

            if (GEM_IWTP_IW_OPTION_DS_BROADCAST == gemiwtp->attr.iwOption)
            {
                shell_print(cli, "GEM:%5d  | (%s, TCONT:NULL)",
                               gpnctp->attr.portId,
                               sg_FlowDirStr[gpnctp->attr.direction]);
                shell_print(cli, "           --broadcast");
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    omci_sprint(cli, "           --ANI Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vid = 0;
                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                        omci_sprint(cli, " %d", vid);
                    }
                    omci_sprint(cli, "\r\n");
                }

                if (gemiwtp->meBase.linkedCount)
                {
                    link = gemiwtp->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        /* Get the bridge for broadcast gemiwtp, there maybe more than one bridge linked it */
                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link       = link->next;
                        CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                        macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                        CA_OMCI_CHECK_CONTINUE(NULL == macbrg);

                        if (macbrg->meBase.linkedCount)
                        {
                            linkbrgp = macbrg->meBase.linkedMeHeader;
                            while (linkbrgp)
                            {
                                /* find the mac port cfg data me only */
                                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                                {
                                    linkbrgp = linkbrgp->next;
                                    continue;
                                }

                                /* Get the bridge mode UNI side */
                                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                                linkbrgp = linkbrgp->next;
                                CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                                if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                                {
                                    continue;
                                }

                                vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                                {
                                    omci_sprint(cli, "           --Uni Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                                    {
                                        vid = 0;
                                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                                        omci_sprint(cli, " %d", vid);
                                    }
                                    omci_sprint(cli, "\r\n");
                                }

                                if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Eth   :0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Iphost:0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Veip  :0x%04X", brgportcfg->attr.tpPointer);
                                }
                            }
                        }
                    }
                }
                continue;
            }

            continue;
        }

        if (OMCI_ME_MULTI_GEM_IWTP_CLASS_ID == link->classId)
        {
            multiGem = (ca_omci_multi_gem_iwtp_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
            CA_OMCI_CHECK_CONTINUE(NULL == multiGem);

            /* If gemport iwtp is not linked, the flow is not complete */
            CA_OMCI_CHECK_CONTINUE(0 == multiGem->meBase.linkedCount);
            link = multiGem->meBase.linkedMeHeader;
            while (link)
            {
                /* find the mac port cfg data me only */
                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID == link->classId)
                {
                    brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                    if (NULL != brgportcfg)
                    {
                        vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                    }
                }

                link = link->next;
            }

            if (MULTI_GEM_IWTP_IW_OPTION_MAC_BRG_LAN == multiGem->attr.iwOption)
            {
                shell_print(cli, "GEM:%5d  | (%s, TCONT: NULL)",
                               gpnctp->attr.portId,
                               sg_FlowDirStr[gpnctp->attr.direction]);
                shell_print(cli, "           --mutilcast Bridge");
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    omci_sprint(cli, "           --ANI Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vid = 0;
                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                        omci_sprint(cli, " %d", vid);
                    }
                    omci_sprint(cli, "\r\n");
                }

                if (multiGem->meBase.linkedCount)
                {
                    link = multiGem->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link       = link->next;
                        CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                        macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                        CA_OMCI_CHECK_CONTINUE(NULL == macbrg);

                        if (macbrg->meBase.linkedCount)
                        {
                            linkbrgp = macbrg->meBase.linkedMeHeader;
                            while (linkbrgp)
                            {
                                /* find the mac port cfg data me only */
                                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                                {
                                    linkbrgp = linkbrgp->next;
                                    continue;
                                }

                                /* Get the bridge mode UNI side */
                                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                                linkbrgp   = linkbrgp->next;
                                CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                                if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                                {
                                    continue;
                                }

                                vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                                {
                                    omci_sprint(cli, "           --Uni Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                                    {
                                        vid = 0;
                                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                                        omci_sprint(cli, " %d", vid);
                                    }
                                    omci_sprint(cli, "\r\n");
                                }

                                if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Eth   :0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Iphost:0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Veip  :0x%04X", brgportcfg->attr.tpPointer);
                                }
                            }
                        }
                    }
                }
                continue;
            }

            if (MULTI_GEM_IWTP_IW_OPTION_8021P == multiGem->attr.iwOption)
            {
                shell_print(cli, "GEM:%5d  | (%s, TCONT:NULL)",
                               gpnctp->attr.portId,
                               sg_FlowDirStr[gpnctp->attr.direction]);
                shell_print(cli, "           --mutilcast 802.1p");
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    omci_sprint(cli, "           --ANI Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vid = 0;
                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                        omci_sprint(cli, " %d", vid);
                    }
                    omci_sprint(cli, "\r\n");
                }

                if (multiGem->meBase.linkedCount)
                {
                    link = multiGem->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the 802.1p me only */
                        if (OMCI_ME_IEEE_DOT1P_SRV_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        ieee8021p = (ca_omci_ieee_dot1p_srv_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link      = link->next;
                        CA_OMCI_CHECK_CONTINUE(NULL == ieee8021p);

                        /* for mutilcast downstream, 802.1p direct link to uni */
                        if ((IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH != ieee8021p->attr.tpType)
                         && (IEEE_DOT1P_SRV_TP_TYPE_IPHOST != ieee8021p->attr.tpType)
                         && (IEEE_DOT1P_SRV_TP_TYPE_VEIP != ieee8021p->attr.tpType))
                        {
                            continue;
                        }

                        if (IEEE_DOT1P_SRV_TP_TYPE_PPTP_ETH == ieee8021p->attr.tpType)
                        {
                            shell_print(cli, "             |--Eth   :0x%04X", ieee8021p->attr.tpPointer);
                        }
                        if (IEEE_DOT1P_SRV_TP_TYPE_IPHOST == ieee8021p->attr.tpType)
                        {
                            shell_print(cli, "             |--Iphost:0x%04X", ieee8021p->attr.tpPointer);
                        }
                        if (IEEE_DOT1P_SRV_TP_TYPE_VEIP == ieee8021p->attr.tpType)
                        {
                            shell_print(cli, "             |--Veip  :0x%04X", ieee8021p->attr.tpPointer);
                        }
                    }
                }
            }

            if (MULTI_GEM_IWTP_IW_OPTION_DONT_CARE == multiGem->attr.iwOption)
            {
                shell_print(cli, "GEM:%5d  | (%s, TCONT:NULL)",
                               gpnctp->attr.portId,
                               sg_FlowDirStr[gpnctp->attr.direction]);
                shell_print(cli, "           --mutilcast no care");
                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                {
                    omci_sprint(cli, "           --ANI Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                    {
                        vid = 0;
                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                        omci_sprint(cli, " %d", vid);
                    }
                    omci_sprint(cli, "\r\n");
                }

                //multicast gem don't care the iw option, so it could be any
                if (multiGem->meBase.linkedCount)
                {
                    link = multiGem->meBase.linkedMeHeader;
                    while (link)
                    {
                        /* find the mac port cfg data me only */
                        if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != link->classId)
                        {
                            link = link->next;
                            continue;
                        }

                        brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(link->classId, link->instanceId));
                        link       = link->next;
                        CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                        macbrg = (ca_omci_mac_brg_srv_t *)(ca_omci_me_entry_lookup(OMCI_ME_MAC_BRG_SRV_CLASS_ID, brgportcfg->attr.brgIdPointer));
                        CA_OMCI_CHECK_CONTINUE(NULL == macbrg);

                        if (macbrg->meBase.linkedCount)
                        {
                            linkbrgp = macbrg->meBase.linkedMeHeader;
                            while (linkbrgp)
                            {
                                /* find the mac port cfg data me only */
                                if (OMCI_ME_MAC_PORT_CFG_CLASS_ID != linkbrgp->classId)
                                {
                                    linkbrgp = linkbrgp->next;
                                    continue;
                                }

                                /* Get the bridge mode UNI side */
                                brgportcfg = (ca_omci_mac_port_cfg_t *)(ca_omci_me_entry_lookup(linkbrgp->classId, linkbrgp->instanceId));
                                linkbrgp   = linkbrgp->next;
                                CA_OMCI_CHECK_CONTINUE(NULL == brgportcfg);

                                if ((MAC_PORT_CFG_TP_TYPE_PPTP_ETH != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_IPHOST != brgportcfg->attr.tpType)
                                 && (MAC_PORT_CFG_TP_TYPE_VEIP != brgportcfg->attr.tpType))
                                {
                                    continue;
                                }

                                vlanfilter = (ca_omci_vlan_tag_filter_t *)ca_omci_me_entry_lookup(OMCI_ME_VLAN_TAG_FILTER_CLASS_ID, brgportcfg->meBase.instanceId);
                                if ((NULL != vlanfilter) && (vlanfilter->attr.number > 0))
                                {
                                    omci_sprint(cli, "           --Uni Map Filter[0x%04X] Vlan Total Num %d Vid", vlanfilter->meBase.instanceId, vlanfilter->attr.number);
                                    for (idx = 0; idx < vlanfilter->attr.number; idx++)
                                    {
                                        vid = 0;
                                        vid = (vlanfilter->attr.vlanList[idx * 2] << 8) | vlanfilter->attr.vlanList[idx * 2 + 1];
                                        omci_sprint(cli, " %d", vid);
                                    }
                                    omci_sprint(cli, "\r\n");
                                }

                                if (MAC_PORT_CFG_TP_TYPE_PPTP_ETH == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Eth   :0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_IPHOST == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Iphost:0x%04X", brgportcfg->attr.tpPointer);
                                }
                                if (MAC_PORT_CFG_TP_TYPE_VEIP == brgportcfg->attr.tpType)
                                {
                                    shell_print(cli, "             |--Veip  :0x%04X", brgportcfg->attr.tpPointer);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}


static int ca_omci_cli_show_info(const struct shell *shell)
{
    ca_omci_onuinfo_obj_t *pOnuInfo = ca_omci_onuinfo_get();
    ca_omci_uint8_t        idx      = 0;

    shell_print(shell, "=======================================");
    shell_print(shell, "omcc       :0x%x", pOnuInfo->omccVersion);
    shell_print(shell, "ipv4Addr   :0x%x", pOnuInfo->ipv4Addr);
    shell_print(shell, "ipv4Mask   :0x%x", pOnuInfo->ipv4Mask);
    shell_print(shell, "ipv4Gw     :0x%x", pOnuInfo->ipv4Gw);
    shell_print(shell, "ponNum     :%d",   pOnuInfo->ponNum);
    shell_print(shell, "ethNum     :%d",   pOnuInfo->ethNum);
    shell_print(shell, "iphostNum  :%d",   pOnuInfo->iphostNum);
    shell_print(shell, "potsNum    :%d",   pOnuInfo->potsNum);
    shell_print(shell, "veipNum    :%d",   pOnuInfo->veipNum);
    shell_print(shell, "ponSlot    :%d",   pOnuInfo->ponSlot);
    shell_print(shell, "ethSlot    :%d",   pOnuInfo->ethSlot);
    shell_print(shell, "iphostSlot :%d",   pOnuInfo->iphostSlot);
    shell_print(shell, "potsSlot   :%d",   pOnuInfo->potsSlot);
    shell_print(shell, "veipSlot   :%d",   pOnuInfo->veipSlot);
    shell_print(shell, "oltType    :%s",   sg_OltTypeMapStr[pOnuInfo->oltType]);
    shell_print(shell, "ponMacMode :%d",   pOnuInfo->ponMacMode);
    shell_print(shell, "maxQ/Tc    :%d",   pOnuInfo->maxQueuePerTc);
    shell_print(shell, "ispType    :%s",   sg_IspTypeMapStr[pOnuInfo->ispType]);
    shell_print(shell, "fecMode    :%s",   sg_FecModeMapStr[pOnuInfo->fecMode]);
    shell_print(shell, "batteryBak :%d",   pOnuInfo->batteryBackup);
    shell_print(shell, "remoteDbg  :%d",   pOnuInfo->remoteDbg);
    shell_print(shell, "powerCsvMd :%d",   pOnuInfo->powerCsvMode);
    shell_print(shell, "ipv6Flag   :%d",   pOnuInfo->ipv6Flag);
    shell_print(shell, "veipIdp    :%d",   pOnuInfo->veipIndepend);
    shell_print(shell, "todRspTime :%llu", pOnuInfo->todRspTime);
    shell_print(shell, "loid       :%s",   pOnuInfo->loid);
    shell_print(shell, "passwd     :%s",   pOnuInfo->passwd);
    shell_print(shell, "vendorId   :%.4s", pOnuInfo->vendorId);
    shell_print(shell, "sn         :%.4s-%02x%02x%02x%02x",
                                           pOnuInfo->sn, pOnuInfo->sn[4], pOnuInfo->sn[5],pOnuInfo->sn[6],pOnuInfo->sn[7]);
    shell_print(shell, "version    :%s",   pOnuInfo->version);
    shell_print(shell, "eqId       :%s",   pOnuInfo->eqId);
    shell_print(shell, "mac        :%x:%x:%x:%x:%x:%x", pOnuInfo->mac[0], pOnuInfo->mac[1], pOnuInfo->mac[2],
                                          pOnuInfo->mac[3], pOnuInfo->mac[4], pOnuInfo->mac[5]);
    /*shell print will add \n at last, use shell_fprintf instead*/
    omci_sprint(shell, "psk        :0x");
    for (idx = 0; idx < OMCI_PSK_LEN; idx++)
    {
        omci_sprint(shell, "%x", pOnuInfo->psk[idx]);
    }
    omci_sprint(shell, "\r\n");

    shell_print(shell,"=======================================");
    for (idx = 0; idx < pOnuInfo->ethNum; idx++)
    {
        shell_print(shell, "Eth  Port idx %d map to Port %d", idx, pOnuInfo->ethPortIdMap[idx]);
    }
    return 0;
}


static int omci_show_flow_map(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow map");
        return 1;
    }

    ca_omci_hal_gemflow_map_show(shell);
    return 0;
}


static int omci_show_flow_cls(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow cls");
        return 1;
    }

    ca_omci_hal_flow_cls_show(shell);
    return 0;
}


static int omci_show_flow_stream_ds(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow stream_ds");
        return 1;
    }

    ca_omci_analysis_flow_tbl_show(shell, OMCI_FLOW_TYPE_DS_STR);
    return 0;
}


static int omci_show_flow_stream_us(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow stream_us");
        return 1;
    }

    ca_omci_analysis_flow_tbl_show(shell, OMCI_FLOW_TYPE_US_STR);
    return 0;
}


static int omci_show_flow_filter(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow filter");
        return 1;
    }

    ca_omci_analysis_flow_tbl_show(shell, OMCI_FLOW_TYPE_FILTER);
    return 0;
}


static int omci_show_flow_gem(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show flow gem");
        return 1;
    }

    ca_omci_analysis_flow_tbl_show(shell, OMCI_FLOW_TYPE_GEM);
    return 0;
}


static int omci_show_info(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show info");
        return 1;
    }

    return ca_omci_cli_show_info(shell);
}


static int omci_show_stream(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show stream");
        return 1;
    }

    return ca_omci_cli_show_stream(shell);
}


static int omci_show_connection(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show connection");
        return 1;
    }

    return ca_omci_cli_show_connection(shell);
}


static int omci_show_upg(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci show upg");
        return 1;
    }

    ca_omci_mgr_img_upg_show(shell);
    return 0;
}


static int omci_pkt_rx_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt rx off");
        return 1;
    }

    ca_omci_pkg_info_rx_debug_set(CA_OMCI_FALSE);
    return 0;
}


static int omci_pkt_rx_on(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt rx on");
        return 1;
    }

    ca_omci_pkg_info_rx_debug_set(CA_OMCI_TRUE);
    return 0;
}


static int omci_pkt_tx_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt tx off");
        return 1;
    }

    ca_omci_pkg_info_tx_debug_set(CA_OMCI_FALSE);
    return 0;
}


static int omci_pkt_tx_on(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt tx on");
        return 1;
    }

    ca_omci_pkg_info_tx_debug_set(CA_OMCI_TRUE);
    return 0;
}


static int omci_pkt_all_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt all off");
        return 1;
    }

    ca_omci_pkg_info_tx_debug_set(CA_OMCI_FALSE);
    ca_omci_pkg_info_rx_debug_set(CA_OMCI_FALSE);
    return 0;
}


static int omci_pkt_all_on(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci pkt all on");
        return 1;
    }

    ca_omci_pkg_info_tx_debug_set(CA_OMCI_TRUE);
    ca_omci_pkg_info_rx_debug_set(CA_OMCI_TRUE);
    return 0;
}


static int omci_log_upg_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log upg off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_UPG);
    return 0;
}


static int omci_log_upg_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log upg warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_UPG);
    return 0;
}


static int omci_log_upg_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log upg error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_UPG);
    return 0;
}


static int omci_log_upg_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log upg off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_UPG);
    return 0;
}


static int omci_log_pm_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log pm off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_PM);
    return 0;
}


static int omci_log_pm_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log pm warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_PM);
    return 0;
}


static int omci_log_pm_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log pm error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_PM);
    return 0;
}


static int omci_log_pm_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log pm off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_PM);
    return 0;
}

static int omci_log_alarm_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log alarm off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_AM);
    return 0;
}


static int omci_log_alarm_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log alarm warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_AM);
    return 0;
}


static int omci_log_alarm_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log alarm error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_AM);
    return 0;
}


static int omci_log_alarm_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log alarm off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_AM);
    return 0;
}


static int omci_log_hal_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log hal off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_HAL);
    return 0;
}


static int omci_log_hal_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log hal warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_HAL);
    return 0;
}


static int omci_log_hal_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log hal error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_HAL);
    return 0;
}


static int omci_log_hal_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log hal off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_HAL);
    return 0;
}


static int omci_log_analysis_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log analysis off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_ANALYSIS);
    return 0;
}


static int omci_log_analysis_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log analysis warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_ANALYSIS);
    return 0;
}


static int omci_log_analysis_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log analysis error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_ANALYSIS);
    return 0;
}


static int omci_log_analysis_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log analysis off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_ANALYSIS);
    return 0;
}


static int omci_log_mng_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log mng off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_MNG);
    return 0;
}


static int omci_log_mng_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log mng warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_MNG);
    return 0;
}


static int omci_log_mng_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log mng error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_MNG);
    return 0;
}


static int omci_log_mng_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log mng off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_MNG);
    return 0;
}


static int omci_log_codec_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log codec off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_CODEC);
    return 0;
}


static int omci_log_codec_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log codec warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_CODEC);
    return 0;
}


static int omci_log_codec_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log codec error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_CODEC);
    return 0;
}


static int omci_log_codec_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log codec off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_CODEC);
    return 0;
}


static int omci_log_common_debug(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log common off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_DEBUG, CA_OMCI_COMMON);
    return 0;
}


static int omci_log_common_warn(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log common warning");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_WARNING, CA_OMCI_COMMON);
    return 0;
}


static int omci_log_common_err(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log common error");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_ERROR, CA_OMCI_COMMON);
    return 0;
}


static int omci_log_common_off(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci log common off");
        return 1;
    }

    omci_loglevel_set(CA_OMCI_LOG_OFF, CA_OMCI_COMMON);
    return 0;
}


static int omci_log_all_debug(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint32_t idx = 0;

    if (argc != 1) {
        shell_print(shell, "usage:omci log all debug");
        return 1;
    }

    for (idx = CA_OMCI_COMMON; idx < CA_OMCI_M_MAX; idx++)
    {
        omci_loglevel_set(CA_OMCI_LOG_DEBUG, idx);
    }
    return 0;
}


static int omci_log_all_warn(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint32_t idx = 0;

    if (argc != 1) {
        shell_print(shell, "usage:omci log all warning");
        return 1;
    }

    for (idx = CA_OMCI_COMMON; idx < CA_OMCI_M_MAX; idx++)
    {
        omci_loglevel_set(CA_OMCI_LOG_WARNING, idx);
    }
    return 0;
}


static int omci_log_all_err(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint32_t idx = 0;

    if (argc != 1) {
        shell_print(shell, "usage:omci log all error");
        return 1;
    }

    for (idx = CA_OMCI_COMMON; idx < CA_OMCI_M_MAX; idx++)
    {
        omci_loglevel_set(CA_OMCI_LOG_ERROR, idx);
    }
    return 0;
}


static int omci_log_all_off(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint32_t idx = 0;

    if (argc != 1) {
        shell_print(shell, "usage:omci log all off");
        return 1;
    }

    for (idx = CA_OMCI_COMMON; idx < CA_OMCI_M_MAX; idx++)
    {
        omci_loglevel_set(CA_OMCI_LOG_OFF, idx);
    }
    return 0;
}


void ca_omci_cli_me_show_all(const struct shell *shell)
{
    ca_omci_uint32_t   i       = 0;
    ca_omci_meclass_t *meclass = NULL;
    ca_omci_mebase_t  *meBase  = NULL;

    for (i = 0; i < OMCI_ME_CLASS_ID_MAX; i++) {
        meclass = ca_omci_me_get(i);
        if (NULL == meclass) {
            continue;
        }
        for (meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, NULL);
             meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, meBase)) {
            if (NULL != meBase->handlers->meDisplay) {
                meBase->handlers->meDisplay(meBase, (void *)shell);
            }
        }
    }
}


void ca_omci_cli_me_show_support(const struct shell *cli)
{
    ca_omci_uint32_t   i       = 0;
    ca_omci_uint16_t   classId = 0;
    ca_omci_meclass_t *meclass = NULL;
    ca_omci_mebase_t  *meBase  = NULL;

    for (i = 0; i < OMCI_ME_CLASS_ID_MAX; i++) {
        meclass = ca_omci_me_get(i);
        if (NULL == meclass) {
            continue;
        }

        classId = ca_omci_me_get_standard_class(i);
        for (meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, NULL);
             meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, meBase)) {
            if (NULL != meBase->handlers->meDisplay) {
                omci_sprint(cli, "Classid %d instance %d\r\n", classId, meBase->instanceId);
            }
        }
    }
}


void ca_omci_cli_me_show_class_all(ca_omci_uint16_t classId, const struct shell *cli)
{
    ca_omci_class_id_e  innerClass = OMCI_ME_CLASS_ID_MAX;
    ca_omci_meclass_t  *meclass    = NULL;
    ca_omci_mebase_t   *meBase     = NULL;

    innerClass = ca_omci_me_get_inner_class(classId);
    if (OMCI_ME_CLASS_ID_MAX == innerClass) {
        omci_sprint(cli, "unknown classid\r\n");
        return;
    }

    meclass = ca_omci_me_get(innerClass);
    if (NULL == meclass) {
        omci_sprint(cli, "No me exist for class %d\r\n", classId);
        return;
    }

    for (meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, NULL);
         meBase; meBase = (ca_omci_mebase_t *)HASH_GetNext(meclass->instHash, meBase)) {
        if (NULL == meBase->handlers->meDisplay) {
            omci_sprint(cli, "Unsupport class %d \r\n", classId);
            return;
        }

        meBase->handlers->meDisplay(meBase, (void *)cli);
    }
}


void ca_omci_cli_me_show(
    ca_omci_uint16_t classId,
    ca_omci_uint16_t instance,
    const struct shell *cli)
{
    ca_omci_class_id_e  innerClass = OMCI_ME_CLASS_ID_MAX;
    ca_omci_meclass_t  *meclass    = NULL;
    ca_omci_mebase_t   *meBase     = NULL;

    innerClass = ca_omci_me_get_inner_class(classId);
    if (OMCI_ME_CLASS_ID_MAX == innerClass) {
        omci_sprint(cli, "unknown classid\r\n");
        return;
    }

    meclass = ca_omci_me_get(innerClass);
    if (NULL == meclass) {
        omci_sprint(cli, "No me exist for class %d\r\n", classId);
        return;
    }

    meBase = ca_omci_me_entry_lookup(innerClass, instance);
    if (NULL == meBase) {
        omci_sprint(cli, "No me exist for class %d, instance %d\r\n", classId, instance);
        return;
    }

    if (NULL != meBase->handlers->meDisplay) {
        meBase->handlers->meDisplay(meBase, (void *)cli);
        return;
    }
    omci_sprint(cli, "Unsupport class %d, instance %d\r\n", classId, instance);
}



static int omci_me_support(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci me support");
        return 1;
    }

    ca_omci_cli_me_show_support(shell);
    return 0;
}


static int omci_me_all(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "usage:omci me all");
        return 1;
    }

    ca_omci_cli_me_show_all(shell);
    return 0;
}


static int omci_me_cls(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint16_t classId  = 0;
    ca_omci_uint16_t instance = 0;

    if (argc != 3) {
        shell_print(shell, "omci me cls <classid> <instanceid|all>");
        return 1;
    }

    if (atoi(argv[1]) > 65535) {
        shell_print(shell, "class id shoule be <1-65535>");
        return 1;
    }

    classId = atoi(argv[1]);
    if (0 == OMCI_STRCMP("all", argv[2])) {
        ca_omci_cli_me_show_class_all(classId, shell);
        return 0;
    }

    if (atoi(argv[2]) > 65535) {
        shell_print(shell, "instance id shoule be <0-65535>");
        return 1;
    }

    instance = atoi(argv[2]);
    ca_omci_cli_me_show(classId, instance, shell);
    return 0;
}


static int omci_reset_onu_cmd(const struct shell *shell, size_t argc, char *argv[])
{
    ca_omci_uint8_t flag = 0;

    if (argc != 1) {
        shell_print(shell, "usage:omci reset");
        return 1;
    }

    return ca_omci_hal_onu_reboot((void *)&flag);
}


static int omci_cfg_file_set(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 3) {
        shell_print(shell, "omci cfg file set <name> <value>");
        return 1;
    }

    ca_omci_cfg_file_set_value(shell, argv[1], argv[2]);
    return 0;
}


static int omci_cfg_file_get(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 2) {
        shell_print(shell, "omci cfg file get <name>");
        return 1;
    }

    ca_omci_cfg_file_get_value(shell, argv[1]);
    return 0;
}


static int omci_cfg_file_support(const struct shell *shell, size_t argc, char *argv[])
{
    if (argc != 1) {
        shell_print(shell, "omci cfg file support");
        return 1;
    }

    ca_omci_cfg_file_support_display(shell);
    return 0;
}


SHELL_STATIC_SUBCMD_SET_CREATE(omci_cfg_file_cmd,
    SHELL_CMD(support, NULL, "omci cfg file support", omci_cfg_file_support),
    SHELL_CMD(set,     NULL, "omci cfg file set cmd", omci_cfg_file_set),
    SHELL_CMD(get,     NULL, "omci cfg file get cmd", omci_cfg_file_get),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_cfg_cmd,
    SHELL_CMD(file, &omci_cfg_file_cmd, "omci cfg file cmds", NULL),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_me_cmd,
    SHELL_CMD(all,     NULL, "omci me all",     omci_me_all),
    SHELL_CMD(support, NULL, "omci me support", omci_me_support),
    SHELL_CMD(cls,     NULL, "omci me cls",     omci_me_cls),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_all_cmd,
    SHELL_CMD(debug,   NULL,  "omci log all debug",   omci_log_all_debug),
    SHELL_CMD(warning, NULL,  "omci log all warning", omci_log_all_warn),
    SHELL_CMD(error,   NULL,  "omci log all error",   omci_log_all_err),
    SHELL_CMD(off,     NULL,  "omci log all off",     omci_log_all_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_upg_cmd,
    SHELL_CMD(debug,   NULL,  "omci log upg debug",   omci_log_upg_debug),
    SHELL_CMD(warning, NULL,  "omci log upg warning", omci_log_upg_warn),
    SHELL_CMD(error,   NULL,  "omci log upg error",   omci_log_upg_err),
    SHELL_CMD(off,     NULL,  "omci log upg off",     omci_log_upg_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_pm_cmd,
    SHELL_CMD(debug,   NULL,  "omci log pm debug",   omci_log_pm_debug),
    SHELL_CMD(warning, NULL,  "omci log pm warning", omci_log_pm_warn),
    SHELL_CMD(error,   NULL,  "omci log pm error",   omci_log_pm_err),
    SHELL_CMD(off,     NULL,  "omci log pm off",     omci_log_pm_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_alarm_cmd,
    SHELL_CMD(debug,   NULL,  "omci log alarm debug",   omci_log_alarm_debug),
    SHELL_CMD(warning, NULL,  "omci log alarm warning", omci_log_alarm_warn),
    SHELL_CMD(error,   NULL,  "omci log alarm error",   omci_log_alarm_err),
    SHELL_CMD(off,     NULL,  "omci log alarm off",     omci_log_alarm_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_hal_cmd,
    SHELL_CMD(debug,   NULL,  "omci log hal debug",   omci_log_hal_debug),
    SHELL_CMD(warning, NULL,  "omci log hal warning", omci_log_hal_warn),
    SHELL_CMD(error,   NULL,  "omci log hal error",   omci_log_hal_err),
    SHELL_CMD(off,     NULL,  "omci log hal off",     omci_log_hal_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_analysis_cmd,
    SHELL_CMD(debug,   NULL,  "omci log analysis debug",   omci_log_analysis_debug),
    SHELL_CMD(warning, NULL,  "omci log analysis warning", omci_log_analysis_warn),
    SHELL_CMD(error,   NULL,  "omci log analysis error",   omci_log_analysis_err),
    SHELL_CMD(off,     NULL,  "omci log analysis off",     omci_log_analysis_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_mng_cmd,
    SHELL_CMD(debug,   NULL,  "omci log mng debug",   omci_log_mng_debug),
    SHELL_CMD(warning, NULL,  "omci log mng warning", omci_log_mng_warn),
    SHELL_CMD(error,   NULL,  "omci log mng error",   omci_log_mng_err),
    SHELL_CMD(off,     NULL,  "omci log mng off",     omci_log_mng_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_codec_cmd,
    SHELL_CMD(debug,   NULL,  "omci log codec debug",   omci_log_codec_debug),
    SHELL_CMD(warning, NULL,  "omci log codec warning", omci_log_codec_warn),
    SHELL_CMD(error,   NULL,  "omci log codec error",   omci_log_codec_err),
    SHELL_CMD(off,     NULL,  "omci log codec off",     omci_log_codec_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_common_cmd,
    SHELL_CMD(debug,   NULL,  "omci log common debug",   omci_log_common_debug),
    SHELL_CMD(warning, NULL,  "omci log common warning", omci_log_common_warn),
    SHELL_CMD(error,   NULL,  "omci log common error",   omci_log_common_err),
    SHELL_CMD(off,     NULL,  "omci log common off",     omci_log_common_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_log_cmd,
    SHELL_CMD(common,   &omci_log_common_cmd,   "omci log common",   NULL),
    SHELL_CMD(codec,    &omci_log_codec_cmd,    "omci log codec",    NULL),
    SHELL_CMD(mng,      &omci_log_mng_cmd,      "omci log mng",      NULL),
    SHELL_CMD(analysis, &omci_log_analysis_cmd, "omci log analysis", NULL),
    SHELL_CMD(hal,      &omci_log_hal_cmd,      "omci log hal",      NULL),
    SHELL_CMD(alarm,    &omci_log_alarm_cmd,    "omci log alarm",    NULL),
    SHELL_CMD(pm,       &omci_log_pm_cmd,       "omci log pm",       NULL),
    SHELL_CMD(upg,      &omci_log_upg_cmd,      "omci log upg",      NULL),
    SHELL_CMD(all,      &omci_log_all_cmd,      "omci log all",      NULL),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_pkt_tx_cmd,
    SHELL_CMD(on,  NULL,  "omci pkt tx on",  omci_pkt_tx_on),
    SHELL_CMD(off, NULL,  "omci pkt tx off", omci_pkt_tx_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_pkt_rx_cmd,
    SHELL_CMD(on,  NULL,  "omci pkt rx on",  omci_pkt_rx_on),
    SHELL_CMD(off, NULL,  "omci pkt rx off", omci_pkt_rx_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_pkt_all_cmd,
    SHELL_CMD(on,  NULL,  "omci pkt all on",  omci_pkt_all_on),
    SHELL_CMD(off, NULL,  "omci pkt all off", omci_pkt_all_off),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_pkt_cmd,
    SHELL_CMD(tx,  &omci_pkt_tx_cmd,  "omci pkt tx debug set",  NULL),
    SHELL_CMD(rx,  &omci_pkt_rx_cmd,  "omci pkt rx debug set",  NULL),
    SHELL_CMD(all, &omci_pkt_all_cmd, "omci pkt all debug set", NULL),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_show_flow_cmd,
    SHELL_CMD(gem,       NULL, "omci show flow gem",       omci_show_flow_gem),
    SHELL_CMD(filter,    NULL, "omci show flow filter",    omci_show_flow_filter),
    SHELL_CMD(stream_us, NULL, "omci show flow stream_us", omci_show_flow_stream_us),
    SHELL_CMD(stream_ds, NULL, "omci show flow stream_ds", omci_show_flow_stream_ds),
    SHELL_CMD(cls,       NULL, "omci show flow cls",       omci_show_flow_cls),
    SHELL_CMD(map,       NULL, "omci show flow map",       omci_show_flow_map),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_show_cmd,
    SHELL_CMD(connection, NULL,                "omci show connection", omci_show_connection),
    SHELL_CMD(stream,     NULL,                "omci show stream",     omci_show_stream),
    SHELL_CMD(info,       NULL,                "omci show info",       omci_show_info),
    SHELL_CMD(flow,       &omci_show_flow_cmd, "omci show flow",       NULL),
    SHELL_CMD(upg,        NULL,                "omci show upg",        omci_show_upg),

    SHELL_SUBCMD_SET_END
);


SHELL_STATIC_SUBCMD_SET_CREATE(omci_debug_commands,
    SHELL_CMD(show,  &omci_show_cmd, "omci show cmds",          NULL),
    SHELL_CMD(pkt,   &omci_pkt_cmd,  "omci pkt debug ctrl cmd", NULL),
    SHELL_CMD(log,   &omci_log_cmd,  "omci log debug ctrl cmd", NULL),
    SHELL_CMD(me,    &omci_me_cmd,   "omci me cmds",            NULL),
    SHELL_CMD(cfg,   &omci_cfg_cmd,  "omci cfg cmds",           NULL),
    SHELL_CMD(reset, NULL,           "omci reset ONU",          omci_reset_onu_cmd),

    SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(omci, &omci_debug_commands, "OMCI commands", NULL);

