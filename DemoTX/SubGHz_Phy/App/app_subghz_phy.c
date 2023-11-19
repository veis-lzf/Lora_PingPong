/**
  ******************************************************************************
  * @file    app_subghz_phy.c
  * @author  MCD Application Team
  * @brief   Application of the SubGHz_Phy Middleware
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
#include "app_subghz_phy.h"
#include "subghz_phy_app.h"
#include "sys_app.h"
#include "stm32_seq.h"

/* USER CODE BEGIN Includes */
#include "radio.h"
/* USER CODE END Includes */

/* External variables ---------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern uint8_t BufferTx[255];
extern volatile uint8_t TimFlag; // 定时器定时标记
/* USER CODE END EV */

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
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Exported functions --------------------------------------------------------*/

void MX_SubGHz_Phy_Init(void)
{
  /* USER CODE BEGIN MX_SubGHz_Phy_Init_1 */

  /* USER CODE END MX_SubGHz_Phy_Init_1 */
  SystemApp_Init();
  SubghzApp_Init();
  /* USER CODE BEGIN MX_SubGHz_Phy_Init_2 */

  /* USER CODE END MX_SubGHz_Phy_Init_2 */
}

void MX_SubGHz_Phy_Process(void)
{
  /* USER CODE BEGIN MX_SubGHz_Phy_Process_1 */
#if PING_PONG_TESET_ENABLE // 使用ST调度器
	UTIL_SEQ_Run(UTIL_SEQ_DEFAULT);
#else

#if ADC_TEST_ENABLE
	Radio.Sleep();
	if (ubDmaTransferStatus == DMA_CONV_COMPLETED)
	{
		//StopADC();
		APP_LOG(TS_ON, VLEVEL_L, "Master Tx start\n\r");
		BufferTx[0] = 0x55;
		BufferTx[1] = 0xaa;
		BufferTx[2] = (g_peakValue >> 8);
		BufferTx[3] = (g_peakValue & 0xff);
		BufferTx[4] = BufferTx[0] ^ BufferTx[1] ^ BufferTx[2] ^ BufferTx[3];
		Radio.Send(BufferTx, 5);
		//StartADC(BUFFER_LENGTH);
		ubDmaTransferStatus = DMA_NOT_CONV_COMPLETED;
	}

	HAL_Delay(1000);
#endif /* ADC_TEST_ENABLE */
	if(TimFlag)
	{
		p_dbg_track;
		uint32_t count = GetTickCount();
		uint16_t index = 0;
		
		// 赋值协议头
		BufferTx[index++] = 0x55;
		BufferTx[index++] = 0xaa;
		
		// 计数值为4字节
		BufferTx[index++] = (count >> 24) & 0xff;
		BufferTx[index++] = (count >> 16) & 0xff;
		BufferTx[index++] = (count >> 8) & 0xff;
		BufferTx[index++] = count & 0xff;
		BufferTx[index] = 0;
		
		// 计算校验和
		for(int i = 0; i < index; i++)
			BufferTx[index] ^= BufferTx[i];
		
		// 通过Lora发送数据
		Radio.Send(BufferTx, index);
		
		HAL_Delay(50);
		// 重新进入休眠
		Radio.Sleep();
		
		// 清除计数，重新启动捕获和定时器
		clearTickCount();
		TimFlag = 0;
		startExitCapture();
		StartCaptureTimer();
		p_dbg_track;
	}
#endif /* PING_PONG_TESET_ENABLE */
	
#if USE_ADC_SOURCEDATA_TEST
	if(ubDmaTransferStatus == DMA_CONV_COMPLETED)
	{
		StopADC();
		for(uint16_t i = 0; i < BUFFER_LENGTH; i++)
		{
			printf("%d\r\n", uhADCDualConvertedValue[i]);
		}
		StartADC(BUFFER_LENGTH);
	}
	HAL_Delay(1000);
#endif
  /* USER CODE END MX_SubGHz_Phy_Process_1 */
}

/* USER CODE BEGIN EF */

/* USER CODE END EF */

/* Private Functions Definition -----------------------------------------------*/
/* USER CODE BEGIN PrFD */

/* USER CODE END PrFD */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
