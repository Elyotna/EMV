/**
 * Spheragone
 * @author Maxime Jourdan <maxime.jourdan@spheragone.fr>
 * @version 1.0
 * Classes et utilitaires pour gérer efficacement des requêtes BER-TLV X.690
 */


#ifndef H_TLV
#define H_TLV

#include <vector>

// Bit 6 du type du TLV. S'il est à 1, le TLV est un conteneur, sinon c'est un TLV simple.
#define TAG_FLAG_CONSTRUCTED 0x20
#define TAG_MASK_CONTINUED 0x1F

/**
 * Class abstraite définissant les caractéristiques principales d'un TLV, qu'il soit simple ou construit
 */
class TLV {

public:

	TLV(int tag);

	virtual ~TLV() { }

	/**
	 * Retourne l'ensemble des données d'un TLV.
	 * Si celui-ci est un groupe de TLV, ceux-ci sont incorporés dedans
	 * Si celui-ci est simple, alors il renvoie Type-Longueur-Valeur dans une seule chaine
	 * @param[out] size sera rempli avec la taille totale retournée
	 * @return le TLV au complet
	 */
	virtual const unsigned char* getData(unsigned char* size) = 0;

	/**
	 * Retourne la longueur des données contenues dans le TLV
	 */
	virtual unsigned char getLength() const = 0;
   
   int getTagLength() const { return tagLength; }

	/**
	 * Retourne le type du TLV
	 */
	const unsigned char getTag() { return tag; }

	/**
	 * Retourne tous les TLV contenus dans celui-ci
	 * en fonction de leur type.
	 * Le vecteur retourné peut être vide si aucun TLV du type demandé
	 * n'a été trouvé.
	 * @param[out] nbTlvs le nombre de TLVs retournés. Doit être initialisé à 0
	 */
	virtual std::vector<TLV*> getTlvsByType(int tag) = 0;

	/**
	 * Retourne le premier TLV trouvé de type correspondant
	 * @param[in] type le type du TLV recherché
	 */
	virtual TLV* getTlvByType(int tag);

	/**
	 * Retourne un TLV simple ou un TLV construit, depuis une chaîne d'octets
	 * @param response typiquement la réponse reçue du serveur à parser
	 * @param size la taille de la reponse
	 * @return un TLV (TLVSimple ou TLVContainer selon les données dans "response") contenant l'ensemble des données formatées
	 */
	static TLV* parseResponse(const unsigned char* response, unsigned short size);
   
   static int computeTagLength(int tag);
   static int parseTag(const unsigned char* in);

protected:

	/**
	 * Le type du TLV
	 */
	int tag;
   int tagLength;
};




#endif