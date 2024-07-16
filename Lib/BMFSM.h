#if !defined(BMFSM_H)
#define BMFSM_H
#include "BMDLNode.h"
#include "BMEv.h"

typedef struct BMFSM {
    BMDLNode_t base; // lockable input queue
    BMDLNode_t outqs; // output queues.

    // event handler
    BMStatus_t (*handler)(struct BMFSM* fsm, BMEv_pt ev);
    BMStatus_t lastStatus;

    void* ctx; // context
} BMFSM_t, *BMFSM_pt;

#define BMFSM_INIOBJ(_varname, _handler, _laststatus, _ctx) { \
    BMDLNode_INIOBJ(_varname.base, NULL), \
    BMDLNode_INIOBJ(_varname.outqs, NULL), \
    _handler, _Laststatus, _ctx }

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