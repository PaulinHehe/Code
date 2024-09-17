#include "layer.h"

namespace NeuralNetwork
{

Layer::Layer():
    input{}, output{}
{

}

Layer::Layer(const Layer& l):
    input{l.input}, output{l.output}
{

}

void Layer::applyGradient(const Layer& gradient, double coef)
{

}

Matrix<double> Layer::getInput() const
{
    return input;
}

Matrix<double> Layer::getOutput() const
{
    return output;
}

int Layer::getNbInputs() const
{
    return -1;
}

int Layer::getNbOutputs() const
{
    return -1;
}

char Layer::getType() const
{
    return 'L';
}

void Layer::print(ostream& ost) const
{
    ost << "Unknown layer\n";
}

void Layer::read(istream& ist)
{

}

Layer& Layer::operator+=(const Layer& L)
{
    return *this;
}

Layer& Layer::operator-=(const Layer& L)
{
    return *this;
}

Layer& Layer::operator*=(double k)
{
    return *this;
}

Layer& Layer::operator/=(double k)
{
    return *this;
}

ostream& operator<<(ostream& ost, const Layer &l)
{
    l.print(ost);
    return ost;
}

istream& operator>>(istream& ist, Layer &l)
{
    l.read(ist);
    return ist;
}

}
