#include "BMEv.h"
#include "BMTest.h"
#include "BMSubtimer.h"
#include "BMUtils.h"

/*!
\brief Unit test for macros related to initialization.
*/
BMStatus_t BMEv_INITUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    int sender = 0, senderData = 1;
    void* pvSender = (void*)&sender;
    void* pvData = (void*)&senderData;
    do {
        BMEv_t ev = BMEv_INIOBJ(BMEvID_TICK, pvSender, pvData);
        BMEv_INIT(&ev);
        for (int i = 0; i < 8; i ++)
        {
            BMEv_ADDREF(&ev);
        }
        if (ev.refctr != 8)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMEv_ADDREF()");
        }
        for (int i = 0; i < 8; i++)
        {
            BMEv_DELREF(&ev);
        }
        if (ev.refctr != 0)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMEv_DELREF()");
        }
        BMEv_SETUINT16(&ev, 32767);
        if (BMEv_GETUINT16(&ev) != 0x7fff)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMEv_GETUINT16");
        }
        BMEv_UNINIT(&ev);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}


/*!
\brief Unit test for EnQ, DeQ, etc.
*/
BMStatus_t BMEv_EnDeQUT()
{
    int sender = 0;
    BMStatus_t status = BMStatus_SUCCESS;
    BMEv_t evsrcs[4] =
    {
        BMEv_INIOBJ(0, (void*)&sender, NULL),
        BMEv_INIOBJ(1, (void*)&sender, NULL),
        BMEv_INIOBJ(2, (void*)&sender, NULL),
        BMEv_INIOBJ(3, (void*)&sender, NULL),
    };
    BMEv_pt evretrieved[4];
    BMDLNode_t queue = BMDLNode_INIOBJ(queue, NULL);
    BMDLNode_INIT(&queue);
    do {
        // Init evsrcs elements.
        for (int i = 0; i < BMArray_SIZE(evsrcs); i++)
        {
            BMEv_pt ptr = evsrcs + i;
            BMEv_INIT(ptr);
            BMEv_SETUINT16(ptr, i);
            if ((status = BMEvQ_EnQ(&queue, ptr)) != BMStatus_SUCCESS) 
            {
                BMTest_ERRLOGBREAKEX("Fail in BMEvQ_EnQ()");
            }
        }
        if (status) break;

        for (int i = 0; i < BMArray_SIZE(evsrcs); i++)
        {
            if (evsrcs[i].refctr != 1)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("(evsrcs[%d].refctr != 1)", i);
            }
            if ((status = BMEvQ_DeQ(&queue, &evretrieved[i])) != BMStatus_SUCCESS)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMEvQ_DeQ()");
            }
            BMEv_DELREF(evretrieved[i]);
            if (evsrcs[i].refctr != 0)
            {
                status = BMStatus_FAILURE;
                BMTest_ERRLOGBREAKEX("Fail in BMEvQ_DeQ()");
            }
        }
        if (status) break;

        // Clean evsrcs elements.
        for (int i = 0; i < BMArray_SIZE(evsrcs); i++)
        {
            BMEv_pt ptr = evsrcs + i;
            BMEv_UNINIT(ptr);
        }
    } while (0);
    BMDLNode_UNINIT(&queue);
    BMTest_ENDFUNC(status);
    return status;
}

/*!
\brief Unit test for EnQ into multiple queues.
*/
BMStatus_t BMEvQList_EnQUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        BMTest_ERRLOGEX(">>> %s is not implemented.", __FUNCTION__);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMEvUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMEv_INITUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEv_INITUT()");
        }
        if ((status = BMEv_EnDeQUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEv_EnDeQUT()");
        }
        if ((status = BMEvQList_EnQUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMEvQList_EnQUT()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}