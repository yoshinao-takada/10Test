#if !defined(BMOBJ_H)
#define BMOBJ_H
#include "BMLock.h"
#include "BMTypes.h"

/*!
\brief Most basic root object.
*/
typedef struct BMObj {
    BMLock_t lock;
    BMID_t objtype;
} BMObj_t, *BMObj_pt;
typedef const BMObj_t *BMObj_cpt;

/*!
\brief const initializer of BMObj for extension objects
*/
#define BMObj_INIOBJEX(_objtype) { _objtype, BMLock_INIOBJ }

/*!
\brief default const initializer
*/
#define BMObj_INIOBJ BMObj_INIOBJEX(BMObj_TYPEID)

/*!
\brief Initilizing macro as a lockable object.
\return BMStatus_SUCCESS or BMStatus_NORESOUCE
*/
#define BMObj_INIT(_objptr) BMLock_INIT(&(_objptr)->lock)

/*!
\brief Uninitilizing macro as a lockable object.
\return BMStatus_SUCCESS or BMStatus_NORESOUCE
*/
#define BMObj_UNINIT(_objptr) BMLock_UNINIT(&(_objptr)->lock)
#endif /* BMOBJ_H */