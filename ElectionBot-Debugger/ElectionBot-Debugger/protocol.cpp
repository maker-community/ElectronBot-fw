#include "protocol.h"
// #include <QDebug>
ElectronBotJointStatus_t ElectronBotjoint[6];
JointStatus_t joint[7];
ProtocolItem_t ProtocolItem;
txbuf_t txbuf;
rxbuf_t rxbuf;

/*
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
};*/


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
    .joint4=true,
    .joint6=true,
    .joint8=true,
    .joint10=true,
    .joint12=true,
    .mpu6050=true,
    .mpu6050Init=true,
    .paj7620=true,
    .paj7620Init=true,
};*/

void myprintf(const char* format, ...)
{
    printf(format);
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

//extern struct iap_t iap;
extern  iap_t iap;
extern  bool UpdateRobotSlavesConnectionStatus_flag;
//此函数可以自己重新写
//void ProtocolProcessing(uint8_t* buf, uint16_t len)
uint8_t ProtocolProcessing(uint8_t* buf, uint16_t len)
{
    uint8_t frameBuf[1024] = { 0 };
    uint8_t idbuf = 0;
    ElectronBotJointStatus_t* test;

    if (ProtocolLookUp(frameBuf, buf, len) == false)
    {
        myprintf("\r\n  No Protocol!!!!!!! \r\n");
        //qDebug("\r\n  No Protocol!!!!!!! \r\n");
        return 0;
    }
    myprintf("\r\n  Protocol is extied !!!!!!! \r\n");
  //  qDebug("\r\n  Protocol is extied !!!!!!! \r\n");

    ProtocolItem.ElectronBotID = frameBuf[1] + frameBuf[2] * 256 + frameBuf[3] * 256 * 256 + frameBuf[4] * 256 * 256 * 256;
    ProtocolItem.cmd = frameBuf[5];
    ProtocolItem.jointID = frameBuf[6];
    ProtocolItem.dataLen = frameBuf[7] + frameBuf[8] * 256;
    ProtocolItem.data = &frameBuf[9];

    myprintf("ElectronBotID=%d\r\n", ProtocolItem.ElectronBotID);
    myprintf("jointID=%d\r\n", ProtocolItem.jointID);
    myprintf("dataLen=%d\r\n", ProtocolItem.dataLen);



    idbuf = ProtocolItem.jointID / 2;
    if (ProtocolItem.cmd == CMD_WriteAllJointStatus || ProtocolItem.cmd == (CMD_WriteAllJointStatus |0x80))
    {
       //  test = (ElectronBotJointStatus_t*)ProtocolItem.data;
       // myprintf("cmd=CMD_WriteAllJointStatus\r\n");
       // myprintf("angleMax=%f\r\n", test->angleMax);
       // myprintf("angle=%f\r\n", test->angle);
       // myprintf("modelAngelMin=%f\r\n", test->modelAngelMin);
      //  myprintf("modelAngelMax=%f\r\n", test->modelAngelMax);
      //  myprintf("inverted=%s\r\n", test->inverted ? "true" : "false");
      //  myprintf("initAngle=%f\r\n", test->initAngle);
     //   myprintf("torqueLimit=%f\r\n", test->torqueLimit);
     //   myprintf("kp=%f\r\n", test->kp);
     //   myprintf("ki=%f\r\n", test->ki);
     //   myprintf("kv=%f\r\n", test->kv);
     //   myprintf("kd=%f\r\n", test->kd);
     //   myprintf("enable=%s\r\n", test->enable ? "true" : "false");
        return CMD_WriteAllJointStatus;
    }

    if (ProtocolItem.cmd == CMD_IAP || ProtocolItem.cmd == (CMD_IAP |0x80))
    {
          uint32_t *PI,currentPacketAddress=0,packetSum=0,currentPacket=0,currentPacketLen=0,crc32Value=0;
          PI= (uint32_t*)ProtocolItem.data;

          currentPacketAddress=PI[0];
          packetSum=PI[1];
          currentPacket=PI[2];
          currentPacketLen=PI[3];
          crc32Value=PI[4];

          if((currentPacketAddress==iap.currentPacketAddress)
             &&(packetSum== iap.packetSum)
             &&(currentPacket==iap.currentPacket)
             &&(currentPacketLen==iap.currentPacketLen)
             &&(crc32Value==iap.crc32)
             )
          {
              iap.currentPacket++;
              //return 1;
          }
        return CMD_IAP;
    }


    if (ProtocolItem.cmd == CMD_ReadRobotSlavesConnectionStatus || ProtocolItem.cmd == (CMD_ReadRobotSlavesConnectionStatus |0x80))
    {
        uint8_t* P;
        P=ProtocolItem.data;
        memcpy((uint8_t*)&RobotSlavesConnectionStatus,P,sizeof(RobotSlavesConnectionStatus_t));
        UpdateRobotSlavesConnectionStatus_flag=true;
        //return 1;
        return CMD_ReadRobotSlavesConnectionStatus;
    }

    return 0;
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
