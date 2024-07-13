#include "BMEv.h"

BMStatus_t BMEvQ_EnQ(BMDLNode_pt queue, BMEv_pt ev)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt newnode = NULL;
    do {
        if ((status = BMDLNode_SGet(&newnode)) != BMStatus_SUCCESS)
        {
            break;
        }
        newnode->data = ev;
        if ((status = BMDLNode_AddNext(queue, newnode)) != BMStatus_SUCCESS)
        {
            break;
        }
        if ((status = BMEv_ADDREF(ev)) < BMStatus_ERRBEGIN)
        {
            status = BMStatus_SUCCESS;
        }
    } while (0);
    return status;
}

BMStatus_t BMEvQ_DeQ(BMDLNode_pt queue, BMEv_pt *ppev)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt gotten = NULL;
    do {
        if ((status = BMDLNode_GetPrev(queue, &gotten)))
        {
            break;
        }
        *ppev = (BMEv_pt)gotten->data;
        status = BMDLNode_SReturn(gotten);
    } while (0);
    return status;
}

BMStatus_t BMEvQList_EnQ(BMDLNode_pt qlist, BMEv_pt ev)
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMLock_LOCK(&qlist->base.lock)) != BMStatus_SUCCESS)
        {
            break;
        }
        if (BMDLNode_EMPTY(qlist))
        {
            break;
        }
        for (BMDLNode_pt iter = qlist->next; iter != qlist; iter = iter->next)
        {
            BMDLNode_pt queue = (BMDLNode_pt)iter->data;
            if ((status = BMEvQ_EnQ(queue, ev)) != BMStatus_SUCCESS)
            {
                break;
            }
        }
    } while (0);
    status = MAX(status, BMLock_UNLOCK(&qlist->base.lock));
    return status;
}
