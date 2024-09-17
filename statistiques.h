#ifndef INCLUDED_STATISTIQUES_H
#define INCLUDED_STATISTIQUES_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

const char CSV_SEPARATOR = ';';

//Structure pour stocker une donnee avec l'iteration a laquelle on l'a obtenue
//Exemple : score a une iteration donnee
template <typename T>
struct ValIter
{
    T val;
    int iter;

    //Test sur val
    bool operator<(const ValIter<T> &autre) const;
};
//Format '(iter, val)'
template <typename T>
ostream& operator<<(ostream& ost, const ValIter<T> &v);

template <typename T>
struct ValFreq
{
    T val;
    int freq;
    int total;

    //Test sur val
    bool operator<(const ValFreq<T> &autre) const;
};
//Format 'val : xx% (freq/total)'
template <typename T>
ostream& operator<<(ostream& ost, const ValFreq<T> &v);

//Genere un fichier csv a partir des valeurs 'vals' suivant ce format :
//Ligne 1 : Nombre d'iterations de vals[0]
//Pour chaque ligne i dans vals : Valeurs dans vals[i]
//ATTENTION : Le nombre d'iterations affiches suit vals[0] donc il faut appeler fusionnerScores au prealable
template <typename T>
void genere_csv_valiter(const std::vector<std::vector<ValIter<T>>> &vals, const string &nomFichier);

//Genere un fichier csv a partir des valeurs 'vals'
//Chaque ligne du tableau 'vals' est une ligne du fichier csv
//Chaque valeur du tableau 'vals' est une case du fichier csv
template <typename T>
void genere_csv_vector(const std::vector<std::vector<T>> &vals, const string &nomFichier);

//Rajoute les iterations manquantes pour que chacun des recuits aient les memes iterations
//scores[i] contient le tableau des meilleurs scores du recuit numero i
//Dans le tableau renvoye, scores[i][j].iter est le meme pour tous les i
template <typename T>
std::vector<std::vector<ValIter<T>>> fusionnerScores(const std::vector<std::vector<ValIter<T>>> &scores);

//Calcule le tableau des frequences correspondant au tableau de valeurs tab
template <typename T>
std::vector<ValFreq<T>> calc_frequences(const std::vector<ValIter<T>> &tab);

//Transpose le fichier csv
bool intervertir_lignes_colonnes_csv(const string &nomFichier);

//Format (par blocs de 'tailleBloc' iterations):
//ans[0] : nombre de deteriorations refusees
//ans[1] : nombre de deteriorations acceptees
//ans[2] : nombre de changements a 0 croisement
//ans[3] : nombre d'ameliorations
std::vector<std::vector<ValIter<double>>> creer_stats_quantite_acceptation(const std::vector<ValIter<double>> &evolAccepte, const std::vector<ValIter<double>> &evolImprove, int tailleBloc);

//Format :
//ans[0] : hauteur du score actuel (amelioration = -1, deterioration = +1)
//ans[1] : montee maximum effectuee depuis derniere hauteur minimum
std::vector<std::vector<ValIter<double>>> creer_stats_deterioration_maximum(const std::vector<ValIter<double>> &evolScore);

#include "statistiques_template.cpp"

#endif