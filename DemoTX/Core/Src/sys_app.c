/**
  ******************************************************************************
  * @file    sys_app.c
  * @author  MCD Application Team
  * @brief   Initializes HW and SW system entities (not related to the radio)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "platform.h"
#include "sys_app.h"
#include "stm32_seq.h"
#include "stm32_systime.h"
#include "stm32_lpm.h"
#include "utilities_def.h"
#include "timer_if.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_TS_SIZE (int) 16
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static uint8_t SYS_TimerInitialisedFlag = 0;
uint16_t g_peakValue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void ADC_Conv_Process(void);
static uint16_t CalcPeek(uint16_t *pbuf, uint32_t len);
/* USER CODE END PFP */

/* Exported functions ---------------------------------------------------------*/
/**
  * @brief initialises the system (dbg pins, trace, mbmux, systiemr, LPM, ...)
  * @param none
  * @retval  none
  */
void SystemApp_Init(void)
{
  /* USER CODE BEGIN SystemApp_Init_1 */
  /* Ensure that MSI is wake-up system clock */
  __HAL_RCC_WAKEUPSTOP_CLK_CONFIG(RCC_STOP_WAKEUPCLOCK_MSI);
	
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC_Init();
  MX_DAC_Init();
  MX_TIM2_Init();

  /*Initialize timer and RTC*/
  UTIL_TIMER_Init();
  SYS_TimerInitialisedFlag = 1;
	  /*Init low power manager*/
  UTIL_LPM_Init();
  /* Disable Stand-by mode */
  UTIL_LPM_SetOffMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);

#if defined (LOW_POWER_DISABLE) && (LOW_POWER_DISABLE == 1)
  /* Disable Stop Mode */
  UTIL_LPM_SetStopMode((1 << CFG_LPM_APPLI_Id), UTIL_LPM_DISABLE);
#elif !defined (LOW_POWER_DISABLE)
#error LOW_POWER_DISABLE not defined
#endif /* LOW_POWER_DISABLE */

  HAL_UART_Receive_IT(&huart2, &dbg_rxdata, sizeof(dbg_rxdata)/sizeof(uint8_t));
  ADC_Init(200000, BUFFER_LENGTH);
  Generate_waveform_SW_update_Config();

  /* USER CODE END SystemApp_Init_1 */
}

/* USER CODE BEGIN ExF */

/* USER CODE END ExF */

/* Private functions ---------------------------------------------------------*/
/* USER CODE BEGIN PrFD */
static uint16_t CalcPeek(uint16_t *pbuf, uint32_t len)
{
	uint16_t min, max;
	uint16_t i = 0;
	min = max = pbuf[0];
	for(i = 1; i < len; i++)
	{
		if(pbuf[i] < min)
			min = pbuf[i];
		if(pbuf[i] > max)
			max = pbuf[i];
	}
	return (max - min);
}

void ADC_Conv_Process(void)
{
	g_peakValue = CalcPeek((uint16_t *)uhADCDualConvertedValue, BUFFER_LENGTH);
	p_info("ADC_Conv_Process completed!,g_peakValue=%d", g_peakValue);
	ubDmaTransferStatus = DMA_CONV_COMPLETED;
}
/* USER CODE END PrFD */
/* HAL overload functions ---------------------------------------------------------*/

/* Use #if 0 if you want to keep the default HAL istead overcharge them*/
/* USER CODE BEGIN Overload_HAL_weaks_1 */
#if 1
/* USER CODE END Overload_HAL_weaks_1 */

/**
  * @brief This function configures the source of the time base.
  * @brief  don't enable systick
  * @param TickPriority: Tick interrupt priority.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  /*Don't enable SysTick if TIMER_IF is based on other counters (e.g. RTC) */
  /* USER CODE BEGIN HAL_InitTick_1 */

  /* USER CODE END HAL_InitTick_1 */
  return HAL_OK;
  /* USER CODE BEGIN HAL_InitTick_2 */

  /* USER CODE END HAL_InitTick_2 */
}

/**
  * @brief Provide a tick value in millisecond measured using RTC
  * @note This function overwrites the __weak one from HAL
  * @retval tick value
  */
uint32_t HAL_GetTick(void)
{
  uint32_t ret = 0;
  /* TIMER_IF can be based on other counter the SysTick e.g. RTC */
  /* USER CODE BEGIN HAL_GetTick_1 */

  /* USER CODE END HAL_GetTick_1 */
  if (SYS_TimerInitialisedFlag == 0)
  {
    /* TIMER_IF_GetTimerValue should be used only once UTIL_TIMER_Init() is initialized */
    /* If HAL_Delay or a TIMEOUT countdown is necessary during initialization phase */
    /* please use temporarily another timebase source (SysTick or TIMx), which implies also */
    /* to rework the above function HAL_InitTick() and to call HAL_IncTick() on the timebase IRQ */
    /* Note: when TIMER_IF is based on RTC, stm32wlxx_hal_rtc.c calls this function before TimeServer is functional */
    /* RTC TIMEOUT will not expire, i.e. if RTC has an hw problem it will keep looping in the RTC_Init function */
    /* USER CODE BEGIN HAL_GetTick_EarlyCall */

    /* USER CODE END HAL_GetTick_EarlyCall */
  }
  else
  {
    ret = TIMER_IF_GetTimerValue();
  }
  /* USER CODE BEGIN HAL_GetTick_2 */

  /* USER CODE END HAL_GetTick_2 */
  return ret;
}

/**
  * @brief This function provides delay (in ms)
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */
void HAL_Delay(__IO uint32_t Delay)
{
  /* TIMER_IF can be based onother counter the SysTick e.g. RTC */
  /* USER CODE BEGIN HAL_Delay_1 */

  /* USER CODE END HAL_Delay_1 */
  TIMER_IF_DelayMs(Delay);
  /* USER CODE BEGIN HAL_Delay_2 */

  /* USER CODE END HAL_Delay_2 */
}
/* USER CODE BEGIN Overload_HAL_weaks_2 */
#endif
/* Redefine here your own if needed */

/* USER CODE END Overload_HAL_weaks_2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

