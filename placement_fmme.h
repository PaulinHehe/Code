#ifndef INCLUDED_PLACEMENT_FMME_H
#define INCLUDED_PLACEMENT_FMME_H
#include "algos_placement.h"

class PlacementFmme : public AlgosPlacement
{
public:
    const double PI = 3.141592653589793;

    string fichierStatsTrans;
    string fichierStatsRot;

    struct Transformation
    {
        double angle, depX, depY, zoom;
        long long score;

        bool operator<(const Transformation &autre) const;
    };

    //placeurF : placeur final utilise apres l'application de FMME
    //tRot : nombre de rotations a tester
    //tTransX : nombre de translations en X a tester
    //tTransY : nombre de translations en Y a tester
    //tZoom : nombre de zooms a tester
    //tFmme : nombre de positionnements initiaux de FMME a tester
    PlacementFmme(const AlgosPlacement &placeurF, int tRot = 1, int tTransX = 1, int tTransY = 1, int tZoom = 1, int tFmme = 1);
    PlacementFmme(const PlacementFmme& P);
    PlacementFmme& operator=(const PlacementFmme &P);
    ~PlacementFmme();

    std::vector<int> calcPositionNoeuds(const Graphe &G) override;
    PlacementFmme* clone() const override;
private:
    //Contient le nombre de rotations/translations/zooms differents qu'il faut tester
    //Les tests a effectuer sont representes par un intervalle segmente en le nombre de tests a effectuer
    //Les intervalles sont :
    //- [0, 2pi[ pour les rotations
    //- ]-1.5, 1.5[ pour les translations
    //- ]0.0, 2.0[ pour les zooms
    int nbTestsRotation;
    int nbTestsTranslationX;
    int nbTestsTranslationY;
    int nbTestsZoom;
    int nbTestsFmme;
    AlgosPlacement* placeurFinal;

    std::vector<Transformation> _testerTransformations(const Graphe &G, const std::vector<Position<double>> &posNoeuds);
};

#endif