#include "mcts.h"

MCTS::MCTS(const Network &net, const GameState &state, const Graphe &G, double crossMed):
    network{net}, startGameState{state}, gameTree{net, state, 1.0}, graph{G}, crossMedian{crossMed}
{

}

void MCTS::execute(int nbSteps)
{
    for (int step = 0; step < nbSteps; step++)
    {
        gameTree.executeStep(network, graph, crossMedian);
    }
}

Matrix<double> MCTS::getProbabilities() const
{
    return gameTree.getProbabilities();
}

int MCTS::getBestMove() const
{
    return gameTree.getBestMove();
}
