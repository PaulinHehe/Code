#include "placement_plus_proche.h"
#include "algos_graphe.h"
#include "geometrie.h"

std::vector<int> PlacementPlusProche::calcPositionNoeuds(const Graphe &G)
{
    std::vector<Position<double>> posNoeuds;
    for (int noeud = 0; noeud < G.getNbNoeuds(); noeud++) posNoeuds.push_back(G.getNoeud(noeud).getPos());

    std::vector<std::vector<double>> matDist;
	for (int idNoeud = 0; idNoeud < G.getNbNoeuds(); idNoeud++)
	{
		matDist.push_back({});
		for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++)
		{
			Position<double> posEmp = G.getEmplacement(idEmp).getPos();
			matDist[idNoeud].push_back(distance(posNoeuds[idNoeud], posEmp));
		}
	}

	AlgosGraphe algos;
	std::vector<int> places = algos.greedyAssignementProblem(matDist);

	return places;
}

PlacementPlusProche* PlacementPlusProche::clone() const
{
    return new PlacementPlusProche(*this);
}