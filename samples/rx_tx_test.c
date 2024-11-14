#include <CC2510.H>
#include <register_export.h>

#define TX_BYTE 0xAB // example byte to send

//void transmitByte(void);
//void receiveByte(void);

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
    while (RFIF & 0x02); // Wait until the TX flag clears, indicating TX is done

    // set CC2511 back to IDLE mode
    RFST = 0x36;  // SIDLE strobe command to enter IDLE mode
}

unsigned char receiveByte() {
    unsigned char receivedByte;

    // set CC2511 to RX mode
    RFST = 0x34;  // SRX strobe command to enter RX mode

    // wait for RX to complete or timeout
    while (!(RFIF & 0x01)); // wait for RX ready, checking RX interrupt flag

    // retrieve the received byte from RX FIFO
    receivedByte = RFD;

    // set CC2511 back to IDLE mode
    RFST = 0x36;  // SIDLE strobe command

    return receivedByte;
}


int main() {
		transmitByte();
		return 0;
}
