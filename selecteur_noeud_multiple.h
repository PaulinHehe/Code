#ifndef SELECTEURNOEUDMULTIPLE_H
#define SELECTEURNOEUDMULTIPLE_H
#include "selecteur_noeud.h"

class SelecteurNoeudMultiple : public SelecteurNoeud{
public:
    SelecteurNoeudMultiple(int n = 3);
    //Selectionne le noeud avec le meilleur score parmis n noeuds aléatoires
    int selectionneNoeud(const Graphe& G) override;
    SelecteurNoeudMultiple* clone() const override;
    int getN() const;
    void setN(int n);
private:
    //le nombre de noeuds selectionnés s'auto incrémente à chaque appel de selectionneNoeud
    int n;
};

#endif