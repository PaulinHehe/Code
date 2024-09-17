#include "softmax_layer_alpha_go_zero.h"

SoftmaxLayerAlphaGoZero* SoftmaxLayerAlphaGoZero::clone() const
{
    return new SoftmaxLayerAlphaGoZero(*this);
}

void SoftmaxLayerAlphaGoZero::calcBackpropagation(const Matrix<double> &dOutput, Matrix<double> &dInput, Layer*& gradient) const
{
    gradient = new SoftmaxLayerAlphaGoZero();
    dInput = dOutput;
    calcBackpropagationInterval(dOutput, dInput, 0, dOutput.height()-1);
    //dInput[dInput.height()-1][0] = dOutput[dOutput.height()-1][0] * SigmoidD(input[input.height()-1][0]);
}

string SoftmaxLayerAlphaGoZero::getTypeSoftmax() const
{
    return "SOFTMAX ALPHAGO ZERO";
}

Matrix<double> SoftmaxLayerAlphaGoZero::applySoftmax(const Matrix<double> &m) const
{
    Matrix<double> ans = m;
    applySoftmaxInterval(ans, 0, m.height()-1);
    //ans[ans.height()-1][0] = Sigmoid(ans[ans.height()-1][0]);
    return ans;
}
