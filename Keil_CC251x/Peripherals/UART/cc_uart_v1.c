#include <cc2510.h> 
#include <peripherals/UART/cc_uart_v1.h>
#include <Common_Shared/blink.h>


/// Define the buffers that are going to be used 
#define UART_RX_BUFFER_SIZE   50 
#define UART_TX_BUFFER_SIZE 	50
unsigned short xdata uartRxBuffer[UART_RX_BUFFER_SIZE]; 
unsigned short xdata uartTxBuffer[UART_TX_BUFFER_SIZE]; 
unsigned short xdata uartRxIndex;

// Interrupt service routines
void uart0_rx_isr(void) interrupt URX0_VECTOR { 
  
	URX0IF = 0; // Hardware will clear... just in case
	//uartRxIndex++; 
  uartRxBuffer[uartRxIndex++] = U0DBUF; 
	
	 if (uartRxIndex >= UART_RX_BUFFER_SIZE) { 
    uartRxIndex = 0; IEN0 &= ~0x04; 
  } 
	blink();
	blink();
} 

// UART Initializatin function
void uartInit(void){
		
	// Enable UART
	U0CSR |= 0x80; 
	
	// Setup UART protocol 
	U0UCR = 0x02;	// Using stop bit high works best for continuity
								// Start bit is set to low and stop to high 
	
	// Set Baude Rate 9600
	U0BAUD = 163; // Register dedicated to just Manitssa 
	U0GCR = 0x08; // Register has lower 5 bits for exponent
	//U0GCR = (U0GCR &~ 0x1F) | 0x08; // = mask that doesnt change upper bits if set for SPI
	
	// Select pins and pin directions 
	// Configure TX (P0.3) and RX (P0.2) pins
  P0SEL |= 0x0C;  // Set P0.2 and P0.3 as peripheral I/O for USART0
  P0DIR |= 0x08;  // Set P0.3 (TX) as output
  P0DIR &= ~0x04; // Set P0.2 (RX) as input
	
	// Setup Interrupts
	EA = 1; 
	URX0IE = 1;
	//IEN2 |= 0x04;
	UTX0IF = 0;
	
	// Enable receiver to listen for Message
	U0CSR |= 0x40; 
	
}


// UART TX 
void uart0Send(unsigned short* uartTxBuf, unsigned short uartTxBufLength) { 
	unsigned short uartTxIndex; 
  U0CSR &= ~0x40; //turn off receiver for RX
	UTX0IF = 0; 
	for (uartTxIndex = 0; uartTxIndex < uartTxBufLength; uartTxIndex++) { 
		U0DBUF = uartTxBuf[uartTxIndex]; 
		while( !UTX0IF ); 
    UTX0IF = 0; 
  } 
	U0CSR |= 0x40; // turn on receiver for RX
} 

// UART RX 
void uart0Receive(unsigned short* uartRxBuf, unsigned short uartRxBufLength) { 
	unsigned short uartRxIndex; 
  U0CSR |= 0x40; URX0IF = 0; 
	for(uartRxIndex = 0; uartRxIndex < uartRxBufLength; uartRxIndex++) {
		while( !URX0IF ); 
		uartRxBuf[uartRxIndex] = U0DBUF; 
		URX0IF = 0; 
	} 
	blink(); blink();
	U0CSR &= ~0x40; 
}


// Buffer Clearing 
void bufferClear(unsigned short* buffer, unsigned short bufferLen){
	int i;

	// NOTE: potential optimization - only clear where needed
	for (i = 0; i < bufferLen; i++){
		
		buffer[i] = '\0';
	}
}




