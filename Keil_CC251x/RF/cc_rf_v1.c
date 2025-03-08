
// Headers
#include <RF/cc_rf_v1.h>
#include <Common_Shared/blink.h>
#include <peripherals/UART/cc_uart_v1.h>

// Variables 
xdata volatile rf_packet rf_rx_buffer;
xdata volatile rf_packet rf_tx_buffer;
uint8_t xdata rf_rx_index;
uint8_t xdata rf_tx_index;
uint8_t xdata rf_rx_length;
bit rf_rx_packet_complete;
uint8_t mode;
uint8_t max_len = 64; // Can change. To change, make sure to update inside packet Handler. 



// Interrupts 
//void rfRxIsr(void) interrupt RFTXRX_VECTOR{
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

//void rfIsr(void) interrupt RFTXRX_VECTOR { 
//	/* Disable when testing RF */
//	// Variables
//	RFTXRXIF = 0;
//	rf_rx_buffer.rawPayload[0] = RFD; 
//	blink();
//		

//}

void rfOverflow(void) interrupt RF_VECTOR {
	uint8_t msg[] = "Overflow\n";

	RFST = SIDLE;
	mode = SIDLE;
	uart0Send(msg, 9);
}


// Initialization 
void rfInit(void){
	
	// Initialize variables 
	rf_rx_index = 0;
	rf_tx_index = 0;
	
	// Setup Registers
	/*  Values taken straight from Smart RF Studio (Data Rate = 9600)*/
	
	IOCFG2 = 0x2E; 
	IOCFG1 = 0x00;
	IOCFG0 = 0x00;  // 0x06; for debugging when low byte set.
 	SYNC1 = 0xD3;
	SYNC0 = 0x91;
	PKTLEN = 0xFF; // 0xFF
	PKTCTRL1 = 0x00; // 0x04 = Append_Status; if we include, it messes with RX
	PKTCTRL0 = 0x05; // 0x05 = CRC enabled with variable length , 0x01 = variable length
	ADDR = 0x00;
	CHANNR = 0x00;
	FSCTRL1 = 0x0A;
	FSCTRL0 = 0x00;
	FREQ2 = 0x65;
	FREQ1 = 0x60;
	FREQ0 = 0x00;	
	MDMCFG4 =	0x76;
	MDMCFG3 =	0xA3;	
	MDMCFG2 =	0x03;	//00 = no preabmle and sync, 0x03 = 30/32 preambe and sync, 0x02 = 16/16 preamble and sync
	MDMCFG1 =	0x23;	
	MDMCFG0 =	0x11;
	DEVIATN =	0x45;	
	MCSM2 = 0x07;
 	MCSM1 = 0x3E; // 0x30; RXOFF_MODE = 11 (Stay in RX), TXOFF_MODE == 10 (Stay in TX); C
	MCSM0 =	 0x14;
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
	//PA_TABLE7 =
	//PA_TABLE6 =
	//PA_TABLE5 =
	//PA_TABLE4 =
	//PA_TABLE3 = 
	//PA_TABLE2 = 
	//PA_TABLE1 = 
	PA_TABLE0 =	0xFE;	
	//PARTNUM
	//VERSION
	//FREQEST = 
	LQI = 0x80; 
	//RSSI = 
	//MARCSTATE
	//PKTSTATUS
	//VCO_VC_DAC
	
	// Interrupt enables 
	RFTXRXIE = 1;		// RFD TX and RX
	IEN2 |= 0x1; // General RF interrupts 
	
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
	
	
	// Turn on frequency synthesizer if not on already
	RFST = SFSTXON; 
	mode = SFSTXON; 
	delayMs(1); 
	
	// Set strobe command for transmit
	RFST = STX; 
	mode = STX; // bookkeeping with global var
	delayMs(1); // necessary evil
	
	// Wait for flag to be set
	//waitRfTxRxFlag();
	//blink();
	//write the first byte (packet length)
	RFD = rfTxBuffer[0];
	blink();
	
	// send the rest of the packet
	for(i = 1; i < rfTxBufLen; i++){
		
		//RFST = STX;	
		delayMs(1);
		//waitRfTxRxFlag(); // wait for flag to be set
		RFD = rfTxBuffer[i];
		//blink();
	
	}
	
	// Calibrate if FS_AUTOSCAL set to `00`
	//RFST = SCAL;
	//mode = SCAL;
	//delayMs(1); // delay 1 MS to allow state transition 
	
	// Return to idle state 
	RFST = SIDLE;
	mode = SIDLE;
	delayMs(1); 
	
	
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

	while(!RFTXRXIF){
		// Wait until flag is set
		// could add timer
		//blink();
	}
	//blink();
	RFTXRXIF = 0;
}


