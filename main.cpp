#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include "windows.h"
  //si bug à l'inclusion de windows.h --> enlever le using namespace std et remplacer partout par std::
#endif

#if defined(_unix) || defined(unix) || defined(_UNIX_) || defined(UNIX)
  #define UNIX
  #include "termios.h"
  #include "unistd.h"
#endif

#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include <mutex>
#include "position.h"
#include "noeud.h"
#include "arete.h"
#include "emplacement.h"
#include "cellule.h"
#include "display_openGL.h"
#include "grille.h"
#include "graphe.h"
#include "manipulateur_json.h"
#include "placement_aleatoire.h"
#include "placement_fmme.h"
#include "placement_plus_proche.h"
#include "placement_ordonne.h"
#include "placement_noeud_par_noeud.h"
#include "glouton_meilleur_deplacement.h"
#include "recuit_simule.h"
#include "recuit_simule_dynamique.h"
#include "recuit_simule_depart_dynamique.h"
#include "utilitaire.h"
#include "selecteur_noeud_aleatoire.h"
#include "selecteur_noeud_binaire.h"
#include "selecteur_noeud_multiple.h"
#include "selecteur_emplacement_aleatoire.h"
#include "selecteur_emplacement_binaire.h"
#include "selecteur_emplacement_multiple.h"
#include "statistiques.h"
#include "trainer.h"
#include "algos_graphe.h"

#include "display_openGL.h"
#include "textRendering.h"
using namespace std;
using namespace NeuralNetwork;

const string CHEMIN = "/media/ghambari/Transcend/Graph_Contest_2024";
//"D:/Graph_Contest_2024";
//"/media/ghambari/Transcend/Graph_Contest_2024";

mutex ioMutex;
mutex csvMutex;

void affiche_graphe_console(const Graphe &G, bool debugInterAretes = false)
{
    cout << "Liste des noeuds : ";
    for (Noeud n : G.getNoeuds()) cout << n.getId() << "," << n.getPos().getX() << "," << n.getPos().getY() << "," << n.getEmplacement() << "|";
    cout << "\n";
    cout << "Liste des emplacements : ";
    for (Emplacement e : G.getEmplacements()) cout << e.getId() << "," << e.getPos().getX() << "," << e.getPos().getY() << "," << e.getNoeud() << "|";
    cout << "\n";
    cout << "Score total : " << G.getScoreTotal() << "\n";
    cout << "Score noeuds : ";
    for (long long v : G.getScoreNoeuds()) cout << v << " ";
    cout << "\n";
    if (debugInterAretes)
    {
        cout << "Intersections aretes :\n";
        for (NbIntersections nb : G.getNbIntersectionsAretes()) cout << nb.nbNormales << "," << nb.nbIllegales << "," << nb.nbSelf << "\n";
    }
}

void test_placement(Graphe G, int id, int numG)
{
    PlacementNoeudParNoeud placeurNoeuds;
    PlacementOrdonne placeurOrdonne;
    PlacementPlusProche placeurProche;
    PlacementFmme placeur(placeurOrdonne, 10, 1, 1, 1, 10);

    std::vector<int> places = placeur.calcPositionNoeuds(G);
    G.placeNoeuds(places);
    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nGRAPHE NUMERO " << numG << "\n";
        cout << "PLACEUR ORDONNE\n";
        cout << "Score total : " << G.getScoreTotal() << "\n";
    }
}

