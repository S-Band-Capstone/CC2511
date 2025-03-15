
// Headers
#include <../include/cc_rf_v1.h>
#include <../include/blink.h>
#include <../include/cc_uart_v1.h>
#include <../include/cc_dma_v1.h>

// Variables 
__xdata volatile packet rf_rx_buffer;
__xdata volatile packet rf_tx_buffer;
uint8_t __xdata rf_rx_index;
uint8_t __xdata rf_tx_index;
uint8_t __xdata rf_rx_length;
__bit volatile rf_rx_packet_complete;
uint8_t mode;
uint8_t max_len = 64; // Can change. To change, make sure to update inside packet Handler. 



// Interrupts 
//void rfRxIsr(void) __interrupt(RFTXRX_VECTOR){
//	
//	uint8_t temp_byte; // Temp byte
//	
//	// read byte from RF data register 
//	temp_byte  = RFD;
//	
//	// return if not valid
//	if (rf_rx_index == 1 && temp_byte != SOF){
//		return; // return error value? 
//	}
//	
//	// Store byte
//	rf_rx_buffer.rawPayload[rf_rx_index] = temp_byte;
//	
//	// Store length for check after, since first byte should hold payload length (in bytes)
//	if(rf_rx_index == 0){
//		
//		rf_rx_length = temp_byte;
//	}
//	
//	// necessary increment for next check block
//	rf_rx_index++;
//	
//	// Packet complete
//	if (rf_rx_index >= rf_rx_length + 3){ // 3 = payload length + SOF + EOF
//		if(rf_rx_buffer.fields.eof == EOF){ // Make sure end of packet 
//			
//			rf_rx_packet_complete = 1; 
//		}
//		
//		rf_rx_index = 0;
//	}
//	/* KEEP state in RX mode, let packet Handler deal with state changes */ 
//	
//}

// void rfIsr(void) __interrupt(RFTXRX_VECTOR){ 
// 	/* Disable when testing RF */
// 	uint8_t msg[] = "RF ISR\n";
// 	// Variables
// 	RFTXRXIF = 0;
// 	//rf_rx_buffer.rawPayload[0] = RFD; 
// 	//blink();
// 	//uart0Send(msg, 7);	

// }

void rfOverflow(void) __interrupt(RF_VECTOR){
	uint8_t msg[] = "Overflow\n";
	uint8_t msg1[] = "TX Underflow\n";
	uint8_t msg2[] = "RX Overflow\n";
	RFST = SIDLE;
	//mode = SIDLE;
	uart0Send(msg, 9);
	
	if (RFIF & 0x80){
		uart0Send(msg1, sizeof(msg1));
	}
	else if(RFIF & 0x40){
		uart0Send(msg2, sizeof(msg2));
	}
		
}


// Initialization 
void rfInit(void){
	
	// Initialize variables 
	rf_rx_index = 0;
	rf_tx_index = 0;
	
	// Setup Registers
	/*  Values taken straight from Smart RF Studio (Data Rate = 2.4kbaude)*/
	IOCFG2 = 0x2E; 
	IOCFG1 = 0x00;
	IOCFG0 = 0x00;  // 0x06; for debugging when low byte set.
 	SYNC1 = 0xD3;
	SYNC0 = 0x91;
	PKTLEN = 0xFF; // 0xFF
	PKTCTRL1 = 0x00; // 0x04 = Append_Status; if we include, it messes with RX
	PKTCTRL0 = 0x01; // 0x05 = CRC enabled with variable length , 0x01 = variable length
	ADDR = 0x00;
	CHANNR = 0x00;
	FSCTRL1 = 0x0A;
	FSCTRL0 = 0x00;
	FREQ2 = 0x65;
	FREQ1 = 0x60;
	FREQ0 = 0x00;	
	MDMCFG4 = 0x76;
	MDMCFG3 = 0xA3;	
	MDMCFG2 = 0x03;	//00 = no preabmle and sync, 0x03 = 30/32 preambe and sync, 0x02 = 16/16 preamble and sync
	MDMCFG1 = 0x23;	
	MDMCFG0 = 0x11;
	DEVIATN = 0x45;	
	MCSM2 = 0x07;
 	MCSM1 = 0x30; // 0x3E; RXOFF_MODE = 11 (Stay in RX), TXOFF_MODE == 10 (Stay in TX); C
	MCSM0 = 0x14;
	FOCCFG = 0x16;
	BSCFG = 0x6c;
	AGCCTRL2 = 0x03; 
	AGCCTRL1 = 0x40;
	AGCCTRL0 = 0x91;
	FREND1 = 0x56;
	FREND0 = 0x10;
	FSCAL3 = 0xA9;
	FSCAL2 = 0x0A;
	FSCAL1 = 0x00;	
	FSCAL0 = 0x11;	
	PA_TABLE0 =	0xFE;
	//LQI = 0x80; 
	
	/*  Values taken straight from Smart RF Studio (Data Rate = 250kbaude)*/
	// IOCFG2 = 0x2E; 
	// IOCFG1 = 0x00;
	// IOCFG0 = 0x00;  // 0x06; for debugging when low byte set.
 	// SYNC1 = 0xD3;
	// SYNC0 = 0x91;
	// PKTLEN = 0xFF; // 0xFF
	// PKTCTRL1 = 0x04; // 0x04 = Append_Status; if we include, it messes with RX
	// PKTCTRL0 = 0x01; // 0x05 = CRC enabled with variable length , 0x01 = variable length
	// ADDR = 0x00;
	// CHANNR = 0x00;
	// FSCTRL1 = 0x0A;
	// FSCTRL0 = 0x00;
	// FREQ2 = 0x65;
	// FREQ1 = 0x60;
	// FREQ0 = 0x00;	
	// MDMCFG4 = 0x1D;
	// MDMCFG3 = 0x55;	
	// MDMCFG2 = 0x73;	// 0x73 = 30/32 preambe and sync match, MSK modulation
	// MDMCFG1 = 0x23;	
	// MDMCFG0 = 0x11;
	// DEVIATN = 0x45;	
	// MCSM2 = 0x07;
 	// MCSM1 = 0x30; // 0x3E: RXOFF_MODE = 11 (Stay in RX), TXOFF_MODE == 10 (Stay in TX)
	// MCSM0 = 0x14;
	// FOCCFG = 0x1D; // Saturation point = 01 (+-)BWChan/8, 
	// BSCFG = 0x1C; 
	// AGCCTRL2 = 0xC7; 
	// AGCCTRL1 = 0x00;
	// AGCCTRL0 = 0xB2;
	// FREND1 = 0xB6;
	// FREND0 = 0x10;
	// FSCAL3 = 0xEA;
	// FSCAL2 = 0x0A;
	// FSCAL1 = 0x00;	
	// FSCAL0 = 0x11;	
	// PA_TABLE0 =	0xFE;
	// LQI = 0x80; 
	
	// Interrupt enables 
	//RFTXRXIE = 1;		// RFD TX and RX
	RFIM = 0xC0;
	IEN2 |= 0x01; // General RF interrupts 
	
	
	
	/* States (Assuming intially startup at IDLE*/ 
	// Set initial state... Probably RX unless interrupt can change state out of IDLE 
	RFST = SIDLE; 
	mode = SIDLE;
	delayMs(1); // delay 1ms 
	
	// Manually calibabrate frequency synthesizer if FS_AUTOSCAL = `00`
	RFST = SCAL;
	mode = SCAL;
	delayMs(1);
	

}

