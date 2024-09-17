#include "graphe.h"
#include "intersection.h"
#include "utilitaire.h"
#include <algorithm>
#include <cmath>
using namespace std;

Graphe::Graphe():
    noeuds{}, voisins{}, nbAretes{0}, emplacements{}, coordReel{false}, grille{nullptr}, scoreTotal{0LL}, nbIntersectionsAretes{}, hauteurGrille{1000000}, largeurGrille{1000000}
{

}

Graphe::~Graphe()
{
    delete grille;
}

Graphe::Graphe(const Graphe &G)
{
    _copieGraphe(G);
}

Graphe& Graphe::operator=(const Graphe &G)
{
    _copieGraphe(G);
    return *this;
}

int Graphe::getNbNoeuds() const
{
    return noeuds.size();
}

std::vector<Noeud> Graphe::getNoeuds() const
{
    return noeuds;
}

Noeud Graphe::getNoeud(int idNoeud) const
{
    return noeuds[idNoeud];
}

std::vector<int> Graphe::getPlacementNoeuds() const
{
    std::vector<int> placement;
    for (int i = 0; i < getNbNoeuds(); i++)
    {
        placement.push_back(noeuds[i].getEmplacement());
    }
    return placement;
}

int Graphe::getNbAretes() const
{
    return nbAretes;
}

std::vector<Arete> Graphe::getAretes() const
{
    return aretes;
}

Arete Graphe::getArete(int idArete) const
{
    return aretes[idArete];
}

std::vector<std::vector<int>> Graphe::getVoisins() const
{
    return voisins;
}

std::vector<int> Graphe::getVoisinsNoeud(int idNoeud) const
{
    return voisins[idNoeud];
}

std::vector<std::vector<int>> Graphe::getMatriceAdjacence() const
{
    std::vector<std::vector<int>> matAdj(getNbNoeuds(), std::vector<int>(getNbNoeuds(), 0));
    for (int idNoeud = 0; idNoeud < getNbNoeuds(); idNoeud++)
    {
        for (int idArete : voisins[idNoeud])
        {
            int prochainNoeud = aretes[idArete].getAutreExtremite(idNoeud);
            matAdj[idNoeud][prochainNoeud] = 1;
        }
    }
    return matAdj;
}

int Graphe::getNbEmplacements() const
{
    return emplacements.size();
}

std::vector<Emplacement> Graphe::getEmplacements() const
{
    return emplacements;
}

Emplacement Graphe::getEmplacement(int idEmp) const
{
    return emplacements[idEmp];
}

const Grille& Graphe::getGrille() const
{
    return *grille;
}

int Graphe::getHauteurGrille() const
{
    return hauteurGrille;
}

int Graphe::getLargeurGrille() const
{
    return largeurGrille;
}

bool Graphe::isCoordReel() const
{
    return coordReel;
}

bool Graphe::hasGrid() const
{
    return grille != nullptr;
}

void Graphe::setHauteurGrille(int hauteur)
{
    hauteurGrille = hauteur;
}

void Graphe::setLargeurGrille(int largeur)
{
    largeurGrille = largeur;
}

Position<double> Graphe::getXYmin() const{
    double x = noeuds[0].getPos().getX();
    double y = noeuds[0].getPos().getY();
    for(int i=1; i<getNbNoeuds(); i++){
        if(noeuds[i].getPos().getY() < y){
            y = noeuds[i].getPos().getY(); 
        }
        if(noeuds[i].getPos().getX() < x){
            x = noeuds[i].getPos().getX(); 
        }
    }
    for(int i=0; i<getNbEmplacements(); i++){
        if(emplacements[i].getPos().getY() < y){
            y = emplacements[i].getPos().getY();
        }
        if(emplacements[i].getPos().getX() < x){
            x = emplacements[i].getPos().getX();
        }
    }
    return Position(x,y);
}
Position<double> Graphe::getXYmax() const{
    double x = noeuds[0].getPos().getX();
    double y = noeuds[0].getPos().getY();
    for(int i=1; i<getNbNoeuds(); i++){
        if(noeuds[i].getPos().getY() > y){
            y = noeuds[i].getPos().getY();
        }
        if(noeuds[i].getPos().getX() > x){
            x = noeuds[i].getPos().getX();
        }
    }
    for(int i=0; i<getNbEmplacements(); i++){
        if(emplacements[i].getPos().getY() > y){
            y = emplacements[i].getPos().getY();
        }
        if(emplacements[i].getPos().getX() > x){
            x = emplacements[i].getPos().getX();
        }
    }
    return Position(x,y);
}

