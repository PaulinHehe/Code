#ifndef INCLUDED_SELECTEUR_NOEUD_H
#define INCLUDED_SELECTEUR_NOEUD_H
#include "graphe.h"

class SelecteurNoeud
{
public:
    virtual int selectionneNoeud(const Graphe &G) = 0;
    virtual SelecteurNoeud* clone() const = 0;
};

#endif