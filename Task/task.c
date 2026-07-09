#include "task.h"

void Task_Init(Task_t **Task)
{
	static Task_t task;
	*Task=&task;
	task.AverageSpeed=0;
	task.Carstartflag=0;
	task.Datumyaw=0;
	task.LastTaskStat=0;
	task.LapCount=0;
	task.StTrPreFlag=0;
	task.TaskFlag=0;
	task.TaskState=0;
}


void Task1(Car_t Car)
{
	float yaw_error;
	if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL))
	{
		return;
	}
	if (Car.buzzer != NULL)
	{
		BuzzerDataUpdate(Car.buzzer);
	}
	if(Car.task->Carstartflag==0)
	{
		MotorStop(Car.Motors);
		return ;
	}
	GraySensorDataUpdate(Car.GraySensor_t);
	if(Car.task->TaskState==TASK1_STATE_IDLE)
	{
		PID_clear(Car.Motors->Left);
		PID_clear(Car.Motors->Right);
		PID_clear(Car.Motors->PidSelfTurn);
		
		Car.task->AverageSpeed=TASK1_SPEED;
		Car.task->Datumyaw=IMU_Attitude.YawTotalAngle;
		Car.task->Deltayaw=0;
		Car.task->TaskState=TASK1_STATE_LEAVE_A;
	}
	if(Car.task->TaskState==TASK1_STATE_LEAVE_A)
	{
		if(Car.GraySensor_t->BinaryData==0xFF)
		{
			Car.task->TaskState=TASK1_STATE_FIND_B;
		}
		yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
		Car.task->Deltayaw = yaw_error;
		MotorPidCtrl(Car.Motors, yaw_error, TASK1_SPEED);
		MotorDataUpdate(Car.Motors);
	}
	else if(Car.task->TaskState==TASK1_STATE_FIND_B)
	{
		if(Car.GraySensor_t->BinaryData!=0xFF)
		{
			MotorStop(Car.Motors);
			Car.task->AverageSpeed = 0;
			Car.task->Carstartflag = 0;
			Car.task->TaskState = TASK1_STATE_FINISH;
			if (Car.buzzer != NULL)
			{
				BuzzerBee(Car.buzzer);
			}

			led_up();
			return;
		}
		yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
		Car.task->Deltayaw = yaw_error;
		MotorPidCtrl(Car.Motors, yaw_error, TASK1_SPEED);
		MotorDataUpdate(Car.Motors);
	}
	else if(Car.task->TaskState==TASK1_STATE_FINISH)
	{
		MotorStop(Car.Motors);
	}
}


