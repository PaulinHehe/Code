#include "network_alpha_go_zero.h"

NetworkAlphaGoZero::NetworkAlphaGoZero(const Network &graphN, const Network &gameN):
    graphNetwork{graphN}, gameNetwork{gameN}
{
    
}

void NetworkAlphaGoZero::applyBatch(const DataBatch &batch, const Matrix<double> &graphNetInput, double coef)
{
    if (getNbCosts() == 0)
    {
        addCost(batch);
    }

    std::vector<FullyConnectedLayer*> graphGrad, gameGrad;
    FullyConnectedLayer* graphN = graphNetwork.getNetwork().clone();
    graphN->applyLayers(graphNetInput);
    FullyConnectedLayer* gameN = gameNetwork.getNetwork().clone();
    for (int i = 0; i < batch.size(); i++)
    {
        gameN->applyLayers(batch[i].input);
        graphGrad.push_back(graphN->clone());
        gameGrad.push_back(gameN->clone());
        Matrix<double> dInput;
        gameN->calcBackpropagation(batch[i].expectedOutput, dInput, *gameGrad[i]);
        Matrix<double> dInputResize(graphN->getNbOutputs(), 1);
        for (int j = 0; j < dInputResize.height(); j++)
        {
            dInputResize[j][0] = dInput[j][0];
        }
        graphN->calcBackpropagationFromOutputGradient(dInputResize, dInput, *graphGrad[i]);
    }

    FullyConnectedLayer* finalGraphGrad = graphGrad[0]->clone();
    FullyConnectedLayer* finalGameGrad = gameGrad[0]->clone();
    for (int i = 1; i < batch.size(); i++)
    {
        *finalGraphGrad += *graphGrad[i];
        *finalGameGrad += *gameGrad[i];
    }
    *finalGraphGrad /= batch.size();
    *finalGameGrad /= batch.size();

    graphN->applyGradient(*finalGraphGrad, coef);
    gameN->applyGradient(*finalGameGrad, coef);
    graphNetwork.setNetwork(*graphN);
    gameNetwork.setNetwork(*gameN);
    addCost(batch);

    delete finalGraphGrad;
    delete finalGameGrad;
    for (int i = 0; i < (int)graphGrad.size(); i++) delete graphGrad[i];
    for (int i = 0; i < (int)gameGrad.size(); i++) delete gameGrad[i];
}

void NetworkAlphaGoZero::applyBatchGameOnly(const DataBatch &batch, double coef)
{
    gameNetwork.applyBatch(batch, coef);
}

void NetworkAlphaGoZero::addCost(const DataBatch &batch)
{
    gameNetwork.addCost(batch);
}

void NetworkAlphaGoZero::clearCosts()
{
    gameNetwork.clearCosts();
}

Network NetworkAlphaGoZero::getGraphNetwork() const
{
    return graphNetwork;
}

Network NetworkAlphaGoZero::getGameNetwork() const
{
    return gameNetwork;
}

int NetworkAlphaGoZero::getNbCosts() const
{
    return gameNetwork.getNbCosts();
}

std::vector<double> NetworkAlphaGoZero::getCosts() const
{
    return gameNetwork.getCosts();
}

double NetworkAlphaGoZero::getCost(int n) const
{
    return gameNetwork.getCost(n);
}

double NetworkAlphaGoZero::getLastCost() const
{
    return gameNetwork.getLastCost();
}

void NetworkAlphaGoZero::setGraphNetwork(const Network &graphN)
{
    graphNetwork = graphN;
}

void NetworkAlphaGoZero::setGameNetwork(const Network &gameN)
{
    gameNetwork = gameN;
}

ostream& operator<<(ostream& ost, const NetworkAlphaGoZero &net)
{
    ost << "Graph network :\n" << net.getGraphNetwork();
    ost << "Game network :\n" << net.getGameNetwork();
    return ost;
}
