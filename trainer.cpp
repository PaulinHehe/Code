#include "trainer.h"

Trainer::Trainer(const NetworkAlphaGoZero &net, const Graphe &G, int batchS, double batchC, int nbG, int turnCM, int nbSimMCTS, double crossMed):
    network{net}, dataGenerator{net.getGameNetwork(), GameState{{}, {}, {}, {}}, G, crossMed}, graph{G}, batchSize{batchS}, batchCoef{batchC}, nbGames{nbG}, turnCountMax{turnCM}, nbSimulationMCTS{nbSimMCTS}, crossMedian{crossMed}, crossEvolution{}
{
    GameState initState = getStartGameState();
    dataGenerator.setStartGameState(initState);
    std::vector<int> places = playGame();
    graph.placeNoeuds(places);
    crossEvolution.push_back(graph.getScoreTotal());
}

void Trainer::trainNetwork(int nbBatches)
{
    for (int iBatch = 0; iBatch < nbBatches; iBatch++)
    {
        if (iBatch % (nbBatches/10) == nbBatches/10-1)
        {
            cout << "Batch " << iBatch+1 << "\n";
        }

        GameState initState = getStartGameState();
        dataGenerator.setStartGameState(initState);
        dataGenerator.resetGameData();
        dataGenerator.playGames(nbGames, turnCountMax, nbSimulationMCTS);
        DataBatch batch = dataGenerator.generateTrainingDataBatch(batchSize);

        double sumCross = 0.0;
        for (int i = 0; i < dataGenerator.getNbGames(); i++)
        {
            sumCross += dataGenerator.getGame(i).getNbCrossings();
        }
        sumCross /= dataGenerator.getNbGames();
        crossEvolution.push_back(sumCross);

        std::vector<std::vector<int>> matAdj = graph.getMatriceAdjacence();
        Matrix<double> graphNetInput(graph.getNbNoeuds() * graph.getNbNoeuds(), 1);
        for (int i = 0; i < graph.getNbNoeuds(); i++)
        {
            for (int j = 0; j < graph.getNbNoeuds(); j++)
            {
                graphNetInput[i*graph.getNbNoeuds() + j][0] = matAdj[i][j];
            }
        }

        network.applyBatch(batch, graphNetInput, batchCoef);
    }
}

void Trainer::trainGameNetworkOnly(int nbBatches)
{
    for (int iBatch = 0; iBatch < nbBatches; iBatch++)
    {
        if (iBatch % (nbBatches/10) == nbBatches/10-1)
        {
            cout << "Batch " << iBatch+1 << "\n";
        }

        dataGenerator.resetGameData();
        dataGenerator.playGames(nbGames, turnCountMax, nbSimulationMCTS);
        DataBatch batch = dataGenerator.generateTrainingDataBatch(batchSize);
        
        double sumCross = 0.0;
        for (int i = 0; i < dataGenerator.getNbGames(); i++)
        {
            sumCross += dataGenerator.getGame(i).getNbCrossings();
        }
        sumCross /= dataGenerator.getNbGames();
        crossEvolution.push_back(sumCross);

        network.applyBatchGameOnly(batch, batchCoef);
    }
}

std::vector<int> Trainer::playGame()
{
    Game finalGame{network.getGameNetwork(), getStartGameState(), graph, crossMedian};
    finalGame.executeGame(turnCountMax, nbSimulationMCTS);
    GameState finalState = finalGame.getFinalGameState();
    std::vector<int> nodeData = finalState.getNodeData();
    return nodeData;
}

NetworkAlphaGoZero Trainer::getNetwork() const
{
    return network;
}

TrainingDataGenerator Trainer::getTrainingDataGenerator() const
{
    return dataGenerator;
}

Graphe Trainer::getGraph() const
{
    return graph;
}

int Trainer::getBatchSize() const
{
    return batchSize;
}

double Trainer::getBatchCoef() const
{
    return batchCoef;
}

int Trainer::getNbGames() const
{
    return nbGames;
}

int Trainer::getTurnCountMax() const
{
    return turnCountMax;
}

int Trainer::getNbSimulationMCTS() const
{
    return nbSimulationMCTS;
}

GameState Trainer::getStartGameState() const
{
    std::vector<std::vector<int>> matAdj = graph.getMatriceAdjacence();
    Matrix<double> input(graph.getNbNoeuds() * graph.getNbNoeuds(), 1);
    for (int i = 0; i < graph.getNbNoeuds(); i++)
    {
        for (int j = 0; j < graph.getNbNoeuds(); j++)
        {
            input[i*graph.getNbNoeuds() + j][0] = matAdj[i][j];
        }
    }
    Matrix<double> gAtt = network.getGraphNetwork().getNetwork().applyLayers(input);
    std::vector<Position<int>> slotsPos;
    std::vector<int> slotData(graph.getNbEmplacements(), -1), nodeData(graph.getNbNoeuds(), -1);
    for (int i = 0; i < graph.getNbEmplacements(); i++)
    {
        slotsPos.push_back(graph.getEmplacement(i).getPos());
    }
    GameState initState(gAtt, slotsPos, slotData, nodeData);
    return initState;
}

int Trainer::getNbEvolutions() const
{
    return crossEvolution.size();
}

std::vector<double> Trainer::getCrossEvolution() const
{
    return crossEvolution;
}

double Trainer::getCross(int iBatch) const
{
    return crossEvolution[iBatch];
}

void Trainer::setNetwork(const NetworkAlphaGoZero &net)
{
    network = net;
}

void Trainer::setGraph(const Graphe &G)
{
    graph = G;
}

void Trainer::setBatchSize(int batchS)
{
    batchSize = batchS;
}

void Trainer::setBatchCoef(double batchC)
{
    batchCoef = batchC;
}

void Trainer::setNbGames(int nbG)
{
    nbGames = nbG;
}

void Trainer::setTurnCountMax(int turnCM)
{
    turnCountMax = turnCM;
}

void Trainer::setNbSimulationMCTS(int nbSimMCTS)
{
    nbSimulationMCTS = nbSimMCTS;
}

double AlphaGoZeroCost(const Matrix<double> &output, const Matrix<double> &Y)
{
    int n = output.height();
    double C = 0.0;
    for (int i = 0; i < n-1; i++)
    {
        C -= Y[i][0] * logCrossEntropy(output[i][0]);
    }
    double valC = output[n-1][0] - Y[n-1][0];
    C += valC * valC;
    return C;
}

Matrix<double> AlphaGoZeroCostD(const Matrix<double> &output, const Matrix<double> &Y)
{
    int n = output.height();
    Matrix<double> dOutput(n, 1);
    for (int i = 0; i < n-1; i++)
    {
        dOutput[i][0] = - Y[i][0] * inverseCrossEntropy(output[i][0]);
    }
    dOutput[n-1][0] = 2 * (output[n-1][0] - Y[n-1][0]);
    return dOutput;
}