void Task2(Car_t Car)
{
	float yaw_error;
	float turn_angle;
	if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL))
	{
		return;
	}
	if (Car.buzzer != NULL)
	{
		BuzzerDataUpdate(Car.buzzer);
	}
	if(Car.task->StTrPreFlag > 0)
    {
        Car.task->StTrPreFlag--;
        led_drive(TASK2_LED_ON, LED_PORT, LED_PIN_13_PIN);
    }
    else
    {
        led_drive(TASK2_LED_OFF, LED_PORT, LED_PIN_13_PIN);
    }
		
		if(Car.task->Carstartflag==0)
		{
			MotorStop(Car.Motors);
			return;
		}
		
		
		GraySensorDataUpdate(Car.GraySensor_t);
		if(Car.task->TaskState==TASK2_STATE_IDLE)
		{
			PID_clear(Car.Motors->Left);
			PID_clear(Car.Motors->Right);
			PID_clear(Car.Motors->PidSelfTurn);
			Car.task->AverageSpeed=TASK2_STRAIGHT_SPEED;
			Car.task->Datumyaw=IMU_Attitude.YawTotalAngle;
			Car.task->TaskState=TASK2_STATE_AB_LEAVE_A;
			BuzzerBee(Car.buzzer);
			Car.task->StTrPreFlag = TASK2_LED_TICKS;
		}
		
		if(Car.task->TaskState==TASK2_STATE_AB_LEAVE_A)
		{
			if (Car.GraySensor_t->BinaryData == 0xFF)
        {
            Car.task->TaskState = TASK2_STATE_AB_FIND_B;
        }

        yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
        Car.task->Deltayaw = yaw_error;

        MotorPidCtrl(Car.Motors, yaw_error, TASK2_STRAIGHT_SPEED);
        MotorDataUpdate(Car.Motors);
		}
		else if(Car.task->TaskState==TASK2_STATE_AB_FIND_B)
		{
			if(Car.GraySensor_t->BinaryData!=0xFF)
			{
				BuzzerBee(Car.buzzer);
				Car.task->StTrPreFlag=TASK2_LED_TICKS;
				PID_clear(Car.Motors->PidSelfTurn);
				Car.task->AverageSpeed = TASK2_TRACE_SPEED;
				Car.task->LastTaskStat = 0;
				Car.task->TaskState = TASK2_STATE_BC_TRACE;
				return;
			}
			
			yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
			Car.task->Deltayaw = yaw_error;

			MotorPidCtrl(Car.Motors, yaw_error, TASK2_STRAIGHT_SPEED);
			MotorDataUpdate(Car.Motors);
			
		}
		else if(Car.task->TaskState==TASK2_STATE_BC_TRACE)
		{
			if(Car.GraySensor_t->BinaryData==0xFF)
			{
				Car.task->LastTaskStat++;
				if(Car.task->LastTaskStat>=TASK2_LINE_LOST_TICKS)
				{
					BuzzerBee(Car.buzzer);
					Car.task->StTrPreFlag=TASK2_LED_TICKS;
					Car.task->AverageSpeed=TASK2_STRAIGHT_SPEED;
					Car.task->Datumyaw=IMU_Attitude.YawTotalAngle;
					Car.task->Deltayaw=0;
					Car.task->LastTaskStat=0;
					Car.task->TaskState=TASK2_STATE_CD_FIND_D;
					PID_clear(Car.Motors->PidSelfTurn);
					return;
				}
				MotorPidCtrl(Car.Motors,0.0f,TASK2_TRACE_SPEED);
				MotorDataUpdate(Car.Motors);
			}
			else 
			{
				Car.task->LastTaskStat = 0;

				turn_angle = GraySensorToTurnAngle(Car.GraySensor_t);
				MotorPidCtrl(Car.Motors,turn_angle,TASK2_TRACE_SPEED);
				MotorDataUpdate(Car.Motors);
			}
		
		}
		else if(Car.task->TaskState==TASK2_STATE_CD_FIND_D)
		{
			if(Car.GraySensor_t->BinaryData!=0xFF)
			{
				BuzzerBee(Car.buzzer);
				Car.task->StTrPreFlag=TASK2_LED_TICKS;
				PID_clear(Car.Motors->PidSelfTurn);
				Car.task->AverageSpeed=TASK2_TRACE_SPEED;
				Car.task->LastTaskStat=0;
				Car.task->TaskState=TASK2_STATE_DA_TRACE;
				return;
			}
			yaw_error=IMU_Attitude.YawTotalAngle-Car.task->Datumyaw;
			Car.task->Deltayaw=yaw_error;
			MotorPidCtrl(Car.Motors,yaw_error,TASK2_STRAIGHT_SPEED);
			MotorDataUpdate(Car.Motors);
		}
		else if(Car.task->TaskState==TASK2_STATE_DA_TRACE)
		{
			if(Car.GraySensor_t->BinaryData==0xFF)
			{
				Car.task->LastTaskStat++;
				if(Car.task->LastTaskStat>=TASK2_LINE_LOST_TICKS)
				{
					MotorStop(Car.Motors);
					Car.task->AverageSpeed=0;
					Car.task->Carstartflag=0;
					Car.task->LastTaskStat=0;
					Car.task->TaskState=TASK2_STATE_FINISH;
					BuzzerBee(Car.buzzer);
					Car.task->StTrPreFlag=TASK2_LED_TICKS;
					return ;
				}
				MotorPidCtrl(Car.Motors, 0.0f, TASK2_TRACE_SPEED);
				MotorDataUpdate(Car.Motors);
				
			}
			else
			{
				Car.task->LastTaskStat=0;
				turn_angle = GraySensorToTurnAngle(Car.GraySensor_t);
				MotorPidCtrl(Car.Motors,turn_angle,TASK2_TRACE_SPEED);
				MotorDataUpdate(Car.Motors);
			}
			
		}
		else if(Car.task->TaskState==TASK2_STATE_FINISH)
		{
			MotorStop(Car.Motors);
		}
		
}


