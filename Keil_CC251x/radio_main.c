
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
		uint8_t msg[] = "Start\n";
		
		// initialize system and modules 
		init(); 
		uartInit(); 
		rfInit();
		dmaInit();
		uart_rx_index = 0;
		
		
		delayMs(1);
		
		rf_tx_buffer.rawPayload[0] = 0x04; //0x3F;
		rf_tx_buffer.rawPayload[1] = SOF;
		rf_tx_buffer.rawPayload[2] = 0x01;
		rf_tx_buffer.rawPayload[3] = 0x00;
		//for(i = 3; i < 4; i++){
			
			//rf_tx_buffer.rawPayload[i] = 0x00;
		//}
		rf_tx_buffer.rawPayload[4] = EOF;
		//rf_tx_buffer.rawPayload[11] = EOF;
		
		uart0Send(msg,6); // for testing
		
		// -------- TEST: RX --------
		
//		RFST = SRX;
//		mode = SRX;
//		delayMs(1);

		// ------ TEST: TX -------

		RFST = SIDLE; 
		mode = SIDLE; 
		delayMs(1);
		
		RFST = SCAL;
		mode = SCAL;
		delayMs(1);
		
		RFST = SIDLE; 
		mode = SIDLE; 
		delayMs(1);
		
//		RFST = SFSTXON;
//		mode = SFSTXON;
//		delayMs(1);
		
		//uart_rx_packet_complete = 0;
		while(1){
		
			rfSend(rf_tx_buffer.rawPayload, 5);
			delayMs(1000);
			
			
			if(uart_rx_packet_complete){
				
				DMAIRQ &= ~DMAIF0;
				uart_rx_packet_complete = 0;
				uartPacketHandler(&uart_rx_buffer);
			}
			if(rf_rx_packet_complete){
				blink();
				DMAIRQ &= ~DMAIF1;
				rf_rx_packet_complete = 0;
				rfPacketHandler(&rf_rx_buffer);
			}

		}
		
		return 0; 
	}
		
		
