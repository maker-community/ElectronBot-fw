#include "robot.h"
#include "usbd_cdc_if.h"

bool iic_lock=false;
uint8_t* Robot::GetPingPongBufferPtr()
{
    return &(usbBuffer.rxData[usbBuffer.pingPongIndex][usbBuffer.rxDataOffset]);
}


void Robot::SendUsbPacket(uint8_t* _data, uint32_t _len)
{
    uint8_t ret;
    do
    {
        ret = CDC_Transmit_HS(_data, _len);
    } while (ret != USBD_OK);
}


void Robot::SwitchPingPongBuffer()
{
    usbBuffer.pingPongIndex = (usbBuffer.pingPongIndex == 0 ? 1 : 0);
    usbBuffer.rxDataOffset = 0;
}


uint8_t* Robot::GetLcdBufferPtr()
{
    return usbBuffer.rxData[usbBuffer.pingPongIndex == 0 ? 1 : 0];
}


//void Robot::ReceiveUsbPacketUntilSizeIs(uint32_t _count)
//{
//  while (usbBuffer.receivedPacketLen != _count);
//  usbBuffer.receivedPacketLen = 0;

//    if (usbBuffer.receivedPacketLen != _count);
//    usbBuffer.receivedPacketLen = 0;
//}

bool Robot::ReceiveUsbPacketUntilSizeIs(uint32_t _count)
{
    while (usbBuffer.receivedPacketLen != _count);
    usbBuffer.receivedPacketLen = 0;

   // if (usbBuffer.receivedPacketLen == _count)
   // {
   //     usbBuffer.receivedPacketLen = 0;
   //     return true;
   // }
    return false;
}


void Robot::UpdateServoAngle(Robot::JointStatus_t &_joint, float _angleSetPoint)
{
    if (_angleSetPoint >= _joint.angleMin && _angleSetPoint <= _joint.angleMax)
    {
        auto* b = (unsigned char*) (&_angleSetPoint);

        i2cTxData[0] = 0x01;
        for (int i = 0; i < 4; i++)
            i2cTxData[i + 1] = *(b + i);

        if(TransmitAndReceiveI2cPacket(_joint.id)==true)
        {
            _joint.angle = *(float *) (i2cRxData + 1);
        }
        else
        {
            _joint.angle =0;
            //_joint.angle = *(float *) (i2cRxData + 1);
        }
    }
}


void Robot::UpdateServoAngle(Robot::JointStatus_t &_joint)
{
    auto* b = (unsigned char*) &(_joint.angle);

    i2cTxData[0] = 0x11;

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);
}


void Robot::SetJointEnable(Robot::JointStatus_t &_joint, bool _enable)
{
    i2cTxData[0] = 0xff;
    i2cTxData[1] = _enable ? 1 : 0;

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);
}

extern bool gesture_iic_lock ;
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"
#include "protocol.h"

extern bool iic_lock;
//bool gesture_iic_lock = false;
extern SemaphoreHandle_t xMutex;
extern bool iicFailPrintfEn;
//void Robot::TransmitAndReceiveI2cPacket(uint8_t _id)
bool Robot::TransmitAndReceiveI2cPacket(uint8_t _id)
{
    HAL_StatusTypeDef state = HAL_ERROR;
    int iicreTry=2;
    //while(gesture_iic_lock == true);
    xSemaphoreTake(xMutex, portMAX_DELAY);
    iic_lock=true;
    //HAL_Delay(3);
   // HAL_Delay(3);
    do
    {
        //state = HAL_I2C_IsDeviceReady(motorI2c);
        state = HAL_I2C_Master_Transmit(motorI2c, _id, i2cTxData, 5, 5);
        iicreTry--;
    } while (state != HAL_OK && iicreTry>0);
    iicreTry=2;

    if(state==HAL_OK)
    {
        JointsConnectionStatusChange(_id, true);
    }else
    {
        JointsConnectionStatusChange(_id, false);
        if(iicFailPrintfEn==true)
        {
            myPrintf("iic write joint%d fail\r\n",_id);
            HAL_Delay(20);
        }
        xSemaphoreGive(xMutex);
        return false;
    }
    state = HAL_ERROR;
    do
    {
        state = HAL_I2C_Master_Receive(motorI2c, _id, i2cRxData, 5, 5);
        iicreTry--;
    } while (state != HAL_OK && iicreTry>0);
   // HAL_Delay(1);
   // iic_lock= false;

   if(state==HAL_OK)
   {
       JointsConnectionStatusChange(_id, true);
   }else
   {
       JointsConnectionStatusChange(_id, false);
       if(iicFailPrintfEn==true)
       {
           myPrintf("iic read joint%d fail\r\n",_id);
           HAL_Delay(20);
       }
       xSemaphoreGive(xMutex);
       return false;
   }

    xSemaphoreGive(xMutex);
    return true;
}


