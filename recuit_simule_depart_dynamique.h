#ifndef INCLUDED_RECUIT_SIMULE_DEPART_DYNAMIQUE
#define INCLUDED_RECUIT_SIMULE_DEPART_DYNAMIQUE
#include "recuit_simule.h"

class RecuitSimuleDepartDynamique : public RecuitSimule
{
public:
    //pDep : pourcentage de deteriorations a accepter au depart d'un recuit
    //nbSimTempDep : nombre de simulations d'etapes du recuit a effectuer pour selectionner la temperature de depart
    //cool : refroidissement de la temperature pendant le recuit
    //coefSeuil : coefficient pour le seuil de fin du recuit, seuil = coefSeuil * tempDep
    //nbRec : nombre de recuits a effectuer a la suite sans amelioration
    //maxNbRec : nombre maximal de recuits a effectuer
    //selectN : selecteur de noeud
    //selectE : selecteur d'emplacement
    //limiteTps : limite de temps (en ms), -1 si inactif
    RecuitSimuleDepartDynamique(double pDep, int nbSimTempDep, double cool, double coefSeuil, int nbRec, int maxNbRec, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps);
    RecuitSimuleDepartDynamique(const RecuitSimuleDepartDynamique &R);
    virtual ~RecuitSimuleDepartDynamique() = default;
    RecuitSimuleDepartDynamique& operator=(const RecuitSimuleDepartDynamique &R);
    virtual RecuitSimuleDepartDynamique* clone() const override;

    virtual void execute(Graphe &G) override;
    virtual void rerecuit(Graphe &G) override;

    //Getters
    double getPourcentageDepart() const;
    int getNbSimulationsTempDep() const;
    double getCoefficientSeuil() const;

    //Setters
    void setPourcentageDepart(double pDep);
    void setNbSimulationsTempDep(int nbSimTempDep);
    void setCoefficientSeuil(double coefSeuil);
private:
    double pourcentageDepart;
    int nbSimulationsTempDep;
    double coefficientSeuil;

    double _calcTempDep(Graphe &G);
};

#endif