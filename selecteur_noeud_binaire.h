#ifndef SELECTEURNOEUDBINAIRE_H
#define SELECTEURNOEUDBINAIRE_H
#include "selecteur_noeud.h"

class SelecteurNoeudBinaire : public SelecteurNoeud{
public:
    //Selectionne le noeud avec le meilleur score parmis 2 noeuds aléatoires
    int selectionneNoeud(const Graphe& G)  override;
    SelecteurNoeudBinaire* clone() const override;
};

#endif