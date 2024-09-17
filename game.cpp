#include "game.h"
#include "utilitaire.h"

Game::Game(const Network &net, const GameState &initState, const Graphe &G, double crossMed):
    network{net}, gameStates{}, turnResults{}, startGameState{initState}, currentGameState{initState}, graph{G}, crossMedian{crossMed}
{

}

void Game::executeGame(int turnCountMax, int nbSimulationMCTS)
{
    if (turnCountMax == -1) turnCountMax = startGameState.getNbNodes();
    for (int turn = 0; turn < turnCountMax; turn++)
    {
        MCTS mcts{network, currentGameState, graph, crossMedian};
        mcts.execute(nbSimulationMCTS);

        Matrix<double> prob = mcts.getProbabilities();
        double randNum = generateRandDouble(0.0, 1.0);
        double cumulRand = 0.0;
        int slotNum = 0;
        while (cumulRand <= randNum)
        {
            cumulRand += prob[slotNum][0];
            slotNum++;
        }
        slotNum--;

        gameStates.push_back(currentGameState);
        currentGameState.placeNode(slotNum);
        turnResults.push_back(mcts.getProbabilities());
    }
    graph.placeNoeuds(currentGameState.getNodeData());
}

int Game::getNbTurns() const
{
    return (int)turnResults.size();
}

GameState Game::getGameState(int turn) const
{
    return gameStates[turn];
}

Matrix<double> Game::getTurnResult(int turn) const
{
    return turnResults[turn];
}

GameState Game::getFinalGameState() const
{
    return currentGameState;
}

GameState Game::getStartGameState() const
{
    return startGameState;
}

long long Game::getNbCrossings() const
{
    return graph.getScoreTotal();
}