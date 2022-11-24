//
// Created by Administrator on 2022/10/5.
//
#include "iap.h"
#include "crcLib.h"



struct iap_t iap;

//typedef  void (*pFunction)(void);
//pFunction Jump_To_Application;
//uint32_t JumpAddress;
//#define APPLICATION_ADDRESS   (uint32_t)0x08004000

uint8_t IAP_OK=0;

struct iapFlashSave_t iapFlashSave;

/* Define the address from where user application will be loaded.
   Note: the 1st sector 0x08000000-0x08003FFF is reserved for the IAP code */
//#define APPLICATION_ADDRESS   (uint32_t)0x08004000

void IAP_Init(void)
{
    iap.currentPacket=0;
    iap.currentPacketAddress=0;
    iap.currentPacketLen=0;
    iap.packetSum=0;
}



#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH的起始地址
#define FLASH_WAITETIME  50000          //FLASH等待超时时间
typedef __IO uint32_t  vu32;

u32 STMFLASH_ReadWord(u32 faddr)
{
    return *(vu32*)faddr;
}

//获取某个地址所在的flash扇区
//addr:flash地址
//返回值:0~11,即addr所在的扇区
u8 STMFLASH_GetFlashSector(u32 addr)
{
    if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_0;
    else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_1;
    else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_2;
    else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_3;
    else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_4;
    else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_5;
    else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_6;
    else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_7;
    else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_8;
    else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_9;
    else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_10;
    return FLASH_SECTOR_11;
}

/*
void IAP_FlashErase(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
//void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;
    u32 addrx=0;
    u32 endaddr=0;
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址

    HAL_FLASH_Unlock();             //解锁
    addrx=WriteAddr;				//写入的起始地址
    endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址

    if(addrx<0X1FFF0000)
    {
        while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
            {
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
                FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
                if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
                {
                    break;//发生错误了
                }
            }else addrx+=4;
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
        }
    }
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
    if(FlashStatus==HAL_OK)
    {
        while(WriteAddr<endaddr)//写数据
        {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
            {
                break;	//写入异常
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    HAL_FLASH_Lock();           //上锁
}*/

void IAP_FlashErase()
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;
    HAL_FLASH_Unlock();             //解锁


                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
                FlashEraseInit.Sector=FLASH_SECTOR_4;   //要擦除的扇区
                FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
                if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
                {
                    //break;//发生错误了
                }
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成


    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    FlashEraseInit.Sector=FLASH_SECTOR_5;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成


    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    FlashEraseInit.Sector=FLASH_SECTOR_6;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成


    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    FlashEraseInit.Sector=FLASH_SECTOR_7;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成


    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    FlashEraseInit.Sector=FLASH_SECTOR_8;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成

    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    FlashEraseInit.Sector=FLASH_SECTOR_9;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成

    HAL_FLASH_Lock();           //上锁
}

void IAP_FlashErase2(uint32_t startAddress ,uint32_t endAddress)
{
    uint8_t  startSector=5,endSector=5;
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;

    printf("start erasing flash ,just a moment\r\n");
    HAL_Delay(20);

    startSector=STMFLASH_GetFlashSector(startAddress);
    endSector=STMFLASH_GetFlashSector(endAddress);

   // printf("starAddress is %08x, endAddress is%08x\r\n",starAddress,endAddress);
   // HAL_Delay(20);

    printf("start sector is 0x%02x, end sector is 0x%02x\r\n",startSector,endSector);
    HAL_Delay(20);

    HAL_FLASH_Unlock();             //解锁
    for(uint8_t i=startSector;i<=endSector;i++)
    {
        FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
       // FlashEraseInit.Sector=FLASH_SECTOR_4;   //要擦除的扇区
        FlashEraseInit.Sector=i;   //要擦除的扇区
        FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
        FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
        if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
        {
            //break;//发生错误了
        }
        FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
    }
    HAL_FLASH_Lock();
}

void IAP_FlashErase3()
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;

    HAL_FLASH_Unlock();             //解锁
    FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
    // FlashEraseInit.Sector=FLASH_SECTOR_4;   //要擦除的扇区
    FlashEraseInit.Sector=FLASH_SECTOR_10;   //要擦除的扇区
    FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
    FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
    if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
    {
        //break;//发生错误了
    }
    FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
    HAL_FLASH_Lock();
}

