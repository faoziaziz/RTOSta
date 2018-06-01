
#include "starter.h"
#include "seven.h"
#include "defines.h"
#include "gbond.h"





uint16_t temp;
uint8_t state, subst;

void GB_Init(void){
	
state = S_IDLE;
subst=0;
}




void GB_Process(void){
	switch(state)
	{
	/// CHECK TICK
	case	S_IDLE:
		SegmentSetCut(123);
		delay_1ms(1500);
	break;
	
	case	S_RUN:
	break;

	case	S_PASS:
	break;

	case	S_FAIL:
	break;

	case	S_CUT:
	break;

	case	S_SEC:
	break;

	case	S_OFS:
	break;

	case	S_ERROR:
	break;

	
	SegmentSetCut(123);
	delay_1ms(1500);
	SegmentSetSec(123);
	delay_1ms(1500);
	SegmentSetOfs(123);
	delay_1ms(1500);
	temp=999;
	while(temp){
			Segment1Set(temp,0);
			Segment2Set(temp,0);
			delay_1ms(100);
			temp--;
	}
}




