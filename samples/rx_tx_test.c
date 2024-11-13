#include <CC2511.H>

// example byte to send
#define TX_BYTE 0xAB

void transmitByte(void);
void receiveByte(void);

int main() {
		transmitByte();
        //receiveByte();
		return 0;
}

void transmitByte() {
    // set CC2511 to TX mode
    RFST = 0x03;  // STX strobe command to enter TX mode

    // wait for TX to be ready
    while (!(RFIF & 0x02)); // check if TX is ready (or add a timeout for reliability)

    // load the byte into TX FIFO
    RFD = TX_BYTE;

    // start transmission
    RFST = 0x35;  // RFTX strobe command

    // wait for transmission to complete
    while (RFIF & 0x02); // wait until the TX flag clears, indicating TX is done

    // set CC2511 back to IDLE mode
    RFST = 0x36;  // SIDLE strobe command to enter IDLE mode
}

unsigned char receiveByte() {
    unsigned char receivedByte;

    // Set CC2511 to RX mode
    RFST = 0x34;  // SRX strobe command to enter RX mode

    // Wait for RX to complete or timeout
    while (!(RFIF & 0x01)); // Wait for RX ready, checking RX interrupt flag

    // Retrieve the received byte from RX FIFO
    receivedByte = RFD;

    // Set CC2511 back to IDLE mode
    RFST = 0x36;  // SIDLE strobe command

    return receivedByte;
}


