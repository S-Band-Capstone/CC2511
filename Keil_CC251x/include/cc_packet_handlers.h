
#ifndef PACKET_HANDLERS_H
#define PACKET_HANDLERS_H

// Headers
#include <blink.h>

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
// NOTE: Probably can just merge them all as one packet for typedef.


typedef union {
	/* Packet strucutre for uart_packet */
	
	uint8_t rawPayload[64];  	
	struct{
		uint8_t length; // before SOF since apart of TI packet format
		uint8_t sof;
		uint8_t command;
		uint8_t payload[60];
		uint8_t eof; 
	}fields;
	
}packet;

typedef union {
	/* Packet strucutre for uart_packet */
	
	uint8_t rawPayload[64];  	
	struct{
		uint8_t length; // before SOF since apart of TI packet format
		uint8_t command;
		uint8_t payload[62];
	
	}fields;
	
}demoPacket;


/* Interrupts */

/* Functions for packet handlers */
void rfPacketHandler(packet *payload);			// RF packet handler
void uartPacketHandler(packet *payload);	// UART packet handler
void demoRfHandler(packet *payload);				// Demo RF handler
void demoUartHandler(packet *payload);			// Demo UART handler
/* Function Getters */
uint8_t* getrfCommand(void);
uint8_t* getUartCommand(void); 

/* Function Setters */



//todo crcCheckUart, crcCheckRf 

#endif