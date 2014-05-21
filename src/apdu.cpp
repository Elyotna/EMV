#include "apdu.h"

using namespace std;

void APDU::setData(const vector<unsigned char>& data) {
   this->data = data;
}

void APDU::setData(const unsigned char* data, int size) {
   this->data.clear();
   this->data.insert(this->data.begin(), data, data+size);
}

vector<unsigned char> APDU::getBuffer() const {
   vector<unsigned char> toReturn;
   
   toReturn.push_back(cla);
   toReturn.push_back(ins);
   toReturn.push_back(P1);
   toReturn.push_back(P2);
   
   if(!data.empty()) {
      toReturn.push_back(data.size());
      toReturn.insert(toReturn.end(), data.begin(), data.end());
   }
   
   if(Le != 0)
      toReturn.push_back(Le);
   
   return toReturn;
}