#include "manipulateur_ogdf.h"
#include "algos_graphe.h"
#include "geometrie.h"
#include "display_openGL.h"

void ManipulateurOgdf::creerGrapheOgdf(const Graphe &G, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const
{
	ogdfG = ogdf::Graph();
	ogdfGA = ogdf::GraphAttributes(ogdfG);
    std::vector<ogdf::node> nodeTab;
	for (int i = 0; i < G.getNbNoeuds(); i++)
	{
		nodeTab.push_back(ogdfG.newNode());
        ogdfGA.x(nodeTab[i]) = G.getNoeud(i).getPos().getX();
        ogdfGA.y(nodeTab[i]) = G.getNoeud(i).getPos().getY();
	}
	for (int i = 0; i < G.getNbAretes(); i++)
	{
		int id1 = G.getArete(i).getNoeudDep();
		int id2 = G.getArete(i).getNoeudArr();
		ogdfG.newEdge(nodeTab[id1], nodeTab[id2]);
	}
}

std::vector<Position<double>> ManipulateurOgdf::extrairePositions(const ogdf::Graph &ogdfG, const ogdf::GraphAttributes &ogdfGA) const
{
	std::vector<Position<double>> posNoeuds;
	for (ogdf::node node : ogdfG.nodes)
	{
		Position<double> pos = {ogdfGA.x(node), ogdfGA.y(node)};
		posNoeuds.push_back(pos);
	}
	return posNoeuds;
}

void ManipulateurOgdf::insererPositions(const std::vector<Position<double>> &posNoeuds, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const
{
	int id = 0;
	for (ogdf::node node : ogdfG.nodes)
	{
		Position<double> pos = posNoeuds[id++];
		ogdfGA.x(node) = pos.getX();
		ogdfGA.y(node) = pos.getY();
	}
}

void ManipulateurOgdf::fusionneBoitesEnglobantes(const Graphe &G, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const
{
	std::vector<Position<double>> posG;
	for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++) posG.push_back(G.getEmplacement(idEmp).getPos());
	std::vector<Position<double>> posOgdf = extrairePositions(ogdfG, ogdfGA);

	::fusionneBoitesEnglobantes(posG, posOgdf);

	insererPositions(posOgdf, ogdfG, ogdfGA);
}