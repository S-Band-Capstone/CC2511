/*
*	File for UART serial communication. 
*
*
*	NOTE: Must have variables defined in .c and externed in .h. Else, compiler no likie.
*				- Thanks Alex 
*	
*
*
*/
// Headers 
#include <peripherals/UART/cc_uart_v1.h>
#include <Handlers/cc_packet_handlers.h>

// Variables
xdata volatile uart_packet uart_rx_buffer;		// Buffer for receive data 
xdata volatile uart_packet uart_tx_uffer;		// Buffer for transmit data 
uint8_t xdata uart_rx_index = 0;							// Indexer for receive 
uint8_t xdata uart_tx_index = 0;							// Indexer for transmit
uint8_t xdata uart_rx_length = 0; 						// Length of incoming packet
bit uart_rx_packet_complete = 0;										// Flag for full packet received 



// Interrupt service routines
//void uart0RxIsr(void) interrupt URX0_VECTOR { 
//  /* Interrupt handler for UART RX. Primary RX source. */
//	
//	// Temp byte transfer variable
//	uint8_t temp_byte;
//	
//	// read byte
//	URX0IF = 0; // Hardware will clear... just in case
//	temp_byte = U0DBUF; 
//	
//	if(uart_rx_index == 0 && temp_byte != SOF){ // for type  uart_packet
//		// Ignore if not valid start byte
//		return; 
//	}
//	
//	// Store byte into to packet
//	uart_rx_buffer.rawPayload[uart_rx_index++] = temp_byte; 
//	
//	// Store length byte which equals 2nd byte (3 because of previous increment)
//	if(uart_rx_index == 3){
//		
//		uart_rx_length = temp_byte;
//	}
//	
//	if (uart_rx_index >= (uart_rx_length + 6)){ // check packet length bytes (SOF(1) + CMD(1) + LEN(1) + DATA(LEN) + CRC(2) + EOF(1))
//		// need to double check number to make sure that condition is correct. 
//		 if(uart_rx_buffer.fields.eof == EOF){
//			uart_rx_packet_complete = 1; 
//			//blink();
//		}
//		uart_rx_index = 0;  // reset index 
//		//uart_rx_packet_complete = 1; // for testing
//		//blink();
//  } 
//} 

// UART Initializatin function
void uartInit(void){
	/* Initialization function for UART0 */
	
	// variables 
	uart_rx_index = 0;
	
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
	URX0IE = 1;
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
	
	for (uart_tx_index = 0; uart_tx_index < uartTxBufLen; uart_tx_index++) { 
		U0DBUF = uartTxBuf[uart_tx_index]; 
		while( !UTX0IF ); 
    	UTX0IF = 0; 
  	} 
	uart_tx_index = 0;
	U0CSR |= 0x40; // turn on receiver for RX
} 

// UART RX 
void uart0Receive(uint8_t* uartRxBuf, uint16_t uartRxBufLen) { 
	/* 	Polling function for UART RX 
	*		rtype: void
	*/ 
	
  U0CSR |= 0x40; URX0IF = 0; 
	for(uart_rx_index = 0; uart_rx_index < uartRxBufLen; uart_rx_index++) {
		while( !URX0IF ); 
		uartRxBuf[uart_rx_index] = U0DBUF; 
		URX0IF = 0; 
	} 
	//blink(); blink();
	U0CSR &= ~0x40; 
}

uart_packet *getUartPacket(void){
	/* 	Getter to return pointer to the buffer
	*		rtype: &uart_packet 		
	*/ 
	return &uart_rx_buffer;
}

void resetRxIndex(void){

	uart_rx_index = 0; 
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
//			bufferClear(uart_tx_uffer.rawPayload, UART_TX_BUFFER_SIZE);
//			uart_tx_uffer.rawPayload[0] = message1;
//			uart_tx_uffer.rawPayload[1] = message2;
//			uart0Send(uart_tx_uffer.rawPayload, UART_TX_BUFFER_SIZE);
//			bufferClear(uart_tx_uffer.rawPayload, UART_TX_BUFFER_SIZE);
//			
//			for(i = 0; i < sizeof(message3)-1; i++){
//				
//					uart_tx_uffer.rawPayload[i] = message3[i];
//			}
//			uart0Send(uart_tx_uffer.rawPayload, UART_TX_BUFFER_SIZE);
//			
//		}
//		
//		return 0; 
//	}

