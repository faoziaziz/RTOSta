#ifndef __DEFINES_H
#define __DEFINES_H


#include "stdlib.h"
#include "stdint.h"



//// konfig systick
#define	TICKINTERVAL	26000	//dipakai tick interrupt, interval: 1000=1ms, 10000=100us, 100000=10us
#define	DELAYMULT		26		//untuk delay 1ms, = TICKINTERVAL/1000


// state 0 = idle ; 1= run; 2= finishpass, 3= finishfail; 4= setcut 5= setsec 6= setofs
#define	S_IDLE	0
#define	S_RUN		1
#define	S_PASS	2
#define	S_FAIL	3
#define	S_CUT		4
#define	S_SEC		5
#define	S_OFS		6
#define	S_ERROR	255

#endif
