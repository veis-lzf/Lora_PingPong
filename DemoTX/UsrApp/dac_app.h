#ifndef __DAC_APP_H__
#define __DAC_APP_H__

#include "main.h"
#include "dac.h"

#if DAC_TEST_ENABLE

#define WAVE_BUFFER_SIZE	128

/* DAC初始化及基本操做接口函数 */
void bsp_dac_init(void);
void bsp_set_dac_value(uint16_t value);
void bsp_set_dac_voltage(float value);

/* 波形发生接口函数 */
void dacl_SetSinWave(uint32_t vpp, uint32_t freq);
void dacl_SetTriWave(uint16_t _low, uint16_t _high, uint32_t freq, uint8_t _duty);
void dacl_SetRectWave(uint16_t _low, uint16_t _high, uint32_t freq, uint8_t _duty);
void dacl_SetSawtoothWave(uint16_t _low, uint16_t _high, uint32_t freq);
 
#endif /* DAC_TEST_ENABLE */

#endif /* __DAC_APP_H__ */