void Graphe::ajouteNoeud(const Position<double> &pos)
{
    noeuds.push_back(Noeud(noeuds.size(), pos));
    voisins.push_back({});
    /*
    if (hasGrid())
    {
        grille->ajouteNoeud(noeuds.back().getPos());
    }
    */
}

void Graphe::ajouteArete(int noeudDep, int noeudArr)
{
    Arete arete{nbAretes, noeudDep, noeudArr, noeuds[noeudDep].getPos(), noeuds[noeudArr].getPos()};
    voisins[noeudDep].push_back(arete.getId());
    voisins[noeudArr].push_back(arete.getId());
    aretes.push_back(arete);
    nbIntersectionsAretes.push_back({0LL, 0LL, 0LL});
    nbAretes++;
    if (hasGrid())
    {
        grille->ajouteArete(noeuds[noeudDep].getPos(), noeuds[noeudArr].getPos());
    }
    _ajouteIntersectionsArete(arete.getId());
}

void Graphe::ajouteEmplacement(const Position<int> &pos)
{
    emplacements.push_back(Emplacement(emplacements.size(), pos));
    /*
    if (hasGrid())
    {
        grille->ajouteEmplacement(emplacements.back().getPos());
    }
    */
}

void Graphe::creeGrille(int nbLignes, int nbColonnes)
{
    if (hasGrid())
    {
        supprimeGrille();
    }
    std::vector<Position<double>> positions;
    for (int idNoeud = 0; idNoeud < noeuds.size(); idNoeud++)
    {
        positions.push_back(noeuds[idNoeud].getPos());
    }
    for (int idEmp = 0; idEmp < emplacements.size(); idEmp++)
    {
        positions.push_back(emplacements[idEmp].getPos());
    }
    int xmin, xmax, ymin, ymax;
    if (positions.empty())
    {
        xmin = xmax = ymin = ymax = 0;
    }
    else
    {
        xmin = (int)floor(positions[0].getX());
        xmax = (int)ceil(positions[0].getX());
        ymin = (int)floor(positions[0].getY());
        ymax = (int)ceil(positions[0].getY());
    }
    for (int iPos = 0; iPos < positions.size(); iPos++)
    {
        xmin = min(xmin, (int)floor(positions[iPos].getX()));
        xmax = max(xmax, (int)ceil(positions[iPos].getX()));
        ymin = min(ymin, (int)floor(positions[iPos].getY()));
        ymax = max(ymax, (int)ceil(positions[iPos].getY()));
    }
    int largTotal = xmax - xmin;
    int hautTotal = ymax - ymin;
    int largCellule = largTotal / nbColonnes + 1;
    int hautCellule = hautTotal / nbLignes + 1;

    grille = new Grille(xmin, ymin, largCellule, hautCellule, nbLignes, nbColonnes);
    //for (const Noeud &n : noeuds) grille->ajouteNoeud(n.getPos());
    for (const Arete &a : aretes) grille->ajouteArete(a.getPosNoeudDep(), a.getPosNoeudArr());
    //for (const Emplacement &e : emplacements) grille->ajouteEmplacement(e.getPos());
}

void Graphe::supprimeGrille()
{
    delete grille;
    grille = nullptr;
}

void Graphe::changeTypeCoord()
{
    coordReel = !coordReel;
    for (int i = 0; i < noeuds.size(); i++)
    {
        noeuds[i].changeTypeCoord();
    }
}

void Graphe::bougeNoeud(int idNoeud, const Position<double> &pos)
{
    for (int i = 0; i < voisins[idNoeud].size(); i++)
    {
        int idArete = voisins[idNoeud][i];
        Arete arete = aretes[idArete];
        _supprimeIntersectionsArete(arete.getId());
        if (arete.getNoeudDep() == idNoeud)
        {
            aretes[arete.getId()].setPosNoeudDep(pos);
            arete.setPosNoeudDep(pos);
        }
        else
        {
            aretes[arete.getId()].setPosNoeudArr(pos);
            arete.setPosNoeudArr(pos);
        }
        if (hasGrid())
        {
            grille->bougeArete(arete.getId(), arete.getPosNoeudDep(), arete.getPosNoeudArr());
        }
        _ajouteIntersectionsArete(arete.getId());
    }
    noeuds[idNoeud].setPos(pos);
}

