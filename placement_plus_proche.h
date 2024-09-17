#ifndef INCLUDED_PLACEMENT_PLUS_PROCHE_H
#define INCLUDED_PLACEMENT_PLUS_PROCHE_H
#include "algos_placement.h"

//Ce placement calcule pour chaque paire Noeud-Emplacement la distance
//Ensuite il va placer les paires dans l'ordre de distance minimale des que c'est possible
//A appliquer apres un placement qui reduit les intersections sans placer les noeuds comme FMME
class PlacementPlusProche : public AlgosPlacement
{
public:
    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementPlusProche* clone() const override;
};

#endif