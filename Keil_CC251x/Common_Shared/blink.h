/*
*	Generic common file that is used to reduce redefines of shared functions.
*	This file is a lot more for visual validation and testing, but can 
* will be used for other common funtions that are not only for testing. 
*
*/

#ifndef BLINK_H
#define BLINK_H

// Headers 
#include <cc2510.h>
/* 
	If I am correct, Keil compiles int as 2 bytes. 
	This means that uint16 = int type, uint32 = long int,
	and uint64 = long long int. 

	Based off datasheet and how keil maps data types. 
	
	CODE Data: 16bit ==> uint16_t
	DATA: 8bit ==> uint8_t
	XDATA: 16bit ==> uint16_t
*/  
	

typedef unsigned char uint8_t; 
typedef unsigned int uint16_t; 
typedef unsigned long int uint32_t; 

// Generalized blink and wait functions. 
void wait(void);
void blink(void);
void delayMs(uint8_t ms);

// Buffer Clear 
void bufferClear(uint16_t* buffer, uint16_t bufferLen); 


#endif 