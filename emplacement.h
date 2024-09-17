#ifndef INCLUDED_EMPLACEMENT_H
#define INCLUDED_EMPLACEMENT_H
#include "position.h"

class Emplacement
{
public:
    Emplacement(int p_id, const Position<int> &p_pos);

    int getId() const;
    Position<int> getPos() const;
    int getNoeud() const;

    void setId(int p_id);
    void setPos(const Position<int> &p_pos);
    void setNoeud(int p_noeud);

    bool estDisponible() const;
private:
    int id;
    Position<int> pos;
    int noeud;
};

#endif