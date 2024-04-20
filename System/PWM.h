#ifndef __PWM_H
#define __PWM_H
#include "stdint.h"
void PWM_Init(void);
void PWM_SetCompare3(uint16_t Compare);  //右边
void PWM_SetCompare4(uint16_t Compare);  //左边
#endif
