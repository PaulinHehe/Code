#include "selecteur_emplacement_multiple.h"

SelecteurEmplacementMultiple::SelecteurEmplacementMultiple(int n, int delai, int incr):
    n{n}, delaiIncrement{delai}, increment{incr}, nbAppels{0}
{}

//Selectionne l'emplacement le plus proche du noeud parmis n emplacements aléatoires
int SelecteurEmplacementMultiple::selectionneEmplacement(const Graphe &G, int nodeId){

    int randomEmpId = generateRand((int)G.getNbEmplacements());
    // on retire si on pioche le meme emplacement
    while (G.getNoeud(nodeId).getEmplacement() == randomEmpId)
    {
        randomEmpId = generateRand((int)G.getNbEmplacements());
    }
    int dist;
    if (n > 1)
    {
        dist = distanceCarre<int>(G.getNoeud(nodeId).getPos(), G.getEmplacement(randomEmpId).getPos()); 
    } 
    for (int i = 1; i < n; i++)
    {
        int randomEmpId2 = generateRand((int)G.getNbEmplacements()); // Selection aléatoire d'un emplacement disponible (pas tres équiprobable)
        // on retire si on pioche le meme emplacement
        while ((G.getNoeud(nodeId).getEmplacement() == randomEmpId2)||(randomEmpId2 == randomEmpId))
        {
            randomEmpId2 = generateRand((int)G.getNbEmplacements());
        }
        int dist2 = distanceCarre<int>(G.getNoeud(nodeId).getPos(), G.getEmplacement(randomEmpId2).getPos());
        if (dist2 < dist) {
            randomEmpId = randomEmpId2;
            dist = dist2;
        }
    }
    nbAppels++;
    while (nbAppels >= delaiIncrement)
    {
        nbAppels -= delaiIncrement;
        n += increment;
    }
    return randomEmpId;
}

SelecteurEmplacementMultiple* SelecteurEmplacementMultiple::clone() const{
    return new SelecteurEmplacementMultiple(*this);
}

int SelecteurEmplacementMultiple::getN() const{
    return n;
}

int SelecteurEmplacementMultiple::getDelaiIncrement() const
{
    return delaiIncrement;
}

int SelecteurEmplacementMultiple::getIncrement() const
{
    return increment;
}

int SelecteurEmplacementMultiple::getNbAppels() const
{
    return nbAppels;
}

void SelecteurEmplacementMultiple::setN(int p_n){
    n = p_n;
}

void SelecteurEmplacementMultiple::setDelaiIncrement(int delai)
{
    delaiIncrement = delai;
}

void SelecteurEmplacementMultiple::setIncrement(int incr)
{
    increment = incr;
}