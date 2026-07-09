#include "Encoder.h"

void EncoderInit(Encoder_t *Encoder)
{
	Encoder->EncoderCount=0;
	Encoder->t_int=0;
	Encoder->v_int=0;
	
	Encoder->V=0;
	Encoder->X=0;
	Encoder->pos=0;
	Encoder->vel=0;
	
}


void EncodersInit(Motors_t *Motor)
{
	static Encoder_t encoderLeftFront;
	static Encoder_t encoderRightFront;


  Motor->EncoderLeft=&encoderLeftFront;
	Motor->EncoderRight=&encoderRightFront;
	
	NVIC_EnableIRQ(Encoder1_INT_IRQN);
	NVIC_EnableIRQ(Encoder1_INT_IRQN);
	
	
	EncoderInit(Motor->EncoderLeft);
	EncoderInit(Motor->EncoderRight);
	
}

void EncoderDataUpdate(Motors_t* Motor)
{
	Motor->EncoderLeft->v_int=Motor->EncoderLeft->EncoderCount;
	Motor->EncoderRight->v_int=Motor->EncoderRight->EncoderCount;
	
	Motor->EncoderLeft->EncoderCount=0;
	Motor->EncoderRight->EncoderCount=0;
	
	
	
	float temp=(EncoderReadingFrequency/EncoderLines)*2*PI;
	
	Motor->EncoderLeft->vel=(Motor->EncoderLeft->v_int*temp)*0.2+Motor->EncoderLeft->vel*0.8;
	Motor->EncoderRight->vel=(Motor->EncoderRight->v_int*temp)*0.2+Motor->EncoderRight->vel*0.8;
	
	Motor->EncoderLeft->V=Motor->EncoderLeft->vel*TireRadius;
	Motor->EncoderRight->V=Motor->EncoderRight->vel*TireRadius;
	
	Motor->EncoderLeft->X+=Motor->EncoderLeft->V/EncoderReadingFrequency;
	Motor->EncoderRight->X+=Motor->EncoderRight->V/EncoderReadingFrequency;
	
}


float EncoderTotalLengthGet(Motors_t* Motors)
{
 return (Motors->EncoderLeft->X+Motors->EncoderRight->X)/2.0f;
 
}


void EncoderLengthClear(Motors_t* Motors)
{
 Motors->EncoderLeft->X=0;
 Motors->EncoderRight->X=0;
 
}