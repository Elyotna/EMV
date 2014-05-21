#ifndef __H_CONNECTOR
#define __H_CONNECTOR

#include "apdu.h"
#include "rapdu.h"

class Connector {

public:

   virtual ~Connector() { }

   virtual void initialize() = 0;
   virtual RAPDU* sendAPDU(const APDU& apdu) = 0;

};

#endif