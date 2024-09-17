#ifndef SELECTEUREMPLACEMENTALEATOIRE_H
#define SELECTEUREMPLACEMENTALEATOIRE_H

#include "selecteur_emplacement.h"


class SelecteurEmplacementAleatoire : public SelecteurEmplacement{
public:
    //Selectionne 1 emplacement aléatoire different du noeud
    int selectionneEmplacement(const Graphe &G, int nodeId) override;
    SelecteurEmplacementAleatoire* clone() const override;
};

#endif