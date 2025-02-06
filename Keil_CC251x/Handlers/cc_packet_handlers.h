
#ifndef PACKET_HANDLERS_H
#define PACKET_HANDLERS_H

// Headers
#include <Common_Shared/blink.h>

// Defined variables
#define SOF 0x7E	// Start of frame
#define EOF 0x7F	// End of frame 




/* Defined commands */
typedef enum{
	/* Can add more commands to 0x0*, 0x1*, and 0x2* to cover more cases*/
	ACK = 0x01, 											// Acknowledge
	REQUEST_TELEMETRY = 0x02, 				// Request Telemetry
	DATA_STORE = 0x10, 								// Store Data
	DATA_SEND = 0x11, 								// Send Data
	CFG_BASE_FREQUENCY = 0x20, 				// Configure Base Frequency
	CFG_CHANNEL_NUMBER = 0x21, 				// Configure Channel Number
	CFG_CHANNEL_BANDWIDTH = 0x22, 		// Configure Channel Bandwidth
	CFG_DATA_RATE = 0x23,  						// Configure Data Rate
	CFG_RX_FILTER_BW = 0x24,  				// Configure RX Filter Bandwidth
	CFG_MODULATION_SCHEME = 0x25, 		// Configure Modulation Scheme
	CFG_DEVIATION_FSK = 0x26, 				// Configure Deviation (FSK)
	CFG_PHASE_TRANSITION_GMSK = 0x27, // Configure Phase Transition Time (GMSK)
	CFG_TX_POWER = 0x28, 							// Configure TX Power
	CFG_DATA_WHITENING = 0x29, 				// Enable Data Whitening
	CFG_MANCHESTER_ENC = 0x2A 				// Enable Manchester Encoding
	
}Commands;

/* Packet structures uart and RF */

typedef union{
	/* Packet strucutre for uart_packet */
	
	uint8_t rawPayload[64];			// Must be 16 bits for pointer addresses. 
	struct{
		uint8_t sof;
		uint8_t command;
		uint8_t length; 
		uint8_t payload[58];
		uint16_t crc;
		uint8_t eof; 
	}fields;
	
}uart_packet;


typedef union {
	/* Packet strucutre for uart_packet */
	
	uint16_t rawpayload[64];  		// Must be 16 bit for pointer addresses 
	struct{
		uint8_t sof;
		uint8_t command;
		uint8_t length; 
		uint8_t payload[56];
		uint32_t crc; 
		uint8_t eof; 
	}fields;
	
}rf_packet;

/* Interrupts */

/* Functions for packet handlers */
void rfPacketHandler(rf_packet *payload);			// RF packet handler
void uartPacketHandler(uart_packet *payload);	// UART packet handler

/* Function Getters */
uint8_t* getrfCommand(void);
uint8_t* getUartCommand(void); 

/* Function Setters */

/* CRC Checks  */
uint32_t rfCrc32(uint16_t* packet );


//todo crcCheckUart, crcCheckRf 

#endif