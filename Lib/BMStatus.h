#if !defined(BMSTATUS_H)
#define BMSTATUS_H
#include <stdint.h>
typedef uint16_t BMStatus_t;

// Result status definitions
#pragma region Resut_Status
#define BMStatus_SUCCESS        0
#define BMStatus_SUCCESSBREAK   1

// state handler result status
#define BMStatus_IGNORE         2
#define BMStatus_ACCEPT         3
#define BMStatus_TRANSIT        4
#define BMStatus_BAD_EVENT      5
// end of state handler result status

#define BMStatus_ERRBEGIN       0x4000
#define BMStatus_FAILURE        (BMStatus_ERRBEGIN + 1)
#define BMStatus_INVALID        (BMStatus_ERRBEGIN + 2)
#define BMStatus_NORESOURCE     (BMStatus_ERRBEGIN + 3)
#define BMStatus_NOCONVERGENCE  (BMStatus_ERRBEGIN + 4)
#define BMStatus_SINGULAR       (BMStatus_ERRBEGIN + 5)
#define BMStatus_NOTFOUND       (BMStatus_ERRBEGIN + 6)
#define BMStatus_END            (BMStatus_ERRBEGIN + 7)
#define BMStatus_RANGE          (BMStatus_ERRBEGIN + 8)
#define BMStatus_RESET(_sptr)   ((*(_sptr)) = BMStatus_SUCCESS)
#define BMStatus_IS_ERR(_s)     (_s >= BMStatus_ERRBEGIN)
#pragma endregion Resut_Status

#endif /* BMSTATUS_H */