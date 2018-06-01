
#ifndef __STARTER_H
#define __STARTER_H


#include "defines.h"

typedef enum {
	CHECKTIMER =0,
	DIGIT_1TI,	DIGIT_1TI_W,
	DIGIT_2TI,	DIGIT_2TI_W,
	DIGIT_3TI,	DIGIT_3TI_W,
	DIGIT_1TO,	DIGIT_1TO_W,
	DIGIT_2TO,	DIGIT_2TO_W,
	DIGIT_3TO,	DIGIT_3TO_W,
} LOOPSTT;


/// goundbond

void LEDset(uint8_t val);
uint8_t getSwitchStopStatus(void);
uint8_t getSwitchStartStatus(void);


/// systick delay, (jangan di pakai di dalam interrupt)
void delay_1ms(uint32_t n_ms);
void delay_Xus(uint32_t n_us);
uint32_t getticks(uint8_t which);


/// loop delay, untuk delay dalam interrup
void Delay3us(void);	//3us
///

/// INIT
void PeripheralInit(void);

/// systick task
void setDigit1(uint16_t val);
void setDigit2(uint16_t val);
void setDigit3(uint16_t val);

#endif
