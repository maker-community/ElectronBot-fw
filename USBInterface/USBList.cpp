#include "USBList.h"
#include <cstdlib>
#include <cstring>

static struct UsbList_t head, tail;


void USB_ListInit()
{
    head.next = &tail;
    head.prev = &head;
    head.pDev = nullptr;
    head.pDevHandle = nullptr;
    head.devNum = -1;

    tail.next = &tail;
    tail.prev = &head;
    tail.pDev = nullptr;
    tail.pDevHandle = nullptr;
}


void USB_ListAdd(struct UsbList_t* _list)
{
    struct UsbList_t* listTmp;
    for (listTmp = head.next; listTmp != &tail; listTmp = listTmp->next)
    {
        if (0 == strcmp(listTmp->Serial, _list->Serial))
        {
            listTmp->pDevHandle = _list->pDevHandle;
            listTmp->pDev = _list->pDev;
            free(_list);
            return;
        }
    }
    _list->next = &tail;
    _list->prev = tail.prev;
    _list->devNum = _list->prev->devNum + 1;

    tail.prev->next = _list;
    tail.prev = _list;
}


int USB_ListLength()
{
    struct UsbList_t* listTmp;
    int len = 0;
    for (listTmp = head.next; listTmp != &tail; listTmp = listTmp->next)
    {
        len++;
    }
    return len;
}


static inline bool USB_IsListHead(struct UsbList_t* _list)
{
    return _list == &head;
}


static inline bool USB_IsListTail(struct UsbList_t* _list)
{
    return _list == &tail;
}


void USB_ListDelete(struct UsbList_t* _list)
{
    if ((!USB_IsListHead(_list)) && (!USB_IsListTail(_list)))
    {
        _list->prev->next = _list->next;
        _list->next->prev = _list->prev;
        free(_list);
    }
}


bool USB_IsListEmpty()
{
    return ((head.next == (&tail)) && (tail.prev == (&head)));
}


struct UsbList_t* USB_ListPost(int _index)
{
    struct UsbList_t* list_tmp;
    if (_index < USB_ListLength())
    {
        for (list_tmp = head.next; list_tmp != &tail; list_tmp = list_tmp->next)
        {
            if (list_tmp->devNum == _index)
            {
                return list_tmp;
            }
        }
    } else
    {
        return nullptr;
    }
    return nullptr;
}


void USB_ListClear()
{
    int i = 0;
    while (!USB_IsListEmpty())
    {
        USB_ListDelete(USB_ListPost(i++));
    }
}