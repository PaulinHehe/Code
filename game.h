#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include "game_state.h"
#include "mcts.h"
#include "neural_network.h"
#include "matrix.h"
#include "graphe.h"

class Game
{
public:
    // net est le reseau de neurone utilise pour la partie
    // initState est l'etat du jeu initial
    Game(const Network &net, const GameState &initState, const Graphe &G, double crossMed);

    // Execute une partie
    // La partie est limite a turnCountMax coups, illimite si turnCountMax = -1
    // nbSimulationMCTS est le nombre de simulation du MCTS a effectuer a chaque coup
    void executeGame(int turnCountMax = -1, int nbSimulationMCTS = 20);

    // Renvoie le nombre de coups qui ont ete joues
    int getNbTurns() const;
    // Renvoie l'etat du jeu avant le coup numero turn (indexe a partir de 0)
    GameState getGameState(int turn) const;
    // Renvoie les probabilites pour chaque emplacement qui ont ete utilisees pour le coup numero turn (indexe a partir de 0)
    Matrix<double> getTurnResult(int turn) const;
    // A appeler APRES avoir execute la partie
    // Renvoie l'etat du jeu final (le dernier calcule)
    GameState getFinalGameState() const;
    // Renvoie l'etat initial du jeu
    GameState getStartGameState() const;
    // Renvoie le score de la partie
    long long getNbCrossings() const;
private:
    Network network;
    std::vector<GameState> gameStates;
    std::vector<Matrix<double>> turnResults;
    GameState startGameState;
    GameState currentGameState;
    Graphe graph;
    double crossMedian;
};

#endif // GAME_H_INCLUDED
