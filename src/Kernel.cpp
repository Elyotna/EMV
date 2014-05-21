#include "Kernel.h"
#include "TLVSimple.h"

#include <string.h>
#include <stdio.h>
#include <exception>

using namespace std;

RAPDU* Kernel::readRecord(unsigned char P1, unsigned char P2) {
   APDU apdu(0, INS_READ_RECORD, P1, P2);  
   return c->sendAPDU(apdu);
}

void Kernel::fillDataFromTag(unsigned char* dest, unsigned int length, unique_ptr<TLVContainer>& t, int tag) {
   unique_ptr<TLVSimple> tlv (dynamic_cast<TLVSimple*>(t->getTlvByType(tag)));
      
   if(!tlv)
      return;
      
   if(tlv->getLength() > length)
      return;
      
   memcpy(dest, tlv->getValue(), tlv->getLength());
}

void Kernel::fillDataFromTag(char* dest, unsigned int length, unique_ptr<TLVContainer>& t, int tag) {
   fillDataFromTag((unsigned char*) dest, length, t, tag);
}

std::vector<ApplicationInfo> Kernel::readPse(unsigned char sfi, const ApplicationInfo& appInfo) {

   unsigned char record = 1;
   vector<ApplicationInfo> toReturn;
   
   for(record = 1; ;) {
      unique_ptr<RAPDU> rapdu(readRecord(sfi, record++));
      if(rapdu->getReturnCode() != 0x9000) // 0x6A83 is the "end of record" response code, but we stop parsing on any error anyway
         break;
         
      const unsigned char* data = &rapdu->getData()[0];
      unique_ptr<TLVContainer> t (dynamic_cast<TLVContainer*>(TLV::parseResponse(data, rapdu->getData().size())));
         
      if(!t)
         continue;
         
      ApplicationInfo app;
      // Initialize APP with parameters taken from SELECT PSE reponse (Language Pref & table index)
      memcpy(&app, &appInfo, sizeof(app));
      
      fillDataFromTag(app.DFName, sizeof(app.DFName), t, TAG_ADF_NAME);
      fillDataFromTag(app.strApplicationLabel, sizeof(app.strApplicationLabel), t, TAG_APPLICATION_LABEL);
      fillDataFromTag(app.strApplicationPreferredName, sizeof(app.strApplicationPreferredName), t, TAG_APP_PREFERRED_NAME);
      fillDataFromTag((unsigned char*)&app.priority, sizeof(app.priority), t, TAG_APP_PRIORITY_INDICATOR);
      
      toReturn.push_back(app);
      printf("Adding app %s\n", app.strApplicationPreferredName);
      
      if(app.priority & 0x80)
         app.needCardholderConfirm = 1;
      
      app.priority &= 0x0F;
   }

   return toReturn;

}

vector<ApplicationInfo> Kernel::getCardAIDs() {
   
   vector<ApplicationInfo> toReturn;
   
   // Try to use PSE Method
   unique_ptr<RAPDU> rapdu(selectDirectory("1PAY.SYS.DDF01"));

   if(rapdu->getReturnCode() == 0x9000) {
      const unsigned char* data = &rapdu->getData()[0];
      unique_ptr<TLVContainer> t (dynamic_cast<TLVContainer*>(TLV::parseResponse(data, rapdu->getData().size())));
         
      if(!t)
         return toReturn;
         
      unique_ptr<TLVSimple> sfiTlv (dynamic_cast<TLVSimple*>(t->getTlvByType(TAG_SFI_OF_DEF)));
      
      if(!sfiTlv)
         return toReturn;
         
      const unsigned char* sfiValue = sfiTlv->getValue();
      
      if(sfiTlv->getLength() != 1)
         return toReturn;
         
      unsigned char sfi = sfiValue[0];
      ApplicationInfo standardApp;
      memset(&standardApp, 0, sizeof(standardApp));
      
      fillDataFromTag(standardApp.strLanguagePreference, sizeof(standardApp.strLanguagePreference), t, TAG_LANGUAGE_PREFERENCE);
      fillDataFromTag(&standardApp.issuerCodeTableIndex, sizeof(standardApp.issuerCodeTableIndex), t, TAG_ISSUER_CODE_TABLE_INDEX);
      
      return readPse(sfi, standardApp);
   
   } else if(rapdu->getReturnCode() == 0x6A82) { // PSE not found - iterate through known AIDs
      for(int i = 0; i < NB_AID; ++i) {
         unique_ptr<RAPDU> rapdu(selectDirectory(CONST_AID[i].aid, CONST_AID[i].length));
         if(rapdu->getReturnCode() != 0x9000)
            continue;
            
         const unsigned char* data = &rapdu->getData()[0];
         
         unique_ptr<TLVContainer> t (dynamic_cast<TLVContainer*>(TLV::parseResponse(data, rapdu->getData().size())));
         
         if(!t)
            continue;
         
         // TODO : parse response AIDs
      }
   
   }

   return toReturn;

}

RAPDU* Kernel::selectDirectory(const char* name, int length) {
   APDU apdu(0, INS_SELECT, 0x04);
   if(length == 0)
      length = strlen(name);
      
   apdu.setData((const unsigned char*)name, length);
   
   return c->sendAPDU(apdu);
}