/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32g4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32g4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
extern TaskHandle_t xHandleTaskUartRead;
extern TaskHandle_t xHandleTaskUartWrite;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim20;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32G4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32g4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */
	/************** UART1_TX ***************/
  /* USER CODE END DMA1_Channel1_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* Check transfer complete */
  if (LL_DMA_IsActiveFlag_TC1(DMA1))
  {
	 /* Clear flag */
	 LL_DMA_ClearFlag_TC1(DMA1);

	 /* Call task UartWrite */
	 BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

	 vTaskNotifyGiveFromISR(xHandleTaskUartWrite, &pxHigherPriorityTaskWoken);
	 portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
  }

  /* Check transfer error */
  else if (LL_DMA_IsActiveFlag_TE1(DMA1))
  {
	 /* Clear flag */
	 LL_DMA_ClearFlag_TE1(DMA1);
  }

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */

	/* Check for IDLE line interrupt */
	if (LL_USART_IsEnabledIT_IDLE(USART1) && LL_USART_IsActiveFlag_IDLE(USART1))
	{
		/* Clear IDLE line flag */
		LL_USART_ClearFlag_IDLE(USART1);

		/* Call task UartRead */
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

		vTaskNotifyGiveFromISR(xHandleTaskUartRead, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 channel2 global interrupt.
  */
void DMA2_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Channel2_IRQn 0 */
  /************** UART1_RX ***************/
  /* USER CODE END DMA2_Channel2_IRQn 0 */

  /* USER CODE BEGIN DMA2_Channel2_IRQn 1 */

	/* Check half-transfer complete interrupt */
	if (LL_DMA_IsEnabledIT_HT(DMA2, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_HT2(DMA2))
	{
		/* Clear half-transfer complete flag */
		LL_DMA_ClearFlag_HT2(DMA2);
	}

	/* Check transfer-complete interrupt */
	if (LL_DMA_IsEnabledIT_TC(DMA2, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TC2(DMA2))
	{
		/* Clear transfer complete flag */
		LL_DMA_ClearFlag_TC2(DMA2);
	}

	/* Check transfer error */
	else if (LL_DMA_IsEnabledIT_TE(DMA2, LL_DMA_CHANNEL_2) && LL_DMA_IsActiveFlag_TE2(DMA2))
	{
		/* Clear flag */
		LL_DMA_ClearFlag_TE2(DMA2);
	}

  /* USER CODE END DMA2_Channel2_IRQn 1 */
}

/**
  * @brief This function handles TIM20 update interrupt.
  */
void TIM20_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM20_UP_IRQn 0 */

  /* USER CODE END TIM20_UP_IRQn 0 */
  HAL_TIM_IRQHandler(&htim20);
  /* USER CODE BEGIN TIM20_UP_IRQn 1 */

  /* USER CODE END TIM20_UP_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
