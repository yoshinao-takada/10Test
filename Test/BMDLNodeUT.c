#include "BMDLNode.h"
#include "BMTest.h"
#include "BMUtils.h"

BMStatus_t BMDLNode_SPoolUT();
BMStatus_t BMDLNode_AddGetPrevNext();
BMStatus_t BMDLNode_NodeFinderUT();


BMStatus_t BMDLNodeUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    do {
        if ((status = BMDLNode_SPoolUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_SPoolUT()");
        }
        if ((status = BMDLNode_AddGetPrevNext()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_AddGetPrevNext()");
        }
        if ((status = BMDLNode_NodeFinderUT()) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_NodeFinderUT()");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNode_SPoolUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    uint16_t count = 0;
    int index = 0;
    BMDLNode_pt nodes[2 * BMDLNode_POOLSIZE];
    do {
        for (; index < 10; index++)
        {
            status = BMDLNode_SGet(nodes + index);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_SGet(nodes + %d)", index);
            }
        }
        if (status) break;
        count = BMDLNode_CountSPool();
        if ((count + index) != BMDLNode_POOLSIZE)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("((count + index) != BMDLNode_POOLSIZE)");
        }
        while ((status = BMDLNode_SGet(nodes + index)) == BMStatus_SUCCESS)
        {
            index++;
        }
        if (index != BMDLNode_POOLSIZE)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("(index != BMDLNode_POOLSIZE)");
        }
        if (0 != BMDLNode_CountSPool())
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("(0 != BMDLNode_CountSPool())");
        }
        for (index = 0; index < BMDLNode_POOLSIZE; index++)
        {
            if ((status = BMDLNode_SReturn(nodes[index])) != BMStatus_SUCCESS)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_SReturn()");
            }
        }
        if (status) break;
        if (BMDLNode_POOLSIZE != BMDLNode_CountSPool())
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("(BMDLNode_POOLSIZE != BMDLNode_CountSPool())");
        }
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

static void GetReturnNodes(BMDLNode_pt *nodes, int size, int is_get)
{
    for (int i = 0; i < size; i++)
    {
        if (is_get)
        {
            BMDLNode_SGet(&nodes[i]);
            nodes[i]->data = (void*)(uint64_t)i;
        }
        else
        {
            BMDLNode_SReturn(nodes[i]);
        }
    }
}

BMStatus_t BMDLNode_AddGetPrevNext()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_t anchor = BMDLNode_INIOBJ(anchor, NULL);
    BMDLNode_INIT(&anchor);
    BMDLNode_pt nodes[4];
    do {
        GetReturnNodes(nodes, BMArray_SIZE(nodes), 1);
        for (int i = 0; i < BMArray_SIZE(nodes); i++)
        {
            status = BMDLNode_AddNext(&anchor, nodes[i]);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_AddNext()");
            }
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(nodes); i++)
        {
            BMDLNode_pt node = NULL;
            status = BMDLNode_GetPrev(&anchor, &node);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetPrev()");
            }
            if ((node->data != nodes[i]->data) ||
                (node != node->next) ||
                (node != node->prev))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetPrev()");
            }
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(nodes); i++)
        {
            status = BMDLNode_AddPrev(&anchor, nodes[i]);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_AddPrev()");
            }
        }
        if (status) break;
        for (int i = 0; i < BMArray_SIZE(nodes); i++)
        {
            BMDLNode_pt node = NULL;
            status = BMDLNode_GetNext(&anchor, &node);
            if (status)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetNext()");
            }
            if ((node->data != nodes[i]->data) ||
                (node != node->next) ||
                (node != node->prev))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_GetNext()");
            }
        }
        GetReturnNodes(nodes, BMArray_SIZE(nodes), 0);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}

BMStatus_t BMDLNode_NodeFinderUT()
{
    BMStatus_t status = BMStatus_SUCCESS;
    BMDLNode_t anchor = BMDLNode_INIOBJ(anchor, NULL);
    BMDLNode_INIT(&anchor);
    BMDLNode_pt nodes[4];
    BMDLNode_pt found;
    uint16_t count;
    do {
        GetReturnNodes(nodes, BMArray_SIZE(nodes), 1);
        for (int i = 0; i < BMArray_SIZE(nodes); i++)
        {
            BMDLNode_AddPrev(&anchor, nodes[i]);
        }
        status = BMDLNode_FIND(&anchor, (void*)(uint64_t)4, &found);
        if (status != BMStatus_NOTFOUND)
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_FIND()");
        }
        status = BMDLNode_FIND(&anchor, (void*)(uint64_t)3, &found);
        if ((status != BMStatus_SUCCESS) ||
            (found != nodes[3]))
        {
            status = BMStatus_FAILURE;
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_FIND()");
        }
        if ((status = BMDLNode_Remove(&anchor, found)) != BMStatus_SUCCESS)
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_Remove()");
        }
        if (((status = BMDLNode_Count(&anchor, &count)) != BMStatus_SUCCESS) ||
            (count != (BMArray_SIZE(nodes) - 1)))
        {
            BMTest_ERRLOGBREAKEX("Fail in BMDLNode_Count()");
        }
        for (int i = 0; i < 3; i++)
        {
            if ((status = BMDLNode_FIND(&anchor, (void*)(uint64_t)i, &found))
                 != BMStatus_SUCCESS)
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_FIND()");
            }
            if ((status = BMDLNode_Remove(&anchor, found)))
            {
                BMTest_ERRLOGBREAKEX("Fail in BMDLNode_Remove()");
            }
        }
        GetReturnNodes(nodes, BMArray_SIZE(nodes), 0);
    } while (0);
    BMTest_ENDFUNC(status);
    return status;
}