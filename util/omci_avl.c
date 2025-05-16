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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "omci_common.h"
#include "omci_avl.h"


struct omci_avl_allocator sg_avl_alloc_def =
{
    omci_avl_malloc,
    omci_avl_free
};


struct avl_table *omci_avl_create(
    omci_avl_comparison_func  *compare,
    void                      *param,
    unsigned int               avlDataSize,
    struct omci_avl_allocator *allocator)
{
    struct avl_table *tree = NULL;

    if (NULL == compare)
    {
        return NULL;
    }

    if (allocator == NULL)
    {
        allocator = &sg_avl_alloc_def;
    }

    tree = allocator->omci_libavl_malloc(allocator, sizeof *tree);
    if (tree == NULL)
    {
        return NULL;
    }

    tree->avlRoot       = NULL;
    tree->avlCompare    = compare;
    tree->avlParam      = param;
    tree->avlAlloc      = allocator;
    tree->avlCount      = 0;
    tree->avlGeneration = 0;
    tree->avlDataSize   = avlDataSize;
    return tree;
}

struct avl_table *omci_avl_dump(
    struct avl_table          *tree,
    omci_avl_copy_func        *copy,
    omci_avl_item_func        *destroy,
    struct omci_avl_allocator *allocator)
{
    omci_avl_traverser_t  traverser = {0};
    struct avl_table     *treeNew   = NULL;
    void                 *item      = NULL;
    void                 *p         = NULL;

    (void)copy;

    if (NULL == tree)
    {
        return NULL;
    }

    if (allocator == NULL)
    {
        allocator = &sg_avl_alloc_def;
    }

    treeNew = allocator->omci_libavl_malloc(allocator, sizeof(*treeNew));
    if (treeNew == NULL)
    {
        return NULL;
    }

    treeNew->avlRoot       = NULL;
    treeNew->avlCompare    = tree->avlCompare;
    treeNew->avlParam      = tree->avlParam;
    treeNew->avlAlloc      = allocator;
    treeNew->avlCount      = 0;
    treeNew->avlGeneration = 0;
    treeNew->avlDataSize   = tree->avlDataSize;

    for (item = omci_avl_t_first(&traverser, tree); item; item = omci_avl_t_next(&traverser))
    {
        p = omci_avl_insert(treeNew, item);
        if (p == NULL)
        {
            omci_avl_destroy(treeNew, destroy);
            return NULL;
        }
    }

    return treeNew;
}


int omci_avl_cmp(struct avl_table *tree0, struct avl_table *tree1)
{
    omci_avl_traverser_t  traverser0 = {0};
    omci_avl_traverser_t  traverser1 = {0};
    void                 *item0      = NULL;
    void                 *item1      = NULL;
    int                   cmp        = 0;

    if ((NULL == tree0) || (NULL == tree1))
    {
        return -1;
    }

    if (tree0->avlCompare != tree1->avlCompare
     || tree0->avlParam != tree1->avlParam
     || tree0->avlAlloc->omci_libavl_malloc != tree1->avlAlloc->omci_libavl_malloc
     || tree0->avlAlloc->omci_libavl_free != tree1->avlAlloc->omci_libavl_free
     || tree0->avlCount != tree1->avlCount)
    {
        return -1;
    }

    for (item0 = omci_avl_t_first(&traverser0, tree0), item1 = omci_avl_t_first(&traverser1, tree1);
         item0 && item1;
         item0 = omci_avl_t_next(&traverser0), item1 = omci_avl_t_next(&traverser1))
    {
        cmp = 0;
        cmp = tree0->avlCompare(item0, item1, tree0->avlParam);
        if (cmp != 0)
        {
            return cmp;
        }
    }

    return 0;
}


void *omci_avl_find(const struct avl_table *tree, const void *item)
{
    const struct avl_node *p = NULL;

    if ((NULL == tree) || (NULL == item))
    {
        return NULL;
    }

    for (p = tree->avlRoot; p != NULL; )
    {
        void *datap = NULL;
        int   cmp   = 0;

        datap = OMCI_AVL_DATA_PTR_GET(p);
        cmp   = tree->avlCompare(item, datap, tree->avlParam);

        if (cmp < 0)
        {
            p = p->avlLink[0];
        }
        else if (cmp > 0)
        {
            p = p->avlLink[1];
        }
        else /* |cmp == 0| */
        {
            return (datap);
        }
    }

    return NULL;
}


