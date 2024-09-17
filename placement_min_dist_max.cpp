#include "placement_min_dist_max.h"

std::vector<int> PlacementMinDistMax::calcPositionNoeuds(const Graphe &G)
{
    struct NoeudEmpDist
	{
		int idN, idE; //idNoeud, idEmplacement
		double dist; //Distance

		bool operator<(const NoeudEmpDist &autre) const
		{
			return dist < autre.dist;
		}
	};

    std::vector<Position<double>> posNoeuds;
    for (int noeud = 0; noeud < G.getNbNoeuds(); noeud++) posNoeuds.push_back(G.getNoeud(noeud).getPos());
    std::vector<NoeudEmpDist> listePlacements;
	for (int idNoeud = 0; idNoeud < G.getNbNoeuds(); idNoeud++)
	{
		for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++)
		{
			Position<double> posEmp = G.getEmplacement(idEmp).getPos();
			listePlacements.push_back({idNoeud, idEmp, distance(posNoeuds[idNoeud], posEmp)});
		}
	}
	sort(listePlacements.begin(), listePlacements.end());
	return {};
}

PlacementMinDistMax* PlacementMinDistMax::clone() const
{
    return new PlacementMinDistMax(*this);
}