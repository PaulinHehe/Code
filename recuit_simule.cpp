#include "recuit_simule.h"
#include <cmath>
#include "utilitaire.h"

RecuitSimule::RecuitSimule(double tDepInit, double coolT, double cool, double seuil, int nbRec, int maxNbRec, const SelecteurNoeud &selectN, const SelecteurEmplacement &selectE, long long int limiteTps):
    tempDepInitiale{tDepInit}, tempDep{tDepInit}, coolTempDep{coolT}, temp{tDepInit}, coolTemp{cool}, seuilTemp{seuil}, nbRecuitsSansAmelioration{nbRec}, maxNbRecuits{maxNbRec}, selecteurNoeud{selectN.clone()}, selecteurEmplacement{selectE.clone()}, limiteTemps{limiteTps}, tempsDepart{-1}, window{nullptr}, textRender{nullptr}
{
    resetStatistiques();
}

RecuitSimule::~RecuitSimule()
{
    delete selecteurNoeud;
    delete selecteurEmplacement;
}

RecuitSimule::RecuitSimule(const RecuitSimule &R)
{
    _copieAttributs(R);
}

RecuitSimule& RecuitSimule::operator=(const RecuitSimule &R)
{
    _copieAttributs(R);
    return *this;
}

RecuitSimule* RecuitSimule::clone() const
{
    return new RecuitSimule(*this);
}


void RecuitSimule::execute(Graphe &G)
{
    resetStatistiques();
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    rerecuit(G);
    if (estFonctionDepart) tempsDepart = -1;
}

void RecuitSimule::rerecuit(Graphe &G)
{
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    tempDep = tempDepInitiale;
    int nbSansAmelioration = 0;
    int nbIter = 0;
    while (!limiteTempsAtteinte() && nbIter == 0 || (G.getScoreTotal() > 0 && nbSansAmelioration < nbRecuitsSansAmelioration && nbIter < maxNbRecuits))
    {
        long long startRecuit = getMS();
        nbIter++;
        cout << "Recuit n." << nbIter << endl;
        long long scoreAvant = G.getScoreTotal();
        recuit(G);
        long long scoreApres = G.getScoreTotal();
        //cout << "Score apres recuit close " << nbIter << " : " << scoreApres << "\n";
        if (scoreApres >= scoreAvant)
        {
            nbSansAmelioration++;
        }
        else
        {
            nbSansAmelioration = 0;
        }
        long long endRecuit = getMS() - startRecuit;
    }
    if (estFonctionDepart) tempsDepart = -1;
}

void RecuitSimule::recuit(Graphe &G){
    #ifdef WIN32
        GetAsyncKeyState(VK_SPACE);
    #endif
    bool estFonctionDepart = (tempsDepart == -1);
    if (estFonctionDepart) tempsDepart = getMS();
    temp = tempDep;
    int nbIter = 0;
    while (!limiteTempsAtteinte() && G.getScoreTotal() > 0 && fabs(temp) > fabs(seuilTemp))
    {     
        keyPressCheck(window, textRender, G, this);
        if(window){
            frameAffichage(window, textRender, G);  
        }
        nbIter++;
        etapeRecuit(G);
        
    }
    G.placeNoeuds(meilleurPlacement);
    tempDep *= coolTempDep;
    if (estFonctionDepart) tempsDepart = -1;
    
}

void RecuitSimule::etapeRecuit(Graphe &G)
{
    initStatistiques(G);

    int idNoeud = selecteurNoeud->selectionneNoeud(G);
    int idEmp = selecteurEmplacement->selectionneEmplacement(G, idNoeud);
    int ancienEmp = G.getNoeud(idNoeud).getEmplacement();
    long long ancienScore = G.getScoreTotal();
    
    G.placeNoeud(idNoeud, idEmp);
    
    long long improve = ancienScore - G.getScoreTotal();
    bool accepte = effectueMouvement(improve, getTemp());
    if (!accepte)
    {
        G.placeNoeud(idNoeud, ancienEmp);
    }

    miseAJourStatistiques(G, improve, accepte);
    
    temp *= coolTemp;
}


double RecuitSimule::getTempDepInit() const
{
    return tempDepInitiale;
}

double RecuitSimule::getTempDep() const
{
    return tempDep;
}

double RecuitSimule::getCoolTempDep() const
{
    return coolTempDep;
}

double RecuitSimule::getTemp() const
{
    return temp;
}

double RecuitSimule::getCoolTemp() const
{
    return coolTemp;
}

double RecuitSimule::getSeuilTemp() const
{
    return seuilTemp;
}

int RecuitSimule::getNbRecuitsSansAmelioration() const
{
    return nbRecuitsSansAmelioration;
}

int RecuitSimule::getMaxNbRecuits() const
{
    return maxNbRecuits;
}

const SelecteurNoeud& RecuitSimule::getSelecteurNoeud() const
{
    return *selecteurNoeud;
}

SelecteurNoeud& RecuitSimule::getSelecteurNoeud()
{
    return *selecteurNoeud;
}

const SelecteurEmplacement& RecuitSimule::getSelecteurEmplacement() const
{
    return *selecteurEmplacement;
}

SelecteurEmplacement& RecuitSimule::getSelecteurEmplacement()
{
    return *selecteurEmplacement;
}

long long int RecuitSimule::getLimiteTemps() const
{
    return limiteTemps;
}

int RecuitSimule::getNumIter() const
{
    return numIter;
}

std::vector<int> RecuitSimule::getMeilleurPlacement() const
{
    return meilleurPlacement;
}

