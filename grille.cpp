#include "grille.h"
#include "utilitaire.h"
#include <cmath>
#include <algorithm>
using namespace std;

Grille::Grille(int xmin, int ymin, int largeurColonnes, int hauteurLigne, int nbLignes, int nbColonnes):
    xmin{xmin}, ymin{ymin},
    largeurColonnes{largeurColonnes}, hauteurLigne{hauteurLigne},
    nbLignes{nbLignes}, nbColonnes{nbColonnes}
{   
    cellules = std::vector<std::vector<Cellule>>();
    for(int i=0; i<nbLignes; i++)
    {
        std::vector<Cellule> ligne;
        for(int j=0; j<nbColonnes; j++)
        {
            ligne.push_back(Cellule(i*nbColonnes+j, {j*largeurColonnes + xmin, (i+1)*hauteurLigne + ymin}, {(j+1)*largeurColonnes + xmin, i*hauteurLigne + ymin}));
        }
        cellules.push_back(ligne);
    }
}
/*
void Grille::ajouteNoeud(const Position<int> & pos){
    int id = (int)ligCellNoeud.size();
    int cellX = (pos.getX()-xmin)/largeurColonnes;
    int cellY = (pos.getY()-ymin)/hauteurLigne; 
    colCellNoeud.push_back(cellX);
    ligCellNoeud.push_back(cellY);
    cellules[cellY][cellX].ajouteNoeud(id);   
}
*/
void Grille::ajouteArete(const Position<int> & posDep, const Position<int> & posArr){
    int id = ligCellAretes.size();
    while (tableMarquage.size() <= id)
    {
        tableMarquage.push_back(false);
    }
    ligCellAretes.push_back({});
    colCellAretes.push_back({});
    _calcTraverseLigCol(posDep, posArr, ligCellAretes[id], colCellAretes[id]);
    for (int i = 0; i < ligCellAretes[id].size(); i++)
    {
        cellules[ligCellAretes[id][i]][colCellAretes[id][i]].ajouteArete(id);
    } 
}
/*
void Grille::ajouteEmplacement(const Position<int> & pos){
    int id = ligCellEmp.size();
    int cellX = (pos.getX()-xmin)/largeurColonnes;
    int cellY = (pos.getY()-ymin)/hauteurLigne; 
    colCellEmp.push_back(cellX);
    ligCellEmp.push_back(cellY);
    cellules[cellY][cellX].ajouteEmplacement(id);   
}

void Grille::bougeNoeud(int idNoeud, const Position<int> & pos){
    cellules[ligCellNoeud[idNoeud]][colCellNoeud[idNoeud]].retireNoeud(idNoeud);
    int cellX = (pos.getX()-xmin)/largeurColonnes;
    int cellY = (pos.getY()-ymin)/hauteurLigne; 
    colCellNoeud[idNoeud] = cellX;
    ligCellNoeud[idNoeud] = cellY;
    cellules[cellY][cellX].ajouteNoeud(idNoeud);
}
*/
void Grille::bougeArete(int idArete, const Position<int> & posDep, const Position<int> & posArr)
{
    for(int i=0; i<ligCellAretes[idArete].size(); i++)
    {
        cellules[ligCellAretes[idArete][i]][colCellAretes[idArete][i]].retireArete(idArete);
    }
    _calcTraverseLigCol(posDep, posArr, ligCellAretes[idArete], colCellAretes[idArete]);
    for (int i = 0; i < ligCellAretes[idArete].size(); i++)
    {
        cellules[ligCellAretes[idArete][i]][colCellAretes[idArete][i]].ajouteArete(idArete);
    }  
}
/*
void Grille::bougeEmplacement(int idEmp, const Position<int> & pos){
    cellules[ligCellEmp[idEmp]][colCellEmp[idEmp]].retireEmplacement(idEmp);
    int cellX = (pos.getX()-xmin)/largeurColonnes;
    int cellY = (pos.getY()-ymin)/hauteurLigne; 
    colCellEmp[idEmp] = cellX;
    ligCellEmp[idEmp] = cellY;
    cellules[cellY][cellX].ajouteEmplacement(idEmp);
}
*/
void Grille::reinitialisation(){
    //ligCellNoeud.resize(0);
    //colCellNoeud.resize(0);
    ligCellAretes.resize(0);
    colCellAretes.resize(0);
    //ligCellEmp.resize(0);
    //colCellEmp.resize(0);
    tableMarquage.clear();
}

