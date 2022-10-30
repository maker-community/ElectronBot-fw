//
// Created by Administrator on 2022/8/5.
//
#include "protocol.h"
#include "iap.h"
#include "crcLib.h"
/* USER CODE END PV */





void test_protocol()
{
    int i=1;
}

flashSave_t flashSave;
//ElectronBotJointStatus_t ElectronBotjoint[6];
JointStatus_t joint[7];
ProtocolItem_t ProtocolItem;
txbuf_t txbuf;
rxbuf_t rxbuf;

/*
int fputc(int ch,FILE *f)
{
    uint8_t temp[1] = {(uint8_t)ch};
    //HAL_UART_Transmit(&huart2, temp, 1, 2);
    HAL_UART_Transmit(&huart1,temp, 1,10);
    return ch;
}

void myPrintf(const char* format, ...)
{
    printf(format);
}

#define  myDebugEn 0
void myDebug(const char* format, ...)
{
#if  myDebugEn
    printf(format);
#endif
}
*/

/*
void SaveJointStatusToFlash(uint8_t *buf ,uint16_t len)
{
    //STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
    uint16_t address32Len=len/4;
    if(address32Len%4!=0)address32Len++;

    STMFLASH_Write(FLASH_JointStatusData, (uint32_t *)buf,len);
}

void ReadJointStatusFromFlash(uint8_t *buf ,uint16_t len)
{
    //STMFLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
    //STMFLASH_Write(FLASH_JointStatusData, (uint32_t *)buf,len);
    //STMFLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead);
    uint16_t address32Len=len/4;
    if(address32Len%4!=0)address32Len++;
    STMFLASH_Read(FLASH_JointStatusData,(uint32_t *)buf,len);
}*/


void BufClear(uint8_t* buf, uint8_t value, uint16_t len)
{
    memset(buf, value, len);
}

uint8_t CheckSum(uint8_t* buf, uint16_t dataAreaLen)
{
    uint16_t i = 0, checkLen = 0;
    uint8_t* checkBuf;
    uint8_t checkValue = 0;
    uint32_t sum = 0;

    checkBuf = &buf[1];
    checkLen = dataAreaLen + 8;
    //i=dataAreaLen+8;
    for (i = 0; i < checkLen; i++)
    {
        sum = sum + checkBuf[i];
    }
    checkValue = sum % 256;
    return checkValue;
}


void ComposeProtocolFrame(uint8_t* frameBuf, uint16_t* frameLen, ProtocolItem_t* items)
{
    uint16_t len = 0, itemLen = 0;
    uint8_t checkValue = 0;

    frameBuf[0] = FrameHead;
    len++;

    itemLen = sizeof(items->ElectronBotID);
    memcpy(&frameBuf[len], &items->ElectronBotID, itemLen);
    len += itemLen;

    if (items->frame == ResponseFrame)
    {
        items->cmd = items->cmd | 0x80;
    }
    itemLen = sizeof(items->cmd);
    memcpy(&frameBuf[len], &items->cmd, itemLen);
    len += itemLen;

    itemLen = sizeof(items->jointID);
    memcpy(&frameBuf[len], &items->jointID, itemLen);
    len += itemLen;

    itemLen = sizeof(items->dataLen);
    memcpy(&frameBuf[len], &items->dataLen, itemLen);
    len += itemLen;

    //itemLen=sizeof(items->dataLen);
    if (items->dataLen > 0)
    {
        memcpy(&frameBuf[len], items->data, items->dataLen);
        len += items->dataLen;
    }

    checkValue = CheckSum(frameBuf, items->dataLen);
    frameBuf[len] = checkValue;
    len++;

    frameBuf[len] = FrameTail;
    len++;

    *frameLen = len;
}


