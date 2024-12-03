 #include <cmath>
#include "common_inc.h"
#include "screen.h"
#include "robot.h"
#include "paj7620u2.h"
//#include "inv_mpu.h"
#include "mpu6050.h"
#include "invMpu.h"
#include "protocol.h"
//#include "crcLib.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "portmacro.h"
#include "semphr.h"


Robot electron(&hspi1, &hi2c1);
float jointSetPoints[6];
bool isEnabled = false;


#define MPU_6050_MAX_INIT_TIME  1000*10 //10秒
 uint32_t MPU_6050_init_time_count=0;
 void MPU_6050_init(void)
 {
     int reg_0x75=0;
     reg_0x75=MPU_Read_Byte(0x75);
     myPrintf("\r\nreg_0x75=%x\r\n",reg_0x75);
     printf("\r\n////////////////////////////////\r\n");
     HAL_Delay(20);
     if(reg_0x75==0x68)
     {
        myPrintf("MPU6050 iic ok!!\r\n");
        MPU6050_ConnectionStatusChange (true);
        HAL_Delay(20);
     }
     else
     {
        MPU6050_ConnectionStatusChange (false);
        myPrintf("MPU6050 iic fail!!\r\n");
        HAL_Delay(20);
        return ;
     }

     uint8_t mpu_dmp_init_return = 1;
    if(MPU6050_ConnectionStatusIs())
    {
        MPU_6050_init_time_count=0;
        while (mpu_dmp_init_return && (MPU_6050_init_time_count<MPU_6050_MAX_INIT_TIME))//MPU DMP初始化
       // while (mpu_dmp_init_return)//MPU DMP初始化
        {
            mpu_dmp_init_return = mpu_dmp_init();
            HAL_Delay(10);
            myDebug("mpu_dmp_init_return=%d\r\n", mpu_dmp_init_return);
            if(mpu_dmp_init_return!=0)printf("Some errors occurred during MPU6050 initialization\r\n");
            HAL_Delay(500);
        }

        if(mpu_dmp_init_return==0)
        {
            MPU6050_InitStatusChange(true);
            printf("MPU6050 init ok\r\n");
        }else
        {
            MPU6050_InitStatusChange(false);
            printf("MPU6050 init fail\r\n");
        }
        HAL_Delay(30);
    }
 }

 uint32_t MPU_6050_read_time_count=0;
void MPU_6050_read(void)
{
    float pitch,roll,yaw; 		//欧拉角
    short aacx,aacy,aacz;		//加速度传感器原始数据
    short gyrox,gyroy,gyroz;	//陀螺仪原始数据
    short temp;					//温度

    if(MPU_6050_read_time_count>=300) {
        MPU_6050_read_time_count=0;
        //printf("100ms");
        HAL_Delay(10);
        if (mpu_dmp_get_data(&pitch, &roll, &yaw) == 0) {
            temp = MPU_Get_Temperature();    //得到温度值
            MPU_Get_Accelerometer(&aacx, &aacy, &aacz);    //得到加速度传感器数据
            MPU_Get_Gyroscope(&gyrox, &gyroy, &gyroz);    //得到陀螺仪数据
            if (1)//GetData=0时 用于USMART调试MPU6050寄存器
            {


                //temp值
                if (temp < 0) {
                    temp = -temp;        //转为正数
                    printf(" Temp:  -%d.%dC\r\n", temp / 100, temp % 10);
                } else
                    printf(" Temp:  %d.%dC\r\n", temp / 100, temp % 10);
                HAL_Delay(10);
                //pitch值
                temp = pitch * 10;
                if (temp < 0) {
                    temp = -temp;        //转为正数
                    printf(" Pitch: -%d.%dC\r\n", temp / 10, temp % 10);
                } else
                    printf(" Pitch:  %d.%dC\r\n", temp / 10, temp % 10);
                HAL_Delay(10);
                //roll值
                temp = roll * 10;
                if (temp < 0) {
                    temp = -temp;        //转为正数
                    printf(" Roll:  -%d.%dC\r\n", temp / 10, temp % 10);
                } else
                    printf(" Roll:  %d.%dC\r\n", temp / 10, temp % 10);
                HAL_Delay(10);
                //yaw值
                temp = yaw * 10;
                if (temp < 0) {
                    temp = -temp;        //转为正数
                    printf(" Yaw:  -%d.%dC\r\n", temp / 10, temp % 10);
                } else
                    printf(" Yaw:   %d.%dC\r\n", temp / 10, temp % 10);
                HAL_Delay(10);
                printf("\r\n");
                HAL_Delay(10);
            }


        }
    }
}






