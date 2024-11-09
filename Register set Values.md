# Values to Set Registers

The proceeding sections outline the different registers that need to be set and their respective values for operation. 


# RF 
The relevant RF registers and values are presented. Both receive (RX) and transmit (TX) are outlined in this section.
We also introduce `Command Strobes` which are relevant to RF radio operations for the CC2511. 

The RF registers that are configured for radio operations are found in XDATA memory. 

There are two important interrupt vectors for RF: 
	1. RFTXRX (Interrupt #0)=> RX data ready or TX data complete. 
	2. RF (Interrupt #1)	=> All other general RF interrupts.

## Command Strobes
Command strobes are used to allow the CPU control of radio operations. They are single byte instructions, which start
internal sequences within the radio. Refer to the state diagram and the table of command strobes which ca be found on
pages 184 and 185 of the datasheet, respectively. 

## Data Rate
The data rate for transmit and expected receive can be programmed with `MDMCFG3.DRATE_M` and `MDMCFG4.DRATE_E`. These two registers control the Mantissa (`_M`) and Exponent (`_E`) of the data rate value. The mantissa/exponent formulas and the data rate formula can be seen below. 
![Data Rate Mantissa/Exponents](image-1.png)
![Data Rate Formula](image.png)
===`NOTE`:=== That the data rate is limited by system clock speed, refer to section 12.1.5.2 of the datasheet.
## Receive 
## Transmit  

# UART 
The relevant UART registers and values are presented below. 

# Timers
The relevant timer registers and values are presented below.

# Data/Memory 

# Interrupts

# General 