void Task34_Run(Car_t Car,int16 lap)
{
	float yaw_error;
	float turn_angle;
	if ((Car.task == NULL) || (Car.Motors == NULL) || (Car.GraySensor_t == NULL))
	{
		return;
	}
	if (Car.buzzer != NULL)
	{
		BuzzerDataUpdate(Car.buzzer);
	}
	
	if(Car.task->StTrPreFlag > 0)
    {
        Car.task->StTrPreFlag--;
		led_drive(TASK34_LED_ON, LED_PORT, LED_PIN_13_PIN);
    }
    else
    {
		led_drive(TASK34_LED_OFF, LED_PORT, LED_PIN_13_PIN);
    }
		
		if(Car.task->Carstartflag==0)
		{
			MotorStop(Car.Motors);
			return;
		}
	GraySensorDataUpdate(Car.GraySensor_t);
	if(Car.task->TaskState==TASK34_STATE_IDLE)
	{
		PID_clear(Car.Motors->Left);
		PID_clear(Car.Motors->Right);
		PID_clear(Car.Motors->PidSelfTurn);
		Car.task->AverageSpeed=TASK34_STRAIGHT_SPEED;
		Car.task->Datumyaw=IMU_Attitude.YawTotalAngle+TASK34_START_AC_YAW_OFFSET;
		Car.task->LapCount=0;
		Car.task->LastTaskStat=0;
		Car.task->Deltayaw=0;
		BuzzerBee(Car.buzzer);
		Car.task->StTrPreFlag=TASK34_LED_TICKS;
		Car.task->TaskState=TASK34_STATE_AC_LEAVE_A;
	}
	else if(Car.task->TaskState==TASK34_STATE_AC_LEAVE_A)
	{
		if(Car.GraySensor_t->BinaryData==0xFF)
		{
			Car.task->TaskState=TASK34_STATE_AC_FIND_C;
		}
		yaw_error=IMU_Attitude.YawTotalAngle-Car.task->Datumyaw;
		Car.task->Deltayaw=yaw_error;
		MotorPidCtrl(Car.Motors,yaw_error,TASK34_STRAIGHT_SPEED);
		MotorDataUpdate(Car.Motors);
		
	}
	else if(Car.task->TaskState==TASK34_STATE_AC_FIND_C)
	{
		if(Car.GraySensor_t->BinaryData!=0xFF)
		{
			BuzzerBee(Car.buzzer);
			Car.task->AverageSpeed=TASK34_TRACE_SPEED;
			Car.task->StTrPreFlag=TASK34_LED_TICKS;
			Car.task->LastTaskStat=0;
			Car.task->TaskState=TASK34_STATE_CB_TRACE;
			PID_clear(Car.Motors->PidSelfTurn);
			return ;
		}
		yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
		Car.task->Deltayaw = yaw_error;

		MotorPidCtrl(Car.Motors, yaw_error, TASK34_STRAIGHT_SPEED);
		MotorDataUpdate(Car.Motors);
	}
	else if(Car.task->TaskState==TASK34_STATE_CB_TRACE)
	{
		if(Car.GraySensor_t->BinaryData==0xFF)
		{
			Car.task->LastTaskStat++;
			if(Car.task->LastTaskStat>=TASK34_LINE_LOST_TICKS)
			{
				BuzzerBee(Car.buzzer);
				Car.task->StTrPreFlag = TASK34_LED_TICKS;
				Car.task->AverageSpeed = TASK34_STRAIGHT_SPEED;
				Car.task->Datumyaw = IMU_Attitude.YawTotalAngle + TASK34_BD_YAW_OFFSET;
				Car.task->Deltayaw = 0;
				Car.task->LastTaskStat = 0;
				Car.task->TaskState = TASK34_STATE_BD_FIND_D;

				PID_clear(Car.Motors->PidSelfTurn);
				return;
			}
			MotorPidCtrl(Car.Motors, 0.0f, TASK34_TRACE_SPEED);
			MotorDataUpdate(Car.Motors);
		}
		else
		{
			Car.task->LastTaskStat = 0;

			turn_angle = TASK34_CB_TRACE_SIGN * GraySensorToTurnAngle(Car.GraySensor_t);

			MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
			MotorDataUpdate(Car.Motors);
		}
	}
	else if(Car.task->TaskState==TASK34_STATE_BD_FIND_D)
	{
		if(Car.GraySensor_t->BinaryData!=0xFF)
		{
			BuzzerBee(Car.buzzer);
			Car.task->AverageSpeed=TASK34_TRACE_SPEED;
			Car.task->TaskState=TASK34_STATE_DA_TRACE;
			Car.task->LastTaskStat=0;
			Car.task->StTrPreFlag=TASK34_LED_TICKS;
			PID_clear(Car.Motors->PidSelfTurn);
			return ;
		}
		yaw_error = IMU_Attitude.YawTotalAngle - Car.task->Datumyaw;
		Car.task->Deltayaw = yaw_error;

		MotorPidCtrl(Car.Motors, yaw_error, TASK34_STRAIGHT_SPEED);
		MotorDataUpdate(Car.Motors);
	}
	else if(Car.task->TaskState==TASK34_STATE_DA_TRACE)
	{
		if(Car.GraySensor_t->BinaryData==0xFF)
		{
			Car.task->LastTaskStat++;
			if(Car.task->LastTaskStat>=TASK34_LINE_LOST_TICKS)
			{
				BuzzerBee(Car.buzzer);
				Car.task->StTrPreFlag=TASK34_LED_TICKS;
				Car.task->LapCount++;
				Car.task->LastTaskStat = 0;
				if(Car.task->LapCount>=lap)
				{
					MotorStop(Car.Motors);
					Car.task->AverageSpeed=0;
					Car.task->Carstartflag=0;
					Car.task->TaskState=TASK34_STATE_FINISH;
					return ;
				}
				Car.task->AverageSpeed = TASK34_STRAIGHT_SPEED;
				Car.task->Datumyaw = IMU_Attitude.YawTotalAngle + TASK34_NEXT_AC_YAW_OFFSET;
				Car.task->Deltayaw = 0;
				Car.task->TaskState = TASK34_STATE_AC_LEAVE_A;

				PID_clear(Car.Motors->PidSelfTurn);
				return ;
			}
			MotorPidCtrl(Car.Motors, 0.0f, TASK34_TRACE_SPEED);
			MotorDataUpdate(Car.Motors);
			
		}
		else
		{
			Car.task->LastTaskStat = 0;
			turn_angle = TASK34_DA_TRACE_SIGN * GraySensorToTurnAngle(Car.GraySensor_t);
			MotorPidCtrl(Car.Motors, turn_angle, TASK34_TRACE_SPEED);
			MotorDataUpdate(Car.Motors);
		}
	}
	else if(Car.task->TaskState==TASK34_STATE_FINISH )
	{
		MotorStop(Car.Motors);
	}
	
	
}


void Task3(Car_t Car)
{
    Task34_Run(Car, 1);
}

void Task4(Car_t Car)
{
    Task34_Run(Car, 4);
}
