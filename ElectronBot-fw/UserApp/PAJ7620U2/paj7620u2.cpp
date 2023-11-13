#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "paj7620u2.h"
#include "protocol.h"
#include "stm32f4xx_hal.h"
//PAJ2670 I2C初始化
void GS_i2c_init(void)
{
    MX_I2C1_Init();
}

u8 GS_Write_Byte(u8 REG_Address,u8 REG_data)
{
    HAL_StatusTypeDef state = HAL_ERROR;
    int i;

    //myDebug("**GS_Write_Byte***\r\n");

    i=IIC_MAX_RETRY;
    do
    {
        state = HAL_I2C_Mem_Write(&hi2c1,PAJ7620_ID,REG_Address,I2C_MEMADD_SIZE_8BIT,&REG_data,1,5);
        i--;
    } while(state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic mem write  fail\r\n");
    }

    return 0;
}

u8 GS_Read_Byte(u8 REG_Address)
{
    u8 REG_data;

    HAL_StatusTypeDef state = HAL_ERROR;
    int i;

   // myDebug("**GS_Read_Byte***\r\n");

   /* i=IIC_MAX_RETRY;
    do
    {
        state = HAL_I2C_Master_Transmit(&hi2c1, PAJ7620_ID, &REG_Address, 1, 5);
        i--;
    }while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic write REG_Address fail\r\n");
    }

    state = HAL_ERROR;
    i=IIC_MAX_RETRY;
    do
    {
        state = HAL_I2C_Master_Receive(&hi2c1, PAJ7620_ID, &REG_data, 1, 5);
        i--;
    } while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic read data fail\r\n");
    }*/

    i=IIC_MAX_RETRY;
    //HAL_Delay(2);
    do
    {
        //state = HAL_I2C_Mem_Write(&hi2c1,(addr<<1)|0,reg,I2C_MEMADD_SIZE_8BIT,0,1,5);
        //state = HAL_I2C_Mem_Write(&hi2c1,addr,reg,I2C_MEMADD_SIZE_8BIT,0,1,5);
        state = HAL_I2C_Mem_Read(&hi2c1, PAJ7620_ID, REG_Address,1, &REG_data,1, 5);
        i--;
    } while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic read data fail\r\n");
       // return 1;
    }

    return REG_data;
}

u8 GS_Read_nByte(u8 REG_Address,u16 len,u8 *buf)
{
    HAL_StatusTypeDef state = HAL_ERROR;
    int i;

  //  myDebug("**GS_Read_nByte***\r\n");

   /* i=IIC_MAX_RETRY;
    do
    {
        state = HAL_I2C_Mem_Write(&hi2c1,PAJ7620_ID,REG_Address,I2C_MEMADD_SIZE_8BIT,0,1,5);
        i--;
    } while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic write REG_Address fail\r\n");
    }

    state = HAL_ERROR;
    i=IIC_MAX_RETRY;
    do
    {
        state = HAL_I2C_Master_Receive(&hi2c1, PAJ7620_ID, buf, len, 5);
        i--;
    }while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic read n data fail\r\n");
    }*/

    i=IIC_MAX_RETRY;
    //HAL_Delay(2);
    do
    {
        //state = HAL_I2C_Mem_Write(&hi2c1,(addr<<1)|0,reg,I2C_MEMADD_SIZE_8BIT,0,1,5);
        //state = HAL_I2C_Mem_Write(&hi2c1,addr,reg,I2C_MEMADD_SIZE_8BIT,0,1,5);
        state = HAL_I2C_Mem_Read(&hi2c1, PAJ7620_ID, REG_Address,1, buf,len, 5);
        i--;
    } while (state != HAL_OK && i>0);
    if(state != HAL_OK)
    {
        myDebug("paj7620u2 iic read n data fail\r\n");
        // return 1;
    }

    return 0;
}

//PAJ7620唤醒
void GS_WakeUp(void)
{
    uint8_t data=0;
    HAL_I2C_Master_Transmit(&hi2c1, PAJ7620_ID, &data, 0, 5);
}

//选择PAJ7620U2 BANK区域
void paj7620u2_selectBank(bank_e bank)
{
    switch(bank)
    {
        case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0寄存器区域
        case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1寄存器区域
    }
}

//PAJ7620U2唤醒
u8 paj7620u2_wakeup(void)
{
    u8 data=0x0a;
    GS_WakeUp();//唤醒PAJ7620U2
    HAL_Delay(5);//唤醒时间>400us
    GS_WakeUp();//唤醒PAJ7620U2
    HAL_Delay(5);//唤醒时间>400us
    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    data = GS_Read_Byte(0x00);//读取状态
    if(data!=0x20) return 0; //唤醒失败

    return 1;
}

