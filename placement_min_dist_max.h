#ifndef INCLUDED_PLACEMENT_MIN_DIST_MAX_H
#define INCLUDED_PLACEMENT_MIN_DIST_MAX_H
#include "algos_placement.h"

//OBSOLETE POUR LE MOMENT
//Ce placement calcule pour chaque paire Noeud-Emplacement la distance
//Ensuite il va placer les paires de telle sorte que le noeud le plus loin de son emplacement soit a la plus petite distance possible
//A appliquer apres un placement qui reduit les intersections sans placer les noeuds comme FMME
class PlacementMinDistMax : public AlgosPlacement
{
public:
    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementMinDistMax* clone() const override;
};

#endif