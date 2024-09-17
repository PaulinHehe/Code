#include "placement_aleatoire.h"
#include "utilitaire.h"

std::vector<int> PlacementAleatoire::calcPositionNoeuds(const Graphe &G)
{
    std::vector<int> tab;
    for (int i = 0; i < G.getNbEmplacements(); i++)
    {
        tab.push_back(i);
    }
    melangeTab(tab);
    while (tab.size() > G.getNbNoeuds())
    {
        tab.pop_back();
    }
    return tab;
}

PlacementAleatoire* PlacementAleatoire::clone() const
{
    return new PlacementAleatoire(*this);
}