//查找inBuf里是否有协议帧,有则将协议帧拷贝到outBuf
bool ProtocolLookUp(uint8_t* frameBuf, uint8_t* inBuf, uint16_t inLen)
{
    uint16_t i;
    uint16_t datalen = 0;
    uint8_t checkValue = 0;
    if (inLen < 11)
    {
        return false;
    }
    for (i = 0; i < inLen; i++)
    {
        if (inBuf[i] == FrameHead && inLen - i >= 11)
        {
            datalen = inBuf[i + 7] + inBuf[i + 8] * 256;
            if (inBuf[i + 7 + 2 + datalen + 2 - 1] == FrameTail)
            {
                checkValue = inBuf[i + 7 + 2 + datalen + 1 - 1];
                if (checkValue == CheckSum(&inBuf[i], datalen))
                {
                    memcpy(frameBuf, &inBuf[i], 7 + 2 + datalen + 2);
                    // *outframeHead = i;
                    return true;
                    // ProtocolProcessing(rxbuf);
                }
            }
        }
    }
    return false;
}

//此函数可以自己重新写
//extern Robot electron();
//extern Robot electron(&hspi1, &hi2c1);
//extern Robot electron;
#define myPrintf printf
#define CMD_IAP                  0x0c


extern struct iap_t iap;
extern uint8_t IAP_OK;

extern iapFlashSave_t iapFlashSave;
uint8_t bingpacketbuf[10][460]={0};


void IAP_Clean(void )
{
    memset((uint8_t*)&iap,0,sizeof(iap));
    memset((uint8_t*)&iapFlashSave,0,sizeof(iapFlashSave));
}
void ProtocolProcessing(uint8_t* buf, uint16_t len)
{
    uint8_t frameBuf[1024] = { 0 };
    uint8_t idbuf = 0;
    ElectronBotJointStatus_t* test;
    ElectronBotJointStatus_t rx,local,*p;
    p=&rx;
    BufClear((uint8_t *)p,0,sizeof(ElectronBotJointStatus_t));
    p=&local;
    BufClear((uint8_t *)p,0,sizeof(ElectronBotJointStatus_t));

    if (ProtocolLookUp(frameBuf, buf, len) == false)
    {
        myPrintf("\r\n  No Protocol!!!!!!! \r\n");
        return;
    }

    ProtocolItem.ElectronBotID = frameBuf[1] + frameBuf[2] * 256 + frameBuf[3] * 256 * 256 + frameBuf[4] * 256 * 256 * 256;
    ProtocolItem.cmd = frameBuf[5];
    ProtocolItem.jointID = frameBuf[6];
    ProtocolItem.dataLen = frameBuf[7] + frameBuf[8] * 256;
    ProtocolItem.data = &frameBuf[9];

  //  myPrintf("ElectronBotID=%d\r\n", ProtocolItem.ElectronBotID);
  //  myPrintf("jointID=%d\r\n", ProtocolItem.jointID);
  //  myPrintf("dataLen=%d\r\n", ProtocolItem.dataLen);

    test = (ElectronBotJointStatus_t*)ProtocolItem.data;

    idbuf = ProtocolItem.jointID / 2;
    uint32_t *PI,POS=0,crc32Valve=0;
    if (ProtocolItem.cmd == CMD_IAP || ProtocolItem.cmd == (CMD_IAP |0x80))
    {
        PI=(uint32_t *)&frameBuf[9];
        iap.currentPacketAddress=PI[0];
        iap.packetSum=PI[1];
        iap.currentPacket=PI[2];
        iap.currentPacketLen=PI[3];
        crc32Valve=PI[4];
        if(iap.currentPacket==0)
        {
            iap.crc32=CRC32_INIT_VALUE;
            iap.crc32Packet=0;
            IAP_FlashErase2(iap.currentPacketAddress, (0x08010000 + iap.packetSum * IAP_PACKET_MAX_DATA_LEN));
        }

       // if(iap.currentPacket<10)
       // {
       //     memcpy(&bingpacketbuf[iap.currentPacket][0],&frameBuf[9+16],iap.currentPacketLen);
       // }

        if(iap.currentPacket<(iap.packetSum))
        {
            if(iap.currentPacket>0)
            {
                if(iap.currentPacket>iap.crc32Packet)
                {
                    iap.crc32 = crc32(iap.crc32, &frameBuf[9 + 20], iap.currentPacketLen);
                    iap.crc32Packet = iap.currentPacket;
                }
            } else
            {
                iap.crc32 = crc32(iap.crc32, &frameBuf[9 + 20], iap.currentPacketLen);
                iap.crc32Packet = iap.currentPacket;
            }
        }

        //放开每包的打印信息更有感觉？
       // printf("crc32Valve is 0x%08x\r\n",crc32Valve);
       // printf("iap.currentPacket is %d, iap.crc32 is 0x%08x\r\n",iap.currentPacket,iap.crc32);
       // HAL_Delay(20);

        if(crc32Valve==iap.crc32)
        {
            WriteFlashu8tou32(iap.currentPacketAddress, &frameBuf[9 + 20], iap.currentPacketLen);

            uint8_t dataBuf[sizeof(iap)] = {0};
            memcpy(dataBuf, &iap, sizeof(iap));

            ProtocolItem.data = dataBuf;
            ProtocolItem.dataLen = 20;
            ProtocolItem.frame = ResponseFrame;
            ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

            HAL_UART_Transmit(&huart1, (uint8_t *) &txbuf.buf, txbuf.dataLen, 0xFFFF);
        }
        //electron.SendUsbPacket(frameBuf,sizeof(frameBuf));
        // electron.SendUsbPacket(txbuf.buf,txbuf.dataLen);


         if(iap.currentPacket==(iap.packetSum-1))
        {
            IAP_OK=1;

            IAP_FlashErase3();
            iapFlashSave.startAddress=APPLICATION_ADDRESS;
            iapFlashSave.endAddress=iap.currentPacketAddress+iap.currentPacketLen;
            iapFlashSave.packetSum=iap.packetSum;
            iapFlashSave.binCrc32Valve=iap.crc32;
            iapFlashSave.lastPacketLen=iap.currentPacketLen;
            iapFlashSave.iapOkFlag=IAP_OK_FLAG_VALVE;

            WriteFlashu8tou32(IAP_FLASH_SAVE_ADDRESS,(uint8_t*)&iapFlashSave,sizeof(iapFlashSave));
            HAL_Delay(50);
            IAP_Clean();

            HAL_Delay(100);
            //myPrintf("IAP ok SystemReset！！！！\r\n");
            myPrintf("app file download complete \r\nsystem reset！！！！\r\n");
            HAL_Delay(100);
            HAL_NVIC_SystemReset();

            /*for(uint8_t i=0;i<10;i++)
            {
                printf("bingpacketbuf %d\r\rn",i);
                HAL_Delay(10);
                for(int j=0;j<460;j++)
                {
                    printf("%02x ",bingpacketbuf[i][j]);//向串口打印接收到的数据
                }
                HAL_Delay(30);
            }*/
        }

        BufClear((uint8_t* )&txbuf,0,sizeof(txbuf));
    }

    if(ProtocolItem.cmd == CMD_ResetMaster)
    {
        HAL_Delay(100);
        myPrintf("system reset！！！！\r\n");
        HAL_Delay(100);
        HAL_NVIC_SystemReset();
    }
}