void Graphe::bougeNoeuds(const std::vector<Position<double>> &posNoeuds)
{
    for (int i = 0; i < getNbNoeuds(); i++)
    {
        noeuds[i].setPos(posNoeuds[i]);
        for (int idArete : voisins[i])
        {
            if (aretes[idArete].getNoeudDep() == i)
            {
                aretes[idArete].setPosNoeudDep(noeuds[i].getPos());
            }
            else
            {
                aretes[idArete].setPosNoeudArr(noeuds[i].getPos());
            }
        }
    }
    if (hasGrid())
    {
        for (int i = 0; i < getNbNoeuds(); i++)
        {
            for (int j = 0; j < voisins[i].size(); j++)
            {
                int idArete = voisins[i][j];
                Arete arete = aretes[idArete];

                grille->bougeArete(arete.getId(), arete.getPosNoeudDep(), arete.getPosNoeudArr());
            }
        }
    }
    _recalcIntersections();
}

void Graphe::placeNoeud(int idNoeud, int idEmp)
{
    int ancienEmp = noeuds[idNoeud].getEmplacement();
    int noeudOccupe = -1;
    if (idEmp != -1) noeudOccupe = emplacements[idEmp].getNoeud();
    if (ancienEmp != -1) //Le noeud est retire de son emplacement
    {
        noeuds[idNoeud].setEmplacement(-1);
        emplacements[ancienEmp].setNoeud(-1);
    }
    if (noeudOccupe != -1) //Switch s'il y a deja un noeud sur l'emplacement
    {
        bougeNoeud(noeudOccupe, noeuds[idNoeud].getPos());
        noeuds[noeudOccupe].setEmplacement(ancienEmp);
        if (ancienEmp != -1)
        {
            emplacements[ancienEmp].setNoeud(noeudOccupe);
        }
    }
    if (idEmp != -1) //Placement du noeud sur le nouvel emplacement
    {
        bougeNoeud(idNoeud, emplacements[idEmp].getPos());
        noeuds[idNoeud].setEmplacement(idEmp);
        emplacements[idEmp].setNoeud(idNoeud);
    }
}

void Graphe::placeNoeuds(const std::vector<int> &posNoeuds)
{
    for (int i = 0; i < getNbNoeuds(); i++)
    {
        if (noeuds[i].estPlace())
        {
            emplacements[noeuds[i].getEmplacement()].setNoeud(-1);
            noeuds[i].setEmplacement(-1);
        }
    }
    for (int i = 0; i < getNbNoeuds(); i++)
    {
        noeuds[i].setEmplacement(posNoeuds[i]);
        if (posNoeuds[i] != -1)
        {
            emplacements[posNoeuds[i]].setNoeud(i);
            noeuds[i].setPos(emplacements[posNoeuds[i]].getPos());
            for (int idArete : voisins[i])
            {
                if (aretes[idArete].getNoeudDep() == i)
                {
                    aretes[idArete].setPosNoeudDep(noeuds[i].getPos());
                }
                else
                {
                    aretes[idArete].setPosNoeudArr(noeuds[i].getPos());
                }
            }
        }
    }
    if (hasGrid())
    {
        for (int i = 0; i < getNbNoeuds(); i++)
        {
            for (int j = 0; j < voisins[i].size(); j++)
            {
                int idArete = voisins[i][j];
                Arete arete = aretes[idArete];

                grille->bougeArete(arete.getId(), arete.getPosNoeudDep(), arete.getPosNoeudArr());
            }
        }
    }
    _recalcIntersections();
}

long long Graphe::calcAmeliorationPlacement(int idNoeud, int idEmp)
{
    int ancienEmp = noeuds[idNoeud].getEmplacement();
    long long ancienScore = scoreTotal;
    placeNoeud(idNoeud, idEmp);
    long long nouveauScore = scoreTotal;
    placeNoeud(idNoeud, ancienEmp);
    return ancienScore - nouveauScore;
}

long long Graphe::getScoreTotal() const
{
    return scoreTotal;
}

std::vector<long long> Graphe::getScoreNoeuds() const
{
    std::vector<long long> scores;
    for (int idNoeud = 0; idNoeud < getNbNoeuds(); idNoeud++)
    {
        scores.push_back(getScoreNoeud(idNoeud));
    }
    return scores;
}

long long Graphe::getScoreNoeud(int idNoeud) const
{
    long long score = 0;
    for (int idArete : voisins[idNoeud])
    {
        NbIntersections nbInter = nbIntersectionsAretes[idArete];
        score += nbInter.nbNormales;
        score += nbInter.nbIllegales * _getPenalite();
        score += nbInter.nbSelf * _getPenalite();
    }
    return score;
}

