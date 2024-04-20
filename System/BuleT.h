#ifndef __BuleT_H
#define __BuleT_H
#include "stdint.h"
void BuleInit(void);
void USART1_IRQHandler(void);
uint8_t BuleT_GetRxData(void);
void BulCon(char Date);

#endif
