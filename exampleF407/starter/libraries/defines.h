#ifndef __DEFINES_H
#define __DEFINES_H






//// konfig port
#define PINLED          GPIO_Pin_0	//led nyala ketika low
#define GPIOLED			GPIOE
#define PINPBUTTON      GPIO_Pin_15	//push button low pas dipencet
#define GPIOPBUTTON     GPIOD
//// konfig systick
#define	TICKINTERVAL	200000	//dipakai tick interrupt, interval: 1000=1ms, 10000=100us, 100000=10us
#define	DELAYMULT		200		//untuk delay 1ms, = TICKINTERVAL/1000
//// oprasi io port (port state)
#define PBUTTONSTATE__i (GPIOPBUTTON->IDR & PINPBUTTON) //read pushbutton pin reg
#define LED_ON__o			GPIOLED->BSRRH = PINLED		//resetbit led
#define LED_OFF__o		GPIOLED->BSRRL = PINLED		//setbit led
#define LED_TOGL__o		GPIOLED->ODR ^= PINLED		//toggle bit led

#define ITCLR_EXTI17__o	EXTI->PR = EXTI_Line17; //EXTI_ClearITPendingBit(EXTI_Line17);
#define ITCLR_ALRA__o	RTC->ISR = (~0x00000180|(RTC->ISR & RTC_ISR_INIT)) //RTC_ClearITPendingBit(RTC_IT_ALRA);
#define CLEAR_ALRA__o	ITCLR_EXTI17__o ITCLR_ALRA__o


#endif