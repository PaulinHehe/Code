#ifndef INCLUDED_INTERSECTION_H
#define INCLUDED_INTERSECTION_H
#include "position.h"
#include "intersection.h"

template <typename T>
T area2(const Position<T> &a, const Position<T> &b, const Position<T> &c);

template <typename T>
bool left(const Position<T> &a, const Position<T> &b, const Position<T> &c);

template <typename T>
bool collinear(const Position<T> &a, const Position<T> &b, const Position<T> &c);

template <typename T>
bool intersectProp(const Position<T> &a, const Position<T> &b, const Position<T> &c, const Position<T> &d);

template <typename T>
bool between(const Position<T> &a, const Position<T> &b, const Position<T> &c);

//renvoie vrai si les segments [p,q] et [r,s] se croisent
template <typename T>
bool seCroisent(const Position<T> &p, const Position<T> &q, const Position<T> &r, const Position<T> &s, bool& isIllegal);

//Pas sur du nom
template <typename T>
bool surSegment(const Position<T> &s, const Position<T> &t, const Position<T> &c);

// compute the intersection point of two edges
template <typename T>
bool seCroisentForce(const Position<T> &a, const Position<T> &b, const Position<T> &c, const Position<T> &d, Position<T> &i);

#include "intersection.cpp"

#endif