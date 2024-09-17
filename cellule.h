#ifndef CELLULE_H
#define CELLULE_H

#include <vector>
#include <set>
#include <iostream>
#include "position.h"
using namespace std;

class Cellule {
public:
    Cellule(int id, const Position<int> &HG, const Position<int> &BD);

    int getId() const;
    /*
    int getNbNoeuds() const;
    std::vector<int> getNoeuds() const;
    bool contientNoeud(int idNoeud) const;
    */
    int getNbAretes() const;
    const std::vector<int> &getAretes() const;
    bool contientArete(int idArete) const;
    /*
    int getNbEmplacements() const;
    std::vector<int> getEmplacements() const;
    bool contientEmplacement(int idEmp) const;
    */
    int getXmin() const;
    int getXmax() const;
    int getYmin() const;
    int getYmax() const;
    //void setNoeuds(const std::vector<int> &noeuds);
    void setAretes(const std::vector<int> &aretes);
    //void setEmplacements(const std::vector<int> &emp);
    //bool ajouteNoeud(int idNoeud);
    //bool retireNoeud(int idNoeud);
    bool ajouteArete(int idArete);
    bool retireArete(int idArete);
    //bool ajouteEmplacement(int idEmp);
    //bool retireEmplacement(int idEmp);
    void reinitialisation();
private:
	int id;
    //std::vector<int> noeuds;
    //std::vector<int> emplacements;
    std::vector<int> indiceAretes;
    std::vector<int> aretes;
    Position<int> hautGauche;
    Position<int> basDroite;
};

#endif