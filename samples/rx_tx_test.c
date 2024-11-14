#include <CC2510.H>
#include <register_export.h>

#define TX_BYTE 0xAB // example byte to send

//void transmitByte(void);
//void receiveByte(void);
//void delay_approx(unsigned int ms);

void initializeRadio() {
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
    //TEST1     = SMARTRF_SETTING_TEST1;
    //TEST0     = SMARTRF_SETTING_TEST0;
    PA_TABLE0 = SMARTRF_SETTING_PA_TABLE0;
    IOCFG0    = SMARTRF_SETTING_IOCFG0;
    LQI       = SMARTRF_SETTING_LQI;
}

void delay_approx(unsigned int ms) {
    unsigned int i, j;

    for (i = 0; i < ms; i++)
        for (j = 0; j < 700; j++);
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

    // set CC2511 to RX mode
    RFST = 0x34;  // SRX strobe command to enter RX mode

    // wait for RX to complete or timeout
    while (!(RFIF & 0x01)); // wait for RX ready, checking RX interrupt flag

    // retrieve the received byte from RX FIFO
    receivedByte = RFD;

    // set CC2511 back to IDLE mode
    RFST = 0x36;  // SIDLE strobe command

    // check if the received byte matches the target byte
    if (receivedByte == TX_BYTE) {
        // set P1.1 as output for LED
        P1DIR |= (1 << 1);

        while (1) {
            P1 ^= (1 << 1);  // toggle P1.1 (LED on/off)
            //delay_ms(1000);
            delay_approx(1000);
        }
    }

    return receivedByte;
}

int main() {
    initializeRadio();
    // two controllers are needed for this test
    // only use the transmit function if you are on the TX controller
    transmitByte();
    // only use the receive function if you are on the RX controller
    //receiveByte();
    return 0;
}

