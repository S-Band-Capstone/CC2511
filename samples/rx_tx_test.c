#include <CC2510.H>
#include <register_export.h>

#define TX_BYTE 0xAB // Example byte to send

#define TEST0 XREG(0xDF25)
#define TEST1 XREG(0xDF24)

//void transmitByte(void);
//void receiveByte(void);
//void delay_approx(unsigned int ms);




void delay_approx(unsigned int ms) {
	unsigned int i, j;
	
	for (i = 0; i < ms; i++)
			for (j = 0; j < 700; j++);
}



void initializeRadio() {
	  // Set CC2511 to RX mode
    //RFST = 0x04;  // SRX strobe command to enter IDLE mode
		//delay_approx(100); // brief delay to allow IDLE to take effect
		//RFST = 0x01; // SRX strobe command to enter SCAL mode
		//delay_approx(100);
	
    PKTCTRL0  = SMARTRF_SETTING_PKTCTRL0;
    CHANNR    = SMARTRF_SETTING_CHANNR;
    FSCTRL1   = SMARTRF_SETTING_FSCTRL1;
    FREQ2     = SMARTRF_SETTING_FREQ2;
    FREQ1     = SMARTRF_SETTING_FREQ1;
    FREQ0     = SMARTRF_SETTING_FREQ0;
    MDMCFG4   = SMARTRF_SETTING_MDMCFG4;
    MDMCFG3   = SMARTRF_SETTING_MDMCFG3;
    MDMCFG2   = SMARTRF_SETTING_MDMCFG2;
    MDMCFG1   = SMARTRF_SETTING_MDMCFG1;
    MDMCFG0   = SMARTRF_SETTING_MDMCFG0;
    DEVIATN   = SMARTRF_SETTING_DEVIATN;
    MCSM0     = SMARTRF_SETTING_MCSM0;
    FOCCFG    = SMARTRF_SETTING_FOCCFG;
    BSCFG     = SMARTRF_SETTING_BSCFG;
    AGCCTRL2  = SMARTRF_SETTING_AGCCTRL2;
    AGCCTRL1  = SMARTRF_SETTING_AGCCTRL1;
    AGCCTRL0  = SMARTRF_SETTING_AGCCTRL0;
    FREND1    = SMARTRF_SETTING_FREND1;
    FSCAL3    = SMARTRF_SETTING_FSCAL3;
    FSCAL1    = SMARTRF_SETTING_FSCAL1;
    FSCAL0    = SMARTRF_SETTING_FSCAL0;
    TEST1     = SMARTRF_SETTING_TEST1;
    TEST0     = SMARTRF_SETTING_TEST0;
    PA_TABLE0 = SMARTRF_SETTING_PA_TABLE0;
    IOCFG0    = SMARTRF_SETTING_IOCFG0;
    LQI       = SMARTRF_SETTING_LQI;

		// SFSTXON
		//RFST = 0x00;
}

void transmitByte() {
		unsigned char state = MARCSTATE;
		unsigned char rfifs = RFIF;
	  //unsigned int i;
	
    // Ensure radio is in IDLE mode
    RFST = 0x04;  // SIDLE
    delay_approx(1);
		state = MARCSTATE; // debugging
		rfifs = RFIF; // debugging

    // Clear TX FIFO
    //RFST = 0x3A;  // SFTX
    //delay_approx(1);

	
		//P1 |= (1 << 1);
    // Load byte into TX FIFO
    RFD = TX_BYTE;
		state = MARCSTATE; // debugging
	  rfifs = RFIF; // debugging
		
    // Start transmission
    RFST = 0x03;  // STX
		state = MARCSTATE; // debugging
		rfifs = RFIF; // debugging
		
		while (MARCSTATE != 0x13) {
			state = MARCSTATE; // debugging
			rfifs = RFIF; // debugging
			// Wait until the RF module enters the TX state
			// Optionally add a timeout to avoid getting stuck
		}
	
		/*
		state = MARCSTATE;
		if (state != 0x13) {  // 0x13 = TX state
				for (i = 0; i < 4; i++) {
						P1 ^= (1 << 1);  // Toggle LED on/off
						delay_approx(1000);  // Longer delay to differentiate
				}
		}
		*/

    // Wait for TX to complete
    while (!(RFIF & 0x10));  // Wait for TX interrupt flag

    // Clear TX interrupt flag
    RFIF &= ~0x10;
		
		//P1 &= !(1 << 1);

    // Return to IDLE mode
    RFST = 0x36;  // SIDLE
}


