#include "BMDLNode.h"
#include "BMUtils.h"
#include <sys/param.h>
#pragma region CORE_METHODS
BMStatus_t BMDLNode_Count(BMDLNode_pt anchor, uint16_t* count)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        (*count) = 0;
        for (BMDLNode_pt iter = anchor->next; iter != anchor; iter = iter->next)
        {
            (*count)++;
        }
    } while (0);
    status = BMLock_UNLOCK((BMLock_pt)anchor);
    return status;
}

/*!
\brief Add a new node at anchoor->next.
*/
BMStatus_t BMDLNode_AddNext(BMDLNode_pt anchor, BMDLNode_pt newnode)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        newnode->prev = anchor;
        newnode->next = anchor->next;
        anchor->next->prev = newnode;
        anchor->next = newnode;
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}

/*!
\brief Add a new node at anchor->prev.
*/
BMStatus_t BMDLNode_AddPrev(BMDLNode_pt anchor, BMDLNode_pt newnode)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        newnode->prev = anchor->prev;
        newnode->next = anchor;
        anchor->prev->next = newnode;
        anchor->prev = newnode;
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}

/*!
\brief Get the node at anchor->next and remove the node from the list.
*/
BMStatus_t BMDLNode_GetNext(BMDLNode_pt anchor, BMDLNode_pt *ppnode)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        if (BMDLNode_EMPTY(anchor))
        {
            status = BMStatus_NOTFOUND;
            break;
        }
        *ppnode = anchor->next;
        anchor->next = (*ppnode)->next;
        (*ppnode)->next->prev = anchor;
        (*ppnode)->next = (*ppnode)->prev = (*ppnode);
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}

/*!
\brief Get the node at anchor->prev and remove the node from the list.
*/
BMStatus_t BMDLNode_GetPrev(BMDLNode_pt anchor, BMDLNode_pt *ppnode)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        if (BMDLNode_EMPTY(anchor))
        {
            status = BMStatus_NOTFOUND;
            break;
        }
        *ppnode = anchor->prev;
        anchor->prev = (*ppnode)->prev;
        (*ppnode)->prev->next = anchor;
        (*ppnode)->next = (*ppnode)->prev = (*ppnode);
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}


/*!
\brief Remove the node matching to toRemove.
*/
BMStatus_t BMDLNode_Remove(BMDLNode_pt anchor, BMDLNode_pt toRemove)
{
    BMStatus_t status = BMStatus_NOTFOUND;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        for (BMDLNode_pt iter = anchor->next; iter != anchor; iter = iter->next)
        {
            if (iter == toRemove)
            {
                iter->prev->next = iter->next;
                iter->next->prev = iter->prev;
                toRemove->next = toRemove->prev = toRemove;
                break;
            }
        }
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}

#pragma endregion CORE_METHODS
#pragma region NODE_FINDER
BMStatus_t BMDLNode_Find(BMDLNode_pt anchor, const void* tofind,
    int (*zeroifmatch)(const void*, const void*), BMDLNode_pt* ppfound)
{
    BMStatus_t status = BMStatus_SUCCESS;
    *ppfound = NULL;
    do {
        if ((status = BMLock_LOCK((BMLock_pt)anchor)) != BMStatus_SUCCESS)
        {
            break;
        }
        status = BMStatus_NOTFOUND;
        for (BMDLNode_pt iter = anchor->next; iter != anchor; iter = iter->next)
        {
            if (0 == zeroifmatch(iter->data, tofind))
            {
                status = BMStatus_SUCCESS;
                *ppfound = iter;
                break;
            }
        }
    } while (0);
    status = MAX(BMLock_UNLOCK((BMLock_pt)anchor), status);
    return status;
}

int BMDLNode_DefaultMatch(const void* pv0, const void* pv1)
{
    #if (BMAlgn_ARCH == 8)
        return (int)(int64_t)(pv1 - pv0);
    #elif (BMAlgn_ARCH == 4)
        return (int)(int32_t)(pv1 - pv0);
    #else
        return (int)(pv1 - pv0)
    #endif
}

#define BMDLNode_FIND(_anchor, _tofind, _ppfound) \
    BMDLNode_Find(_anchor, _tofind, BMDLNode_DefaultMatch, _ppfound)
#pragma endregion NODE_FINDER

#pragma region STATIC_POOL_METHODS
static BMDLNode_t spool = BMDLNode_INIOBJ(spool, NULL);
static BMDLNode_t pooledNodes[BMDLNode_POOLSIZE];

BMStatus_t BMDLNode_CreateSPool()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_INIT(&spool.base.lock)) != BMStatus_SUCCESS)
        {
            break;
        }
        for (int i = 0; i < BMArray_SIZE(pooledNodes); i++)
        {
            BMDLNode_pt node = pooledNodes + i;
            node->data = NULL;
            node->next = node->prev = node;
            node->base.lock = BMLock_INIOBJ;
            if ((status = BMLock_INIT(&node->base.lock)) != BMStatus_SUCCESS)
            {
                break;
            }
            if ((status = BMDLNode_AddPrev(&spool, node)) != BMStatus_SUCCESS)
            {
                break;
            }
        }
        if (status) break;
    } while (0);
    return status;
}

void BMDLNode_DestroySPool()
{
    for (int i = 0; i < BMArray_SIZE(pooledNodes); i++)
    {
        BMDLNode_pt node = pooledNodes + i;
        BMLock_UNINIT(&node->base.lock);
    }
    BMLock_UNINIT(&spool.base.lock);
}

BMStatus_t BMDLNode_SGet(BMDLNode_pt *ppnode)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if (BMDLNode_EMPTY(&spool))
        {
            status = BMStatus_NORESOURCE;
            break;
        }
        status = BMDLNode_GetNext(&spool, ppnode);
    } while (0);
    return status;
}

BMStatus_t BMDLNode_SReturn(BMDLNode_pt node)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((node != node->next) || (node != node->prev))
        { // node is not correctly isolated.
            status = BMStatus_INVALID;
            break;
        }
        status = BMDLNode_AddNext(&spool, node);
    } while (0);
    return status;
}

uint16_t BMDLNode_CountSPool()
{
    uint16_t count = 0;
    BMDLNode_Count(&spool, &count);
    return count;
}
#pragma endregion STATIC_POOL_METHODS

BMStatus_t BMMDList_Add(BMDLNode_pt list, BMDLNode_pt toAdd)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt newnode = NULL;
    do {
        if ((status = BMLock_LOCK(&list->base.lock)) != BMStatus_SUCCESS)
        {
            break;
        }
        if ((status = BMDLNode_SGet(&newnode)) != BMStatus_SUCCESS)
        {
            break;
        }
        newnode->data = toAdd;
        status = BMDLNode_AddNext(list, newnode);
    } while (0);
    status = MAX(status, BMLock_UNLOCK(&list->base.lock));
    return status;
}
