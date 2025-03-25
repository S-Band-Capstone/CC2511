

#ifndef CC_UART_V1_H
#define CC_UART_V1_H

// Headers
#include <cc2510fx.h> 
#include <blink.h>
#include <cc_packet_handlers.h>

// Definded Variables 
#define UART_RX_BUFFER_SIZE   64 
#define UART_TX_BUFFER_SIZE 	64

// Variables 
extern volatile __xdata packet uart_rx_buffer;		// Buffer for receive data 
extern volatile __xdata packet uart_tx_buffer; 	// Buffer for transmit data
extern uint8_t __xdata uart_rx_index;									// Indexer for receive
extern uint8_t __xdata uart_tx_index;									// Indexer for transmit
extern uint8_t __xdata uart_rx_length; 								// Length of incoming packet
extern volatile __bit uart_rx_packet_complete;							// Flag for full packet received
extern volatile __bit uart_tx_packet_complete;


// Interrupts 
//void uart0RxIsr(void) __interrupt(URX0_VECTOR); 

// Initializer
void uartInit(void);

// Functions for peripheral control
void uart0Send(uint8_t *uartTxBuf, uint8_t uartTxBufLen);
void uart0SendUnstructured(uint8_t *uartTxBuf, uint8_t bufferLen );
void uart0Receive(uint8_t *suartRxBuf, uint16_t uartRxBufLen);

// Function Getters 
packet *getUartPacket(void);

// Function Setters 
void setUartTxBuffer(uint8_t *uartTxBuf, uint8_t uartTxBufLen);
void resetRxIndex(void); 



#endif
