#include "MotorControl.h"


fp32 BrushMotor_PID[3] = {BrushMotor_PID_KP , BrushMotor_PID_KI	,	BrushMotor_PID_KD	};
fp32 SelfTurn_PID[3] 	 = {SelfTurn_PID_KP , SelfTurn_PID_KI	,	SelfTurn_PID_KD	};

void BrushMotorInit (BrushMotor_t* BrushMotor , int ERF , int EL , float TR)
{
	BrushMotor->state = Disable; 

	/*---------- 控制参数 ----------*/
	BrushMotor->ERF= ERF;
	BrushMotor->EL=  EL;
	BrushMotor->TR=  TR;
	BrushMotor->ExpectOutput = 0;
	BrushMotor->Output = 0;
}

void MotorInit (Motors_t** Motors)
{
 static Motors_t motors;
*Motors = &motors;
	PWMStart(PWM_0_INST);
 PWM_OUTput(0,0);
 
 static BrushMotor_t motorLeftFront;
 static BrushMotor_t motorRightFront;
 static Pid_t pidselfturn;
 static Pid_t pidLeftFront;
 static Pid_t pidRightFront;
 
 motors.MotorLeft = &motorLeftFront;
 motors.MotorRight = &motorRightFront;
 
 motors.PidSelfTurn=&pidselfturn;
 
 motors.Left=&pidLeftFront;
 motors.Right=&pidRightFront;
 
 BrushMotorInit(motors.MotorLeft,EncoderReadingFrequency,EncoderLines,TireRadius);
 BrushMotorInit(motors.MotorRight,EncoderReadingFrequency,EncoderLines,TireRadius);
 
 PID_init(motors.PidSelfTurn,SelfTurn_PID_mode ,SelfTurn_PID ,SelfTurn_PID_Maxout,SelfTurn_PID_MaxIout);
 PID_init(motors.Left,BrushMotor_PID_mode,BrushMotor_PID,BrushMotor_PID_Maxout,BrushMotor_PID_MaxIout);
 PID_init(motors.Right,BrushMotor_PID_mode,BrushMotor_PID,BrushMotor_PID_Maxout,BrushMotor_PID_MaxIout);
 
 
}
 
void MotorsPWMSet(Motors_t* Motors)
{
	int pwm[2];
	
	pwm[0]=ABS(Motors->MotorLeft->Output);
	
	pwm[1]=ABS(Motors->MotorRight->Output);

   PWM_OUTput(pwm[0],pwm[1]);
}

void MotorStop(Motors_t* Motors)
{
	Motors->MotorLeft->Output=0;
	Motors->MotorRight->Output=0;
	
	MotorDataUpdate(Motors);
	
}



void MotorDirectionSet(Motors_t* Motors)
{
	(Motors->MotorLeft->Output>0)?(DL_GPIO_setPins(Motor_PORT,Motor_PIN_0_PIN)):(DL_GPIO_clearPins(Motor_PORT,Motor_PIN_0_PIN));
	(Motors->MotorRight->Output>0)?(DL_GPIO_setPins(Motor_PORT,Motor_PIN_1_PIN)):(DL_GPIO_clearPins(Motor_PORT,Motor_PIN_1_PIN));
}

void MotorPidCtrl (Motors_t* Motors,fp32 TurnAngleSet,fp32 AverageSpeedSet)
{
	float ExpectDifferentia=PID_Calc(Motors->PidSelfTurn,TurnAngleSet,0);
	Motors->MotorLeft->ExpectOutput=AverageSpeedSet-ExpectDifferentia;
	Motors->MotorRight->ExpectOutput=AverageSpeedSet+ExpectDifferentia;
	
	Motors->MotorLeft->Output=PID_Calc(Motors->Left,Motors->EncoderLeft->V,Motors->MotorLeft->ExpectOutput);
	Motors->MotorRight->Output=PID_Calc(Motors->Right,Motors->EncoderRight->V,Motors->MotorRight->ExpectOutput);
	
}

void MotorDataUpdate (Motors_t* Motors)
{
	MotorDirectionSet(Motors);
	MotorsPWMSet(Motors);
}
