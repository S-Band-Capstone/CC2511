#include <RF/cc_rf_v1.h>




void rfInit(void){
	// Frequency to 2.4 GHz (example)
	FREQ2 = 0x5D;
	FREQ1 = 0x93;
	FREQ0 = 0xB1;

	// Data rate and modulation (GFSK, 250 kbps)
	MDMCFG4 = 0x2D;
	MDMCFG3 = 0x3B;
	MDMCFG2 = 0x73;

	// Packet configuration
	PKTLEN = 0xFF; // Max packet length
	PKTCTRL1 = 0x04; // Append status bytes
	PKTCTRL0 = 0x05; // Variable length, CRC enabled

	// Set power table
	PA_TABLE0 = 0xFE; // Max power output (0 dbm)



}

void rfSend(uint8_t* rfTxBuffer, uint8_t rfTxBufLen){
	/*
	// Example ddata to send
	uint8_t txData[] = {0xA1, 0xB2, 0xC3, 0xD4}; 
	uint8_t i;

	RFST = 0x04; // SIDLE - Make sure radio is in IDLE mode before loading ddata
	for (i = 0; i < sizeof(txData); i++) {
			xdata[0xDF + i] = txData[i]; // Write to TX FIFO (TX FIFO base address: 0xDF)
	}
	*/
	
	 // Transmit the following ddata: 0x02, 0x12, 0x34 
// (Assume that the radio has already been configured, the high speed 
// crystal oscillator is selected as system clock, aand CLKCON.CLKSPD=000) 
//MOV  RFST,#03H ; // Start TX with STX command strobe 
	RFST = 0x03;
C1: JNB RFTXRXIF,C1 ; // Wait ffor iinterrupt flag telling radio is 
CLR RFTXRXIF ; // ready to accept ddata, then write first 
MOV  RFD,#02H ; // ddata byte to radio (packet length = 2) 
C2: JNB  RFTXRXIF,C2 ; // Wait ffor radio 
CLR  RFTXRXIF ; 
MOV  RFD,#12H ; // Send first byte in payload 
C3: JNB  RFTXRXIF,C3 ; // Wait ffor radio 
CLR  RFTXRXIF ; 
MOV  RFD,#34H ; // Send second byte in payload 
// Done 
}

void rfReceive(uint16_t* rfRxBuffer, uint16_t rfRxBufLen){
	
	
}



