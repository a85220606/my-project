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
#include <string.h>
#include <stdlib.h>
#include "omci_hash.h"


#define NextP(h, d) *(DATA_PTR*)((char*)d + h->next_offset)
#define KeyP(h, d)  (DATA_PTR)((char*)d + h->key_offset)


HASH_TABLE *HASH_Create(unsigned int size, unsigned int keyoffset, unsigned int nextoffset)
{
    HASH_TABLE *h;

    if (!(h = (HASH_TABLE *)malloc(sizeof(HASH_TABLE)))) {
        return(NULL);
    }

    if (!(h->array.data = (void**)malloc( sizeof(DATA_PTR) * size))) {
        free(h);
        return(NULL);
    }

    memset(h->array.data, 0, size * sizeof(DATA_PTR));

    h->size = size;
    h->count = 0;
    h->resize = 1;
    h->key_offset = keyoffset;
    h->hash = (HASH_HashProc)HASH_DefaultHashFunction;
    h->lookup = (HASH_LookupProc)HASH_DefaultLookupFunction;
    h->destroy = NULL;

    h->next_offset = nextoffset;

    return(h);
}


void HASH_ChangeSize(HASH_TABLE *h, unsigned int size)
{
    unsigned int index;

    DATA_PTR *array = (void **)malloc(sizeof(DATA_PTR) * size);
    DATA_PTR moving;
    unsigned int old;
    if (!array) {
        return;
    }
    else {       
        memset(array, 0, size * sizeof(DATA_PTR));
        for (old = 0; old < h->size; old++) {
            while ((moving = h->array.data[old])) {
                h->array.data[old] = NextP(h, moving);
                index = (h->hash)(KeyP(h, moving), size);
                NextP(h, moving) = array[index];
                array[index] = moving;
            }
        }
        free(h->array.data);
        h->array.data = array;
    }
    h->size = size;
}

void HASH_ClearFn(HASH_TABLE* h, HASH_DestroyProc destroy)
{
    unsigned int index;
    DATA_PTR data;

    if (destroy) {
        for (index = 0; index < h->size; index++) {
            while ((data = h->array.data[index])) {
                h->array.data[index] = NextP(h, data);
                (destroy)(data);
            }
        }
    }

    h->count = 0;
}


void HASH_DestroyFn(HASH_TABLE* h, HASH_DestroyProc destroy)
{
    HASH_ClearFn(h, destroy);
    free(h->array.data);
    free(h);
}


DATA_PTR HASH_Insert(HASH_TABLE* h, DATA_PTR data)
{
    DATA_PTR key;
    unsigned int index;

    key = KeyP(h, data);

    if ((h->resize) && (h->count >= (h->resize)*(h->size))) {
        unsigned int new_size = h->size*2;
        HASH_ChangeSize(h, new_size);
    }

    index = (h->hash)(key, h->size);

    NextP(h, data) = h->array.data[index];
    h->array.data[index] = data;

    h->count++;

    return(data);

}

void HASH_RemoveFn(HASH_TABLE *h, DATA_PTR data, HASH_DestroyProc destroy)
{
    unsigned int index;

    index = (h->hash)(KeyP(h, data), h->size);

    DATA_PTR tmp, prev;
    /* Find Prev */
    for (tmp = h->array.data[index], prev = NULL; tmp; prev = tmp, tmp = NextP(h, tmp)) {
        if (tmp == data) break;
    }

    if (prev) { NextP(h, prev) = NextP(h, data); }
    else { h->array.data[index] = NextP(h, data); }
    if (destroy) { (destroy)(data); }

    h->count--;

}

void HASH_RemoveByKeyFn(HASH_TABLE *h, DATA_PTR key, HASH_DestroyProc destroy)
{
    unsigned int index;
    DATA_PTR data;

    index = (h->hash)(key, h->size);

    DATA_PTR prev;
    for (data = h->array.data[index], prev = NULL; data; prev = data, data = NextP(h, data)) {
        if ((h->lookup)(KeyP(h, data), key)) break; /* found it */
    }
    if (!data) return;
    if (prev) { NextP(h, prev) = NextP(h, data); }
    else { h->array.data[index] = NextP(h, data); }
    if (destroy) { (destroy)(data); }

    h->count--;

}

DATA_PTR HASH_Lookup(HASH_TABLE* h, DATA_PTR key)
{
    unsigned int index = 0;
    DATA_PTR data = NULL;

    index = (h->hash)(key, h->size);

    for (data = h->array.data[index]; data; data = NextP(h, data)) {
        if ((h->lookup)(KeyP(h, data), key)) break;
    }

    return(data);
}

DATA_PTR HASH_GetNext(HASH_TABLE *h, DATA_PTR current)
{
    unsigned int       index;
    DATA_PTR        data;
    HASH_CONTAINER *cont = NULL;

    if (!current) { /* Find first item */
        for (index = 0; ((index < h->size) && (!(h->array.data[index]))); index++);
        if (index < h->size) {
            data = h->array.data[index];
        }
        else {
            data = NULL; /* table is empty */
        }
    }
    else {
        data = NextP(h, current);
        if (!data) { /* current was last one on last_index */
            index = (h->hash)(KeyP(h, current), h->size);
            for (index++; ((index < h->size) && (!(h->array.data[index]))); index++);
            if (index < h->size) data = h->array.data[index];
            else data = NULL; /* current was last one in table */
        }
    }
    h->last_cont = cont;

    return(data);
}


unsigned int HASH_DefaultHashFunction(char *key, unsigned int size)
{
    unsigned short key1 = 0;
    memcpy(&key1, key, 2);
    return ((unsigned short)key1%size);
}


int HASH_DefaultLookupFunction(char *k1, char *k2)
{
    return (!memcmp(k1, k2, 2));
}
/*lint -restore*/

