#include "selecteur_noeud_aleatoire.h"
#include "utilitaire.h"

int SelecteurNoeudAleatoire::selectionneNoeud(const Graphe& G) {
    return generateRand(G.getNbNoeuds()-1);
}
SelecteurNoeudAleatoire* SelecteurNoeudAleatoire::clone() const{
    return new SelecteurNoeudAleatoire(*this);
}