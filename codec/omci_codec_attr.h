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
#ifndef __OMCI_CODEC_ATTR_H__
#define __OMCI_CODEC_ATTR_H__


#define OMCI_MASK_ATTR_EXIST(i, a) ((a) & (1 << (16 - (i))))
#define OMCI_MASK_ATTR_SET(i, a)   ((a) = (a) | ((1 << (16 - i))))


#define OMCI_ME_LINK(src, dst)\
     if (NULL != dst)\
        {\
            ca_omci_linked_me_add((ca_omci_mebase_t *)(src), (ca_omci_mebase_t *)(dst));\
        }


#define OMCI_CHECK_LINK_ME_SW(me, classId, attrName, idx)\
     if (CA_OMCI_OK != ca_omci_me_relation_switch((ca_omci_mebase_t *)me, classId, meAttr->attrName, me->attr.attrName))\
        {\
            result->result = OMCI_MSG_RST_ATTR_FAIL;\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## idx, result->exeMask);\
            return CA_OMCI_E_PARAM;\
        }

#define OMCI_RM_LINK_ME(me, dstMe)\
     if ((NULL != me) && (NULL != dstMe))\
        {\
            ca_omci_linked_me_delete((ca_omci_mebase_t *)me, dstMe);\
        }

#define OMCI_UPDATE_ATTR(i, attrName)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## i, mask)) \
        {\
            me->attr.attrName = meAttr->attrName;\
        }

#define OMCI_GET_REAL_TIME_ATTR(i, attrName, meName)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## i, inMask))\
        {\
            meName->attr.attrName = attrV.attrName;\
        }


#define OMCI_UPDATE_PM(attrName)\
     do {\
            me->preAttr.attrName = me->attr.attrName;\
            me->attr.attrName    = attr.attrName;\
        }while(0)


#define OMCI_CHECK_ATTR_RANGE(attrDesc, min, max, attrIdx)\
     if (attrDesc < min || attrDesc > max)\
        {\
            result->result = OMCI_MSG_RST_PARAM_ERR;\
            OMCI_MASK_ATTR_SET(attrIdx, result->exeMask);\
            return CA_OMCI_E_PARAM;\
        }


#define OMCI_CHECK_ATTR_MASK(fullSetMask)\
     do {\
            ca_omci_uint16_t mask    = 0;\
            ca_omci_uint16_t oriMask = fullSetMask;\
            mask = (~(oriMask)) & inMask;\
            if (0 != mask)\
            {\
                result->optMask = mask;\
                result->result  = OMCI_MSG_RST_ATTR_FAIL;\
                OMCICOERR("unsupport attribute mask:[0x%x]\n", mask);\
                return CA_OMCI_E_PARAM;\
            }\
        }while(0)


#define OMCI_GET_MIBCNT_NUM(obj)\
     do {\
            ca_omci_uint32_t attrLen = ca_omci_me_## obj ##_attr_len_get();\
            if (0 == attrLen % OMCI_BASE_MIB_UPLOAD_NEXT_LEN_MAX)\
            {\
                me->meBase.mibUploadCnt = attrLen / OMCI_BASE_MIB_UPLOAD_NEXT_LEN_MAX;\
            }\
            else\
            {\
                me->meBase.mibUploadCnt = attrLen / OMCI_BASE_MIB_UPLOAD_NEXT_LEN_MAX + 1;\
            }\
        }while(0)


/* the final 2 bytes is mask used  for extern message, the first 2 byte is the length*/
#define OMCI_UPLOAD_HDR_ENCODE(obj)\
     do {\
            ca_omci_uint16_t tmp = 0;\
            ca_omci_uint16_t standardCls = ca_omci_me_get_standard_class(obj->meBase.classId);\
            if (CA_OMCI_TRUE == ext)\
            {\
                offset += sizeof(ca_omci_uint16_t);\
            }\
            tmp = ca_omci_ntohs(standardCls);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset += sizeof(ca_omci_uint16_t);\
            tmp = ca_omci_ntohs(obj->meBase.instanceId);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset += sizeof(ca_omci_uint16_t);\
            offset += sizeof(ca_omci_uint16_t);\
        }while(0)


