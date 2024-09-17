#ifndef NEURONE_LAYER_H_INCLUDED
#define NEURONE_LAYER_H_INCLUDED
#include "matrix.h"
#include "layer.h"
#include "activation_function_layer.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace NeuralNetwork
{

double Identity(double x);
double IdentityD(double x);
const ActivationFunction IdentityF = {Identity, IdentityD, "Identity"};
double Relu(double x);
double ReluD(double x);
const ActivationFunction ReluF = {Relu, ReluD, "Relu"};
double Sigmoid(double x);
double SigmoidD(double x);
const ActivationFunction SigmoidF = {Sigmoid, SigmoidD, "Sigmoid"};

class NeuroneLayer : public Layer
{
public:
    NeuroneLayer();
    // w est le tableau des poids, b est le tableau des biais, f est la fonction d'activation
    NeuroneLayer(const Matrix<double> &w, const Matrix<double> &b, ActivationFunction f = IdentityF);
    // nbInputs est le nombre de neurones a l'entree de la couche, nbOutputs est le nombre de neurones a la sortie de la couche
    NeuroneLayer(int nbInputs, int nbOutputs, ActivationFunction f);
    // les valeurs des tableaux de poids et de biais sont initialises aleatoiremenet dans l'intervalle [mini, maxi]
    NeuroneLayer(int nbInputs, int nbOutputs, double mini = 0.0, double maxi = 1.0, ActivationFunction f = IdentityF);
    NeuroneLayer(const NeuroneLayer &n);

    NeuroneLayer* clone() const override;

    Matrix<double> applyLayer(const Matrix<double> &m) const override;
    Matrix<double> applyLayer(const Matrix<double> &m) override;
    void calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const override;
    void applyGradient(const Layer& gradient, double coef) override;

    // Tableau des poids
    Matrix<double> getWeights() const;
    // Tableau des biais
    Matrix<double> getBias() const;
    // Fonction d'activation
    ActivationFunction getActivationFunction() const;
    // Nombre de neurones d'entree
    int getNbInputs() const override;
    // Nombre de neurones de sortie
    int getNbOutputs() const override;
    char getType() const override;

    // Tableau des poids
    void setWeights(const Matrix<double> &m);
    // Tableau des biais
    void setBias(const Matrix<double> &m);
    // Fonction d'activation
    void setActivationFunction(const ActivationFunction &f);

    // Toutes les operations sont faites sur le tableau des poids et des biais
    NeuroneLayer& operator+=(const Layer &L) override;
    NeuroneLayer& operator-=(const Layer &L) override;
    NeuroneLayer& operator*=(double k) override;
    NeuroneLayer& operator/=(double k) override;

    // Affichage et lecture du tableau des poids puis du tableau des biais
    void print(ostream &ost) const override;
    void read(istream& ist) override;
private:
    Matrix<double> weights;
    Matrix<double> bias;
    ActivationFunctionLayer activationLayer;
};

// Toutes les operations sont faites sur le tableau des poids et des biais
NeuroneLayer operator+(const NeuroneLayer &A, const NeuroneLayer &B);
NeuroneLayer operator-(const NeuroneLayer &A, const NeuroneLayer &B);
NeuroneLayer operator-(const NeuroneLayer &n);
NeuroneLayer operator*(double k, const NeuroneLayer &n);
NeuroneLayer operator*(const NeuroneLayer &n, double k);
NeuroneLayer operator/(const NeuroneLayer &n, double k);

}

#endif // NEURONE_LAYER_H_INCLUDED
