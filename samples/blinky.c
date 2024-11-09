#include <CC2510.H>
#define b00000001 0x01  // Bit 0
#define b00000010 0x02  // Bit 1
#define b00000100 0x04  // Bit 2
#define b00001000 0x08  // Bit 3
#define b00010000 0x10  // Bit 4
#define b00100000 0x20  // Bit 5
#define b01000000 0x40  // Bit 6
#define b10000000 0x80  // Bit 7

void delay(unsigned int ms);  // Declare delay function prototype
void delay_ms(unsigned int ms);

void main() {
		CLKCON = 0x00;  // Set to 32 MHz system clock with no division
	
    P1DIR |= b00000010;  // Set P1.0 as output (or the appropriate GPIO for the LED)

    while (1) {
        P1 ^= b00000010;  // Toggle P1.0 (LED on/off)
        //delay(1000); // Use the existing delay function if available
			  delay_ms(1000);
    }
}

// rough... better to use timers of course
void delay(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 700; j++);  // Adjust the 120 value as needed
}


void timer1_init() {
    T1CTL = 0x0E;     // Set Timer 1 in modulo mode with a prescaler
    T1CNTL = 0x00;    // Clear Timer 1 low byte
    T1CNTH = 0x00;    // Clear Timer 1 high byte
    T1CC0L = 0x80;    // Set lower byte of compare value (32,000 cycles)
    T1CC0H = 0x7D;    // Set upper byte of compare value
}


void delay_ms(unsigned int ms) {
    // Calculate the required timer count based on the clock frequency and `ms` value
    unsigned int timer_count = ms * 32000;  // 32 MHz clock means 32,000 cycles per ms

    // Set Timer 1 in modulo mode with no prescaling
    T1CTL = 0x0E;  // Timer 1 in modulo mode

    // Load the calculated value into the Timer 1 counter registers
    T1CNTL = (timer_count & 0xFF);           // Lower byte
    T1CNTH = ((timer_count >> 8) & 0xFF);    // Higher byte

    // Start the timer
    T1CTL |= 0x02;

    // Wait until timer overflow occurs (or alternative flag if available)
    while (!(T1CNTL & 0x80));

    // Stop the timer
    T1CTL &= ~0x02;
}


