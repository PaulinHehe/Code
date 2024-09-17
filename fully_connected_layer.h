#ifndef FULLY_CONNECTED_LAYER_H_INCLUDED
#define FULLY_CONNECTED_LAYER_H_INCLUDED
#include "neurone_layer.h"
#include "layer.h"
#include "matrix.h"
#include <vector>
#include <iostream>
using namespace std;

namespace NeuralNetwork
{

const double COEF_INFINI = 1e9; //Infini utilise pour les coefficients des couches

class operationNbLayerError : public exception {};
class outputEmptyOrIncorrectError : public exception {};

// cost renvoie le cout etant donne la matrice de sortie obtenue et la matrice de sortie attendue
// outputDerivative renvoie le gradient de la matrice de sortie etant donne la matrice de sortie obtenue et la matrice de sortie attendue
struct CostFunction
{
    double (*cost)(const Matrix<double>&, const Matrix<double>&);
    Matrix<double> (*outputDerivative)(const Matrix<double>&, const Matrix<double>&);
    string funcName = "unknown";
};

// Erreur quadratique moyenne
double MSE(const Matrix<double> &An, const Matrix<double> &Y);
Matrix<double> MSED(const Matrix<double> &An, const Matrix<double> &Y);
const CostFunction MSEF = {MSE, MSED, "Mean Square Error"};
// Entropie croisee
double inverseCrossEntropy(double x);
double logCrossEntropy(double x);
double CrossEntropy(const Matrix<double> &An, const Matrix<double> &Y);
Matrix<double> CrossEntropyD(const Matrix<double> &An, const Matrix<double> &Y);
const CostFunction CrossEntropyF = {CrossEntropy, CrossEntropyD, "Cross Entropy"};

class FullyConnectedLayer
{
public:
    FullyConnectedLayer();
    // Initialisation a partir de la liste des couches et de la fonction de cout
    FullyConnectedLayer(const std::vector<Layer*> &l, const CostFunction &c = {MSE, MSED});
    // nbNeurones contient le nombre de neurones entre chaque couche utilise pour initialiser des NeuroneLayer, f est leur fonction d'activation
    // endLayers contient les couches a rajouter apres les NeuroneLayer
    // startLayers contient les couches a rajouter avant les NeuroneLayer
    FullyConnectedLayer(const std::vector<int> &nbNeurones, const ActivationFunction &f, const CostFunction &c = {MSE, MSED}, const std::vector<Layer*>& endLayers = {}, const std::vector<Layer*>& startLayers = {});
    // Toutes les valeurs des NeuroneLayer sont initialisees aleatoirement entre [mini, maxi]
    FullyConnectedLayer(const std::vector<int> &nbNeurones, double mini = 0.0, double maxi = 1.0, const ActivationFunction &f = {Identity, IdentityD}, const CostFunction &c = {MSE, MSED}, const std::vector<Layer*>& endLayers = {}, const std::vector<Layer*>& startLayers = {});
    FullyConnectedLayer(const FullyConnectedLayer &f);

    virtual ~FullyConnectedLayer();

    FullyConnectedLayer& operator=(const FullyConnectedLayer& f);
    // Pour le polymorphisme
    virtual FullyConnectedLayer* clone() const;

    // Applique la matrice d'entree sur la couche dense
    // Renvoie la matrice de sortie
    // Met a jour input et output pour la version modifiable
    virtual Matrix<double> applyLayers(const Matrix<double> &m) const;
    virtual Matrix<double> applyLayers(const Matrix<double> &m);
    // A appeler APRES avoir applique une entree sur la couche dense
    // Effectue la backpropagation sur l'ensemble des couches
    // Y est la sortie attendue
    // gradient est le gradient de la couche dense
    virtual void calcBackpropagation(const Matrix<double> &Y, Matrix<double> &dInput, FullyConnectedLayer &gradient) const;
    virtual void calcBackpropagationFromOutputGradient(const Matrix<double> &dOutput, Matrix<double> &dInput, FullyConnectedLayer &gradient) const;
    // Effectue la descente de gradient sur chaque couche de la couche dense
    // Coef mutliplie le gradient
    void applyGradient(const FullyConnectedLayer &gradient, double coef = 1.0);
    // A appeler APRES avoir applique une entree sur la couche dense
    // Calcule le cout, Y est la sortie attendue
    double calcCost(const Matrix<double> &Y) const;

    // Renvoie le nombre de couches
    int getNbLayers() const;
    // Renvoie la liste des couches
    std::vector<Layer*> getLayers() const;
    // Renvoie la couche a l'indice n
    Layer& operator[](int n);
    const Layer& operator[](int n) const;
    // Renvoie la fonction de cout
    CostFunction getCostFunction() const;
    // Renvoie la derniere matrice utilisee pour l'entree
    Matrix<double> getInput() const;
    // Renvoie la derniere matrice obtenue en sortie
    Matrix<double> getOutput() const;
    // Renvoie le nombre de neurones d'entree, -1 si taille variable
    int getNbInputs() const;
    // Renvoie le nombre de neurones de sortie, -1 si taille variable
    int getNbOutputs() const;
    // For debug purposes
    virtual char getType() const;

    void setCostFunction(const CostFunction &c);
    // Ajoute une couche a la couche dense
    void addLayer(const Layer &n);
    // Supprime la derniere couche
    void deleteLastLayer();

    // Les operations sont effectuees sur les coefficients des couches
    FullyConnectedLayer& operator+=(const FullyConnectedLayer &f);
    FullyConnectedLayer& operator-=(const FullyConnectedLayer &f);
    FullyConnectedLayer& operator*=(double k);
    FullyConnectedLayer& operator/=(double k);

    // Affichage et lecture de chaque couche dans l'ordre
    virtual void print(ostream &ost) const;
    virtual void read(istream &ist);
protected:
    std::vector<Layer*> layers;
    CostFunction costFunction;
    Matrix<double> input;
    Matrix<double> output;
};

// Les operations sont effectuees sur les coefficients des couches
FullyConnectedLayer operator+(const FullyConnectedLayer &A, const FullyConnectedLayer &B);
FullyConnectedLayer operator-(const FullyConnectedLayer &A, const FullyConnectedLayer &B);
FullyConnectedLayer operator-(const FullyConnectedLayer &f);
FullyConnectedLayer operator*(double k, const FullyConnectedLayer &f);
FullyConnectedLayer operator*(const FullyConnectedLayer &f, double k);
FullyConnectedLayer operator/(const FullyConnectedLayer &f, double k);
ostream& operator<<(ostream& ost, const FullyConnectedLayer &f);
istream& operator>>(istream& ist, FullyConnectedLayer &f);

}

#endif // FULLY_CONNECTED_LAYER_H_INCLUDED
