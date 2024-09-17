#include "recuit_simule_depart_dynamique.h"

RecuitSimuleDepartDynamique::RecuitSimuleDepartDynamique(double pDep, int nbSimtempDep, double cool, double coefSeuil, int nbRec, int maxNbRec, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps):
    RecuitSimule{0.0, 0.0, cool, 0.0, nbRec, maxNbRec, selectN, selectE, limiteTps}, pourcentageDepart{pDep}, nbSimulationsTempDep{nbSimtempDep}, coefficientSeuil{coefSeuil}
{

}

RecuitSimuleDepartDynamique::RecuitSimuleDepartDynamique(const RecuitSimuleDepartDynamique &R):
    RecuitSimule{R}
{
    pourcentageDepart = R.pourcentageDepart;
    nbSimulationsTempDep = R.nbSimulationsTempDep;
    coefficientSeuil = R.coefficientSeuil;
}

RecuitSimuleDepartDynamique& RecuitSimuleDepartDynamique::operator=(const RecuitSimuleDepartDynamique &R)
{
    RecuitSimule::operator=(R);
    pourcentageDepart = R.pourcentageDepart;
    nbSimulationsTempDep = R.nbSimulationsTempDep;
    coefficientSeuil = R.coefficientSeuil;
    return *this;
}

RecuitSimuleDepartDynamique* RecuitSimuleDepartDynamique::clone() const
{
    return new RecuitSimuleDepartDynamique(*this);
}

void RecuitSimuleDepartDynamique::execute(Graphe &G)
{
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    RecuitSimuleDepartDynamique::rerecuit(G);
    if (estFonctionDepart) tempsDepart = -1;
}

void RecuitSimuleDepartDynamique::rerecuit(Graphe &G)
{
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    int nbSansAmelioration = 0;
    int nbIter = 0;
    while (!limiteTempsAtteinte() && nbIter == 0 || (G.getScoreTotal() > 0 && nbSansAmelioration < getNbRecuitsSansAmelioration() && nbIter < getMaxNbRecuits()))
    {
        double tDep = _calcTempDep(G);
        cout << "Temperature de depart : " << tDep << "\n";
        setTempDep(tDep);
        setSeuilTemp(tDep * coefficientSeuil);

        nbIter++;
        long long scoreAvant = G.getScoreTotal();
        std::vector<int> placementAvant = G.getPlacementNoeuds();
        RecuitSimule::recuit(G);
        cout << "Apres recuit numero " << nbIter << " : " << G.getScoreTotal() << "\n";
        long long scoreApres = G.getScoreTotal();
        if (scoreApres >= scoreAvant)
        {
            nbSansAmelioration++;
            G.placeNoeuds(placementAvant);
        }
        else
        {
            nbSansAmelioration = 0;
        }
    }
    if (estFonctionDepart) tempsDepart = -1;
}

double RecuitSimuleDepartDynamique::getPourcentageDepart() const
{
    return pourcentageDepart;
}

int RecuitSimuleDepartDynamique::getNbSimulationsTempDep() const
{
    return nbSimulationsTempDep;
}

double RecuitSimuleDepartDynamique::getCoefficientSeuil() const
{
    return coefficientSeuil;
}

void RecuitSimuleDepartDynamique::setPourcentageDepart(double pDep)
{
    pourcentageDepart = pDep;
}

void RecuitSimuleDepartDynamique::setNbSimulationsTempDep(int nbSimTempDep)
{
    nbSimulationsTempDep = nbSimTempDep;
}

void RecuitSimuleDepartDynamique::setCoefficientSeuil(double coefSeuil)
{
    coefficientSeuil = coefSeuil;
}

double RecuitSimuleDepartDynamique::_calcTempDep(Graphe &G)
{
    std::vector<long long int> changements;
    int nbAmeliorations = 0;
    for (int i = 0; i < nbSimulationsTempDep; i++)
    {
        //Le clonage evite la modification des selecteurs
        SelecteurNoeud* selectN = getSelecteurNoeud().clone();
        SelecteurEmplacement* selectE = getSelecteurEmplacement().clone();
        int idNoeud = selectN->selectionneNoeud(G);
        int idEmp = selectE->selectionneEmplacement(G, idNoeud);
        long long int improve = G.calcAmeliorationPlacement(idNoeud, idEmp);
        if (improve > 0) nbAmeliorations++;
        changements.push_back(improve);
        delete selectN;
        delete selectE;
    }
    cout << "nombre ameliorations : " << nbAmeliorations << "\n";
    if (nbAmeliorations > 0) return 0.000001;
    sort(changements.begin(), changements.end());
    int nbAcceptes = round(changements.size() * pourcentageDepart);
    if (nbAcceptes == 0)
    {
        return 0.000001; //Valeur infiniment petite
    }
    double maxAccepte = max((double)-changements[changements.size() - nbAcceptes], 0.000001);
    return maxAccepte;
}