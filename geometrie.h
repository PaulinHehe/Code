#ifndef INCLUDED_GEOMETRIE_H
#define INCLUDED_GEOMETRIE_H
#include "position.h"
#include <math.h>
#include <vector>

//renvoie 1, 2, 3 ou 4 si le point t est 1: en haut a droite, 2: en haut a gauche, 3: en bas a gauche, 4: en bas a droite du point s
//en cas d'egalite c'est haut et droite qui l'emportent
template <typename T>
int quadrant(const Position<T> &s, const Position<T> &t);

//si c est a gauche de (s;t) renvoie 1, si c est a droite de (s;t) renvoie -1, si c,s et t sont alignes renvoie 0
template <typename T>
int aGauche(const Position<T> &s, const Position<T> &t, const Position<T> &c);

//Renvoie vrai si c est dans le rectangle form� par st
template <typename T>
bool dansRectangle(const Position<T> &s, const Position<T> &t, const Position<T> &c);

//Renvoie la distance au carre entre deux points
template <typename T>
T distanceCarre(const Position<T> &a, const Position<T> &b);
//Renvoie la distance entre deux points (ATTENTION AUX ENTIERS !)
template <typename T>
T distance(const Position<T> &a, const Position<T> &b);
//Renvoie la distance de manhattan
template <typename T>
T distanceManhattan(const Position<T> &a, const Position<T> &b);

template <typename T> 
int getNumPositionFromClosest(const Position<T> &a, const std::vector<Position<T>> &tabPos);

//Renvoie l'indice d
template <typename T> 
int getNumPositionFromRayon(const Position<T> &a, const std::vector<Position<T>> &tabPos, double rayon);

//Calcule les coins de la boite englobante d'un ensemble de points
//BG est le coin Bas-Gauche de la boite
//HD est le coin Haut-Droit de la boite
template <typename T>
void calcBoiteEnglobante(const std::vector<Position<T>> &pos, Position<T> &BG, Position<T> &HD);

//Bouge les noeuds de l'ensemble posB pour que la boite englobante de posB devienne egale a celle de posA
//La transformation est effectuee par 'applatissements' ou 'aggrandissements' en x et en y
template <typename T>
void fusionneBoitesEnglobantes(const std::vector<Position<T>> &posA, std::vector<Position<T>> &posB);

//Renvoie l'ensemble de points apres la rotation de centre et d'angle donne
//angle : angle en radian
template <typename T>
std::vector<Position<T>> rotation(const std::vector<Position<T>> &pos, const Position<T> &centre, double angle);

//Renvoie l'ensemble de points apres la translation de vecteur vect
template <typename T>
std::vector<Position<T>> translation(const std::vector<Position<T>> &pos, const Position<T> &vect);

//Renvoie l'ensemble de points apres le zoom de centre donne et de puissance qte
template <typename T>
std::vector<Position<T>> zoom(const std::vector<Position<T>> &pos, const Position<T> &centre, double qte);

//Renvoie l'ensemble de points apres la rotation d'angle donne autour du centre de la boite englobante
//angle : angle en radian
template <typename T>
std::vector<Position<T>> rotationRelative(const std::vector<Position<T>> &pos, double angle);

//Renvoie l'ensemble de points apres la translation de vecteur donne
//vect : vecteur representant la translation a effectuer en nombre de boite englobante ({1, 1} = translation d'une boite en haut et d'une boite a droite)
template <typename T>
std::vector<Position<T>> translationRelative(const std::vector<Position<T>> &pos, const Position<double> &vect);

//Renvoie l'ensemble de points apres le zoom de quantite qte centre sur le centre de la boite englobante
template <typename T>
std::vector<Position<T>> zoomRelatif(const std::vector<Position<T>> &pos, double qte);

template <typename T>
Position<T> barycentre(const std::vector<Position<T>> &pos);


#include "geometrie.cpp"

#endif