//PAJ7620U2初始化
//返回值：0:失败 1:成功
u8 paj7620u2_init(void)
{
    u8 i;
    u8 status;
    uint8_t readdata=0;

    GS_i2c_init();//IIC初始化
    status = paj7620u2_wakeup();//唤醒PAJ7620U2
    if(status)
    {
        PAJ7620_ConnectionStatusChange(true);
        myPrintf("PAJ7620 iic ok\r\n");\
        HAL_Delay(20);
    }
    else
    {
        PAJ7620_ConnectionStatusChange(false);
        myPrintf("PAJ7620 iic fail\r\n");
        HAL_Delay(20);
        return 0;
    }

    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域

    for(i=0;i<INIT_SIZE;i++)
    {
        GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//初始化PAJ7620U2
        myDebug("paj7620u2 write address %x  write data %x\r\n",init_Array[i][0],init_Array[i][1]);
        readdata=GS_Read_Byte(init_Array[i][0]);//初始化PAJ7620U2
        myDebug("paj7620u2 read address %x  read data %x\r\n",init_Array[i][0],readdata);
        //if(init_Array[i][1]!=readdata)
       // {
       //     PAJ7620_InitStatusChange(false);
       // }
        myDebug("********\r\n");
    }

    PAJ7620_InitStatusChange(true);
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
    if(PAJ7620_InitStatusIs())
    {
        myPrintf("paj7620u2 init ok!!!\r\n");
        HAL_Delay(10);
    }
    else
    {
        myPrintf("paj7620u2 init fail!!!\r\n");
        HAL_Delay(10);
    }

    return 1;
}

//手势识别测试
void Gesture_test(void)
{
    u8 i;
    u8 status;
    u8 data[2]={0x00};
    u16 gesture_data;

    paj7620u2_init();
    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    for(i=0;i<GESTURE_SIZE;i++)
    {
        GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
    }
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域

    while(1)
    {
        status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态
        if(gesture_data)
        {
            gesture_data =(u16)data[1]<<8 | data[0];
            switch(gesture_data)
            {
                case GES_UP:                myPrintf("Left\r\n");           break; //向上Left
                case GES_DOWM:              myPrintf("Right\r\n");          break; //向下Right
                case GES_LEFT:              myPrintf("Dowm\r\n");           break; //向左Dowm
                case GES_RIGHT:             myPrintf("Up\r\n");             break; //向右Up
                case GES_FORWARD:           myPrintf("Forward\r\n");        break; //向前
                case GES_BACKWARD:          myPrintf("Backward\r\n");       break; //向后
                case GES_CLOCKWISE:         myPrintf("Clockwise\r\n");      break; //顺时针
                case GES_COUNT_CLOCKWISE:   myPrintf("AntiClockwise\r\n");  break; //逆时针
                case GES_WAVE:              myPrintf("Wave\r\n");           break; //挥动
                default:                                                         ; break;
            }
        }

        HAL_Delay(50);
    }
}

void GestureInit(void)
{
    u8 i;
    paj7620u2_init();
    paj7620u2_selectBank(BANK0);//进入BANK0寄存器区域
    for(i=0;i<GESTURE_SIZE;i++)
    {
        GS_Write_Byte(gesture_arry[i][0],gesture_arry[i][1]);//手势识别模式初始化
    }
    paj7620u2_selectBank(BANK0);//切换回BANK0寄存器区域
}


extern bool iic_lock;
bool gesture_iic_lock = false;
extern SemaphoreHandle_t xMutex;
uint32_t  timeCount=0;
uint32_t  GestureTimeCount=0;
void Gesture(void)
{
    u8 status;
    u8 data[2]={0x00};
    u16 gesture_data;
    //HAL_Delay(1);
   // vTaskDelay(1);

   // while(iic_lock);
    //gesture_iic_lock= true;
    //timeCount++;
   // if(GestureTimeCount>=100) {
   //     GestureTimeCount=0;
        //timeCount=0;

        //xSemaphoreTake(xMutex, portMAX_DELAY);
         vPortEnterCritical();

        // HAL_Delay(3);
        HAL_Delay(1);
        //if (iic_lock== false)
        //vTaskDelay(1);
       // MX_I2C1_Init();
         status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);//读取手势状态
        //gesture_iic_lock = false;
        //vTaskDelay(1);
         HAL_Delay(1);
        //xSemaphoreGive(xMutex);
        vPortExitCritical();
   // }

    if(!status)
    {
        gesture_data =(u16)data[1]<<8 | data[0];
        if(gesture_data)
        {
            switch(gesture_data)
            {
                case GES_UP:                myPrintf("Left\r\n");           break; //向上Left
                case GES_DOWM:              myPrintf("Right\r\n");          break; //向下Right
                case GES_LEFT:              myPrintf("Dowm\r\n");           break; //向左Dowm
                case GES_RIGHT:             myPrintf("Up\r\n");             break; //向右Up
                case GES_FORWARD:           myPrintf("Forward\r\n");        break; //向前
                case GES_BACKWARD:          myPrintf("Backward\r\n");       break; //向后
                case GES_CLOCKWISE:         myPrintf("Clockwise\r\n");      break; //顺时针
                case GES_COUNT_CLOCKWISE:   myPrintf("AntiClockwise\r\n");  break; //逆时针
                case GES_WAVE:              myPrintf("Wave\r\n");           break; //挥动
                default:                                                         ; break;
            }
        }
    }
    //vTaskDelay(10);
    //HAL_Delay(10);
}