extern bool iic_lock;

#define BUFFERSIZE 1200           //可以接收的最大字符个数
 uint8_t ReceiveBuff[BUFFERSIZE]; //接收缓冲区
 uint32_t recv_end_flag = 0,Rx_len;//接收完成中断标志，接收到字符长度


 void uart1_data(void)
 {
     if(recv_end_flag ==1)//接收完成标志
     {
       //  printf("\r\n");
       //  HAL_Delay(5);
         //printf("数据长度=%d\r\n",Rx_len);//打印接收到的数据长度
         //printf("数据内容:");

     //      printf("dataLen=%d\r\n",Rx_len);//打印接收到的数据长度
     //       HAL_Delay(20);
     //      printf("data:");
     //       HAL_Delay(20);
     //       for(int i=0;i<Rx_len;i++)
     //      {
     //          printf("%x ",ReceiveBuff[i]);//向串口打印接收到的数据
     //          HAL_Delay(5);
     //      }

           //printf("data:");
           // HAL_Delay(20);
           //for(int i=0;i<Rx_len;i++)
          // {
          //     printf("%c",ReceiveBuff[i]);//向串口打印接收到的数据
          //     HAL_Delay(5);
          // }

      //   HAL_Delay(20);
         //WriteFlashu8tou32(APPLICATION_ADDRESS,ReceiveBuff,Rx_len);
         ProtocolProcessing(ReceiveBuff,Rx_len);
       //  printf("\r\n");
       //  HAL_Delay(5);
         for(int i = 0; i < Rx_len ; i++) //清空接收缓存区
             ReceiveBuff[i]=0;//置0
         Rx_len=0;//接收数据长度清零
         recv_end_flag=0;//接收标志位清零0
     }
     //开启下一次接收
     HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
 }
/* USER CODE END 4 */

#define  CHECK_JOINTS_CONNECTION_STATUS_TIME 1000
uint32_t CheckJointsConnectionStatus_time_count=0;
void CheckJointsConnectionStatus()
{
    if(CheckJointsConnectionStatus_time_count>=CHECK_JOINTS_CONNECTION_STATUS_TIME)
    {
        CheckJointsConnectionStatus_time_count=0;
        electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
        electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
        electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
        electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
        electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
        electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);
    }
 }

 extern struct RotationTest_t RotationTest;
 bool iicFailPrintfEn= false;
void rotationTest(void)
{
    char VERSION[]={"1.0.1.0"};//{"1.0.0.0"}
    myPrintf("------------------------------------ \r\n");

    myPrintf("ElectronBot-fw (rotation test mode)\r\n");
    //myPrintf("ElectronBot-fw (rotation test mode)\r\n");
    myPrintf("version is %s\r\n",VERSION);
    myPrintf("------------------------------------ \r\n");
    HAL_Delay(200);
     iicFailPrintfEn=true;
    while(1) {
        if (RotationTest.en_flag) {
            electron.joint[0].id = RotationTest.JointId;
            electron.joint[0].angleMin = 0;
            electron.joint[0].angleMax = 180;
            electron.joint[0].angle = 0;
            electron.joint[0].modelAngelMin = -90;
            electron.joint[0].modelAngelMax = 90;

            // 2.使用广播地址是能
            electron.SetJointEnable(electron.joint[0], true);

            // 3.这时候就能看到舵机做往复运动了。
            while (RotationTest.en_flag)
                //   while (1)
            {
                for (int i = RotationTest.Angle.StartAngle;
                     i < RotationTest.Angle.EndAngle && RotationTest.en_flag; i += 1) {
                    float angle = i;
                    electron.UpdateJointAngle(electron.joint[0], angle);
                    myPrintf("joint %d angle:%f\r\n", electron.joint[0].id, angle);
                    StatusReportingOnce();
                    HAL_Delay(20);
                }
                for (int i = RotationTest.Angle.EndAngle;
                     i > RotationTest.Angle.StartAngle && RotationTest.en_flag; i -= 1) {
                    float angle = i;
                    electron.UpdateJointAngle(electron.joint[0], angle);
                    myPrintf("joint %d angle:%f\r\n", electron.joint[0].id, angle);
                    StatusReportingOnce();
                    HAL_Delay(20);
                }
            }
            electron.SetJointEnable(electron.joint[0], false);
        }
    }
}
 void paj7620Test()
 {
     char VERSION[]={"1.0.1.0"};//{"1.0.0.0"}
     myPrintf("------------------------------------ \r\n");

     myPrintf("ElectronBot-fw (paj7620 test mode)\r\n");
     //myPrintf("ElectronBot-fw (rotation test mode)\r\n");
     myPrintf("version is %s\r\n",VERSION);
     myPrintf("------------------------------------ \r\n");
     HAL_Delay(200);

     GestureInit();
     StatusReportingOnce();
     while(1) Gesture();
 }

 void mpu6050Test()
 {
     char VERSION[]={"1.0.1.0"};//{"1.0.0.0"}
     myPrintf("------------------------------------ \r\n");

     myPrintf("ElectronBot-fw (mpu6050 test mode)\r\n");
     //myPrintf("ElectronBot-fw (rotation test mode)\r\n");
     myPrintf("version is %s\r\n",VERSION);
     myPrintf("------------------------------------ \r\n");
     HAL_Delay(200);
     MPU_6050_init();
     StatusReportingOnce();
     while(1)MPU_6050_read();
 }
