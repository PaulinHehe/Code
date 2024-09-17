#include "arete.h"

Arete::Arete(int p_id, int dep, int arr, const Position<double> &posDep, const Position<double> &posArr):
    id{p_id}, noeudDep{dep}, noeudArr{arr}, posNoeudDep{posDep}, posNoeudArr{posArr}
{

}

int Arete::getId() const
{
    return id;
}

int Arete::getNoeudDep() const
{
    return noeudDep;
}

int Arete::getNoeudArr() const
{
    return noeudArr;
}

Position<double> Arete::getPosNoeudDep() const
{
    return posNoeudDep;
}

Position<double> Arete::getPosNoeudArr() const
{
    return posNoeudArr;
}

void Arete::setId(int p_id)
{
    id = p_id;
}

void Arete::setNoeudDep(int dep)
{
    noeudDep = dep;
}

void Arete::setNoeudArr(int arr)
{
    noeudArr = arr;
}

void Arete::setPosNoeudDep(const Position<double> &posDep)
{
    posNoeudDep = posDep;
}

void Arete::setPosNoeudArr(const Position<double> &posArr)
{
    posNoeudArr = posArr;
}

bool Arete::contient(int idNoeud) const
{
    return noeudDep == idNoeud || noeudArr == idNoeud;
}

bool Arete::contient(const Noeud &n) const
{
    return contient(n.getId());
}

int Arete::getAutreExtremite(int idNoeud) const
{
    if (noeudDep == idNoeud) return noeudArr;
    return noeudDep;
}