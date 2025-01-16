

#ifndef CC_UART_V1_H
#define CC_UART_V1_H

void bufferClear(unsigned short* buffer, unsigned short bufferLen);
void uartInit(void);
void uart0Send(unsigned short* uartTxBuf, unsigned short uartTxBufLength);
void uart0Receive(unsigned short* uartRxBuf, unsigned short uartRxBufLength);
void uart0_rx_isr(void); 

#endif
