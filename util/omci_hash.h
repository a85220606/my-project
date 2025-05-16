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
/*
 * $Id: omci_hash.h,v 1.1 2012/12/05 01:53:26 qxu Exp $
 */
#ifndef _AAL_HASH_H_
#define _AAL_HASH_H_

#define _HASH_P(ARGS) ARGS

/*********************  Attribute Definitions **********************/
typedef void *DATA_PTR;

enum HASH_ERROR {
   HASH_UnknownErr,
   HASH_MemoryErr,
   HASH_TableCorrupted,
   HASH_BadIndex,
   HASH_BadArgument,
   HASH_NoMember
};

/*********************  Process Definitions  ***********************/
typedef void     (*HASH_ProcessProc)     _HASH_P((DATA_PTR));
typedef void     (*HASH_ProcessPlusProc) _HASH_P((DATA_PTR, DATA_PTR));
typedef unsigned int (*HASH_HashProc)        _HASH_P((DATA_PTR, unsigned int));
typedef int      (*HASH_LookupProc)      _HASH_P((DATA_PTR, DATA_PTR));
typedef void     (*HASH_DestroyProc)     _HASH_P((DATA_PTR));
typedef void     (*HASH_ErrorProc)       _HASH_P((DATA_PTR, enum HASH_ERROR, char*));
typedef void     (*HASH_PrintProc)       _HASH_P((unsigned int, DATA_PTR, DATA_PTR));

/**********************  Structure Definitions *********************/
typedef struct _hash_container {
   struct _hash_container *next;
   DATA_PTR data;
} HASH_CONTAINER;

typedef struct _hash_table {
   union {
      DATA_PTR        *data;
      HASH_CONTAINER **cont;
   } array;

   unsigned int size;
   unsigned int count;
   unsigned int resize;

   unsigned short next_offset;
   unsigned short key_offset;

   HASH_HashProc    hash;
   HASH_LookupProc  lookup;
   HASH_DestroyProc destroy;

   HASH_CONTAINER *last_cont;
} HASH_TABLE;

#define HASH_OPTIONAL_ARG ...

/*********************  Golbal Variables  **************************/
extern const char *HASH_errlist[];

/*********************  Function Definitions ***********************/
/*  Creation and Destruction */
extern HASH_TABLE*     HASH_Create                _HASH_P((unsigned int, unsigned int, unsigned int));
extern void            HASH_ChangeSize            _HASH_P((HASH_TABLE*, unsigned int));
extern void            HASH_ClearFn               _HASH_P((HASH_TABLE*, HASH_ProcessProc));
extern void            HASH_DestroyFn             _HASH_P((HASH_TABLE*, HASH_ProcessProc));

/*  Insertion and Removal  */
extern DATA_PTR        HASH_Insert                _HASH_P((HASH_TABLE*, DATA_PTR));
extern void            HASH_RemoveFn              _HASH_P((HASH_TABLE*, DATA_PTR, HASH_DestroyProc));
extern void            HASH_RemoveByKeyFn         _HASH_P((HASH_TABLE*, DATA_PTR, HASH_DestroyProc));

/*  Lookup  */
extern DATA_PTR        HASH_Lookup                _HASH_P((HASH_TABLE*, DATA_PTR));

/*  iteration support */
extern DATA_PTR        HASH_GetNext               _HASH_P((HASH_TABLE*, DATA_PTR));

/* Default Functions */
extern unsigned        HASH_DefaultHashFunction   _HASH_P((char*, unsigned int));
extern int             HASH_DefaultLookupFunction _HASH_P((char*, char*));

/*********************  Macros  *******************/
#define HASH_Clear(h)               HASH_ClearFn(h, (h)->destroy)
#define HASH_Destroy(h)             HASH_DestroyFn(h, (h)->destroy)
#define HASH_Remove(h, d)           HASH_RemoveFn(h, d, (h)->destroy)
#define HASH_RemoveByKey(h, d)      HASH_RemoveByKeyFn(h, d, (h)->destroy)

#define HASH_SetAttribute(h, a, v)  HASH_SetAttributes(h, a, v, NULL);
#define HASH_GetAttribute(h, a, v)  HASH_GetAttributes(h, a, v, NULL);

#define HASH_Offset(a, b) (((char*)b > (char*)a) ? (char*)b - (char*)a : (char*)a - (char*)b)

#define HASH_GetSize(h)   ((h)->size)
#define HASH_GetCount(h)  ((h)->count)

#define HASH_IterateCast(h, d, type)    for (d = (type) HASH_GetNext(h, NULL); d; d = (type) HASH_GetNext(h, d))

#define HASH_Iterate(h, d) for (d = HASH_GetNext(h, NULL); d; d = HASH_GetNext(h, d))

#define HASH_IterateNext(h, d) for (d = HASH_GetNext(h, d); d; d = HASH_GetNext(h, d))

#undef HASH_OPTIONAL_ARG
#undef _HASH_P

#endif

