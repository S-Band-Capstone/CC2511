/*
*
*
*
*
*/

#include <Handlers/cc_packet_handlers.h>
#include <peripherals/UART/cc_uart_v1.h>


void rfPacketHandler(rf_packet *payload){
	
	// declare variables at start. 
	Commands rf_cmd = payload->fields.command; // get command from payload
	
	if(payload->fields.sof != SOF || payload->fields.eof != EOF){
		return; // return error val? 
	}

	// CRC check to see for errors. 
	
	
	switch (rf_cmd){// switch to process payload based on command
		
		case ACK: {
			// switch state to IDEL - packet received - or continue based on Length
			uint8_t msg[] = "Packet Received!\n";
			uart0Send(msg, sizeof(msg)-1);
			break;
		}
		case DATA_STORE:{
			// Send data to xdata to be stored. For later use
			break;
		}
		case DATA_SEND: {
			// Retrieve data from memory over SPI and DMA and send via RF. 
			break;
		}
		
		// TODO: Complete all other cases
	}


	
}

void uartPacketHandler(uart_packet *payload){
//void uartPacketHandler(void *payload, bit type){	
	
	// Variables 
	Commands uart_cmd;
	uint8_t length;
	uint16_t crc;
	uint8_t sof;
	uint8_t eof; 
	//uint8_t recData[58];
	//blink();
	
//	if(type){
//		
//		uart_cmd = ((uart_packet *)payload)->fields.command;
//		length = ((uart_packet *)payload)->fields.length;
//		sof = ((uart_packet *)payload)->fields.sof;
//		eof = ((uart_packet *)payload)->fields.eof;
//		//recData = ((uart_packet *)payload)->fields.payload;
//		
//	}else{
//		
//		uart_cmd = ((uart_dma_packet *)payload)->fields.command;
//		length = ((uart_dma_packet *)payload)->fields.length;
//		sof = ((uart_dma_packet *)payload)->fields.sof;
//		eof = ((uart_dma_packet *)payload)->fields.eof;
//		//recData = (union uart_dma_packet *)payload->fields.payload;
//	}
	
	uart_cmd = payload->fields.command; // get command from 
	length = payload->fields.length;
	crc = payload->fields.crc;
	sof = payload->fields.sof; 
	eof = payload->fields.eof;
	
	if(sof != SOF || eof != EOF){// return if bad structure
		
		return; 
	}
//	if(sof != SOF || eof != EOF){
//		return;
//	}
	
	switch(uart_cmd){ 	// switch to process payload based on command
		
		case ACK: {
			// Packet has been acknowledged that it is received
				uint8_t msg[] = ACK; 
				//uint8_t msg[] = "Acknowledge\n"; // Returned words could just be from ENUM (Smaller)
				//uart0Send(msg, sizeof(msg)-1); // for sending string
				uart0Send(&msg,1);
				blink();
			}break;
		case DATA_STORE: {
			// Store data to be received into memory via DMA and SPI 
				uint8_t msg = DATA_STORE; 
				//uint8_t msg[] = "Data Stored\n";
				//uart0Send(msg, sizeof(msg)-1); // for sending string
				uart0Send(&msg,1);	
				blink();
				blink(); 
			}break;
		case DATA_SEND: {
			// Send data through uart (OBC communications) 
				uint8_t msg = DATA_SEND; 
				//uint8_t msg[] = "Data Sent\n";
				//uart0Send(msg, sizeof(msg)-1); // for sending string
				uart0Send(&msg,1);
				blink();
				blink();
				blink();

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