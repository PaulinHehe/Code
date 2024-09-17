#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED
#include "matrix.h"
#include <iostream>
using namespace std;

namespace NeuralNetwork
{

class Layer
{
public:
    Layer();
    Layer(const Layer& l);

    virtual ~Layer() = default;

    // Pour le polymorphisme
    virtual Layer* clone() const = 0;

    // Calcule la matrice de sortie de la couche
    // m est la matrice des neurones d'entree
    // Met input et output a jour pour la version modifiable
    virtual Matrix<double> applyLayer(const Matrix<double> &m) const = 0;
    virtual Matrix<double> applyLayer(const Matrix<double> &m) = 0;
    // A appeler APRES avoir applique une entree sur la couche
    // Effectue la backpropagation sur la couche en question
    // dOutput : gradient des neurones de sortie
    // dInput : gradient des neurones d'entree
    // gradient : gradient de la couche
    virtual void calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const = 0;
    // Effectue la descente de gradient sur la couche actuelle
    // coef est utilisee pour multiplier le gradient
    virtual void applyGradient(const Layer& gradient, double coef);

    // Renvoie la derniere matrice utilisee pour l'entree
    Matrix<double> getInput() const;
    // Renvoie la derniere matrice obtenue en sortie
    Matrix<double> getOutput() const;
    // Renvoie le nombre de neurones d'entree, -1 si taille variable
    virtual int getNbInputs() const;
    // Renvoie le nombre de neurones de sortie, -1 si taille variable
    virtual int getNbOutputs() const;
    // Debug purpose
    virtual char getType() const;

    // Affichage et lecture des coefficients de la couche
    virtual void print(ostream& ost) const;
    virtual void read(istream& ist);

    // Operations effectuees sur les coefficients de la couche
    virtual Layer& operator+=(const Layer& L);
    virtual Layer& operator-=(const Layer& L);
    virtual Layer& operator*=(double k);
    virtual Layer& operator/=(double k);
protected:
    Matrix<double> input;
    Matrix<double> output;
};

ostream& operator<<(ostream& ost, const Layer &l);
istream& operator>>(istream& ist, Layer &l);

}

#endif // LAYER_H_INCLUDED
