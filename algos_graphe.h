#ifndef INCLUDED_ALGOS_GRAPHE_H
#define INCLUDED_ALGOS_GRAPHE_H
#include "graphe.h"
#include <float.h>
#include <vector>
using namespace std;

class AlgosGraphe
{
public:
    struct AreteAlgo
    {
        int id, noeud;
    };

    //Renvoie le tableau des composantes fortement connexes du graphe
    //Chaque element du tableau contient l'ensemble des ids des noeuds d'une composante
    std::vector<std::vector<int>> calcComposantesFortementConnexes(const Graphe &G) const;

    //Rempli le nombre de noeuds et d'aretes du graphe
    //voisins contient le graphe
    //n et m sont le nombre de noeuds et d'aretes
    void compteNoeudsAretes(const std::vector<std::vector<AreteAlgo>> &voisins, int &n, int &m) const;
    //Calcule les deux parties qui composent un graphe biparti
    //voisins contient le graphe
    //X et Y contiennent les noeuds de chaque partie du graphe
    void calcPartiesGrapheBiparti(const std::vector<std::vector<AreteAlgo>> &voisins, std::vector<int> &X, std::vector<int> &Y) const;
    //Effectue un parcours en largeur pour le couplage maximum
    //Objectif : localiser les departs et arrivee de chemins augmentants
    //departs contient les points de depart du parcours (noeuds libres)
    //dist contient la distance de chaque noeud au depart
    //libresTrouves contient les noeuds de fin de parcours
    void bfsCouplageMaxBiparti(const std::vector<int> &departs, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<bool> &estLibre, const std::vector<bool> &estDansCouplage, std::vector<int> &dist, std::vector<int> &libresTrouves) const;
    //Effectue un parcours en profondeur pour le couplage maximum
    //Objectif : trouve un chemin augmentant
    //chemin contient les aretes du chemin augmentant (vide si non trouve)
    bool dfsCouplageMaxBiparti(int noeud, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<int> &dist, const std::vector<bool> &estLibre, const std::vector<bool> &estDansCouplage, std::vector<bool> &dejaFait, std::vector<AreteAlgo> &chemin) const;
    //Trouve le couplage maximal dans un graphe biparti
    //Renvoie le tableau des aretes faisant partie du couplage
    //Complexite : O(E^(3/2))
    std::vector<int> couplageMaxBiparti(const std::vector<std::vector<AreteAlgo>> &voisins) const;
    //Effectue un bfs pour le recouvrement minimal
    //departs contient les points de depart du parcours
    //vertexCover contient les noeuds du recouvrement minimal
    //restreint est a vrai si les points de departs sont des noeuds libres
    void bfsMinVertexCoverBipartite(const std::vector<int> &departs, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<bool> &estDansCouplage, std::vector<bool> &dejaFait, std::vector<int> &vertexCover, bool restreint) const;
    //Trouve le recouvrement minimal des sommets d'un graphe biparti
    //Renvoie le tableau des noeuds faisant partie du recouvrement
    //Complexite : O(E^(3/2))
    std::vector<int> minVertexCoverBipartite(const std::vector<std::vector<AreteAlgo>> &voisins) const;
    //Calcule la meilleure repartition des entrees sur les sorties en fonction de la matrice des couts donnee
    //c[i][j] doit contenir combien cela coute d'assigner l'entree i a la sortie j
    //Renvoie le tableau qui contient, pour chaque entree i, la sortie j qui lui est assignee (-1 si aucun assignement)
    //Complexite : O(n^3)
    std::vector<int> assignementProblem(const std::vector<std::vector<double>> &c) const;

    //Calcule une bonne repartition des entrees sur les sorties en fonction de la matrice des couts donnee
    //Voir assignementProblem pour des precisions d'utilisation
    //Complexite : O(n^2log(n)) (plus rapide mais moins precis)
    std::vector<int> greedyAssignementProblem(const std::vector<std::vector<double>> &c) const;
};

#endif