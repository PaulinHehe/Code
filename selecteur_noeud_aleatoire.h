#ifndef SELECTEURNOEUDALEATOIRE_H
#define SELECTEURNOEUDALEATOIRE_H
#include "selecteur_noeud.h"

class SelecteurNoeudAleatoire : public SelecteurNoeud{
public:
    int selectionneNoeud(const Graphe& G) override;
    SelecteurNoeudAleatoire* clone() const override;
};

#endif