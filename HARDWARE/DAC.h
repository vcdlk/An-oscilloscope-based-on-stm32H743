#ifndef __DAC_H
#define __DAC_H
#include "sys.h"

extern DAC_HandleTypeDef DAC1_Handler;//DAC¾ä±ú

void DacInit(void);
void dac1_InitForDMA(uint32_t _BufAddr, uint32_t _Count, uint32_t _DacFreq);



#endif