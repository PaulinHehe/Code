#ifndef NETWORK_ALPHA_GO_ZERO_H_INCLUDED
#define NETWORK_ALPHA_GO_ZERO_H_INCLUDED
#include "neural_network.h"
using namespace NeuralNetwork;

class NetworkAlphaGoZero
{
public:
    // Cette classe contient deux reseaux de neurones dont la sortie de l'un est une partie de l'entree de l'autre
    // graphN est le reseau de neurone qui repere les caracteristiques du graphe
    // gameN est le reseau de neurone qui apprend a jouer une partie
    // graphN est utilise comme entree par gameN
    NetworkAlphaGoZero(const Network &graphN, const Network &gameN);

    // Applique le batch de donnees sur les deux reseaux
    // graphNetInput est la derniere entree donnee au reseau graphN
    void applyBatch(const DataBatch &batch, const Matrix<double> &graphNetInput, double coef);
    // Applique le batch de donnees sur le reseau de jeu uniquement
    void applyBatchGameOnly(const DataBatch &batch, double coef);
    // Calcule le cout du batch et l'ajoute a la liste
    void addCost(const DataBatch &batch);
    // Supprime tous les couts
    void clearCosts();

    // Renvoie le reseau pour le graphe
    Network getGraphNetwork() const;
    // Renvoie le reseau pour le jeu
    Network getGameNetwork() const;
    // Renvoie la taille de la liste des couts
    int getNbCosts() const;
    // Renvoie la liste des couts
    std::vector<double> getCosts() const;
    // Renvoie le cout numero n
    double getCost(int n) const;
    // Renvoie le dernier cout calcule
    double getLastCost() const;

    // Remplace le reseau pour le graphe
    void setGraphNetwork(const Network &graphN);
    // Remplace le reseau pour le jeu
    void setGameNetwork(const Network &gameN);
private:
    Network graphNetwork;
    Network gameNetwork;
};

ostream& operator<<(ostream& ost, const NetworkAlphaGoZero &net);

#endif // NETWORK_ALPHA_GO_ZERO_H_INCLUDED