/**************************************************************************************************
 * rf.c
 *
 * Example RF driver for the CC2511/CC2510 using the Keil C51 compiler.
 *
 * This implementation uses the following strobe commands (per datasheet page 185):
 *    1) SFSTXON: Enable and calibrate the frequency synthesizer.
 *    2) SCAL:    Calibrate the frequency synthesizer and turn it off.
 *               (Can be strobed from IDLE without manual calibration.)
 *    3) SRX:     Enable RX (will calibrate if coming from IDLE).
 *    4) STX:     In IDLE: Enable TX.
 *               In RX (with CCA enabled): Only go to TX if the channel is clear.
 *    5) SIDLE:   Enter IDLE state.
 *    6) SNOP:    No operation.
 *
 * Additionally, extra strobes (SFRX and SFTX) are defined for flushing the RX/TX FIFOs.
 *
 * Author: [Your Name]
 * Date:  [Date]
 **************************************************************************************************/


///*--------------------------------------------------------------------------------------------------
// *  Define Strobe Command Macros (per datasheet page 185)
// *--------------------------------------------------------------------------------------------------*/
//#define SFSTXON   0x00   	// Enable and calibrate frequency synthesizer
//#define SCAL      0x01  	// Calibrate frequency synthesizer and then turn it off
//#define SRX       0x02	  // Enable RX (calibrates if coming from IDLE)
//#define STX       0x03   	// Enable TX (if in IDLE; in RX+CCA, go to TX only if clear)
//#define SIDLE     0x04  	// Enter IDLE state
//#define SNOP      0xFF  	// No operation

///* Additional strobe commands used in this example */
//#define SFRX      0x3A   // Flush RX FIFO
//#define SFTX      0x3B   // Flush TX FIFO

///* Optional: Define a reset strobe if needed (not part of your provided list) */
//// #define SRES      0x30   // Reset chip

///*--------------------------------------------------------------------------------------------------
// *  Define Default Radio Configuration Values
// *
// *  These sample values configure the radio for a 2.4 GHz, GFSK, 115200-baud system.
// *  IMPORTANT: Verify and adjust these values against your datasheet and application needs.
// *--------------------------------------------------------------------------------------------------*/
//#define DEFAULT_SYNC1     0xD3    // Sync word high byte
//#define DEFAULT_SYNC0     0x91    // Sync word low byte
//#define DEFAULT_PKTLEN    0xFF    // Maximum packet length (variable-length mode)
//#define DEFAULT_PKTCTRL1  0x04    // Packet automation control: no address check
//#define DEFAULT_PKTCTRL0  0x45    // Variable-length packet, CRC enabled, whitening on
//#define DEFAULT_ADDR      0x00    // Device address (modify as needed)
//#define DEFAULT_CHANNR    0x00    // Channel number (0 = base frequency)

//#define DEFAULT_FSCTRL1   0x06    // Frequency synthesizer control (example value)
//#define DEFAULT_FSCTRL0   0x00    // Frequency synthesizer control (example value)

///* Frequency control words for ~2.4 GHz operation (example values) */
//#define DEFAULT_FREQ2     0x5C    
//#define DEFAULT_FREQ1     0x80    
//#define DEFAULT_FREQ0     0x00    

