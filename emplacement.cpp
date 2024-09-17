#include "emplacement.h"

Emplacement::Emplacement(int p_id, const Position<int> &p_pos):
    id{p_id}, pos{p_pos}, noeud{-1}
{

}

int Emplacement::getId() const
{
    return id;
}

Position<int> Emplacement::getPos() const
{
    return pos;
}

int Emplacement::getNoeud() const
{
    return noeud;
}

void Emplacement::setId(int p_id)
{
    id = p_id;
}

void Emplacement::setPos(const Position<int> &p_pos)
{
    pos = p_pos;
}

void Emplacement::setNoeud(int p_noeud)
{
    noeud = p_noeud;
}

bool Emplacement::estDisponible() const
{
    return noeud == -1;
}