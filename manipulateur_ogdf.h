#ifndef INCLUDED_MANIPULATEUR_OGDF_H
#define INCLUDED_MANIPULATEUR_OGDF_H
#include "graphe.h"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

class ManipulateurOgdf
{
public:
    //Cree un graphe ogdf et ses attributs a partir d'un objet graphe
    void creerGrapheOgdf(const Graphe &G, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const;
    
    //Renvoie les positions des noeuds du graphe ogdf sous forme de tableau de positions
    //Inverse de la fonction insererPositions
    std::vector<Position<double>> extrairePositions(const ogdf::Graph &ogdfG, const ogdf::GraphAttributes &ogdfGA) const;

    //Modifie les positions du graphe ogdf pour mettre celles du tableau posNoeuds
    //Inverse de la fonction extrairePositions
    //ATTENTION : le graphe ogdf doit contenir le meme nombre de noeuds que le tableau posNoeuds
    void insererPositions(const std::vector<Position<double>> &posNoeuds, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const;

    //Bouge les noeuds du graphe ogdf de telle sorte que les boites englobantes des emplacements du graphe G et des noeuds du graphe d'ogdf soient les memes
    //Les modifications effectuees sont des "applatissements" ou "elargissements" en x et en y
    void fusionneBoitesEnglobantes(const Graphe &G, ogdf::Graph &ogdfG, ogdf::GraphAttributes &ogdfGA) const;
};

#endif