unsigned char receiveByte() {
    unsigned char receivedByte = 0;
    unsigned int timeout = 10;
		unsigned int i;
	
    // Ensure radio is in IDLE mode
    RFST = 0x36;  // SIDLE
    delay_approx(1);

    // Clear RX FIFO
    RFST = 0x3B;  // SFRX
    delay_approx(1);

    // Set radio to RX mode
    RFST = 0x34;  // SRX

    // Wait for RX or timeout
    while (!(RFIF & 0x01) && timeout--) {
        delay_approx(100);  // Short delay
    }

		if (timeout) {
				// Retrieve received byte
				receivedByte = RFD;

				// Check if the received byte matches the expected value
				if (receivedByte == TX_BYTE) {  // TX_BYTE is 0xAB in your case
						// Blink LED 3 times quickly to indicate success
						for (i = 0; i < 6; i++) {
								P1 ^= (1 << 1);  // Toggle LED on/off
								delay_approx(200);  // Short delay for quick blinking (adjust as needed)
						}
				} else {
						// Handle unexpected value blink 3 times slowly
						for (i = 0; i < 6; i++) {
								P1 ^= (1 << 1);  // Toggle LED on/off
								delay_approx(1000);  // Longer delay to differentiate
						}
				}
		} else {
						for (i = 0; i < 4; i++) { // even slower blink but 2 times for nothing received
								P1 ^= (1 << 1);  // Toggle LED on/off
								delay_approx(2000);  // Longer delay to differentiate
						}
		}

    // Return to IDLE mode
    RFST = 0x36;  // SIDLE

    return receivedByte;
}