void test_recuit_simule(Graphe G, int id, int numG)
{
    std::vector<double> cools = {-1, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.99, 0.9999, 0.999, -1, 0.999, 0.9999, 0.9999};
    std::vector<std::vector<ValIter<double>>> stats;
    PlacementNoeudParNoeud placeurFinal;
    PlacementFmme placeur(placeurFinal, 1, 1, 1, 1, 100);
    PlacementAleatoire melangeur;

    double tDep = 100.0, coolTdep = 0.99, cool = -1, seuil = tDep / 1e6;
    if (G.getNbNoeuds() <= 30) cool = 0.9999945;
    else if (G.getNbNoeuds() <= 100) cool = 0.999999;
    else cool = 0.99999945;
    int nbRec = 10, maxNbRec = 1000;
    long long int limiteTps = 3600000;
    SelecteurNoeudAleatoire selectN;
    SelecteurEmplacementMultiple selectE;
    RecuitSimule heuristique(tDep, coolTdep, cool, seuil, nbRec, maxNbRec, selectN, selectE, limiteTps);

    std::vector<int> places = placeur.calcPositionNoeuds(G);
    G.placeNoeuds(places);

    int nbCellules = 1.5 * sqrt(G.getNbAretes());
    G.creeGrille(nbCellules, nbCellules);

    long long scoreInitial = G.getScoreTotal();

    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nLANCEMENT GRAPHE " << numG << " (thread " << this_thread::get_id() << ")\n";
        cout << "Score initial : " << scoreInitial << "\n";
    }

    long long int tpsDep = getMS();
    heuristique.execute(G);
    long long int tpsFin = getMS();

    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nGRAPHE NUMERO : " << numG << "\n";
        cout << "Score initial : " << scoreInitial << "\n";
        cout << "Score final : " << G.getScoreTotal() << "\n";
        cout << "Temps d'execution : " << tpsFin - tpsDep << "ms\n";
    }

    stats.push_back(heuristique.getMeilleursScores());
    stats.push_back(heuristique.getEvolScore());
    stats.push_back(heuristique.getEvolTemp());
    stats.push_back(heuristique.getEvolImprove());
    stats.push_back(heuristique.getEvolProba());

    {
        lock_guard<mutex> lock(csvMutex);
        genere_csv_valiter(stats, CHEMIN + "/inputs/2023/stats-graphe-" + to_string(numG) + ".csv");
        intervertir_lignes_colonnes_csv(CHEMIN + "/inputs/2023/stats-graphe-" + to_string(numG) + ".csv");
        ManipulateurJson manip;
        manip.ecrireGrapheDansJson(CHEMIN + "/inputs/2023/solution-graphe-" + to_string(numG) + ".json", G);
    }
}

void test_recuit_simule_dynamique(Graphe G, int id, int numG)
{
    std::vector<int> maxIters = {-1, 100000, 100000, 100000, 100000, 100000, 100000, 100000, 5000, 100000, 100000, -1, 100000, 100000, 100000};
    std::vector<std::vector<ValIter<double>>> stats;
    PlacementOrdonne placeurFinal;
    PlacementFmme placeur(placeurFinal, 10, 1, 1, 1, 10);
    PlacementAleatoire melangeur;

    std::vector<int> places = placeur.calcPositionNoeuds(G);
    G.placeNoeuds(places);
    int nbCellules = 1.5 * sqrt(G.getNbAretes());
    G.creeGrille(nbCellules, nbCellules);

    int maxIter = 300000000, nbSimTempDep = 1000, nbMaxRec = 20;
    if (numG == 8) nbSimTempDep = 100;
    if (numG < 8) maxIter = 100000;
    double pDep = 0.3, Ka = 1.0, coolDep = 0.99;
    long long int limiteTps = 3600000/nbMaxRec;
    SelecteurNoeudAleatoire selectN;
    SelecteurEmplacementMultiple selectE;
    RecuitSimuleDynamique heuristique(pDep, nbSimTempDep, maxIter, Ka, coolDep, selectN, selectE, limiteTps);

    long long scoreInitial = G.getScoreTotal();
    std::vector<long long> scoreFinaux;

    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nLANCEMENT GRAPHE " << numG << "\n";
        cout << "Score initial : " << scoreInitial << "\n";
    }

    long long int tpsDep = getMS();
    for (int i = 0; i < nbMaxRec; i++)
    {
        {
            lock_guard<mutex> lock(ioMutex);
            cout << "Graphe " << numG << ", execution recuit dynamique numero " << i << "... (" << G.getScoreTotal() << ")\n";
        }

        heuristique.execute(G);
        scoreFinaux.push_back(G.getScoreTotal());
        //dispOpenGL(G);
    }
    long long int tpsFin = getMS();

    stats.push_back(heuristique.getMeilleursScores());
    stats.push_back(heuristique.getEvolScore());
    stats.push_back(heuristique.getEvolImprove());
    stats.push_back(heuristique.getEvolAccepte());
    stats.push_back(heuristique.getEvolTemp());
    stats.push_back(heuristique.getEvolProba());
    stats.push_back(heuristique.getEvolDeltaScore());
    stats.push_back(heuristique.getEvolDeltaEntropie());
    
    {
        lock_guard<mutex> lock(ioMutex);

        cout << "\nGRAPHE NUMERO : " << numG << "\n";
        cout << "Score finaux : ";
        affiche(scoreFinaux);
        cout << "Temperature finale : " << heuristique.getTemp() << "\n";
        cout << "Temps d'execution : " << tpsFin - tpsDep << "ms\n";
    }

    {
        lock_guard<mutex> lock(csvMutex);
        genere_csv_valiter(stats, CHEMIN + "/inputs/2023/stats-graphe-" + to_string(numG) + ".csv");
        intervertir_lignes_colonnes_csv(CHEMIN + "/inputs/2023/stats-graphe-" + to_string(numG) + ".csv");
        ManipulateurJson manip;
        manip.ecrireGrapheDansJson(CHEMIN + "/inputs/2023/solution-graphe-" + to_string(numG) + ".json", G);
    }
}

