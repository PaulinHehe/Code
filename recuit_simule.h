#ifndef INCLUDED_RECUIT_SIMULE_H
#define INCLUDED_RECUIT_SIMULE_H
#include "algos_heuristiques.h"
#include "graphe.h"
#include "selecteur_emplacement.h"
#include "selecteur_noeud.h"
#include "statistiques.h"
#include "display_openGL.h"

class RecuitSimule : public AlgosHeuristiques
{
public:
    //Nombre d'iterations avant de prendre un echantillon statistique
    //Attention si la valeur est a 1, la memoire pour les statistiques est d'environ 100 octets par iteration
    const int INTERVALLE_STATISTIQUES = 1000;

    //tDepInit : temperature de depart
    //coolT : refroidissement de la temperature de depart
    //cool : refroidissement de la temperature pendant le recuit
    //seuil : seuil d'arret du recuit
    //nbRec : nombre de recuits a effectuer a la suite sans amelioration
    //maxNbRec : nombre maximal de recuits a effectuer
    //selectN : selecteur de noeud
    //selectE : selecteur d'emplacement
    //limiteTps : limite de temps (en ms), -1 si inactif
    RecuitSimule(double tDepInit, double coolT, double cool, double seuil, int nbRec, int maxNbRec, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps);
    RecuitSimule(const RecuitSimule &R);
    virtual ~RecuitSimule();
    RecuitSimule& operator=(const RecuitSimule &R);
    virtual RecuitSimule* clone() const override;

    virtual void execute(Graphe &G) override;

    //Effectue plusieurs recuits a la suite
    //Mise a jour automatique de tempDep
    virtual void rerecuit(Graphe &G);
    //Effectue un recuit
    //Mise a jour automatique de temp
    //Modifie tempDep
    virtual void recuit(Graphe &G);
    //Effectue une etape du recuit
    //Modifie numIter, temp et les tableaux de stats
    virtual void etapeRecuit(Graphe &G);
    

    //Getters
    double getTempDepInit() const;
    double getTempDep() const;
    double getCoolTempDep() const;
    virtual double getTemp() const;
    double getCoolTemp() const;
    double getSeuilTemp() const;
    int getNbRecuitsSansAmelioration() const;
    int getMaxNbRecuits() const;
    const SelecteurNoeud& getSelecteurNoeud() const;
    SelecteurNoeud& getSelecteurNoeud();
    const SelecteurEmplacement& getSelecteurEmplacement() const;
    SelecteurEmplacement& getSelecteurEmplacement();
    long long int getLimiteTemps() const;
    //Getters statistiques
    int getNumIter() const;
    std::vector<int> getMeilleurPlacement() const;
    std::vector<ValIter<double>> getMeilleursScores() const;
    std::vector<ValIter<double>> getEvolImprove() const;
    std::vector<ValIter<double>> getEvolProba() const;
    std::vector<ValIter<double>> getEvolAccepte() const;
    std::vector<ValIter<double>> getEvolScore() const;
    std::vector<ValIter<double>> getEvolTemp() const;

    //Setters
    void setTemp(int t);
    void setTempDepInit(double tDepInit);
    void setTempDep(double tDep);
    void setCoolTempDep(double coolT);
    void setTemp(double t);
    void setCoolTemp(double cool);
    void setSeuilTemp(double seuil);
    void setNbRecuitsSansAmelioration(int nbRec);
    void setMaxNbRecuits(int maxNbRec);
    void setSelecteurNoeud(const SelecteurNoeud &selectN);
    void setSelecteurEmplacement(const SelecteurEmplacement &selectE);
    void setLimiteTemps(long long int limiteTps);
    //Reinitialise le tableau des meilleurs scores et placements
    void resetStatistiques();
protected:
    //Attribut fonctionnel du recuit
    long long int limiteTemps; //en ms
    long long int tempsDepart; //en ms (-1 si non actif)

    //Statistiques
    //Garantie : les tableaux contiennent toujours la valeur de la derniere iteration effectuee (temporairement cependant)
    bool derniereStatTemporaire; //vrai si les dernieres valeurs des tableaux de stats sont des valeurs temporaires (ni meilleur score, ni dans INTERVALLE_STATISTIQUES)
    int numIter;
    std::vector<int> meilleurPlacement;
    std::vector<ValIter<double>> meilleursScores; //meilleur score
    std::vector<ValIter<double>> evolImprove; //tableau des improve
    std::vector<ValIter<double>> evolProba; //probabilite d'accepter le changement
    std::vector<ValIter<double>> evolAccepte; //vrai si on a accepte le changement
    std::vector<ValIter<double>> evolScore; //score
    std::vector<ValIter<double>> evolTemp; //temperature

    //Ajoute 'ajout' a numIter
    void ajouteIter(int ajout);
    //Initialise les tableaux des statistiques si vides
    //G : graphe sur lequel le recuit est execute
    void initStatistiques(const Graphe &G);
    //Met a jour les tableaux des statistiques pour cette etape
    //G : graphe sur lequel le recuit est execute
    //improve : amelioration en nombre de croisements cette etape-ci
    //accepte : vrai si le changement a ete accepte cette etape-ci
    void miseAJourStatistiques(const Graphe &G, int improve, bool accepte);
    
    //Renvoie la probabilite d'effectuer le mouvement (entre 0 et 1)
    virtual double getProbabiliteMouvement(long long improve, double temp) const;
    //Renvoie vrai si le recuit effectue le mouvement, faux si le mouvement est refuse
    virtual bool effectueMouvement(long long improve, double temp) const;

    //Renvoie vrai si la limite de temps est atteinte
    bool limiteTempsAtteinte() const;
private:
    //Attributs fonctionnels du recuit
    double tempDepInitiale;
    double tempDep;
    double coolTempDep;
    double temp;
    double coolTemp;
    double seuilTemp;
    int nbRecuitsSansAmelioration;
    int maxNbRecuits;
    SelecteurNoeud* selecteurNoeud;
    SelecteurEmplacement* selecteurEmplacement;
    GLFWwindow* window;
    GLFWwindow* textRender;


    //Copie les attributs de R dans this
    //Utilise par le constructeur par recopie et l'operateur =
    void _copieAttributs(const RecuitSimule &R);
};



#endif