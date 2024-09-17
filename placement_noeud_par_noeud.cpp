#include "placement_noeud_par_noeud.h"
#include "algos_graphe.h"
#include "geometrie.h"

std::vector<int> PlacementNoeudParNoeud::calcPositionNoeuds(const Graphe &G)
{
    std::vector<Position<double>> posNoeuds;
    for (int noeud = 0; noeud < G.getNbNoeuds(); noeud++) posNoeuds.push_back(G.getNoeud(noeud).getPos());
    std::vector<Position<double>> posEmp;
    for (int emp = 0; emp < G.getNbEmplacements(); emp++) posEmp.push_back(G.getEmplacement(emp).getPos());
    fusionneBoitesEnglobantes(posEmp, posNoeuds);

    std::vector<int> places;
    std::vector<bool> dispo(G.getNbEmplacements(), true);
    for (int idNoeud = 0; idNoeud < G.getNbNoeuds(); idNoeud++)
    {
        int meilleurEmp = -1;
        for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++)
        {
            if (dispo[idEmp])
            {
                if (meilleurEmp == -1 || distance<double>(posNoeuds[idNoeud], posEmp[meilleurEmp]) > distance<double>(posNoeuds[idNoeud], posEmp[idEmp]))
                {
                    meilleurEmp = idEmp;
                }
            }
        }
        places.push_back(meilleurEmp);
        dispo[meilleurEmp] = false;
    }

	return places;
}

PlacementNoeudParNoeud* PlacementNoeudParNoeud::clone() const
{
    return new PlacementNoeudParNoeud(*this);
}