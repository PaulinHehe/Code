#ifndef INCLUDED_PLACEMENT_NOEUD_PAR_NOEUD_H
#define INCLUDED_PLACEMENT_NOEUD_PAR_NOEUD_H
#include "algos_placement.h"

//Ce placement place les noeuds un par un en suivant leur id
//Il place chaque noeud sur l'emplacement disponible le plus proche
//A appliquer apres un placement qui reduit les intersections sans placer les noeuds comme FMME
class PlacementNoeudParNoeud : public AlgosPlacement
{
public:
    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementNoeudParNoeud* clone() const override;
};

#endif