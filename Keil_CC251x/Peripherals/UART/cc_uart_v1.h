

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
extern volatile uart_packet xdata uartRxBuffer;		// Buffer for receive data 
extern volatile uart_packet xdata uartTxBuffer; 	// Buffer for transmit data
extern uint8_t xdata uartRxIndex;									// Indexer for receive
extern uint8_t xdata uartRxLength; 								// Length of incoming packet
extern bit rxPacketComplete;											// Flag for full packet received
extern uint8_t xdata uartTxIndex;									// Indexer for transmit

// Interrupts 
void uart0_rx_isr(void); 

// Initializer
void uartInit(void);

// Functions for peripheral control
void uart0Send(uint8_t *uartTxBuf, uint16_t uartTxBufLen);
void uart0Receive(uint16_t *suartRxBuf, uint16_t uartRxBufLen);

// Function Getters 
uart_packet *getUartPacket(void);

// Function Setters 
void resetRxIndex(void); 



#endif
