#include <cc2510.h> // Include CC2511 header file 
#include <UART-CC2511_V1.c> 

 
 void wait(void ){
/* Generic wait/nop function. */
    ;
}

void blink(void){
/*
Blink LED on board: 
More for visual and debugging purposes. 
	
Value of 'i' should ideally be equal to or smaller than 30k
*/
	int i = 0;                     /* Delay var */
	P1DIR |= ( 1 << 1 );

	P1 ^= ( 1 << 1 ) ;                /* Toggle LED Pin */
	for (i = 0; i < 25000; i++)  {    /* Delay for 50000 Counts */
		wait();                         /* call wait function */
		wait();
		wait();
	}
		
}
bit messageCheck(unsigned short* uartRxBuf, unsigned short bufferLen){

	int i;
	char testMsg[] = "test"; 
	for(i = 0; i < bufferLen; i++){
	
		if(testMsg[i] !=  uartRxBuf[i]){
			return 0; 
		}
	}
	return 1; 
}

/*-- Base Initialization --*/
void init(void){ 
/*      
This is a base fucntion to initialize the base system. 
Think of it as a warm up before the system does anything,
to make sure that it is in the right mode initially. 
*/
		
	// Clock set to 24 MHz with HS XOSC
	int i = 0;
	CLKCON &= 0x80;  // The OSC32K should be set to '1' if 
									 // using HS XOSC (Refer to datasheet) 
	
	// Wait for HS XOSC to be stable
	while(SLEEP & 0x40){
		wait(); 
	}
	
	// Turn off HS RCOSC
	// Calibration Done
	SLEEP |= 0x04; 
	
	// Blink
	while(i < 10){
		blink(); 
		i++; 
	}
	
	
}


// UART SECTION // 


// MAIN SECTION //
/*

Main fucntion by itself without a loop will loop indefinitely 
without a loop. For proper fuctionality, incorporate loop in 
the main function for things to run correctly. 

*/
int main(void){
	unsigned char message1 = 'H'; // NEED TO DECLARE Variables outside of loops
	unsigned char message2 = 'i'; // 
	unsigned char message3[] = " Hello\n";
	unsigned int i;

	// initialize system and modules 
	init(); 
	uartInit(); 
	
	while(1){
		bufferClear(uartTxBuffer, UART_TX_BUFFER_SIZE);
		uartTxBuffer[0] = message1;
		uartTxBuffer[1] = message2;
		uart0Send(uartTxBuffer, UART_TX_BUFFER_SIZE);
		bufferClear(uartTxBuffer, UART_TX_BUFFER_SIZE);
			
		for(i = 0; i < sizeof(message3)-1; i++){
			
			uartTxBuffer[i] = message3[i];
		}
		uart0Send(uartTxBuffer, UART_TX_BUFFER_SIZE);
	
		
	}
	
	return 0; 
}
	
		