void Grille::aretesAvecIntersectionPotentielles(int idArete, std::vector<int> &listeAretes)
{
    listeAretes.clear();
    listeAretes.reserve(tableMarquage.size());
    for (int i=0; i<ligCellAretes[idArete].size(); i++)
    {
        int cellY = ligCellAretes[idArete][i];
        int cellX = colCellAretes[idArete][i];
        const std::vector<int> &aretes = cellules[cellY][cellX].getAretes();
        for (int id : aretes)
        {
            if (!tableMarquage[id] && id != idArete)
            {
                listeAretes.push_back(id);
                tableMarquage[id] = true;
            }
        }
    }
    for (int i : listeAretes)
    {
        tableMarquage[i] = false;
    }
}

void Grille::_calcTraverseLigCol(const Position<int> &posDep, const Position<int> &posArr, std::vector<int> &traverseLig, std::vector<int> &traverseCol) const
{
    int cellXdep = (posDep.getX()-xmin)/largeurColonnes;
    int cellYdep = (posDep.getY()-ymin)/hauteurLigne;
    int cellXarr = (posArr.getX()-xmin)/largeurColonnes;
    int cellYarr = (posArr.getY()-ymin)/hauteurLigne;
    long long diffX = posArr.getX() - posDep.getX();
    long long diffY = posArr.getY() - posDep.getY();
    long long coefX = 1; //Direction dans laquelle on voyage
    long long coefY = 1; //Direction dans laquelle on voyage
    if (diffX < 0)
    {
        coefX = -1;
        diffX = -diffX;
    }
    if (diffY < 0)
    {
        coefY = -1;
        diffY = -diffY;
    }
    long long resteX = cellules[cellYdep][cellXdep].getXmax() - posDep.getX(); //Ecart relatif au bord haut droit de la cellule
    long long resteY = cellules[cellYdep][cellXdep].getYmax() - posDep.getY(); //Ecart relatif au bord haut droit de la cellule
    if (coefX == -1) resteX = largeurColonnes - resteX; //Si on va a gauche
    if (coefY == -1) resteY = hauteurLigne - resteY; //Si on va en bas
    long long ecartX = resteX * diffY; //Ecart relatif pondere par la pente
    long long ecartY = resteY * diffX; //Ecart relatif pondere par la pente
    
    long long cellX = cellXdep, cellY = cellYdep;
    traverseLig.clear();
    traverseCol.clear();
    traverseLig.push_back(cellY);
    traverseCol.push_back(cellX);
    while (cellX != cellXarr || cellY != cellYarr)
    {
        if (ecartX < ecartY || (ecartX == ecartY && coefY < 0)) //On touche le bord en X en premier (priorite : le plus haut puis le plus a droite)
        {
            cellX += coefX; //On avance ou recule d'une colonne
            ecartY -= ecartX; //Mise a jour de l'ecart en y
            ecartX = largeurColonnes * diffY; //Reinitialisation de l'ecart en x
        }
        else //On touche le bord en Y en premier
        {
            cellY += coefY; //On avance ou recule d'une ligne
            ecartX -= ecartY; //Mise a jour de l'ecart en x
            ecartY = hauteurLigne * diffX; //Reinitialisation de l'ecart en y
        }
        traverseLig.push_back(cellY);
        traverseCol.push_back(cellX);
    }
}

int Grille::getXmin() const{
    return xmin;
}
int Grille::getYmin() const{
    return ymin;
}

int Grille::getLageurColonnes()const{
    return largeurColonnes;
}
int Grille::getHauteurLignes() const{
    return hauteurLigne;
}

int Grille::getNbColonnes() const{
    return nbColonnes;
}
int Grille::getNbLignes() const{
    return nbLignes;
}

const Cellule& Grille::getCellule(int i, int j)const{
    return cellules[i][j];
}
const std::vector<Cellule> Grille::getLigneCellules(int i)const{
    return cellules[i];
}
const std::vector<std::vector<Cellule>> Grille::getCellules()const{
    return cellules;
}