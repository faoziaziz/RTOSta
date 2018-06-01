
#ifndef __STARTER_H
#define __STARTER_H


#include "stdlib.h"
#include "stdint.h"






/// systick delay, (jangan di pakai di dalam interrupt)
void delay_1ms(uint32_t n_ms);
void delay_Xus(uint32_t n_us);
//uint32_t getticks(void);
//uint32_t getlapse(uint8_t which);

/// loop delay, untuk delay dalam interrup
void Delay3us(void);	//3us
void PeripheralInit(void);
/// RTC
void RTC_Set(uint8_t yer, uint8_t mon, uint8_t dat, uint8_t hor, uint8_t min, uint8_t sec);
void RTC_collectTime (void);
uint32_t RTC_getsecond(void);
void resetOVERLOADsecond(void);

uint8_t RTC_getYer(void);
uint8_t RTC_getMon(void);
uint8_t RTC_getDay(void);
uint8_t RTC_getHor(void);
uint8_t RTC_getMin(void);
uint8_t RTC_getSec(void);

#endif
