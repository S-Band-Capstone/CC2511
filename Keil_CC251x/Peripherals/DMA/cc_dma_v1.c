
// Headers 
#include <peripherals/DMA/cc_dma_v1.h>

// DMA channel configurations variables 
xdata DMA_CFG dma_channels[5]; 
const DMA_CFG dma_init_val = {0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00};

// Initializer 
void dmaInit(void){
	
	uint8_t i; 
	
	// Allocate memory in xdata for DMA channels and set struct to 0x00
	for(i = 0; i < 5; i++){ // init channels 
		
		dma_channels[i] = dma_init_val; 
	}
	
	// Set addresses for DMA configuration registers
	DMA0CFGL = (uint8_t)((uint16_t)&dma_channels[0] & 0x00FF);  // Low byte of address for channel 1
  DMA0CFGH = (uint8_t)(((uint16_t)&dma_channels[0]) >> 8);  	// High byte of address for channel 1
	DMA0CFGL = (uint8_t)((uint16_t)&dma_channels[1] & 0x00FF);  // Low byte of address for channel 2-5
  DMA0CFGH = (uint8_t)(((uint16_t)&dma_channels[1]) >> 8);  	// High byte of address for channel 2-5
	
	// Setup DMA for UART 
	/* Have data transfers and stores happen over DMA to allow CPU to cycle seperately */
	
	// Setup DMA for SPI 
	/* Passing and storing from SPI over DMA, shouldn't require CPU  to process data */
	
	// Setup DMA for RF
	/* Have stored to set memory address so that if CPU interrupts DMA is passing seperately */ 
	
}