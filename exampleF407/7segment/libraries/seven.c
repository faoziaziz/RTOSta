
#include "starter.h"
#include "seven.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"




////////////////////// SEVEN SEGMENT /////////////////////////
///
///
/////////////////////////////////////////////////////////////

static uint8_t Digit81, Digit82, Digit83, Digit91, Digit92, Digit93;
///
static void SegmentSet(void){
	setDigit1((Digit91 << 8) | Digit81);
	setDigit2((Digit92 << 8) | Digit82);
	setDigit3((Digit93 << 8) | Digit83);
}
void Segment1Set(uint16_t value, uint8_t dot)
{
	if(value>999) value=999;
	Digit81 = value/100;
	value -= Digit81*100;
	Digit82 = value/10;
	value -= Digit82*10;
	Digit83 = value;
	Digit81 = SevSgmnt[Digit81];
	Digit82 = SevSgmnt[Digit82];
    Digit83 = SevSgmnt[Digit83];
    if(dot==1) Digit82--;
	else if(dot==2) Digit81--;
	SegmentSet();
}
void Segment2Set(uint16_t value, uint8_t dot)
{
	if(value>999) value=999;
	Digit91 = value/100;
	value -= Digit91*100;
	Digit92 = value/10;
	value -= Digit92*10;
	Digit93 = value;
	Digit91 = SevSgmnt[Digit91];
	Digit92 = SevSgmnt[Digit92];
    Digit93 = SevSgmnt[Digit93];
    if(dot==1) Digit92--;
	else if(dot==2) Digit91--;
	SegmentSet();
}

void SegmentSetCut(uint16_t value){ // cuttoff Cut 999
    Digit81 = SevSgmnt[13];
	Digit82 = SevSgmnt[15];
	Digit83 = SevSgmnt[14];
    Segment2Set(value,1);
}
void SegmentSetSec(uint16_t value){ // timer 99.9 5Ec
    Digit91 = SevSgmnt[5];
	Digit92 = SevSgmnt[12];
	Digit93 = SevSgmnt[13];
    Segment1Set(value,1);
}
void SegmentSetOfs(uint16_t value){ // offset 0F5 99.9
    Digit81 = SevSgmnt[10];
	Digit82 = SevSgmnt[11];
	Digit83 = SevSgmnt[5];
    Segment2Set(value,1);
}
void SegmentSetSearch(void)
{
    uint16_t tmpj=0x0101;
	uint8_t tmpi=8;
	setDigit1(0xFFFF);
	delay_1ms(200);
	setDigit2(0xFFFF);
	delay_1ms(200);
	setDigit3(0);
	delay_1ms(200);
	setDigit1(tmpj);
	delay_1ms(200);
	setDigit1(0);
	delay_1ms(200);
	setDigit1(tmpj);
	delay_1ms(200);
	while(tmpi){
		setDigit1(tmpj<<=1);
		delay_1ms(1500);
		tmpi--;
	}

}


