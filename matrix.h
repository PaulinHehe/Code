#ifndef MATRICE_H_INCLUDED
#define MATRICE_H_INCLUDED
#include <vector>
#include <iostream>
#include <exception>
using namespace std;

namespace NeuralNetwork
{

class operationMatrixSizeError : public exception {};

template <typename T>
class Matrix
{
public:
    Matrix();
    // h est la hauteur, w est la largeur de la matrice
    // Chaque valeur de la matrice est initialisee aleatoirement dans l'intervalle [mini, maxi]
    Matrix(int h, int w, T mini = T{0}, T maxi = T{1});
    // Initialisation a partir d'un tableau de valeur
    Matrix(const std::vector<std::vector<T>> &m);
    Matrix(const Matrix<T> &m);

    // Renvoie la ligne a l'indice i
    std::vector<T>& operator[](int i);
    const std::vector<T>& operator[](int i) const;
    // Operations classiques sur les matrices
    Matrix<T>& operator+=(const Matrix<T>& m);
    Matrix<T>& operator-=(const Matrix<T>& m);
    Matrix<T>& operator*=(const Matrix<T>& m);
    // Renvoie une matrice contenant le produit des valeurs aux memes indices
    // Formule : Si C = A & B, C[i][j] = A[i][j] * B[i][j]
    Matrix<T>& operator&=(const Matrix<T>& m);
    Matrix<T>& operator*=(T k);
    Matrix<T>& operator/=(T k);

    // Renvoie la hauteur
    int height() const;
    // Renvoie la largeur
    int width() const;

    // Applique la fonction f sur chaque case de la matrice
    void apply(T (*f)(T));
    // Transpose la matrice
    void transpose();
    // Renvoie la somme de toutes les valeurs de la matrice
    T sum() const;
private:
    std::vector<std::vector<T>> mat;
};

// Operations classiques sur les matrices
template <typename T>
Matrix<T> operator+(const Matrix<T> &A, const Matrix<T> &B);
template <typename T>
Matrix<T> operator-(const Matrix<T> &A, const Matrix<T> &B);
template <typename T>
Matrix<T> operator*(const Matrix<T> &A, const Matrix<T> &B);
// Renvoie une matrice contenant le produit des valeurs aux memes indices
// Formule : Si C = A & B, C[i][j] = A[i][j] * B[i][j]
template <typename T>
Matrix<T> operator&(const Matrix<T> &A, const Matrix<T> &B);
// Produit scalaire des matrices A et B
template <typename T>
T operator^(const Matrix<T> &A, const Matrix<T> &B);
template <typename T>
Matrix<T> operator*(T k, const Matrix<T> &A);
template <typename T>
Matrix<T> operator*(const Matrix<T> &A, T k);
template <typename T>
Matrix<T> operator/(const Matrix<T> &A, T k);
template <typename T>
Matrix<T> operator-(const Matrix<T> &A);
template <typename T>
bool operator==(const Matrix<T> &A, const Matrix<T> &B);
template <typename T>
bool operator!=(const Matrix<T> &A, const Matrix<T> &B);
// Applique la fonction f sur chaque case de la matrice
template <typename T>
Matrix<T> apply(T (*f)(T), const Matrix<T> &m);
// Renvoie la transposee de la matrice
template <typename T>
Matrix<T> transpose(const Matrix<T> &m);
// Affichage et lecture ligne par ligne puis colonne par colonne
template <typename T>
ostream& operator<<(ostream& ost, const Matrix<T> &m);
template <typename T>
istream& operator>>(istream& ist, Matrix<T> &m);

}

#include "matrix.cpp"


#endif // MATRICE_H_INCLUDED