void VersionSubmission(char * buf,uint8_t len)
{
    ProtocolItem.ElectronBotID=0;
    ProtocolItem.jointID=0;
    ProtocolItem.cmd=CMD_VersionSubmission;
    ProtocolItem.data = (uint8_t*)buf;
    ProtocolItem.dataLen = len;
    ProtocolItem.frame = ResponseFrame;
    ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);

    HAL_UART_Transmit(&huart1, (uint8_t *) &txbuf.buf, txbuf.dataLen, 0xFFFF);
    HAL_Delay(20);
}

/*
void JointStatusUpdata(void)
{
    ElectronBotJointStatus_t flashSaved;
    int i=0;

    ReadJointStatusFromFlash((uint8_t *)&flashSave,sizeof(flashSave_t));

    if(flashSave.saveFlag!=0x5555aaaa)
    {
        for(i=1;i<7;i++)
        {

            flashSave.ElectronBotjoint[i].angleMin = electron.joint[i].angleMin;
            flashSave.ElectronBotjoint[i].angleMax = electron.joint[i].angleMax;

            flashSave.ElectronBotjoint[i].modelAngelMax=electron.joint[i].modelAngelMax;
            flashSave.ElectronBotjoint[i].modelAngelMin=electron.joint[i].modelAngelMin;

            //flashSave.ElectronBotjoint[i].enable=electron.joint[i].enable;
            flashSave.ElectronBotjoint[i].inverted=electron.joint[i].inverted;

            flashSave.ElectronBotjoint[i].enable=false;
            flashSave.ElectronBotjoint[i].torqueLimit=0.5;
            flashSave.ElectronBotjoint[i].kp=10;
            flashSave.ElectronBotjoint[i].ki=0;
            flashSave.ElectronBotjoint[i].kv=0;
            flashSave.ElectronBotjoint[i].kd=50;

            electron.UpdateJointAngle(electron.joint[i], 0);
        }
    }
    else
    {
        for(i=1;i<7;i++)
        {
            memcpy(&flashSaved,&flashSave.ElectronBotjoint[i],sizeof(ElectronBotJointStatus_t));
            electron.joint[i].angleMin=flashSaved.angleMin;
            electron.joint[i].angleMax=flashSaved.angleMax;

            electron.joint[i].modelAngelMax =flashSaved.modelAngelMax;
            electron.joint[i].modelAngelMin=flashSaved.modelAngelMin;

            electron.joint[i].inverted=flashSaved.inverted;

            electron.UpdateJointAngle(electron.joint[i], 0);//

            electron.SetJointInitAngle(electron.joint[i], flashSaved.initAngle);
            electron.SetJointTorqueLimit(electron.joint[i], flashSaved.torqueLimit);
            electron.SetJointKp(electron.joint[i],flashSaved.kp);
            electron.SetJointKi(electron.joint[i],flashSaved.ki);
            electron.SetJointKv(electron.joint[i],flashSaved.kv);
            electron.SetJointKd(electron.joint[i],flashSaved.kd);

            electron.SetJointEnable(electron.joint[i],flashSaved.kd);
        }
    }
}*/