///* Modem Configuration Registers (MDMCFGx) */
//#define DEFAULT_MDMCFG4   0x2D    // Channel bandwidth and data rate exponent (example)
//#define DEFAULT_MDMCFG3   0x3B    // Channel spacing mantissa (example)
//#define DEFAULT_MDMCFG2   0x73    // Modulation format (GFSK), etc. (example)
//#define DEFAULT_MDMCFG1   0x23    // FEC, channel filter, etc. (example)
//#define DEFAULT_MDMCFG0   0x11    // Data rate mantissa (example)
//#define DEFAULT_DEVIATN   0xB1    // Modem deviation setting (example)

///* Main Radio Control State Machine Configuration (MCSM) */
//#define DEFAULT_MCSM2     0x07    // RX timeouts, etc. (example)
//#define DEFAULT_MCSM1     0x30    // Auto-calibration and RX/TX transition (example)
//#define DEFAULT_MCSM0     0x18    // TX/RX FIFO thresholds, etc. (example)

///* Frequency Offset Compensation and Bit Synchronization */
//#define DEFAULT_FOCCFG    0x16    // Frequency offset compensation configuration (example)
//#define DEFAULT_BSCFG     0x6C    // Bit synchronization configuration (example)

///* AGC (Automatic Gain Control) Settings */
//#define DEFAULT_AGCCTRL2  0x43    // AGC control (example)
//#define DEFAULT_AGCCTRL1  0x40    // AGC control (example)
//#define DEFAULT_AGCCTRL0  0x91    // AGC control (example)

///* Front End RX/TX Configuration */
//#define DEFAULT_FREND1    0x56    // Front end RX configuration (example)
//#define DEFAULT_FREND0    0x10    // Front end TX configuration (example)

///* Frequency Synthesizer Calibration */
//#define DEFAULT_FSCAL3    0xE9    // Calibration (example)
//#define DEFAULT_FSCAL2    0x2A    // Calibration (example)
//#define DEFAULT_FSCAL1    0x00    // Calibration (example)
//#define DEFAULT_FSCAL0    0x1F    // Calibration (example)

///* Power Amplifier Table (set TX output power) */
//#define DEFAULT_PA_TABLE0 0xC0    // Example: highest output power
//#define DEFAULT_PA_TABLE1 0x00
//#define DEFAULT_PA_TABLE2 0x00
//#define DEFAULT_PA_TABLE3 0x00
//#define DEFAULT_PA_TABLE4 0x00
//#define DEFAULT_PA_TABLE5 0x00
//#define DEFAULT_PA_TABLE6 0x00
//#define DEFAULT_PA_TABLE7 0x00

///* Optional: Variables for received packet handling */
//volatile bit packetReceived = 0;            // Flag set by RF ISR when a packet is received
//volatile unsigned char xdata rxBuffer[256];         // Buffer to store received packet data
//volatile unsigned char xdata rxLength = 0;          // Length of the received packet

///*--------------------------------------------------------------------------------------------------
// *  Function Prototypes
// *--------------------------------------------------------------------------------------------------*/
//void rfInitT(void);
//void rfTransmit(unsigned char* d, unsigned char length);
//void rfEnterRX(void);
//void processReceivedPacket(void);


///*--------------------------------------------------------------------------------------------------
// *  RF Interrupt Service Routine
// *
// *  This ISR is triggered on RF events (for example, when a packet is received).
// *  It reads the radio’s state (via MARCSTATE), and if a complete packet is detected,
// *  reads the packet from the RX FIFO.
// *
// *  The RF interrupt vector is defined as vector 16 (see IOCC2510.h).
// *--------------------------------------------------------------------------------------------------*/
//void rfISR(void) interrupt 16
//{
//    unsigned char marcState;

//    /* Read the main radio state. Reading MARCSTATE may clear the interrupt flag. */
//    marcState = MARCSTATE;

//    /* In this simple example, we assume that when MARCSTATE equals 0x0F the RX FIFO
//       contains a complete packet. (Check your datasheet for the correct state code.) */
//    if(marcState == 0x0F)
//    {
//        unsigned char i, len;

//        /* Read the first byte from the RX FIFO as the packet length */
//        len = RFD;
//        rxLength = len;

//        /* Read each byte from the RX FIFO */
//        for(i = 0; i < len; i++)
//        {
//            rxBuffer[i] = RFD;
//        }

//        /* (Optional) Read additional appended status bytes (e.g. RSSI, LQI, CRC OK)
//           if your packet format requires them. */

