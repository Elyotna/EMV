#include "TLVContainer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

TLVContainer::TLVContainer(int tag) : TLV(tag) {
	if(!(tag & TAG_FLAG_CONSTRUCTED))
		puts("Alerte : Instanciation d'un TLVContainer mais le bit 6 du type ne vaut pas '1'");
}

TLVContainer::~TLVContainer() {
	for(vector<TLV*>::iterator it = tlvs.begin(); it != tlvs.end(); ++it) {
		TLV* t = *it;
		delete t;
	}
}

void TLVContainer::addTLV(TLV* t) {
	tlvs.push_back(t);
}

/** 
 * Retourne la longueur du contenu de ce tlv
 */
unsigned char TLVContainer::getLength() const {

	unsigned char toReturn = 0;

	for(vector<TLV*>::const_iterator it = tlvs.begin(); it != tlvs.end(); ++it) {
		const TLV* t = *it;
		toReturn += static_cast<unsigned char>(t->getTagLength() + 1 + t->getLength());
	}

	return toReturn;
}

/**
 * Retourne le premier TLV trouvé de type correspondant
 * @param type le type de TLV recherché
 */
TLV* TLVContainer::getTlvByType(int tag) {
	if(tag == this->tag)
		return this;

	for(vector<TLV*>::const_iterator it = tlvs.begin(); it != tlvs.end(); ++it) {
		TLV* t = *it;
		TLV* returned = t->getTlvByType(tag);
		if(returned)
			return returned;
	}

	return NULL;
}

std::vector<TLV*> TLVContainer::getTlvsByType(int tag) {

	vector<TLV*> toReturn;

	if(tag == this->tag)
		toReturn.push_back(this);

	for(vector<TLV*>::const_iterator it = tlvs.begin(); it != tlvs.end(); ++it) {
		TLV* t = *it;
		std::vector<TLV*> tlvsByType = t->getTlvsByType(tag);
		toReturn.insert(toReturn.end(), tlvsByType.begin(), tlvsByType.end());
	}

	return toReturn;
}

/**
 * Retourne un TLV conteneur (construit) depuis une chaîne d'octets
 * @param response typiquement la réponse reçue du serveur à parser
 * @return un TLVContainer contenant un ou plusieurs autres TLVs
 */
TLVContainer* TLVContainer::parseResponse(const unsigned char* response, unsigned short size) {
	
	if(size < 2) {
		puts("Erreur : Impossible de parser un TLV construit de taille inférieure à 2");
		return NULL;
	}

	int tag = parseTag(response);
	unsigned short length = response[computeTagLength(tag)];
   
   printf("Parsing TLVContainer - tag %x - length %d\n", tag, length);

	TLVContainer* toReturn = new TLVContainer(tag);

	unsigned short tempLength = 0;

	while(tempLength < length) {
      printf(" |");
		TLV* t = TLV::parseResponse(response+tempLength+computeTagLength(tag)+1, size-tempLength-computeTagLength(tag)-1);

		if(!t)
			return NULL;

		toReturn->addTLV(t);

		unsigned short tlvLength = t->getLength();
		tempLength += (tlvLength+t->getTagLength()+1);
	}

	if(tempLength != length) {
		printf("Erreur pour TLV 0x%02x, taille finale (%d) différente de la taille attendue (%d)\n", tag, tempLength, length);
		return NULL;
	}

	return toReturn;
	
}

/**
 * Retourne ce TLV en entier, c'est-à-dire son type, sa longueur et le contenu
 * de tous les TLV qu'il contient.
 * @param[out] size sera rempli avec la longueur du tableau retourné
 * @return le contenu du TLV
 */
const unsigned char* TLVContainer::getData(unsigned char* size) {

	unsigned char* buff = new unsigned char[256];
	*size = 0;

	for(vector<TLV*>::iterator it = tlvs.begin(); it != tlvs.end(); ++it) {
		TLV* t = *it;
		unsigned char tlvSize;
		const unsigned char* tlvContent = t->getData(&tlvSize);
		memcpy(buff+*size, tlvContent, tlvSize);
		*size += tlvSize;
		delete[] tlvContent;
	}

	unsigned char* toReturn = new unsigned char[1+getTagLength()+*size];
	memcpy(toReturn, &tag, getTagLength());
	toReturn[getTagLength()] = getLength();
	memcpy(toReturn+1+getTagLength(), buff, *size);

	*size += (1+getTagLength());

	delete[] buff;

	return toReturn;

}
