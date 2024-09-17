#ifndef INCLUDED_UTILITAIRE_TEMPLATE_CPP
#define INCLUDED_UTILITAIRE_TEMPLATE_CPP
#include "utilitaire.h"

template <typename T>
void melangeTab(std::vector<T> &tab)
{
    for (int i = tab.size()-1; i > 0; i--)
    {
        int id = generateRand(i+1);
        swap(tab[i], tab[id]);
    }
}

template <typename T>
bool contientTab(const std::vector<T> &tab, const T &val)
{
    for (int i = 0; i < tab.size(); i++)
    {
        if (tab[i] == val)
        {
            return true;
        }
    }
    return false;
}

template <typename T>
bool retireTab(std::vector<T> &tab, const T &val)
{
    for (int i = 0; i < tab.size(); i++)
    {
        if (tab[i] == val)
        {
            tab[i] = tab.back();
            tab.pop_back();
            return true;
        }
    }
    return false;
}

template <typename T>
double moyenne(const std::vector<T> &tab)
{
    double S = 0.0;
    for (T v : tab) S += v;
    return S/tab.size();
}

template <typename T>
double variance(const std::vector<T> &tab)
{
    double moy = moyenne(tab);
    double S = 0.0;
    for (T v : tab) S += (v - moy) * (v - moy);
    return S/tab.size();
}

template <typename T>
double ecartType(const std::vector<T> &tab)
{
    return sqrt(variance(tab));
}

template <typename T>
double medianne(const std::vector<T> &tab)
{
    std::vector<T> tabT = tab;
    sort(tabT.begin(), tabT.end());
    if (tabT.size()%2 == 0)
    {
        return (tabT[tabT.size()/2-1] + tabT[tabT.size()/2]) * 0.5;
    }
    else
    {
        return tabT[tabT.size()/2];
    }
}

template <typename T>
T maximum(const std::vector<T> &tab)
{
    T maxi = tab[0];
    for (int i = 1; i < tab.size(); i++) maxi = max(maxi, tab[i]);
    return maxi;
}

template <typename T>
T minimum(const std::vector<T> &tab)
{
    T mini = tab[0];
    for (int i = 1; i < tab.size(); i++) mini = min(mini, tab[i]);
    return mini;
}

template <typename T>
void affiche(const std::vector<T> &tab)
{
    for (T v : tab) cout << v << " ";
    cout << "\n";
}

template <typename T>
void affiche(const std::vector<std::vector<T>> &tab)
{
    for (std::vector<T> v : tab) affiche(v);
}

#endif