uint8_t GestureMainResumeEn=0;
 extern  osThreadId_t gestureTaskHandle;
 uint8_t JointChangeEn=0;
 extern SemaphoreHandle_t xMutex;

 uint32_t g_msCounter=0;
 uint32_t g_InitOK=0;
 void GestureMain(void)
 {
     //while(1);
    // HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
    // __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

     //MX_DMA_Init();
    // MX_USART1_UART_Init();
  //   HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
 //    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
 //    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);normalModeTestnormalModeTest

    // while(1);
     //while(1)uart1_data();
    // GestureInit();
   //  MPU_6050_init();
     while(1){
        // CheckJointsConnectionStatus();
       //  Gesture();
      //   MPU_6050_read();
        // StatusReporting();
         uart1_data();
         //Gesture();

         /*if(JointChangeEn) {
             xSemaphoreTake(xMutex, portMAX_DELAY);
             JointChangeEn = 0;
             xSemaphoreGive(xMutex);
             uint8_t *ptr = electron.GetExtraDataRxPtr();
             //vTaskDelay(1);
             HAL_Delay(1);
             if (isEnabled != (bool) ptr[0]) {
                 isEnabled = ptr[0];
                 electron.SetJointEnable(electron.joint[1], isEnabled);
                 electron.SetJointEnable(electron.joint[2], isEnabled);
                 electron.SetJointEnable(electron.joint[3], isEnabled);
                 electron.SetJointEnable(electron.joint[4], isEnabled);
                 electron.SetJointEnable(electron.joint[5], isEnabled);
                 electron.SetJointEnable(electron.joint[6], isEnabled);
             }
             for (int j = 0; j < 6; j++) {
                 jointSetPoints[j] = *((float *) (ptr + 4 * j + 1));
             }
             //vTaskDelay(1);
             HAL_Delay(1);
             electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
             electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
             electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
             electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
             electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
             electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);
            // vTaskDelay(1);
             HAL_Delay(1);
         }
        // Gesture();

         GestureMainResumeEn= 1;*/
        // vTaskSuspend(NULL);
       //  vTaskResume((TaskHandle_t)defaultTaskHandle);
       //  vTaskSuspend ((TaskHandle_t)defaultTaskHandle);


     //    HAL_Delay(1);
         if(g_msCounter>=1000 && g_InitOK>=1)
         {
             g_msCounter=0;
             electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);
             myPrintf("electron.joint[6].angle=%d",(int)electron.joint[6].angle);
         }
        // vTaskSuspend ((TaskHandle_t)gestureTaskHandle);
     }
     MPU_6050_init();
     while(1)MPU_6050_read();

     while(1)HAL_Delay(50);
 }

 //char VERSION[]={"1.0.0.0"};

