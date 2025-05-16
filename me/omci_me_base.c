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
#include "omci_me_base.h"
#include "omci_codec_pkt.h"
#include "omci_me.h"


ca_omci_st_e ca_omci_linked_me_add(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe)
{
    ca_omci_me_link_t *link = NULL;

    OMCI_PARAM_NULL_RETURN(srcMe);
    OMCI_PARAM_NULL_RETURN(destMe);

    if (ca_omci_me_linked(srcMe, destMe))
    {
        return CA_OMCI_OK;
    }

    link = omci_malloc(sizeof(ca_omci_me_link_t));
    OMCI_PARAM_NULL_RETURN(link);

    link->classId    = srcMe->classId;
    link->instanceId = srcMe->instanceId;
    link->next       = destMe->linkedMeHeader; /*insert it at the header */

    destMe->linkedMeHeader = link; 
    destMe->linkedCount++;
    return CA_OMCI_OK;
}


ca_omci_st_e ca_omci_linked_me_delete(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe)
{
    ca_omci_me_link_t *link = NULL;
    ca_omci_me_link_t *prev = NULL;

    OMCI_PARAM_NULL_RETURN(srcMe);
    OMCI_PARAM_NULL_RETURN(destMe);

    link = destMe->linkedMeHeader;
    prev = link;

    while (link)
    {
        if ((link->classId == srcMe->classId) &&
            (link->instanceId == srcMe->instanceId))
        {
            if (link == destMe->linkedMeHeader)
            {
                destMe->linkedMeHeader = link->next;
            }
            else
            {
                prev->next = link->next;
            }

            OMCI_FREE(link);
            destMe->linkedCount--;
            break;
        }

        prev = link;
        link = link->next;
    }

    return CA_OMCI_OK;
}


ca_omci_bool_t ca_omci_me_linked(ca_omci_mebase_t *srcMe, ca_omci_mebase_t *destMe)
{
    ca_omci_me_link_t *link = NULL;

    if (NULL == srcMe || destMe == srcMe)
    {
        OMCIERR("NULL point");
        return CA_OMCI_FALSE;
    }

    link = destMe->linkedMeHeader;
    while (link)
    {
        if ((link->classId == srcMe->classId) && (link->instanceId == srcMe->instanceId))
        {
            return CA_OMCI_TRUE;
        }
        link = link->next;
    }
    return CA_OMCI_FALSE;
}

ca_omci_st_e ca_omci_linked_me_clear(ca_omci_mebase_t *pMe)
{
    ca_omci_me_link_t *link = NULL;

    OMCI_PARAM_NULL_RETURN(pMe);

    link = pMe->linkedMeHeader;

    while (link)
    {
        pMe->linkedMeHeader = link->next;
        OMCI_FREE(link);
        link = pMe->linkedMeHeader;
        pMe->linkedCount--;
    }

    pMe->linkedMeHeader = NULL;
    return CA_OMCI_OK;
}


ca_omci_uint8_t  ca_omci_linked_me_count_get(ca_omci_mebase_t *pMe)
{
    if (NULL == pMe)
    {
        return 0;
    }
    return pMe->linkedCount;
}

ca_omci_bool_t  ca_omci_me_null_pointer(ca_omci_uint16_t instanceId)
{
    return instanceId == 0 || instanceId == OMCI_ME_NULL_POINTER;
}

ca_omci_st_e ca_omci_me_relation_switch(
    ca_omci_mebase_t *me,
    ca_omci_uint16_t  classId,
    ca_omci_uint16_t  newInst,
    ca_omci_uint16_t  oldInst)
{
    ca_omci_mebase_t *base = NULL;

    OMCI_PARAM_NULL_RETURN(me);
    
    if (!ca_omci_me_null_pointer(oldInst))
    {
        base = ca_omci_me_entry_lookup(classId, oldInst);
        if (base)
        {
            ca_omci_linked_me_delete(me, base);
        }
    }

    base = NULL;
    if (!ca_omci_me_null_pointer(newInst))
    {
        base = ca_omci_me_entry_lookup(classId, newInst);
        if (base == NULL)
        {
            OMCIERR("Class[%d] instance[0x%x] doesn't exist", newInst, classId); 
            return CA_OMCI_E;
        }
        ca_omci_linked_me_add(me, base);
    }

    return CA_OMCI_OK;
}



void ca_omci_base_init(
    ca_omci_mebase_t     *me,
    ca_omci_uint16_t      classId,
    ca_omci_me_handler_t *handlers)
{
    OMCI_PARAM_IS_NULL(me);

    me->classId        = classId;
    me->amBmp          = NULL;
    me->arcBmp         = NULL;
    me->handlers       = handlers;
    me->linkedMeHeader = NULL;
    me->linkedCount    = 0;
    me->next           = NULL;
    me->mibUploadCnt   = 1;/*default is 1*/
}

