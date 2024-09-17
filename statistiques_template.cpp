#ifndef INCLUDED_STATISTIQUES_TEMPLATE_CPP
#define INCLUDED_STATISTIQUES_TEMPLATE_CPP
#include "statistiques.h"
#include "utilitaire.h"

template <typename T>
bool ValIter<T>::operator<(const ValIter<T> &autre) const
{
    return val < autre.val;
}

template <typename T>
ostream& operator<<(ostream& ost, const ValIter<T> &v)
{
    ost << "(" << v.iter << "," << v.val << ")";
    return ost;
}

template <typename T>
bool ValFreq<T>::operator<(const ValFreq<T> &autre) const
{
    return val < autre.val;
}

template <typename T>
ostream& operator<<(ostream& ost, const ValFreq<T> &v)
{
    double pourcentage = 100.0 * v.freq / v.total;
    ost << v.val << " : " << pourcentage << "% (" << v.freq << "/" << v.total << ")\n";
    return ost;
}

template <typename T>
void genere_csv_valiter(const std::vector<std::vector<ValIter<T>>> &vals, const string &nomFichier)
{
    ofstream fout(nomFichier);
    std::vector<std::vector<ValIter<T>>> tab = fusionnerScores(vals);
    fout << "Iter";
    for (auto v : tab[0]) fout << CSV_SEPARATOR << v.iter;
    fout << "\n";
    for (int i = 0; i < tab.size(); i++)
    {
        fout << "Donnees " << i+1;
        for (auto v : tab[i]) fout << CSV_SEPARATOR << v.val;
        fout << "\n";
    }
}

template <typename T>
void genere_csv_vector(const std::vector<std::vector<T>> &vals, const string &nomFichier)
{
    ofstream fout(nomFichier);
    for (int i = 0; i < vals.size(); i++)
    {
        for (int j = 0; j < vals[i].size(); j++)
        {
            if (j != 0) fout << CSV_SEPARATOR;
            fout << vals[i][j];
        }
        fout << "\n";
    }
}

template <typename T>
std::vector<std::vector<ValIter<T>>> fusionnerScores(const std::vector<std::vector<ValIter<T>>> &scores)
{
    std::vector<std::vector<ValIter<T>>> Nscores(scores.size());
    std::vector<int> id(scores.size(), 0);
    int nbFinis = 0;
    while (nbFinis < scores.size())
    {
        int iterMin = 1e9;
        for (int i = 0; i < scores.size(); i++)
        {
            if (id[i] < scores[i].size())
            {
                iterMin = min(iterMin, scores[i][id[i]].iter);
            }
        }
        for (int i = 0; i < scores.size(); i++)
        {
            if (id[i] < scores[i].size())
            {
                if (iterMin == scores[i][id[i]].iter)
                {
                    id[i]++;
                    if (id[i] == scores[i].size())
                    {
                        nbFinis++;
                    }
                }
            }
            if (id[i] == 0) Nscores[i].push_back({(T)0, iterMin});
            else Nscores[i].push_back({scores[i][id[i]-1].val, iterMin});
        }
    }
    return Nscores;
}

template <typename T>
std::vector<ValFreq<T>> calc_frequences(const std::vector<ValIter<T>> &tab)
{
    std::vector<ValIter<T>> t = tab;
    sort(t.begin(), t.end());
    std::vector<ValFreq<T>> freq;
    freq.push_back({t[0].val, 1, (int)t.size()});
    for (int i = 1; i < t.size(); i++)
    {
        if (t[i].val == t[i-1].val)
        {
            freq[freq.size()-1].freq++;
        }
        else
        {
            freq.push_back({t[i].val, 1, (int)t.size()});
        }
    }
    return freq;
}

#endif