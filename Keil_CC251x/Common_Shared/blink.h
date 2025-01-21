

#ifndef BLINK_H
#define BLINK_H

// Define data types 
/* 
	If I am correct, Keil compiles int as 2 bytes. 
	This means that uint16 = int type, uint32 = long int,
	and uint64 = long long int. 
*/ 
typedef unsigned char uint8_t; 
typedef unsigned int uint16_t; 
typedef unsigned long int uint32_t; 
typedef unsigned long long int uint64_t; 



// Generalized blink and wait functions. 
void wait(void);
void blink(void);

#endif 