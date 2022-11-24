/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "stdio.h"
#include "protocol.h"
#include "iap.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
/* USER CODE BEGIN PV */


/* USER CODE BEGIN PFP */


#define BUFFERSIZE 1200           //可以接收的最大字符个数
uint8_t ReceiveBuff[BUFFERSIZE]; //接收缓冲区
uint32_t recv_end_flag = 0,Rx_len;//接收完成中断标志，接收到字符长度


#define FLASH_JointStatusData ADDR_FLASH_SECTOR_11
void uart1_data(void);					//接收函数

#ifdef __GNUC__									//串口重定向
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END PFP */

void myPrintf(const char* format, ...)
{
    printf(format);
    HAL_Delay(20);
}

void uart1_data(void)
{
    if(recv_end_flag ==1)//接收完成标志
    {
        //printf("数据长度=%d\r\n",Rx_len);//打印接收到的数据长度
        //printf("数据内容:");
       /* printf("dataLen=%d\r\n",Rx_len);//打印接收到的数据长度
        printf("data:");
        for(int i=0;i<Rx_len;i++)
        {
            printf("%x",ReceiveBuff[i]);//向串口打印接收到的数据
        }
        printf("data:");
        for(int i=0;i<Rx_len;i++)
        {
            printf("%c",ReceiveBuff[i]);//向串口打印接收到的数据
        }
        HAL_Delay(20);*/
        //WriteFlashu8tou32(APPLICATION_ADDRESS,ReceiveBuff,Rx_len);
        ProtocolProcessing(ReceiveBuff,Rx_len);
        printf("\r\n");
        for(int i = 0; i < Rx_len ; i++) //清空接收缓存区
            ReceiveBuff[i]=0;//置0
        Rx_len=0;//接收数据长度清零
        recv_end_flag=0;//接收标志位清零0
    }
    //开启下一次接收
    HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
}
/* USER CODE END 4 */



typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
//#define APPLICATION_ADDRESS   (uint32_t)0x08004000
#define APPLICATION_ADDRESS   (uint32_t)0x08010000
uint32_t APPLICATION_ADDRESS_data=0;
extern uint8_t IAP_OK;
extern struct iapFlashSave_t iapFlashSave;

void jumpToApp()
{
    myPrintf("****jump to app****\r\n");
    HAL_Delay(50);

    HAL_UART_MspDeInit(&huart1);
    HAL_NVIC_DisableIRQ(DMA2_Stream2_IRQn);
    __HAL_RCC_DMA2_CLK_DISABLE();
    HAL_NVIC_DisableIRQ(USART1_IRQn);
    __disable_irq();
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除中断标志



    JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
    Jump_To_Application = (pFunction) JumpAddress;

    __set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
    __set_FAULTMASK(1);
    Jump_To_Application();
}


uint32_t timeCounter=0;
char VERSION[]={"1.0.1.0"};//char VERSION[]={"1.0.0.0"};
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();


    memset(ReceiveBuff,0,sizeof (ReceiveBuff));
    recv_end_flag = 0,
    Rx_len=0;

  MX_DMA_Init();
  MX_USART1_UART_Init();
  HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,BUFFERSIZE);
  __HAL_UART_CLEAR_IDLEFLAG(&huart1);
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    HAL_Delay(1000);
    myPrintf("------------------------------------ \r\n");
    myPrintf("ElectronBot-IAP \r\n");
    myPrintf("version is %s\r\n",VERSION);
    myPrintf("------------------------------------ \r\n");
    //myPrintf("IAP program is running \r\n");
    //VersionSubmission(VERSION,sizeof(VERSION));

    HAL_Delay(1000);

    IAP_Clean();
    IAP_ReadIapStatusFromFlash();
    IAP_OK=0;

    APPLICATION_ADDRESS_data=0;

    //uint8_t ReceiveBuff[BUFFERSIZE]; //接收缓冲区

    uint8_t status=0;

    if(iapFlashSave.iapOkFlag==IAP_OK_FLAG_VALVE)
    {
        if(IAP_CrcCheck())
        {
            myPrintf("crc32 check ok!!!\r\n");
            HAL_Delay(10);
            jumpToApp();
        }
        else
        {
            myPrintf("crc32 check fail!!!\r\n");
            HAL_Delay(10);
        }
    }
    else if(iapFlashSave.appIapEnFlag==IAP_APP_IAP_EN_FLAG)
    {
        timeCounter=0;
    }

    while(1)
    {
        if(timeCounter>=1000 && (false==IAP_DownloadingIs()))
        {
            timeCounter=0;
            myPrintf("app is not exist\r\n");
        }
        uart1_data();
    }

  /* USER CODE END 3 */
}




/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
