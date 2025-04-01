
// Headers 
#include <../include/cc_dma_v1.h>
#include <../include/cc_uart_v1.h>
#include <../include/cc_spi_v1.h>
#include <../include/cc_rf_v1.h>
#include <../include/cc_packet_handlers.h>

// DMA channel configurations variables 
__xdata dma_cfg dma_channels[5]; 
const dma_cfg dma_init_val = {{0x0000}, {0x0000}, {0x00}, {0x00}, {0x00}, {0x00}};

// Interrupts 
void dmaIsr(void) __interrupt(DMA_VECTOR){
	//uint8_t msg[] = "DMA ISR\n";
	// uint8_t msg1[] = "DMA IF\n";
	//uart0SendUnstructured(msg, 8); // For testing 

	// Handle for either UART TX (DMAIF3 = 0x08), RFTX (DMAIF2 = 0x04), RFTX (DMAIF1 = 0x02), UART RX (DMAIF0 = 0x01)
	if((DMAIRQ & DMAIF0)){ // UART RX

		// UART Packet Complete 
		//uartPacketHandler(&uart_rx_buffer); 
		DMAIRQ &= ~(0x01);
		URX0IF = 0;
		uart_rx_packet_complete = 1;
		//uart0Send(uart_rx_buffer.rawPayload, uart_rx_buffer.fields.length); // for testing

	}
	else if((DMAIRQ & DMAIF1)){ // RF RX
	
		// RF Packet Complete
		//rfPacketHandler(&rf_rx_buffer);
		DMAIRQ &= ~(0x02);
		RFTXRXIF = 0;
		rf_rx_packet_complete = 1;
		//uart0Send(rf_rx_buffer.rawPayload, rf_rx_buffer.fields.length); // for testing
		//dmaAbort(1);
		
	}else if ((DMAIRQ & DMAIF2)){ // RF TX
		
		//uart0Send(rf_tx_buffer.rawPayload, rf_tx_buffer.fields.length); // for testing
		DMAIRQ &= ~(0x04);
		RFTXRXIF = 0;
		rf_tx_packet_complete = 1;
		dmaAbort(2);
		setDmaArm(1);
		//delayMs(1);
		//RFST = SIDLE;  //causes bad sends 
		//mode = SIDLE;
		
	}else if ((DMAIRQ & DMAIF3)){ // UART TX
		// uart0Send(msg1, 7);
		DMAIRQ &= ~(0x08);
		UTX0IF = 0;
		uart_tx_packet_complete = 1; 
		dmaAbort(3);
		setDmaArm(0);
		//U0CSR |= 0x40; // turn on receiver for RX
		

	}else if ((DMAIRQ & DMAIF4)){ // SPI RX/TX
		// uart0Send(msg1, 7);
		DMAIRQ &= ~(0x10);
		UTX1IF = 0;
		URX1IF = 0;
		dmaAbort(4);
	}
	
	DMAIF = 0;
}

