#include "selecteur_noeud_binaire.h"
#include "utilitaire.h"

//Selectionne le noeud avec le meilleur score parmis 2 noeuds aléatoires
int SelecteurNoeudBinaire::selectionneNoeud(const Graphe& G) {
    int randomId = generateRand(G.getNbNoeuds()-1);
    int randomId2;
    do {
        randomId2 = generateRand(G.getNbNoeuds()-1);
    } while (randomId2 == randomId);

    if (G.getScoreNoeud(randomId) > G.getScoreNoeud(randomId2)){
        return randomId;
    }
    return randomId2;
}
SelecteurNoeudBinaire* SelecteurNoeudBinaire::clone() const{
    return new SelecteurNoeudBinaire(*this);
}