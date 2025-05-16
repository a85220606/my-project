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
#include "omci_me_next.h"
#include "omci_mq.h"
#include "omci_timer.h"


extern ca_omci_st_e ca_omci_mq_send(ca_omci_inner_msg_t *msg);


void ca_omci_next_timer_handler(void *data)
{
    ca_omci_next_t *next = (ca_omci_next_t *)data;

    OMCI_PARAM_IS_NULL(next);

    next->timerHdr = CA_INVALID_TIMER;
    next->timeout  = CA_OMCI_TRUE;
}


void ca_omci_next_timer_timeout(void *data)
{
    ca_omci_inner_msg_t  msg;
    ca_omci_timer_def_e  timerDef = CA_OMCI_TIMER_NEXT;

    OMCI_MEMSET(&msg, 0, sizeof(msg));

    msg.hdr.msgType = OMCI_INNER_MSG_TYPE_TIMEOUT;
    msg.hdr.dataLen = sizeof(ca_omci_next_t) + sizeof(ca_omci_timer_def_e);//pdata + timer def
    OMCI_MEMCPY(msg.data, &timerDef,sizeof(ca_omci_timer_def_e)); //cp timer def
    OMCI_MEMCPY((msg.data + sizeof(ca_omci_timer_def_e)), data, sizeof(ca_omci_next_t));//cp pData
    if (CA_OMCI_OK != ca_omci_mq_send(&msg))
    {
        OMCIERR("send to inner mq error for next timer failed");
    }
}


/*when do get , call this api to set the total num*/
void ca_omci_next_init(ca_omci_next_t *next, ca_omci_uint16_t total)
{
    OMCI_PARAM_IS_NULL(next);

    next->totalSeq = total;
    next->nextSeq  = 0;

    /*stop the time if necessary*/
    if (next->timerHdr != CA_INVALID_TIMER)
    {
        ca_timer_del(next->timerHdr);
    }

    /* start timer*/
    next->timerHdr = ca_timer_add(OMCI_NEXT_TIMEOUT, ca_omci_next_timer_timeout, next);
    next->timeout  = CA_OMCI_FALSE;
}


/* delete a me, need call this*/
void ca_omci_next_finish(ca_omci_next_t *next)
{
    /* delete the timer */
    if (next->timerHdr != CA_INVALID_TIMER)
    {
        ca_timer_del(next->timerHdr);
        next->timerHdr = CA_INVALID_TIMER;
    }
}


ca_omci_bool_t ca_omci_next_valid(ca_omci_next_t *next, ca_omci_uint16_t getSeq)
{
    if (NULL == next)
    {
        return CA_OMCI_FALSE;
    }

    if (next->timeout)
    {
        return CA_OMCI_FALSE;
    }

    if (getSeq >= next->totalSeq)
    {
        return CA_OMCI_FALSE;
    }

    if (next->nextSeq != getSeq)
    {
        return CA_OMCI_FALSE;
    }

    return CA_OMCI_TRUE;
}


void ca_omci_next_success(ca_omci_next_t *next)
{
    OMCI_PARAM_IS_NULL(next);

    next->nextSeq++;

    /* delete the time and */
    if (next->timerHdr != CA_INVALID_TIMER)
    {
        ca_timer_del(next->timerHdr);
    }

    /* start timer*/
    next->timeout  = CA_OMCI_FALSE;
    next->timerHdr = ca_timer_add(OMCI_NEXT_TIMEOUT, ca_omci_next_timer_timeout, next);
}

