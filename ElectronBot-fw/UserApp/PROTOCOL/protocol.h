//
// Created by Administrator on 2022/8/5.
//

//#ifndef ELECTRONBOT_FW_PROTOCOL_H
//#define ELECTRONBOT_FW_PROTOCOL_H

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_flash_ex.h"
#include "flash.h"
//#include <cmath>
#include "stm32f4xx_hal_uart.h"
#include "usart.h"
#include "myPrintf.h"
#include "usbd_cdc_if.h"
#include "usbd_def.h"
//#include <stdint.h>

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
#define CMD_ResetMaster          0x0d
#define CMD_JointRotationTest    0x0e
#define CMD_JointRotationTestClose 0x0f
#define CMD_IAP                  0x0c

#define uint8_t  unsigned char
#define uint16_t unsigned short
#define uint32_t unsigned int
#define uint64_t unsigned long


#define FlashSaveFlag 0x5555aaaa

#define IAP_FLASH_SAVE_ADDRESS ADDR_FLASH_SECTOR_10

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
    bool enable=false;  //舵机使能
   // uint32_t saveFlag;
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

struct RobotSlavesConnectionStatus_t
{
   // bool joint0;
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

//JointStatus_t joint[7];

enum CmdOrResFrame { CommandFrame, ResponseFrame };
struct ProtocolItem_t {
    uint32_t ElectronBotID;
    uint8_t  cmd;
    uint8_t  jointID;
    uint16_t dataLen;
    uint8_t* data;

    CmdOrResFrame frame; //设置帧类型  命令帧或者应答帧  若为应答帧 cmd的最高bit位被值1;
    bool    SaveEn;
};
//ProtocolItem_t ProtocolItem;


struct txbuf_t {
    uint16_t dataLen;
    uint8_t buf[600];
};
//txbuf_t txbuf;

struct rxbuf_t {
    uint16_t dataLen;
    uint8_t* frameHead;
    uint8_t buf[600];
};
//rxbuf_t rxbuf;

struct flashSave_t{
    uint16_t len;
    uint32_t saveFlag[7]={0};
    ElectronBotJointStatus_t ElectronBotjoint[7];
};

struct RotationTestAngle_t
{
    double  StartAngle;
    double  EndAngle;
}    ;

struct RotationTest_t
{
    uint8_t JointId;
    bool en_flag;
    RotationTestAngle_t Angle;
};

void testComposeProtocolFrame();
void ProtocolProcessing(uint8_t* buf, uint16_t len);//处理数据
bool ProtocolLookUp(uint8_t* frameBuf, uint8_t* inBuf, uint16_t inLen);//查找inBuf里是否有协议帧,有则将协议帧拷贝到frameBuf
void ComposeProtocolFrame(uint8_t* frameBuf, uint16_t* frameLen, ProtocolItem_t* items);//组帧协议帧拷贝到frameBuf
void testReceiveMasterUsbData(uint8_t *buf, uint16_t len);
void test_protocol();
void JointStatusUpdata(void);
//void myPrintf(const char* format, ...);
//void myDebug(const char* format, ...);

void ReadJointStatusFromFlash(uint8_t *buf ,uint16_t len);
void SaveJointStatusToFlash(uint8_t *buf ,uint16_t len);


void JointsConnectionStatusChange(uint8_t jointId,bool ConnectionStatus);
bool JointsConnectionStatusIs(uint8_t jointId);

void MPU6050_ConnectionStatusChange(bool ConnectionStatus);
bool MPU6050_ConnectionStatusIs();
void MPU6050_InitStatusChange(bool InitStatus);
bool MPU6050_InitStatusIs();

void PAJ7620_ConnectionStatusChange(bool ConnectionStatus);
bool PAJ7620_ConnectionStatusIs();
void PAJ7620_InitStatusChange(bool InitStatus);
bool PAJ7620_InitStatusIs();
void PrintfElectronBotJointStatus(ElectronBotJointStatus_t * Status,uint8_t id);

#endif //PROTOCOL_H
