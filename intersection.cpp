#ifndef INCLUDED_INTERSECTION_TEMPLATE_CPP
#define INCLUDED_INTERSECTION_TEMPLATE_CPP
#include "intersection.h"
#include "geometrie.h"
using namespace std;

template <typename T>
T area2(const Position<T> &a, const Position<T> &b, const Position<T> &c) {
	return (b.getX() - a.getX()) * (c.getY() - a.getY()) - (c.getX() - a.getX()) * (b.getY() - a.getY());
}

template <typename T>
bool left(const Position<T> &a, const Position<T> &b, const Position<T> &c) {
	return area2(a, b, c) > 0;
}

template <typename T>
bool collinear(const Position<T> &a, const Position<T> &b, const Position<T> &c) {
	return area2(a, b, c) == 0;
}

template <typename T>
bool intersectProp(const Position<T> &a, const Position<T> &b, const Position<T> &c, const Position<T> &d) {
	if (collinear(a, b, c) || collinear(a, b, d) || collinear(c, d, a) || collinear(c, d, b))
		return false;
	return (left(a, b, c) ^ left(a, b, d)) && (left(c, d, a) ^ left(c, d, b));
}

template <typename T>
bool between(const Position<T> &a, const Position<T> &b, const Position<T> &c) {
	if (!collinear(a, b, c))
		return false;
	if (a.getX() != b.getX())
		return ((a.getX() < c.getX()) && (c.getX() < b.getX())) || ((a.getX() > c.getX()) && (c.getX() > b.getX()));
	else
		return ((a.getY() < c.getY()) && (c.getY() < b.getY())) || ((a.getY() > c.getY()) && (c.getY() > b.getY()));
}

template <typename T>
bool seCroisent(const Position<T> &p, const Position<T> &q, const Position<T> &r, const Position<T> &s, bool& isIllegal) {
	isIllegal = false;
	if (intersectProp(p, q, r, s))
	{
		return true;
	}
	else if (between(p, q, r) || between(p, q, s) || between(r, s, p) || between(r, s, q)) {
		isIllegal = true;
		return true;
	}
	return false;
}

template <typename T>
bool surSegment(const Position<T> &s, const Position<T> &t, const Position<T> &c) {
	return between(s, t, c) || (s.getX() == c.getX() && s.getY() == c.getY()) || (t.getX() == c.getX() && t.getY() == c.getY());
}

template <typename T>
bool seCroisentForce(const Position<T> &a, const Position<T> &b, const Position<T> &c, const Position<T> &d, Position<T> &i)
{
	T denominator = (b.getX() - a.getX()) * (d.getY() - c.getY()) - (b.getY() - a.getY()) * (d.getX() - c.getX());
    if (denominator == 0) {
        return false;
    }
    T t = ((c.getY() - a.getY()) * (d.getX() - c.getX()) - (c.getX() - a.getX()) * (d.getY() - c.getY())) / denominator;
    T u = ((b.getY() - a.getY()) * (d.getX() - c.getX()) - (b.getX() - a.getX()) * (d.getY() - c.getY())) / denominator;
    if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
        i.setX(a.getX() + t*(b.getX() - a.getX()));
        i.setY(a.getX() + t*(b.getY() - a.getY()));
        return true;
    } else {
        return false;
    }
}

#endif