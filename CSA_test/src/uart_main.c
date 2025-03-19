
// Headers 
#include <cc2510fx.h> 
#include <../include/blink.h>
#include <../include/cc_uart_v1.h> 

// File path 'includes' are relative to where project is found. 
// Not relative to the file that is using the 'include'.


/*-- Base Initialization --*/
void init(void){ 
/*      
This is a base function to initialize the base system. 
Think of it as a warm up before the system does anything,
to make sure that it is in the right mode initially. 
*/
		
	// Clock set to 24 MHz with HS XOSC
	uint8_t i = 0;
	CLKCON &= 0x80;  // The OSC32K should be set to '1' if 
									 // using HS XOSC (Refer to datasheet) 
	
	// Wait for HS XOSC to be stable
	while(SLEEP & 0x40){
		wait(); 
	}
	
	// Turn off HS RCOSC
	// Calibration Done
	SLEEP |= 0x04; 
	
	// Enable all interrupts 
	EA = 1;
	
	
	// Blink
	while(i < 11){
		blink(); 
		i++; 
	}
	
	
}

// MAIN SECTION //
/*

Main function by itself without a loop will loop indefinitely 
without a loop. For proper fuctionality, incorporate loop in 
the main function for things to run correctly. 

*/
int main(void){
		
	// temp variable
	uint8_t i; 
	__xdata uint8_t msg[7] = "Start\n";
    __xdata uint8_t beep[5] = "beep\n";
    __xdata uint8_t boop[5] = "boop\n";

	
	// initialize system and modules 
	init(); 
	uartInit();
    dmaInit();
    delayMs(1000);
    // set debug GPIO pins to output mode
    P2DIR |= ( 1 << 1 ) | (1 << 2);
	
	uart0Send(msg,6); // for testing
	
	while(1){
		

        P2 ^= ( 1 << 1 ) ;                // Toggle Debug Data
        P2 ^= ( 1 << 2 ) ;                // Toggle Debug Clock
        uart0Send(beep,5);
        delayMs(1000);

        P2 ^= ( 1 << 1 ) ;                // Toggle Debug Data
        P2 ^= ( 1 << 2 ) ;                // Toggle Debug Clock
        uart0Send(boop,5);
        delayMs(1000);
	}
	
	return 0; 
}
		
		
