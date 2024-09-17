#include "recuit_simule_dynamique.h"

RecuitSimuleDynamique::RecuitSimuleDynamique(double pDep, int nbSimTempDep, int maxIter, double Ka, double coolDep, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps):
    RecuitSimule{1.0, 1.0, 1.0, 1.0, 0, 1, selectN, selectE, limiteTps}, pourcentageDepart{pDep}, nbSimulationsTempDep{nbSimTempDep}, nbMaxIterations{maxIter}, KaCoef{Ka}, coolDepart{coolDep}
{

}

RecuitSimuleDynamique* RecuitSimuleDynamique::clone() const
{
    return new RecuitSimuleDynamique(*this);
}

void RecuitSimuleDynamique::execute(Graphe &G)
{
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    tempInitiale = calcTemperatureDepart(G);
    deltaScore = 0.0;
    deltaEntropie = 0.0;
    dernierCroisements = -1;
    int nbIter = 0;
    while (!limiteTempsAtteinte() && G.getScoreTotal() > 0 && nbIter < nbMaxIterations)
    {
        etapeRecuit(G);
        nbIter++;
    }
    G.placeNoeuds(meilleurPlacement);
    if (estFonctionDepart) tempsDepart = -1;
}

void RecuitSimuleDynamique::etapeRecuit(Graphe &G)
{
    miseAJourTemperature(G.getScoreTotal(), dernierCroisements);
    dernierCroisements = G.getScoreTotal();
    RecuitSimule::etapeRecuit(G);
}

double RecuitSimuleDynamique::getTemp() const
{
    return tempDynamique;
}

int RecuitSimuleDynamique::getNbMaxIterations() const
{
    return nbMaxIterations;
}

double RecuitSimuleDynamique::getTempInitiale() const
{
    return tempInitiale;
}

double RecuitSimuleDynamique::getKaCoef() const
{
    return KaCoef;
}

std::vector<ValIter<double>> RecuitSimuleDynamique::getEvolDeltaEntropie() const
{
    return evolDeltaEntropie;
}

std::vector<ValIter<double>> RecuitSimuleDynamique::getEvolDeltaScore() const
{
    return evolDeltaScore;
}

void RecuitSimuleDynamique::setNbMaxIterations(int maxIter)
{
    nbMaxIterations = maxIter;
}

void RecuitSimuleDynamique::setTempInitiale(double tempInit)
{
    tempInitiale = tempInit;
}

void RecuitSimuleDynamique::setKaCoef(double Ka)
{
    KaCoef = Ka;
}

double RecuitSimuleDynamique::calcTemperatureDepart(Graphe &G)
{
    std::vector<long long int> changements;
    for (int i = 0; i < nbSimulationsTempDep; i++)
    {
        //Le clonage evite la modification des selecteurs
        SelecteurNoeud* selectN = getSelecteurNoeud().clone();
        SelecteurEmplacement* selectE = getSelecteurEmplacement().clone();
        int idNoeud = selectN->selectionneNoeud(G);
        int idEmp = selectE->selectionneEmplacement(G, idNoeud);
        long long int improve = G.calcAmeliorationPlacement(idNoeud, idEmp);
        changements.push_back(improve);
        delete selectN;
        delete selectE;
    }
    sort(changements.begin(), changements.end());
    while (changements.back() >= 0)
    {
        changements.pop_back();
    }
    if (changements.empty())
    {
        return G.getScoreTotal() / 10.0;
    }
    int nbAcceptes = round(changements.size() * pourcentageDepart);
    if (nbAcceptes == 0) nbAcceptes = 1;
    double maxAccepte = (double)-changements[changements.size() - nbAcceptes];
    return maxAccepte;
}

void RecuitSimuleDynamique::miseAJourTemperature(double nbCActuel, double nbCDernierTour)
{
    if (nbCDernierTour != -1)
    {
        deltaScore += nbCActuel - nbCDernierTour;
    }
    if (!evolAccepte.empty() && evolAccepte.back().val)
    {
        deltaEntropie += log(evolProba.back().val);
    }
    if (deltaScore >= 0 || deltaEntropie == 0)
    {
        tempDynamique = tempInitiale;
    }
    else
    {
        tempDynamique = KaCoef * deltaScore / deltaEntropie;
    }

    if (derniereStatTemporaire && !evolDeltaScore.empty())
    {
        evolDeltaScore.pop_back();
        evolDeltaEntropie.pop_back();
    }
    evolDeltaEntropie.push_back({deltaEntropie, numIter+1});
    evolDeltaScore.push_back({deltaScore, numIter+1});
    
    RecuitSimule::setTemp(getTemp());
    tempInitiale *= coolDepart;
}