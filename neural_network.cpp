#include "neural_network.h"

namespace NeuralNetwork
{

int DataBatch::size() const
{
    return (int)dataPieces.size();
}

DataPiece& DataBatch::operator[](int n)
{
    return dataPieces[n];
}

const DataPiece& DataBatch::operator[](int n) const
{
    return dataPieces[n];
}

Network::Network(const FullyConnectedLayer &f):
    network{nullptr}, costs{}
{
    network = f.clone();
}

Network::Network(const Network &n):
    network{nullptr}, costs{n.costs}
{
    network = n.network->clone();
}

Network::~Network()
{
    delete network;
}

Network& Network::operator=(const Network &n)
{
    network = n.network->clone();
    costs = n.costs;
    return *this;
}

const FullyConnectedLayer& Network::getNetwork() const
{
    return *network;
}

void Network::applyBatch(const DataBatch &batch, double coef)
{
    if (costs.empty())
    {
        addCost(batch);
    }

    std::vector<FullyConnectedLayer*> gradients;
    for (int i = 0; i < batch.size(); i++)
    {
        network->applyLayers(batch[i].input);
        gradients.push_back(network->clone()); //Pour garder le meme type que network
        Matrix<double> dInput;
        network->calcBackpropagation(batch[i].expectedOutput, dInput, *gradients[i]);
    }

    FullyConnectedLayer* finalGradient = gradients[0]->clone();
    for (int i = 1; i < batch.size(); i++)
    {
        *finalGradient += *gradients[i];
    }
    *finalGradient /= batch.size();

    network->applyGradient(*finalGradient, coef);
    addCost(batch);

    delete finalGradient;
    for (int i = 0; i < (int)gradients.size(); i++) delete gradients[i];
}

void Network::addCost(const DataBatch &batch)
{
    double C = 0;
    for (int i = 0; i < batch.size(); i++)
    {
        network->applyLayers(batch[i].input);
        C += network->calcCost(batch[i].expectedOutput);
    }
    C /= batch.size();
    costs.push_back(C);
}

void Network::clearCosts()
{
    costs.clear();
}

int Network::getNbCosts() const
{
    return (int)costs.size();
}

std::vector<double> Network::getCosts() const
{
    return costs;
}

double Network::getCost(int n) const
{
    return costs[n];
}

double Network::getLastCost() const
{
    return costs.back();
}

void Network::setNetwork(const FullyConnectedLayer &n)
{
    delete network;
    network = n.clone();
}

ostream& operator<<(ostream &ost, const Network &net)
{
    ost << "Network :\n" << net.getNetwork();
    ost << "Costs : ";
    for (double v : net.getCosts()) ost << v << " ";
    ost << "\n";
    return ost;
}

}
