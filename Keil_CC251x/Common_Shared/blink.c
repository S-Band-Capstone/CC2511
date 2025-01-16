
// Generic file for blink and wait.
// More for testing than anything. 

#include <cc2510.h> 
#include <Common_Shared/blink.h>

 void wait(void ){
/* Generic wait/nop function. */
    ;
}

void blink(void){
/*
Blink LED on board: 
More for visual and debugging purposes. 
	
Value of 'i' should ideally be equal to or smaller than 30k
*/
	int i = 0;                     /* Delay var */
	P1DIR |= ( 1 << 1 );

	P1 ^= ( 1 << 1 ) ;                /* Toggle LED Pin */
	for (i = 0; i < 25000; i++)  {    /* Delay for 50000 Counts */
		wait();                         /* call wait function */
		wait();
		wait();
	}
		
}