void Robot::SetJointTorqueLimit(Robot::JointStatus_t &_joint, float _percent)
{
    if (_percent >= 0 && _percent <= 1)
    {
        auto* b = (unsigned char*) (&_percent);

        i2cTxData[0] = 0x26;
        for (int i = 0; i < 4; i++)
            i2cTxData[i + 1] = *(b + i);

        TransmitAndReceiveI2cPacket(_joint.id);

        _joint.angle = *(float*) (i2cRxData + 1);

        HAL_Delay(500); // wait servo reset
    }
}


void Robot::SetJointId(Robot::JointStatus_t &_joint, uint8_t _id)
{
    i2cTxData[0] = 0x21;
    i2cTxData[1] = _id;

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);

    HAL_Delay(500); // wait servo reset
}


void Robot::SetJointInitAngle(Robot::JointStatus_t &_joint, float _angle)
{
    float sAngle = _joint.inverted ?
                   (_angle - _joint.modelAngelMin) /
                   (_joint.modelAngelMax - _joint.modelAngelMin) *
                   (_joint.angleMin - _joint.angleMax) + _joint.angleMax :
                   (_angle - _joint.modelAngelMin) /
                   (_joint.modelAngelMax - _joint.modelAngelMin) *
                   (_joint.angleMax - _joint.angleMin) + _joint.angleMin;


    if (sAngle >= _joint.angleMin && sAngle <= _joint.angleMax)
    {
        auto* b = (unsigned char*) (&_angle);

        i2cTxData[0] = 0x27;
        for (int i = 0; i < 4; i++)
            i2cTxData[i + 1] = *(b + i);

        TransmitAndReceiveI2cPacket(_joint.id);

        _joint.angle = *(float*) (i2cRxData + 1);

        HAL_Delay(500); // wait servo reset
    }
}


void Robot::SetJointKp(Robot::JointStatus_t &_joint, float _value)
{
    auto* b = (unsigned char*) (&_value);

    i2cTxData[0] = 0x22;
    for (int i = 0; i < 4; i++)
        i2cTxData[i + 1] = *(b + i);

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);

    HAL_Delay(500); // wait servo reset
}


void Robot::SetJointKi(Robot::JointStatus_t &_joint, float _value)
{
    auto* b = (unsigned char*) (&_value);

    i2cTxData[0] = 0x23;
    for (int i = 0; i < 4; i++)
        i2cTxData[i + 1] = *(b + i);

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);

    HAL_Delay(500); // wait servo reset
}


void Robot::SetJointKv(Robot::JointStatus_t &_joint, float _value)
{
    auto* b = (unsigned char*) (&_value);

    i2cTxData[0] = 0x24;
    for (int i = 0; i < 4; i++)
        i2cTxData[i + 1] = *(b + i);

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);

    HAL_Delay(500); // wait servo reset
}


void Robot::SetJointKd(Robot::JointStatus_t &_joint, float _value)
{
    auto* b = (unsigned char*) (&_value);

    i2cTxData[0] = 0x25;
    for (int i = 0; i < 4; i++)
        i2cTxData[i + 1] = *(b + i);

    TransmitAndReceiveI2cPacket(_joint.id);

    _joint.angle = *(float*) (i2cRxData + 1);

    HAL_Delay(500); // wait servo reset
}


uint8_t* Robot::GetExtraDataRxPtr()
{
    return GetLcdBufferPtr() + 60 * 240 * 3;
}


void Robot::UpdateJointAngle(Robot::JointStatus_t &_joint)
{
    UpdateServoAngle(_joint);

    float jAngle = _joint.inverted ?
                   (_joint.angleMax - _joint.angle) /
                   (_joint.angleMax - _joint.angleMin) *
                   (_joint.modelAngelMax - _joint.modelAngelMin) + _joint.modelAngelMin :
                   (_joint.angle - _joint.angleMin) /
                   (_joint.angleMax - _joint.angleMin) *
                   (_joint.modelAngelMax - _joint.modelAngelMin) + _joint.modelAngelMin;

    _joint.angle = jAngle;
}


void Robot::UpdateJointAngle(Robot::JointStatus_t &_joint, float _angleSetPoint)
{
    float sAngle = _joint.inverted ?
                   (_angleSetPoint - _joint.modelAngelMin) /
                   (_joint.modelAngelMax - _joint.modelAngelMin) *
                   (_joint.angleMin - _joint.angleMax) + _joint.angleMax :
                   (_angleSetPoint - _joint.modelAngelMin) /
                   (_joint.modelAngelMax - _joint.modelAngelMin) *
                   (_joint.angleMax - _joint.angleMin) + _joint.angleMin;

    UpdateServoAngle(_joint, sAngle);

    float jAngle = _joint.inverted ?
                   (_joint.angleMax - _joint.angle) /
                   (_joint.angleMax - _joint.angleMin) *
                   (_joint.modelAngelMax - _joint.modelAngelMin) + _joint.modelAngelMin :
                   (_joint.angle - _joint.angleMin) /
                   (_joint.angleMax - _joint.angleMin) *
                   (_joint.modelAngelMax - _joint.modelAngelMin) + _joint.modelAngelMin;

    _joint.angle = jAngle;
}
