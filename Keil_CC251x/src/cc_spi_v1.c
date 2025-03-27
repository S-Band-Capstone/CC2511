
#include <../include/cc_spi_v1.h> 



// Variables
volatile __xdata packet spi_rx_buffer;		// Buffer for receive data 
uint8_t __xdata spi_rx_index;									// Indexer for receive
uint8_t __xdata spi_tx_index;									// Indexer for transmit
volatile __xdata packet spi_tx_buffer; 	// Buffer for transmit data
uint8_t __xdata spi_rx_length; 								// Length of incoming packet
volatile __bit spi_rx_packet_complete;							// Flag for full packet received

void USART1_RX_ISR(void) __interrupt(URX1_VECTOR) {
    // Read received byte
    uint8_t data = U1DBUF;
    
    // Store in buffer if there's room
    if (spi_rx_index < sizeof(spi_rx_buffer.rawPayload)) {
        spi_rx_buffer.rawPayload[spi_rx_index++] = data;
    }
    
    // Check if this is a complete packet
    if (spi_rx_index == spi_rx_length) {
        spi_rx_packet_complete = 1;
        
        // Set SSN high to end transmission
        P1_4 = 1;
    }
    
    // Clear interrupt flag
    URX1IF = 0;
}


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



void spi1Send(uint8_t* spiOutBuffer, uint16_t spiOutBufLen){ // polling 
	uint16_t i;
	
	// Set SSN low to begin transmission
    P1_4 = 0;
    
    // Poll-based implementation
    for(i = 0; i < spiOutBufLen; i++) {
        U1CSR &= ~0x02;              // Clear RX_BYTE
        U1DBUF = spiOutBuffer[i];    // Write data to TX buffer
        
        // Wait for TX to complete (TX_BYTE set)
        while(!(U1CSR & 0x01));
    }
    
    // Set SSN high to end transmission
    P1_4 = 1;
}
	



void spi1Receive(uint8_t* spiInBuffer, uint16_t spiInBufLen){
	uint16_t i;
    
    // Set SSN low to begin transmission
    P1_4 = 0;
    
    // Poll-based implementation
    for(i = 0; i < spiInBufLen; i++) {
        U1CSR &= ~0x02;              // Clear RX_BYTE
        U1DBUF = 0xFF;               // Write dummy byte to generate clock
        
        // Wait for RX to complete (RX_BYTE set)
        while(!(U1CSR & 0x02));
        
        spiInBuffer[i] = U1DBUF;     // Read received data
    }
    
    // Set SSN high to end transmission
    P1_4 = 1;
}
	




