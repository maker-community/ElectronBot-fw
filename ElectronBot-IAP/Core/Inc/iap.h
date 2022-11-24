//
// Created by Administrator on 2022/10/5.
//

#ifndef F405_USART1_IAP_H
#define F405_USART1_IAP_H
#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "stdbool.h"

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;



#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbyte */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbyte */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbyte */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbyte */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbyte */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbyte */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbyte */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbyte */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbyte */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbyte */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbyte */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbyte */

/* End of the Flash address */
#define USER_FLASH_END_ADDRESS        0x080FFFFF
/* Define the user application size */
#define USER_FLASH_SIZE   (USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS + 1)


#define  CRC32_INIT_VALUE   0xFFFFFFFF
#define  IAP_OK_FLAG_VALVE    0x55AAAA55
#define  IAP_APP_IAP_EN_FLAG  0x33CCCC33
#define  IAP_PACKET_MAX_DATA_LEN 460

struct iap_t
{
    uint32_t currentPacketAddress;
    uint32_t packetSum;
    uint32_t currentPacket;
    uint32_t currentPacketLen;
    uint32_t crc32;
    uint32_t crc32Packet;
};

#define IAP_FLASH_SAVE_ADDRESS ADDR_FLASH_SECTOR_10

struct iapFlashSave_t
{
    uint32_t startAddress;
    uint32_t endAddress;
    uint32_t packetSum;
    uint32_t currentPacket;
    uint32_t currentPacketLen;
    uint32_t currentPacketAddress;
    uint32_t lastPacketLen;
    uint32_t binCrc32Valve;
    uint32_t crc32Packet;
    uint32_t crc32;
    uint32_t appIapEnFlag;
    uint32_t iapOkFlag;
};


#define APPLICATION_ADDRESS   (uint32_t)0x08010000




void WriteFlashu8tou32(u32 WriteAddr,u8 *pBuffer,u32 u8DataLen);
void IAP_FlashErase();
void IAP_FlashErase2(uint32_t starAddress ,uint32_t endAddress);
void IAP_FlashErase3(void);
void IAP_ReadIapStatusFromFlash(void);
uint8_t IAP_CrcCheck(void);
void IAP_Clean(void );
bool IAP_DownloadingIs();

#ifdef __cplusplus
}
#endif
#endif //F405_USART1_IAP_H
