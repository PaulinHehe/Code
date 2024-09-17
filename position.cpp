#ifndef INCLUDED_POSITION_CPP
#define INCLUDED_POSITION_CPP
#include "position.h"

template <typename T>
Position<T>::Position():
    x{T(0)}, y{T(0)}
{

}

template <typename T>
Position<T>::Position(T p_x, T p_y):
    x{p_x}, y{p_y}
{

}

template <typename T>
template <typename S>
Position<T>::Position(const Position<S> &pos):
    x{T(pos.getX())}, y{T(pos.getY())}
{
    
}

template <typename T>
T Position<T>::getX() const
{
    return x;
}

template <typename T>
T Position<T>::getY() const
{
    return y;
}

template <typename T>
void Position<T>::setX(T p_x)
{
    x = p_x;
}

template <typename T>
void Position<T>::setY(T p_y)
{
    y = p_y;
}

template <typename T>
Position<T>& Position<T>::operator+=(const Position<T> &pos)
{
    *this = *this + pos;
    return *this;
}

template <typename T>
Position<T>& Position<T>::operator-=(const Position<T> &pos)
{
    *this = *this - pos;
    return *this;
}

template <typename T>
Position<T>& Position<T>::operator*=(T k)
{
    *this = *this * k;
    return *this;
}

template <typename T>
Position<T>& Position<T>::operator/=(T k)
{
    *this = *this / k;
    return *this;
}

template <typename T>
Position<T> operator+(const Position<T> &A, const Position<T> &B)
{
    return Position<T>(A.getX() + B.getX(), A.getY() + B.getY());
}

template <typename T>
Position<T> operator-(const Position<T> &A, const Position<T> &B)
{
    return A + (-B);
}

template <typename T>
Position<T> operator*(T k, const Position<T> &pos)
{
    return Position<T>(k * pos.getX(), k * pos.getY());
}

template <typename T>
Position<T> operator*(const Position<T> &pos, T k)
{
    return k * pos;
}

template <typename T>
Position<T> operator/(const Position<T> &pos, T k)
{
    return (T{1}/k) * pos;
}

template <typename T>
Position<T> operator-(const Position<T> &pos)
{
    return T{-1} * pos;
}

template <typename T>
ostream& operator<<(ostream &ost, const Position<T> &pos)
{
    ost << pos.getX() << "," << pos.getY();
    return ost;
}

template <typename T>
istream& operator>>(istream &ist, Position<T> &pos)
{
    T x, y;
    ist >> x >> y;
    pos.setX(x);
    pos.setY(y);
    return ist;
}

#endif