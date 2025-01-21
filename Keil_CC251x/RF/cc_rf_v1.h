

// refer to page 47 of the datasheet for RF
// radio register descriptions.

#include <Common_Shared/blink.h>

#ifndef CC_RF_V1_H
#define CC_RF_V1_H

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

void rfInit(void); 
void rfSend(unsigned short* rfTxBuf, unsigned short rfTxBufLength);
void rfReceive(unsigned short* rfTxBuf, unsigned short rfTxBufLength); 
void bufferClear(unsigned short* buffer, unsigned short bufferLen); 





#endif