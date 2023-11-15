#ifndef __ADC_APP_H__
#define __ADC_APP_H__

#include "main.h"
#include "adc.h"
#include "tim.h"

#define Vref (3.301f)
#define ADCRand	(4096)
#define BUFFER_LENGTH	(512)
#define ADC_HANDLE	(&hadc)

// ADC×ª»»×´Ì¬
#define DMA_NOT_CONV_COMPLETED	0
#define DMA_CONV_COMPLETED		1
#define DMA_CONV_FAILED			2

extern __IO  uint8_t ubDmaTransferStatus;
extern __IO uint16_t uhADCDualConvertedValue[BUFFER_LENGTH];

void ADC_Init(uint32_t rate, uint32_t length);
void StartADC(uint32_t length);
void StopADC(void);

#endif /* __ADC_APP_H__ */
