
#ifndef PACKET_HANDLERS_H
#define PACKET_HANDLERS_H

//defining unit8_t
typedef unsigned char uint8_t; 
typedef unsigned int uint16_t; 

// defining start and end frame
#define SOF 0x7E
#define EOF 0x7F

// Packet strucutre 
typedef xdata struct{

	// include SOF? //1 byte
	uint8_t command; // 1 byte
	uint8_t lenght;  // 1 byte 
	uint8_t payloadBuffer[50]; // 1 byte 
	uint16_t crc;  // 2 bytes UART and 16 bytes RF? 
	// include EOF? 
}packet;


void rfPacketHandler(void);
void uartPacketHandler(void);
//todo crcCheckUart, crcCheckRf 

#endif