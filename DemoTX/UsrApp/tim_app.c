#include "tim.h"
#include "tim_app.h"
#include "exit_app.h"

// ��ʱ����ʱ���
// 1����ʾ��ʱ�ж��Ѵ���
// 0����ʾδ����
volatile uint8_t TimFlag;

/***************************************************************************
 ** \brief	 InitCaptureTimer
 **			 
 **	\param [in] ms_tick: ��ʱ���жϼ������λ��ms         
 ** \return ��
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
 ** \return ��
 ** \note
***************************************************************************/
void StartCaptureTimer(void)
{
	HAL_TIM_Base_Start_IT(&htim16); // ֹͣ��ʱ���ж�
}

/***************************************************************************
 ** \brief	 stopCaptureTimer
 **			      
 ** \return ��
 ** \note
***************************************************************************/
void stopCaptureTimer(void)
{
	HAL_TIM_Base_Stop_IT(&htim16); // ֹͣ��ʱ���ж�
}

/***************************************************************************
 ** \brief	 HAL_TIM_PeriodElapsedCallback
 **			 
 **	\param [in] htim: ��ʱ�����       
 ** \return ��
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
		stopExitCapture(); // ֹͣ����
		stopCaptureTimer(); // ֹͣ��ʱ�����ȴ��������
		TimFlag = 1;
	}
}
