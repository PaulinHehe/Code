#include "tree_mcts.h"
#include "fully_connected_layer.h"
#include <cmath>

TreeMCTS::TreeMCTS(const Network &net, const GameState &initState, double temp):
    children{{}}, parent{{-1, -1, 0, -1.0}}, value{-1.0}, temperature{temp}, startGameState{initState}, currentGameState{initState}
{
    init(net);
}

void TreeMCTS::init(const Network& net)
{
    expandNode(net, 0);
}

void TreeMCTS::executeStep(const Network &net, Graphe &G, double crossMed)
{
    executeSearchNode(net, G, 0, crossMed);
}

double TreeMCTS::executeSearchNode(const Network &net, Graphe &G, int node, double crossMed)
{
    if (currentGameState.isGameFinished() || value[node] == -1)
    {
        if (value[node] == -1)
        {
            if (currentGameState.isGameFinished())
            {
                G.placeNoeuds(currentGameState.getNodeData());
                value[node] = calcZScore(G.getScoreTotal(), crossMed);
            }
            else
            {
                expandNode(net, node);
            }
        }
        parent[node].addVvalue(value[node]);
        return value[node];
    }
    int bestSlot = -1;
    int bestChild = -1;
    int bestNextNode = -1;
    double bestEdgeValue = -1;
    for (int i = 0; i < (int)children[node].size(); i++)
    {
        const EdgeMCTS &edge = children[node][i];
        int slot = edge.getSlotTransition();
        if (currentGameState.isSlotAvailable(slot))
        {
            double edgeValue = edge.getEdgeValue();
            if (bestSlot == -1 || edgeValue > bestEdgeValue)
            {
                bestSlot = slot;
                bestChild = i;
                bestNextNode = edge.getNextNodeMCTS();
                bestEdgeValue = edgeValue;
            }
        }
    }
    currentGameState.placeNode(bestSlot);
    double newValue = executeSearchNode(net, G, bestNextNode, crossMed);
    currentGameState.removeNodeFromSlot();
    children[node][bestChild].addVvalue(newValue);
    parent[node].addVvalue(newValue);
    return newValue;
}

void TreeMCTS::expandNode(const Network &net, int node)
{
    Matrix<double> input = currentGameState.getAsInputMatrix();
    const FullyConnectedLayer& layers = net.getNetwork();
    Matrix<double> output = layers.applyLayers(input);
    int nbSlots = currentGameState.getNbSlots();
    int nodeT = currentGameState.getNextNode();
    double V = output[nbSlots][0];
    value[node] = V;
    for (int slot = 0; slot < nbSlots; slot++)
    {
        double P = output[slot][0];
        children[node].push_back(EdgeMCTS{nodeT, slot, (int)value.size(), P});
        children.push_back({});
        parent.push_back(EdgeMCTS{nodeT, slot, node, P});
        value.push_back(-1.0);
    }
}

Matrix<double> TreeMCTS::getProbabilities() const
{
    int nbSlots = startGameState.getNbSlots();
    Matrix<double> prob{nbSlots, 1};
    for (const EdgeMCTS &edge : children[0])
    {
        int slot = edge.getSlotTransition();
        if (currentGameState.isSlotAvailable(edge.getSlotTransition())) prob[slot][0] = pow(edge.getNvalue(), 1.0 / temperature);
        else prob[slot][0] = 0.0;
    }
    prob /= prob.sum();
    return prob;
}

int TreeMCTS::getBestMove() const
{
    Matrix<double> prob = getProbabilities();
    int bestSlot = 0;
    for (int slot = 0; slot < startGameState.getNbSlots(); slot++)
    {
        if (prob[slot][0] > prob[bestSlot][0])
        {
            bestSlot = slot;
        }
    }
    return bestSlot;
}

std::vector<std::vector<EdgeMCTS>> TreeMCTS::getChildren() const
{
    return children;
}

double TreeMCTS::calcZScore(long long nbInter, double crossMed)
{
    double ratio;
    if (nbInter == 0) ratio = 10; //Valeur arbitrairement grosse
    else if (crossMed == 0) ratio = 0.5 / nbInter; //Petite punition pour ne pas avoir trouve l'optimum
    else ratio = crossMed / nbInter;
    return 0.2 * log2(ratio);
}