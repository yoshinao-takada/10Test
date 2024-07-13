#if !defined(BMUTILS_H)
#define BMUTILS_H
#define BMAlgn_ARCH     8   // x64, AArch64
// #define BMAlgn_ARCH     4   // x86, AArch32, Cortex-M, -R, C2k
// #define BMAlgn_ARCH     2   // MS430, RL78

#define BMAlgn_MASK     (BMAlgn_ARCH - 1)
#define BMAlgn_DOWN(_n) ((_n) & ~BMAlgn_MASK)
#define BMAlgn_UP(_n)   (BMAlgn_DOWN(_n) + BMAlgn_ARCH)
#define BMAlgn_TO_ARCH(_n) \
    (((_n) & BMAlgn_MASK) ? BMAlgn_UP(_n) : BMAlgn_DOWN(_n))

#define BMEndian_SWAPBYTES(_btptrA, _btptrB, _btptrTMP) \
    *(_btptrTMP) = *(_btptrA); \
    *(_btptrA) = *(_btptrB); \
    *(_btptrB) = *(_btptrTMP)

#define BMEndian_REV16(_anyptr) { \
    uint8_t _tmp; \
    uint8_t* _btptr = (uint8_t*)(_anyptr); \
    BMENDIAN_SWAPBYTES(_btptr, _btptr + 1, &_tmp); \
}

#define BMENDIAN_REV32(_anyptr) { \
    uint8_t _tmp; \
    uint8_t* _btptr = (uint8_t*)(_anyptr); \
    BMENDIAN_SWAPBYTES(_btptr, _btptr + 3, &_tmp); \
    BMENDIAN_SWAPBYTES(_btptr + 1, _btptr + 2, &_tmp); \
}

#define BMArray_SIZE(_a)    (sizeof(_a)/sizeof(_a[0]))

/*!
\brief fill array with a value.
\param _beginptr [in,out] _beginptr is modified in the macro. The final
    value is equal to _endptr. Threfore _beginptr must be modifiable
    with a real body.
*/
#define BMArray_FILL(_beginptr, _endptr, _val) \
    for (; _beginptr != (_endptr); _beginptr++) \
    { \
        *_beginptr = _val; \
    }

#endif /* BMUTILS_H */