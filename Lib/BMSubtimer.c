#include "BMSubtimer.h"
#include <assert.h>

static BMStatus_t BMSubtimer_RaiseEv(BMDLNode_pt evqlist, BMEv_pt ev)
{
    BMStatus_t status = BMStatus_SUCCESS;
    for (BMDLNode_pt iter = evqlist->next; iter != evqlist; iter = iter->next)
    {
        BMDLNode_pt evq = (BMDLNode_pt)iter->data;
        assert(
            (evq->base.objtype == BMDLNode_TYPEID) ||
            (evq->base.objtype == BMDLNodeSPool_TYPEID)
        );
        status = MAX(BMEvQ_EnQ(evq, ev), status);
        if (status) break;
    }
    return status;
}

BMStatus_t BMSubtimer_Tick(BMSubtimer_pt subtimer)
{
    BMStatus_t status = BMStatus_SUCCESS;
    if (subtimer->count)
    {
        if (--(subtimer->count) == 0)
        {
            subtimer->count = subtimer->reload;
            status = BMSubtimer_RaiseEv(&subtimer->evqlist, &subtimer->evtick);
        }
    }
    return status;
}

static BMStatus_t BMSubtimers_SetCount
(BMSubtimers_pt subtimers, uint16_t id, uint16_t value)
{
    BMStatus_t status = BMStatus_NOTFOUND;
    do {

    } while (0);
    return status;
}

static BMStatus_t BMSubtimers_Tick(BMSubtimers_pt subtimers)
{
    BMStatus_t status = BMStatus_SUCCESS;
    for (BMDLNode_pt node = subtimers->subtimers.next;
        node != &subtimers->subtimers; node = node->next)
    {
        BMSubtimer_pt subtimer = (BMSubtimer_pt)node->data;
        status = MAX(status, BMSubtimer_Tick(subtimer));
    }
    return status;
}
BMStatus_t BMSubtimers_Crunch(BMSubtimers_pt subtimers)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMEv_pt ev = NULL;
    do {
        if (BMDLNode_EMPTY(&subtimers->base)) break;
        if ((status = BMEvQ_DeQ(&subtimers->base, &ev)) != BMStatus_SUCCESS)
        {
            break;
        }
        switch (ev->evid)
        {
            case BMEvID_TICK:
            break;
            case BMEvID_SETCOUNT:
            break;
        }
        BMEv_DELREF(ev);
    } while (0);
    return status;
}