void normalMode(void )
{
    char VERSION[]={"1.0.1.1"};//{"1.0.0.0"}
    myPrintf("------------------------------------ \r\n");

    myPrintf("ElectronBot-fw (normal mode)\r\n");
    //myPrintf("ElectronBot-fw (rotation test mode)\r\n");
    myPrintf("version is %s\r\n",VERSION);
    myPrintf("------------------------------------ \r\n");
    HAL_Delay(200);

    JointStatusUpdata();
     StatusReportingOnce();
    float t = 0;

    // electron.SetJointKp(electron.joint[2], 40);
    // electron.SetJointTorqueLimit(electron.joint[2], 1.0);
    // while(1);
    g_InitOK=1;
    while (true)
    {
#if 1
        for (int p = 0; p < 4; p++)
        {
            // send joint angles
            for (int j = 0; j < 6; j++)
                for (int i = 0; i < 4; i++)
                {
                    auto* b = (unsigned char*) &(electron.joint[j + 1].angle);
                    electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                }
            electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);

            electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

            // get joint angles
            uint8_t* ptr = electron.GetExtraDataRxPtr();
            if (isEnabled != (bool) ptr[0])
            {
                isEnabled = ptr[0];
                electron.SetJointEnable(electron.joint[1], isEnabled);
                electron.SetJointEnable(electron.joint[2], isEnabled);
                electron.SetJointEnable(electron.joint[3], isEnabled);
                electron.SetJointEnable(electron.joint[4], isEnabled);
                electron.SetJointEnable(electron.joint[5], isEnabled);
                electron.SetJointEnable(electron.joint[6], isEnabled);
            }
            for (int j = 0; j < 6; j++)
            {
                jointSetPoints[j] = *((float*) (ptr + 4 * j + 1));
            }

            while (electron.lcd->isBusy);
            if (p == 0)
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3);
            else
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3, true);
        }
        HAL_Delay(1);
#endif
        /*xSemaphoreTake(xMutex, portMAX_DELAY);
        JointChangeEn = 1;
        xSemaphoreGive(xMutex);
        vTaskResume((TaskHandle_t)gestureTaskHandle);
        //vTaskSuspend ((TaskHandle_t)defaultTaskHandle);

        vTaskDelay(25);
        */
        t += 0.01;

        electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
        electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
        electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
        electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
        electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
        electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

        HAL_Delay(1);
        if(g_msCounter>=1000)
        {
            myPrintf("electron.joint[6].angle=%d",electron.joint[6].angle);
        }
        //Gesture();
//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

        // printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
        //        jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
        //        jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
    }
}

 void normalModeTest(void )
 {
     char VERSION[]={"1.0.1.1"};//{"1.0.0.0"}
     myPrintf("------------------------------------ \r\n");

     myPrintf("ElectronBot-fw (normal mode)\r\n");
     //myPrintf("ElectronBot-fw (rotation test mode)\r\n");
     myPrintf("version is %s\r\n",VERSION);
     myPrintf("------------------------------------ \r\n");
     HAL_Delay(200);

     JointStatusUpdata();
     StatusReportingOnce();
     float t = 0;
     iicFailPrintfEn= true;
     // electron.SetJointKp(electron.joint[2], 40);
     // electron.SetJointTorqueLimit(electron.joint[2], 1.0);
     // while(1);


     electron.SetJointEnable(electron.joint[1],true);
     electron.SetJointEnable(electron.joint[2], true);
     electron.SetJointEnable(electron.joint[3], true);
     electron.SetJointEnable(electron.joint[4], true);
     electron.SetJointEnable(electron.joint[5], true);
     electron.SetJointEnable(electron.joint[6], true);

     while(1)
     {
         for (int i = -15;
              i < 15; i += 1) {
             float angle = i;

             electron.UpdateJointAngle(electron.joint[1], angle);
             electron.UpdateJointAngle(electron.joint[2], angle);
             electron.UpdateJointAngle(electron.joint[3], angle);
             electron.UpdateJointAngle(electron.joint[4], angle);
             electron.UpdateJointAngle(electron.joint[5], angle);
             electron.UpdateJointAngle(electron.joint[6], angle);
            // Gesture();
             HAL_Delay(20);
         }
         for (int i = 15;
              i > -15; i -= 1) {
             float angle = i;
             electron.UpdateJointAngle(electron.joint[1], angle);
             electron.UpdateJointAngle(electron.joint[2], angle);
             electron.UpdateJointAngle(electron.joint[3], angle);
             electron.UpdateJointAngle(electron.joint[4], angle);
             electron.UpdateJointAngle(electron.joint[5], angle);
             electron.UpdateJointAngle(electron.joint[6], angle);
             //Gesture();
             HAL_Delay(20);
         }
     }
     electron.SetJointEnable(electron.joint[0], false);


     while (true)
     {
#if 1
         for (int p = 0; p < 4; p++)
         {
             // send joint angles
             for (int j = 0; j < 6; j++)
                 for (int i = 0; i < 4; i++)
                 {
                     auto* b = (unsigned char*) &(electron.joint[j + 1].angle);
                     electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                 }
             electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);

             electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

             // get joint angles
             /*uint8_t* ptr = electron.GetExtraDataRxPtr();
             if (isEnabled != (bool) ptr[0])
             {
                 isEnabled = ptr[0];
                 electron.SetJointEnable(electron.joint[1], isEnabled);
                 electron.SetJointEnable(electron.joint[2], isEnabled);
                 electron.SetJointEnable(electron.joint[3], isEnabled);
                 electron.SetJointEnable(electron.joint[4], isEnabled);
                 electron.SetJointEnable(electron.joint[5], isEnabled);
                 electron.SetJointEnable(electron.joint[6], isEnabled);
             }
             for (int j = 0; j < 6; j++)
             {
                 jointSetPoints[j] = *((float*) (ptr + 4 * j + 1));
             }*/

             while (electron.lcd->isBusy);
             if (p == 0)
                 electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                60 * 240 * 3);
             else
                 electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                60 * 240 * 3, true);
         }
         HAL_Delay(1);
