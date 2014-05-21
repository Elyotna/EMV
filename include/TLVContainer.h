/**
 * Spheragone
 * @author Maxime Jourdan <maxime.jourdan@spheragone.fr>
 * @version 1.0
 * Classe g�rant un TLV construit dans le cadre de la norme BER-TLV (X.690)
 * Le 6� bit doit �tre � 1 pour le type de ce genre de TLV.
 */

#ifndef H_TLVCONTAINER
#define H_TLVCONTAINER

#include "TLV.h"

/**
 * D�finit un TLV conteneur : comprend plusieurs tlvs
 * Aussi appel� TLV "construit"
 */
class TLVContainer : public TLV {
public:
	TLVContainer(int tag);

	virtual ~TLVContainer();

	/**
	 * Retourne ce TLV en entier au format BER-TLV (X.690), c'est-�-dire son type, sa longueur et son contenu
	 * concat�n�s dans une cha�ne d'octets.
	 * @param[out] size sera rempli avec la longueur du tableau retourn�
	 * @return le contenu du TLV
	 */
	virtual const unsigned char* getData(unsigned char* size);

	/**
	 * Ajoute un tlv dans ce TLV construit
	 * @param t un TLV, simple ou construit
	 */
	void addTLV(TLV* t);

	/**
	 * Retourne tous les TLV contenus dans celui-ci
	 * en fonction de leur type.
	 * Le vecteur retourn� peut �tre vide si aucun TLV du type demand�
	 * n'a �t� trouv�.
	 */
	virtual std::vector<TLV*> getTlvsByType(int tag);

	/**
	 * Retourne le premier TLV trouv� de type correspondant
	 */
	virtual TLV* getTlvByType(int tag);

	/**
	 * Retourne la longueur du contenu de ce tlv
	 */
	virtual unsigned char getLength() const;

private:

	friend class TLV;

	/**
	 * Retourne un TLV conteneur (construit) depuis une cha�ne d'octets
	 * @param response typiquement la r�ponse re�ue du serveur � parser
	 * @return un TLVContainer contenant un ou plusieurs autres TLVs
	 */
	static TLVContainer* parseResponse(const unsigned char* response, unsigned short size);

	/**
	 * La liste des TLVs compris dans ce conteneur
	 */
	std::vector<TLV*> tlvs;
};

#endif