#ifndef SELECTEUREMPLACEMENTMULTIPLE_H
#define SELECTEUREMPLACEMENTMULTIPLE_H

#include "selecteur_emplacement.h"

class SelecteurEmplacementMultiple : public SelecteurEmplacement{
public:
    //n est le nombre d'emplacement a tester pour la selection
    //delai est le nombre de selection a faire avant d'incrementer n
    //incr est l'increment de n
    SelecteurEmplacementMultiple(int n = 3, int delai = 100000, int incr = 1);
    //Selectionne l'emplacement le plus proche du noeud parmis n emplacements aléatoires
    int selectionneEmplacement(const Graphe &G, int nodeId) override;
    SelecteurEmplacementMultiple* clone() const override;
    int getN() const;
    int getDelaiIncrement() const;
    int getIncrement() const;
    int getNbAppels() const;
    void setN(int n);
    void setDelaiIncrement(int delai);
    void setIncrement(int incr);
private:
    int n;
    int delaiIncrement;
    int increment;
    int nbAppels; //Nombre d'appels de la fonction de selection (modulo delai)
};

#endif