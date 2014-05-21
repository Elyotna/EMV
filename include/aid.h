#ifndef __H_AID
#define __H_AID

#include <vector>

struct AID {

   const char* aid;
   int length;
   const char* name;

};

const AID CONST_AID[] = { 
                         { "\xA0\x00\x00\x00\x25\x01", 6, "AMEX" },
                         { "\xA0\x00\x00\x00\x03\x10\x10", 7, "VISA" }
                       };
const int NB_AID = 2;


#endif