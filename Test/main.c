#define BMTEST_MAIN_C
#include "BMTest.h"
#include "BMDLNode.h"
#include <stdlib.h>

BMStatus_t BMDLNodeUT();
BMStatus_t BMEvUT();


int main()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMTest_START;
    do {
        if ((status = BMDLNode_CreateSPool()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_CreateSPool()");
        }
        if ((status = BMDLNodeUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNodeUT()");
        }
        if ((status = BMEvUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEvUT()");
        }
        BMTest_PRINTF("Count SPool = %u/%u\n", 
            BMDLNode_CountSPool(), BMDLNode_POOLSIZE);
    } while (0);
    BMDLNode_DestroySPool();
    BMTest_ENDFUNC(status);
    BMTest_END;
    return status ? EXIT_FAILURE : EXIT_SUCCESS;
}