
#include "starter.h"
#include "seven.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

#define delaydigit 99

static void seven_init(void);
static void sevevsegmentloop(void);
static void dosomething(void);
LOOPSTT		loopstate;






//////////////////////////////// PORT CONFIG ////////////////////////////
///
///
void GPIO_port_init (void)
{
	GPIO_InitTypeDef	GPIO_InitS;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE );
	//onboard LED
	GPIO_InitS.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitS.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_InitS.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitS.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	//nggo 7segment abcdefg
	GPIO_InitS.GPIO_Pin = 0xFFFF; //GPIO_Pin_All;
	GPIO_Init (GPIOE, &GPIO_InitS);
	//nggo led 8 10 12 , common pin 13 14 15
	GPIO_InitS.GPIO_Pin = 0xF500; //(GPIO_Pin_8,10,12,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_Init (GPIOD, &GPIO_InitS);
	GPIOD->BSRRH = GPIO_Pin_8; //reset=on
	//onboard pushbutton
	GPIO_InitS.GPIO_Mode	= GPIO_Mode_IN;
	GPIO_InitS.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitS.GPIO_PuPd	= GPIO_PuPd_DOWN;
	GPIO_InitS.GPIO_Pin		= 0x0A00; //(GPIO_Pin_9|GPIO_Pin_11);
	GPIO_Init (GPIOD, &GPIO_InitS);
	GPIOD->BSRRH = GPIO_Pin_10; //reset=on
}
/////////////////////////////// END PORT CONFIG //////////////////////////

// init
static uint32_t coreticks, coretocks;

void PeripheralInit(void){
	SystemInit();
	coreticks = 0;
	coretocks = 0x80000000;
	while(SysTick_Config(SystemCoreClock/TICKINTERVAL)!=0){} //init & start systick
	GPIO_port_init();
	seven_init();
}
// end init

////////////////////////// SYSTEMTICK AND HANDLER /////////////////////
///
///
void SysTick_Handler (void)
{
	coreticks++; coretocks++;

	sevevsegmentloop();
}
//////////////////////// END SYSTEMTICK AND HANDLER ///////////////////



///////////////////// DELAY Systick Based Delays //////////////////////
///
///
uint32_t getticks(uint8_t which)
{
	if(which) return coretocks;
	else return coreticks;
}

void delay_Xus(uint32_t n_us)	//delay 1 systick
{
	uint32_t stop;
	if(coreticks<coretocks){
		stop=coreticks+n_us; while(coreticks<stop){}
	}else{
		stop=coretocks+n_us; while(coretocks<stop){}
	} 
}  /// valid!!
///
void delay_1ms(uint32_t n_ms)
{	
	delay_Xus(n_ms*DELAYMULT);
}

//////////////////////// DELAY LOOP ///////////////////////
void Delay3us(void) // delay model loop
{
	uint8_t counter =0;
	do {
	  if ( ++counter > 51 ) { return; }
	} while (1);
}
///////////////////  /////// END of DELAY ///////  ///////////////////





static uint32_t counter;
void seven_init(void)
{
    GPIOD->BSRRH = GPIO_Pin_12; //reset=on
    GPIOD->BSRRL = GPIO_Pin_13; //reset=on
    GPIOD->BSRRL = GPIO_Pin_14; //reset=on
    GPIOD->BSRRL = GPIO_Pin_15; //reset=on
    delay_1ms(100);
    GPIOE->ODR = 0xFFFF;
    loopstate=CHECKTIMER;
    counter=coreticks;
}

void LEDset(uint8_t val){
	if(val==1) GPIOD->BSRRH = GPIO_Pin_8; //reset=on
	else GPIOD->BSRRL = GPIO_Pin_8; //set=off
	if(val==2) GPIOD->BSRRH = GPIO_Pin_10; //reset=on
	else GPIOD->BSRRL = GPIO_Pin_10; //set=off
	if(val==3) GPIOD->BSRRH = GPIO_Pin_12; //reset=on
	else GPIOD->BSRRL = GPIO_Pin_12; //set=off
}













/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
///////////////////////////////// SYSTICK TASKS /////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////



static uint16_t Digit1, Digit2, Digit3;
static uint8_t checktimerstate;

