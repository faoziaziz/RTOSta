
#include "defines.h"
#include "starter.h"

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_pwr.h"
#include "misc.h"



//////////////////////////////// PORT CONFIG ////////////////////////////
///
///
void GPIO_port_init (void)
{
	GPIO_InitTypeDef	GPIO_InitS;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE , ENABLE );
	//onboard LED
	GPIO_InitS.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitS.GPIO_Speed	= GPIO_Speed_2MHz;
	GPIO_InitS.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitS.GPIO_PuPd		= GPIO_PuPd_NOPULL;
	GPIO_InitS.GPIO_Pin		= (PINLED);
	GPIO_Init (GPIOLED, &GPIO_InitS);
	//onboard pushbutton
	GPIO_InitS.GPIO_Mode		= GPIO_Mode_IN;
	GPIO_InitS.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitS.GPIO_Pin		= (PINPBUTTON);
	GPIO_Init (GPIOPBUTTON, &GPIO_InitS);
}
/////////////////////////////// END PORT CONFIG //////////////////////////



////////////////////////// SYSTEMTICK AND HANDLER /////////////////////
///
///
static uint32_t coreticks = 0;
static uint32_t coretocks = 0x80000000;
void SysTick_Handler (void)
{
	coreticks++; coretocks++;
}
//////////////////////// END SYSTEMTICK AND HANDLER ///////////////////



///////////////////////////// RTC init ////////////////////////////////
///
///
uint8_t RTClock_Init (void)
{
	uint8_t loop = 0;
	uint32_t attempt = 0;
	RTC_InitTypeDef   RTC_InitS;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); // Apply clock Vbatt pin
	PWR_BackupAccessCmd(ENABLE);    // enable access Vbatt ke RTC dll (bisa dicofig)
	RCC_LSEConfig(RCC_LSE_ON);      // Enable LSE OSC
	while(loop==RESET && attempt<33000000)
	{
		loop=RCC_GetFlagStatus(RCC_FLAG_LSERDY); // loop=1 jika ok
		attempt++;
	}
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE); // pakai LSE (crystal) untuk RTC
	if(loop==RESET)
	{	//jika LSE gagal
		RCC_LSEConfig(RCC_LSE_OFF);
		RCC_LSICmd(ENABLE);           // Enable LSI OSC (internal RC osc)
		while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET){}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);	// pakai LSI 
	}
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForSynchro();
	RTC_InitS.RTC_AsynchPrediv	= 0x7F;	//pembagi max = 128 - 1 (0x7F)
	RTC_InitS.RTC_SynchPrediv	= 0xFF;    // (32.768 / 128) - 1 = 255 (0xFF)
	RTC_InitS.RTC_HourFormat	= RTC_HourFormat_24;
	RTC_Init(&RTC_InitS);
	return loop;
}
///////////////////////////// END RTC init /////////////////////////////



////////////////////////////// INTERRUPT /////////////////////////////
///
///
void PostInit(void)
{
	EXTI_InitTypeDef EXTI_InitS;
	NVIC_InitTypeDef NVIC_InitS;      
   /// exti alarm (interrup dari alarm RTC bisa lewat EXTI 17)
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitS.EXTI_Line	= EXTI_Line17;
	EXTI_InitS.EXTI_Mode	= EXTI_Mode_Interrupt;
	EXTI_InitS.EXTI_Trigger	= EXTI_Trigger_Rising;
	EXTI_InitS.EXTI_LineCmd	= ENABLE;
	EXTI_Init( &EXTI_InitS );
	/// NVIC (misc.h) interrupt vectors
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //config group
	/// rtc alarm
	NVIC_InitS.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitS.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitS.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitS.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitS );     
	/// ALARM dari RTC
	RTC_AlarmTypeDef RTC_AlarmS;

	RTC_AlarmS.RTC_AlarmMask = RTC_AlarmMask_All;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmS);
	RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_8); // 1 detik (bisa 1 sd 1/256s)
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
	CLEAR_ALRA__o;	 //clear interrupt pending bit alarm A
}
//////////////////////////// END INTERRUPT ////////////////////////////

 


// fungsi utama di starter.c
uint8_t LSE;
void PeripheralInit(void){
	GPIO_port_init();
	while(SysTick_Config(SystemCoreClock/TICKINTERVAL)!=0){} //init & start systick
	while( PBUTTONSTATE__i ){ //loop sampai push button dipencet
		LED_TOGL__o ; //togle onboard led
		delay_1ms(1000);
	}delay_1ms(3000);
	LSE = RTClock_Init(); // 1 = LSE
	PostInit();
}
// end fungsi utama




///////////////////// DELAY Systick Based Delays //////////////////////
///
///
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
//////////////////////// TICK TIMER ///////////////////////
/*uint32_t getticks(void)
{
	uint32_t result; result=coretocks;
	if(coreticks<coretocks){ return coreticks; }
	else if(coretocks>0x7FFFFFFF) return 0;
	else{	return (result|0x80000000); }
	///	valid!!
}
///
uint32_t getlapse(uint8_t which)
{
	if(which) return coretocks;
	else{ return coreticks; }
}	/// valid!! */
///
//////////////////////// DELAY LOOP ///////////////////////
void Delay3us(void) // delay model loop
{
	uint8_t counter =0;
	do {
	  if ( ++counter > 51 ) { return; }
	} while (1);
}
///////////////////  /////// END of DELAY ///////  ///////////////////



///////////////////////////// DATE & TIME ////////////////////////////
///
///
/// timesub bertambah tiap detik ( bisa dipakai untuk hitung2an detik )
static uint32_t timesub = 0;
void RTC_Alarm_IRQHandler(void){
	CLEAR_ALRA__o; //clear interrupt pending bit alarm A
	timesub++;
	if(LSE) LED_TOGL__o ; //kedip jika pakai LSE
}
///
RTC_TimeTypeDef  RTC_TimeS;
RTC_DateTypeDef  RTC_DateS;
///
void RTC_Set(uint8_t yer, uint8_t mon, uint8_t dat, uint8_t hor, uint8_t min, uint8_t sec)
{
	RTC_TimeS.RTC_Hours		= hor;
	RTC_TimeS.RTC_Minutes	= min;
	RTC_TimeS.RTC_Seconds	= sec;  
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeS);
	RTC_DateS.RTC_Date		= dat;
	RTC_DateS.RTC_Month		= mon;
	RTC_DateS.RTC_Year		= yer;
	RTC_SetDate(RTC_Format_BCD, &RTC_DateS);
}
///
void RTC_collectTime (void) // dipakai untuk update nilai RTC_TimeS dan RTC_DateS
{
	RTC_GetTime(RTC_Format_BCD, &RTC_TimeS);
	RTC_GetDate(RTC_Format_BCD, &RTC_DateS);
}
///
void resetOVERLOADsecond(void)	{	while(timesub>5){timesub=0;}	}
uint32_t RTC_getsecond(void)		{	return timesub;				}
///
uint8_t RTC_getYer(void){	return RTC_DateS.RTC_Year;		}
uint8_t RTC_getMon(void){	return RTC_DateS.RTC_Month;	}
uint8_t RTC_getDay(void){	return RTC_DateS.RTC_Date;		}
///
uint8_t RTC_getHor(void){	return RTC_TimeS.RTC_Hours;	}
uint8_t RTC_getMin(void){	return RTC_TimeS.RTC_Minutes;	}
uint8_t RTC_getSec(void){	return RTC_TimeS.RTC_Seconds;	}
/////////////////////////// END DATE & TIME ///////////////////////////


