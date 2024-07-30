//
// Created by Administrator on 2022/8/5.
//
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "protocol.h"
#include "robot.h"
#include "myPrintf.h"

#define FLASH_JointStatusData ADDR_FLASH_SECTOR_11


struct RobotSlavesConnectionStatus_t  RobotSlavesConnectionStatus
{
    .joint2=false,
    .joint4=false,
    .joint6=false,
    .joint8=false,
    .joint10=false,
    .joint12=false,
    .mpu6050=false,
    .mpu6050Init=false,
    .paj7620=false,
    .paj7620Init=false,
};

/*
struct RobotSlavesConnectionStatus_t  RobotSlavesConnectionStatus
        {
                .joint2=true,
                .joint4=false,
                .joint6=true,
                .joint8=false,
                .joint10=false,
                .joint12=false,
                .mpu6050=false,
                .mpu6050Init=false,
                .paj7620=false,
                .paj7620Init=false,
        };
*/
void JointsConnectionStatusChange(uint8_t jointId,bool ConnectionStatus)
{
    uint8_t *P;
    P=(uint8_t* )&RobotSlavesConnectionStatus;
    if(jointId)
    {
        P[(jointId/2)-1]=ConnectionStatus;
    }
}

bool JointsConnectionStatusIs(uint8_t jointId)
{
    uint8_t *P;
    P=(uint8_t* )&RobotSlavesConnectionStatus;
    return P[jointId/2];
}

void MPU6050_ConnectionStatusChange(bool ConnectionStatus)
{
    RobotSlavesConnectionStatus.mpu6050=ConnectionStatus;
}

bool MPU6050_ConnectionStatusIs()
{
    return RobotSlavesConnectionStatus.mpu6050;
}

void MPU6050_InitStatusChange(bool InitStatus)
{
    RobotSlavesConnectionStatus.mpu6050Init=InitStatus;
}

bool MPU6050_InitStatusIs()
{
    return RobotSlavesConnectionStatus.mpu6050Init;
}


void PAJ7620_ConnectionStatusChange(bool ConnectionStatus)
{
    RobotSlavesConnectionStatus.paj7620=ConnectionStatus;
}

bool PAJ7620_ConnectionStatusIs()
{
    return RobotSlavesConnectionStatus.paj7620;
}

void PAJ7620_InitStatusChange(bool InitStatus)
{
    RobotSlavesConnectionStatus.paj7620Init=InitStatus;
}

bool PAJ7620_InitStatusIs()
{
    return RobotSlavesConnectionStatus.paj7620Init;
}


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
    if(len%4!=0)address32Len++;
    STMFLASH_Read(FLASH_JointStatusData,(uint32_t *)buf,address32Len);
}


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
extern Robot electron;
extern SemaphoreHandle_t xMutex_Uart1;



