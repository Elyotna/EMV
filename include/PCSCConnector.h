#ifndef __H_PCSC_CONNECTOR
#define __H_PCSC_CONNECTOR

#include <winscard.h>

#include "Connector.h"
#include "apdu.h"
#include "rapdu.h"

class PCSCConnector : public Connector {

private:

   SCARDCONTEXT hContext;
   LPTSTR mszReaders;
   SCARDHANDLE hCard;
   DWORD dwReaders, dwActiveProtocol;
   SCARD_IO_REQUEST pioSendPci;

public:
   ~PCSCConnector();
   virtual void initialize();
   virtual RAPDU* sendAPDU(const APDU& apdu);

};

#endif