# Values to Set Registers

The proceeding sections outline the different registers that need to be set and their respective values for operation. 


# RF (Section: 13)
The relevant RF registers and values are presented. Both receive (RX) and transmit (TX) are outlined in this section.
We also introduce `Command Strobes`, `Data Rate`, and `Receive Channel Bandwidth` which are relevant to RF radio operations for the CC2511. THese can be found in the overview section.

## Overview 
The RF registers that are configured for radio operations are found in XDATA memory. There are two important interrupt vectors for RF:
1. RFTXRX (Interrupt #0)=> RX data ready or TX data complete. 
2. RF (Interrupt #1)	=> All other general RF interrupts.

### Command Strobes (Section 13.1)
Command strobes are used to allow the CPU control of radio operations. They are single byte instructions, which start
internal sequences within the radio. Refer to the state diagram and the table of command strobes which ca be found on
pages 184 and 185 of the datasheet, respectively. 

### Data Rate (Section: 13.5)
The data rate for transmit and expected receive can be programmed with `MDMCFG3.DRATE_M` and `MDMCFG4.DRATE_E`. These two registers control the Mantissa (`_M`) and Exponent (`_E`) of the data rate value. The mantissa/exponent formulas and the data rate formula can be seen below.
 
![Data Rate Mantissa/Exponents](image-2.png)
![Data Rate Formula](image-1.png)

`NOTE:` That the data rate is limited by system clock speed, refer to section 12.1.5.2 of the datasheet.

### Receiver Channel Filter Bandwidth (Section: 13.6)
For the receiver, we can program the receiver channel filter register to meet the bandwidth requirements. To configure the bandwidth a mantissa/exponent format is used. We use `MDMCFG4.CHANBW_E` and `MDMCFG4.CHANBW_M` to program the filter bandwidth. The formal will look as follows:
![Receiver Filter Bandwidth Formula](image.png)

| **MDMCFG4 . CHANBW_E** | **00** | **01** | **10** | **11** |
|------------------------|--------|--------|--------|--------|
| **CHANBW_M = 00**      | 812    | 406    | 203    | 102    |
| **CHANBW_M = 01**      | 650    | 325    | 162    | 81     |
| **CHANBW_M = 10**      | 541    | 270    | 135    | 68     |
| **CHANBW_M = 11**      | 464    | 232    | 116    | 58     |

**Channel Filter Bandwidths [kHz] (assuming f<sub>ref</sub> = 26 MHz)**

| **MDMCFG4 . CHANBW_E** | **00** | **01** | **10** | **11** |
|------------------------|--------|--------|--------|--------|
| **CHANBW_M = 00**      | 750    | 375    | 188    | 94     |
| **CHANBW_M = 01**      | 600    | 300    | 150    | 75     |
| **CHANBW_M = 10**      | 500    | 250    | 125    | 63     |
| **CHANBW_M = 11**      | 429    | 214    | 107    | 54     |

**Channel Filter Bandwidths [kHz] (assuming f<sub>ref</sub> = 24 MHz)**



## Receive 
## Transmit  

# UART 
The relevant UART registers and values are presented below. 

# Timers
The relevant timer registers and values are presented below.

# Data/Memory 

# Interrupts

# General 