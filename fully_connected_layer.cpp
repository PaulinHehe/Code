#include "fully_connected_layer.h"
#include <cmath>

namespace NeuralNetwork
{

double MSE(const Matrix<double> &An, const Matrix<double> &Y)
{
    Matrix<double> diff = An - Y;
    double C = diff ^ diff;
    return C;
}

Matrix<double> MSED(const Matrix<double> &An, const Matrix<double> &Y)
{
    Matrix<double> dAn = 2.0 * (An - Y);
    return dAn;
}

double inverseCrossEntropy(double x)
{
    if (x == 0) return COEF_INFINI;
    return 1.0 / x;
}

double logCrossEntropy(double x)
{
    if (x == 0) return -COEF_INFINI;
    return log(x);
}

double CrossEntropy(const Matrix<double> &An, const Matrix<double> &Y)
{
    Matrix<double> logAn = apply(logCrossEntropy, An);
    double C = - (Y ^ logAn);
    return C;
}

Matrix<double> CrossEntropyD(const Matrix<double> &An, const Matrix<double> &Y)
{
    Matrix<double> invAn = apply(inverseCrossEntropy, An);
    Matrix<double> dAn = - (Y & invAn);
    return dAn;
}

FullyConnectedLayer::FullyConnectedLayer():
    layers{}, costFunction{MSE, MSED}, input{}, output{}
{

}

FullyConnectedLayer::FullyConnectedLayer(const std::vector<Layer*> &l, const CostFunction &c):
    layers{}, costFunction{c}, input{}, output{}
{
    for (Layer* v : l)
    {
        layers.push_back(v->clone());
    }
}

FullyConnectedLayer::FullyConnectedLayer(const std::vector<int> &nbNeurones, const ActivationFunction &f, const CostFunction &c, const std::vector<Layer*>& endLayers, const std::vector<Layer*>& startLayers):
    layers{}, costFunction{c}, input{}, output{}
{
    for (Layer* v : startLayers)
    {
        layers.push_back(v->clone());
    }
    for (int i = 0; i < (int)nbNeurones.size()-1; i++)
    {
        layers.push_back(new NeuroneLayer{nbNeurones[i], nbNeurones[i+1], f});
    }
    for (Layer* v : endLayers)
    {
        layers.push_back(v->clone());
    }
}

FullyConnectedLayer::FullyConnectedLayer(const std::vector<int> &nbNeurones, double mini, double maxi, const ActivationFunction &f, const CostFunction &c, const std::vector<Layer*>& endLayers, const std::vector<Layer*>& startLayers):
    layers{}, costFunction{c}, input{}, output{}
{
    for (Layer* v : startLayers)
    {
        layers.push_back(v->clone());
    }
    for (int i = 0; i < (int)nbNeurones.size()-1; i++)
    {
        layers.push_back(new NeuroneLayer{nbNeurones[i], nbNeurones[i+1], mini, maxi, f});
    }
    for (Layer* v : endLayers)
    {
        layers.push_back(v->clone());
    }
}

FullyConnectedLayer::FullyConnectedLayer(const FullyConnectedLayer &f):
    layers{}, costFunction{f.costFunction}, input{f.input}, output{f.output}
{
    for (Layer* v : f.layers)
    {
        layers.push_back(v->clone());
    }
}

FullyConnectedLayer::~FullyConnectedLayer()
{
    for (int i = 0; i < (int)layers.size(); i++) delete layers[i];
}

FullyConnectedLayer& FullyConnectedLayer::operator=(const FullyConnectedLayer &f)
{
    if (this == &f) return *this;
    for (int i = 0; i < (int)layers.size(); i++) delete layers[i];
    layers.clear();
    for (Layer* v : f.layers)
    {
        layers.push_back(v->clone());
    }
    costFunction = f.costFunction;
    input = f.input;
    output = f.output;
    return *this;
}

FullyConnectedLayer* FullyConnectedLayer::clone() const
{
    return new FullyConnectedLayer(*this);
}

Matrix<double> FullyConnectedLayer::applyLayers(const Matrix<double> &m) const
{
    Matrix<double> ans = m;
    for (int i = 0; i < (int)layers.size(); i++)
    {
        ans = layers[i]->applyLayer(ans);
    }
    return ans;
}

Matrix<double> FullyConnectedLayer::applyLayers(const Matrix<double> &m)
{
    input = m;
    output = m;
    for (int i = 0; i < (int)layers.size(); i++)
    {
        output = layers[i]->applyLayer(output);
    }
    return output;
}

void FullyConnectedLayer::calcBackpropagation(const Matrix<double> &Y, Matrix<double> &dInput, FullyConnectedLayer &gradient) const
{
    if (output.height() != Y.height() || output.width() != Y.width())
    {
        throw outputEmptyOrIncorrectError();
    }
    Matrix<double> dOutput = costFunction.outputDerivative(output, Y);
    calcBackpropagationFromOutputGradient(dOutput, dInput, gradient);
}

void FullyConnectedLayer::calcBackpropagationFromOutputGradient(const Matrix<double> &dOutput, Matrix<double> &dInput, FullyConnectedLayer &gradient) const
{
    Matrix<double> dAk = dOutput;
    std::vector<Layer*> ans;
    for (int layer = getNbLayers()-1; layer >= 0; layer--)
    {
        Matrix<double> dA;
        Layer* grad;
        layers[layer]->calcBackpropagation(dAk, dA, grad);
        ans.push_back(grad);
        ans.back()->applyLayer(dA);
        dAk = dA;
    }
    dInput = dAk;
    gradient = {};
    for (int layer = getNbLayers()-1; layer >= 0; layer--)
    {
        gradient.addLayer(*(ans[layer]));
    }

    for (int i = 0; i < (int)ans.size(); i++) delete ans[i];
}

void FullyConnectedLayer::applyGradient(const FullyConnectedLayer &gradient, double coef)
{
    *this = *this - coef * gradient;
}

double FullyConnectedLayer::calcCost(const Matrix<double> &Y) const
{
    if (output.height() != Y.height() || output.width() != Y.width())
    {
        throw outputEmptyOrIncorrectError();
    }
    Matrix<double> An = output;
    double C = costFunction.cost(An, Y);
    return C;
}

int FullyConnectedLayer::getNbLayers() const
{
    return (int)layers.size();
}

std::vector<Layer*> FullyConnectedLayer::getLayers() const
{
    return layers;
}

Layer& FullyConnectedLayer::operator[](int n)
{
    return *layers[n];
}

const Layer& FullyConnectedLayer::operator[](int n) const
{
    return *layers[n];
}

CostFunction FullyConnectedLayer::getCostFunction() const
{
    return costFunction;
}

Matrix<double> FullyConnectedLayer::getInput() const
{
    return input;
}

Matrix<double> FullyConnectedLayer::getOutput() const
{
    return output;
}

int FullyConnectedLayer::getNbInputs() const
{
    if (layers.empty()) return 0;
    int iLayer = 0;
    while (iLayer < (int)layers.size() && layers[iLayer]->getNbInputs() == -1)
    {
        iLayer++;
    }
    if (iLayer == (int)layers.size()) return -1;
    return layers[iLayer]->getNbInputs();
}

int FullyConnectedLayer::getNbOutputs() const
{
    if (layers.empty()) return 0;
    int iLayer = (int)layers.size()-1;
    while (iLayer >= 0 && layers[iLayer]->getNbOutputs() == -1)
    {
        iLayer--;
    }
    if (iLayer == -1) return -1;
    return layers[iLayer]->getNbOutputs();
}

char FullyConnectedLayer::getType() const
{
    return 'F';
}

void FullyConnectedLayer::setCostFunction(const CostFunction &c)
{
    costFunction = c;
}

void FullyConnectedLayer::addLayer(const Layer &n)
{
    layers.push_back(n.clone());
}

void FullyConnectedLayer::deleteLastLayer()
{
    layers.pop_back();
}

FullyConnectedLayer& FullyConnectedLayer::operator+=(const FullyConnectedLayer &f)
{
    *this = *this + f;
    return *this;
}

FullyConnectedLayer& FullyConnectedLayer::operator-=(const FullyConnectedLayer &f)
{
    *this = *this - f;
    return *this;
}

FullyConnectedLayer& FullyConnectedLayer::operator*=(double k)
{
    *this = *this * k;
    return *this;
}

FullyConnectedLayer& FullyConnectedLayer::operator/=(double k)
{
    *this = *this / k;
    return *this;
}

void FullyConnectedLayer::print(ostream& ost) const
{
    for (int i = 0; i < getNbLayers(); i++)
    {
        ost << "LAYER " << (i+1) << " :\n" << *(layers[i]);
    }
}

void FullyConnectedLayer::read(istream& ist)
{
    for (int i = 0; i < getNbLayers(); i++)
    {
        ist >> *(layers[i]);
    }
}

FullyConnectedLayer operator+(const FullyConnectedLayer &A, const FullyConnectedLayer &B)
{
    if (A.getNbLayers() != B.getNbLayers())
    {
        throw operationNbLayerError();
    }
    FullyConnectedLayer Acopy{A}, Bcopy{B}, f{};
    for (int i = 0; i < A.getNbLayers(); i++)
    {
        Layer* L = A[i].clone();
        *L += Bcopy[i];
        f.addLayer(*L);
    }
    f.setCostFunction(A.getCostFunction());
    return f;
}

FullyConnectedLayer operator-(const FullyConnectedLayer &A, const FullyConnectedLayer &B)
{
    return A + (-B);
}

FullyConnectedLayer operator-(const FullyConnectedLayer &f)
{
    return (-1.0) * f;
}

FullyConnectedLayer operator*(double k, const FullyConnectedLayer &f)
{
    FullyConnectedLayer c{f};
    for (int i = 0; i < c.getNbLayers(); i++)
    {
        c[i] *= k;
    }
    return c;
}

FullyConnectedLayer operator*(const FullyConnectedLayer &f, double k)
{
    return k * f;
}

FullyConnectedLayer operator/(const FullyConnectedLayer &f, double k)
{
    return (1.0 / k) * f;
}

ostream& operator<<(ostream& ost, const FullyConnectedLayer &f)
{
    f.print(ost);
    return ost;
}

istream& operator>>(istream& ist, FullyConnectedLayer &f)
{
    f.read(ist);
    return ist;
}

}
