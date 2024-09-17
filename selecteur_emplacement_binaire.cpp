#include "selecteur_emplacement_binaire.h"

//Selectionne l'emplacement le plus proche du noeud parmis 2 emplacements aléatoires
int SelecteurEmplacementBinaire::selectionneEmplacement(const Graphe &G, int nodeId) {
    int randomEmpId = generateRand((int)G.getNbEmplacements()); // Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
    // on retire si on pioche le meme emplacement
    while (G.getNoeud(nodeId).getEmplacement() == randomEmpId) {
        randomEmpId = generateRand((int)G.getNbEmplacements());
    }
    int randomEmpId2 = generateRand((int)G.getNbEmplacements()); // Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
    // on retire si on pioche le meme emplacement
    while ((G.getNoeud(nodeId).getEmplacement() == randomEmpId2)||(randomEmpId2 == randomEmpId)) {
        randomEmpId2 = generateRand((int)G.getNbEmplacements());
    }
    int dist1 = distanceCarre<int>(G.getNoeud(nodeId).getPos(), G.getEmplacement(randomEmpId).getPos());
    int dist2 = distanceCarre<int>(G.getNoeud(nodeId).getPos(), G.getEmplacement(randomEmpId2).getPos());
    if (dist2 < dist1) {
        randomEmpId = randomEmpId2;
    }
    return randomEmpId;
}

SelecteurEmplacementBinaire* SelecteurEmplacementBinaire::clone() const {
    return new SelecteurEmplacementBinaire(*this);
}
