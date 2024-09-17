#include "training_data_generator.h"
#include "utilitaire.h"
#include "tree_mcts.h"
#include <cmath>

TrainingDataGenerator::TrainingDataGenerator(const Network &net, const GameState &initState, const Graphe &G, double crossMed):
    gameNetwork{net}, startGameState{initState}, graph{G}, games{}, crossMedian{crossMed}
{
    
}

void TrainingDataGenerator::playGames(int nbGames, int turnCountMax, int nbSimulationMCTS)
{
    for (int iGame = 0; iGame < nbGames; iGame++)
    {
        Game game{gameNetwork, startGameState, graph, crossMedian};
        game.executeGame(turnCountMax, nbSimulationMCTS);
        games.push_back(game);
    }
}

void TrainingDataGenerator::resetGameData()
{
    games.clear();
}

DataBatch TrainingDataGenerator::generateTrainingDataBatch(int batchSize) const
{
    DataBatch batch;
    Graphe G{graph};
    for (int iGame = 0; iGame < (int)games.size(); iGame++)
    {
        GameState finalState = games[iGame].getFinalGameState();
        std::vector<int> nodeData = finalState.getNodeData();
        G.placeNoeuds(nodeData);
        double zScore = TreeMCTS::calcZScore(G.getScoreTotal(), crossMedian);
        for (int iTurn = 0; iTurn < games[iGame].getNbTurns(); iTurn++)
        {
            GameState state = games[iGame].getGameState(iTurn);
            Matrix<double> input = state.getAsInputMatrix();
            Matrix<double> prob = games[iGame].getTurnResult(iTurn);
            Matrix<double> output(prob.height()+1, 1);
            for (int i = 0; i < prob.height(); i++) output[i][0] = prob[i][0];
            output[output.height()-1][0] = zScore;
            DataPiece dataPiece = {input, output};
            batch.dataPieces.push_back(dataPiece);
        }
    }
    melangeTab(batch.dataPieces);
    while (batch.size() > batchSize)
    {
        batch.dataPieces.pop_back();
    }
    return batch;
}

Network TrainingDataGenerator::getGameNetwork() const
{
    return gameNetwork;
}

GameState TrainingDataGenerator::getStartGameState() const
{
    return startGameState;
}

Graphe TrainingDataGenerator::getGraph() const
{
    return graph;
}

int TrainingDataGenerator::getNbGames() const
{
    return games.size();
}

std::vector<Game> TrainingDataGenerator::getGames() const
{
    return games;
}

Game TrainingDataGenerator::getGame(int game) const
{
    return games[game];
}

void TrainingDataGenerator::setGameNetwork(const Network &net)
{
    gameNetwork = net;
}

void TrainingDataGenerator::setStartGameState(const GameState &initState)
{
    startGameState = initState;
}

void TrainingDataGenerator::setGraph(const Graphe &G)
{
    graph = G;
}