void PrintfElectronBotJointStatus(ElectronBotJointStatus_t * Status,uint8_t id)
{

    myPrintf("*******joint%dStatus******\r\n",id);
    HAL_Delay(100);
    myPrintf("angleMin=%f\r\n", Status->angleMin);
    HAL_Delay(100);
    myPrintf("angleMax=%f\r\n", Status->angleMax);
    HAL_Delay(100);
    myPrintf("angle=%f\r\n", Status->angle);
    HAL_Delay(100);
    myPrintf("modelAngelMin=%f\r\n", Status->modelAngelMin);
    HAL_Delay(100);
    myPrintf("modelAngelMax=%f\r\n", Status->modelAngelMax);
    HAL_Delay(100);
    myPrintf("inverted=%s\r\n", Status->inverted ? "true" : "false");
    HAL_Delay(100);
    myPrintf("initAngle=%f\r\n", Status->initAngle);
    HAL_Delay(100);
    myPrintf("torqueLimit=%f\r\n", Status->torqueLimit);
    HAL_Delay(100);
    myPrintf("kp=%f\r\n", Status->kp);
    HAL_Delay(100);
    myPrintf("ki=%f\r\n", Status->ki);
    HAL_Delay(100);
    myPrintf("kv=%f\r\n", Status->kv);
    HAL_Delay(100);
    myPrintf("kd=%f\r\n", Status->kd);
    HAL_Delay(100);
    myPrintf("enable=%s\r\n", Status->enable ? "true" : "false");
    HAL_Delay(100);
}

void JointStatusUpdata(void)
{

}


uint8_t testbuf[300] = { 0 };
uint8_t testbuf2[300] = { 0 };