void *omci_avl_probe(struct avl_table *tree, void *item)
{
    struct avl_node *y = NULL; /* Top node to update balance factor, and parent. */
    struct avl_node *z = NULL; /* Top node to update balance factor, and parent. */
    struct avl_node *p = NULL; /* Iterator, and parent. */
    struct avl_node *q = NULL; /* Iterator, and parent. */
    struct avl_node *n = NULL; /* Newly inserted node. */
    struct avl_node *w = NULL; /* New root of rebalanced subtree. */
    int            dir   = 0;  /* Direction to descend. */
    void          *datap = NULL;
    int            cmp   = 0;
    int            k     = 0;  /* Number of cached results. */
    unsigned char  da[OMCI_AVL_MAX_HEIGHT] = {0}; /* Cached comparison results. */

    if ((NULL == tree) || (NULL == item))
    {
        return NULL;
    }

    z   = (struct avl_node *)&tree->avlRoot;
    y   = tree->avlRoot;
    dir = 0;

    for (q = z, p = y; p != NULL; q = p, p = p->avlLink[dir])
    {
        datap = OMCI_AVL_DATA_PTR_GET(p);
        cmp   = tree->avlCompare(item, datap, tree->avlParam);
        if (cmp == 0)
        {
            return (datap);
        }

        if (p->avlBalance != 0)
        {
            z = q, y = p, k = 0;
        }

        da[k++] = dir = cmp > 0;
    }

    n = q->avlLink[dir] = tree->avlAlloc->omci_libavl_malloc(tree->avlAlloc, OMCI_AVL_NODE_ALLOC_SIZE_GET(tree));
    if (n == NULL)
    {
        return NULL;
    }

    tree->avlCount++;
    datap = OMCI_AVL_DATA_PTR_GET(n);
    memcpy(datap, item, tree->avlDataSize);
    n->avlLink[0] = n->avlLink[1] = NULL;
    n->avlBalance = 0;
    if (y == NULL)
    {
        return (datap);
    }

    for (p = y, k = 0; p != n; p = p->avlLink[da[k]], k++)
    {
        if (da[k] == 0)
        {
            p->avlBalance--;
        }
        else
        {
            p->avlBalance++;
        }
    }

    if (y->avlBalance == -2)
    {
        struct avl_node *x = y->avlLink[0];

        if (x->avlBalance == -1)
        {
            w = x;
            y->avlLink[0] = x->avlLink[1];
            x->avlLink[1] = y;
            x->avlBalance = y->avlBalance = 0;
        }
        else
        {
            w = x->avlLink[1];
            x->avlLink[1] = w->avlLink[0];
            w->avlLink[0] = x;
            y->avlLink[0] = w->avlLink[1];
            w->avlLink[1] = y;

            if (w->avlBalance == -1)
            {
                x->avlBalance = 0, y->avlBalance = +1;
            }
            else if (w->avlBalance == 0)
            {
                x->avlBalance = y->avlBalance = 0;
            }
            else /* |w->avl_balance == +1| */
            {
                x->avlBalance = -1, y->avlBalance = 0;
            }

            w->avlBalance = 0;
        }
    }
    else if (y->avlBalance == +2)
    {
        struct avl_node *x = y->avlLink[1];

        if (NULL != x)
        {
            if (x->avlBalance == +1)
            {
                w = x;
                y->avlLink[1] = x->avlLink[0];
                x->avlLink[0] = y;
                x->avlBalance = y->avlBalance = 0;
            }
            else
            {
                w = x->avlLink[0];
                x->avlLink[0] = w->avlLink[1];
                w->avlLink[1] = x;
                y->avlLink[1] = w->avlLink[0];
                w->avlLink[0] = y;
                if (w->avlBalance == +1)
                {
                    x->avlBalance = 0, y->avlBalance = -1;
                }
                else if (w->avlBalance == 0)
                {
                    x->avlBalance = y->avlBalance = 0;
                }
                else /* |w->avlBalance == -1| */
                {
                    x->avlBalance = +1, y->avlBalance = 0;
                }
                w->avlBalance = 0;
            }
        }
    }
    else
    {
        return datap;
    }

    z->avlLink[y != z->avlLink[0]] = w;

    tree->avlGeneration++;
    return datap;
}


