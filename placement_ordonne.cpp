#include "placement_ordonne.h"
#include "algos_graphe.h"
#include "geometrie.h"

std::vector<int> PlacementOrdonne::calcPositionNoeuds(const Graphe &G)
{
    struct PosX
    {
        Position<double> pos;
        int id;

        bool operator<(const PosX &autre) const
        {
            if (pos.getX() != autre.pos.getX()) return pos.getX() < autre.pos.getX();
            return pos.getY() < autre.pos.getY();
        }
    };

    struct PosY
    {
        Position<double> pos;
        int id;

        bool operator<(const PosY &autre) const
        {
            if (pos.getY() != autre.pos.getY()) return pos.getY() < autre.pos.getY();
            return pos.getX() < autre.pos.getX();
        }
    };

    std::vector<PosX> posXN, posXE;
    std::vector<PosY> posYN, posYE;
    for (int noeud = 0; noeud < G.getNbNoeuds(); noeud++)
    {
        Position<double> pos = G.getNoeud(noeud).getPos();
        posXN.push_back({pos, noeud});
        posYN.push_back({pos, noeud});
    }
    for (int emp = 0; emp < G.getNbEmplacements(); emp++)
    {
        Position<double> pos = G.getEmplacement(emp).getPos();
        posXE.push_back({pos, emp});
        posYE.push_back({pos, emp});
    }
    sort(posXN.begin(), posXN.end());
    sort(posXE.begin(), posXE.end());
    sort(posYN.begin(), posYN.end());
    sort(posYE.begin(), posYE.end());
    std::vector<Position<double>> numN(G.getNbNoeuds()), numE(G.getNbEmplacements());
    for (int num = 0; num < G.getNbNoeuds(); num++)
    {
        numN[posXN[num].id].setX(1.0 * num / (G.getNbNoeuds()-1));
        numN[posYN[num].id].setY(1.0 * num / (G.getNbNoeuds()-1));
    }
    for (int num = 0; num < G.getNbEmplacements(); num++)
    {
        numE[posXE[num].id].setX(1.0 * num / (G.getNbEmplacements()-1));
        numE[posYE[num].id].setY(1.0 * num / (G.getNbEmplacements()-1));
    }

    std::vector<std::vector<double>> matDist;
	for (int idNoeud = 0; idNoeud < G.getNbNoeuds(); idNoeud++)
	{
		matDist.push_back({});
		for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++)
		{
			matDist[idNoeud].push_back(distanceManhattan(numN[idNoeud], numE[idEmp]));
		}
	}

	AlgosGraphe algos;
	std::vector<int> places = algos.greedyAssignementProblem(matDist);

	return places;
}

PlacementOrdonne* PlacementOrdonne::clone() const
{
    return new PlacementOrdonne(*this);
}