void testComposeProtocolFrame()
{
    ElectronBotJointStatus_t test, * p;

    p = &test;
    BufClear((uint8_t*)p, 0, sizeof(ElectronBotJointStatus_t));

    test.angleMin = 10;
    test.angleMax = 30;
    test.angle = 0;
    test.modelAngelMin = 12;
    test.modelAngelMax = 32;
    test.inverted = true;
    test.initAngle = 15;
    test.torqueLimit = 0.5;
    test.kp = 20;
    test.ki = 1;
    test.kv = 2;
    test.kd = 3;
    test.enable = true;

    memcpy((uint8_t*)testbuf, (uint8_t*)p, sizeof(ElectronBotJointStatus_t));

    ProtocolItem_t* p2;
    p2 = &ProtocolItem;
    BufClear((uint8_t*)p2, 0, sizeof(ProtocolItem_t));

    ProtocolItem.ElectronBotID = 12345678;
    ProtocolItem.cmd = CMD_WriteAllJointStatus;
    ProtocolItem.jointID = 12;
    ProtocolItem.dataLen = sizeof(ElectronBotJointStatus_t);
    ProtocolItem.data = testbuf;
    ProtocolItem.frame = CommandFrame;

    ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen ,&ProtocolItem);//组帧
    ProtocolProcessing(txbuf.buf, txbuf.dataLen);//解帧

    BufClear((uint8_t*)&txbuf, 0, sizeof(txbuf));
}

//uint8_t frameBuf[1024] = {0};
void testReceiveMasterUsbData(uint8_t *buf, uint16_t len)
{
    //uint8_t frameBuf[200] = {0};
    uint8_t frameBuf[400] = {0};
    ElectronBotJointStatus_t rx,local,*p;
    p=&rx;
    BufClear((uint8_t *)p,0,sizeof(ElectronBotJointStatus_t));
    p=&local;
    BufClear((uint8_t *)p,0,sizeof(ElectronBotJointStatus_t));

    if (ProtocolLookUp(frameBuf, buf, len) == false)
    {
        myPrintf("\r\n testReceiveMasterUsbData() not found protocol\r\n");
        return;
    }

    //uint8_t *buf;
    //buf=&rxbuf.buf[rxbuf.frameHead];
    //buf=inbuf;

    ProtocolItem.ElectronBotID=frameBuf[1]+frameBuf[2]*256+frameBuf[3]*256*256+frameBuf[4]*256*256*256;
    ProtocolItem.cmd=frameBuf[5];
    ProtocolItem.jointID=frameBuf[6];
    ProtocolItem.dataLen=frameBuf[7]+frameBuf[8]*256;
    ProtocolItem.data=&frameBuf[9];
    ProtocolItem.SaveEn=false;

    memcpy(&local,ProtocolItem.data,sizeof(ElectronBotJointStatus_t));

    myPrintf("ElectronBotID=%d\r\n",ProtocolItem.ElectronBotID);
    HAL_Delay(200);
    myPrintf("cmd=%d\r\n",ProtocolItem.cmd);
    HAL_Delay(200);
    myPrintf("jointID=%d\r\n",ProtocolItem.jointID);
    HAL_Delay(200);
    myPrintf("angleMin=%f\r\n",local.angleMin);
    //char str[80];
    // smyPrintf(str,"angleMin=%f\r\n",local.angleMin);
    // HAL_UART_Transmit(&huart1,(uint8_t *)str, strlen(str),10);

    HAL_Delay(200);
    myPrintf("angleMax=%f\r\n",local.angleMax);
    HAL_Delay(200);
    myPrintf("angle=%f\r\n",local.angle);
    HAL_Delay(200);
    myPrintf("modelAngelMin=%f\r\n",local.modelAngelMin);
    HAL_Delay(200);
    myPrintf("modelAngelMax=%f\r\n",local.modelAngelMax);
    HAL_Delay(200);
    //myPrintf("inverted=%d\r\n",local.inverted);
    myPrintf("inverted=%s\r\n",local.inverted?"true" :"false");
    HAL_Delay(200);
    myPrintf("initAngle=%f\r\n",local.initAngle);
    HAL_Delay(200);
    myPrintf("torqueLimit=%f\r\n",local.torqueLimit);
    HAL_Delay(200);
    myPrintf("kp=%f\r\n",local.kp);
    HAL_Delay(200);
    myPrintf("ki=%f\r\n",local.ki);
    HAL_Delay(200);
    myPrintf("kv=%f\r\n",local.kv);
    HAL_Delay(200);
    myPrintf("kd=%f\r\n",local.kd);
    HAL_Delay(200);
    myPrintf("enable=%s\r\n",local.enable?"true" :"false");
    HAL_Delay(200);

}