#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <cmath>

#define FrameHead  0xea
#define FrameTail  0xea

//#define CMD_SetAllJointStatus  0x01
//#define CMD_GetAllJointStatus  0x02
//#define CMD_SetJointID         0x03
//#define CMD_SetElectronBotID   0x04
//#define CMD_GetElectronBotID   0x05


#define CMD_WriteAllJointStatus  0x01
#define CMD_ReadAllJointStatus   0x02
#define CMD_WriteJointID         0x03
#define CMD_WriteElectronBotID   0x04
#define CMD_ReadElectronBotID    0x05

#define CMD_ReadRobotSlavesConnectionStatus    0x0a
#define CMD_IAP                  0x0c
#define CMD_ResetMaster          0x0d
#define CMD_JointRotationTest    0x0e
#define CMD_JointRotationTestClose 0x0f

#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long


struct ElectronBotJointStatus_t
{
    // uint8_t id;
    float angleMin;
    float angleMax;
    float angle;
    float modelAngelMin;
    float modelAngelMax;
    bool  inverted = false;
    float initAngle;
    float torqueLimit;
    float kp;
    float ki;
    float kv;
    float kd;
    bool enable;  //舵机使能
};
//ElectronBotJointStatus_t ElectronBotjoint[6];


struct JointStatus_t
{
    uint8_t id;
    float angleMin;
    float angleMax;
    float angle;
    float modelAngelMin;
    float modelAngelMax;
    bool inverted = false;
};
//JointStatus_t joint[7];


/*
struct RobotSlavesConnectionStatus_t
{
    bool joint0;
    bool joint2;
    bool joint4;
    bool joint6;
    bool joint8;
    bool joint10;
    bool joint12;
    bool mpu6050;
    bool mpu6050Init;
    bool paj7620;
    bool paj7620Init;
};*/

struct RobotSlavesConnectionStatus_t
{
    //bool joint0;
    bool joint2;
    bool joint4;
    bool joint6;
    bool joint8;
    bool joint10;
    bool joint12;
    bool mpu6050;
    bool mpu6050Init;
    bool paj7620;
    bool paj7620Init;
};

enum CmdOrResFrame { CommandFrame, ResponseFrame };
struct ProtocolItem_t {
    uint32_t ElectronBotID;
    uint8_t  cmd;
    uint8_t  jointID;
    uint16_t dataLen;
    uint8_t* data;

    CmdOrResFrame frame; //设置帧类型  命令帧或者应答帧  若为应答帧 cmd的最高bit位被值1;
   // bool    SaveEn;
};
//ProtocolItem_t ProtocolItem;


struct txbuf_t {
    uint16_t dataLen;
    uint8_t buf[300];
};
//txbuf_t txbuf;

struct rxbuf_t {
    uint16_t dataLen;
    uint8_t* frameHead;
    uint8_t buf[300];
};
//rxbuf_t rxbuf;

//#define APP_ADDRESS 0x08000000
#define  CRC32_INIT_VALUE 0xFFFFFFFF
#define APP_ADDRESS 0x08010000
#define IAP_PACKET_MAX_DATA_LEN 460
struct iap_t
{
    uint64_t codeLen=0;
    uint32_t currentPacketAddress=APP_ADDRESS;
    uint32_t packetSum=0;
    uint32_t currentPacket=0;
    uint32_t currentPacketLen=0;

    uint32_t crc32;
    uint32_t crc32Packet;

    bool     waitPacketResponseFlag=false;
    uint32_t waitPacketResponseTimeCount=0;
    uint8_t  currentPacketData[IAP_PACKET_MAX_DATA_LEN]={0};
    uint8_t  data[1024*1024]={0};

};

//extern struct iap_t iap;
extern struct iap_t iap;


void testComposeProtocolFrame();
//void ProtocolProcessing(uint8_t* buf, uint16_t len);//处理数据
uint8_t ProtocolProcessing(uint8_t* buf, uint16_t len);
bool ProtocolLookUp(uint8_t* frameBuf, uint8_t* inBuf, uint16_t inLen);//查找inBuf里是否有协议帧,有则将协议帧拷贝到frameBuf
void ComposeProtocolFrame(uint8_t* frameBuf, uint16_t* frameLen, ProtocolItem_t* items);//组帧协议帧拷贝到frameBuf

#endif 