void *omci_avl_insert(struct avl_table *table, void *item)
{
    return omci_avl_probe(table, item);
}


void *omci_avl_replace(struct avl_table *table, void *item)
{
    void *p = omci_avl_probe(table, item);

    if (p != NULL)
    {
        memcpy(p, item, table->avlDataSize);
    }

    return p;
}


void *omci_avl_delete(struct avl_table *tree, const void *item)
{
    struct avl_node *pa[OMCI_AVL_MAX_HEIGHT]; /* Nodes. */
    unsigned char    da[OMCI_AVL_MAX_HEIGHT]; /* |avlLink[]| indexes. */
    int              k     = 0;               /* Stack pointer. */
    void            *datap = NULL;
    struct avl_node *p     = NULL;            /* Traverses tree to find node to delete. */
    int              cmp   = 0;               /* Result of comparison between |item| and |p|. */
    int              dir   = 0;

    if ((NULL == tree) || (NULL == item))
    {
        return NULL;
    }

    k = 0;
    p = (struct avl_node *)&tree->avlRoot;
    for (cmp = -1; cmp != 0; cmp = tree->avlCompare (item, datap, tree->avlParam))
    {
        dir     = cmp > 0;
        pa[k]   = p;
        da[k++] = dir;
        p       = p->avlLink[dir];

        if (p == NULL)
        {
            return NULL;
        }

        datap = OMCI_AVL_DATA_PTR_GET(p);
    }

    if (p->avlLink[1] == NULL)
    {
        pa[k - 1]->avlLink[da[k - 1]] = p->avlLink[0];
    }
    else
    {
        struct avl_node *r = p->avlLink[1];

        if (r->avlLink[0] == NULL)
        {
            r->avlLink[0] = p->avlLink[0];
            r->avlBalance = p->avlBalance;
            pa[k - 1]->avlLink[da[k - 1]] = r;
            da[k]   = 1;
            pa[k++] = r;
        }
        else
        {
            struct avl_node *s = NULL;
            int j = k++;

            for (;;)
            {
                da[k] = 0;
                pa[k++] = r;
                s = r->avlLink[0];
                if (s->avlLink[0] == NULL)
                {
                    break;
                }

                r = s;
            }

            s->avlLink[0] = p->avlLink[0];
            r->avlLink[0] = s->avlLink[1];
            s->avlLink[1] = p->avlLink[1];
            s->avlBalance = p->avlBalance;

            pa[j - 1]->avlLink[da[j - 1]] = s;
            da[j] = 1;
            pa[j] = s;
        }
    }

    tree->avlAlloc->omci_libavl_free(tree->avlAlloc, p);

    while (--k > 0)
    {
        struct avl_node *y = pa[k];

        if (da[k] == 0)
        {
            y->avlBalance++;
            if (y->avlBalance == +1)
            {
                break;
            }
            else if (y->avlBalance == +2)
            {
                struct avl_node *x = y->avlLink[1];

                if (NULL != x && x->avlBalance == -1)
                {
                    struct avl_node *w = NULL;

                    w = x->avlLink[0];
                    x->avlLink[0] = w->avlLink[1];
                    w->avlLink[1] = x;
                    y->avlLink[1] = w->avlLink[0];
                    w->avlLink[0] = y;

                    if (w->avlBalance == +1)
                    {
                        x->avlBalance = 0, y->avlBalance = -1;
                    }
                    else if (w->avlBalance == 0)
                    {
                        x->avlBalance = y->avlBalance = 0;
                    }
                    else /* |w->avlBalance == -1| */
                    {
                        x->avlBalance = +1, y->avlBalance = 0;
                    }

                    w->avlBalance = 0;
                    pa[k - 1]->avlLink[da[k - 1]] = w;
                }
                else if (NULL != x)
                {
                    y->avlLink[1] = x->avlLink[0];
                    x->avlLink[0] = y;
                    pa[k - 1]->avlLink[da[k - 1]] = x;
                    if (x->avlBalance == 0)
                    {
                        x->avlBalance = -1;
                        y->avlBalance = +1;
                        break;
                    }
                    else
                    {
                        x->avlBalance = y->avlBalance = 0;
                    }
                }
            }
        }
        else
        {
            y->avlBalance--;
            if (y->avlBalance == -1)
            {
                break;
            }
            else if (y->avlBalance == -2)
            {
                struct avl_node *x = y->avlLink[0];

                if (x->avlBalance == +1)
                {
                    struct avl_node *w = NULL;

                    w = x->avlLink[1];
                    x->avlLink[1] = w->avlLink[0];
                    w->avlLink[0] = x;
                    y->avlLink[0] = w->avlLink[1];
                    w->avlLink[1] = y;

                    if (w->avlBalance == -1)
                    {
                        x->avlBalance = 0, y->avlBalance = +1;
                    }
                    else if (w->avlBalance == 0)
                    {
                        x->avlBalance = y->avlBalance = 0;
                    }
                    else /* |w->avlBalance == +1| */
                    {
                        x->avlBalance = -1, y->avlBalance = 0;
                    }

                    w->avlBalance = 0;
                    pa[k - 1]->avlLink[da[k - 1]] = w;
                }
                else
                {
                    y->avlLink[0] = x->avlLink[1];
                    x->avlLink[1] = y;
                    pa[k - 1]->avlLink[da[k - 1]] = x;

                    if (x->avlBalance == 0)
                    {
                        x->avlBalance = +1;
                        y->avlBalance = -1;
                        break;
                    }
                    else
                    {
                        x->avlBalance = y->avlBalance = 0;
                    }
                }
            }
        }
    }

    tree->avlCount--;
    tree->avlGeneration++;
    return datap;
}


