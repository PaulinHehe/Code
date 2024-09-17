#ifndef INCLUDED_GEOMETRIE_CPP
#define INCLUDED_GEOMETRIE_CPP
#include "geometrie.h"
#include <algorithm>
using namespace std;

template <typename T>
int quadrant(const Position<T> &s, const Position<T> &t)
{
	if (t.getX() >= s.getX())
	{
		if (t.getY() >= s.getY())
		{
			return 1;
		}
		return 4;
	}
	if (t.getY() >= s.getY())
	{
		return 2;
	}
	return 3;
}

template <typename T>
int aGauche(const Position<T> &s, const Position<T> &t, const Position<T> &c)
{
	T det = (t.getX() - s.getX()) * (c.getY() - s.getY()) - (t.getY() - s.getY()) * (c.getX() - s.getX());
	if (det > 0) return 1;
	if (det < 0) return -1;
	return 0;
}

template <typename T>
bool dansRectangle(const Position<T> &s, const Position<T> &t, const Position<T> &c)
{
	return (c.getX() < max(s.getX(), t.getX()) && c.getX() >= min(s.getX(), t.getX()) && c.getY() < max(s.getY(), t.getY()) && c.getY() >= min(s.getY(), t.getY()));
}

template <typename T>
T distanceCarre(const Position<T> &a, const Position<T> &b)
{
	T diffX = a.getX() - b.getX();
	T diffY = a.getY() - b.getY();
	return diffX * diffX + diffY * diffY;
}

template <typename T>
T distance(const Position<T> &a, const Position<T> &b)
{
    T diffX = a.getX() - b.getX();
	T diffY = a.getY() - b.getY();
	return sqrt(diffX * diffX + diffY * diffY);
}

template <typename T>
T distanceManhattan(const Position<T> &a, const Position<T> &b)
{
	T diffX = abs(a.getX() - b.getX());
	T diffY = abs(a.getY() - b.getY());
	return diffX + diffY;
}

template <typename T> 
int getNumPositionFromRayon(const Position<T> &a, const std::vector<Position<T>> &tabPos, double rayon){
    int index = -1;
    for(int i = 0; i<tabPos.size(); i++){
        if(distanceCarre(a, tabPos[i]) <= rayon*rayon){
            if(index != -1){
                if(getNumPositionFromClosest(a, {tabPos[index], tabPos[i]}) == 1){ index = i; }        
            }else{
                index = i;
            }
        }
    }
    return index;
}

template <typename T> 
int getNumPositionFromClosest(const Position<T> &a, const std::vector<Position<T>> &tabPos){
    int index = -1;
    for(int i = 0; i<tabPos.size(); i++){
        if(distanceCarre(a, tabPos[i]) < distanceCarre(a, tabPos[index])){            
            index = i;
        }
    }
    return index;
}

template <typename T>
void calcBoiteEnglobante(const std::vector<Position<T>> &pos, Position<T> &BG, Position<T> &HD)
{
	BG = pos[0];
	HD = pos[0];
	for (int i = 1; i < pos.size(); i++)
	{
		if (pos[i].getX() < BG.getX()) BG.setX(pos[i].getX());
		if (pos[i].getX() > HD.getX()) HD.setX(pos[i].getX());
		if (pos[i].getY() < BG.getY()) BG.setY(pos[i].getY());
		if (pos[i].getY() > HD.getY()) HD.setY(pos[i].getY());
	}
}

template <typename T>
void fusionneBoitesEnglobantes(const std::vector<Position<T>> &posA, std::vector<Position<T>> &posB)
{
	Position<double> BGA, HDA, BGB, HDB;
	calcBoiteEnglobante(posA, BGA, HDA);
	calcBoiteEnglobante(posB, BGB, HDB);
	for (int i = 0; i < posB.size(); i++)
	{
		double xRatio, yRatio;
		if (HDB.getX() != BGB.getX()) xRatio = (posB[i].getX() - BGB.getX()) / (HDB.getX() - BGB.getX());
		else xRatio = 0.0;
		if (HDB.getY() != BGB.getY()) yRatio = (posB[i].getY() - BGB.getY()) / (HDB.getY() - BGB.getY());
		else yRatio = 0.0;
		double Nx = xRatio * (HDA.getX() - BGA.getX()) + BGA.getX();
		double Ny = yRatio * (HDA.getY() - BGA.getY()) + BGA.getY();
		posB[i] = Position<double>{Nx, Ny};
	}
}

template <typename T>
std::vector<Position<T>> rotation(const std::vector<Position<T>> &pos, const Position<T> &centre, double angle)
{
	std::vector<Position<T>> ans;
	for (Position<T> p : pos)
	{
		Position<T> pC = p - centre;
		Position<T> im = Position<T>{cos(angle) * pC.getX() - sin(angle) * pC.getY(), sin(angle) * pC.getX() + cos(angle) * pC.getY()};
		ans.push_back(im + centre);
	}
	return ans;
}

template <typename T>
std::vector<Position<T>> translation(const std::vector<Position<T>> &pos, const Position<T> &vect)
{
	std::vector<Position<T>> ans;
	for (Position<T> p : pos)
	{
		ans.push_back(p + vect);
	}
	return ans;
}

template <typename T>
std::vector<Position<T>> zoom(const std::vector<Position<T>> &pos, const Position<T> &centre, double qte)
{
	std::vector<Position<T>> ans;
	for (Position<T> p : pos)
	{
		ans.push_back((p - centre) * qte + centre);
	}
	return ans;
}

template <typename T>
std::vector<Position<T>> rotationRelative(const std::vector<Position<T>> &pos, double angle)
{
	Position<T> BG, HD;
	calcBoiteEnglobante(pos, BG, HD);
	return rotation<T>(pos, (BG + HD)/T{2}, angle);
}

template <typename T>
std::vector<Position<T>> translationRelative(const std::vector<Position<T>> &pos, const Position<double> &vect)
{
	Position<T> BG, HD;
	calcBoiteEnglobante(pos, BG, HD);
	Position<T> diff = HD - BG;
	return translation<T>(pos, Position<T>{vect.getX() * diff.getX(), vect.getY() * diff.getY()});
}

template <typename T>
std::vector<Position<T>> zoomRelatif(const std::vector<Position<T>> &pos, double qte)
{
	Position<T> BG, HD;
	calcBoiteEnglobante(pos, BG, HD);
	return zoom<T>(pos, (BG + HD)/T{2}, qte);
}

template <typename T>
Position<T> barycentre(const std::vector<Position<T>> &pos)
{
	Position<T> S;
	for (Position<T> p : pos)
	{
		S += p;
	}
	S /= (T)pos.size();
	return S;
}


#endif