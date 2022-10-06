#include "USBInterface.h"
#include "USBList.h"
#include "usb.h"

// #define TEST_INTERFACE

int VID = 0x1001;
int PID = 0x8023;

usb_dev_handle* pUsbDeviceHandle = nullptr;
static bool needInit = true;

static int ScanDevice()
{
    struct usb_bus* bus;
    struct usb_device* dev;
    int devNum = 0;
    for (bus = usb_get_busses(); bus; bus = bus->next)
        for (dev = bus->devices; dev; dev = dev->next)
            if (dev->descriptor.idVendor == VID && dev->descriptor.idProduct == PID)
                devNum++;

    return devNum;
}


DLL_API
int USB_ScanDevice(int _usbPid, int _usbVid)
{
    if (needInit)
    {
        USB_ListInit();
        usb_init();
        usb_find_busses();
        needInit = false;
    }
    usb_find_devices();
    VID = _usbVid;
    PID = _usbPid;
    return ScanDevice();
}


bool OpenDevice(int _devIndex)
{
    struct usb_bus* bus;
    struct usb_device* dev;
    struct usb_dev_handle* devHandle;
    int devNum;

    struct UsbList_t* usb_list;
    if (needInit)
    {
        devNum = USB_ScanDevice(PID, VID);
        if (devNum <= 0)
            return false;
    }
    for (bus = usb_get_busses(); bus; bus = bus->next)
    {
        for (dev = bus->devices; dev; dev = dev->next)
        {
            if (dev->descriptor.idVendor == VID
                && dev->descriptor.idProduct == PID)
            {
                devHandle = usb_open(dev);
                if (devHandle != nullptr)
                {
#if defined(__APPLE__) || defined(__linux__)

                    int claimResult = usb_claim_interface(devHandle, 1);
                    if (claimResult != 0) {
                        printf("usb_claim_interface failed: %d\n", claimResult);
                    }

#endif
                    usb_list = (struct UsbList_t*) malloc(sizeof(struct UsbList_t));
                    usb_list->pDevHandle = devHandle;
                    usb_list->pDev = dev;
                    if (dev->descriptor.iSerialNumber)
                    {
                        int ret = usb_get_string_simple(devHandle, dev->descriptor.iSerialNumber,
                                                        usb_list->Serial, 128);
                        if (ret > 0)
                            USB_ListAdd(usb_list);
                    }
                }
            }
        }
    }
    return (_devIndex < USB_ListLength());
}


bool CheckDevice(int _devIndex)
{
    struct UsbList_t* usbList;
    int devNum;
    if (needInit)
    {
        devNum = USB_ScanDevice(PID, VID);
        if (devNum <= 0)
            return false;
    }
    usbList = USB_ListPost(_devIndex);

    if ((usbList != nullptr) && (usbList->pDevHandle != nullptr))
    {
        if (usb_close(usbList->pDevHandle) != 0)
            return false;
        return ((usbList->pDevHandle = usb_open(usbList->pDev)) != nullptr);
    } else
    {
        return false;
    }
}


DLL_API
bool USB_OpenDevice(int _devIndex)
{
    return OpenDevice(_devIndex);
}


DLL_API
bool USB_CheckDevice(int _devIndex)
{
    return CheckDevice(_devIndex);
}


bool CloseDevice(int _devIndex)
{
    struct UsbList_t* usbList;
    int ret;
    int devNum;
    if (needInit)
    {
        devNum = USB_ScanDevice(PID, VID);
        if (devNum <= 0)
            return false;
    }
    usbList = USB_ListPost(_devIndex);
    if ((usbList != nullptr) && (usbList->pDevHandle != nullptr))
    {
        ret = usb_close(usbList->pDevHandle);
        usbList->pDevHandle = nullptr;
        return (ret == 0);
    } else
    {
        return false;
    }
}


DLL_API
bool USB_CloseDevice(int _devIndex)
{
    return CloseDevice(_devIndex);
}