// Initializer 
void dmaInit(void){
	
	// Variables 
	uint8_t i; 
	
	// Allocate memory in xdata for DMA channels and set struct to 0x00
	for(i = 0; i < 5; i++){ // init channels 
		
		dma_channels[i] = dma_init_val; 
	}
	
	// Set addresses for DMA configuration registers
	DMA0CFGL = (uint8_t)((uint16_t)&dma_channels[0] & 0x00FF);  // Low byte of address for channel 0
  	DMA0CFGH = (uint8_t)(((uint16_t)&dma_channels[0]) >> 8);  	// High byte of address for channel 0
	DMA1CFGL = (uint8_t)((uint16_t)&dma_channels[1] & 0x00FF);  // Low byte of address for channel 1-4
  	DMA1CFGH = (uint8_t)(((uint16_t)&dma_channels[1]) >> 8);  	// High byte of address for channel 1-4
	
	// Setup DMA for UART receive Channel 0
	/* Have data transfers and stores happen over DMA to allow CPU to cycle seperately. Use X_U0DBUF */
	dma_channels[0].srcAddrHi = (uint8_t)((uint16_t)&X_U0DBUF >> 8); // High byte of source address, XDATA U0DBUF (Byte 0)
	dma_channels[0].srcAddrLo = (uint8_t)((uint16_t)&X_U0DBUF & 0x00FF); // Low byte of source address XDATA U0DBUF (Byte 1) 
	dma_channels[0].dstAddrHi = (uint8_t)((uint16_t)&uart_rx_buffer.rawPayload >> 8); // High byte of destination address, XDATA U0DBUF (Byte 2) 
	dma_channels[0].dstAddrLo = (uint8_t)((uint16_t)&uart_rx_buffer.rawPayload & 0x00FF); // Low byte of destination address XDATA U0DBUF (Byte 3) 
	dma_channels[0].byte4 = 0x20; // VLen = 001 (transfer n + 1 Bytes), LEN[12:8] = 00000 (Byte 4)
	dma_channels[0].byte5 = 0x40; // LEN[7:0] = 00111111 => 63 bytes(Byte 5); 
	dma_channels[0].byte6 = 0x4E; // Word = 0 (8-bits), tmod = 10 (repeated single), trig = 01110 (RX complete trigger) (Byte 6)
	dma_channels[0].byte7 = 0x1A; //SrcInc = 00 (no increment), DstInc = 01 (increment by 1 byte), IRQMASK = 1 (generates interrupt), M8 = 0 (8bits), prioirty = 10 (Priority access)
	

	// Setup DMA for SPI 
	/* Passing and storing from SPI over DMA, shouldn't require CPU  to process data */

	
	// Setup DMA for RF Receive Channel 1
	/* Have stored to set memory address so that if CPU interrupts DMA is passing seperately */ 
	dma_channels[1].srcAddrHi = (uint8_t)((uint16_t)&X_RFD >> 8); // High byte of source address, XDATA RFD (Byte 0)
	dma_channels[1].srcAddrLo = (uint8_t)((uint16_t)&X_RFD & 0x00FF); // Low byte of source address XDATA RFD (Byte 1) 
	dma_channels[1].dstAddrHi = (uint8_t)((uint16_t)&rf_rx_buffer.rawPayload[0] >> 8); // High byte of destination address, XDATA RFRX Buffer (Byte 2) 
	dma_channels[1].dstAddrLo = (uint8_t)((uint16_t)&rf_rx_buffer.rawPayload[0] & 0x00FF); // Low byte of destination address XDATA RFRX BufferByte 3) 
	dma_channels[1].byte4 = 0x20; // VLen = 001 (transfer n + 1 Bytes), LEN[12:8] = 00000 (Byte 4)
	dma_channels[1].byte5 = 0x40; // LEN[7:0] = 01000000 => 64 bytes(Byte 5); 
	dma_channels[1].byte6 = 0x53; // Word = 0 (8-bits), tmod = 10 (repeated single), trig = 10011 (RF TX/RX trigger) (Byte 6)
	dma_channels[1].byte7 = 0x1A; //SrcInc = 00 (no increment), DstInc = 01 (increment by 1 byte), IRQMASK = 1 (generates interrupt), M8 = 0 (8bits), prioirty = 10 (Priority access)
	

	// Setup DMA for RF Transmit Channel 2
	dma_channels[2].srcAddrHi = (uint8_t)((uint16_t)&rf_tx_buffer.rawPayload >> 8); // High byte of source address, XDATA RFTX (Byte 0)
	dma_channels[2].srcAddrLo = (uint8_t)((uint16_t)&rf_tx_buffer.rawPayload & 0x00FF); // Low byte of source address XDATA RFTX (Byte 1) 
	dma_channels[2].dstAddrHi = (uint8_t)((uint16_t)&X_RFD >> 8); // High byte of destination address, XDATA RFD (Byte 2) 
	dma_channels[2].dstAddrLo = (uint8_t)((uint16_t)&X_RFD & 0x00FF); // Low byte of destination address XDATA RFD (Byte 3) 
	dma_channels[2].byte4 = 0x20; // VLen = 001 (transfer n + 1 Bytes), LEN[12:8] = 00000 (Byte 4)
	dma_channels[2].byte5 = 0x40; // LEN[7:0] = 01000000 => 64 bytes(Byte 5); 
	dma_channels[2].byte6 = 0x53; // Word = 0 (8-bits), tmod = 10 (repeated single), trig = 10011 (RF TX/RX trigger) (Byte 6)
	dma_channels[2].byte7 = 0x4A; //SrcInc = 01 (increment by 1 Byte), DstInc = 00 (no increment), IRQMASK = 1 (generates interrupt), M8 = 0 (8bits), prioirty = 10 (Priority access)
	

	// Setup DMA for UART Transmit Channel 3
	dma_channels[3].srcAddrHi = (uint8_t)((uint16_t)&uart_tx_buffer.rawPayload[0] >> 8); // High byte of source address, XDATA RFTX (Byte 0)
	dma_channels[3].srcAddrLo = (uint8_t)((uint16_t)&uart_tx_buffer.rawPayload[0] & 0x00FF); // Low byte of source address XDATA RFTX (Byte 1) 
	dma_channels[3].dstAddrHi = (uint8_t)((uint16_t)&X_U0DBUF >> 8); // High byte of destination address, XDATA RFD (Byte 2) 
	dma_channels[3].dstAddrLo = (uint8_t)((uint16_t)&X_U0DBUF & 0x00FF); // Low byte of destination address XDATA RFD (Byte 3) 
	dma_channels[3].byte4 = 0x20; // VLen = 001 (transfer n + 1 Bytes), LEN[12:8] = 00000 (Byte 4)
	dma_channels[3].byte5 = 0x40; // LEN[7:0] = 01000000 => 64 bytes(Byte 5); 
	dma_channels[3].byte6 = 0x4F; //0x4F Word = 0 (8-bits), tmod = 00 (single), trig = 01111 (UART TX trigger) (Byte 6)
	dma_channels[3].byte7 = 0x4A; //SrcInc = 01 (increment by 1 Byte), DstInc = 00 (no increment), IRQMASK = 1 (generates interrupt), M8 = 0 (8bits), prioirty = 10 (Priority access)


	// ARM DMA Channel
	/* Takes 9 system clocks for DMA config to be set */
	DMAARM |= 0x0B; // 0x03: ARM DMA channel 0 (UART), DMA channel 1 (RFRX); 0x0B ARM DMA Channel 0 (UART UART RX), DMA Channel 1 (RFRX), DMA Channel 3 (UART TX)
	//delayMs(1);// allow channels to ARM.
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	// Enable interrupts 
	IEN1 |= 0x01; 
	DMAIF = 0;

}

