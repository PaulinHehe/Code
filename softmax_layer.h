#ifndef SOFTMAX_LAYER_H_INCLUDED
#define SOFTMAX_LAYER_H_INCLUDED
#include "matrix.h"
#include "layer.h"
#include <iostream>
using namespace std;

namespace NeuralNetwork
{

class SoftmaxLayer : public Layer
{
public:
    virtual SoftmaxLayer* clone() const override;

    Matrix<double> applyLayer(const Matrix<double> &m) const override;
    Matrix<double> applyLayer(const Matrix<double> &m) override;
    virtual void calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const override;
    // Applique la formule de backpropagation sur l'intervalle donne
    // ligDep est inclus, ligFin est exclus
    void calcBackpropagationInterval(const Matrix<double> &dOutput, Matrix<double> &dInput, int ligDep, int ligFin) const;
    void applyGradient(const Layer& gradient, double coef) override;

    // Debug and printing purpose
    virtual string getTypeSoftmax() const;
    void print(ostream& ost) const override;
    void read(istream& ist) override;

    // Applique la fonction softmax sur toute la matrice
    virtual Matrix<double> applySoftmax(const Matrix<double> &m) const;
    // Applique la fonction softmax sur l'intervalle selectionne de la matrice
    // ligDep est inclus, ligFin est exclus
    void applySoftmaxInterval(Matrix<double> &m, int ligDep, int ligFin) const;
};

}

#endif // SOFTMAX_LAYER_H_INCLUDED
