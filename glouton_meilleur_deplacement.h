#ifndef INCLUDED_GLOUTON_MEILLEUR_DEPLACEMENT_H
#define INCLUDED_GLOUTON_MEILLEUR_DEPLACEMENT_H
#include "algos_heuristiques.h"
#include "graphe.h"

class GloutonMeilleurDeplacement : public AlgosHeuristiques
{
public:
    void execute(Graphe &G) override;
    GloutonMeilleurDeplacement* clone() const override;
    
    long long chercheMeilleureAmelioration(Graphe &G, int &idNoeudTrouve, int &idEmpTrouve);
    long long chercheMeilleureAmeliorationNoeud(Graphe &G, int idNoeud, int &idEmpTrouve);
};

#endif