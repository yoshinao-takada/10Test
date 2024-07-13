#if !defined(BMLOCK_H)
#define BMLOCK_H
#include "BMStatus.h"
#include <pthread.h>
#include <sys/param.h>

/*!
\brief Global notification of status record.
*/
#if defined(BMLOCK_C)
BMStatus_t BMLock_Status = BMStatus_SUCCESS;
#else
extern BMStatus_t BMLock_Status;
#endif

typedef pthread_spinlock_t BMLock_t;
typedef pthread_spinlock_t *BMLock_pt;

#define BMLock_LOCK(lockptr) (BMLock_Status = \
    MAX((pthread_spin_lock(lockptr) ? \
    BMStatus_NORESOURCE : BMStatus_SUCCESS), BMLock_Status))

#define BMLock_TRYLOCK(lockptr) (BMLock_Status = \
    MAX((pthread_spin_trylock(lockptr) ? \
    BMStatus_NORESOURCE : BMStatus_SUCCESS), BMLock_Status))

#define BMLock_UNLOCK(lockptr) (BMLock_Status = \
    MAX((pthread_spin_unlock(lockptr) ? \
    BMStatus_FAILURE : BMStatus_SUCCESS), BMLock_Status))

#define BMLock_INIT(lockptr) (BMLock_Status = \
    MAX((pthread_spin_init(lockptr, PTHREAD_PROCESS_PRIVATE) ? \
    BMStatus_NORESOURCE : BMStatus_SUCCESS), BMLock_Status))

#define BMLock_UNINIT(lockptr) (BMLock_Status = \
    MAX((pthread_spin_destroy(lockptr) ? \
    BMStatus_FAILURE : BMStatus_SUCCESS), BMLock_Status))

#define BMLock_INIOBJ   (0)
#endif /* BMLOCK_H */