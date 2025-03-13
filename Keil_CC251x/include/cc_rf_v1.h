
// pages 183-225
// refer to page 47 of the datasheet for RF
// radio register descriptions.

#ifndef CC_RF_V1_H
#define CC_RF_V1_H

// Headers
#include <blink.h>
#include <cc_packet_handlers.h>


// Defined variables 

/* Stobe Commands (REFER TO DATASHEET PAGE: 185) */
#define SFSTXON			0x00 // Turn frequency synthesizer on
#define SCAL				0x01 // Manual calibration ON if FS_AUTOSCAL is set to `00`
#define SRX 				0x02 // Enable RX and go into receive mode. Calibrate if coming from IDEL
#define STX					0x03 // Enable TX and go into transmit mode. Calibrate if coming from IDLE
#define SIDLE 			0x04 // Enter IDLE state. Frequency Synthesizer turned off. 
#define SNOP 				0x55 // Can be anything other than previous defined strobe cmds 
#define MAX_LEN			0x40 // Can be changed for some thing greater. MAX_LEN = 64

// Variables
extern __xdata volatile packet rf_rx_buffer;			// Buffer for receive data 
extern __xdata volatile packet rf_tx_buffer; 		// Buffer for transmit data
extern uint8_t __xdata rf_rx_index;									// Indexer for receive
extern uint8_t __xdata rf_tx_index;									// Indexer for transmit
extern uint8_t __xdata rf_rx_length; 								// Length of incoming packet
extern __bit rf_rx_packet_complete;									// Flag for full packet received
//extern uint8_t mode; 															// Current state of the system
extern uint8_t max_len; 													// Max length of buffers (best for interrupts)

// Interrupts 
//void rfRxIsr(void) __interrupt(RFTXRX_VECTOR);
//void rfIsr(void) __interrupt(RFTXRX_VECTOR);
void rfRxOverflow(void);

// Initalizer
void rfInit(void); 

// Functions for RF controls
void rfSend(uint8_t* rfTxBuffer, uint16_t rfTxBufLen);
void rfReceive(uint8_t* rfRxBuffer, uint16_t rfRxBufLen); 
static void waitRfTxRxFlag(void); 
void rfStateMachine(uint8_t mode);



// Functions for state control 

// Function Getters

// Function Setters 


/*	rf interrupts: (page 185-187)
		1) RFTXRX: RX data ready or TX complete
			=> relate to RFD register 
			flag: TCON.RFRXRXIF
			- RX: Ready to be read when triggered 
			- TX: Ready for new byte to be written
						For TX, flag will not be asserted 
						before STX strobe
		2) RF: Refers to all other RF interrupts
		
		
		RF Data Register: 
		1) RFD: Can be seen as a 1 byte FIFO register 
			=> 	
			- Strobe Commands are important for when to
				read or write
				
				
		RF Data Transfers: (refer to DMA page 98) 
		- Can implement RX and TX FIFOs buffers in memory 
			and it is recommended to use DMA for data transfers 
			between buffers and RFD. 
		
		
		Strobe Commands: (page 185)
		1) SFSTXON:	Enable and calibrate freq synthesizer
		2) SCAL:	Calibrate freq synthesizer and turn it off.
				=> Can strobe from IDEL without manual calibration. 
		3) SRX: Enable RX
				=> Calibrate if coming from IDEL. 
		4) STX: 
			=> In IDEL: Enable TX 
			=> In RX state and CCA enabled: Only go to TX state 
				if channel is clear. 
		5) SIDLE: Enter IDEL state. 
		6) SNOP: No operation
		
		
		RF State Register (RFST): 
		1) Active states: STX and SRX 
		2) Manual calibration: SCAL
		3) Automaic calibration: MSCSMO.FS_AUTOCAL
			- Calibrate when going from IDLE to RX or TX
			- Calibrate when going from RX or TX to IDLE
			- Calibrate every fourth time when going from
				either RX or TX to IDLE automatically. 
		
		REFER TO PAGE 201 FOR STATE TRANSMISSION AND TRANSITION TIMES 
*/



#endif