#if !defined(BMFSM_H)
#define BMFSM_H
#include "BMDLNode.h"
#include "BMEv.h"

typedef struct BMFSM {
    BMDLNode_t base; // lockable input queue
    BMDLNode_pt outqs; // output queues.
    int outqsSize;

    // event handler
    BMStatus_t (*handler)(struct BMFSM* fsm, BMEv_pt ev);
    BMStatus_t lastStatus;

    void* ctx; // context
} BMFSM_t, *BMFSM_pt;

/*!
\brief Enqueue an event.
*/
#define BMFSM_ENQ(_fsmptr, _evptr) BMEv_EnQ(_evptr, &_fsmptr->base)

/*!
\brief Crunch the first queued event if it exists.
*/
BMStatus_t BMFSM_Crunch(BMFSM_pt fsm);

/*!
\brief Enqueue an event into one of output queues.
*/
BMStatus_t BMFSM_EnQOut(BMFSM_pt fsm, int qindex, BMEv_pt ev);

#endif /* BMFSM_H */