struct RotationTest_t RotationTest{.en_flag=false,};

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



    idbuf = ProtocolItem.jointID / 2;
    if (ProtocolItem.cmd == CMD_WriteAllJointStatus || ProtocolItem.cmd == (CMD_WriteAllJointStatus |0x80))
    {
        test = (ElectronBotJointStatus_t*)ProtocolItem.data;

        PrintfElectronBotJointStatus(test,ProtocolItem.jointID);
    }

    //uint8_t idbuf=0;
    //uint8_t *buf;
    // buf=&rxbuf.buf[rxbuf.frameHead];

    ProtocolItem.ElectronBotID=buf[1]+buf[2]*256+buf[3]*256*256+buf[4]*256*256*256;
    ProtocolItem.cmd=buf[5];
    ProtocolItem.jointID=buf[6];
    ProtocolItem.dataLen=buf[7]+buf[8]*256;
    ProtocolItem.data=&buf[9];
    ProtocolItem.SaveEn=false;

    if(ProtocolItem.cmd == CMD_WriteAllJointStatus )
    {
        idbuf=ProtocolItem.jointID/2;
        memcpy(&local,&flashSave.ElectronBotjoint[idbuf],sizeof(ElectronBotJointStatus_t));

        memcpy(&rx,ProtocolItem.data,sizeof(ElectronBotJointStatus_t));
        if(rx.angleMin!=local.angleMin)
        {
            electron.joint[idbuf].angleMin=rx.angleMin;
            local.angleMin=rx.angleMin;
            ProtocolItem.SaveEn = true;
        }

        if(rx.angleMax!=local.angleMax)
        {
            electron.joint[idbuf].angleMin=rx.angleMax;
            local.angleMax=rx.angleMax;
            ProtocolItem.SaveEn = true;
        }

        if(rx.modelAngelMin!=local.modelAngelMin)
        {
            electron.joint[idbuf].modelAngelMin=rx.modelAngelMin;
            local.modelAngelMin=rx.modelAngelMin;
            ProtocolItem.SaveEn = true;
        }

        if(rx.modelAngelMax!=local.modelAngelMax)
        {
            electron.joint[idbuf].modelAngelMax=rx.modelAngelMax;
            local.modelAngelMax=rx.modelAngelMax;
            ProtocolItem.SaveEn = true;
        }

        if(rx.torqueLimit!=local.torqueLimit)
        {
           // electron.SetJointTorqueLimit(electron.joint[idbuf], rx.torqueLimit);
           // HAL_Delay(20);
            local.torqueLimit=rx.torqueLimit;
            ProtocolItem.SaveEn = true;
        }

        if(rx.kp!=local.kp)
        {
           // electron.SetJointKp(electron.joint[idbuf],rx.kp);
            //HAL_Delay(20);
            local.kp=rx.kp;
            ProtocolItem.SaveEn = true;
        }

       /*
        *
        *
        * if(rx.angle!=local.angle)//屏蔽暂时不用的部分
        {
            electron.joint[idbuf].angle=rx.angle;
            ProtocolItem.SaveEn = true;
            //  electron.UpdateJointAngle(electron.joint[idbuf], rx.angle);
            HAL_Delay(20);
        }

        if(rx.initAngle!=local.initAngle)
        {
           // electron.SetJointInitAngle(electron.joint[idbuf], rx.initAngle);
            HAL_Delay(20);
            local.initAngle=rx.initAngle;
            ProtocolItem.SaveEn = true;
        }

        if(rx.ki!=local.ki)
        {
           // electron.SetJointKi(electron.joint[idbuf],rx.ki);
           // HAL_Delay(20);
            local.ki=rx.kp;
            ProtocolItem.SaveEn = true;
        }

        if(rx.kv!=local.kv)
        {
           // electron.SetJointKv(electron.joint[idbuf],rx.kv);
           // HAL_Delay(20);
            local.kv=rx.kv;
            ProtocolItem.SaveEn = true;
        }

        if(rx.kd!=local.kd)
        {
           // electron.SetJointKd(electron.joint[idbuf],rx.kd);
           // HAL_Delay(20);
            local.kd=rx.kd;
            ProtocolItem.SaveEn = true;
        }

        if(rx.enable!=local.enable)
        {
           // electron.SetJointEnable(electron.joint[idbuf],rx.enable);
          //  HAL_Delay(20);
            local.enable=rx.enable;
            ProtocolItem.SaveEn = true;
        }

        if(rx.inverted!=local.inverted)
        {
            // electron.joint[idbuf].inverted=rx.inverted;
            local.inverted=rx.inverted;
            ProtocolItem.SaveEn = true;
        }*/

        flashSave_t *P2;
        if(ProtocolItem.SaveEn == true)
        {
            ProtocolItem.SaveEn = false;
           // local.saveFlag = FlashSaveFlag;
            memcpy(&flashSave.ElectronBotjoint[idbuf],&local,sizeof(ElectronBotJointStatus_t));
            HAL_Delay(20);
            //p=&ElectronBotjoint[0];
           // p=ElectronBotjoint;
            flashSave.saveFlag[idbuf]=FlashSaveFlag;
            P2=&flashSave;
            SaveJointStatusToFlash((uint8_t *)&flashSave,sizeof(flashSave_t));
            //SaveJointStatusToFlash((uint8_t *)P2,sizeof(flashSave_t));
            //SaveJointStatusToFlash((uint8_t *)P2,100);
            //electron.SetJointEnable(electron.joint[0], true);

            ProtocolItem.dataLen=0;
            ProtocolItem.frame=ResponseFrame;
            ComposeProtocolFrame(txbuf.buf,&txbuf.dataLen,&ProtocolItem);

            //electron.SendUsbPacket(frameBuf,sizeof(frameBuf));
           // electron.SendUsbPacket(txbuf.buf,txbuf.dataLen);

            BufClear((uint8_t* )&txbuf,0,sizeof(txbuf));

        }
    }

    if(ProtocolItem.cmd == CMD_ReadAllJointStatus)
    {
        ProtocolItem.dataLen= sizeof(ElectronBotJointStatus_t);
        ProtocolItem.frame=ResponseFrame;
        ProtocolItem.data=(uint8_t*)&flashSave.ElectronBotjoint[idbuf];
        ComposeProtocolFrame(txbuf.buf,&txbuf.dataLen,&ProtocolItem);
    }

    if(ProtocolItem.cmd == CMD_WriteJointID )
    {
        uint8_t newId=0;
        newId=ProtocolItem.data[0];

       // electron.joint[0].id = 0;  //这里还是用广播
        electron.joint[0].id=ProtocolItem.jointID;
        electron.joint[0].angleMin = 0;
        electron.joint[0].angleMax = 180;
        electron.joint[0].angle = 0;
        electron.joint[0].modelAngelMin = -90;
        electron.joint[0].modelAngelMax = 90;
        electron.SetJointId(electron.joint[0], newId);

        HAL_Delay(1000);
        // 3.等待舵机参数保存。将主控 joint[0].id改为 2
        electron.joint[0].id = newId;  //这里改为新地址
        electron.joint[0].angleMin = 0;
        electron.joint[0].angleMax = 180;
        electron.joint[0].angle = 0;
        electron.joint[0].modelAngelMin = -90;
        electron.joint[0].modelAngelMax = 90;
        // 4.使用新设置的2地址通讯
        electron.SetJointEnable(electron.joint[0], true);
        HAL_Delay(5);
        electron.SetJointEnable(electron.joint[0], false);

        myPrintf("old joint id is %d,new joint id is%d\r\n",ProtocolItem.jointID,newId);
        ProtocolItem.dataLen=0;
        ProtocolItem.frame=ResponseFrame;
        ComposeProtocolFrame(txbuf.buf,&txbuf.dataLen,&ProtocolItem);
    }

    if(ProtocolItem.cmd == CMD_JointRotationTest )
    {
        RotationTest.JointId=ProtocolItem.jointID;
        RotationTest.en_flag=true;
        memcpy((uint8_t*) &RotationTest.Angle,ProtocolItem.data,sizeof (RotationTestAngle_t));
    }

    if(ProtocolItem.cmd == CMD_JointRotationTestClose )
    {
        if(RotationTest.JointId==ProtocolItem.jointID)
        {
            RotationTest.en_flag = false;
            myPrintf("joint weil stop,plase wait a moment\r\n ");
            HAL_Delay(20);
        }
    }

    if(ProtocolItem.cmd == CMD_ReadRobotSlavesConnectionStatus)
    {
        ProtocolItem.dataLen=sizeof (RobotSlavesConnectionStatus_t);
        ProtocolItem.data=(uint8_t*)&RobotSlavesConnectionStatus;
        ProtocolItem.frame=ResponseFrame;
        ComposeProtocolFrame(txbuf.buf,&txbuf.dataLen,&ProtocolItem);
        HAL_Delay(20);
       // xMutex_Uart1 =xSemaphoreCreateMutex( );
        //xSemaphoreTake(xMutex_Uart1, portMAX_DELAY);
        HAL_UART_Transmit(&huart1 , (uint8_t *)&txbuf.buf, txbuf.dataLen, 0xFFFF);
       // xSemaphoreGive(xMutex_Uart1);
    }

    if(ProtocolItem.cmd == CMD_ResetMaster)
    {
        HAL_Delay(100);
        myPrintf("system reset\r\n");
        HAL_Delay(100);
        HAL_NVIC_SystemReset();
    }

    if (ProtocolItem.cmd == CMD_IAP || ProtocolItem.cmd == (CMD_IAP |0x80))
    {
        HAL_Delay(100);
        myPrintf("system reset for IAP\r\n");
        HAL_Delay(100);
        IAP_FlashErase3();
        __disable_irq();
        HAL_NVIC_SystemReset();
    }
    BufClear((uint8_t* )&rxbuf,0,sizeof(rxbuf));
}
 uint32_t StatusReportingTimeCount=0;