//        /* Set the flag indicating that a packet has been received */
//        packetReceived = 1;
//    }

//    /* If additional interrupt flag clearing is needed, add it here. */
//}

///*--------------------------------------------------------------------------------------------------
// *  rfInit()
// *
// *  Initializes the RF core by writing configuration registers, calibrating the synthesizer,
// *  flushing the FIFOs, and setting the radio to the IDLE state.
// *
// *  Note: Although many RF examples begin with a reset (SRES), the strobe list provided here
// *  does not include SRES. If a reset is desired, define and use SRES as needed.
// *--------------------------------------------------------------------------------------------------*/
//void rfInitT(void)
//{
//    /* Optionally, reset the radio if required.
//       Uncomment the line below if SRES is defined and you wish to reset.
//    */
//    // RFST = SRES;
//    // delay_ms(1);  // Allow time for the reset to complete
//	
//		
//	
//    /* Write radio configuration registers */
//    SYNC1     = DEFAULT_SYNC1;        // Set sync word high byte
//    SYNC0     = DEFAULT_SYNC0;        // Set sync word low byte
//    PKTLEN    = DEFAULT_PKTLEN;       // Maximum packet length (variable-length mode)
//    PKTCTRL1  = DEFAULT_PKTCTRL1;     // Packet automation control
//    PKTCTRL0  = DEFAULT_PKTCTRL0;     // Variable-length, CRC enabled, whitening on
//    ADDR      = DEFAULT_ADDR;         // Device address
//    CHANNR    = DEFAULT_CHANNR;       // Channel number

//    /* Frequency synthesizer settings */
//    FSCTRL1   = DEFAULT_FSCTRL1;
//    FSCTRL0   = DEFAULT_FSCTRL0;
//    FREQ2     = DEFAULT_FREQ2;
//    FREQ1     = DEFAULT_FREQ1;
//    FREQ0     = DEFAULT_FREQ0;

//    /* Modem configuration for data rate and modulation */
//    MDMCFG4   = DEFAULT_MDMCFG4;
//    MDMCFG3   = DEFAULT_MDMCFG3;
//    MDMCFG2   = DEFAULT_MDMCFG2;
//    MDMCFG1   = DEFAULT_MDMCFG1;
//    MDMCFG0   = DEFAULT_MDMCFG0;
//    DEVIATN   = DEFAULT_DEVIATN;

//    /* Main Radio Control State Machine configuration */
//    MCSM2     = DEFAULT_MCSM2;
//    MCSM1     = DEFAULT_MCSM1;
//    MCSM0     = DEFAULT_MCSM0;

//    /* Frequency Offset Compensation and Bit Synchronization */
//    FOCCFG    = DEFAULT_FOCCFG;
//    BSCFG     = DEFAULT_BSCFG;

//    /* AGC settings */
//    AGCCTRL2  = DEFAULT_AGCCTRL2;
//    AGCCTRL1  = DEFAULT_AGCCTRL1;
//    AGCCTRL0  = DEFAULT_AGCCTRL0;

//    /* Front end configuration */
//    FREND1    = DEFAULT_FREND1;
//    FREND0    = DEFAULT_FREND0;

//    /* Frequency synthesizer calibration */
//    FSCAL3    = DEFAULT_FSCAL3;
//    FSCAL2    = DEFAULT_FSCAL2;
//    FSCAL1    = DEFAULT_FSCAL1;
//    FSCAL0    = DEFAULT_FSCAL0;

//    /* PA Table: set the TX output power */
//    PA_TABLE0 = DEFAULT_PA_TABLE0;
//    PA_TABLE1 = DEFAULT_PA_TABLE1;
//    PA_TABLE2 = DEFAULT_PA_TABLE2;
//    PA_TABLE3 = DEFAULT_PA_TABLE3;
//    PA_TABLE4 = DEFAULT_PA_TABLE4;
//    PA_TABLE5 = DEFAULT_PA_TABLE5;
//    PA_TABLE6 = DEFAULT_PA_TABLE6;
//    PA_TABLE7 = DEFAULT_PA_TABLE7;