// Functions 
void rfSend(uint8_t *rfTxBuffer, uint16_t rfTxBufLen){
	
	// Variables 
	uint8_t i = 0;		// loop iterator
	
	delayMs(100);
	RFD = rfTxBuffer[0];
	
}

void rfReceive(uint8_t* rfRxBuffer, uint16_t rfRxBufLen){
	
	// Variables 
	uint8_t i = 0;
	uint8_t packet_length;
	
	// Set strobe command for receive 
	//RFST = SRX;
	if(mode != SRX){
		mode = SRX;
	}
		delayMs(1);
	
	// Wait for flag to be set
	//waitRfTxRxFlag();
	
	// Get first byte (packet length)
	packet_length = RFD;
	if(packet_length >= rfRxBufLen){// check to make sure packet can fit in buffer.
		
		return; // maybe return error code
	}else{
		
		rfRxBuffer[0] = packet_length;
		
		// get rest of the bytes 
		for(i = 1; i < packet_length; i++){
		
			//RFST = SRX;
			//delayMs(1);
			//waitRfTxRxFlag(); 
			rfRxBuffer[i] = RFD; 
			//blink();
		}
	}
	
	// Calibrate if FS_AUTOSCAL set to `00`
	//RFST = SCAL;
	//mode = SCAL;
	//delayMs(1); // delay 1 MS to allow state transition 
	
	// Return to IDLE
	RFST = SIDLE;
	mode = SIDLE; 
	delayMs(1);
	
}

static void waitRfTxRxFlag(void){

	while (RFTXRXIF) {
		// Wait until flag is set
		// could add timer
		//blink();
	}
	//blink();
	RFTXRXIF = 0;
}

void rfStateMachine(uint8_t mode) {
	
	RFTXRXIF = 0;
	switch (mode) { // Transition States
		case SFSTXON: {
			
			// Set state 
			RFST = SFSTXON;
			break;
		}
		
		case SRX: { 
			
			// Make sure Append status is off
			PKTCTRL1 = 0x00;
			
			// Disable DMA for RFTX and enable for RFRX
			//DMAARM &= ~(0x04); 	// Disables DMA channel 2 (RFTX)
			dmaAbort(2);
			//DMAARM |= 0x03; 	// ARM DMA channel 0 (UART), DMA channel 1 (RFRX)
			setDmaArm(1);
			
			// Set state
			RFST = SRX;
			//delayMs(1);

			break;
		}

		case STX: {
			
			// Disable DMA for RFRX and enable for RFTX
			//DMAARM &= ~(0x02); 	// Disables DMA channel 1 (RFRX)
			dmaAbort(1);
			//DMAARM |= 0x05; 	// ARM DMA Channel 0 (UART), DMA Channel 1 (RFTX)
			setDmaArm(2);
			//delayMs(1);
			
			// Set state
			RFST = STX;
			//delayMs(1);
			//while (!(RFTXRXIF & 0x01));
			//RFTXRXIF = 0;

			break;
		}

		case SIDLE: {

			break;
		}

		case SCAL: {

			break;
		}
	}

	switch (mode) { // State Actions
		case SFSTXON: {

			break;
		}
		
		case SRX: {

			break;
		}

		case STX: {
			rfSend(rf_tx_buffer.rawPayload, 5);
			//dmaRequest(2);
			
			break;
		}

		case SIDLE: {

			break;
		}

		case SCAL: {

			break;
		}
	}
}
