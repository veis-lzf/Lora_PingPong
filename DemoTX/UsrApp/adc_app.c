#include "adc_app.h"
#include "debug.h"

/* DMA缓冲区变量 ----------------------------------------------------------------------*/
__IO uint16_t uhADCDualConvertedValue[BUFFER_LENGTH];		// ADC采集缓冲区大小
__IO uint16_t uhADCDualConver = 0;

/* Variable to report status of DMA transfer of ADC group regular conversions */
/*  0: DMA transfer is not completed                                          */
/*  1: DMA transfer is completed                                              */
/*  2: DMA transfer has not yet been started yet (initial state)              */
__IO   uint8_t ubDmaTransferStatus = DMA_CONV_FAILED; /* Variable set into DMA interruption callback */


static void TIM_Config(uint32_t rate);

void ADC_Init(uint32_t rate, uint32_t length)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	
	MX_ADC_Init();
	
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	// 设置指定ADC的规则组通道，一个序列，采样时间
	if(rate >= 500000) sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;	        // ADC1,ADC通道,采样时间为1.5+12.5=14周期	  			    
	else if(rate >=  200000) sConfig.SamplingTime = ADC_SAMPLETIME_7CYCLES_5;	// ADC1,ADC通道,采样时间为7.5+12.5=20周期	  			    
	else if(rate >= 100000) sConfig.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;	// ADC1,ADC通道,采样时间为13.5+12.5=26周期	  			    
	else if(rate >= 40000) sConfig.SamplingTime = ADC_SAMPLETIME_79CYCLES_5;	// ADC1,ADC通道,采样时间为79.5+12.5=92周期	  			    
	else sConfig.SamplingTime = ADC_SAMPLETIME_160CYCLES_5;	                    // ADC1,ADC通道,采样时间为160.5+12.5=173周期
	
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	
	StartADC(length);
	TIM_Config(rate);
	
}

// 启动ADC转换
void StartADC(uint32_t length)
{
	if (HAL_ADCEx_Calibration_Start(&hadc) != HAL_OK)
	{
		/* Calibration Error */
		Error_Handler();
	}
	if (HAL_ADC_Start_DMA(ADC_HANDLE, (uint32_t *)uhADCDualConvertedValue, length) != HAL_OK)
	{
		/* ADC conversion start error */
		Error_Handler();
	}  
}

// 停止ADC转换
void StopADC(void)
{
	HAL_ADC_Stop_DMA(ADC_HANDLE);
}

static void TIM_Config(uint32_t rate)
{
	if (HAL_TIM_Base_Stop(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	
	if(rate >= 10000) // 10k以上高精度计数
	{
		htim2.Init.Prescaler = 8000000 / rate - 1; // 10us触发一次
		htim2.Init.Period = 6 - 1;
	}
	else if(rate <= 20) //  20Hz以下低频计数
	{
		htim2.Init.Prescaler = 10000 / rate - 1;  // 50ms触发一次
		htim2.Init.Period = 4800 - 1;
	}
	else
	{
		htim2.Init.Prescaler = 1000000 / rate - 1; // 1ms触发一次
		htim2.Init.Period = 48 - 1;
	}
	
	htim2.Instance = TIM2;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	// 启动定时器计数
	if (HAL_TIM_Base_Start(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
}
extern void ADC_Conv_Process(void *);
/* ADC转换完成回调函数 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc_x)
{
	if(hadc_x == ADC_HANDLE)
	{	
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		ADC_Conv_Process(NULL);
	}
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef *hadc_x)
{
	if(hadc_x == ADC_HANDLE)
	{
		//ubDmaTransferStatus = DMA_NOT_CONV_COMPLETED;
		//p_info("----------");
	}
}
