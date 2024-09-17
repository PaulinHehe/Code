#include "glouton_meilleur_deplacement.h"
#include "utilitaire.h"

void GloutonMeilleurDeplacement::execute(Graphe &G)
{
    bool fini = false;
    while (!fini)
    {
        int idNoeud, idEmp;
        long long improve = chercheMeilleureAmelioration(G, idNoeud, idEmp);
        if (idNoeud == -1) fini = true;
        else
        {
            G.placeNoeud(idNoeud, idEmp);
        }
    }
}

GloutonMeilleurDeplacement* GloutonMeilleurDeplacement::clone() const
{
    return new GloutonMeilleurDeplacement(*this);
}

long long GloutonMeilleurDeplacement::chercheMeilleureAmelioration(Graphe &G, int &idNoeudTrouve, int &idEmpTrouve)
{
    long long bestImprove = 0;
    idEmpTrouve = -1;
    idNoeudTrouve = -1;
    for (int idNoeud = 0; idNoeud < G.getNbNoeuds(); idNoeud++)
    {
        int idE;
        long long improve = chercheMeilleureAmeliorationNoeud(G, idNoeud, idE);
        if (improve > bestImprove)
        {
            idNoeudTrouve = idNoeud;
            idEmpTrouve = idE;
            bestImprove = improve;
        }
    }
    return bestImprove;
}

long long GloutonMeilleurDeplacement::chercheMeilleureAmeliorationNoeud(Graphe &G, int idNoeud, int &idEmpTrouve)
{
    long long bestImprove = 0;
    idEmpTrouve = -1;
    for (int idE = 0; idE < G.getNbEmplacements(); idE++)
    {
        long long improve = G.calcAmeliorationPlacement(idNoeud, idE);
        if (improve > bestImprove)
        {
            idEmpTrouve = idE;
            bestImprove = improve;
        }
    }
    return bestImprove;
}