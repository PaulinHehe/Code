#include "softmax_layer.h"
#include <cmath>
#include <vector>

namespace NeuralNetwork
{

SoftmaxLayer* SoftmaxLayer::clone() const
{
    return new SoftmaxLayer(*this);
}

Matrix<double> SoftmaxLayer::applyLayer(const Matrix<double> &m) const
{
    Matrix<double> output = applySoftmax(m);
    return output;
}

Matrix<double> SoftmaxLayer::applyLayer(const Matrix<double> &m)
{
    input = m;
    output = applySoftmax(m);
    return output;
}

void SoftmaxLayer::calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const
{
    gradient = new SoftmaxLayer();
    dInput = dOutput;
    calcBackpropagationInterval(dOutput, dInput, 0, dOutput.height());
}

void SoftmaxLayer::calcBackpropagationInterval(const Matrix<double> &dOutput, Matrix<double> &dInput, int ligDep, int ligFin) const
{
    std::vector<int> notNullIndex;
    for (int j = ligDep; j < ligFin; j++)
    {
        if (dOutput[j][0] != 0.0)
        {
            notNullIndex.push_back(j);
        }
    }

    for (int i = ligDep; i < ligFin; i++)
    {
        dInput[i][0] = 0.0;
        for (int j : notNullIndex)
        {
            if (i == j)
            {
                dInput[i][0] += output[j][0] * (1 - output[j][0]) * dOutput[j][0];
            }
            else
            {
                dInput[i][0] += - output[j][0] * output[i][0] * dOutput[j][0];
            }
        }
    }
}

void SoftmaxLayer::applyGradient(const Layer& gradient, double coef)
{

}

string SoftmaxLayer::getTypeSoftmax() const
{
    return "SOFTMAX";
}

void SoftmaxLayer::print(ostream& ost) const
{
    ost << getTypeSoftmax() << "\n";
    ost << "input :\n" << input;
    ost << "output :\n" << output;
}

void SoftmaxLayer::read(istream& ist)
{

}

Matrix<double> SoftmaxLayer::applySoftmax(const Matrix<double> &m) const
{
    Matrix<double> ans = m;
    applySoftmaxInterval(ans, 0, ans.height());
    return ans;
}

void SoftmaxLayer::applySoftmaxInterval(Matrix<double> &m, int ligDep, int ligFin) const
{
    double S = 0.0;
    for (int lig = ligDep; lig < ligFin; lig++)
    {
        S += exp(m[lig][0]);
    }
    for (int lig = ligDep; lig < ligFin; lig++)
    {
        m[lig][0] = exp(m[lig][0]) / S;
    }
}

}