/* the 4 bytes is class and instance used  for extern messge, save the attribue len*/
#define OMCI_UPLOAD_MASK_ENCODE\
     do {\
            ca_omci_uint16_t tmp   = ca_omci_ntohs(mask);\
            ca_omci_uint16_t meLen = ca_omci_ntohs(*contLen);\
            if (CA_OMCI_TRUE == ext)\
            {\
                OMCI_MEMCPY((contents + 6), &tmp, sizeof(ca_omci_uint16_t));\
                OMCI_MEMCPY(contents, &meLen, sizeof(ca_omci_uint16_t));\
            }\
            else\
            {\
                OMCI_MEMCPY((contents + 4), &tmp, sizeof(ca_omci_uint16_t));\
            }\
        }while(0)

#define OMCI_BASIC_UPLOAD_MASK_ENCODE\
      do {\
             ca_omci_uint16_t tmp = ca_omci_ntohs(mask);\
             OMCI_MEMCPY((contents + 4), &tmp, sizeof(ca_omci_uint16_t));\
         }while(0)


#define OMCI_UPLOAD_ATTR_UINT8(attrDesc, x)\
     do {\
            OMCI_MEMCPY((contents + offset), &attrDesc, sizeof(attrDesc));\
            offset  += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
            *contLen += sizeof(attrDesc);\
        }while(0)


#define OMCI_UPLOAD_ATTR_UINT16(attrDesc, x)\
     do {\
            ca_omci_uint16_t tmp = ca_omci_htons(attrDesc);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset  += sizeof(ca_omci_uint16_t);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
            *contLen += sizeof(ca_omci_uint16_t);\
        }while(0)


#define OMCI_UPLOAD_ATTR_UINT32(attrDesc, x)\
     do {\
            ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint32_t));\
            offset  += sizeof(ca_omci_uint32_t);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
            *contLen += sizeof(ca_omci_uint32_t);\
        }while(0)


#define OMCI_UPLOAD_ATTR_UINT64(attrDesc, x)\
     do {\
            ca_omci_uint64_t tmp = ca_omci_htonll(attrDesc);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint32_t));\
            offset  += sizeof(ca_omci_uint64_t);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
            *contLen += sizeof(ca_omci_uint64_t);\
        }while(0)


#define OMCI_UPLOAD_ATTR_UINT16_T(attrDesc, size, x)\
     do {\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint16_t tmp = ca_omci_htons(attrDesc[idx]);\
                OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
                offset  += sizeof(ca_omci_uint16_t);\
                *contLen += sizeof(ca_omci_uint16_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }while(0)
     
     
#define OMCI_UPLOAD_ATTR_UINT32_T(attrDesc, size, x)\
     do {\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc[idx]);\
                OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                *contLen += sizeof(ca_omci_uint32_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }while(0)
     
     
#define OMCI_UPLOAD_ATTR_UINT64_T(attrDesc, size, x)\
     do {\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint32_t tmp = ca_omci_htonll(attrDesc[idx]);\
                OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint64_t));\
                offset += sizeof(ca_omci_uint64_t);\
                *contLen += sizeof(ca_omci_uint64_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }while(0)

 
#define OMCI_UPLOAD_ATTR_SIZE(i, attrDesc, x)\
        OMCI_UPLOAD_ATTR_UINT ## i (attrDesc, x);\


#define OMCI_UPLOAD_ATTR_SIZE_T(i, attrDesc, size, x)\
        OMCI_UPLOAD_ATTR_UINT ## i ## _T(attrDesc, size, x);\


#define OMCI_UPLOAD_TBL_UINT8(attrDesc)\
     do {\
            OMCI_MEMCPY((contents + offset), &attrDesc, sizeof(attrDesc));\
            offset  += sizeof(attrDesc);\
            *contLen += sizeof(attrDesc);\
        }while(0)


#define OMCI_UPLOAD_TBL_UINT16(attrDesc)\
     do {\
            ca_omci_uint16_t tmp = ca_omci_htons(attrDesc);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset  += sizeof(ca_omci_uint16_t);\
            *contLen += sizeof(ca_omci_uint16_t);\
        }while(0)


#define OMCI_UPLOAD_TBL_UINT32(attrDesc)\
     do {\
            ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc);\
            OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint32_t));\
            offset  += sizeof(ca_omci_uint32_t);\
           *contLen += sizeof(ca_omci_uint32_t);\
        }while(0)


