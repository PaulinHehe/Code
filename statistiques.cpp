#include "statistiques.h"

bool intervertir_lignes_colonnes_csv(const string &nomFichier)
{
    std::vector<std::vector<string>> data;
    ifstream fin(nomFichier);
    if (!fin) return false;
    
    string line;
    while (getline(fin, line))
    {
        stringstream lineS(line);
        string cell;
        data.push_back({});
        while (getline(lineS, cell, ';'))
        {
            data.back().push_back(cell);
        }
    }
    fin.close();
    
    std::vector<std::vector<string>> dataT(data[0].size(), std::vector<string>(data.size()));
    for (int i = 0; i < data.size(); i++)
    {
        for (int j = 0; j < data[i].size(); j++)
        {
            dataT[j][i] = data[i][j];
        }
    }

    ofstream fout(nomFichier);
    for (int i = 0; i < dataT.size(); i++)
    {
        for (int j = 0; j < dataT[i].size(); j++)
        {
            if (j != 0) fout << ";";
            fout << dataT[i][j];
        }
        fout << "\n";
    }
    fout.close();

    return true;
}

std::vector<std::vector<ValIter<double>>> creer_stats_quantite_acceptation(const std::vector<ValIter<double>> &evolAccepte, const std::vector<ValIter<double>> &evolImprove, int tailleBloc)
{
    std::vector<std::vector<ValIter<double>>> stats(4);
    for (int iter = 0; iter < evolAccepte.size(); iter += tailleBloc)
    {
        int decalage = 0;
        std::vector<ValIter<double>> bloc(stats.size(), ValIter<double>{0, iter});
        while (decalage < tailleBloc && iter + decalage < evolAccepte.size())
        {
            int id = iter + decalage;
            if (evolAccepte[id].val)
            {
                if (evolImprove[id].val > 0)
                {
                    bloc[3].val += 1;
                }
                else if (evolImprove[id].val == 0)
                {
                    bloc[2].val += 1;
                }
                else
                {
                    bloc[1].val += 1;
                }
            }
            else
            {
                bloc[0].val += 1;
            }
            decalage++;
        }
        for (int i = 0; i < stats.size(); i++)
        {
            stats[i].push_back(bloc[i]);
        }
    }
    return stats;
}

std::vector<std::vector<ValIter<double>>> creer_stats_deterioration_maximum(const std::vector<ValIter<double>> &evolScore)
{
    std::vector<std::vector<ValIter<double>>> ans(2);
    int hauteur = 0, hauteurMin = 0;
    for (int i = 1; i < evolScore.size(); i++)
    {
        double evol = evolScore[i].val - evolScore[i-1].val;
        hauteur += evol;
        hauteurMin = min(hauteurMin, hauteur);
        ans[0].push_back({(double)hauteur, evolScore[i].iter});
        ans[1].push_back({(double)(hauteur - hauteurMin), evolScore[i].iter});
    }
    return ans;
}