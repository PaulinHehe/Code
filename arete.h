#ifndef INCLUDED_ARETE_H
#define INCLUDED_ARETE_H
#include "noeud.h"
#include "position.h"

class Arete
{
public:
    Arete(int p_id, int dep, int arr, const Position<double> &posDep, const Position<double> &posArr);

    int getId() const;
    int getNoeudDep() const;
    int getNoeudArr() const;
    Position<double> getPosNoeudDep() const;
    Position<double> getPosNoeudArr() const;

    void setId(int p_id);
    void setNoeudDep(int dep);
    void setNoeudArr(int arr);
    void setPosNoeudDep(const Position<double> &posDep);
    void setPosNoeudArr(const Position<double> &posArr);

    bool contient(int idNoeud) const;
    bool contient(const Noeud &n) const;
    //Renvoie l'id du noeud a l'autre extremite de l'arete
    //idNoeud est le noeud dont on cherche l'autre extremite
    int getAutreExtremite(int idNoeud) const;
private:
    int id;
    int noeudDep;
    int noeudArr;
    Position<double> posNoeudDep;
    Position<double> posNoeudArr;
};

#endif