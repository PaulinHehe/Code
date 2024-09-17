#ifndef SELECTEUREMPLACEMENTBINAIRE_H
#define SELECTEUREMPLACEMENTBINAIRE_H

#include "selecteur_emplacement.h"

class SelecteurEmplacementBinaire : public SelecteurEmplacement{
public:
    //Selectionne l'emplacement le plus proche du noeud parmis 2 emplacements aléatoires
    int selectionneEmplacement(const Graphe &G, int nodeId) override;
    SelecteurEmplacementBinaire* clone() const override;
};

#endif