//    /* (Optional) Configure GDO pins if required by your application.
//       For example, you may want to set:
//         IOCFG2 = <value>;  // GDO2 function
//         IOCFG1 = <value>;  // GDO1 function
//         IOCFG0 = <value>;  // GDO0 function
//    */

//    /* Calibrate the frequency synthesizer by issuing the SCAL strobe.
//       (SCAL both calibrates and turns off the synthesizer; it can be issued from IDLE.)
//    */
//    RFST = SCAL;
//    delayMs(1);  // Wait for calibration to complete


//    /* Enable RF interrupts (e.g., the RF TX/RX done interrupt) */
//    RFTXRXIE = 1;

//    /* Put the radio into the IDLE state */
//    RFST = SIDLE;
//}

///*--------------------------------------------------------------------------------------------------
// *  rfEnterRX()
// *
// *  Prepares the radio for reception by flushing the RX FIFO and issuing the SRX strobe.
// *--------------------------------------------------------------------------------------------------*/
//void rfEnterRX(void)
//{
//    RFST = SRX;   // Issue the strobe to enter RX mode (will auto-calibrate if coming from IDLE)
//}

///*--------------------------------------------------------------------------------------------------
// *  rfTransmit()
// *
// *  Transmits a packet via the TX FIFO.
// *
// *  Parameters:
// *    data   - Pointer to the data to transmit.
// *    length - Number of data bytes.
// *
// *  Note:
// *    This routine is blocking; it waits until transmission is complete by polling the
// *    radio’s state. In a production system you may wish to add a timeout or use interrupts.
// *--------------------------------------------------------------------------------------------------*/
//void rfTransmit(unsigned char* d, unsigned char length)
//{
//    unsigned char i;

//    /* Flush the TX FIFO before loading new data */
//    RFST = SFTX;

//    /* In variable-length packet mode, the first byte is the packet length */
//    RFD = length;

//    /* Load the packet data into the TX FIFO */
//    for(i = 0; i < length; i++)
//    {
//        RFD = d[i];
//    }

//    /* Issue the STX strobe to start transmission.
//       (Depending on the current state the radio will either immediately enter TX
//        or, if in RX with CCA enabled, will check the channel first.)
//    */
//    RFST = STX;

//    /* Wait for transmission to complete.
//       Here we block until MARCSTATE returns to the IDLE state.
//       (Check your datasheet for the proper state code.)
//    */
//    while(MARCSTATE != 0x01)
//    {
//        // Optionally implement a timeout here.
//    }
//}


///*--------------------------------------------------------------------------------------------------
// *  processReceivedPacket()
// *
// *  Processes a packet that has been received (stored in rxBuffer, with length rxLength).
// *  This function is a placeholder for your packet-processing code.
// *--------------------------------------------------------------------------------------------------*/
//void processReceivedPacket(void)
//{
//    if(packetReceived)
//    {
//        /* Insert your packet-handling code here. For example, verify CRC, parse commands, etc.
//           The received data is available in rxBuffer with rxLength bytes.
//        */

//        /* Clear the flag when finished processing */
//        packetReceived = 0;
//    }
//}

/*--------------------------------------------------------------------------------------------------
 *  main()
 *
 *  The main routine initializes the RF module, enables interrupts, enters RX mode, and then
 *  loops forever checking for received packets.
 *--------------------------------------------------------------------------------------------------*/
//void main(void)
//{
//    /* (Optional) Disable watchdog timer and configure system clocks as needed */

//    /* Initialize the RF subsystem */
//    rfInit();

//    /* Enable global interrupts */
//    EA = 1;

//    /* Enter RX mode so that incoming packets will be received */
//    rfEnterRX();

//    while(1)
//    {
//        /* Check if a packet has been received and process it */
//        if(packetReceived)
//        {
//            processReceivedPacket();
//        }
//        
//        /* (Optional) To transmit a test packet periodically, uncomment the block below:
//        {
//            unsigned char testPacket[5] = { 'H', 'e', 'l', 'l', 'o' };
//            rfTransmit(testPacket, 5);
//            delay_ms(1000);
//        }
//        */
//    }
//}
