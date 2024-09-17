#ifndef GRILLE_H
#define GRILLE_H

#include "cellule.h"
#include "position.h"
#include <vector>
using namespace std;

class Grille{
public:
    Grille(int xmin, int ymin, int largeurColonnes, int hauteurLigne, int nbLignes, int nbColonnes);

    //void ajouteNoeud(const Position<int> & pos);
    void ajouteArete(const Position<int> & posDep, const Position<int> & posArr);
    //void ajouteEmplacement(const Position<int> & pos);
    //void bougeNoeud(int idNoeud, const Position<int> & pos);
    void bougeArete(int idArete, const Position<int> & posDep, const Position<int> & posArr);
    //void bougeEmplacement(int idEmp, const Position<int> & pos);
    void reinitialisation();
    void aretesAvecIntersectionPotentielles(int idArete, std::vector<int> &listeAretes);

    int getXmin() const;
    int getYmin() const;

    int getLageurColonnes() const;
    int getHauteurLignes() const;

    int getNbColonnes() const;
    int getNbLignes() const;

    const Cellule& getCellule(int i, int j)const;
    const std::vector<Cellule> getLigneCellules(int i)const;
    const std::vector<std::vector<Cellule>> getCellules()const;
    
private:
    std::vector<std::vector<Cellule>> cellules;
    //std::vector<int> ligCellNoeud, colCellNoeud;
    std::vector<std::vector<int>> ligCellAretes, colCellAretes;
    //std::vector<int> ligCellEmp, colCellEmp;
    int xmin, ymin;
    int largeurColonnes, hauteurLigne;
    int nbColonnes, nbLignes;
    std::vector<bool> tableMarquage;

    void _calcTraverseLigCol(const Position<int> &posDep, const Position<int> &posArr, std::vector<int> &traverseLig, std::vector<int> &traverseCol) const;
};



#endif