void StatusReportingOnce()
{
    ProtocolItem.cmd = CMD_ReadRobotSlavesConnectionStatus;
    ProtocolItem.dataLen = sizeof(RobotSlavesConnectionStatus_t);
    ProtocolItem.data = (uint8_t *) &RobotSlavesConnectionStatus;
    ProtocolItem.frame = ResponseFrame;
    ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);
// xMutex_Uart1 =xSemaphoreCreateMutex( );
//xSemaphoreTake(xMutex_Uart1, portMAX_DELAY);
    HAL_UART_Transmit(&huart1, (uint8_t *) &txbuf.buf, txbuf.dataLen, 0xFFFF);
    HAL_Delay(10);
}
void StatusReporting()
{

    if(StatusReportingTimeCount>1000) {
        StatusReportingTimeCount=0;
        ProtocolItem.cmd = CMD_ReadRobotSlavesConnectionStatus;
        ProtocolItem.dataLen = sizeof(RobotSlavesConnectionStatus_t);
        ProtocolItem.data = (uint8_t *) &RobotSlavesConnectionStatus;
        ProtocolItem.frame = ResponseFrame;
        ComposeProtocolFrame(txbuf.buf, &txbuf.dataLen, &ProtocolItem);
// xMutex_Uart1 =xSemaphoreCreateMutex( );
//xSemaphoreTake(xMutex_Uart1, portMAX_DELAY);
        HAL_UART_Transmit(&huart1, (uint8_t *) &txbuf.buf, txbuf.dataLen, 0xFFFF);
        HAL_Delay(10);
    }
// xSemaphoreGive(xMutex_Uart1);

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
   // HAL_Delay(40);
    myPrintf("*******joint%dStatus******\r\n",id);
  //  HAL_Delay(40);
    myPrintf("angleMin=%f\r\n", Status->angleMin);
  //  HAL_Delay(40);
    myPrintf("angleMax=%f\r\n", Status->angleMax);
 //   HAL_Delay(40);
    myPrintf("modelAngelMin=%f\r\n", Status->modelAngelMin);
 //   HAL_Delay(40);
    myPrintf("modelAngelMax=%f\r\n", Status->modelAngelMax);
  //  HAL_Delay(40);
    myPrintf("torqueLimit=%f\r\n", Status->torqueLimit);
  //  HAL_Delay(40);
    myPrintf("kp=%f\r\n", Status->kp);
   // HAL_Delay(40);


    /*myPrintf("\r\n");


    myPrintf("initAngle=%f\r\n", Status->initAngle);

    myPrintf("angle=%f\r\n", Status->angle);

    myPrintf("ki=%f\r\n", Status->ki);

    myPrintf("kv=%f\r\n", Status->kv);

    myPrintf("kd=%f\r\n", Status->kd);

    myPrintf("enable=%s\r\n", Status->enable ? "true" : "false");

    myPrintf("inverted=%s\r\n", Status->inverted ? "true" : "false");*/

}

