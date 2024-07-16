#include "BMFSM.h"

BMStatus_t BMFSM_Crunch(BMFSM_pt fsm)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMEv_pt ev = NULL;
    do {
        if (BMDLNode_EMPTY(&fsm->base))
        { // no pending event exists.
            break;
        }
        if ((status = BMEvQ_DeQ(&fsm->base, &ev)) != BMStatus_SUCCESS)
        { // fail to retrieve a queued event.
            break;
        }
        fsm->lastStatus = fsm->handler(fsm, ev);
        if (fsm->lastStatus == BMStatus_BAD_EVENT)
        {
            status = BMStatus_INVALID;
        }
    } while (0);
    return status;
}

BMStatus_t BMFSM_EnQOut(BMFSM_pt fsm, int qindex, BMEv_pt ev)
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_pt outq = NULL;
    do {
        if ((status = BMDLNode_PeekList(&fsm->outqs, qindex, &outq))
            != BMStatus_SUCCESS)
        {
            break;
        }
        status = BMEvQ_EnQ(outq, ev);
    } while (0);
    return status;
}