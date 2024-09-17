#ifndef INCLUDED_ALGOS_PLACEMENT_H
#define INCLUDED_ALGOS_PLACEMENT_H
#include <vector>
#include "graphe.h"
using namespace std;

class AlgosPlacement
{
public:
    virtual std::vector<int> calcPositionNoeuds(const Graphe &G) = 0;
    virtual AlgosPlacement* clone() const = 0;
};

#endif