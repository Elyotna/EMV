#include "rapdu.h"

using namespace std;

RAPDU::RAPDU(const unsigned char* received, int size) : SW1(0), SW2(0) {
   if(size < 2)
      return;
      
   SW1 = received[size-2];
   SW2 = received[size-1];
   
   if(size > 2) {
      data.insert(data.begin(), received, received+size-2);
   }
}

unsigned short RAPDU::getReturnCode() const {
   unsigned short toReturn = SW1;

   toReturn <<= 8;
   toReturn += SW2;
   
   return toReturn;
}