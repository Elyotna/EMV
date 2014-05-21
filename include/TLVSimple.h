/**
 * Spheragone
 * @author Maxime Jourdan <maxime.jourdan@spheragone.fr>
 * @version 1.0
 * Classes et utilitaires pour g�rer efficacement des requ�tes BER-TLV
 */

#ifndef H_TLVSIMPLE
#define H_TLVSIMPLE

#include "TLV.h"

/**
 * D�finit un TLV simple - Type Length Value
 */
class TLVSimple : public TLV {

public:

	TLVSimple() :TLV(0), length(0), value(0) { }
	
	virtual ~TLVSimple() { if(value) delete[] value; }

	/**
	 * Construit un TLV � partir de son type et de sa taille.
	 * Une valeur peut �tre optionnellement fournie pour acc�l�rer le processus de cr�ation.
	 */
	TLVSimple(int tag, unsigned char length, const unsigned char* value = 0);

	/**
	 * Retourne ce TLV en entier au format BER-TLV (X.690), c'est-�-dire son type, sa longueur et son contenu
	 * concat�n�s dans une cha�ne d'octets.
	 * @param[out] size sera rempli avec la longueur du tableau retourn�
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
	 * Retourne un TLV simple depuis une cha�ne d'octets
	 * @param response typiquement la r�ponse re�ue du serveur � parser
	 * @return un TLV (TLVSimple ou TLVContainer selon les donn�es dans "response") contenant l'ensemble des donn�es format�es
	 */
	static TLVSimple* parseResponse(const unsigned char* response, unsigned short size);

	unsigned char length;
	unsigned char* value;
};

#endif
