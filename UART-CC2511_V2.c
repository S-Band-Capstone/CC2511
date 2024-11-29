#include "CC2510.h"

SFRBIT(UIEN2, 0x9A, _IEN207, _IEN206, WDTIE, P1IE, UTX1IE, UTX0IE, P2IE, RFIE)
SBIT(_IEN207, 0xA1)
SBIT(_IEN206, 0xA0)
SBIT(WDTIE, 0x9F)
SBIT(P1IE, 0x9E)
SBIT(UTX1IE, 0x9D)
SBIT(UTX0IE, 0x9C)
SBIT(P2IE, 0x9B)
SBIT(RFIE, 0x9A)
/*U0CSR*/
SFRBIT(UU0CSR, 0x86, MODE, RE, SLAVE, FE, ERR, RX_BYTE, TX_BYTE, ACTIVE)
SBIT(MODE, 0x8D)
SBIT(RE, 0x8C)
SBIT(SLAVE, 0x8B)
SBIT(FE, 0x8A)
SBIT(ERR, 0x89)
SBIT(RX_BYTE, 0x88)
SBIT(TX_BYTE, 0x87)
SBIT(ACTIVE, 0x86)
/*U0UCR*/
SFRBIT(UU0UCR, 0xC4, FLUSH, FLOW, D9, BIT9, PARITY, SPB, STOP, START)
SBIT(FLUSH, 0xD1)
SBIT(FLOW, 0xD0)
SBIT(D9, 0xC9)
SBIT(BIT9, 0xC8)
SBIT(PARITY, 0xC7)
SBIT(SPB, 0xC6)
SBIT(STOP, 0xC5)
SBIT(START, 0xC4)


// Baud rate parameters for 9600 bps (example)
#define UART_BAUD_M 34
#define UART_BAUD_E 12

#define UART_RX_BUFFER_SIZE 50
#define UART_TX_BUFFER_SIZE 50

volatile unsigned char uartRxBuffer[UART_RX_BUFFER_SIZE];
volatile unsigned char uartTxBuffer[UART_TX_BUFFER_SIZE];
volatile unsigned char uartRxIndex = 0;
volatile unsigned char uartTxIndex = 0;

// Function prototypes 
void uartInitBitrate(unsigned char uartBaudM, unsigned char uartBaudE);

// Function to initialize the UART with baud rate and protocol
void uartInit(void) {
    
		P0SEL = 0x0c;
		P0DIR = 0x08;
		
		// Initialize the UART baud rate
    uartInitBitrate(UART_BAUD_M, UART_BAUD_E);

    // Set UART to 8 data bits, 1 stop bit, no parity
    U0CSR = 0x80;  // Set USART mode to UART (U0CSR.MODE = 1)

    // Start bit level: low, stop bit level: high, 1 stop bit, no parity, 8-bit data
    U0UCR = 0x00;  // U0UCR.START = 0 (start bit low), U0UCR.STOP = 1 (stop bit high)
                  // U0UCR.SPB = 0 (1 stop bit), U0UCR.PARITY = 0 (no parity)
                  // U0UCR.BIT9 = 0 (8-bit data), U0UCR.D9 = 0 (not using 9th bit)
                  // U0UCR.FLOW = 0 (no flow control)

    // Enable the UART RX and TX interrupts
    IEN0 |= (URX0IE | UTX0IE);  // Enable RX and TX interrupts
    EA = 1;                     // Enable global interrupts
}

// Function to initialize the UART bitrate
void uartInitBitrate(unsigned char uartBaudM, unsigned char uartBaudE) {
    U0BAUD = uartBaudM;    // Set the baud rate mantissa
    U0GCR = (U0GCR & ~0x1F) | uartBaudE;  // Set the baud rate exponent
}

// Function to transmit data over UART
void uartTransmit(unsigned char message) {
    uartTxBuffer[uartTxIndex++] = message;  // Store the data in TX buffer
    if (uartTxIndex >= UART_TX_BUFFER_SIZE) {
        uartTxIndex = 0;  // Wrap around if buffer overflows
    }
    if (uartTxIndex == 1) {
        // If it's the first byte in the buffer, enable TX interrupt to send it
        U0DBUF = uartTxBuffer[0];
    }
}

// Function to receive data from UART
unsigned char uartReceive(void) {
	unsigned int i = 0; 
	
	if (uartRxIndex > 0) {
        unsigned char message = uartRxBuffer[0];  // Get the received data
        
		for (i = 1; i < uartRxIndex; i++) {
            uartRxBuffer[i - 1] = uartRxBuffer[i];  // Shift the buffer contents
        }
        uartRxIndex--;  // Decrement the index
        return message;
    }
    return 0;  // Return 0 if buffer is empty
}

// UART RX interrupt service routine
void URX0_ISR(void) interrupt URX0_VECTOR {
    unsigned char message = U0DBUF;  // Read received data from the UART buffer
    if (uartRxIndex < UART_RX_BUFFER_SIZE) {
        uartRxBuffer[uartRxIndex++] = message;  // Store data in RX buffer
    }
    // The interrupt flag is cleared automatically by hardware
}

// UART TX interrupt service routine
void UTX0_ISR(void) interrupt UTX0_VECTOR {
    
	unsigned int i =0;
	if (uartTxIndex > 0) {
        // Load the next byte to transmit
        U0DBUF = uartTxBuffer[0];
        // Shift the buffer contents
        for (i = 1; i < uartTxIndex; i++) {
            uartTxBuffer[i - 1] = uartTxBuffer[i];
        }
        uartTxIndex--;  // Decrement the TX buffer index
    } else {
        // If buffer is empty, disable the TX interrupt
        IEN0 &= ~UTX0IE;  // Disable TX interrupt when buffer is empty
    }
}

int main(void) {
    // Initialize the UART
    uartInit();

    // Main loop
    while (1) {
        // Check if data is received
       /* if (uartRxIndex > 0) {
            // Read the received data
            unsigned char receivedData = uartReceive();
            // Echo the received data back through UART
            uartTransmit(receivedData);
        } */

        uartTransmit('H');
				uartTransmit('i');
				uartTransmit('\n');

     
    }

    return 0;
}