#endif
         xSemaphoreTake(xMutex, portMAX_DELAY);
         JointChangeEn = 1;
         xSemaphoreGive(xMutex);
         vTaskResume((TaskHandle_t)gestureTaskHandle);
         //vTaskSuspend ((TaskHandle_t)defaultTaskHandle);

         vTaskDelay(25);

         t += 0.01;

         /* electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
          electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
          electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
          electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
          electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
          electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

          HAL_Delay(1);
          */
//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

         // printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
         //        jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
         //        jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
     }
 }
 void Main(void)
 {
     HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
     __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

     //while(1)uart1_data();
     //while(1);


   // while(1)
   // {
  //      myPrintf("OTA bin test 0.7!!!!\r\n");
  //      HAL_Delay(500);
  //  }

     HAL_Delay(500);
     electron.lcd->Init(Screen::DEGREE_0);
     electron.lcd->SetWindow(0, 239, 0, 239);

   //  while(1)normalModeTest();
     while(1)normalMode();
    // while(1)rotationTest();
    // while(1)paj7620Test();
    // while(1)mpu6050Test();


   /*  electron.UpdateJointAngle(electron.joint[1], 0);
     electron.UpdateJointAngle(electron.joint[2], 0);
     electron.UpdateJointAngle(electron.joint[3], 0);
     electron.UpdateJointAngle(electron.joint[4], 0);
     electron.UpdateJointAngle(electron.joint[5], 0);
     electron.UpdateJointAngle(electron.joint[6], 0);

     electron.SetJointKp(electron.joint[2],40);
     HAL_Delay(10);
     electron.SetJointKp(electron.joint[4],40);
     HAL_Delay(10);
     electron.SetJointKp(electron.joint[5],40);
     HAL_Delay(10);
     electron.SetJointTorqueLimit(electron.joint[6],0.7);
     HAL_Delay(10);
     electron.SetJointKp(electron.joint[6],50);
     HAL_Delay(10);
*/
      //  while(1);
     JointStatusUpdata();


     float t = 0;

     // electron.SetJointKp(electron.joint[2], 40);
     // electron.SetJointTorqueLimit(electron.joint[2], 1.0);
       // while(1);
     while (true)
     {
#if 1
         for (int p = 0; p < 4; p++)
         {
             // send joint angles
             for (int j = 0; j < 6; j++)
                 for (int i = 0; i < 4; i++)
                 {
                     auto* b = (unsigned char*) &(electron.joint[j + 1].angle);
                     electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                 }
             electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);

             electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

             // get joint angles
             uint8_t* ptr = electron.GetExtraDataRxPtr();
             if (isEnabled != (bool) ptr[0])
             {
                 isEnabled = ptr[0];
                 electron.SetJointEnable(electron.joint[1], isEnabled);
                 electron.SetJointEnable(electron.joint[2], isEnabled);
                 electron.SetJointEnable(electron.joint[3], isEnabled);
                 electron.SetJointEnable(electron.joint[4], isEnabled);
                 electron.SetJointEnable(electron.joint[5], isEnabled);
                 electron.SetJointEnable(electron.joint[6], isEnabled);
             }
             for (int j = 0; j < 6; j++)
             {
                 jointSetPoints[j] = *((float*) (ptr + 4 * j + 1));
             }

             while (electron.lcd->isBusy);
             if (p == 0)
                 electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                60 * 240 * 3);
             else
                 electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                60 * 240 * 3, true);
         }
         HAL_Delay(1);
#endif


         t += 0.01;

         electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
         electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
         electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
         electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
         electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
         electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

         HAL_Delay(1);

