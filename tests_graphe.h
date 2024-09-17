#ifndef TESTSGRAPHE_H
#define TESTSGRAPHE_H

#include "graphe.h"

Position<double> generePosition(int xmax=100, int ymax=100);
void genereNoeuds(Graphe& G, int n, int min=0);
void genereEmplacements(Graphe& G, int n, int min=0);
void genereAretes(Graphe& G, int n, int min=0);
void placeAleatoire(Graphe& G);

void affichePlacement(Graphe& G);
void afficheNoeuds(Graphe& G);
void remplirGraphe(Graphe &G, int nbN, int nbE, int nbA);

#endif