#ifndef __H_RAPDU
#define __H_RAPDU

#include <vector>

class RAPDU {

private:

   std::vector<unsigned char> data;
   unsigned char SW1;
   unsigned char SW2;

public:

   RAPDU() : SW1(0), SW2(0) { }
   
   RAPDU(const unsigned char* received, int size);
   unsigned short getReturnCode() const;
   const std::vector<unsigned char>& getData() { return data; }

};

#endif