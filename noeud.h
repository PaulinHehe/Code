#ifndef INCLUDED_NOEUD_H
#define INCLUDED_NOEUD_H
#include "position.h"

class Noeud
{
public:
    //Initialisation d'un noeud par son id
    //Par defaut les coordonnees sont entieres et a l'origine, l'emplacement est vide
    Noeud(int p_id);
    Noeud(int p_id, const Position<double> &p_pos);

    int getId() const;
    int getEmplacement() const;
    //Renvoie un arrondi a l'entier si les coordonnees sont entieres
    Position<double> getPos() const;

    void setId(int p_id);
    void setEmplacement(int emp);
    //Un setter pour chaque type de coordonnees
    //Ne change pas le type de coordonnees
    void setPos(const Position<double> &p_pos);

    bool isCoordReel() const;
    //Renvoie vrai s'il est sur un emplacement
    bool estPlace() const;

    //Passe de coordonnees entieres a reelles et vice-versa
    void changeTypeCoord();
private:
    int id;
    int emplacement; //id de l'emplacement si place, -1 sinon
    Position<double> pos; //arrondi a l'entier le plus proche si coordonnees entieres
    bool coordReel; //vrai si les coordonnees sont reelles, faux si ce sont des entiers
};

#endif