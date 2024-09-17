#include "noeud.h"
#include <cmath>
using namespace std;

Noeud::Noeud(int p_id):
    id{p_id}, emplacement{-1}, pos{0, 0}, coordReel{false}
{
    
}

Noeud::Noeud(int p_id, const Position<double> &p_pos):
    id{p_id}, emplacement{-1}, pos{p_pos}, coordReel{false}
{
    
}

int Noeud::getId() const
{
    return id;
}

int Noeud::getEmplacement() const
{
    return emplacement;
}

Position<double> Noeud::getPos() const
{
    return pos;
}

void Noeud::setId(int p_id)
{
    id = p_id;
}

void Noeud::setEmplacement(int emp)
{
    emplacement = emp;
}

void Noeud::setPos(const Position<double> &p_pos)
{
    pos = p_pos;
}

bool Noeud::isCoordReel() const
{
    return coordReel;
}

bool Noeud::estPlace() const
{
    return emplacement != -1;
}

void Noeud::changeTypeCoord()
{
    coordReel = !coordReel;
    if (!coordReel)
    {
        pos.setX(round(pos.getX()));
        pos.setY(round(pos.getY()));
    }
}