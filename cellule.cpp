#include "cellule.h"
#include "utilitaire.h"

Cellule::Cellule(int p_id, const Position<int> &HG, const Position<int> &BD): 
    id{p_id},
    hautGauche{HG},
    basDroite{BD}
{}

int Cellule::getId() const{
    return id;
}   
/*
int Cellule::getNbNoeuds() const{
    return (int)noeuds.size();
}
std::vector<int> Cellule::getNoeuds() const{
    return noeuds;
}    
bool Cellule::contientNoeud(int idNoeud) const{
    return contientTab(noeuds, idNoeud);
}
*/
int Cellule::getNbAretes() const{
    return (int)aretes.size();
}

const std::vector<int> &Cellule::getAretes() const{
    return aretes;
}

bool Cellule::contientArete(int idArete) const{
    if (idArete < 0 || idArete >= indiceAretes.size()) return false;
    return indiceAretes[idArete] != -1;
}
/*
int Cellule::getNbEmplacements() const{
    return (int)emplacements.size();
}
std::vector<int> Cellule::getEmplacements() const{
    return emplacements;
}
bool Cellule::contientEmplacement(int idEmp) const{
    return contientTab(emplacements, idEmp);
}
*/
int Cellule::getXmin() const{
    return hautGauche.getX();
}
int Cellule::getXmax() const{
    return basDroite.getX();
}
int Cellule::getYmin() const{
    return basDroite.getY();
}
int Cellule::getYmax() const{
    return hautGauche.getY();
}
/*
void Cellule::setNoeuds(const std::vector<int> &p_noeuds){
    noeuds = p_noeuds;
}
*/
void Cellule::setAretes(const std::vector<int> &p_aretes){
    for (int idArete : aretes) retireArete(idArete);
    for (int idArete : p_aretes) ajouteArete(idArete);
}
/*
void Cellule::setEmplacements(const std::vector<int> &p_emp){
    emplacements = p_emp;
}

bool Cellule::ajouteNoeud(int idNoeud){
    noeuds.push_back(idNoeud);
    return true;
}
bool Cellule::retireNoeud(int idNoeud){
    return retireTab(noeuds, idNoeud);
}
*/
bool Cellule::ajouteArete(int idArete){
    if (indiceAretes.size() <= idArete)
    {
        indiceAretes.resize(idArete+1, -1);
    }
    if (indiceAretes[idArete] != -1) return false;
    indiceAretes[idArete] = (int)aretes.size();
    aretes.push_back(idArete);
    return true;
}

bool Cellule::retireArete(int idArete){
    if (!contientArete(idArete)) return false;
    int derniereArete = aretes.back();
    indiceAretes[derniereArete] = indiceAretes[idArete];
    aretes[indiceAretes[idArete]] = derniereArete;
    aretes.pop_back();
    indiceAretes[idArete] = -1;
    return true;
}
/*
bool Cellule::ajouteEmplacement(int idEmp){
    emplacements.push_back(idEmp);
    return true;
}
bool Cellule::retireEmplacement(int idEmp){
    return retireTab(emplacements, idEmp);
}
*/
void Cellule::reinitialisation(){
    //noeuds.clear();
    //emplacements.clear();
    indiceAretes.clear();
    aretes.clear();
}