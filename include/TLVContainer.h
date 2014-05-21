/**
 * Spheragone
 * @author Maxime Jourdan <maxime.jourdan@spheragone.fr>
 * @version 1.0
 * Classe gérant un TLV construit dans le cadre de la norme BER-TLV (X.690)
 * Le 6è bit doit être à 1 pour le type de ce genre de TLV.
 */

#ifndef H_TLVCONTAINER
#define H_TLVCONTAINER

#include "TLV.h"

/**
 * Définit un TLV conteneur : comprend plusieurs tlvs
 * Aussi appelé TLV "construit"
 */
class TLVContainer : public TLV {
public:
	TLVContainer(int tag);

	virtual ~TLVContainer();

	/**
	 * Retourne ce TLV en entier au format BER-TLV (X.690), c'est-à-dire son type, sa longueur et son contenu
	 * concaténés dans une chaîne d'octets.
	 * @param[out] size sera rempli avec la longueur du tableau retourné
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
	 * Le vecteur retourné peut être vide si aucun TLV du type demandé
	 * n'a été trouvé.
	 */
	virtual std::vector<TLV*> getTlvsByType(int tag);

	/**
	 * Retourne le premier TLV trouvé de type correspondant
	 */
	virtual TLV* getTlvByType(int tag);

	/**
	 * Retourne la longueur du contenu de ce tlv
	 */
	virtual unsigned char getLength() const;

private:

	friend class TLV;

	/**
	 * Retourne un TLV conteneur (construit) depuis une chaîne d'octets
	 * @param response typiquement la réponse reçue du serveur à parser
	 * @return un TLVContainer contenant un ou plusieurs autres TLVs
	 */
	static TLVContainer* parseResponse(const unsigned char* response, unsigned short size);

	/**
	 * La liste des TLVs compris dans ce conteneur
	 */
	std::vector<TLV*> tlvs;
};

#endif