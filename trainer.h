#ifndef TRAINER_H_INCLUDED
#define TRAINER_H_INCLUDED
#include "network_alpha_go_zero.h"
#include "training_data_generator.h"
#include "softmax_layer_alpha_go_zero.h"
#include "graphe.h"
#include "matrix.h"
using namespace NeuralNetwork;
using namespace std;

class Trainer
{
public:
    // net est le reseau de neurone
    // G est le graphe sur lequel il on veut entrainer le reseau de neurone
    // batchS est la taille des batchs d'entrainement
    // batchC est le coefficient de descente
    // nbG est le nombre de parties par batch
    // turnCM est le nombre de coups maximum par partie (-1 si on joue tous les coups)
    // nbSimMCTS est le nombre de simulation de coup par MCTS
    // crossMed est le nombre de croisements median qui va etre utilise pour le calcul du score (en general un recuit avec tres faible cool)
    Trainer(const NetworkAlphaGoZero &net, const Graphe &G, int batchS, double batchC, int nbG, int turnCM, int nbSimMCTS, double crossMed);

    // Entraine le reseau de neurone complet sur un certain nombre de batchs
    void trainNetwork(int nbBatches);
    // Entraine le reseau de jeu uniquement sur un certain nombre de batchs
    void trainGameNetworkOnly(int nbBatches);
    // THE FINAL AND ULTIMATE GAME
    // Renvoie le placement obtenu lors de l'execution d'une partie
    std::vector<int> playGame();

    // Getters
    NetworkAlphaGoZero getNetwork() const;
    TrainingDataGenerator getTrainingDataGenerator() const;
    Graphe getGraph() const;

    int getBatchSize() const;
    double getBatchCoef() const;
    int getNbGames() const;
    int getTurnCountMax() const;
    int getNbSimulationMCTS() const;
    GameState getStartGameState() const;

    // Renvoie la taille de crossEvolution
    // Attention, l'etat initial est compte dedans
    int getNbEvolutions() const;
    std::vector<double> getCrossEvolution() const;
    double getCross(int iBatch) const;

    // Setters
    void setNetwork(const NetworkAlphaGoZero &net);
    void setGraph(const Graphe &G);
    void setBatchSize(int batchS);
    void setBatchCoef(double batchC);
    void setNbGames(int nbG);
    void setTurnCountMax(int turnCM);
    void setNbSimulationMCTS(int nbSimMCTS);
private:
    NetworkAlphaGoZero network;
    TrainingDataGenerator dataGenerator;
    Graphe graph;
    int batchSize;
    double batchCoef;
    int nbGames;
    int turnCountMax;
    int nbSimulationMCTS;
    double crossMedian;
    std::vector<double> crossEvolution; 
};

// Fonction de cout pour le reseau de neurone
double AlphaGoZeroCost(const Matrix<double> &output, const Matrix<double> &Y);
Matrix<double> AlphaGoZeroCostD(const Matrix<double> &output, const Matrix<double> &Y);
const CostFunction AlphaGoZeroCostF = {AlphaGoZeroCost, AlphaGoZeroCostD, "Alpha Go Zero Cost"};

#endif //TRAINER_H_INCLUDED