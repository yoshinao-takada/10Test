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
    do {
        if (qindex >= fsm->outqsSize)
        { // out of range
            status = BMStatus_RANGE;
            break;
        }
        status = BMEvQList_EnQ(fsm->outqs + qindex, ev);
    } while (0);
    return status;
}