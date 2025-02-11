
#ifndef CC_DMA_V1_H
#define CC_DMA_V1_H

// Headers 
#include <Common_Shared/blink.h>

// DMA channel configuration structure
typedef struct{
	  
		uint16_t srcAddr;   // Source Address (XDATA)
    uint16_t destAddr;  // Destination Address (XDATA)
    uint8_t vlen;       // Transfer Length Mode
    uint8_t len;        // Number of bytes to transfer
    uint8_t tmodeTrig;  // Transfer Mode + Trigger
    uint8_t ctrl;       // Address Increment, IRQ Mask, Priority
	
} DMA_CFG; 

// DMA channel configurations  (pointer in fast DATA memory, and structure in XDATA) 
extern xdata DMA_CFG dma_channels[5];  
extern const DMA_CFG dma_init_val;

// Initializer
void dmaInit(void); 

// Getters 

// Setters 
void setDmaChannel(void);

#endif