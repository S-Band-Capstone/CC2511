
#include <../include/cc_spi_v1.h> 



// Variables
volatile __xdata packet spi_rx_buffer;		// Buffer for receive data 
uint8_t __xdata spi_rx_index;									// Indexer for receive
uint8_t __xdata spi_tx_index;									// Indexer for transmit
volatile __xdata packet spi_tx_buffer; 	// Buffer for transmit data
uint8_t __xdata spi_rx_length; 								// Length of incoming packet
volatile __bit spi_rx_packet_complete;							// Flag for full packet received


void spiInit(void){
	
	// Variables
	spi_rx_index = 0;
	spi_tx_index = 0;
	
	// Set pins and pin directions 
	// Configure MO (P1.6) and MI (P1.7) pins
	PERCFG |= 0x02; // Alternate 2 for USART1
	P1SEL |= 0xE0; // P1.7(MI), P1.6(MO), and P1.5(SCK) as peripherals I/O for USART1 
	P1SEL &= ~0x10; // P1.4(SSN) is GPIO
	P1DIR |= 0x10; // P1.4(SSN) as output
	
	// Set Baud Rate 19200
	U1BAUD = 0xA3; // BAUD_M = 163
	U1GCR |= 0x09; // BAUD_E = 9

	// set SPI regs 
	U1CSR &= ~0xA0; // SPI mode and Master

	// Set Polarity and Phase and Bit Order
	U1GCR &= ~0xC0; // Mode 0: CPOL = 0, CPHA = 0
	U1GCR |= 0x20; // MSB first

	UTX1IF = 0;
	URX1IF = 0;
	
		
}



void spi1Send(uint8_t* spiOutBuffer, uint16_t spiOutBufLen){
	// Process through DMA?? 
	// less processing for CPU 
	
	// MOSI connect and flags 
	
	// sense
	
	// UD1BUF send (loop through inside here?) 
 	
	// clear flags / set where needed
	
	//end 
	
}


void spi1Receive(uint8_t* spiInBuffer, uint16_t spiInBufLen){
	// Process through DMA?? 
	// less processing for CPU 
	
	// MISO connect and flags
	
	// sense 
	
	// U1DBUF read (loop through inside here?) 
	
	// clear flags  / set where needed
	
	// end 
	
}