//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

        // printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
        //        jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
        //        jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
     }
 }//


 extern "C"
 void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
 {
     electron.lcd->isBusy = false;
 }


/*
enum USBD_Status_e
        {
            SendUsbPacket,
            ReceiveUsbPacket,
            lcdP1,
            lcdP3,
        };
 enum USBD_Status_e USBD_Status;
bool lcdWriteEn = false;
int p;
void Main(void)
{
    u8 time=0;
    int reg_0x25=0,reg_107=0;
    float pitch,roll,yaw; 		//欧拉角
    short aacx,aacy,aacz;		//加速度传感器原始数据
    short gyrox,gyroy,gyroz;	//陀螺仪原始数据
    short temp;					//温度

   // MPU_Read_Byte(0);
    HAL_Delay(4000);
    electron.lcd->Init(Screen::DEGREE_0);
    electron.lcd->SetWindow(0, 239, 0, 239);

    // GestureInit();
    // while(1)Gesture();
    //while(1);
    //test_mpu();
    //mpu_dmp_init();


   // HAL_NVIC_DisableIRQ(USART1_IRQn);
   // __disable_irq();
    reg_0x25=MPU_Read_Byte(0x75);
    myPrintf("\r\nreg_0x25=%x\r\n",reg_0x25);
    if(reg_0x25==0x68)myPrintf("MPU6050 iic ok!!");else{myPrintf("MPU6050 iic fail!!");}
    HAL_Delay(20);
    //reg_107=MPU_Read_Byte(107);
    //myPrintf("\r\nreg_107=%x\r\n",reg_107);
    //HAL_Delay(20);
    //MPU_Write_Byte(107,128);
    //MPU_Write_Byte(107,1);
    //reg_107=0;
    //reg_107=MPU_Read_Byte(107);
    //myPrintf("\r\nreg_107=%x\r\n",reg_107);
    //HAL_Delay(20);

    //while(1);
    uint8_t mpu_dmp_init_return=1;
    while(mpu_dmp_init_return)//MPU DMP初始化
    {
        mpu_dmp_init_return=mpu_dmp_init();
        HAL_Delay(20);
        printf("mpu_dmp_init_return=%d\r\n",mpu_dmp_init_return);
        HAL_Delay(20);
        printf("MPU6050 Error!!!\r\n");
        HAL_Delay(500);
    }
    printf("MPU6050 OK\r\n");
    HAL_Delay(20);

    while(1)
    {

        if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)
        {
            temp=MPU_Get_Temperature();	//得到温度值
            MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//得到加速度传感器数据
            MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//得到陀螺仪数据
            if(1)//GetData=0时 用于USMART调试MPU6050寄存器
            {

                if((time%10)==0)
                {
                    //temp值
                    if(temp<0)
                    {
                        temp=-temp;		//转为正数
                        printf(" Temp:  -%d.%dC\r\n",temp/100,temp%10);
                    }else
                        printf(" Temp:  %d.%dC\r\n",temp/100,temp%10);
                    HAL_Delay(10);
                    //pitch值
                    temp=pitch*10;
                    if(temp<0)
                    {
                        temp=-temp;		//转为正数
                        printf(" Pitch: -%d.%dC\r\n",temp/10,temp%10);
                    }else
                        printf(" Pitch:  %d.%dC\r\n",temp/10,temp%10);
                    HAL_Delay(10);
                    //roll值
                    temp=roll*10;
                    if(temp<0)
                    {
                        temp=-temp;		//转为正数
                        printf(" Roll:  -%d.%dC\r\n",temp/10,temp%10);
                    }else
                        printf(" Roll:  %d.%dC\r\n",temp/10,temp%10);
                    HAL_Delay(10);
                    //yaw值
                    temp=yaw*10;
                    if(temp<0)
                    {
                        temp=-temp;		//转为正数
                        printf(" Yaw:  -%d.%dC\r\n",temp/10,temp%10);
                    }else
                        printf(" Yaw:   %d.%dC\r\n",temp/10,temp%10);
                    time=0;
                    HAL_Delay(10);
                    printf("\r\n");
                    HAL_Delay(10);
                }

            }

        }
        time++;
    }



    electron.UpdateJointAngle(electron.joint[1], 0);
    electron.UpdateJointAngle(electron.joint[2], 0);
    electron.UpdateJointAngle(electron.joint[3], 0);
    electron.UpdateJointAngle(electron.joint[4], 0);
    electron.UpdateJointAngle(electron.joint[5], 0);
    electron.UpdateJointAngle(electron.joint[6], 0);
    electron.SetJointKp(electron.joint[2],40);
    HAL_Delay(10);
    electron.SetJointKp(electron.joint[4],40);
    HAL_Delay(10);
    electron.SetJointKp(electron.joint[5],40);
    HAL_Delay(10);
    electron.SetJointTorqueLimit(electron.joint[6],0.7);
    HAL_Delay(10);
    electron.SetJointKp(electron.joint[6],50);
    HAL_Delay(10);

    USBD_Status=SendUsbPacket;

    float t = 0;

    p=0;
    while (true)
    {
        //Gesture();
#if 1
       //for (int p = 0; p < 4; p++)
       // {
           // Gesture();
            // send joint angles
            if(USBD_Status==SendUsbPacket) {
                for (int j = 0; j < 6; j++)
                    for (int i = 0; i < 4; i++) {
                        auto *b = (unsigned char *) &(electron.joint[j + 1].angle);
                        electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                    }
                electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);
                USBD_Status=ReceiveUsbPacket;
            }
           // electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

           if(p==0 && USBD_Status==ReceiveUsbPacket && electron.ReceiveUsbPacketUntilSizeIs(224)== true)
           {
               USBD_Status=lcdP1;

               uint8_t *ptr = electron.GetExtraDataRxPtr();
               if (isEnabled != (bool) ptr[0]) {
                   isEnabled = ptr[0];
                   electron.SetJointEnable(electron.joint[1], isEnabled);
                   electron.SetJointEnable(electron.joint[2], isEnabled);
                   electron.SetJointEnable(electron.joint[3], isEnabled);
                   electron.SetJointEnable(electron.joint[4], isEnabled);
                   electron.SetJointEnable(electron.joint[5], isEnabled);
                   electron.SetJointEnable(electron.joint[6], isEnabled);
               }
               for (int j = 0; j < 6; j++) {
                   jointSetPoints[j] = *((float *) (ptr + 4 * j + 1));
               }

           }

           else if((USBD_Status==ReceiveUsbPacket||USBD_Status==lcdP1) && electron.ReceiveUsbPacketUntilSizeIs(224)== true)
           {
                USBD_Status=lcdP3;

                uint8_t *ptr = electron.GetExtraDataRxPtr();
                if (isEnabled != (bool) ptr[0]) {
                    isEnabled = ptr[0];
                    electron.SetJointEnable(electron.joint[1], isEnabled);
                    electron.SetJointEnable(electron.joint[2], isEnabled);
                    electron.SetJointEnable(electron.joint[3], isEnabled);
                    electron.SetJointEnable(electron.joint[4], isEnabled);
                    electron.SetJointEnable(electron.joint[5], isEnabled);
                    electron.SetJointEnable(electron.joint[6], isEnabled);
                }
                for (int j = 0; j < 6; j++) {
                    jointSetPoints[j] = *((float *) (ptr + 4 * j + 1));
                }

            }


          // while(electron.ReceiveUsbPacketUntilSizeIs(224)!= true)
          // {
               // Gesture();
          // }

           // if(electron.ReceiveUsbPacketUntilSizeIs(224)== true) {
                // get joint angles


               //  while (electron.lcd->isBusy);
                     //Gesture();
                //if (electron.lcd->isBusy) {}
                //else {

                    //iic_lock = true;
                    if (p == 0 && electron.lcd->isBusy == false && USBD_Status==lcdP1)
                        electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                       60 * 240 * 3);
                    else if(electron.lcd->isBusy == false && USBD_Status==lcdP3)
                        electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                                       60 * 240 * 3, true);
                    //iic_lock = false;
               // }
            //}
        //}
        HAL_Delay(1);
#endif


        t += 0.01;

        electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
        electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
        electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
        electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
        electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
        electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

        HAL_Delay(1);

//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

      //  printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
      //         jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
      //         jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
    }
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    electron.lcd->isBusy = false;
    p++;
    if(p>=4 && USBD_Status==lcdP3)p=0;
    if(USBD_Status==lcdP3 || USBD_Status==lcdP1)
    {
        USBD_Status=SendUsbPacket;
    }
}
*/


