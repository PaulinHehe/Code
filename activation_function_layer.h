#ifndef ACTIVATION_FUNCTION_LAYER_H_INCLUDED
#define ACTIVATION_FUNCTION_LAYER_H_INCLUDED
#include "matrix.h"
#include "layer.h"
#include <iostream>
using namespace std;

namespace NeuralNetwork
{

// func contient la fonction d'activation
// derivative contient sa derivee
struct ActivationFunction
{
    double (*func)(double);
    double (*derivative)(double);
    string funcName = "unknown";
};

class ActivationFunctionLayer : public Layer
{
public:
    ActivationFunctionLayer(const ActivationFunction &f);
    ActivationFunctionLayer* clone() const override;

    Matrix<double> applyLayer(const Matrix<double> &m) const override;
    Matrix<double> applyLayer(const Matrix<double> &m) override;
    void calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const override;

    void print(ostream& ost) const override;

    ActivationFunction getActivationFunction() const;
    void setActivationFunction(const ActivationFunction &f);
private:
    ActivationFunction activationFunction;
};

}

#endif // ACTIVATION_FUNCTION_LAYER_H_INCLUDED
