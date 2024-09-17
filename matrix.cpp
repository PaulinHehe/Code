#ifndef MATRICE_CPP_INCLUDED
#define MATRICE_CPP_INCLUDED
#include "matrix.h"
#include "utilitaire.h"

namespace NeuralNetwork
{

template <typename T>
Matrix<T>::Matrix():
    mat{}
{
    
}

template <typename T>
Matrix<T>::Matrix(int h, int w, T mini, T maxi):
    mat{}
{
    mat.resize(h, std::vector<T>(w));
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (mini != maxi)
            {
                double p = generateRandDouble(mini, maxi);
                mat[i][j] = T{p};
            }
            else
            {
                mat[i][j] = mini;
            }
        }
    }
}

template <typename T>
Matrix<T>::Matrix(const std::vector<std::vector<T>> &m):
    mat{m}
{
    
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &m):
    mat{m.mat}
{
    
}

template <typename T>
std::vector<T>& Matrix<T>::operator[](int i)
{
    return mat[i];
}

template <typename T>
const std::vector<T>& Matrix<T>::operator[](int i) const
{
    return mat[i];
}

template <typename T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& m)
{
    *this = *this + m;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator-=(const Matrix<T>& m)
{
    *this = *this - m;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(const Matrix<T>& m)
{
    *this = *this * m;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator&=(const Matrix<T>& m)
{
    *this = (*this) & m;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator*=(T k)
{
    *this = *this * k;
    return *this;
}

template <typename T>
Matrix<T>& Matrix<T>::operator/=(T k)
{
    *this = *this / k;
    return *this;
}

template <typename T>
int Matrix<T>::height() const
{
    return (int)mat.size();
}

template <typename T>
int Matrix<T>::width() const
{
    if (height() == 0) return 0;
    return (int)mat[0].size();
}

template <typename T>
void Matrix<T>::apply(T (*f)(T))
{
    for (int i = 0; i < height(); i++)
    {
        for (int j = 0; j < width(); j++)
        {
            mat[i][j] = f(mat[i][j]);
        }
    }
}

template <typename T>
void Matrix<T>::transpose()
{
    Matrix<T> c = Matrix<T>{width(), height()};
    for (int i = 0; i < height(); i++)
    {
        for (int j = 0; j < width(); j++)
        {
            c[j][i] = mat[i][j];
        }
    }
    *this = c;
}

template <typename T>
T Matrix<T>::sum() const
{
    T S = T{0};
    for (int i = 0; i < height(); i++)
    {
        for (int j = 0; j < width(); j++)
        {
            S += mat[i][j];
        }
    }
    return S;
}

template <typename T>
Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B)
{
    if (A.height() != B.height() || A.width() != B.width())
    {
        throw operationMatrixSizeError();
    }
    Matrix<T> C{A};
    for (int i = 0; i < B.height(); i++)
    {
        for (int j = 0; j < B.width(); j++)
        {
            C[i][j] += B[i][j];
        }
    }
    return C;
}

template <typename T>
Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B)
{
    return A + (-B);
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B)
{
    if (A.width() != B.height())
    {
        throw operationMatrixSizeError();
    }
    Matrix<T> C{A.height(), B.width(), {0}, {0}};
    for (int i = 0; i < C.height(); i++)
    {
        for (int j = 0; j < C.width(); j++)
        {
            for (int d = 0; d < A.width(); d++)
            {
                C[i][j] += A[i][d] * B[d][j];
            }
        }
    }
    return C;
}

template <typename T>
Matrix<T> operator&(const Matrix<T> &A, const Matrix<T> &B)
{
    if (A.height() != B.height() || A.width() != B.width())
    {
        throw operationMatrixSizeError();
    }
    Matrix<T> C{A};
    for (int i = 0; i < B.height(); i++)
    {
        for (int j = 0; j < B.width(); j++)
        {
            C[i][j] *= B[i][j];
        }
    }
    return C;
}

template <typename T>
T operator^(const Matrix<T> &A, const Matrix<T> &B)
{
    Matrix<T> andProduct = A & B;
    return andProduct.sum();
}

template <typename T>
Matrix<T> operator*(T k, const Matrix<T> &A)
{
    Matrix<T> B{A};
    for (int i = 0; i < A.height(); i++)
    {
        for (int j = 0; j < A.width(); j++)
        {
            B[i][j] *= k;
        }
    }
    return B;
}

template <typename T>
Matrix<T> operator*(const Matrix<T> &A, T k)
{
    return k * A;
}

template <typename T>
Matrix<T> operator/(const Matrix<T> &A, T k)
{
    return (T{1} / k) * A;
}

template <typename T>
Matrix<T> operator-(const Matrix<T> &A)
{
    return T{-1} * A;
}

template <typename T>
bool operator==(const Matrix<T> &A, const Matrix<T> &B)
{
    if (A.height() != B.height() || A.width() != B.width()) return false;
    for (int i = 0; i < A.height(); i++)
    {
        for (int j = 0; j < A.width(); j++)
        {
            if (A[i][j] != B[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool operator!=(const Matrix<T> &A, const Matrix<T> &B)
{
    return !(A == B);
}

template <typename T>
Matrix<T> apply(T (*f)(T), const Matrix<T> &m)
{
    Matrix<T> c = m;
    c.apply(f);
    return c;
}

template <typename T>
Matrix<T> transpose(const Matrix<T> &m)
{
    Matrix<T> c = m;
    c.transpose();
    return c;
}

template <typename T>
ostream& operator<<(ostream& ost, const Matrix<T> &m)
{
    for (int i = 0; i < m.height(); i++)
    {
        for (int j = 0; j < m.width(); j++)
        {
            ost << m[i][j] << " ";
        }
        ost << "\n";
    }
    return ost;
}

template <typename T>
istream& operator>>(istream& ist, Matrix<T> &m)
{
    for (int i = 0; i < m.height(); i++)
    {
        for (int j = 0; j < m.width(); j++)
        {
            ist >> m[i][j];
        }
    }
    return ist;
}

template class Matrix<double>;

}

#endif // MATRICE_CPP_INCLUDED
