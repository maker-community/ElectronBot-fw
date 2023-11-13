#ifndef __FREERTOS_H
#define __FREERTOS_H

#ifdef __cplusplus
extern "C" {
#endif

// List of semaphores

// List of Tasks
/*--------------------------------- System Tasks -------------------------------------*/
extern osThreadId_t defaultTaskHandle;      // Usage: 800 Bytes stack
extern osThreadId_t gestureTaskHandle;
extern osThreadId_t gyroscopeTaskHandle;
extern osThreadId_t uartTaskHandle;
/*---------------------------------- User Tasks --------------------------------------*/

/*---------------- xxxK (used) / xxxK (for FreeRTOS) / xxxK (total) ------------------*/


#ifdef __cplusplus
}
#endif

#endif