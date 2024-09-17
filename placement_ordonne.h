#ifndef INCLUDED_PLACEMENT_ORDONNE_H
#define INCLUDED_PLACEMENT_ORDONNE_H
#include "algos_placement.h"

//Ce placement calcule pour chaque paire Noeud-Emplacement la distance definie comme suite :
//Chaque noeud est donne un ou plusieurs numeros en suivant l'ordre sur un ou plusieurs axes (par exemple abscisse), de meme pour les emplacements
//La distance calculee est la distance de Manatthan entre les numeros des noeuds et des emplacements
//Ensuite il va placer les paires dans l'ordre de distance minimale des que c'est possible
//A appliquer apres un placement qui reduit les intersections sans placer les noeuds comme FMME
class PlacementOrdonne : public AlgosPlacement
{
public:
    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementOrdonne* clone() const override;
};

#endif