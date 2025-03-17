/*

		DMA works over XDATA so make sure the source and destination are in XDATA 


*/


#ifndef CC_DMA_V1_H
#define CC_DMA_V1_H

// Headers 
#include <blink.h>

// Defines 
#define DMAIF0		0x01
#define DMAIF1 		0x02 
#define DMAIF2		0x04
#define DMAIF3		0x08
#define DMAIF4		0x10

// DMA channel configuration structure
typedef struct{
	  
	uint8_t srcAddrHi;   	// Source Address High Byte(XDATA)
	uint8_t srcAddrLo;   	// Source Address Low Byte(XDATA)
	uint8_t dstAddrHi;  	// Destination Address High Byte (XDATA)
	uint8_t dstAddrLo;  	// Destination Address Low Byte (XDATA)
	uint8_t byte4;       	// VLen [7:5] (3 bits) +  Len upper5 [4:0] (5 bits)
	uint8_t byte5;       	// Len low8 [7:0] (8bits)
	uint8_t byte6;  		 	// Word [7] (1 bit) + TMODE [6:5] (2 bits) + Trig [4:0] (5 bits)
	uint8_t byte7;       		// SrcInc [7:6] (2 bits) + DstInc [5:4] (2 bits) + IRQMask [3] (1 bit) + M8 [2] (1 bit) + Prio [1:0] (2 bits)
} dma_cfg; 

// DMA channel configurations  
extern __xdata dma_cfg dma_channels[5];  
extern const dma_cfg dma_init_val;

// Interrupts 
void dmaIsr(void) __interrupt(DMA_VECTOR);

// Initializer
void dmaInit(void); 

// Functions
volatile void dmaAbort(uint8_t channel);
void dmaRequest(uint8_t channel);
	
// Getters 

// Setters 
volatile void setDmaArm(uint8_t channel);
void setDmaDisarmrm(uint8_t channel);


#endif