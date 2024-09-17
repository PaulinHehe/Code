#ifndef INCLUDED_POSITION_H
#define INCLUDED_POSITION_H
#include <iostream>
using namespace std;

template <typename T>
class Position
{
public:
    Position();
    Position(T p_x, T p_y);
    template <typename S>
    Position(const Position<S> &pos);

    inline T getX() const;
    inline T getY() const;

    void setX(T p_x);
    void setY(T p_y);

    Position<T>& operator+=(const Position<T> &pos);
    Position<T>& operator-=(const Position<T> &pos);
    Position<T>& operator*=(T k);
    Position<T>& operator/=(T k);
private:
    T x;
    T y;
};

template <typename T>
Position<T> operator+(const Position<T> &A, const Position<T> &B);
template <typename T>
Position<T> operator-(const Position<T> &A, const Position<T> &B);
template <typename T>
Position<T> operator*(T k, const Position<T> &pos);
template <typename T>
Position<T> operator*(const Position<T> &pos, T k);
template <typename T>
Position<T> operator/(const Position<T> &pos, T k);
template <typename T>
Position<T> operator-(const Position<T> &pos);
template <typename T>
ostream& operator<<(ostream& ost, const Position<T> &pos);
template <typename T>
istream& operator>>(istream& ist, Position<T> &pos);

#include "position.cpp"

#endif