#include <stdio.h>

#include "TLVSimple.h"
#include "TLVContainer.h"

TLV::TLV(int tag) {
   tagLength = computeTagLength(tag);
   printf("tag = %04x ; Length = %d\n", tag, tagLength);
}

int TLV::computeTagLength(int tag) {
   if(tag & 0xFF000000)
      return 4;
   else if(tag & 0xFFFF0000)
      return 3;
   else if(tag & 0xFFFFFF00)
      return 2;
   else
      return 1;
}

TLV* TLV::parseResponse(const unsigned char* response, unsigned short size) {

	if(size < 3) {
		puts("Erreur : Impossible de parser un TLV de taille inférieure à 3");
		return NULL;
	}

	unsigned char type = response[0];

	// TLV construit
	if(type & TAG_FLAG_CONSTRUCTED)
		return TLVContainer::parseResponse(response, size);

	// TLV simple
	return TLVSimple::parseResponse(response, size);
}

TLV* TLV::getTlvByType(int tag) {
	if(tag == this->tag)
		return this;

	return NULL;
}

int TLV::parseTag(const unsigned char* in) {
   int i = 0;
   int toReturn = 0;
   int tmp;
   
   do {
      tmp = in[i];
      toReturn += (tmp << 8*i);
      ++i;
   } while((i == 1 && (tmp & TAG_MASK_CONTINUED) == TAG_MASK_CONTINUED) || (i > 1 && tmp & 0x80));
   
   return toReturn;
}