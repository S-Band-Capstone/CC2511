// Headers
#include <Common_Shared/blink.h>

 void wait(void ){
/* Generic wait/nop function. */
    ;
}

void blink(void){
/*
* Blink LED on board: 
* More for visual and debugging purposes. 
*	
* Value of 'i' should ideally be equal to or smaller than 30k
*/
	
	int i = 0;           // Delay variable
	P1DIR |= ( 1 << 1 );

	P1 ^= ( 1 << 1 ) ;                //Toggle LED Pin 
	for (i = 0; i < 30000; i++)  {    // Delay for 25000 Counts 
		wait();                         // call wait function 
		wait();
		wait();
	}
		
}

// Buffer Clearing 
void bufferClear(uint16_t* buffer, uint16_t bufferLen){
/*
* Buffer clearing, to clear buffers out after use.
* Buffers should ideally not be used to store data, 
* but used as middle man for transfering to the appropriate
* address store locations. 	
*/
	
	int i;

	// NOTE: potential optimization - only clear where needed
	for (i = 0; i < bufferLen; i++){
		
		buffer[i] = '\0';
	}
}