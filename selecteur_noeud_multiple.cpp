#include "selecteur_noeud_multiple.h"
#include "utilitaire.h"

SelecteurNoeudMultiple::SelecteurNoeudMultiple(int n) : n{n}
{}

//Selectionne le noeud avec le meilleur score parmis n noeuds aléatoires
//Incrémente n
int SelecteurNoeudMultiple::selectionneNoeud(const Graphe& G) {
    int randomId = generateRand(G.getNbNoeuds()-1);
    int randomId2;
    for(int i=0; i<n-1; i++){
        do {
            randomId2 = generateRand(G.getNbNoeuds()-1);
        } while (randomId2 == randomId);
        if (G.getScoreNoeud(randomId2) > G.getScoreNoeud(randomId)){
            randomId = randomId2;
        }
    } 
    n++;
    return randomId;
}
SelecteurNoeudMultiple* SelecteurNoeudMultiple::clone() const{
    return new SelecteurNoeudMultiple(*this);
}

int SelecteurNoeudMultiple::getN() const{
    return n;
}
void SelecteurNoeudMultiple::setN(int p_n){
    n = p_n;
}