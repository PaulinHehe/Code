#ifndef INCLUDED_RECUIT_SIMULE_DYNAMIQUE_H
#define INCLUDED_RECUIT_SIMULE_DYNAMIQUE_H
#include "recuit_simule.h"

class RecuitSimuleDynamique : public RecuitSimule
{
public:
    //pDep : pourcentage de deteriorations a laisser passer pour la temperature de depart
    //nbSimTempDep : nombre de mouvements a tester pour calculer la temperature de depart
    //maxIter : nombre d'itérations a effectuer par execution
    //Ka : coefficient de precision (multiplie la temperature), accelere le recuit si faible mais il devient moins precis (recommande : 1.0)
    //coolDep : coefficient de refroidissement de la temperature initiale (recommande : 0.99)
    //selectN : selecteur de noeud
    //selectE : selecteur d'emplacement
    //limiteTps : limite de temps (en ms), -1 si inactif
    RecuitSimuleDynamique(double pDep, int nbSimTempDep, int maxIter, double Ka, double coolDep, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps);
    virtual ~RecuitSimuleDynamique() = default;
    virtual RecuitSimuleDynamique* clone() const override;

    virtual void execute(Graphe &G) override;
    virtual void etapeRecuit(Graphe &G) override;

    virtual double getTemp() const override;
    int getNbMaxIterations() const;
    double getTempInitiale() const;
    double getKaCoef() const;
    std::vector<ValIter<double>> getEvolDeltaEntropie() const;
    std::vector<ValIter<double>> getEvolDeltaScore() const;

    void setNbMaxIterations(int maxIter);
    void setTempInitiale(double tempInit);
    void setKaCoef(double Ka);
protected:
    std::vector<ValIter<double>> evolDeltaEntropie;
    std::vector<ValIter<double>> evolDeltaScore;

    //Met a jour la temperature initiale (a appeler avant le recuit)
    double calcTemperatureDepart(Graphe &G);

    //Met a jour les attributs de calcul de la temperature
    //nbCActuel : nombre de croisements du graphe a l'iteration actuelle
    //nbCDernierTour : nombre de croisements du graphe a l'iteration precedente
    void miseAJourTemperature(double nbCActuel, double nbCDernierTour);
private:
    //Variables fonctionnelles du recuit
    long long dernierCroisements; //nombre de croisements a l'iteration precedente (pour la temperature)
    bool dernierAccepte; //vrai si le dernier changement a ete accepte

    //Parametres recuit
    double pourcentageDepart;
    int nbSimulationsTempDep;
    int nbMaxIterations;
    double KaCoef; //Quality/runtime tradeoff recommended : 1.0
    double coolDepart; //Refroidissement de la temperature initiale, recommande : 0.99

    //Calcul temperature
    double tempInitiale;
    double tempDynamique;
    double deltaScore;
    double deltaEntropie;
};

#endif