
#ifndef PACKET_HANDLERS_H
#define PACKET_HANDLERS_H

//defining unit8_t
//typedef unsigned char uint8_t; 
//typedef unsigned int uint16_t; 

// defining start and end frame
#define SOF 0x7E
#define EOF 0x7F

// Packet strucutre 
struct xdata kissPacketUart{

	// include SOF? 
	uint8_t sof; //1 byte
	uint8_t command; // 1 byte
	uint8_t lenght;  // 1 byte 
	uint8_t payloadBuffer[50]; // 1 byte 
	uint16_t crc;  // 2 bytes UART and 4 bytes RF? 
	uint8_t eof; //1 byte 
	//include EOF
};

struct xdata kissPacketRF {
	
	// include SOF? 
	uint8_t sof; //1 byte
	uint8_t command; // 1 byte
	uint8_t lenght;  // 1 byte 
	uint8_t payloadBuffer[50]; // 1 byte 
	uint32_t crc;  // 4 bytes UART and 4 bytes RF? 
	uint8_t eof; //1 byte 
	//include EOF
	
	
};



void rfPacketHandler(void);
void uartPacketHandler(void);
//todo crcCheckUart, crcCheckRf 

#endif