void test_recuit_simule_depart_dynamique(Graphe G, int id, int numG)
{
    std::vector<double> cools = {-1, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.999, 0.9999, 0.9999, -1, 0.9999, 0.9999, 0.9999};
    std::vector<std::vector<ValIter<double>>> stats;
    PlacementOrdonne placeurFinal;
    PlacementFmme placeur(placeurFinal);
    PlacementAleatoire melangeur;

    double pDep = 0.5, cool = cools[numG], coefSeuil = 1.0/1e6;
    int nbSimTempDep = 1000, nbRec = 10, maxNbRec = 10;
    long long int limiteTps = -1;
    SelecteurNoeudAleatoire selectN;
    SelecteurEmplacementMultiple selectE;
    RecuitSimuleDepartDynamique heuristique(pDep, nbSimTempDep, cool, coefSeuil, nbRec, maxNbRec, selectN, selectE, limiteTps);

    std::vector<int> places = placeur.calcPositionNoeuds(G);
    G.placeNoeuds(places);
    int nbCellules = 1.5 * sqrt(G.getNbAretes());
    G.creeGrille(nbCellules, nbCellules);

    long long scoreInitial = G.getScoreTotal();

    long long int tpsDep = getMS();
    heuristique.execute(G);
    long long int tpsFin = getMS();

    {
        lock_guard<mutex> lock(ioMutex);

        cout << "GRAPHE NUMERO : " << numG << "\n";
        cout << "Score initial : " << scoreInitial << "\n";
        cout << "Score final : " << G.getScoreTotal() << "\n";
        cout << "Temps d'execution : " << tpsFin - tpsDep << "ms\n";
    }

    stats.push_back(heuristique.getEvolScore());
    stats.push_back(heuristique.getEvolTemp()); 

    //genere_csv_valiter(stats, CHEMIN + "/inputs/2023/stats-graphes.csv");
    //intervertir_lignes_colonnes_csv(CHEMIN + "/inputs/2023/stats-graphes.csv");
}