//从指定地址开始写入指定长度的数据
//特别注意:因为STM32F4的扇区实在太大,没办法本地保存扇区数据,所以本函数
//         写地址如果非0XFF,那么会先擦除整个扇区且不保存扇区数据.所以
//         写非0XFF的地址,将导致整个扇区数据丢失.建议写之前确保扇区里
//         没有重要数据,最好是整个扇区先擦除了,然后慢慢往后写.
//该函数对OTP区域也有效!可以用来写OTP区!
//OTP区域地址范围:0X1FFF7800~0X1FFF7A0F(注意：最后16字节，用于OTP数据块锁定，别乱写！！)
//WriteAddr:起始地址(此地址必须为4的倍数!!)
//pBuffer:数据指针
//NumToWrite:字(32位)数(就是要写入的32位数据的个数.)
void STMFLASH_Write(u32 WriteAddr,u32 *pBuffer,u32 NumToWrite)
{
    FLASH_EraseInitTypeDef FlashEraseInit;
    HAL_StatusTypeDef FlashStatus=HAL_OK;
    u32 SectorError=0;
    u32 addrx=0;
    u32 endaddr=0;
    if(WriteAddr<STM32_FLASH_BASE||WriteAddr%4)return;	//非法地址

    HAL_FLASH_Unlock();             //解锁
    addrx=WriteAddr;				//写入的起始地址
    endaddr=WriteAddr+NumToWrite*4;	//写入的结束地址

   /* if(addrx<0X1FFF0000)
    {
        while(addrx<endaddr)		//扫清一切障碍.(对非FFFFFFFF的地方,先擦除)
        {
            if(STMFLASH_ReadWord(addrx)!=0XFFFFFFFF)//有非0XFFFFFFFF的地方,要擦除这个扇区
            {
                FlashEraseInit.TypeErase=FLASH_TYPEERASE_SECTORS;       //擦除类型，扇区擦除
                FlashEraseInit.Sector=STMFLASH_GetFlashSector(addrx);   //要擦除的扇区
                FlashEraseInit.NbSectors=1;                             //一次只擦除一个扇区
                FlashEraseInit.VoltageRange=FLASH_VOLTAGE_RANGE_3;      //电压范围，VCC=2.7~3.6V之间!!
                if (HAL_FLASHEx_Erase(&FlashEraseInit, &SectorError) != HAL_OK)
                {
                    break;//发生错误了
                }
            }else addrx+=4;
            FLASH_WaitForLastOperation(FLASH_WAITETIME);                //等待上次操作完成
        }
    }*/
    FlashStatus=FLASH_WaitForLastOperation(FLASH_WAITETIME);            //等待上次操作完成
    if(FlashStatus==HAL_OK)
    {
        while(WriteAddr<endaddr)//写数据
        {
            if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD,WriteAddr,*pBuffer)!=HAL_OK)//写入数据
            {
                break;	//写入异常
            }
            WriteAddr+=4;
            pBuffer++;
        }
    }
    HAL_FLASH_Lock();           //上锁
}

//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToRead:字(32位)数
void STMFLASH_Read(u32 ReadAddr,u32 *pBuffer,u32 NumToRead)
{
    u32 i;
    for(i=0;i<NumToRead;i++)
    {
        pBuffer[i]=STMFLASH_ReadWord(ReadAddr);//读取4个字节.
        ReadAddr+=4;//偏移4个字节.
    }
}

//////////////////////////////////////////测试用///////////////////////////////////////////
//WriteAddr:起始地址
//WriteData:要写入的数据
void Test_Write(u32 WriteAddr,u32 WriteData)
{
    STMFLASH_Write(WriteAddr,&WriteData,1);//写入一个字
}

void WriteFlashu8tou32(u32 WriteAddr,u8 *pBuffer,u32 u8DataLen)
{
    u32 u32DataLen=0;
    u32DataLen=u8DataLen/4;
    if((u8DataLen%4)>0)
    {
        u32DataLen++;
    }
    STMFLASH_Write(WriteAddr,(u32*)pBuffer,u32DataLen);
}

void ReadFlashu8tou32(u32 WriteAddr,u8 *pBuffer,u32 u8DataLen)
{
    u32 u32DataLen=0;
    u32DataLen=u8DataLen/4;
    if((u8DataLen%4)>0)
    {
        u32DataLen++;
    }
    STMFLASH_Read(WriteAddr,(u32*)pBuffer,u32DataLen);
}

void IAP_ReadIapStatusFromFlash()
{
    uint32_t len=sizeof (iapFlashSave);
    uint16_t address32Len=len/4;
    if(address32Len%4!=0)address32Len++;
    STMFLASH_Read(IAP_FLASH_SAVE_ADDRESS,(uint32_t *)&iapFlashSave,len);
}
#define  CRC32_INIT_VALUE 0xFFFFFFFF


uint8_t IAP_CrcCheck(void)
{
    uint8_t buf[IAP_PACKET_MAX_DATA_LEN]={0};

    iapFlashSave.crc32=CRC32_INIT_VALUE;
    for(uint32_t i=0;i<iapFlashSave.packetSum;i++ )
    {
        iapFlashSave.currentPacket=i;
        iapFlashSave.currentPacketAddress=(APPLICATION_ADDRESS+iapFlashSave.currentPacket*IAP_PACKET_MAX_DATA_LEN);
        if(iapFlashSave.currentPacket==(iapFlashSave.packetSum-1))
        {
            iapFlashSave.currentPacketLen=iapFlashSave.lastPacketLen;
            ReadFlashu8tou32(iapFlashSave.currentPacketAddress,&buf,iapFlashSave.currentPacketLen);
            iapFlashSave.crc32 = crc32(iapFlashSave.crc32, buf, iapFlashSave.currentPacketLen);
        }
        else
        {
            iapFlashSave.currentPacketLen=IAP_PACKET_MAX_DATA_LEN;
            ReadFlashu8tou32(iapFlashSave.currentPacketAddress,buf,iapFlashSave.currentPacketLen);
            iapFlashSave.crc32 = crc32(iapFlashSave.crc32, buf, iapFlashSave.currentPacketLen);
        }

       // iapFlashSave.currentPacketAddress=(iapFlashSave.currentPacket*IAP_PACKET_MAX_DATA_LEN);
        //STMFLASH_Read(iapFlashSave.currentPacketAddress,(uint32_t *)&buf,(iapFlashSave.currentPacketLen/4));
        //ReadFlashu8tou32(iapFlashSave.currentPacketAddress,&buf,iapFlashSave.currentPacketLen);


        iapFlashSave.crc32Packet = iapFlashSave.currentPacket;
    }

    //printf("iapFlashSave.crc32=0x%08x",iapFlashSave.crc32);
    printf("app file crc32 value is 0x%08x\r\n",iapFlashSave.crc32);
    HAL_Delay(10);
    if(iapFlashSave.binCrc32Valve==iapFlashSave.crc32)
    {
        return 1;
    }
    return 0;
}


bool IAP_DownloadingIs()
{
    if (iap.packetSum > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
