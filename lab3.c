#include "stdio.h"
#include "NuMicro.h"
#include "tmr.h"
#include "system_init.h"
#include "GUI.h"
#include "display.h"
#include "BNCTL.h"
#include "StepMotorAgent.h"

/* define max and mini speed */
#define MaxSpeed	17
#define MinSpeed	1

/* global variable define */
uint32_t	timecount;
uint8_t		dir;
uint32_t	speed;
	
int main(void){
	/* local variable define */
	char motor_state_buf[30];
	char SPD_buf[30];
	char direction_buf[30];
	uint32_t  speedCTL;
	
	/* Init System */
    SYS_Init();
	
	/*button initialize*/
	BTN_init();
	
	/*Step Motor initialize*/
	StepMtr_Initial();
	
    /* GUI display initialize */
	Display_Init();
	
	/* Init TMR0 for timecount */
	TMR0_Initial();
	
	/* Set initial value for speed direction */
	dir = 0x01;
	speed = 10;
		
    while(1){
		/* Scan button */
		BTN_task();

		if(Btn_IsOneShot(0x01) == 0x01){
            //stop
			GUI_Clear(); // clear windows
			Btn_OneShotClear(0x01); //clear oneshot flag
            speed = 0;
		}
		if(Btn_IsOneShot(0x02) == 0x02){
            //direction control
			dir ^= 0x01; //xor to dir
			GUI_Clear(); //clear windows
			Btn_OneShotClear(0x02); //clear oneshot flag
		}				
		if(Btn_IsOneShot(0x04) == 0x04){
            //speed up
			GUI_Clear();//clear windows
			Btn_OneShotClear(0x04);//clear oneshot flag
            if(speed < MaxSpeed)
                speed++;
		}
		if(Btn_IsOneShot(0x08) == 0x08){
            //speed down
			GUI_Clear(); //clear windows
			Btn_OneShotClear(0x08); //clear oneshot flag
			if(speed == 0) //let speed not to be 0
				speed = 1;
      else if(speed > MinSpeed)
                speed--;// let speed won't get over minspeed
		}
					
		/* Step motor output */
		if(speed)
			speedCTL = 1000/speed;
		else
			speedCTL = 0;
				
		StepMtr_Task(dir, speedCTL);
				
        //writ motor state buffer
		sprintf(SPD_buf,"speed : %02d rpm" , speed*6);//6~102
        //writ direction buffer

        //Display_buf(, 1, 1);motor state
        if(speed == 0){
					Display_buf("motor state : Stopped", 1, 1);
        }
        else{
					Display_buf("motor state : Running", 1, 1);
        }

				Display_buf(SPD_buf, 1, 25);
        if(dir)
            Display_buf("direction : Clockwise", 1, 49);
        else
            Display_buf("direction : CounterClockwise", 1, 49);
       
	}
}
