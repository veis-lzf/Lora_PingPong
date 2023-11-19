#include "tim.h"
#include "tim_app.h"
#include "exit_app.h"

// 定时器定时标记
// 1：表示定时中断已触发
// 0：表示未触发
volatile uint8_t TimFlag;

/***************************************************************************
 ** \brief	 InitCaptureTimer
 **			 
 **	\param [in] ms_tick: 定时器中断间隔，单位是ms         
 ** \return 无
 ** \note
***************************************************************************/
void InitCaptureTimer(uint32_t ms_tick)
{
	HAL_TIM_Base_DeInit(&htim16);
	
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 48000;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = ms_tick;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
	{
	Error_Handler();
	}

	HAL_TIM_Base_Start_IT(&htim16);
}

/***************************************************************************
 ** \brief	 StartCaptureTimer
 **			      
 ** \return 无
 ** \note
***************************************************************************/
void StartCaptureTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim16); // 停止定时器中断
}

/***************************************************************************
 ** \brief	 stopCaptureTimer
 **			      
 ** \return 无
 ** \note
***************************************************************************/
void stopCaptureTimer(void)
{
	HAL_TIM_Base_Stop_IT(&htim16); // 停止定时器中断
}

/***************************************************************************
 ** \brief	 HAL_TIM_PeriodElapsedCallback
 **			 
 **	\param [in] htim: 定时器句柄       
 ** \return 无
 ** \note
***************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim16)
	{
		/*
		uint32_t tick;
		p_dbg_track;
		tick = GetTickCount();
		p_info("EXIT Count:%d", tick);
		*/
		//p_dbg_track;
		stopExitCapture(); // 停止触发
		stopCaptureTimer(); // 停止定时器，等待处理完成
		TimFlag = 1;
	}
}
