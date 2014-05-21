/**
 * Spheragone
 * @author Maxime Jourdan <maxime.jourdan@spheragone.fr>
 * @version 1.0
 * Classes et utilitaires pour gérer efficacement des requêtes BER-TLV
 */

#ifndef H_TLVSIMPLE
#define H_TLVSIMPLE

#include "TLV.h"

/**
 * Définit un TLV simple - Type Length Value
 */
class TLVSimple : public TLV {

public:

	TLVSimple() :TLV(0), length(0), value(0) { }
	
	virtual ~TLVSimple() { if(value) delete[] value; }

	/**
	 * Construit un TLV à partir de son type et de sa taille.
	 * Une valeur peut être optionnellement fournie pour accélérer le processus de création.
	 */
	TLVSimple(int tag, unsigned char length, const unsigned char* value = 0);

	/**
	 * Retourne ce TLV en entier au format BER-TLV (X.690), c'est-à-dire son type, sa longueur et son contenu
	 * concaténés dans une chaîne d'octets.
	 * @param[out] size sera rempli avec la longueur du tableau retourné
	 * @return le contenu du TLV
	 */
	virtual const unsigned char* getData(unsigned char* size);

	/**
	 * retourne la longueur du contenu de ce TLV
	 */
	virtual unsigned char getLength() const { return length; } 

	/**
	 * Retourne le contenu de ce TLV
	 */
	unsigned char* getValue() { return value; }

	virtual std::vector<TLV*> getTlvsByType(int tag);


private:

	friend class TLV;
	
	/**
	 * Retourne un TLV simple depuis une chaîne d'octets
	 * @param response typiquement la réponse reçue du serveur à parser
	 * @return un TLV (TLVSimple ou TLVContainer selon les données dans "response") contenant l'ensemble des données formatées
	 */
	static TLVSimple* parseResponse(const unsigned char* response, unsigned short size);

	unsigned char length;
	unsigned char* value;
};

#endif
