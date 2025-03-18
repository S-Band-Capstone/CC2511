

#ifndef CC_SPI_V1_H
#define CC_SPI_V1_H

#include <blink.h>
#include <../include/cc_uart_v1.h>
#include <../include/cc_dma_v1.h>
#include <../include/cc_packet_handlers.h>

/*
	32Mb SPI 
	Image Size: 450 KB 
	(1024px*1024px*3rgb)/(1024*8bits) image size ==> 384 KB
	(512px*512px*3rgb)/(1024*8bits) image size ==> 96 KB 
	(105px*105px*3rgb)/(1024*8bits) image size ==> 4.04 KB
	should try to get 128*128*3 for image size for memory consistency. 
	
	Image size probably will be compressed to the smallest size in best case (4.04 KB).
	Image size in worst case will be 96 KB. 

*/

// Variables
extern volatile __xdata packet spi_rx_buffer;		// Buffer for receive data 
extern volatile __xdata packet spi_tx_buffer; 	// Buffer for transmit data
extern uint8_t __xdata spi_rx_index;									// Indexer for receive
extern uint8_t __xdata spi_tx_index;									// Indexer for transmit
extern uint8_t __xdata spi_rx_length; 								// Length of incoming packet
extern volatile __bit spi_rx_packet_complete;							// Flag for full packet received

/* Interupts */

/* Initializer */
void spiInit(void); 

/* Functions for peripheral control */
void spi1Send(uint8_t *spiOutBuffer, uint16_t spiOutBufLen); 	//MOSI buffer tx
void spi1Receive(uint8_t *spiInBuffer, uint16_t spiInBufLen); 	// MISO buffer rx

/* Function Getters */

/* Function Setters */





#endif 