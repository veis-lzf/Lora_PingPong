#ifndef __DAC_APP_H__
#define __DAC_APP_H__

#include "main.h"
#include "dac.h"

/* refer to reference manual).  */
#define DIGITAL_SCALE_12BITS             ((uint32_t) 0xFFF)

void Generate_waveform_SW_update_Config(void);
void Generate_waveform_SW_update(void);
  
#endif /* __DAC_APP_H__ */
