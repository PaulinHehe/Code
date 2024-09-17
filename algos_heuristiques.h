#ifndef INCLUDED_ALGOS_HEURISTIQUES_H
#define INCLUDED_ALGOS_HEURISTIQUES_H
#include "graphe.h"

class AlgosHeuristiques
{
public:
    virtual void execute(Graphe &G) = 0;
    virtual AlgosHeuristiques* clone() const = 0;
};

#endif