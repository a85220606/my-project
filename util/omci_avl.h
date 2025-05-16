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

Copyright (c) 2019 by CA Incorporated
****************************************************************************/
#ifndef __OMCI_AVL_H__
#define __OMCI_AVL_H__


#define OMCI_AVL_MAX_HEIGHT 256
#define OMCI_AVL_SUB_TREE   2


#define OMCI_AVL_COUNT(table) ((size_t) (table)->avlCount)
#define OMCI_AVL_NODE_ALLOC_SIZE_GET(treepm) (sizeof(*((treepm)->avlRoot)) + (treepm)->avlDataSize)
#define OMCI_AVL_DATA_PTR_GET(nodepm) ((void*)(((char *)(nodepm)) + (sizeof(*(nodepm)))))


typedef int   omci_avl_comparison_func(const void *avlA, const void *avlB, void *avlParam);
typedef void  omci_avl_item_func(void *avlItem, void *avlParam);
typedef void *omci_avl_copy_func(void *avlItem, void *avlParam);


struct omci_avl_allocator
{
    void *(*omci_libavl_malloc)(struct omci_avl_allocator *allocator, size_t size);
    void (*omci_libavl_free)(struct omci_avl_allocator *allocator, void *block);
};


extern struct omci_avl_allocator omci_avl_alloc_def;


typedef struct avl_node
{
    struct avl_node *avlLink[OMCI_AVL_SUB_TREE];  /* Subtrees. */
    int              avlBalance;                  /* Balance factor. */
}omci_avl_node_t;


typedef struct avl_table
{
    struct avl_node           *avlRoot;       /* Tree's root. */
    omci_avl_comparison_func  *avlCompare;    /* Comparison function. */
    void                      *avlParam;      /* Extra argument to |avl_compare|. */
    struct omci_avl_allocator *avlAlloc;      /* Memory allocator. */
    size_t                     avlCount;      /* Number of items in tree. */
    unsigned int               avlGeneration; /* Generation number. */
    unsigned int               avlDataSize;   /* Size of data in avl_node. */
}omci_avl_table_t;


typedef struct avl_traverser
{
    struct avl_table *avlTable;          /* Tree being traversed. */
    struct avl_node  *avlNode;           /* Current node in tree. */
    struct avl_node  *avlStack[OMCI_AVL_MAX_HEIGHT];
                                         /* All the nodes above |avl_node|. */
    size_t            avlHeight;         /* Number of nodes in |avl_parent|. */
    unsigned int      avlGeneration;     /* Generation number. */
}omci_avl_traverser_t;


struct avl_table *omci_avl_create(omci_avl_comparison_func *compare, void *param, unsigned int avlDataSize, struct omci_avl_allocator *allocator);
struct avl_table *omci_avl_dump(struct avl_table *tree, omci_avl_copy_func *copy, omci_avl_item_func *destroy, struct omci_avl_allocator *allocator);


void *omci_avl_malloc(struct omci_avl_allocator *allocator, size_t size);
void  omci_avl_free(struct omci_avl_allocator *allocator, void *block);
void *omci_avl_item_copy(void *avlItem, void *avlParam);
void  omci_avl_item_destroy(void *avlItem, void *avlParam);
int   omci_avl_cmp(struct avl_table *tree0, struct avl_table *tree1);
void  omci_avl_clean(struct avl_table *tree, omci_avl_item_func *destroy);
void  omci_avl_destroy(struct avl_table *tree, omci_avl_item_func *destroy);
void *omci_avl_probe(struct avl_table *tree, void *item);
void *omci_avl_insert(struct avl_table *table, void *item);
void *omci_avl_replace(struct avl_table *table, void *item);
void *omci_avl_delete(struct avl_table *tree, const void *item);
void *omci_avl_find(const struct avl_table *tree, const void *item);
void  omci_avl_t_init(struct avl_traverser *trav, struct avl_table *tree);
void *omci_avl_t_first(struct avl_traverser *trav, struct avl_table *tree);
void *omci_avl_t_last(struct avl_traverser *trav, struct avl_table *tree);
void *omci_avl_t_find(struct avl_traverser *trav, struct avl_table *tree, void *item);
void *omci_avl_t_insert(struct avl_traverser *trav, struct avl_table *tree, void *item);
void *omci_avl_t_copy(struct avl_traverser *trav, const struct avl_traverser *src);
void *omci_avl_t_next(struct avl_traverser *trav);
void *omci_avl_t_prev(struct avl_traverser *trav);
void *omci_avl_t_cur(struct avl_traverser *trav);
void *omci_avl_t_replace(struct avl_traverser *trav, void *pNew);
#endif /*__OMCI_AVL_H__*/