void JointStatusUpdata(void)
{
    ElectronBotJointStatus_t flashSaved;
    //uint8_t i;

    ReadJointStatusFromFlash((uint8_t *)&flashSave,sizeof(flashSave_t));

    //for(i=1;i<7;i++)
    //    for(i=1;i<=7;i++)
            //for(i=1;i<=7;i++)
            for (int i = 1; i <= 6; i++)
    {
        HAL_Delay(50);
        if(flashSave.saveFlag[i] !=FlashSaveFlag)
        {
            flashSave.ElectronBotjoint[i].angleMin = electron.joint[i].angleMin;
            flashSave.ElectronBotjoint[i].angleMax = electron.joint[i].angleMax;

            flashSave.ElectronBotjoint[i].modelAngelMax = electron.joint[i].modelAngelMax;
            flashSave.ElectronBotjoint[i].modelAngelMin = electron.joint[i].modelAngelMin;

            //flashSave.ElectronBotjoint[i].enable=electron.joint[i].enable;
            flashSave.ElectronBotjoint[i].angle = electron.joint[i].angle;
            flashSave.ElectronBotjoint[i].initAngle = 0;

            flashSave.ElectronBotjoint[i].inverted = electron.joint[i].inverted;

            flashSave.ElectronBotjoint[i].enable = false;
            flashSave.ElectronBotjoint[i].torqueLimit = 0.7;
            //flashSave.ElectronBotjoint[i].kp = 10;
            flashSave.ElectronBotjoint[i].kp = 25;
            flashSave.ElectronBotjoint[i].ki = 0;
            flashSave.ElectronBotjoint[i].kv = 0;
            flashSave.ElectronBotjoint[i].kd = 50;
            PrintfElectronBotJointStatus(&flashSave.ElectronBotjoint[i],i*2);

            electron.UpdateJointAngle(electron.joint[i], 0);
            HAL_Delay(50);
            electron.SetJointKp(electron.joint[i], flashSaved.kp);
            HAL_Delay(50);

            /*HAL_Delay(50);
            electron.SetJointInitAngle(electron.joint[i], flashSaved.initAngle);
            HAL_Delay(50);
            electron.SetJointTorqueLimit(electron.joint[i], flashSaved.torqueLimit);
            HAL_Delay(50);
            electron.SetJointKp(electron.joint[i], flashSaved.kp);
            HAL_Delay(50);
            electron.SetJointKi(electron.joint[i], flashSaved.ki);
            HAL_Delay(50);
            electron.SetJointKv(electron.joint[i], flashSaved.kv);
            HAL_Delay(50);
            electron.SetJointKd(electron.joint[i], flashSaved.kd);
            HAL_Delay(50);*/

           // electron.UpdateJointAngle(electron.joint[i], 0);
        }
        else
        {
            memcpy(&flashSaved, &flashSave.ElectronBotjoint[i], sizeof(ElectronBotJointStatus_t));

            electron.joint[i].angleMin = flashSaved.angleMin;
            electron.joint[i].angleMax = flashSaved.angleMax;

            electron.joint[i].modelAngelMax = flashSaved.modelAngelMax;
            electron.joint[i].modelAngelMin = flashSaved.modelAngelMin;

            electron.joint[i].inverted = flashSaved.inverted;

            PrintfElectronBotJointStatus(&flashSave.ElectronBotjoint[i], i * 2);
            electron.UpdateJointAngle(electron.joint[i], 0);
            HAL_Delay(50);

            if (flashSaved.torqueLimit != 0.5)
            {
                electron.SetJointTorqueLimit(electron.joint[i], flashSaved.torqueLimit);
                HAL_Delay(50);
            }

            if (flashSaved.kp != 10) {
                electron.SetJointKp(electron.joint[i], flashSaved.kp);
                HAL_Delay(50);
            }

           /*
            *
            *
            *
            * if (flashSaved.initAngle != 0)
            {
               // electron.SetJointInitAngle(electron.joint[i], flashSaved.initAngle);
                HAL_Delay(50);
            }
             if (flashSaved.ki != 0) {
                electron.SetJointKi(electron.joint[i], flashSaved.ki);
                HAL_Delay(50);
            }

            if (flashSaved.kv != 0) {
                electron.SetJointKv(electron.joint[i], flashSaved.kv);
                HAL_Delay(50);
            }

            if (flashSaved.kd!=50)
            {
                electron.SetJointKd(electron.joint[i], flashSaved.kd);
                HAL_Delay(50);
            }*/
            //electron.SetJointEnable(electron.joint[i], flashSaved.kd);
        }
    }
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