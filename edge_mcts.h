#ifndef EDGE_MCTS_H_INCLUDED
#define EDGE_MCTS_H_INCLUDED
#include <iostream>
using namespace std;

class EdgeMCTS
{
public:
    // EdgeMCTS represente un coup de jeu (placement d'un noeud sur un emplacement)
    // nodeT : noeud du graphe place pendant le coup
    // slotT : emplacement rempli pendant le coup
    // nextNode : noeud auquel l'arete est reliee dans l'arbre du MCTS
    // P : probabilite du coup donnee par le reseau de neurone
    EdgeMCTS(int nodeT, int slotT, int nextNode, double P);

    double getPvalue() const;
    int getNvalue() const;
    double getVsumValue() const;
    double getQvalue() const;
    double getUvalue() const;
    double getEdgeValue() const;
    int getNodeTransition() const;
    int getSlotTransition() const;
    int getNextNodeMCTS() const;
    // Ajoute la valeur obtenue pendant le passage de la simulation du MCTS a l'arete
    void addVvalue(double V);
private:
    // Probabilite du coup donnee par le reseau de neurone
    double Pvalue;
    // Nombre de passage par cette arete pendant les simulations du MCTS
    int Nvalue;
    // Somme des valeurs obtenues dans les sous-arbres de cette arete lors des simulations du MCTS
    double VsumValue;
    // Noeud du graphe place pendant le coup
    int nodeTransition;
    // Emplacement rempli pendant le coup
    int slotTransition;
    // Noeud auquel l'arete est reliee dans l'arbre du MCTS
    int nextNodeMCTS;
};

ostream& operator<<(ostream& ost, const EdgeMCTS &edge);

#endif // EDGE_MCTS_H_INCLUDED
