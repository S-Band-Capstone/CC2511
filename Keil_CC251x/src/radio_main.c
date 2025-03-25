
// Headers 
#include <cc2510fx.h> 
#include <../include/blink.h>
#include <../include/cc_uart_v1.h> 
#include <../include/cc_dma_v1.h> 
#include <../include/cc_packet_handlers.h>
#include <../include/cc_rf_v1.h>

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
	__xdata uint8_t start_msg[7] = "Start\n";
	__xdata uint8_t end_msg[7] = "End\n";
	
	// initialize system and modules 
	init(); 
	uartInit(); 
	rfInit();
	dmaInit();
	
	
	
//		delayMs(1);
	
	//rf_tx_buffer.rawPayload[0] = 0x04;
	//rf_tx_buffer.rawPayload[1] = SOF;
	//rf_tx_buffer.rawPayload[2] = 0x01;
	//rf_tx_buffer.rawPayload[3] = 0xD9;
	//rf_tx_buffer.rawPayload[4] = EOF;
	//  rf_tx_buffer.rawPayload[0] = 0x3F;
	//  rf_tx_buffer.rawPayload[1] = SOF;
	//  rf_tx_buffer.rawPayload[2] = 0x01;
	//  for(i =3 ; i < 63; i++){
	//  	rf_tx_buffer.rawPayload[i] = 0xD3;
	//  }
	//  rf_tx_buffer.rawPayload[63] = EOF;
	
	uart0SendUnstructured(start_msg,6); // for testing

	RFST = SCAL;
	delayMs(1);
	// RFST = STX;
	// i = 100;
	// while(i--){
		
		
	// 	//setRfState(STX);
	// 	//delayMs(1);
	// 	rfSend(rf_tx_buffer.rawPayload, 63);

	// }
	// uart0Send(end_msg,4); // for testing
	RFST = SRX;
	i = 0;

	//dmaAbort(0);
	while(1){
		
		//setRfState(SRX);
		RFST = SRX;
		
		if(uart_rx_packet_complete){
		
			DMAIRQ &= ~DMAIF0;
			uart_rx_packet_complete = 0;
			uartPacketHandler(&uart_rx_buffer); // Handle packet UART
			//demoUartHandler(&uart_rx_buffer);
		}
		if(rf_rx_packet_complete){
			
			DMAIRQ &= ~DMAIF1;
			rf_rx_packet_complete = 0;	
			rfPacketHandler(&rf_rx_buffer); // Handle Packet RX
			//demoRfHandler(&rf_rx_buffer);
			
		}

	}
	
	return 0; 
}
		
		