static void omci_trav_refresh(struct avl_traverser *trav)
{
    if (NULL == trav)
        return;

    trav->avlGeneration = trav->avlTable->avlGeneration;

    if (trav->avlNode != NULL)
    {
        omci_avl_comparison_func *cmp   = trav->avlTable->avlCompare;
        void                     *param = trav->avlTable->avlParam;
        struct avl_node          *node  = trav->avlNode;
        struct avl_node          *i     = NULL;

        trav->avlHeight = 0;
        for (i = trav->avlTable->avlRoot; i != node;)
        {
            if ((trav->avlHeight < OMCI_AVL_MAX_HEIGHT) && (NULL != i))
            {
                trav->avlStack[trav->avlHeight++] = i;
                i = i->avlLink[cmp(OMCI_AVL_DATA_PTR_GET(node), OMCI_AVL_DATA_PTR_GET(i), param) > 0];
            }
        }
    }
}


void omci_avl_t_init(struct avl_traverser *trav, struct avl_table *tree)
{
    trav->avlTable      = tree;
    trav->avlNode       = NULL;
    trav->avlHeight     = 0;
    trav->avlGeneration = tree->avlGeneration;
}


void *omci_avl_t_first(struct avl_traverser *trav, struct avl_table *tree)
{
    struct avl_node *x = NULL;

    if ((NULL == tree) || (NULL == trav))
    {
        return NULL;
    }

    trav->avlTable      = tree;
    trav->avlHeight     = 0;
    trav->avlGeneration = tree->avlGeneration;

    x = tree->avlRoot;
    if (x != NULL)
    {
        while (x->avlLink[0] != NULL)
        {
            if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
            {
                trav->avlStack[trav->avlHeight++] = x;
                x = x->avlLink[0];
            }
        }
    }

    trav->avlNode = x;
    return x != NULL ? OMCI_AVL_DATA_PTR_GET(x) : NULL;
}


