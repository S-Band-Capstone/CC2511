
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
extern DMA_CFG xdata *dma_ch0; 	// Channel 0 for DMA
extern DMA_CFG xdata *dma_ch1;	// Channel 1 for DMA
extern DMA_CFG xdata *dma_ch2;	// Channel 2 for DMA
extern DMA_CFG xdata *dma_ch3;	// Channel 3 for DMA
extern DMA_CFG xdata *dma_ch4;	// Channel 4 for DMA

// Initializer
void dmaInit(void); 

// Getters 

// Setters 
void setDmaChannel(void);

#endif