#include <cc2510.h> // Include CC2511 header file
#include <string.h> 

// Defining Registers for use 
/* IEN2*/
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

// Function prototypes
void UART_Init(void);
void UART_SendChar(char c);
char UART_ReceiveChar(void);
void UART_SendString(const char* str);
void blink(void);
void wait(void);

// Global Buffer
volatile char message[16] = ""; 

// ISR
void uart_rx_isr(void) interrupt URX0_VECTOR  {
    char received_data = U0DBUF; // Read received data
    URX0IF = 0; // Clear RX interrupt flag ==> Cleared by hardware when vectored to
    
}

volatile int message_index = 1;
void uart_tx_isr(void) interrupt UTX0_VECTOR {
	
	if (message[message_index] != '\0') {
		U0DBUF = message[message_index++]; // Send the next character
		blink(); 
    } else {
        UTX0IE = 0;  // Disable TX interrupt after sending the last character
        message_index = 1; // Reset index for the next transmission
    }
    UTX0IF = 0; // Clear TX interrupt flag
	blink(); 
}

void main() {
    // Initialize UART
    UART_Init();

    // Example usage
	//strcpy(message,"Hello World\n" );
    UTX0IF = 0;            // Clear TX interrupt flag
							// Enable TX interrupt

    // Start transmission by sending the first character
 
		UART_SendChar('h');
    while (1) {
        // Echo received characters
        if (U0CSR & 0x04) { // Check if data is received
            char received = UART_ReceiveChar();
            UART_SendChar(received); // Echo back
        }
    }
}

void UART_Init(void) {
    // Baud rate configuration for 9600 baud
    U0BAUD = 163;  // BAUD_M = 163
    U0GCR = 8;     // BAUD_E = 8

    // Enable UART mode
    UU0CSR |= 0x80;  // Set UART mode (MSB of U0CSR)

    // Configure TX (P0.3) and RX (P0.2) pins
    P0SEL |= 0x0C;  // Set P0.2 and P0.3 as peripheral I/O for USART0
    P0DIR |= 0x08;  // Set P0.3 (TX) as output
    P0DIR &= ~0x04; // Set P0.2 (RX) as input
	
	// enable interrupts
		EA = 1; 
		IEN2 |= 0x04;
		UTX0IF = 0;
		// Enable receiver and transmitter
		U0CSR |= 0x40;  // Enable receiver
}
	
void UART_SendChar(char c) {
    // Wait for TX buffer to be ready 
    while (!(TX_BYTE) && !(ACTIVE));
		U0DBUF = c;
}

void UART_SendString(const char *str) {
    //UTX0IE = 1; 
	while (*str) {
        UART_SendChar(*str++);
    }
}

char UART_ReceiveChar(void) {
    // Wait until data is received
    while (!(U0CSR & 0x04));
    return U0DBUF; // Return received character
}

void wait(void){
	;
}
void blink(void){
	
	unsigned int i;                     /* Delay var */
	P1DIR |= ( 1 << 1 ) ;
	while (i < 2500) {                         /* Loop forever */
		P1 ^= ( 1 << 1 ) ;                /* Toggle LED Pin */
		for (i = 0; i < 2500; i++)  {    /* Delay for 50000 Counts */
		wait ();                         /* call wait function */
		}
		
	}
}
