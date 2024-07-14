#if !defined(BMDLNODE_H)
#define BMDLNODE_H
#include "BMObj.h"
// static pool size
#define BMDLNode_POOLSIZE       128

#pragma region BASIC_DECL
typedef struct BMDLNode {
    BMObj_t base;
    struct BMDLNode *next, *prev;
    void* data;
} BMDLNode_t, *BMDLNode_pt;

#define BMDLNode_INIOBJ(_varname, _data) { BMObj_INIOBJ, \
    &(_varname), &(_varname), _data }

#define BMDLNode_INIT(_varptr) BMObj_INIT(&(_varptr)->base)

#define BMDLNode_UNINIT(_varptr) BMObj_UNINIT(&(_varptr)->base)

/*!
\brief return NOT 0 if _anchor has one or more linked nodes.
*/
#define BMDLNode_HAS_ANY(_anchor) ((_anchor)->prev != (_anchor))

/*!
\brief return NOT 0 if _anchor has no linked nodes.
*/
#define BMDLNode_EMPTY(_anchor) ((_anchor)->prev == (_anchor))
#pragma endregion BASIC_DECL

#pragma region CORE_METHODS
/*!
\brief count nodes linked to anchor.
*/
BMStatus_t BMDLNode_Count(BMDLNode_pt anchor, uint16_t* count);

/*!
\brief Add a new node at anchoor->next.
*/
BMStatus_t BMDLNode_AddNext(BMDLNode_pt anchor, BMDLNode_pt newnode);

/*!
\brief Add a new node at anchor->prev.
*/
BMStatus_t BMDLNode_AddPrev(BMDLNode_pt anchor, BMDLNode_pt newnode);

/*!
\brief Get the node at anchor->next and remove the node from the list.
*/
BMStatus_t BMDLNode_GetNext(BMDLNode_pt anchor, BMDLNode_pt *ppnode);

/*!
\brief Get the node at anchor->prev and remove the node from the list.
*/
BMStatus_t BMDLNode_GetPrev(BMDLNode_pt anchor, BMDLNode_pt *ppnode);

/*!
\brief Remove the node matching to toRemove.
*/
BMStatus_t BMDLNode_Remove(BMDLNode_pt anchor, BMDLNode_pt toRemove);
#pragma endregion CORE_METHODS

#pragma region NODE_FINDER
/*!
\brief find the node with the matched condition specified by zeroifmatch.
    Searching begins from anchor->next and the first matching node is reported.
    Other matching nodes are ignored even it they exist.
\param tofind [in] payload pointer to compare
\param zeroifmatch [in] comparator function which returns zero if the two
    payload pointers satisfy match conditions.
\param ppfound [out] found node ptr
*/
BMStatus_t BMDLNode_Find(BMDLNode_pt anchor, const void* tofind,
    int (*zeroifmatch)(const void*, const void*), BMDLNode_pt* ppfound);

int BMDLNode_DefaultMatch(const void* pv0, const void* pv1);

#define BMDLNode_FIND(_anchor, _tofind, _ppfound) \
BMDLNode_Find(_anchor, _tofind, BMDLNode_DefaultMatch, _ppfound)
#pragma endregion NODE_FINDER

#pragma region STATIC_POOL_METHODS
/*!
\brief Create a static pool of BMDLNode_t.
    The real body of the pool is declared in "BMDLNode.c".
*/
BMStatus_t BMDLNode_CreateSPool();

/*!
\brief Destroy the static pool.
*/
void BMDLNode_DestroySPool();

/*!
\brief Get a node from the static pool.
*/
BMStatus_t BMDLNode_SGet(BMDLNode_pt *ppnode);

/*!
\brief Return a node into the static pool.
*/
BMStatus_t BMDLNode_SReturn(BMDLNode_pt node);

/*!
\brief Count nodes in the static pool.
*/
uint16_t BMDLNode_CountSPool();
#pragma endregion STATIC_POOL_METHODS

#pragma region MULTI_DIMENSIONAL_LIST
/*!
\brief Add a list to a multi-dimensional list.
*/
BMStatus_t BMMDList_Add(BMDLNode_pt list, BMDLNode_pt toAdd);
#pragma endregion MULTI_DIMENSIONAL_LIST
#endif /* BMDLNODE_H */