void setDigit1(uint16_t val){	Digit1=val;	}
void setDigit2(uint16_t val){	Digit2=val;	}
void setDigit3(uint16_t val){	Digit3=val;	}





/////////////////////////////////////////////////////////////////////////////

//////////////////////////// seven segment scan task ////////////////////////

/////////////////////////////////////////////////////////////////////////////





static inline void sevevsegmentloop(void)
{
	switch(loopstate)
	{
	/// CHECK TICK
	case CHECKTIMER:
		if(checktimerstate){
			if(coreticks<0x80000000){
				loopstate = DIGIT_1TI;
			}
			else{
				loopstate = DIGIT_1TO;
				checktimerstate=0;
				counter -= 0x80000000;
			}
		}else{
			if(coretocks<0x80000000){
				loopstate = DIGIT_1TO;
			}
			else{
				loopstate = DIGIT_1TI;
				checktimerstate=1;
				counter -= 0x80000000;
			}
		}
		break;

	/// RUN WITH TICKS
	case DIGIT_1TI:
		if(coreticks>counter){
			GPIOD->BSRRH = GPIO_Pin_15; //reset digit3
			loopstate = DIGIT_1TI_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_1TI_W:
		GPIOE->ODR = (uint16_t) Digit1;
		GPIOD->BSRRL = GPIO_Pin_13; //set digit1
		loopstate = DIGIT_2TI;
		break;

	case DIGIT_2TI:
		if(coreticks>counter){
			GPIOD->BSRRH = GPIO_Pin_13; //reset digit1
			loopstate = DIGIT_2TI_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_2TI_W:
		GPIOE->ODR = (uint16_t) Digit2;
		GPIOD->BSRRL = GPIO_Pin_14; //set digit2
		loopstate = DIGIT_3TI;
		break;

		case DIGIT_3TI:
		if(coreticks>counter){
			GPIOD->BSRRH = GPIO_Pin_14; //reset digit2
			loopstate = DIGIT_3TI_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_3TI_W:
		GPIOE->ODR = (uint16_t) Digit3;
		GPIOD->BSRRL = GPIO_Pin_15; //set digit3
		loopstate = CHECKTIMER;
		break;
	
	/// RUN WITH TOCKS
	case DIGIT_1TO:
		if(coretocks>counter){
			GPIOD->BSRRH = GPIO_Pin_15; //reset digit3
			loopstate = DIGIT_1TO_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_1TO_W:
		GPIOE->ODR = (uint16_t) Digit1;
		GPIOD->BSRRL = GPIO_Pin_13; //set digit1
		loopstate = DIGIT_2TO;
		break;

	case DIGIT_2TO:
		if(coretocks>counter){
			GPIOD->BSRRH = GPIO_Pin_13; //reset digit1
			loopstate = DIGIT_2TO_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_2TO_W:
		GPIOE->ODR = (uint16_t) Digit2;
		GPIOD->BSRRL = GPIO_Pin_14; //set digit2
		loopstate = DIGIT_3TO;
		break;

	case DIGIT_3TO:
		if(coretocks>counter){
			GPIOD->BSRRH = GPIO_Pin_14; //reset digit2
			loopstate = DIGIT_3TO_W;
			counter += delaydigit;
		}else dosomething();
		break;

	case DIGIT_3TO_W:
		GPIOE->ODR = (uint16_t) Digit3;
		GPIOD->BSRRL = GPIO_Pin_15; //set digit3
		loopstate = CHECKTIMER;
		break;
	}
}







/////////////////////////////////////////////////////////////////////////////

//////////////////////////// push button task ///////////////////////////////

/////////////////////////////////////////////////////////////////////////////




uint8_t switch1state, switch2state;
static void dosomething(void)
{
	if((GPIOD->IDR & GPIO_Pin_9)&&(switch1state<50)) switch1state++; //stop switch
	else{		if(switch1state>0) switch1state--;	}
	if((GPIOD->IDR & GPIO_Pin_11)&&(switch2state<50)) switch2state++; //start switch
	else{		if(switch2state>0) switch2state--;	}
}
uint8_t getSwitchStopStatus(void){
	if(switch1state>12) return 1;
	else return 0;
}
uint8_t getSwitchStartStatus(void){
	if(switch2state>12) return 1;
	else return 0;
}

