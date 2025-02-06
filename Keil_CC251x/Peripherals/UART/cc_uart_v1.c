/*
*	File for UART serial communication. 
*
*
*	NOTE: Must have variables defined in .c and externed in .h. Else, compiler no likie.
*				- Thanks Alex 
*	
*	NOTE: Buffer sizes must be 16 bits, as memory addresses are 16 bits. Actually not true.
*
*/
// Headers 
#include <peripherals/UART/cc_uart_v1.h>
#include <Handlers/cc_packet_handlers.h>

// Variables
volatile uart_packet xdata uartRxBuffer;		// Buffer for receive data 
volatile uart_packet xdata uartTxBuffer;		// Buffer for transmit data 
uint8_t xdata uartRxIndex = 0;							// Indexer for receive 
uint8_t xdata uartRxLength = 0; 						// Length of incoming packet
bit rxPacketComplete = 0;										// Flag for full packet received 
uint8_t xdata uartTxIndex = 0;							// Indexer for transmit


// Interrupt service routines
void uart0_rx_isr(void) interrupt URX0_VECTOR { 
  /* Interrupt handler for UART RX. Primary RX source. */
	
	// Temp byte transfer variable
	uint8_t temp_byte;
	
	// read byte
	URX0IF = 0; // Hardware will clear... just in case
	temp_byte = U0DBUF; 
	
	if(uartRxIndex == 0 && temp_byte != SOF){
		// Ignore if not valid start byte
		return; 
	}
  
	// Store byte into to packet
	uartRxBuffer.rawPayload[uartRxIndex++] = temp_byte; 
	
	if(uartRxIndex == 3){
		// Store length byte which equals 3 byte
		uartRxLength = temp_byte;
	}
	if (uartRxIndex >= (uartRxLength + 6)){ // check packet length bytes (SOF(1) + CMD(1) + LEN(1) + DATA(LEN) + CRC(2) + EOF(1))
		// need to double check number to make sure that condition is correct. 
		 if(uartRxBuffer.rawPayload[uartRxIndex] == EOF){
			rxPacketComplete = 1; 
			//blink();
		}
		uartRxIndex = 0;  // reset index 
		rxPacketComplete = 1; // for testing
		//blink();
  } 
	//blink();
	//blink();
} 

// UART Initializatin function
void uartInit(void){
	/* Initialization function for UART0 */
	
	// variables 
	uartRxIndex = 0;
	
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
void uart0Send(uint8_t *uartTxBuf, uint16_t uartTxBufLen) { 
	/* 	Polling function for UART TX
	*		rtype: void
	*
	*		NOTE: When sending char array, BufLen = length - 1. Array Indexing.
	*					When sending standalone char, BufLen = length. Singular value. 
	*/
	
  U0CSR &= ~0x40; //turn off receiver for RX
	UTX0IF = 0; 
	
	for (uartTxIndex = 0; uartTxIndex < uartTxBufLen; uartTxIndex++) { 
		U0DBUF = uartTxBuf[uartTxIndex]; 
		while( !UTX0IF ); 
    UTX0IF = 0; 
  } 
	U0CSR |= 0x40; // turn on receiver for RX
} 

// UART RX 
void uart0Receive(uint16_t* uartRxBuf, uint16_t uartRxBufLen) { 
	/* 	Polling function for UART RX 
	*		rtype: void
	*/ 
	
	unsigned short uartRxIndex; 
  U0CSR |= 0x40; URX0IF = 0; 
	for(uartRxIndex = 0; uartRxIndex < uartRxBufLen; uartRxIndex++) {
		while( !URX0IF ); 
		uartRxBuf[uartRxIndex] = U0DBUF; 
		URX0IF = 0; 
	} 
	blink(); blink();
	U0CSR &= ~0x40; 
}

uart_packet *getUartPacket(void){
	/* 	Getter to return pointer to the buffer
	*		rtype: &uart_packet 		
	*/ 
	return &uartRxBuffer;
}

void setRxIndex(void){

	uartRxIndex =0; 
}

/***************************************** EXAMPLE USES **************************************/

/*		UART TX SEND		*/
//	int main(void){
//		
//		unsigned char message1 = 'H'; // NEED TO DECLARE Variables outside of loops
//		unsigned char message2 = 'i'; // 
//		unsigned char message3[] = " Hello\n";
//		unsigned int i;
//		
//		// initialize system and modules 
//		init(); 
//		uartInit(); 
//		
//		
//		while(1){
//			
//			bufferClear(uartTxBuffer.rawPayload, UART_TX_BUFFER_SIZE);
//			uartTxBuffer.rawPayload[0] = message1;
//			uartTxBuffer.rawPayload[1] = message2;
//			uart0Send(uartTxBuffer.rawPayload, UART_TX_BUFFER_SIZE);
//			bufferClear(uartTxBuffer.rawPayload, UART_TX_BUFFER_SIZE);
//			
//			for(i = 0; i < sizeof(message3)-1; i++){
//				
//					uartTxBuffer.rawPayload[i] = message3[i];
//			}
//			uart0Send(uartTxBuffer.rawPayload, UART_TX_BUFFER_SIZE);
//			
//		}
//		
//		return 0; 
//	}

