#ifndef  __ENCODER_H
#define  __ENCODER_H



#include "databyte.h"
#include "headfile.h"

void EncoderInit(Encoder_t *Encoder);
void EncodersInit(Motors_t *Motor);
void EncoderDataUpdate(Motors_t* Motor);
float EncoderTotalLengthGet(Motors_t* Motors);
void EncoderLengthClear(Motors_t* Motors);



#endif