DLL_API
bool USB_BulkTransmit(int _devIndex, int _pipeNum, char* _sendBuffer, int _len, int _timeout)
{
    int ret;
    pUsbDeviceHandle = USB_ListPost(_devIndex)->pDevHandle;
    if (pUsbDeviceHandle == nullptr)
        return false;

#ifdef TEST_INTERFACE
    if (usb_claim_interface(pUsbDeviceHandle, 0) < 0)
    {
        usb_close(pUsbDeviceHandle);
        return false;
    }
#endif

    ret = usb_bulk_write(pUsbDeviceHandle, _pipeNum, _sendBuffer, _len, _timeout);
    if ((_len % 512) == 0)
        usb_bulk_write(pUsbDeviceHandle, _pipeNum, _sendBuffer, 0, _timeout);

#ifdef TEST_INTERFACE
    usb_release_interface(pUsbDeviceHandle, 0);
#endif
    return ret == _len;
}


DLL_API
bool USB_IntTransmit(int _devIndex, int _pipeNum, char* _sendBuffer, int _len, int _timeout)
{
    int ret;
    pUsbDeviceHandle = USB_ListPost(_devIndex)->pDevHandle;
    if (pUsbDeviceHandle == nullptr)
        return false;

#ifdef TEST_INTERFACE
    if (usb_claim_interface(pUsbDeviceHandle, 0) < 0)
    {
        usb_close(pUsbDeviceHandle);
        return false;
    }
#endif
    ret = usb_interrupt_write(pUsbDeviceHandle, _pipeNum, _sendBuffer, _len, _timeout);
#ifdef TEST_INTERFACE
    usb_release_interface(pUsbDeviceHandle, 0);
#endif
    return ret == _len;
}


DLL_API
bool USB_CtrlData(int _devIndex, int _requestType, int _request, int _value,
                  int _index, char* _bytes, int _size, int _timeout)
{
    int ret;
    pUsbDeviceHandle = USB_ListPost(_devIndex)->pDevHandle;
    if (pUsbDeviceHandle == nullptr)
        return false;

#ifdef TEST_INTERFACE
    if (usb_claim_interface(pUsbDeviceHandle, 0) < 0)
    {
        usb_close(pUsbDeviceHandle);
        return false;
    }
#endif
    ret = usb_control_msg(pUsbDeviceHandle, _requestType, _request, _value, _index, _bytes, _size, _timeout);

#ifdef TEST_INTERFACE
    usb_release_interface(pUsbDeviceHandle, 0);
#endif
    return ret == _size;
}


DLL_API
int USB_BulkReceive(int _devIndex, int _pipeNum, char* _readBuffer, int _len, int _timeout)
{
    int ret;
    pUsbDeviceHandle = USB_ListPost(_devIndex)->pDevHandle;
    if (pUsbDeviceHandle == nullptr)
        return 0;

#ifdef TEST_INTERFACE
    if (usb_claim_interface(pUsbDeviceHandle, 0) < 0)
    {
        usb_close(pUsbDeviceHandle);
        return 0;
    }
#endif

    ret = usb_bulk_read(pUsbDeviceHandle, _pipeNum, _readBuffer, _len, _timeout);

#ifdef TEST_INTERFACE
    usb_release_interface(pUsbDeviceHandle, 0);
#endif

    return ret;
}


DLL_API
int USB_IntReceive(int _devIndex, int _pipeNum, char* _readBuffer, int _len, int _timeout)
{
    int ret;
    pUsbDeviceHandle = USB_ListPost(_devIndex)->pDevHandle;
    if (pUsbDeviceHandle == nullptr)
        return 0;

#ifdef TEST_INTERFACE
    if (usb_claim_interface(pUsbDeviceHandle, 0) < 0)
    {
        usb_close(pUsbDeviceHandle);
        return 0;
    }
#endif
    ret = usb_interrupt_read(pUsbDeviceHandle, _pipeNum, _readBuffer, _len, _timeout);
#ifdef TEST_INTERFACE
    usb_release_interface(pUsbDeviceHandle, 0);
#endif

    return ret;
}