// Functions
volatile void dmaAbort(uint8_t channel){
	
	// Abort/Disable DMA channel
	DMAARM = (0x80 | (1 <<channel)); // Channel Abort + bit set to abort. 
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");
	
	
}

volatile void dmaRequest(uint8_t channel){
	
	// Request DMA transfer for channel
	DMAREQ = (1 << channel); // Channel Request + bit set to request
}

// Setters 
volatile void setDmaArm(uint8_t channel){
		
	// Arm DMA channel
	DMAARM |= (1 << channel); 
	__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");__asm__("NOP");	
}

void setDmaConfig(uint8_t channel, __xdata uint8_t *srcAddr, __xdata uint8_t *dstAddr, uint8_t srcInc, uint8_t dstInc, uint8_t trigger, uint8_t tMode) {


	// Abort any ongoing DMA transfer on this channel
	dmaAbort(channel);

	// Set source and destination addresses
	dma_channels[channel].srcAddrHi = (uint8_t)((uint16_t)srcAddr >> 8);
	dma_channels[channel].srcAddrLo = (uint8_t)((uint16_t)srcAddr & 0x00FF);
	dma_channels[channel].dstAddrHi = (uint8_t)((uint16_t)dstAddr >> 8);
	dma_channels[channel].dstAddrLo = (uint8_t)((uint16_t)dstAddr & 0x00FF);

	// Set VLEN and transfer length
	// Using VLEN=001 (transfer n+1 bytes) for consistency with your existing code
	dma_channels[channel].byte4 = 0x20; // VLen = 001, LEN[12:8]
	dma_channels[channel].byte5 = 0x40;                  // LEN[7:0]

	// Set word size, transfer mode, and trigger
	// Word size=0 (8-bit transfers)
	dma_channels[channel].byte6 = (((tMode & 0x03) >> 5) | (trigger & 0x1F));

	// Set source/destination increment, IRQ mask, and priority
	// IRQMask=1 (generates interrupt), M8=0 (8-bit transfer), priority=10 (high)
	dma_channels[channel].byte7 = ((srcInc & 0x03) << 6) | ((dstInc & 0x03) << 4) | 0x0A;

	//setDmaArm(channel);
}



