#include "PCSCConnector.h"
#include <iostream>
#include <stdio.h>
using namespace std;

void printPacket(const unsigned char* buffer, unsigned int size) {

   unsigned int i;
   
   for(i = 0; i < size; ++i) {
      if(i != 0&& i%16 == 0) {
         for(unsigned int j = i-16; j < i; ++j) {
         
            if(buffer[j] >= 32 && buffer[j] <= 126)
               printf("%c", buffer[j]);
            else
               printf(".");
         }
         
         puts("");
      
      }
      
      if(i%16 == 0) {
         printf("%03d-%03d ", i, min(i+15, size-1));
      }

      printf("%02hhx ", buffer[i]);
   }
   
   for(i = ((16-i%16)%16); i > 0; --i)
      printf("   ");
   
   for(i = size- (size%16 == 0 ? 16 : size%16); i < size; ++i) {
      if(buffer[i] >= 32 && buffer[i] <= 126)
         printf("%c", buffer[i]);
      else
         printf(".");
   }
   
   puts("\n");
}

PCSCConnector::~PCSCConnector() {
   SCardDisconnect(hCard, SCARD_LEAVE_CARD);
   SCardFreeMemory(hContext, mszReaders);
   SCardReleaseContext(hContext);
}

void PCSCConnector::initialize() {
   if(SCardEstablishContext(SCARD_SCOPE_SYSTEM, 0, 0, &hContext) != SCARD_S_SUCCESS) {
      cerr << "Error : couldn't establish PC/SC context" << endl;
      return;
   }

   dwReaders = SCARD_AUTOALLOCATE;

   if(SCardListReaders(hContext, 0, (LPTSTR)&mszReaders, &dwReaders) != SCARD_S_SUCCESS) {
      cerr << "Error : Couldn't list readers" << endl;
      return;
   }

   cout << "reader name: " << mszReaders << endl;
   
   if(SCardConnect(hContext, mszReaders, SCARD_SHARE_SHARED, SCARD_PROTOCOL_T0 | SCARD_PROTOCOL_T1, &hCard, &dwActiveProtocol) != SCARD_S_SUCCESS) {
      cerr << "Error : Couldn't connect to card" << endl;
      return;
   }

    switch(dwActiveProtocol)
    {
     case SCARD_PROTOCOL_T0:
      pioSendPci = *SCARD_PCI_T0;
      break;

     case SCARD_PROTOCOL_T1:
      pioSendPci = *SCARD_PCI_T1;
      break;
    }
}

RAPDU* PCSCConnector::sendAPDU(const APDU& apdu) {
   unsigned char pbRecvBuffer[258];
   DWORD dwRecvLength = sizeof(pbRecvBuffer);
   std::vector<unsigned char> toSend = apdu.getBuffer();
   
   printf("Sending APDU ; Size %d\n", toSend.size());
   printPacket(&toSend[0], toSend.size());
   
   SCardTransmit(hCard, &pioSendPci, &toSend[0], toSend.size(),NULL, pbRecvBuffer, &dwRecvLength);
   
   printf("Received RAPDU ; Size %d\n", dwRecvLength);
   printPacket(pbRecvBuffer, dwRecvLength);
   
   // SW1 == 0x61 -> Ready to receive data, length = SW2
   if(dwRecvLength == 2 && pbRecvBuffer[0] == 0x61) {
      APDU toSend(0, 0xC0, 0, 0, pbRecvBuffer[1]);
      return sendAPDU(toSend);
   }

   return new RAPDU(pbRecvBuffer, dwRecvLength);
}