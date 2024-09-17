#include "neurone_layer.h"

namespace NeuralNetwork
{

double Identity(double x)
{
    return x;
}

double IdentityD(double x)
{
    return 1;
}

double Relu(double x)
{
    return max(0.0, x);
}

double ReluD(double x)
{
    if (x > 0) return 1;
    return 0;
}

double Sigmoid(double x)
{
    return 1 / (1+exp(-x));
}

double SigmoidD(double x)
{
    double v = Sigmoid(x);
    return v * (1-v);
}

NeuroneLayer::NeuroneLayer():
    Layer{}, weights{}, bias{}, activationLayer{IdentityF}
{
    
}

NeuroneLayer::NeuroneLayer(const Matrix<double> &w, const Matrix<double> &b, ActivationFunction f):
    Layer{}, weights{w}, bias{b}, activationLayer{f}
{
    
}

NeuroneLayer::NeuroneLayer(int nbInputs, int nbOutputs, ActivationFunction f):
    Layer{}, weights{nbOutputs, nbInputs}, bias{nbOutputs, 1}, activationLayer{f}
{
    
}

NeuroneLayer::NeuroneLayer(int nbInputs, int nbOutputs, double mini, double maxi, ActivationFunction f):
    Layer{}, weights{nbOutputs, nbInputs, mini, maxi}, bias{nbOutputs, 1, mini, maxi}, activationLayer{f}
{
    
}

NeuroneLayer::NeuroneLayer(const NeuroneLayer &n):
    weights{n.weights}, bias{n.bias}, activationLayer{n.activationLayer}
{

}

NeuroneLayer* NeuroneLayer::clone() const
{
    return new NeuroneLayer(*this);
}

Matrix<double> NeuroneLayer::applyLayer(const Matrix<double> &m) const
{
    Matrix<double> tempAns = weights * m + bias;
    Matrix<double> ans = activationLayer.applyLayer(tempAns);
    return ans;
}

Matrix<double> NeuroneLayer::applyLayer(const Matrix<double> &m)
{
    input = m;
    Matrix<double> tempOutput = weights * input + bias;
    output = activationLayer.applyLayer(tempOutput);
    return output;
}

void NeuroneLayer::calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const
{
    Matrix<double> dW, dB;
    Matrix<double> prod;
    Layer* tempGrad;
    activationLayer.calcBackpropagation(dOutput, prod, tempGrad);
    dInput = transpose(weights) * prod;
    dW = prod * transpose(input);
    dB = prod;
    gradient = new NeuroneLayer{dW, dB, activationLayer.getActivationFunction()};
    delete tempGrad;
}

void NeuroneLayer::applyGradient(const Layer& gradient, double coef)
{
    const NeuroneLayer* gradN = dynamic_cast<const NeuroneLayer*>(&gradient);
    if (!gradN) return;
    *this -= (*gradN) * coef;
}

Matrix<double> NeuroneLayer::getWeights() const
{
    return weights;
}

Matrix<double> NeuroneLayer::getBias() const
{
    return bias;
}

ActivationFunction NeuroneLayer::getActivationFunction() const
{
    return activationLayer.getActivationFunction();
}

int NeuroneLayer::getNbInputs() const
{
    return weights.width();
}

int NeuroneLayer::getNbOutputs() const
{
    return weights.height();
}

char NeuroneLayer::getType() const
{
    return 'N';
}

void NeuroneLayer::setWeights(const Matrix<double> &m)
{
    weights = m;
    output = Matrix<double>{};
}

void NeuroneLayer::setBias(const Matrix<double> &m)
{
    bias = m;
    output = Matrix<double>{};
}

void NeuroneLayer::setActivationFunction(const ActivationFunction &f)
{
    activationLayer.setActivationFunction(f);
    output = Matrix<double>{};
}

NeuroneLayer& NeuroneLayer::operator+=(const Layer &L)
{
    const NeuroneLayer* nP = dynamic_cast<const NeuroneLayer*>(&L);
    if (!nP) return *this;
    *this = *this + *nP;
    return *this;
}

NeuroneLayer& NeuroneLayer::operator-=(const Layer &L)
{
    const NeuroneLayer* nP = dynamic_cast<const NeuroneLayer*>(&L);
    if (!nP) return *this;
    *this = *this - *nP;
    return *this;
}

NeuroneLayer& NeuroneLayer::operator*=(double k)
{
    *this = *this * k;
    return *this;
}

NeuroneLayer& NeuroneLayer::operator/=(double k)
{
    *this = *this / k;
    return *this;
}

void NeuroneLayer::print(ostream& ost) const
{
    ost << "weights :\n" << getWeights();
    ost << "bias :\n" << getBias();
    ost << activationLayer;
    ost << "input :\n" << getInput();
    ost << "output :\n" << getOutput();
}

void NeuroneLayer::read(istream& ist)
{
    Matrix<double> w{getNbOutputs(), getNbInputs()}, b{getNbOutputs(), 1};
    ist >> w >> b;
    setWeights(w);
    setBias(b);
}

NeuroneLayer operator+(const NeuroneLayer &A, const NeuroneLayer &B)
{
    NeuroneLayer c{};
    c.setWeights(A.getWeights() + B.getWeights());
    c.setBias(A.getBias() + B.getBias());
    c.setActivationFunction(A.getActivationFunction());
    return c;
}

NeuroneLayer operator-(const NeuroneLayer &A, const NeuroneLayer &B)
{
    return A + (-B);
}

NeuroneLayer operator-(const NeuroneLayer &n)
{
    return (-1.0) * n;
}

NeuroneLayer operator*(double k, const NeuroneLayer &n)
{
    NeuroneLayer c{};
    c.setWeights(n.getWeights() * k);
    c.setBias(n.getBias() * k);
    c.setActivationFunction(n.getActivationFunction());
    return c;
}

NeuroneLayer operator*(const NeuroneLayer &n, double k)
{
    return k * n;
}

NeuroneLayer operator/(const NeuroneLayer &n, double k)
{
    return (1.0/k) * n;
}

}
