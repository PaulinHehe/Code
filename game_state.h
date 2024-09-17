#ifndef GAME_STATE_H_INCLUDED
#define GAME_STATE_H_INCLUDED
#include "matrix.h"
#include "position.h"
#include <vector>
using namespace std;
using namespace NeuralNetwork;

class GameState
{
public:
    // gAtt : Neurones contenant les caracteristiques du graphe
    // xS : abscisses des emplacements
    // yS : ordonnees des emplacements
    // slotD : contient le noeud contenu par chaque emplacement
    // nodeD : contient l'emplacement sur lequel un noeud est place
    GameState(const Matrix<double> &gAtt, const std::vector<Position<int>> &posS, const std::vector<int> &slotD, const std::vector<int> &nodeD);
    // Initialise xMin, xMax, yMin, yMax, nextNode
    void init();

    // Renvoie la matrice d'entree qui correspond a l'etat du jeu
    // Utilisee pour le reseau de neurone
    Matrix<double> getAsInputMatrix() const;
    // Renvoie le nombre d'emplacement
    int getNbSlots() const;
    // Renvoie le nombre de noeuds
    int getNbNodes() const;
    // Renvoie l'indice du premier noeud non place
    // NbNodes si tous les noeuds sont places
    int getNextNode() const;
    // Renvoie les donnees sur les emplacements
    std::vector<int> getSlotData() const;
    // Renvoie les donnees sur les noeuds
    std::vector<int> getNodeData() const;

    // Renvoie vrai si tous les noeuds sont places
    bool isGameFinished() const;
    // Renvoie vrai si l'emplacement slot est disponible
    bool isSlotAvailable(int slot) const;

    // Place le prochain noeud non place sur l'emplacement slot
    bool placeNode(int slot);
    // Retire le dernier noeud place de son emplacement
    bool removeNodeFromSlot();
private:
    Matrix<double> graphAttributes;
    std::vector<Position<int>> slotsPos;
    // Extremum des coordonnees des emplacements
    int xMin, xMax, yMin, yMax;
    std::vector<int> slotData;
    std::vector<int> nodeData;
    // Indice du premier noeud non place
    int nextNode;

    // Conversion des coordonnees entieres en coordonnees flottantes
    // Les coordonnees de sorties sont dans l'intervalle [0, 1] x [0, 1]
    void _convertCoordDouble(const Position<int> posInt, Position<double>& posDouble) const;
};

#endif // GAME_STATE_H_INCLUDED
