#ifndef __H_KERNEL
#define __H_KERNEL

#include <memory>
#include <vector>

#include "PCSCConnector.h"
#include "aid.h"
#include "TLVContainer.h"
#include "EMVConstants.h"

class Kernel {

private:
   std::unique_ptr<Connector> c;
   std::vector<ApplicationInfo> aids;

public:

   Kernel(Connector* c) : c(c) { }
   
   RAPDU* selectDirectory(const char* name, int length = 0);
   RAPDU* readRecord(unsigned char P1, unsigned char P2);
   
   void fillDataFromTag(unsigned char* dest, unsigned int length, std::unique_ptr<TLVContainer>& t, int tag);
   void fillDataFromTag(char* dest, unsigned int length, std::unique_ptr<TLVContainer>& t, int tag);
   
   std::vector<ApplicationInfo> getCardAIDs();
   std::vector<ApplicationInfo> readPse(unsigned char sfi, const ApplicationInfo& appInfo);

};

#endif