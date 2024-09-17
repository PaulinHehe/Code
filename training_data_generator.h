#ifndef TRAINING_DATA_GENERATOR_H_INCLUDED
#define TRAINING_DATA_GENERATOR_H_INCLUDED
#include "neural_network.h"
#include "game_state.h"
#include "game.h"
#include "graphe.h"
#include <vector>
using namespace std;
using namespace NeuralNetwork;

class TrainingDataGenerator
{
public:
    // Cette classe est en charge de la creation de batchs de donnees pour le reseau de neurone qui joue
    // net est le reseau de neurone de jeu
    // initState et l'etat initial du jeu
    // G est le graphe qu'on doit placer
    // crossMed est le nombre de croisements median utilise pour le calcul de la valeur d'une position
    TrainingDataGenerator(const Network &net, const GameState &initState, const Graphe &G, double crossMed);
    
    // Joue des parties
    // nbGames = nombre de parties
    // turnCountMax = nombre de coups par partie (-1 si on joue jusqu'au bout)
    // nbSimulationMCTS = nombre de simulation de coups par le MCTS
    void playGames(int nbGames, int turnCountMax, int nbSimulationMCTS);
    // Supprime toutes les parties jouees
    void resetGameData();
    // Genere un batch de donnees selectionnees au hasard parmi toutes les parties jouees
    DataBatch generateTrainingDataBatch(int batchSize) const;

    // Renvoie le reseau de neurone
    Network getGameNetwork() const;
    // Renvoie l'etat initial du jeu
    GameState getStartGameState() const;
    // Renvoie le graphe avec lequel on joue
    Graphe getGraph() const;
    // Renvoie le nombre de parties jouees
    int getNbGames() const;
    // Renvoie le tableau des parties
    std::vector<Game> getGames() const;
    // Renvoie la partie numero 'game'
    Game getGame(int game) const;
    
    // Remplace le reseau de neurone
    void setGameNetwork(const Network &net);
    // Remplace l'etat inital du jeu
    void setStartGameState(const GameState &initState);
    // Remplace le graphe avec lequel on joue
    void setGraph(const Graphe &G);
private:
    Network gameNetwork;
    GameState startGameState;
    Graphe graph;
    std::vector<Game> games;
    double crossMedian;
};

#endif // TRAINING_DATA_GENERATOR_H_INCLUDED