void *omci_avl_t_last(struct avl_traverser *trav, struct avl_table *tree)
{
    struct avl_node *x = NULL;

    if ((NULL == tree) || (NULL == trav))
    {
        return NULL;
    }

    trav->avlTable      = tree;
    trav->avlHeight     = 0;
    trav->avlGeneration = tree->avlGeneration;

    x = tree->avlRoot;
    if (x != NULL)
    {
        while (x->avlLink[1] != NULL)
        {
            if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
            {
                trav->avlStack[trav->avlHeight++] = x;
                x = x->avlLink[1];
            }
        }
    }

    trav->avlNode = x;
    return x != NULL ? OMCI_AVL_DATA_PTR_GET(x) : NULL;
}


void *omci_avl_t_find(struct avl_traverser *trav, struct avl_table *tree, void *item)
{
    struct avl_node *p = NULL;
    struct avl_node *q = NULL;

    if ((NULL == tree) || (NULL == trav) || (NULL == item))
    {
        return NULL;
    }

    trav->avlTable      = tree;
    trav->avlHeight     = 0;
    trav->avlGeneration = tree->avlGeneration;

    for (p = tree->avlRoot; p != NULL; p = q)
    {
        void *datap = NULL;
        int   cmp   = 0;

        datap = OMCI_AVL_DATA_PTR_GET(p);
        cmp = tree->avlCompare(item, datap, tree->avlParam);

        if (cmp < 0)
        {
            q = p->avlLink[0];
        }
        else if (cmp > 0)
        {
            q = p->avlLink[1];
        }
        else /* |cmp == 0| */
        {
            trav->avlNode = p;
            return (datap);
        }

        if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
        {
            trav->avlStack[trav->avlHeight++] = p;
        }
    }

    trav->avlHeight = 0;
    trav->avlNode   = NULL;
    return NULL;
}


void *omci_avl_t_insert(
    struct avl_traverser *trav,
    struct avl_table     *tree,
    void                 *item)
{
    void **p = NULL;

    if ((NULL == tree) || (NULL == trav) || (NULL == item))
    {
        return NULL;
    }

    p = omci_avl_probe(tree, item);
    if (p != NULL)
    {
        trav->avlTable      = tree;
        trav->avlNode       = ((struct avl_node *)((char *) p - sizeof(struct avl_node)));
        trav->avlGeneration = tree->avlGeneration - 1;
        return p;
    }

    omci_avl_t_init(trav, tree);
    return NULL;
}


void *omci_avl_t_copy(struct avl_traverser *trav, const struct avl_traverser *src)
{
    if ((NULL == trav) || (NULL == src))
    {
        return NULL;
    }

    if (trav != src)
    {
        trav->avlTable      = src->avlTable;
        trav->avlNode       = src->avlNode;
        trav->avlGeneration = src->avlGeneration;
        if (trav->avlGeneration == trav->avlTable->avlGeneration)
        {
            trav->avlHeight = src->avlHeight;
            memcpy (trav->avlStack, (const void *) src->avlStack, sizeof *trav->avlStack * trav->avlHeight);
        }
    }

    return trav->avlNode != NULL ? OMCI_AVL_DATA_PTR_GET(trav->avlNode) : NULL;
}


void *omci_avl_t_next(struct avl_traverser *trav)
{
    struct avl_node *x = NULL;

    if (NULL == trav)
        return NULL;

    if (trav->avlGeneration != trav->avlTable->avlGeneration)
    {
        omci_trav_refresh(trav);
    }
    
    x = trav->avlNode;
    if (x == NULL)
    {
        return omci_avl_t_first(trav, trav->avlTable);
    }
    else if (x->avlLink[1] != NULL)
    {
        if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
        {
            trav->avlStack[trav->avlHeight++] = x;
            x = x->avlLink[1];

            while (x->avlLink[0] != NULL)
            {
                if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
                {
                    trav->avlStack[trav->avlHeight++] = x;
                    x = x->avlLink[0];
                }
            }
        }
    }
    else
    {
        struct avl_node *y = NULL;

        do
        {
            if (trav->avlHeight == 0)
            {
                trav->avlNode = NULL;
                return NULL;
            }

            y = x;
            x = trav->avlStack[--trav->avlHeight];
        } while (y == x->avlLink[1]);
    }

    trav->avlNode = x;

    return (OMCI_AVL_DATA_PTR_GET(x));
}


