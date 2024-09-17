#ifndef SOFTMAX_LAYER_ALPHA_GO_ZERO_H_INCLUDED
#define SOFTMAX_LAYER_ALPHA_GO_ZERO_H_INCLUDED
#include "softmax_layer.h"
#include "neurone_layer.h" //Sigmoid
using namespace std;
using namespace NeuralNetwork;

class SoftmaxLayerAlphaGoZero : public SoftmaxLayer
{
    virtual SoftmaxLayerAlphaGoZero* clone() const override;

    virtual void calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const override;

    virtual string getTypeSoftmax() const override; //Debug and printing purpose

    virtual Matrix<double> applySoftmax(const Matrix<double> &m) const;
};

#endif // SOFTMAX_LAYER_ALPHA_GO_ZERO_H_INCLUDED
