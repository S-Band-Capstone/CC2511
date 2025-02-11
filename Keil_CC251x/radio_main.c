
// Headers 
#include <cc2510.h> 
#include <Common_Shared/blink.h>
#include <peripherals/UART/cc_uart_v1.h> 
#include <Handlers/cc_packet_handlers.h>

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
	int i = 0;
	CLKCON &= 0x80;  // The OSC32K should be set to '1' if 
									 // using HS XOSC (Refer to datasheet) 
	
	// Wait for HS XOSC to be stable
	while(SLEEP & 0x40){
		wait(); 
	}
	
	// Turn off HS RCOSC
	// Calibration Done
	SLEEP |= 0x04; 
	
	// Blink
	while(i < 10){
		blink(); 
		i++; 
	}
	
	
}


// UART SECTION // 


// MAIN SECTION //
/*

Main function by itself without a loop will loop indefinitely 
without a loop. For proper fuctionality, incorporate loop in 
the main function for things to run correctly. 

*/
	int main(void){
		
		// temp variable
		
		
		// initialize system and modules 
		init(); 
		uartInit(); 
		uart_rx_index = 0;
		
		
		while(1){
			
		
			if(uart_rx_packet_complete){
				uartPacketHandler(&uart_rx_buffer);
				//uart0Send(uart_rx_buffer.rawPayload, 64);
				uart_rx_packet_complete = 0;
			}

		}
		
		return 0; 
	}
		
		
