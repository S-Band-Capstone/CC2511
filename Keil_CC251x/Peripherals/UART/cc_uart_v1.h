

#ifndef CC_UART_V1_H
#define CC_UART_V1_H

// Headers
#include <cc2510.h> 
#include <Common_Shared/blink.h>
#include <Handlers/cc_packet_handlers.h>

// Definded Variables 
#define UART_RX_BUFFER_SIZE   64 
#define UART_TX_BUFFER_SIZE 	64

// Variables 
extern xdata volatile uart_packet uart_rx_buffer;		// Buffer for receive data 
extern xdata volatile uart_packet uart_tx_buffer; 	// Buffer for transmit data
extern xdata volatile uart_dma_packet uart_dma_rx_buffer; // Buffer for receive data with DMA (TESTING)
extern uint8_t xdata uart_rx_index;									// Indexer for receive
extern uint8_t xdata uart_tx_index;									// Indexer for transmit
extern uint8_t xdata uart_rx_length; 								// Length of incoming packet
extern bit uart_rx_packet_complete;									// Flag for full packet received


// Interrupts 
void uart0RxIsr(void); 

// Initializer
void uartInit(void);

// Functions for peripheral control
void uart0Send(uint8_t *uartTxBuf, uint16_t uartTxBufLen);
void uart0Receive(uint8_t *suartRxBuf, uint16_t uartRxBufLen);

// Function Getters 
uart_packet *getUartPacket(void);

// Function Setters 
void resetRxIndex(void); 



#endif
