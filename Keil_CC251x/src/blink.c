// Headers
#include <../include/blink.h>

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
	for (i = 0; i < 25000; i++)  {    // Delay for 25000 Counts 
		wait();                         // call wait function 
		wait();
		wait();
	}

}

void delayMs(uint16_t ms) {
	
	/*
		Delay function in ms
		
		nano second (ns) = 10^-9 
		milla second (ms) = 10^-3
		clock frequency = 24Mhz => 1/24Mhz = 1 clock cycle	
		1 clock cycle = 41.67 ns 
		1 ms = 24000 clock cycles 
	
	*/
	
	uint32_t i = 4000 * ms; // assumuing a 1 machine cycle ~6 clock cycle
	while(i--){
		__asm__("nop");
	}
}

// Timer Register Function
void timerDelayMs(uint16_t ms){
	// Still needs to be worked on. 
	// Configure Timer 1
	T1CTL = 0x00;          // Stop timer
	T1CNTL = 0x00;         // Clear counter

	// Set prescaler to 128 (24MHz/128 = 187.5kHz)
	// Each tick = 5.33μs
	T1CTL = 0x0C;          // Prescaler 128

	// Calculate ticks needed for ms delay
	// 187.5 ticks per ms
	uint16_t ticks = ms * 187;

	// Start timer
	T1CTL |= 0x02;         // Start timer in modulo mode

	// Wait until timer reaches desired count
	while(T1CNTL < ticks); 

	// Stop timer
	T1CTL = 0x00;

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