std::vector<ValIter<double>> RecuitSimule::getMeilleursScores() const
{
    return meilleursScores;
}

std::vector<ValIter<double>> RecuitSimule::getEvolImprove() const
{
    return evolImprove;
}

std::vector<ValIter<double>> RecuitSimule::getEvolProba() const
{
    return evolProba;
}

std::vector<ValIter<double>> RecuitSimule::getEvolAccepte() const
{
    return evolAccepte;
}

std::vector<ValIter<double>> RecuitSimule::getEvolScore() const
{
    return evolScore;
}

std::vector<ValIter<double>> RecuitSimule::getEvolTemp() const
{
    return evolTemp;
}

void RecuitSimule::setTemp(int t){
    temp = t;
}

void RecuitSimule::setTempDepInit(double tDepInit)
{
    tempDepInitiale = tDepInit;
}

void RecuitSimule::setTempDep(double tDep)
{
    tempDep = tDep;
}

void RecuitSimule::setCoolTempDep(double coolT)
{
    coolTempDep = coolT;
}

void RecuitSimule::setTemp(double t)
{
    temp = t;
}

void RecuitSimule::setCoolTemp(double cool)
{
    coolTemp = cool;
}

void RecuitSimule::setSeuilTemp(double seuil)
{
    seuilTemp = seuil;
}

void RecuitSimule::setNbRecuitsSansAmelioration(int nbRec)
{
    nbRecuitsSansAmelioration = nbRec;
}

void RecuitSimule::setMaxNbRecuits(int maxNbRec)
{
    maxNbRecuits = maxNbRec;
}

void RecuitSimule::setSelecteurNoeud(const SelecteurNoeud &selectN)
{
    selecteurNoeud = selectN.clone();
}

void RecuitSimule::setSelecteurEmplacement(const SelecteurEmplacement &selectE)
{
    selecteurEmplacement = selectE.clone();
}

void RecuitSimule::setLimiteTemps(long long int limiteTps)
{
    limiteTemps = limiteTps;
}

void RecuitSimule::resetStatistiques()
{
    meilleursScores.clear();
    evolImprove.clear();
    evolProba.clear();
    evolAccepte.clear();
    evolScore.clear();
    evolTemp.clear();
    numIter = 0;
    derniereStatTemporaire = false;
}

void RecuitSimule::ajouteIter(int ajout)
{
    numIter += ajout;
}

void RecuitSimule::initStatistiques(const Graphe &G)
{
    if (meilleurPlacement.empty())
    {
        meilleurPlacement = G.getPlacementNoeuds();
    }
    if (meilleursScores.empty())
    {
        meilleursScores.push_back({(double)G.getScoreTotal(), numIter});
        evolScore.push_back({(double)G.getScoreTotal(), numIter});
    }
}

void RecuitSimule::miseAJourStatistiques(const Graphe &G, int improve, bool accepte)
{
    numIter++;
    if (G.getScoreTotal() < meilleursScores.back().val)
    {
        meilleursScores.push_back({(double)G.getScoreTotal(), numIter});
        meilleurPlacement = G.getPlacementNoeuds();
    }
    if (derniereStatTemporaire && !evolScore.empty())
    {
        evolImprove.pop_back();
        evolProba.pop_back();
        evolAccepte.pop_back();
        evolScore.pop_back();
        evolTemp.pop_back();
    }
    evolImprove.push_back({(double)improve, numIter});
    evolProba.push_back({getProbabiliteMouvement(improve, getTemp()), numIter});
    evolAccepte.push_back({(double)accepte, numIter});
    evolScore.push_back({(double)G.getScoreTotal(), numIter});
    evolTemp.push_back({getTemp(), numIter});
    derniereStatTemporaire = !(numIter%INTERVALLE_STATISTIQUES == 0 || meilleursScores.back().iter == numIter);
}

double RecuitSimule::getProbabiliteMouvement(long long improve, double temp) const
{
    if (temp == 0.0)
    {
        if (improve >= 0) return 1.0;
        else return 0.0;
    }
    double seuil = exp(improve / temp);
    seuil = min(1.0, seuil);
    seuil = max(0.0, seuil);
    return seuil;
}

bool RecuitSimule::effectueMouvement(long long improve, double temp) const
{
    double proba = getProbabiliteMouvement(improve, temp);
    double nbAlea = generateRandDouble(0.0, 1.0);
    return nbAlea < proba;
}

bool RecuitSimule::limiteTempsAtteinte() const
{
    if (limiteTemps < 0) return false;
    return getMS() - tempsDepart >= limiteTemps;
}

void RecuitSimule::_copieAttributs(const RecuitSimule &R)
{
    //Attributs fonctionnels
    tempDepInitiale = R.tempDepInitiale;
    tempDep = R.tempDep;
    coolTempDep = R.coolTempDep;
    temp = R.temp;
    coolTemp = R.coolTemp;
    seuilTemp = R.seuilTemp;
    nbRecuitsSansAmelioration = R.nbRecuitsSansAmelioration;
    maxNbRecuits = R.maxNbRecuits;
    selecteurNoeud = R.selecteurNoeud->clone();
    selecteurEmplacement = R.selecteurEmplacement->clone();
    limiteTemps = R.limiteTemps;

    //Statistiques
    numIter = R.numIter;
    meilleurPlacement = R.meilleurPlacement;
    meilleursScores = R.meilleursScores;
    evolImprove = R.evolImprove;
    evolProba = R.evolProba;
    evolAccepte = R.evolAccepte;
    evolScore = R.evolScore;
    evolTemp = R.evolTemp;
}