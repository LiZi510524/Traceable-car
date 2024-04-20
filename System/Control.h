#ifndef __Control_H
#define __Control_H

extern float Integral_bias;
int Position_PD (int Encoder,int Target);
int Position_PID (float Encoder,int Target);
void Motor(void);
void Xianfu_PID(void);
int readOUT(int num);

#endif
