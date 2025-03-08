/*
*
*
*
*
*/

// 1) Get RFTX Working
// 2) RFTX DMA 
// 3) Validate RF registers 
// --> May need to switch register values between states
// 4) Packet Handler 
// 5) SPI -- Refer to SWRA223 
// --> Include memory module
// 6) Validate what needs interrupts and what doesn't 


#include <Handlers/cc_packet_handlers.h>
#include <peripherals/UART/cc_uart_v1.h>
#include <RF/cc_rf_v1.h>

//static Commands uart_cmd; 			
//static uint8_t uart_length;
//static uint8_t uart_sof;
//static uint8_t uart_eof; 
//static Commands rf_cmd; 				
//static uint8_t rf_length;
//static uint8_t rf_sof;
//static uint8_t rf_eof; 


void rfPacketHandler(rf_packet *payload){
	
	// Variables 
	Commands rf_cmd;
	uint8_t length;
	uint8_t sof;
	uint8_t eof;
	uint8_t i;	
	uint8_t msg2[] = "handler\n";
	
	rf_cmd = payload->fields.command; // Get command
	length = payload->fields.length; 	// Get length
	sof = payload->fields.sof; 				// Get sof
	eof = payload->fields.eof;				// Get eof
	
	
	//uart0Send(msg2,8);
	
	if(sof != SOF || eof != EOF){
		return; // return error val? 
	}	
	
	//uart0Send(msg2,8);
	//uart0Send(&rf_cmd, 1);
	switch (rf_cmd){// switch to process payload based on command
		
		case ACK: {
			// switch state to IDEL - packet received - or continue based on Length
			//uint8_t msg[] = "Packet Received!\n";
			//rfSend(msg, sizeof(msg)-1);
			uint8_t msg = ACK; 
			uart0Send(&msg, 1);
			uart0Send(payload, sizeof(*payload));
//			for(i = 0; i < 10; i++){
//				
//				blink();
//			}
			break;
		}
		case DATA_STORE:{
			// Send data to xdata to be stored. For later use
			//uint8_t msg[] = "Data Store!\n";
			uint8_t msg = DATA_STORE;
			uart0Send(&msg, 1);
			
			break;
		}
		case DATA_SEND: {
			// Retrieve data from memory over SPI and DMA and send via RF. 
			//uint8_t msg[] = "Data Send!\n";
			uint8_t msg = DATA_SEND;
			uart0Send(&msg, 1);
			
			break;
		}
		
		// TODO: Complete all other cases
	}
	
}

void uartPacketHandler(uart_packet *payload){
	
	// Variables 
	Commands uart_cmd;
	uint8_t length;
	uint8_t sof;
	uint8_t eof; 
	uint8_t i;
	
	uart_cmd = payload->fields.command; // Get command
	length = payload->fields.length;		// Get length
	sof = payload->fields.sof; 					// Get sof	
	eof = payload->fields.eof;					// Get eof
	
	if(sof != SOF || eof != EOF){// return if bad structure
		
		return; 
	}
	
	switch(uart_cmd){ 	// switch to process payload based on command
		
		case ACK: {
			// Packet has been acknowledged that it is received
			uint8_t msg[] = ACK; 
			//uint8_t msg[] = "Acknowledge\n"; // Returned words could just be from ENUM (Smaller)
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			uart0Send(&msg,1);
			for(i = 0; i < 2; i++){
				
				blink();
			}

		}break;
		case DATA_STORE: {
			// Store data to be received into memory via DMA and SPI 
			uint8_t msg = DATA_STORE; 
			//uint8_t msg[] = "Data Stored\n";
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			uart0Send(&msg,1);	
			for( i = 0; i < 4; i++){
					
				blink();
			}
		}break;
		case DATA_SEND: {
			// Send data through uart (OBC communications) 
			uint8_t msg = DATA_SEND; 
			//uint8_t msg[] = "Data Sent\n";
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			uart0Send(&msg,1);
			for(i = 0; i < 6; i++){
				
				blink();
			}	
			for(i = 0; i < 10; i++){
				rfSend(rf_tx_buffer.rawPayload, 4); // For demo purposes
			}
		}break;
		// TODO: complete all other cases
	}
	
	
}

uint8_t *getrfCommand(){
	
	uint8_t command; 
	
	return command;
}

uint8_t *getUartCommand(){
	
	uint8_t command; 
	
	return command;
}	