#ifndef  __VOFA_H
#define  __VOFA_H


#include "databyte.h"
#include "headfile.h"
void Vofa_SendByte(uint8_t data);
void Vofa_SendData(const uint8_t *data, uint16_t length);
void Vofa_Task(void);

void Vofa_Init(Motors_t *motors);
void Vofa_SendJustFloat(void);


#endif