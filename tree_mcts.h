#ifndef TREE_MCTS_H_INCLUDED
#define TREE_MCTS_H_INCLUDED
#include "neural_network.h"
#include "edge_mcts.h"
#include "game_state.h"
#include "graphe.h"
#include <vector>
using namespace std;
using namespace NeuralNetwork;

class TreeMCTS
{
public:
    // net est le reseau de neurone utilise par l'arbre
    // initState est l'etat de la racine de l'arbre
    // temp est la temperature utilisee pour le MCTS
    TreeMCTS(const Network &net, const GameState &initState, double temp);
    // Initialise le score de la racine
    void init(const Network &net);

    // Execute une etape du MCTS
    void executeStep(const Network &net, Graphe &G, double crossMed);
    // Execute une etape de recherche du meilleur noeud pour le MCTS (recursif)
    double executeSearchNode(const Network &net, Graphe &G, int node, double crossMed);
    // Calcule le score du noeud de l'arbre et les probabilites pour les coups suivants
    void expandNode(const Network &net, int node);

    // Renvoie le tableau des probabilites pour le premier coup
    Matrix<double> getProbabilities() const;
    // Renvoie l'emplacement du meilleur premier coup de l'arbre
    int getBestMove() const;

    // Calcule le score final d'une partie
    // nbInter est le nombre d'intersections
    // crossMed est le score median
    static double calcZScore(long long nbInter, double crossMed);


    ////// DEBUG
    std::vector<std::vector<EdgeMCTS>> getChildren() const;
private:
    std::vector<std::vector<EdgeMCTS>> children;
    std::vector<EdgeMCTS> parent;
    std::vector<double> value;
    double temperature;
    GameState startGameState;
    GameState currentGameState;
};

#endif // TREE_MCTS_H_INCLUDED
