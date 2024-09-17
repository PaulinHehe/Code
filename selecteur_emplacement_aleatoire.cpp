#include "selecteur_emplacement_aleatoire.h"

//Selectionne 1 emplacement aléatoire different du noeud
int SelecteurEmplacementAleatoire::selectionneEmplacement(const Graphe &G, int nodeId){
    int randomEmpId = generateRand((int)G.getNbEmplacements()); // Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
    while (G.getNoeud(nodeId).getEmplacement() == randomEmpId) {
        randomEmpId = generateRand((int)G.getNbEmplacements());
    }
    return randomEmpId;
}
SelecteurEmplacementAleatoire* SelecteurEmplacementAleatoire::clone()const{
    return new SelecteurEmplacementAleatoire(*this);
}