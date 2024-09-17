#ifndef INCLUDED_PLACEMENT_ALEATOIRE_H
#define INCLUDED_PLACEMENT_ALEATOIRE_H
#include <vector>
#include "graphe.h"
#include "algos_placement.h"
using namespace std;

class PlacementAleatoire : public AlgosPlacement
{
public:
    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementAleatoire* clone() const override;
};

#endif