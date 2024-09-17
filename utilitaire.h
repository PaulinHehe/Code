#ifndef INCLUDED_UTILITAIRE_H
#define INCLUDED_UTILITAIRE_H
#include <random>
#include "position.h"
#include <chrono>
#include <ctime>
#include <vector>
#include <cmath>
#include <mutex>
using namespace std::chrono;
using namespace std;

//Renvoie le nombre de millisecondes depuis l'epoch
long long int getMS();
//Thread safe rand function
int getRand();
//Renvoie un nombre entier aleatoire dans [0, n[
int generateRand(int n);
//Renvoie un nombre reel aleatoire dans [mini, maxi[
double generateRandDouble(double mini, double maxi);

//Melange un tableau
template <typename T>
void melangeTab(std::vector<T> &tab);
//Renvoie vrai si le tableau contient la valeur val
template <typename T>
bool contientTab(const std::vector<T> &tab, const T &val);
//Retire la valeur val du tableau (ne conserve pas l'ordre)
template <typename T>
bool retireTab(std::vector<T> &tab, const T &val);

//Renvoie la moyenne du tableau
template <typename T>
double moyenne(const std::vector<T> &tab);
//Renvoie la variance du tableau
template <typename T>
double variance(const std::vector<T> &tab);
//Renvoie l'ecart-type du tableau
template <typename T>
double ecartType(const std::vector<T> &tab);
//Renvoie la medianne du tableau
template <typename T>
double medianne(const std::vector<T> &tab);
//Renvoie le maximum du tableau
template <typename T>
T maximum(const std::vector<T> &tab);
//Renvoie le minimum du tableau
template <typename T>
T minimum(const std::vector<T> &tab);

//Affiche un tableau sur une ligne
template <typename T>
void affiche(const std::vector<T> &tab);
//Affiche un tableau 2D
template <typename T>
void affiche(const std::vector<std::vector<T>> &tab);


#include "utilitaire_template.cpp"

#endif