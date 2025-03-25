/*
*
*
*
*
*/

#include <../include/cc_packet_handlers.h>
#include <../include/cc_dma_v1.h>
#include <../include/cc_uart_v1.h>
#include <../include/cc_rf_v1.h>


void rfPacketHandler(packet *payload){
	
	// Variables 
	Commands rf_cmd;
	uint8_t length;
	uint8_t sof;
	uint8_t eof;
	//uint8_t i;	
	uint8_t msg2[] = "handler\n";
	
	rf_cmd = payload->fields.command; // Get command
	length = payload->fields.length; 	// Get length
	sof = payload->fields.sof; 				// Get sof
	eof = payload->fields.eof;				// Get eof
	
	
	//uart0SendUnstructured(msg2,8);
	
	if(sof != SOF || eof != EOF){
		return; // return error val? 
	}	
	
	switch (rf_cmd){// switch to process payload based on command
		
		case ACK: {
			// switch state to IDEL - packet received - or continue based on Length
			//uint8_t msg[] = "Packet Received!\n";
			//rfSend(msg, sizeof(msg)-1);
			uint8_t msg = ACK;
			//dmaAbort(3); // sending single packet
			//uart0SendCmd(&msg, 0x01);
			rfSend(payload->rawPayload, length);
			//setDmaArm(3);

		}break;
		case DATA_STORE:{
			// Send data to xdata to be stored. For later use
			//uint8_t msg[] = "Data Store!\n";
			uint8_t msg = DATA_STORE;
			//uart0Send(&msg, 0x01);
			dmaAbort(3);
			RFD = msg;
			setDmaArm(3);

		}break;
		case DATA_SEND: {
			// Retrieve data from memory over SPI and DMA and send via RF. 
			//uint8_t msg[] = "Data Send!\n";
			uint8_t msg = DATA_SEND;
			//uart0Send(&msg, 0x01);
			setRfState(STX);
			dmaAbort(3);
			RFD = msg;
			setDmaArm(3);
			
			
		}break;
		case MSG: {
			// Send message to OBC via RF
			//uint8_t msg[] = "Message Sent!\n";
			uart0Send(payload->rawPayload, length);// take out SOF, EOF, and CMD
			
		}break;
		
		// TODO: Complete all other cases
	}
	
}

void uartPacketHandler(packet *payload){
	
	// Variables 
	Commands uart_cmd;
	uint8_t length;
	uint8_t sof;
	uint8_t eof; 
	//uint8_t i;
	uint8_t msg_error[] = "Error\n";
	

	uart_cmd = payload->fields.command; // Get command
	length = payload->fields.length;		// Get length
	sof = payload->fields.sof; 					// Get sof	
	eof = payload->fields.eof;					// Get eof
	
	if(sof != SOF || payload->rawPayload[length] != EOF){// return if bad structure
		uart0SendUnstructured(msg_error, 6);
		return; 
	}
	
	switch(uart_cmd){ 	// switch to process payload based on command
		
		case ACK: {
			// Packet has been acknowledged that it is received
			uint8_t msg = ACK; 
			//uint8_t msg[] = "Acknowledge\n"; // Returned words could just be from ENUM (Smaller)
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			//uart0SendCmd(&msg,0x01);
			dmaAbort(3);
			U0DBUF = msg; // send to UART
			uart_rx_packet_complete = 0;
			setDmaArm(3);

		}break;
		case DATA_STORE: {
			// Store data to be received into memory via DMA and SPI 
			uint8_t msg = DATA_STORE; 
			//uint8_t msg[] = "Data Stored\n";
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			//uart0Send(&msg,1);	
			dmaAbort(3);
			U0DBUF = msg;
			uart_rx_packet_complete = 0;
			setDmaArm(3);

		}break;
		case DATA_SEND: {
			// Send data through uart (OBC communications) 
			uint8_t msg = DATA_SEND; 
			//uint8_t msg[] = "Data Sent\n";
			//uart0Send(msg, sizeof(msg)-1); // for sending string
			uart0Send(&msg,0x01);
			dmaAbort(3);
			U0DBUF = msg;
			uart_rx_packet_complete = 0;
			setDmaArm(3);
		}break;
		case MSG: {

			//setRfState(STX);
			RFST = STX;	
			rfSend(payload->rawPayload, length);
			delayMs(1);
			RFST = SRX;
			//setRfState(SRX);
		}break;
		// TODO: complete all other cases
	}
	
	//uart_rx_packet_complete = 0;
}



void demoRfHandler(packet *payload){


	// Variables 
	Commands rf_cmd;
	uint8_t length;
	uint8_t i;
	uint8_t rfH[] = "RF HANDLER\n";

	uart0Send(rfH, 11);

	rf_cmd = payload->fields.command; // Get command
	length = payload->fields.length; 	// Get length

	uart0Send(payload->rawPayload, length);
	setRfState(SRX);
	rf_rx_packet_complete = 0;

	// switch(rf_cmd){
	// 	case ACK: {
	// 		// Acknowledge packet received
	// 		uint8_t msg[] = "Acknowledge\n"; 
	// 		uart0Send(msg, 0x0C);

	// 		setRfState(STX);
	// 		//setRfTxBuffer(msg, sizeof(msg));
	// 		rfSend(payload->rawPayload, length);
	// 		setRfState(SIDLE);
	// 		break;
	// 	}
	// 	case DATA_STORE: {
	// 		// Store data received
	// 		uint8_t msg[] = "Data Stored\n";
			
	// 		for (i = 0; i < length; i++){
	// 			temp_buffer[i] = payload->rawPayload[i];
	// 		}
	// 		uart0Send(msg, 0x0C);
	// 		uart0Send(payload->rawPayload, length);
			
	// 		setRfState(STX);
	// 		//setRfTxBuffer(msg, sizeof(msg));
	// 		rfSend(payload->rawPayload, length);



	// 		break;
	// 	}
	// 	case DATA_SEND: {
	// 		// Send data
	// 		uint8_t msg[] = "Data Sent\n";

	// 		setRfState(STX);
	// 		//setRfTxBuffer(msg, sizeof(msg));
	// 		rfSend(payload->rawPayload, length);	
	// 		uart0Send(msg, 0x0A);

	// 		break;
	// 	}
	// 	defualt: {
	// 		// default case
	// 		uart0send(payload->rawPayload, length);
	// 		setRfState(SRX);
	// 		break;
	// 	} 
	// }
}


void demoUartHandler(packet *payload){

	// Variables 
	Commands uart_cmd;
	uint8_t length;
	uint8_t i;
	uint8_t uartH[] = "UART HANDLER\n";

	//uart0Send(uartH, 13);
	
	uart_cmd = payload->fields.command; // Get command
	length = payload->fields.length; 	// Get length

	setRfState(STX); 
	rfSend(payload->rawPayload, length);

	uart_rx_packet_complete = 0;
	setRfState(SRX);
	// switch(uart_cmd){
	// 	case ACK: {
	// 		// Acknowledge packet received
	// 		break;
	// 	}
	// 	case DATA_STORE: {
	// 		// Store data received
	// 		uint8_t msg[] = "Data Stored\n";
			
	// 		for (i = 0; i < length; i++){
	// 			temp_buffer[i] = payload->rawPayload[i];
	// 		}
	// 		uart0Send(msg, 0x0C);

	// 		break;
	// 	}
	// 	case DATA_SEND: {
	// 		// Send data

	// 		break;
	// 	}
	// 	defualt: {
	// 		// default case
	// 		setRfState(STX);
	// 		rfSend(payload->rawPayload, length);

	// 		break;
	// 	} 
	// }
}