void *omci_avl_t_prev(struct avl_traverser *trav)
{
    struct avl_node *x = NULL;

    if (NULL == trav)
        return NULL;

    if (trav->avlGeneration != trav->avlTable->avlGeneration)
    {
        omci_trav_refresh(trav);
    }

    x = trav->avlNode;
    if (x == NULL)
    {
        return omci_avl_t_last(trav, trav->avlTable);
    }
    else if (x->avlLink[0] != NULL)
    {
        if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
        {
            trav->avlStack[trav->avlHeight++] = x;
            x = x->avlLink[0];

            while (x->avlLink[1] != NULL)
            {
                if (trav->avlHeight < OMCI_AVL_MAX_HEIGHT)
                {
                    trav->avlStack[trav->avlHeight++] = x;
                    x = x->avlLink[1];
                }
            }
        }
    }
    else
    {
        struct avl_node *y = NULL;

        do
        {
            if (trav->avlHeight == 0)
            {
                trav->avlNode = NULL;
                return NULL;
            }

            y = x;
            x = trav->avlStack[--trav->avlHeight];
        } while (y == x->avlLink[0]);
    }

    trav->avlNode = x;

    return (OMCI_AVL_DATA_PTR_GET(x));
}


void *omci_avl_t_cur(struct avl_traverser *trav)
{
    if (NULL == trav)
        return NULL;

    return trav->avlNode != NULL ? OMCI_AVL_DATA_PTR_GET(trav->avlNode) : NULL;
}


void *omci_avl_t_replace(struct avl_traverser *trav, void *pNew)
{
    void *old = NULL;

    if ((NULL == trav) || (NULL == trav->avlNode) || (NULL == pNew))
        return NULL;

    old = OMCI_AVL_DATA_PTR_GET(trav->avlNode);
    memcpy(old, pNew, trav->avlTable->avlDataSize);

    return old;
}


void omci_avl_clean(struct avl_table *tree, omci_avl_item_func *destroy)
{
    struct avl_node *p = NULL;
    struct avl_node *q = NULL;

    (void)destroy;

    if (NULL == tree)
        return;

    for (p = tree->avlRoot; p != NULL; p = q)
    {
        if (p->avlLink[0] == NULL)
        {
            q = p->avlLink[1];
            tree->avlAlloc->omci_libavl_free(tree->avlAlloc, p);
        }
        else
        {
            q = p->avlLink[0];
            p->avlLink[0] = q->avlLink[1];
            q->avlLink[1] = p;
        }
    }

    tree->avlRoot       = NULL;
    tree->avlCount      = 0;
    tree->avlGeneration = 0;
}


void omci_avl_destroy(struct avl_table *tree, omci_avl_item_func *destroy)
{
    struct avl_node *p = NULL;
    struct avl_node *q = NULL;

    (void)destroy;

    if (NULL == tree)
        return;

    for (p = tree->avlRoot; p != NULL; p = q)
    {
        if (p->avlLink[0] == NULL)
        {
            q = p->avlLink[1];
            tree->avlAlloc->omci_libavl_free(tree->avlAlloc, p);
        }
        else
        {
            q = p->avlLink[0];
            p->avlLink[0] = q->avlLink[1];
            q->avlLink[1] = p;
        }
    }

    tree->avlAlloc->omci_libavl_free(tree->avlAlloc, tree);
}


void *omci_avl_malloc(struct omci_avl_allocator *allocator, size_t size)
{
    if ((NULL == allocator) || (0 >= size))
        return NULL;

    return malloc((unsigned long)size);
}


void omci_avl_free(struct omci_avl_allocator *allocator, void *block)
{
    if ((NULL == allocator) || (NULL == block))
        return;

    free(block);
}


void *omci_avl_item_copy(void *avlItem, void *avlParam)
{
    void *item = NULL;

    if ((NULL == avlItem) || (NULL == avlParam))
        return NULL;

    item = malloc((unsigned long)avlParam);
    if (item == NULL)
    {
        return NULL;
    }

    memcpy(item, avlItem, (unsigned long)avlParam);
    return item;
}


void omci_avl_item_destroy(void *avlItem, void *avlParam)
{
    if ((NULL == avlItem) || (NULL == avlParam))
        return;

    free(avlItem);
}

