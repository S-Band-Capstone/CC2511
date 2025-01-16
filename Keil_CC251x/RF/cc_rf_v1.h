
#ifndef CC_RF_V1_H
#define CC_RF_V1_H

void rfInit(void); 
void rfSend(unsigned short* rfTxBuf, unsigned short rfTxBufLength);
void rfReceive(unsigned short* rfTxBuf, unsigned short rfTxBufLength); 
void bufferClear(unsigned short* buffer, unsigned short bufferLen); 

#endif