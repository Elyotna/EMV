#ifndef __H_APDU
#define __H_APDU

#include <vector>

class APDU {

private:
   unsigned char cla;
   unsigned char ins;
   unsigned char P1;
   unsigned char P2;
   unsigned char Le;
   std::vector<unsigned char> data;

public:

   APDU(unsigned char cla = 0, unsigned char ins = 0, unsigned char P1 = 0, unsigned char P2 = 0, unsigned char Le = 0) : cla(cla), ins(ins), P1(P1), P2(P2), Le(Le) { }
   void setData(const std::vector<unsigned char>& data);
   void setData(const unsigned char* data, int size);
   std::vector<unsigned char> getBuffer() const;


};

#endif