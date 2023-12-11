#include "user_freertos.h"
//-------------------------------------------------------------------------------
TaskHandle_t xHandleTaskUartRead;
TaskHandle_t xHandleTaskUartWrite;
//-------------------------------------------------------------------------------
QueueHandle_t xHandleQueueUartTx;
//-------------------------------------------------------------------------------

/* to check the size of the heap */
size_t free_heap_size = 0;
size_t min_free_heap_size = 0;
//-------------------------------------------------------------------------------
void freertos_init(void);
void UartRead(void * pvParameters);
void UartWrite(void * pvParameters);

/* Init function for freertos objects */
void freertos_init(void)
{
	/* Create the queues */
	xHandleQueueUartTx=xQueueCreate(SIZE_QUEUE_UART_TX, sizeof(char*));
	vQueueAddToRegistry(xHandleQueueUartTx, "xHandleQueueUartTx");

	/* Create the threads */
	#if TASK_UART_READ
		DBGInfo("TaskUartRead create...");
		if(xTaskCreate(
				UartRead,
				"TaskUartRead",
				STACK_TASK_UART_READ,
				NULL,
				PRIORITY_TASK_UART_READ,
				&xHandleTaskUartRead
				) != pdPASS)
		{
			DBGInfo("fault!\n");
		}
		else
		{
			DBGInfo("OK!\n");
		}
	#endif
	//---------------------------------------------------------------------------
	#if TASK_UART_WRITE
		DBGInfo("TaskUartWrite create...");
		if(xTaskCreate(
				UartWrite,
				"TaskUartWrite",
				STACK_TASK_UART_WRITE,
				NULL,
				PRIORITY_TASK_UART_WRITE,
				&xHandleTaskUartWrite
				) != pdPASS)
		{
			DBGInfo("fault!\n");
		}
		else
		{
			DBGInfo("OK!\n");
		}
	#endif
}

/* Task UartRead */
void UartRead(void * pvParameters)
{
  /* for working with a ring buffer */
  static uint16_t old_pos;
  static uint16_t pos=0;
  static uint16_t size_single_block=0;
  static uint16_t size_first_block=0;
  static uint16_t size_second_block=0;
  static uint16_t GeneralBufSize=0;
  static uint16_t GeneralBufCnt=0;

  /* for working with string */
  static uint8_t* p_start_str;
  static uint8_t* p_end_str;
  static bool new_str = true;
  static uint16_t size_str_single_part = 0;
  static uint16_t size_str_first_part = 0;
  static uint16_t size_str_second_part = 0;
  char* ptr;

  /* IDLE interrupt enable */
  LL_USART_EnableIT_IDLE(USART1);

  for(;;)
  {
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	/* Calculate current position in buffer and check for new data available */
	pos = UART_RX_DMA_BUFFER_SIZE - LL_DMA_GetDataLength(DMA2, LL_DMA_CHANNEL_2);
	if (pos != old_pos)
	{                       /* Check change in received data */
		if (pos > old_pos)
		{                    /* Current position is over previous one */
			/*
			 * Processing is done in "linear" mode.
			 *
			 * Application processing is fast with single data block,
			 * length is simply calculated by subtracting pointers
			 *
			 * [   0   ]
			 * [   1   ] <- old_pos |------------------------------------|
			 * [   2   ]            |                                    |
			 * [   3   ]            | Single block (len = pos - old_pos) |
			 * [   4   ]            |                                    |
			 * [   5   ]            |------------------------------------|
			 * [   6   ] <- pos
			 * [   7   ]
			 * [ N - 1 ]
			 */
			size_single_block=pos-old_pos;

			GeneralBufSize=size_single_block;
		}
		else
		{
			/*
			 * Processing is done in "overflow" mode..
			 *
			 * Application must process data twice,
			 * since there are 2 linear memory blocks to handle
			 *
			 * [   0   ]            |---------------------------------|
			 * [   1   ]            | Second block (len = pos)        |
			 * [   2   ]            |---------------------------------|
			 * [   3   ] <- pos
			 * [   4   ] <- old_pos |---------------------------------|
			 * [   5   ]            |                                 |
			 * [   6   ]            | First block (len = N - old_pos) |
			 * [   7   ]            |                                 |
			 * [ N - 1 ]            |---------------------------------|
			 */

			size_first_block=UART_RX_DMA_BUFFER_SIZE-old_pos;
			size_second_block=pos;

			GeneralBufSize=size_first_block+size_second_block;
		}

		old_pos = pos;  /* Save current position as old for next transfers */

		/* data processing */
		do
		{
			if(usart_rx_dma_buffer[GeneralBufCnt] == '\n')
			{
				p_end_str = &usart_rx_dma_buffer[GeneralBufCnt];

				if(p_start_str > p_end_str)
				{
					size_str_first_part = &usart_rx_dma_buffer[UART_RX_DMA_BUFFER_SIZE] - p_start_str;
					size_str_second_part = p_end_str - &usart_rx_dma_buffer[0] + 1;

					/* Allocate memory */
					ptr = pvPortMalloc(size_str_first_part + size_str_second_part + 1);

					if(ptr != NULL)
					{
						memcpy(ptr, p_start_str, size_str_first_part);
						memcpy(ptr + size_str_first_part, &usart_rx_dma_buffer[0], size_str_second_part);
						*(ptr + size_str_first_part + size_str_second_part) = '\0';
						// send queue
						if(xQueueSendToBack(xHandleQueueUartTx, &ptr, portMAX_DELAY) == errQUEUE_FULL)
						{
							DBGInfo("queue full\n");
						}
					}
					else DBGInfo("memory allocate\n");
				}
				else
				{
					size_str_single_part = p_end_str - p_start_str + 1;

					/* Allocate memory */
					ptr = pvPortMalloc(size_str_single_part + 1);

					if(ptr != NULL)
					{
						memcpy(ptr, p_start_str, size_str_single_part);
						*(ptr + size_str_single_part) = '\0';
						// send queue
						if(xQueueSendToBack(xHandleQueueUartTx, &ptr, portMAX_DELAY) == errQUEUE_FULL)
						{
							DBGInfo("queue full\n");
						}
					}
					else DBGInfo("memory allocate\n");
				}

				new_str = true;
			}
			else
			{
				if(new_str)
				{
					p_start_str = &usart_rx_dma_buffer[GeneralBufCnt];
					new_str = false;
				}
			}

			GeneralBufCnt++;
			if(GeneralBufCnt == UART_RX_DMA_BUFFER_SIZE) GeneralBufCnt=0;
		}
		while(--GeneralBufSize);
	}
  }
}

/* Task UartWrite */
void UartWrite(void * pvParameters)
{
	static char *str = NULL;

	for(;;)
	{
		if (str != NULL)
		{
			/* Free allocated memory */
			vPortFree(str);
		}

		if(xQueueReceive(xHandleQueueUartTx, &str, portMAX_DELAY) == pdPASS)
		{
			uart_transmit((uint8_t*)str, strlen(str));
		}
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

/* For FreeRTOS Debug in CubeIDE */
void configureRunTime(void)
{
	HAL_TIM_Base_Start(&htim7);
}

unsigned long getRunTimeCounter(void)
{
   static unsigned long counter = 0;

   counter += __HAL_TIM_GET_COUNTER(&htim7);
	__HAL_TIM_SET_COUNTER(&htim7, 0);

   return counter;
}

/* To check the size of the heap */
void vApplicationIdleHook(void)
{
	free_heap_size = xPortGetFreeHeapSize();
	min_free_heap_size = xPortGetMinimumEverFreeHeapSize();
}
