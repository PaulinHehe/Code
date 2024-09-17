#ifndef NEURAL_NETWORK_H_INCLUDED
#define NEURAL_NETWORK_H_INCLUDED
#include "fully_connected_layer.h"
#include <vector>
using namespace std;

namespace NeuralNetwork
{

// Input contient les valeurs des neurones d'entree
// ExpectedOutput contient les valeurs attendues pour les neurones de sortie par rapport a l'entree donnee
struct DataPiece
{
    Matrix<double> input, expectedOutput;
};

struct DataBatch
{
    std::vector<DataPiece> dataPieces;
    
    int size() const;
    DataPiece& operator[](int n);
    const DataPiece& operator[](int n) const;
};

class Network
{
public:
    // f est la couche dense utilisee par le reseau de neurone
    Network(const FullyConnectedLayer &f);
    Network(const Network &n);

    ~Network();

    Network& operator=(const Network& n);

    // Applique chaque donnee du batch et effectue la backpropagation sur l'ensemble du batch
    // Coef est utilisee pour multiplier le gradient obtenu lors de la backpropagation
    void applyBatch(const DataBatch &batch, double coef = 1.0);
    // Ajoute une valeur de cout au tableau des couts
    // La valeur provient du resultat du batch en parametre
    void addCost(const DataBatch &batch);
    // Supprime tous les couts stockes
    void clearCosts();

    // Couche dense
    const FullyConnectedLayer& getNetwork() const;
    // Taille du tableau des couts
    int getNbCosts() const;
    // Tableau des couts
    std::vector<double> getCosts() const;
    // Cout a l'indice n
    double getCost(int n) const;
    // Cout du dernier batch
    double getLastCost() const;

    // Couche dense
    void setNetwork(const FullyConnectedLayer &n);
private:
    FullyConnectedLayer* network;
    std::vector<double> costs;
};

ostream& operator<<(ostream &ost, const Network &net);

}

#endif // NEURAL_NETWORK_H_INCLUDED
