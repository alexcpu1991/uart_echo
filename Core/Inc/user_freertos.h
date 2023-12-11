#ifndef USER_FREERTOS_H_
#define USER_FREERTOS_H_
//----------------------------------------------------------------------
#include <debug_print.h>
#include "stm32g4xx_hal.h"
#include "cmsis_os.h"
#include "tim.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "usart.h"
//----------------------------------------------------------------------
extern QueueHandle_t xHandleQueueUartTx;
extern TaskHandle_t xHandleTaskPid;
//----------------------------------------------------------------------

/******************** Task create ****************************/
#define TASK_UART_READ           	1// (0-disable; 1-enable)
#define TASK_UART_WRITE          	1// (0-disable; 1-enable)

/******************** Queue size *****************************/
#define SIZE_QUEUE_UART_TX          10

/******************* Task priority ***************************/
#define PRIORITY_TASK_UART_READ           	3
#define PRIORITY_TASK_UART_WRITE          	3

/********************* Task stack ****************************/
#define STACK_TASK_UART_READ           		(configMINIMAL_STACK_SIZE)
#define STACK_TASK_UART_WRITE          		(configMINIMAL_STACK_SIZE)

//----------------------------------------------------------------------
void freertos_init(void);
void configureRunTime(void);
unsigned long getRunTimeCounter(void);
//void vApplicationIdleHook( void );
//----------------------------------------------------------------------
#endif
