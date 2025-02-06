
#include <Peripherals/SPI/cc_spi_v1.h> 




void spiInit(void){
	
	// crystal stabalization check 
	
	// set pin directions 
	
	// set SPI regs 
	
	// set SPI interrupts if needed 
	
	// set flags
	
}


void spi1Send(uint16_t* spiOutBuffer, uint16_t spiOutBufLen){
	// Process through DMA?? 
	// less processing for CPU 
	
	// MOSI connect and flags 
	
	// sense
	
	// UD1BUF send (loop through inside here?) 
 	
	// clear flags / set where needed
	
	//end 
	
}


void spi1Receive(uint16_t* spiInBuffer, uint16_t spiInBufLen){
	// Process through DMA?? 
	// less processing for CPU 
	
	// MISO connect and flags
	
	// sense 
	
	// U1DBUF read (loop through inside here?) 
	
	// clear flags  / set where needed
	
	// end 
	
}



