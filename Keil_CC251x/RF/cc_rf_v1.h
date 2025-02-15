

// refer to page 47 of the datasheet for RF
// radio register descriptions.

#ifndef CC_RF_V1_H
#define CC_RF_V1_H

// Headers
#include <Common_Shared/blink.h>
#include <Handlers/cc_packet_handlers.h>


// Defined variables 
/* SmartRF Low-Data rate settings */
#define SMARTRF_SETTING_PKTCTRL0    0x05
#define SMARTRF_SETTING_CHANNR      0x03
#define SMARTRF_SETTING_FSCTRL1     0x0A
#define SMARTRF_SETTING_FREQ2       0x65
#define SMARTRF_SETTING_FREQ1       0x0A
#define SMARTRF_SETTING_FREQ0       0xAA
#define SMARTRF_SETTING_MDMCFG4     0x76
#define SMARTRF_SETTING_MDMCFG3     0xA3
#define SMARTRF_SETTING_MDMCFG2     0x03
#define SMARTRF_SETTING_MDMCFG1     0x23
#define SMARTRF_SETTING_MDMCFG0     0x55
#define SMARTRF_SETTING_DEVIATN     0x45
#define SMARTRF_SETTING_MCSM0       0x14
#define SMARTRF_SETTING_FOCCFG      0x16
#define SMARTRF_SETTING_FSCAL1      0x00
#define SMARTRF_SETTING_FSCAL0      0x11
#define SMARTRF_SETTING_TEST1       0x31
#define SMARTRF_SETTING_TEST0       0x09
#define SMARTRF_SETTING_PA_TABLE0   0xFE
#define SMARTRF_SETTING_IOCFG0      0x06
#define SMARTRF_SETTING_LQI         0x7F

/* Stobe Commands (REFER TO DATASHEET PAGE: 185) */
#define SFSTXON			0x00 
#define SCAL				0x01
#define SRX 				0x02
#define STX					0x03
#define SIDLE 			0x04
#define SNOP 				0x05 // Can be anything other than previous defined strobe cmds 

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

// Variables
extern volatile rf_packet xdata rf_rx_buffer;			// Buffer for receive data 
extern volatile rf_packet xdata rf_tx_buffer; 		// Buffer for transmit data
extern uint8_t xdata rf_rx_index;									// Indexer for receive
extern uint8_t xdata rf_tx_index;									// Indexer for transmit
extern uint8_t xdata rf_rx_length; 								// Length of incoming packet
extern bit rf_rx_packet_complete;									// Flag for full packet received
extern uint8_t mode; 															// Current state of the system

// Interrupts 
void rfRxIsr(void);

// Initalizer
void rfInit(void); 

// Functions for RF controls
void rfSend(uint8_t* rfTxBuffer, uint16_t rfTxBufLen);
void rfReceive(uint8_t* rfRxBuffer, uint16_t rfRxBufLen); 



// Functions for state control 

// Function Getters

// Function Setters 






#endif