#if !defined(BMEV_H)
#define BMEV_H
#include "BMObj.h"
#include "BMDLNode.h"

typedef struct {
    BMObj_t base;
    BMID_t evid;
    BMObj_cpt sender; // sender object
    uint16_t refctr; // reference counter of multi-listeners
    void* data; // any data can be carried by an event.
} BMEv_t, *BMEv_pt;

#define BMEv_INIOBJ(_evid, _sender, _data) { \
    BMObj_INIOBJEX(BMEv_TYPEID), _evid, _sender, 0, _data }
/*!
\brief Set data to uint16_t immediate value.
*/
#define BMEv_SETUINT16(_varptr, _u16data) \
    (_varptr)->data = (void*)(uint64_t)(_u16data)

/*!
\brief Retrieve uint16 value casted from data.
*/
#define BMEv_GETUINT16(_varptr) (uint16_t)(uint64_t)((_varptr)->data)

#define BMEv_ADDREF(_evptr) ( \
    BMLock_LOCK((BMLock_pt)(_evptr)), (_evptr)->refctr++, \
    BMLock_UNLOCK((BMLock_pt)(_evptr)))

#define BMEv_DELREF(_evptr) ( \
    BMLock_LOCK((BMLock_pt)(_evptr)), (_evptr)->refctr--, \
    BMLock_UNLOCK((BMLock_pt)(_evptr)))

#pragma region Queue_and_QList
/*!
\brief Enqueue an event to a queue.
\param queue [in,out] An event queue
\param ev [in] an event
*/
BMStatus_t BMEvQ_EnQ(BMDLNode_pt queue, BMEv_pt ev);

/*!
\brief Dequeue an event from a queue
\param queue [in,out] An event queue
\param ppev [in] pointer-pointer to an event. The retrieved event is
    stored in *ppev.
*/
BMStatus_t BMEvQ_DeQ(BMDLNode_pt queue, BMEv_pt *ppev);

/*!
\brief Enqueue an event to multiple queues.
\param qlist [in,out] A list of event queues.
\param ev [in] an event.
*/
BMStatus_t BMEvQList_EnQ(BMDLNode_pt qlist, BMEv_pt ev);


#pragma endregion Queue_and_QList
#endif /* BMEV_H */