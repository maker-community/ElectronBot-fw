#ifndef __USB_LIST_H
#define __USB_LIST_H

#include <cstdio>

struct UsbList_t
{
    struct UsbList_t* next, * prev;
    struct usb_device* pDev;
    struct usb_dev_handle* pDevHandle;
    int devNum;
    char Serial[128];
};

void USB_ListInit();
void USB_ListAdd(struct UsbList_t* _list);
struct UsbList_t* USB_ListPost(int _index);
bool USB_IsListEmpty();
void USB_ListClear();
void USB_ListDelete(struct UsbList_t* _list);
int USB_ListLength();

#endif
