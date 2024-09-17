#ifndef MCTS_H_INCLUDED
#define MCTS_H_INCLUDED
#include "neural_network.h"
#include "game_state.h"
#include "tree_mcts.h"
#include "graphe.h"
using namespace NeuralNetwork;

class MCTS
{
public:
    // net est le reseau de neurone utilise par le MCTS
    // state est l'etat du jeu sur lequel il faut appliquer le MCTS
    MCTS(const Network &net, const GameState &state, const Graphe &G, double crossMed);

    // Execute nbSteps simulations du MCTS
    void execute(int nbSteps);

    // Renvoie le tableau des probabilites pour chaque coup
    Matrix<double> getProbabilities() const;
    // Renvoie l'emplacement pour le meilleur coup
    int getBestMove() const;
private:
    Network network;
    GameState startGameState;
    TreeMCTS gameTree;
    Graphe graph;
    double crossMedian;
};

#endif // MCTS_H_INCLUDED
