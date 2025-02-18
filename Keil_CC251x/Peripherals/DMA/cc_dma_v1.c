
// Headers 
#include <peripherals/DMA/cc_dma_v1.h>
#include <peripherals/UART/cc_uart_v1.h>
#include <Handlers/cc_packet_handlers.h>

// DMA channel configurations variables 
xdata dma_cfg dma_channels[5]; 
const dma_cfg dma_init_val = {0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00};

// Initializer 
void dmaInit(void){
	
	// Variables 
	uint8_t i; 
	
	// Allocate memory in xdata for DMA channels and set struct to 0x00
	for(i = 0; i < 5; i++){ // init channels 
		
		dma_channels[i] = dma_init_val; 
	}
	
	// Set addresses for DMA configuration registers
	DMA0CFGL = (uint8_t)((uint16_t)&dma_channels[0] & 0x00FF);  // Low byte of address for channel 0
  DMA0CFGH = (uint8_t)(((uint16_t)&dma_channels[0]) >> 8);  	// High byte of address for channel 0
	DMA1CFGL = (uint8_t)((uint16_t)&dma_channels[1] & 0x00FF);  // Low byte of address for channel 1-4
  DMA1CFGH = (uint8_t)(((uint16_t)&dma_channels[1]) >> 8);  	// High byte of address for channel 1-4
	
	// Setup DMA for UART receive 
	/* Have data transfers and stores happen over DMA to allow CPU to cycle seperately. Use X_U0DBUF */
	dma_channels[0].srcAddrHi = (uint8_t)((uint16_t)&X_U0DBUF >> 8); // High byte of source address, XDATA U0DBUF (Byte 0)
	dma_channels[0].srcAddrLo = (uint8_t)((uint16_t)&X_U0DBUF & 0x00FF); // Low byte of source address XDATA U0DBUF (Byte 1) 
	dma_channels[0].dstAddrHi = (uint8_t)((uint16_t)&uart_dma_rx_buffer.rawPayload >> 8); // High byte of destination address, XDATA U0DBUF (Byte 2) 
	dma_channels[0].dstAddrLo = (uint8_t)((uint16_t)&uart_dma_rx_buffer.rawPayload & 0x00FF); // Low byte of destination address XDATA U0DBUF (Byte 3) 
	dma_channels[0].byte4 = 0x40; // VLen and upper 5 bits of Len. VLEn = transfer n bytes (Byte 4)
	dma_channels[0].byte5 = 0x40; // Len of maximum value to transfer 64 bytes (Byte 5); 
	dma_channels[0].byte6 = 0x4E; // Word = 0 (8-bits), tmod = 10 (repeated single), trig = 01110 (RX complete trigger) (Byte 6)
	dma_channels[0].byte7 = 0x11; //SrcInc = 00 (no increment), DstInc = 01 (increment by 1 byte), IRQMASK = 0 (doesnt generate interrupt), M8 = 0 (8bits), prioirty = 01 (normal access)
	
	DMAARM |= 0x01; // arm DMA channel 0 for UART
	
	// Setup DMA for SPI 
	/* Passing and storing from SPI over DMA, shouldn't require CPU  to process data */
	
	// Setup DMA for RF
	/* Have stored to set memory address so that if CPU interrupts DMA is passing seperately */ 
	
	// Enable interrupts 
	
	
}