#include <CC2510.H>

#define CYCLES_PER_MS 32000

void delay_ms(unsigned int ms);
void timer1_init(void);

void main() {
    // CLK CONTROL REG
    // 7,6 --> OSC32K,OSC   --> set (32.768 kHz) crystal oscillator
    // 5:3 --> TICKSPD[2:0] --> set f_ref = 26 MHz for timers
    // 2:0 --> CLKSPD[2:0]  --> set f_ref = 26 MHz for clock
	CLKCON = 0x00;

    timer1_init()
	
    // set P1.1 as output for LED
    P1DIR |= (1 << 1);

    while (1) {
        P1 ^= (1 << 1);  // Toggle P1.1 (LED on/off)
	    delay_ms(1000);
    }
}

void timer1_init() {
    // TIMER1 CONTROL & STATUS REG
    // 7,6,5 --> CH2IF, CH1IF, CH0IF --> clear timer1 all channel interrupt flags
    // 4     --> OVFIF               --> clear timer1 ctr overflow interrupt flag
    // 3:2   --> DIV[1:0]            --> set tick frequency / 128
    // 1:0   --> MODE[1:0]           --> set modulo, repeatedly count from 0x0000 to T1CC0
    T1CTL  = 0x0E;  // 0000 1110
                    
    // writing anything to these resets them to 0x00
    // TIMER1 COUNTER HIGH BYTE REG
    T1CNTH = 0x00;
    // TIMER1 COUNTER LOW BYTE REG
    T1CNTL = 0x00;
    
    // TIMER1 CHANNEL0 CAPTURE/COMPARE VALUE HIGH
    T1CC0H = 0x7D;  // 0111 1101
    // TIMER1 CHANNEL0 CAPTURE/COMPARE VALUE LOW
    T1CC0L = 0x80;  // 1000 0000
                    // --> 32,000 ticks
}


void delay_ms(unsigned int ms) {
    unsigned int timer_count = ms * CYCLES_PER_MS;

    // set timer 1 in modulo mode with no prescaling
    T1CTL = 0x0E;  // timer 1 in modulo mode

    // load the calculated value into the Timer 1 counter registers
    T1CNTL = (timer_count & 0xFF);           // lower byte
    T1CNTH = ((timer_count >> 8) & 0xFF);    // higher byte

    // start the timer
    T1CTL |= (1 << 1);

    // wait until timer overflow occurs (or alternative flag if available)
    while (!(T1CNTL & 0x80));

    // stop the timer
    T1CTL &= ~(1 << 1);
}

