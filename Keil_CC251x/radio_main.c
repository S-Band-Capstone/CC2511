
// Headers 
#include <cc2510.h> 
#include <Common_Shared/blink.h>
#include <peripherals/UART/cc_uart_v1.h> 
#include <peripherals/DMA/cc_dma_v1.h> 
#include <Handlers/cc_packet_handlers.h>
#include <RF/cc_rf_v1.h>

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
//	while(i < 11){
//		blink(); 
//		i++; 
//	}
	
	
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
		uint8_t i; 
		
		// initialize system and modules 
		init(); 
		//uartInit(); 
		rfInit();
		uart_rx_index = 0;

		rf_tx_buffer.rawPayload[0] = 0x3F;
		rf_tx_buffer.rawPayload[1] = SOF;
		rf_tx_buffer.rawPayload[2] = 0x01;
		for(i = 3; i < 63; i++){
			
			rf_tx_buffer.rawPayload[i] = 0xD3;
		}
		rf_tx_buffer.rawPayload[63] = EOF;
		
		while(1){
			
			rfSend(rf_tx_buffer.rawPayload, 64);
			delayMs(10);
//			RFST = STX; 
//			mode = STX;
//			delayMs(1);
			
			// For Demo of UART
//			if(uart_rx_packet_complete){
//				uartPacketHandler(&uart_rx_buffer);
//				//uart0Send(uart_rx_buffer.rawPayload, 64);
//				uart_rx_packet_complete = 0;
//			}

		}
		
		return 0; 
	}
		
		