#define OMCI_UPLOAD_TBL_UINT16_T(attrDesc, size)\
     do {\
            for (tblIdx = 0; tblIdx < size; tblIdx++)\
            {\
                ca_omci_uint16_t tmp = ca_omci_htons(attrDesc[tblIdx]);\
                OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint16_t));\
                offset  += sizeof(ca_omci_uint16_t);\
                *contLen += sizeof(ca_omci_uint16_t);\
            }\
        }while(0)


#define OMCI_UPLOAD_TBL_UINT32_T(attrDesc, size)\
     do {\
            for (tblIdx = 0; tblIdx < size; idx++)\
            {\
                ca_omci_uint16_t tmp = ca_omci_htonl(attrDesc[tblIdx]);\
                OMCI_MEMCPY((contents + offset), &tmp, sizeof(ca_omci_uint32_t));\
                offset  += sizeof(ca_omci_uint32_t);\
                *contLen += sizeof(ca_omci_uint32_t);\
            }\
        }while(0)


#define OMCI_SET_ATTR_UINT8(attrDesc, name, x)\
    do {\
           OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
           offset += sizeof(attrDesc);\
           OMCICODBG("attr %s = 0x%x", name, attrDesc);\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT16(attrDesc, name, x)\
    do {\
           OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
           offset   += sizeof(attrDesc);\
           attrDesc  = ca_omci_ntohs(attrDesc);\
           OMCICODBG("attr %s = 0x%x", name, attrDesc);\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT32(attrDesc, name, x)\
    do {\
           OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
           offset   += sizeof(attrDesc);\
           attrDesc  = ca_omci_ntohl(attrDesc);\
           OMCICODBG("attr %s = 0x%x", name, attrDesc);\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT64(attrDesc, name, x)\
    do {\
           OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
           offset   += sizeof(attrDesc);\
           attrDesc  = ca_omci_ntohll(attrDesc);\
           OMCICODBG("attr %s = %lld", name, attrDesc);\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT8_T(attrDesc, name, size, x)\
    do {\
           OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
           offset += sizeof(attrDesc);\
           for (idx = 0; idx < size; idx++)\
           {\
               OMCICODBG("attr %s[%d] = 0x%x", name, idx, attrDesc[idx]);\
           }\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT16_T(attrDesc, name, size, x)\
    do {\
           OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
           offset += sizeof(attrDesc);\
           for (idx = 0; idx < size; idx++)\
           {\
               attrDesc[idx] = ca_omci_ntohs(attrDesc[idx]);\
               OMCICODBG("attr %s = 0x%x", name, attrDesc[idx]);\
           }\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT32_T(attrDesc, name, size, x)\
    do {\
           OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
           offset += sizeof(attrDesc);\
           for (idx = 0; idx < size; idx++)\
           {\
               attrDesc[idx] = ca_omci_ntohl(attrDesc[idx]);\
               OMCICODBG("attr %s = 0x%x", name, attrDesc[idx]);\
           }\
           OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }while(0)


#define OMCI_SET_ATTR_UINT64_T(attrDesc, name, size, x)\
     do {\
            OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            for (idx = 0; idx < size; idx++)\
            {\
                attrDesc[idx] = ca_omci_ntohll(attrDesc[idx]);\
                OMCICODBG("attr %s = 0x%x", name, attrDesc[idx]);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }while(0)


#define OMCI_SET_TBL_UINT8(attrDesc, name)\
     do {\
            OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            OMCICODBG("tbl attr %s = 0x%x", name, attrDesc);\
        }while(0)


#define OMCI_SET_TBL_UINT16(attrDesc, name)\
     do {\
            OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            attrDesc    = ca_omci_ntohs(attrDesc);\
            OMCICODBG("tbl attr %s = 0x%x", name, attrDesc);\
        }while(0)
            
            
#define OMCI_SET_TBL_UINT32(attrDesc, name)\
     do {\
            OMCI_MEMCPY(&attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            attrDesc    = ca_omci_ntohl(attrDesc);\
            OMCICODBG("attr %s = 0x%x", name, attrDesc);\
        }while(0)


#define OMCI_SET_TBL_UINT8_T(attrDesc, name, size)\
     do {\
            OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            for (idx = 0; idx < size; idx++)\
            {\
                OMCICODBG("tbl attr %s = 0x%x", name, attrDesc[idx]);\
            }\
        }while(0)
     
     
#define OMCI_SET_TBL_UINT16_T(attrDesc, name, size)\
     do {\
            OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            for (idx = 0; idx < size; idx++)\
            {\
                attrDesc[idx] = ca_omci_ntohs(attrDesc[idx]);\
                OMCICODBG("attr %s = 0x%x", name, attrDesc[idx]);\
            }\
        }while(0)
     
     
#define OMCI_SET_TBL_UINT32_T(attrDesc, name, size)\
     do {\
            OMCI_MEMCPY(attrDesc, (attr + offset), sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            for (idx = 0; idx < size; idx++)\
            {\
                attrDesc[idx] = ca_omci_ntohl(attrDesc[idx]);\
                OMCICODBG("attr %s = 0x%x", name, attrDesc[idx]);\
            }\
        }while(0)


#define OMCI_UPDATE_ATTR_UINT(i, attrDesc, name, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            OMCI_SET_ATTR_UINT ## i (attrDesc, name, x);\
        }\


#define OMCI_UPDATE_ATTR_UINT_T(i, attrDesc, name, size, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
             OMCI_SET_ATTR_UINT ## i ##_T(attrDesc, name, size, x);\
        }\


#define OMCI_GET_ATTR_UINT8(attrDesc, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            OMCI_MEMCPY((attr + offset), &attrDesc, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT16(attrDesc, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint16_t tmp = ca_omci_htons(attrDesc);\
            OMCI_MEMCPY((attr + offset), &tmp, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT32(attrDesc, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc);\
            OMCI_MEMCPY((attr + offset), &tmp, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT64(attrDesc, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint64_t tmp = ca_omci_htonll(attrDesc);\
            OMCI_MEMCPY((attr + offset), &tmp, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT8_T(attrDesc, size, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint32_t attrLen = size;\
            if ((CA_OMCI_TRUE != ext) && (size > OMCI_MAX_GET_LEN))\
            {\
                OMCI_MEMCPY((attr + offset), &attrLen, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
                result->mask    = mask;\
                result->attrLen = size;\
                result->exeMask = 0;\
                result->result = OMCI_MSG_RST_SUCCESSFUL;\
                return CA_OMCI_OK;\
            }\
            OMCI_MEMCPY((attr + offset), attrDesc, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT16_T(attrDesc, size, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint32_t attrLen = sizeof(ca_omci_uint16_t) * size;\
            if ((CA_OMCI_TRUE != ext) && (attrLen > OMCI_MAX_GET_LEN))\
            {\
                OMCI_MEMCPY((attr + offset), &attrLen, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
                result->mask    = mask;\
                result->attrLen = attrLen;\
                result->exeMask = 0;\
                result->result = OMCI_MSG_RST_SUCCESSFUL;\
                return CA_OMCI_OK;\
            }\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint16_t tmp = ca_omci_htons(attrDesc[idx]);\
                OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint16_t));\
                offset += sizeof(ca_omci_uint16_t);\
                result->attrLen += sizeof(ca_omci_uint16_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
        }


#define OMCI_GET_ATTR_UINT32_T(attrDesc, size, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint32_t attrLen = sizeof(ca_omci_uint32_t) * size;\
            if ((CA_OMCI_TRUE != ext) && (attrLen > OMCI_MAX_GET_LEN))\
            {\
                OMCI_MEMCPY((attr + offset), &attrLen, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
                result->mask    = mask;\
                result->attrLen = attrLen;\
                result->exeMask = 0;\
                result->result = OMCI_MSG_RST_SUCCESSFUL;\
                return CA_OMCI_OK;\
            }\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc[idx]);\
                OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                result->attrLen += sizeof(ca_omci_uint32_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }


#define OMCI_GET_ATTR_UINT64_T(attrDesc, size, x)\
     if (OMCI_MASK_ATTR_EXIST(OMCI_ME_ATTR ## x, inMask)) \
        {\
            ca_omci_uint32_t attrLen = sizeof(ca_omci_uint64_t) * size;\
            if ((CA_OMCI_TRUE != ext) && (attrLen > OMCI_MAX_GET_LEN))\
            {\
                OMCI_MEMCPY((attr + offset), &attrLen, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
                result->mask    = mask;\
                result->attrLen = attrLen;\
                result->exeMask = 0;\
                result->result = OMCI_MSG_RST_SUCCESSFUL;\
                return CA_OMCI_OK;\
            }\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint64_t tmp = ca_omci_htonll(attrDesc[idx]);\
                OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint64_t));\
                offset += sizeof(ca_omci_uint64_t);\
                result->attrLen += sizeof(ca_omci_uint64_t);\
            }\
            OMCI_MASK_ATTR_SET(OMCI_ME_ATTR ## x, mask);\
       }


#define OMCI_GET_TBL_UINT8(attrDesc)\
     do {\
            OMCI_MEMCPY((attr + offset), &attrDesc, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
            result->attrLen += sizeof(attrDesc);\
        }while(0)


#define OMCI_GET_TBL_UINT16(attrDesc)\
     do {\
            ca_omci_uint16_t tmp = ca_omci_htons(attrDesc);\
            OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset += sizeof(ca_omci_uint16_t);\
            result->attrLen += sizeof(ca_omci_uint16_t);\
        }while(0)


#define OMCI_GET_TBL_UINT32(attrDesc)\
     do {\
            ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc);\
            OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint32_t ));\
            offset += sizeof(ca_omci_uint32_t );\
            result->attrLen += sizeof(ca_omci_uint32_t);\
        }while(0)


#define OMCI_GET_TBL_UINT16_T(attrDesc, size)\
     do {\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint16_t tmp = ca_omci_htons(attrDesc[idx]);\
                OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint16_t));\
                offset += sizeof(ca_omci_uint16_t);\
                result->attrLen += sizeof(ca_omci_uint16_t);\
            }\
        }while(0)
          
          
#define OMCI_GET_TBL_UINT32_T(attrDesc, size)\
     do {\
            for (idx = 0; idx < size; idx++)\
            {\
                ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc[idx]);\
                OMCI_MEMCPY((attr + offset), &tmp, sizeof(ca_omci_uint32_t));\
                offset += sizeof(ca_omci_uint32_t);\
                result->attrLen += sizeof(ca_omci_uint32_t);\
            }\
        }while(0)


#define OMCI_CP_TBL_UINT8(attrDesc)\
     do {\
            OMCI_MEMCPY((tblAttr + offset), &attrDesc, sizeof(attrDesc));\
            offset += sizeof(attrDesc);\
        }while(0)


#define OMCI_CP_TBL_UINT16(attrDesc)\
     do {\
            ca_omci_uint16_t tmp = ca_omci_htons(attrDesc);\
            OMCI_MEMCPY((tblAttr + offset), &tmp, sizeof(ca_omci_uint16_t));\
            offset += sizeof(ca_omci_uint16_t);\
        }while(0)


#define OMCI_CP_TBL_UINT32(attrDesc)\
     do {\
            ca_omci_uint32_t tmp = ca_omci_htonl(attrDesc);\
            OMCI_MEMCPY((tblAttr + offset), &tmp, sizeof(ca_omci_uint32_t ));\
            offset += sizeof(ca_omci_uint32_t );\
        }while(0)


#define OMCI_MIB_FILE_SAVE(p, len) \
    do {\
            ca_omci_uint32_t writeLen = fwrite((p), 1, (len), fileP);\
            if (NULL != p)\
            {\
                OMCI_FREE(p);\
            }\
            if (len != writeLen)\
            {\
                OMCICOERR("save mib file for me inner class %d, instance %d", me->classId, me->instanceId);\
                return CA_OMCI_E;\
            }\
        }while(0)

#define OMCI_MIB_FILE_READ(length) \
     if ((length) != fread(pVal, 1, (length), fileP))\
        {\
            OMCICOERR("read mib file for me inner class %d, instance %d", me->classId, me->instanceId);\
            if (NULL != pVal)\
            {\
                OMCI_FREE(pVal);\
            }\
            return CA_OMCI_E;\
        }\
        *len -= length;\



#endif /* __OMCI_CODEC_ATTR_H__ */