std::vector<NbIntersections> Graphe::getNbIntersectionsAretes() const
{
    return nbIntersectionsAretes;
}

NbIntersections Graphe::getNbIntersectionsArete(int idArete) const
{
    return nbIntersectionsAretes[idArete];
}

void Graphe::_copieGraphe(const Graphe &G)
{
    noeuds = G.noeuds;
    voisins = G.voisins;
    aretes = G.aretes;
    nbAretes = G.nbAretes;
    emplacements = G.emplacements;
    coordReel = G.coordReel;
    scoreTotal = G.scoreTotal;
    nbIntersectionsAretes = G.nbIntersectionsAretes;
    hauteurGrille = G.hauteurGrille;
    largeurGrille = G.largeurGrille;
    if (G.grille != nullptr) grille = new Grille(*G.grille);
    else grille = nullptr;
}

void Graphe::_ajouteIntersectionsArete(int idArete)
{
    _miseAJourIntersectionsArete(idArete, 1);
}

void Graphe::_supprimeIntersectionsArete(int idArete)
{
    _miseAJourIntersectionsArete(idArete, -1);
}

void Graphe::_miseAJourIntersectionsArete(int idArete, int coef)
{
    _getAretesPotentielles(idArete, _listeAretesPotentielles);
    for (int id : _listeAretesPotentielles)
    {
        _miseAJourIntersectionsArete(idArete, id, coef);
    }
}

void Graphe::_miseAJourIntersectionsArete(int idArete1, int idArete2, int coef)
{
    int typeInter = _getTypeIntersection(idArete1, idArete2);
    long long scoreDiff;
    switch (typeInter)
    {
        case 0:
            scoreDiff = coef;
            nbIntersectionsAretes[idArete1].nbNormales += coef;
            nbIntersectionsAretes[idArete2].nbNormales += coef;
            break;
        case 1:
            scoreDiff = _getPenalite() * coef;
            nbIntersectionsAretes[idArete1].nbIllegales += coef;
            nbIntersectionsAretes[idArete2].nbIllegales += coef;
            break;
        case 2:
            scoreDiff = _getPenalite() * coef;
            nbIntersectionsAretes[idArete1].nbSelf += coef;
            nbIntersectionsAretes[idArete2].nbSelf += coef;
            break;
        default:
            scoreDiff = 0LL;
            break;
    }
    scoreTotal += scoreDiff;
}

void Graphe::_getAretesPotentielles(int idArete, std::vector<int> &listeAretes)
{
    if (hasGrid())
    {
        grille->aretesAvecIntersectionPotentielles(idArete, listeAretes);
    }
    else
    {
        _getAretesPotentiellesSansGrille(idArete, listeAretes);
    }
}

void Graphe::_getAretesPotentiellesSansGrille(int idArete, std::vector<int> &listeAretes)
{
    listeAretes.clear();
    listeAretes.reserve(nbAretes);
    for (int i = 0; i < idArete; i++) listeAretes.push_back(i);
    for (int i = idArete+1; i < nbAretes; i++) listeAretes.push_back(i);
}

int Graphe::_getTypeIntersection(int id1, int id2) const
{
    Position<double> dep1 = aretes[id1].getPosNoeudDep();
    Position<double> arr1 = aretes[id1].getPosNoeudArr();
    Position<double> dep2 = aretes[id2].getPosNoeudDep();
    Position<double> arr2 = aretes[id2].getPosNoeudArr();
    int type;
    if (isCoordReel())
    {
        type = _getTypeIntersection<double>(dep1, arr1, dep2, arr2);
    }
    else
    {
        type = _getTypeIntersection<long long>(dep1, arr1, dep2, arr2);
    }
    if (type == 1)
    {
        //Test intersection self
        if (aretes[id1].contient(aretes[id2].getNoeudDep()) || aretes[id1].contient(aretes[id2].getNoeudArr()))
        {
            type = 2;
        }
    }
    return type;
}

long long Graphe::_getPenalite() const
{
    return (long long)noeuds.size();
}

void Graphe::_recalcIntersections()
{
    scoreTotal = 0;
    nbIntersectionsAretes.clear();
    nbIntersectionsAretes.resize(getNbAretes(), {0, 0, 0});
    for (int id1 = 0; id1 < getNbAretes(); id1++)
    {
        for (int id2 = id1+1; id2 < getNbAretes(); id2++)
        {
            _miseAJourIntersectionsArete(id1, id2, 1);
        }
    }
}