void test_recuit_simule_hybride(Graphe G, int id, int numG)
{
    std::vector<std::vector<ValIter<double>>> stats;
    PlacementOrdonne placeurFinal;
    PlacementFmme placeur(placeurFinal, 10, 1, 1, 1, 10);
    PlacementAleatoire melangeur;

    std::vector<int> places = placeur.calcPositionNoeuds(G);
    G.placeNoeuds(places);
    int nbCellules = 1.5 * sqrt(G.getNbAretes());
    G.creeGrille(nbCellules, nbCellules);

    long long scoreInitial = G.getScoreTotal();
    std::vector<long long> scoreFinaux;
    long long int tpsDep = getMS();

    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nLANCEMENT GRAPHE " << numG << " (thread " << this_thread::get_id() << ")\n";
        cout << "Score initial : " << scoreInitial << "\n";
    }

    double tDep = 100.0, coolTdep = 0.99, cool = -1, seuil = tDep / 1e6;
    if (G.getNbNoeuds() <= 30) cool = 0.9999945;
    else if (G.getNbNoeuds() <= 100) cool = 0.999999;
    else cool = 0.99999945;
    int nbRec = 10, maxNbRec = 1000;
    long long int limiteTps = 1200000;
    SelecteurNoeudAleatoire selectN;
    SelecteurEmplacementMultiple selectE;
    RecuitSimule heuristique1(tDep, coolTdep, cool, seuil, nbRec, maxNbRec, selectN, selectE, limiteTps);
    
    heuristique1.execute(G);



    long long int tpsFin = getMS();

    {
        lock_guard<mutex> lock(ioMutex);
        cout << "\nGRAPHE NUMERO : " << numG << "\n";
        cout << "Score initial : " << scoreInitial << "\n";
        cout << "Score final : " << G.getScoreTotal() << "\n";
        cout << "Temps d'execution : " << tpsFin - tpsDep << "ms\n";
    }
}

void test_global()
{
    std::vector<int> numGraphes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14};
    std::vector<thread> threads;
    for (int i = 0; i < numGraphes.size(); i++)
    {
        int iG = numGraphes[i];
        string fichier = "/inputs/2023/graphe-" + to_string(iG) + ".json";
        Graphe G;
        ManipulateurJson manip;
        if (!manip.lireGrapheDepuisJson(CHEMIN + fichier, G))
        {
            lock_guard<mutex> lock(ioMutex);
            cout << "Probleme dans la lecture du graphe " << iG << "\n";
            return;
        }

        threads.emplace_back(test_recuit_simule_dynamique, G, i, iG);
    }

    for (int i = 0; i < threads.size(); i++)
    {
        threads[i].join();
    }
}

void test_temps(int seed)
{
    for (int i = 5; i <= 5; i++)
    {       
        string fichier = "/inputs/2023/graphe-" + to_string(i) + ".json";
        Graphe G;
        ManipulateurJson manip;
        if (!manip.lireGrapheDepuisJson(CHEMIN + fichier, G))
        {
            cout << "Probleme dans la lecture du graphe " << i << "\n";
            return;
        }

        std::vector<double> cools = {-1, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.9999, 0.99, 0.9999, 0.999, -1, 0.999, 0.9999, 0.9999};
        PlacementAleatoire melangeur;

        double tDep = 100.0, coolTdep = 0.99, cool = -1, seuil = tDep / 1e6;
        if (G.getNbNoeuds() <= 30) cool = 0.9999945;
        else if (G.getNbNoeuds() <= 100) cool = 0.999999;
        else cool = 0.99;
        cool = 0.99;
        int nbRec = 10, maxNbRec = 10;
        long long int limiteTps = 36000000;
        SelecteurNoeudAleatoire selectN;
        SelecteurEmplacementAleatoire selectE;
        RecuitSimule heuristique(tDep, coolTdep, cool, seuil, nbRec, maxNbRec, selectN, selectE, limiteTps);

        std::vector<int> places = melangeur.calcPositionNoeuds(G);
        G.placeNoeuds(places);

        int nbCellules = 1.5 * sqrt(G.getNbAretes());
        G.creeGrille(nbCellules, nbCellules);

        long long scoreInitial = G.getScoreTotal();


        cout << "GRAPHE NUMERO " << i << endl;

        Graphe C(G);
        srand(seed);
        heuristique.execute(C); 
    }
}

std::vector<int> seeds = {2000, 4321, 9999, 10000000, 5, 19, 666666666};
int seed;



int main()
{
    seed = 123; //DEBUG
    //srand(seed);

    /*for(int i = 0; i<1; i++){
        seed = seeds[i];
        cout << "\nseed : " << seed << "\n";
        test_temps(seeds[i]);
    }*/
   
    //displayTextBase();
    test_temps(seed);
}