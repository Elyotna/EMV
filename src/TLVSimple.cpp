#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TLVSimple.h"

using namespace std;

/**
 * Construit un TLV à partir de son type et de sa taille.
 * Une valeur peut être optionnellement fournie pour accélérer le processus de création.
 */
TLVSimple::TLVSimple(int tag, unsigned char length, const unsigned char* value) : TLV(tag), length(length) {
	if(length == 0) {
		printf("Erreur : Ajout d'un TLV de taille 0. (type %04x)\n", tag);
		return;
	}

	this->value = new unsigned char[length];

	if(value)
		memcpy(this->value, value, length);
}

/**
 * Retourne ce TLV en entier au format BER-TLV (X.690), c'est-à-dire son type, sa longueur et son contenu
 * concaténés dans une chaîne d'octets.
 * @param[out] size sera rempli avec la longueur du tableau retourné
 * @return le contenu du TLV
 */
const unsigned char* TLVSimple::getData(unsigned char* size) {

	*size = length+getTagLength()+1;
	unsigned char* finalTLV = new unsigned char[*size];
   
	memcpy(finalTLV, &tag, getTagLength());
	finalTLV[getTagLength()] = length;
	memcpy(finalTLV+1+getTagLength(), value, length);

	return finalTLV;
}

/**
 * Retourne un TLV simple depuis une chaîne d'octets
 * @param response typiquement la réponse reçue du serveur à parser
 * @return un TLV (TLVSimple ou TLVContainer selon les données dans "response") contenant l'ensemble des données formatées
 */
TLVSimple* TLVSimple::parseResponse(const unsigned char* response, unsigned short size) {

	int tag = parseTag(response);
	unsigned char length = response[computeTagLength(tag)];
   
   printf("Parsing TLVSimple - tag %x - length %d\n", tag, length);

	if(length > size-1-computeTagLength(tag)) {
		puts("Erreur : longueur du TLV supérieure à la taille du buffer");
		return NULL;
	}

	const unsigned char* data = response+1+computeTagLength(tag);

	return new TLVSimple(tag, length, data);
}

std::vector<TLV*> TLVSimple::getTlvsByType(int tag) {

	vector<TLV*> toReturn;

	if(tag == this->tag)
		toReturn.push_back(this);


	return toReturn;
}
