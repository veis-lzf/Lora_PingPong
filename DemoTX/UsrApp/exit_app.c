#include "exit_app.h"

volatile static uint32_t m_TickCount = 0;
static uint8_t m_exit_type;

/***************************************************************************
 ** \brief	 IncTickCount
 **			       
 ** \return ��
 ** \note
***************************************************************************/
static void IncTickCount(void)
{
	m_TickCount++;
}

/***************************************************************************
 ** \brief	 InitExitCapture
 **			 
 **	\param [in] type: �ж�����         
 ** \return ��
 ** \note
***************************************************************************/
void InitExitCapture(eEXIT_Type_t type)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	// ʹ���ⲿ�ж����ڵ�GPIOʱ��
	__HAL_RCC_GPIOC_CLK_ENABLE();
	
	// �����ⲿ�ж�������ź�����
	GPIO_InitStruct.Pin = EXT_INPUT_Pin;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	
	m_exit_type = (uint8_t)type;
	switch(m_exit_type)
	{
		case e_Falling: // �½���
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
			GPIO_InitStruct.Pull = GPIO_PULLUP;
			break;
		case e_Rising: // ������
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
			GPIO_InitStruct.Pull = GPIO_PULLDOWN;
			break;
		case e_Falling_Rising: // ˫����
			GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			break;
		case e_Low_Level: // �͵�ƽ
			break;
		case e_High_Level: // �ߵ�ƽ
			break;
	}
	
	HAL_GPIO_Init(EXT_INPUT_GPIO_Port, &GPIO_InitStruct);
	
	// �����ж����ȼ���ʹ���ⲿ�ж��ж�
	HAL_NVIC_SetPriority(EXTI2_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

/***************************************************************************
 ** \brief	 startExitCapture
 **			         
 ** \return ��
 ** \note
***************************************************************************/
void startExitCapture(void)
{
	HAL_NVIC_EnableIRQ(EXTI2_IRQn);
}

/***************************************************************************
 ** \brief	 GetTickCount
 **			         
 ** \return ��ǰ����ֵ
 ** \note
***************************************************************************/
uint32_t GetTickCount(void)
{
	return m_TickCount;
}

/***************************************************************************
 ** \brief	 stopExitCapture
 **			         
 ** \return ��
 ** \note
***************************************************************************/
void stopExitCapture(void)
{
	HAL_NVIC_DisableIRQ(EXTI2_IRQn);
}

/***************************************************************************
 ** \brief	 stopExitCapture
 **			         
 ** \return ��
 ** \note
***************************************************************************/
void clearTickCount(void)
{
	m_TickCount = 0;
}

/***************************************************************************
 ** \brief	 HAL_GPIO_EXTI_Callback
 **			 
 **	\param [in] GPIO_Pin: �����жϵ�GPIO���         
 ** \return ��
 ** \note
***************************************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
//	p_dbg_track;
	if(GPIO_Pin == EXT_INPUT_Pin)
	{
		// ���ڴ�����Ƶ���ĳ��ϣ��簴�����
	#if USR_IN_KEY_DET
		HAL_Delay(10);
		GPIO_PinState pinState = HAL_GPIO_ReadPin(EXT_INPUT_GPIO_Port, EXT_INPUT_Pin);
		switch(m_exit_type)
		{
			case e_Falling:
				if(pinState != GPIO_PIN_SET) {
					return;
				}
				break;
			case e_Rising:
				if(pinState != GPIO_PIN_RESET) {
					return;
				}
				break;
		}
	#endif
		IncTickCount(); // �����½��ضԼ������������Լ�
	}
	__HAL_GPIO_EXTI_CLEAR_IT(EXT_INPUT_Pin);
}