int main() {
		IEN0 |= 0x80;  // Enable global interrupts (EA bit)
		RFIM |= 0x10;  // Enable TX interrupt
		CLKCON = 0x00;
		SLEEP = 0x04;  // Use high-speed crystal oscillator
	  delay_approx(1000);
		//while (!(SLEEP & 0x20));  // Wait for crystal oscillator to stabilize
	
	  // set P1.1 as output for LED
    P1DIR |= (1 << 1); // why does this turn it on?
	  P1 ^= (1 << 1); // toggle off
	
		//initializeRadio();
	  //RFST = 0x04; // set to IDLE to allow for radio registers to be set
	  PKTCTRL0  = SMARTRF_SETTING_PKTCTRL0;
	
		if (PKTCTRL0 != SMARTRF_SETTING_PKTCTRL0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
	
    CHANNR    = SMARTRF_SETTING_CHANNR;
		
		if (CHANNR != SMARTRF_SETTING_CHANNR) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
    FSCTRL1   = SMARTRF_SETTING_FSCTRL1;
		
		if (FSCTRL1 != SMARTRF_SETTING_FSCTRL1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
    FREQ2     = SMARTRF_SETTING_FREQ2;
		
		if (FREQ2 != SMARTRF_SETTING_FREQ2) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		
    FREQ1     = SMARTRF_SETTING_FREQ1;

		if (FREQ1 != SMARTRF_SETTING_FREQ1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		
    FREQ0     = SMARTRF_SETTING_FREQ0;
    
		if (FREQ0 != SMARTRF_SETTING_FREQ0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		MDMCFG4   = SMARTRF_SETTING_MDMCFG4;
    
		if (MDMCFG4 != SMARTRF_SETTING_MDMCFG4) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		MDMCFG3   = SMARTRF_SETTING_MDMCFG3;
		
		if (MDMCFG3 != SMARTRF_SETTING_MDMCFG3) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		//R0 = 0xBB;
    MDMCFG2   = SMARTRF_SETTING_MDMCFG2;
    
		if (MDMCFG2 != SMARTRF_SETTING_MDMCFG2) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		MDMCFG1   = SMARTRF_SETTING_MDMCFG1;
    
		if (MDMCFG1 != SMARTRF_SETTING_MDMCFG1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		MDMCFG0   = SMARTRF_SETTING_MDMCFG0;
    
				if (MDMCFG0 != SMARTRF_SETTING_MDMCFG0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		DEVIATN   = SMARTRF_SETTING_DEVIATN;
    
				if (DEVIATN != SMARTRF_SETTING_DEVIATN) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		MCSM0     = SMARTRF_SETTING_MCSM0;
    
				if (MCSM0 != SMARTRF_SETTING_MCSM0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		FOCCFG    = SMARTRF_SETTING_FOCCFG;
    
				if (FOCCFG != SMARTRF_SETTING_FOCCFG) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		BSCFG     = SMARTRF_SETTING_BSCFG;
    
				if (BSCFG != SMARTRF_SETTING_BSCFG) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		AGCCTRL2  = SMARTRF_SETTING_AGCCTRL2;
    
		if (AGCCTRL2 != SMARTRF_SETTING_AGCCTRL2) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		AGCCTRL1  = SMARTRF_SETTING_AGCCTRL1;
    
				if (AGCCTRL1 != SMARTRF_SETTING_AGCCTRL1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		AGCCTRL0  = SMARTRF_SETTING_AGCCTRL0;
    
				if (AGCCTRL0 != SMARTRF_SETTING_AGCCTRL0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		FREND1    = SMARTRF_SETTING_FREND1;
    
				if (FREND1 != SMARTRF_SETTING_FREND1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		FSCAL3    = SMARTRF_SETTING_FSCAL3;
    
				if (FSCAL3 != SMARTRF_SETTING_FSCAL3) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		FSCAL1    = SMARTRF_SETTING_FSCAL1;
    
				if (FSCAL1 != SMARTRF_SETTING_FSCAL1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		FSCAL0    = SMARTRF_SETTING_FSCAL0;
    
		if (FSCAL0 != SMARTRF_SETTING_FSCAL0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		TEST1     = SMARTRF_SETTING_TEST1;
		if (TEST1 != SMARTRF_SETTING_TEST1) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
    TEST0     = SMARTRF_SETTING_TEST0;
		
		if (TEST0 != SMARTRF_SETTING_TEST0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		PA_TABLE0 = SMARTRF_SETTING_PA_TABLE0;
    
		if (PA_TABLE0 != SMARTRF_SETTING_PA_TABLE0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		IOCFG0    = SMARTRF_SETTING_IOCFG0;
    
		if (IOCFG0 != SMARTRF_SETTING_IOCFG0) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		
		LQI       = SMARTRF_SETTING_LQI;

		if (LQI != SMARTRF_SETTING_LQI) {
				P1 |= (1 << 1);  // Toggle P1.1 (LED on)
				//delay_ms(1000);
				delay_approx(1000);
				P1 ^= (1 << 1);  // Toggle P1.1 (LED off)
				//delay_ms(1000);
		}
		
		// SFSTXON - 
		RFST = 0x00;
		
		// END OF SETUP
		
		
		//transmitByte();
	  //receiveByte();
		
		
		// EDIT BASED ON CURRENT TEST CONFIG
		
		// RX
		/*
		while (1) {
				unsigned char received = receiveByte();
				if (received == TX_BYTE) {
						// Blink LED on successful reception
						P1 ^= (1 << 1);  // Toggle LED
						delay_approx(500);  // Short delay
				}
		}
		*/
		// TX
		while (1) {
				transmitByte();
				delay_approx(1000);  // Send every second
		}
		
		//return 0;
}
