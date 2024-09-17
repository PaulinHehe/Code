#include "activation_function_layer.h"

namespace NeuralNetwork
{

ActivationFunctionLayer::ActivationFunctionLayer(const ActivationFunction &f):
    activationFunction{f}
{

}

ActivationFunctionLayer* ActivationFunctionLayer::clone() const
{
    return new ActivationFunctionLayer(*this);
}

Matrix<double> ActivationFunctionLayer::applyLayer(const Matrix<double> &m) const
{
    Matrix<double> ans = apply(activationFunction.func, m);
    return ans;
}

Matrix<double> ActivationFunctionLayer::applyLayer(const Matrix<double> &m)
{
    input = m;
    output = apply(activationFunction.func, m);
    return output;
}

void ActivationFunctionLayer::calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const
{
    dInput = dOutput & apply(activationFunction.derivative, input);
    gradient = clone();
}

void ActivationFunctionLayer::print(ostream& ost) const
{
    ost << "activation function : " << activationFunction.funcName << "\n";
}

ActivationFunction ActivationFunctionLayer::getActivationFunction() const
{
    return activationFunction;
}

void ActivationFunctionLayer::setActivationFunction(const ActivationFunction &f)
{
    activationFunction = f;
    output = Matrix<double>{};
}

}
