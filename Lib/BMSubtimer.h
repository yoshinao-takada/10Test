#if !defined(BMSUBTIMER_H)
#define BMSUBTIMER_H
#include "BMEv.h"

#define BMEvID_TICK             0
#define BMEvID_SETCOUNT         1

typedef struct {
    BMDLNode_t evqlist; // event queues of downstream objects
    uint16_t id, count, reload;
    BMEv_t evtick; // tick event
} BMSubtimer_t, *BMSubtimer_pt;

#define BMSubtimer_INIOBJ(_varname, _id, _count, _reload, _handler) { \
    BMDLNode_INIOBJ(_varname.evqlist, NULL), _id, _count, _reload, \
    BMEv_INIOBJ(BMEvID_TICK, &_varname, NULL) }

#define BMSubtimer_INIT(_varptr, _sender) \
    BMDLNode_INIT(&_varptr.evqlist); (_varptr)->evtick.sender = (_sender)

#define BMSubtimer_UNINIT(_varptr) BMDLNode_UNINIT(&_varptr.evqlist)

/*!
\brief Clock a subtimer.
*/
BMStatus_t BMSubtimer_Tick(BMSubtimer_pt subtimer);

/*!
\struct BMSubtimers_t
\brief input queue and a subtimer list.
*/
typedef struct {
    BMDLNode_t base; // input event queue
    BMDLNode_t subtimers; // subtimer list
} BMSubtimers_t, *BMSubtimers_pt;

#define BMSubtimers_INIT(_varptr) BMDLNode_INIT(&_varptr->base)
#define BMSubtimers_UNINIT(_varptr) BMDLNode_UNINIT(&_varptr->base)
/*!
\brief Enqueue an event.
*/
#define BMSubtimers_ENQ(_subtimersptr, _evptr) \
    BMEvQ_EnQ(&_subtimersptr->base, _evptr)

/*!
\brief Crunch enqueued event.
*/
BMStatus_t BMSubtimers_Crunch(BMSubtimers_pt subtimers);
#endif /* BMSUBTIMER_H */