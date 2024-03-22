#include "stdio.h"
#include "NuMicro.h"
#include "tmr.h"
#include "system_init.h"
#include "GUI.h"
#include "display.h"

// define constant
#define MaxSpeed	1000		//10Hz(0.1s)	led toogle speed
#define MinSpeed	50000		//0.2Hz(5s)		led toogle speed
#define SW_UP			PC9			//UP					JoyStick
#define SW_DOWN		PG4			//DOWN				JoyStick
#define SW_CTR		PG3			//CENTER			JoyStick
#pragma anon_union

// define global variable
uint32_t timecount;
uint32_t sec = 0;
uint32_t hour = 0;
uint32_t min = 0;
char buf[20];

// define function
void Clock_Task(void);
void clock_init(void);
void clock_tick(void);
void LED_showing(uint32_t SpeedCtl);
void GPIO_init(void);
uint32_t JoyStick(unsigned char BTN_state);

// Code for inserting Union for this version of Keil
#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#endif

// union for joystick pins
typedef union{
	struct{
		unsigned UP		:1;
		unsigned DOWN	:1;
		unsigned CTR	:1;
	};
unsigned char B;
}Pins;

Pins joystick_pins;

int main(void){	
    SYS_Init(); // System Initialize
	
	TMR0_Initial(); // Timer Initialize
		
	clock_init(); // clock initialize
	
	GPIO_init(); // GPIO Initialize
	
	Display_Init();	// Display Initialize
		
    while(1){
		char clock_buf[20]; // display buffer
			
		joystick_pins.UP = SW_UP; //SW_UP
        joystick_pins.DOWN = SW_DOWN; //SW_DOWN
        joystick_pins.CTR = SW_CTR;	//SW_CTR;
                
        LED_showing(JoyStick(joystick_pins.B)); // LED showing function(get value from joystick function)
		clock_tick(); //clock update
			
		sprintf(clock_buf, "%02d:%02d:%02d", hour, min, sec); // clock buffer
		Display_buf(clock_buf, 270, 1);	//clock display
	}
}
/* GPIO initialize */
void GPIO_init(void)
{
		GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT) ;	 // SW1
		GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT) ; 	 // LEDR1
		GPIO_SetMode(PH, BIT7, GPIO_MODE_OUTPUT) ;   // LEDG1
		GPIO_SetMode(PC, BIT9, GPIO_MODE_INPUT) ;	 // Joystyick_LEFT(UP)
		GPIO_SetMode(PG, BIT4, GPIO_MODE_INPUT) ;	 // Joystyick_RIGHT(DOWN)
		GPIO_SetMode(PG, BIT3, GPIO_MODE_INPUT) ;	 // Joystyick_CENTER
}

//time initialize
void clock_init(void){
		sec = 0; // second
		min = 0; // minute
		hour = 0; // hour
}

// clock update function
void clock_tick(void){
	static uint32_t old_timecount = 0;
	
	if((uint32_t)(timecount - old_timecount) < 10000)//default SpeedCtl : 1Hz
		return;
		
		old_timecount = timecount;	//update the old time
		sec++;

	//time update
	if (sec == 60){
        sec = 0;
        min++; // update minute
    if (min == 60){
            min = 0;
            hour++; // update hour
            if (hour == 24) hour = 0; // reset hour when it is 24
        }
    }
    sprintf(buf, "%02d:%02d:%02d", hour, min, sec); // clock buffer
    Display_buf(buf, 100, 100); // display clock
}

static uint32_t old_timecount = 0; // old time
static uint32_t SpeedCtl = 10000; // default speed

// Joystick function
uint32_t JoyStick(unsigned char BTN_state){
	if(timecount-old_timecount < 1000)return SpeedCtl;
	old_timecount = timecount;	//update the old time
		
	switch(BTN_state){
		case 0x06:	// when up button pressed
            if(SpeedCtl > MaxSpeed)
                SpeedCtl -= 1000;
			break;
		case 0x05:	// when down button pressed
            if(SpeedCtl < MinSpeed)
                SpeedCtl += 1000;
			break;
		case 0x03:	// when center button pressed
            SpeedCtl = 0; // stop LED
			break;
		default:
			SpeedCtl = SpeedCtl; // do not change the speed
			break;	
		}
	return SpeedCtl;
}

// LED showing function
void LED_showing(uint32_t SpeedCtl){
	static uint32_t old_timecount = 0;
	static unsigned char f=0x01;
	if(timecount-old_timecount >= SpeedCtl){
      	f^=0x01; // xor operation (change state of f)
		PH6=(f)?1:0; // LEDR1 display	
		PH7=(f)?0:1; // LEDG1 display
		old_timecount = timecount; // update old time
	}  	
}