/*
#include <cmath>
#include "common_inc.h"
#include "screen.h"
#include "robot.h"


Robot electron(&hspi1, &hi2c1);
float jointSetPoints[6];
bool isEnabled = false;

void Main(void)
{
    HAL_Delay(2000);
    electron.lcd->Init(Screen::DEGREE_0);
    electron.lcd->SetWindow(0, 239, 0, 239);

#if 1
    // 0.先只连接一个舵机,不设置地址，测试硬件和舵机固件是否OK。

    // 1.确保广播Joint的变量正确，直接更新 UpdateJointAngle
    //   可能会因为角度不在变量范围内不发送指令。（请详细读代码）
    electron.joint[0].id = 0;
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;

    // 2.使用广播地址是能
    electron.SetJointEnable(electron.joint[0], true);

    // 3.这时候就能看到舵机做往复运动了。
    while (1)
    {
        for (int i = -90; i < 90; i += 1)
        {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
        for (int i = 90; i > -90; i -= 1)
        {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
    }
#endif

#if 0
    // 0.当能够正常用广播地址控制单个舵机后，现在进行舵机的ID设置、
    //   还是只连接一个舵机

    // 1. 比如需要把这个舵机ID设置为2

    // 2.确保广播Joint的变量正确，直接更新 UpdateJointAngle
    //   可能会因为角度不在变量范围内不发送指令。（请详细读代码）
    electron.joint[0].id = 0;  //这里还是用广播
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;
    electron.SetJointId(electron.joint[0], 12);

    HAL_Delay(1000);
    // 3.等待舵机参数保存。将主控 joint[0].id改为 2
    electron.joint[0].id = 12;  //这里改为新地址
    electron.joint[0].angleMin = 0;
    electron.joint[0].angleMax = 180;
    electron.joint[0].angle = 0;
    electron.joint[0].modelAngelMin = -90;
    electron.joint[0].modelAngelMax = 90;

    // 4.使用新设置的2地址通讯
    electron.SetJointEnable(electron.joint[0], true);
    while (1)
    {
        for (int i = -90; i < 90; i += 1)
        {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
        for (int i = 90; i > -90; i -= 1)
        {
            float angle = i;
            electron.UpdateJointAngle(electron.joint[0], angle);
            HAL_Delay(20);
        }
    }

    // 5.之后的程序就不需要 步骤2中的 设置地址了。
    // 只需要用2地址，就能通讯上这个舵机，其他的舵机也是一样。

#endif

    float t = 0;

    while (true)
    {
#if 1
        for (int p = 0; p < 4; p++)
        {
            // send joint angles
            for (int j = 0; j < 6; j++)
                for (int i = 0; i < 4; i++)
                {
                    auto* b = (unsigned char*) &(electron.joint[j + 1].angle);
                    electron.usbBuffer.extraDataTx[j * 4 + i + 1] = *(b + i);
                }
            electron.SendUsbPacket(electron.usbBuffer.extraDataTx, 32);

            electron.ReceiveUsbPacketUntilSizeIs(224); // last packet is 224bytes

            // get joint angles
            uint8_t* ptr = electron.GetExtraDataRxPtr();
            if (isEnabled != (bool) ptr[0])
            {
                isEnabled = ptr[0];
                electron.SetJointEnable(electron.joint[1], isEnabled);
                electron.SetJointEnable(electron.joint[2], isEnabled);
                electron.SetJointEnable(electron.joint[3], isEnabled);
                electron.SetJointEnable(electron.joint[4], isEnabled);
                electron.SetJointEnable(electron.joint[5], isEnabled);
                electron.SetJointEnable(electron.joint[6], isEnabled);
            }
            for (int j = 0; j < 6; j++)
            {
                jointSetPoints[j] = *((float*) (ptr + 4 * j + 1));
            }

            while (electron.lcd->isBusy);
            if (p == 0)
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3);
            else
                electron.lcd->WriteFrameBuffer(electron.GetLcdBufferPtr(),
                                               60 * 240 * 3, true);
        }
        HAL_Delay(1);
#endif


        t += 0.01;

        electron.UpdateJointAngle(electron.joint[1], jointSetPoints[0]);
        electron.UpdateJointAngle(electron.joint[2], jointSetPoints[1]);
        electron.UpdateJointAngle(electron.joint[3], jointSetPoints[2]);
        electron.UpdateJointAngle(electron.joint[4], jointSetPoints[3]);
        electron.UpdateJointAngle(electron.joint[5], jointSetPoints[4]);
        electron.UpdateJointAngle(electron.joint[6], jointSetPoints[5]);

        HAL_Delay(1);

//      electron.UpdateJointAngle(electron.joint[ANY], 65 + 75 * std::sin(t));

        printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
               jointSetPoints[0], jointSetPoints[1], jointSetPoints[2],
               jointSetPoints[3], jointSetPoints[4], jointSetPoints[5]);
    }
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    electron.lcd->isBusy = false;
}*/