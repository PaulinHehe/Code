#include "placement_fmme.h"
#include "placement_plus_proche.h"
#include "placement_ordonne.h"
#include "manipulateur_ogdf.h"
#include "algos_graphe.h"
#include "display_openGL.h"
#include "utilitaire.h"
#include "statistiques.h"
#include <cmath>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/energybased/FastMultipoleEmbedder.h>

bool PlacementFmme::Transformation::operator<(const PlacementFmme::Transformation &autre) const
{
    return score < autre.score;
}

PlacementFmme::PlacementFmme(const AlgosPlacement& placeurF, int tRot, int tTransX, int tTransY, int tZoom, int tFmme):
    nbTestsRotation{tRot}, nbTestsTranslationX{tTransX}, nbTestsTranslationY{tTransY}, nbTestsZoom{tZoom}, nbTestsFmme{tFmme}
{
    placeurFinal = placeurF.clone();
}

PlacementFmme::PlacementFmme(const PlacementFmme &P)
{
    nbTestsRotation = P.nbTestsRotation;
    nbTestsTranslationX = P.nbTestsTranslationX;
    nbTestsTranslationY = P.nbTestsTranslationY;
    nbTestsZoom = P.nbTestsZoom;
    placeurFinal = P.placeurFinal->clone();
}

PlacementFmme& PlacementFmme::operator=(const PlacementFmme& P)
{
    nbTestsRotation = P.nbTestsRotation;
    nbTestsTranslationX = P.nbTestsTranslationX;
    nbTestsTranslationY = P.nbTestsTranslationY;
    nbTestsZoom = P.nbTestsZoom;
    placeurFinal = P.placeurFinal->clone();
    return *this;
}

PlacementFmme::~PlacementFmme()
{
    delete placeurFinal;
}

std::vector<int> PlacementFmme::calcPositionNoeuds(const Graphe &G)
{
    Graphe Gcopy = G;

    ManipulateurOgdf manip;
    ogdf::Graph ogdfG;
    ogdf::GraphAttributes ogdfGA;
    manip.creerGrapheOgdf(G, ogdfG, ogdfGA);
    AlgosGraphe algos;
    std::vector<std::vector<int>> CFC = algos.calcComposantesFortementConnexes(G);
    ogdf::FastMultipoleMultilevelEmbedder fmme;
    fmme.multilevelUntilNumNodesAreLess(CFC.size() + 10);

    std::vector<int> meilleuresPlaces;
    long long meilleurScore = 1e18;
    for (int i = 0; i < nbTestsFmme; i++)
    {
        fmme.call(ogdfGA);
        manip.fusionneBoitesEnglobantes(G, ogdfG, ogdfGA); //Inefficace
        std::vector<Position<double>> posNoeuds = manip.extrairePositions(ogdfG, ogdfGA);
        std::vector<Transformation> transformations = _testerTransformations(G, posNoeuds);
        Transformation transMin = minimum(transformations);
        std::vector<Position<double>> posTrans = translationRelative(posNoeuds, {transMin.depX, transMin.depY});
        std::vector<Position<double>> posRot = rotationRelative(posTrans, transMin.angle);
        std::vector<Position<double>> posZoom = zoomRelatif(posRot, transMin.zoom);
        
        Graphe GNoeuds;
        for (Position<double> pos : posZoom) GNoeuds.ajouteNoeud(pos);
        for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++) GNoeuds.ajouteEmplacement(G.getEmplacement(idEmp).getPos());
        std::vector<int> places = placeurFinal->calcPositionNoeuds(GNoeuds);

        if (transMin.score < meilleurScore)
        {
            meilleurScore = transMin.score;
            meilleuresPlaces = places;
        }
    }

    return meilleuresPlaces;
}

PlacementFmme* PlacementFmme::clone() const
{
    return new PlacementFmme(*this);
}

std::vector<PlacementFmme::Transformation> PlacementFmme::_testerTransformations(const Graphe &G, const std::vector<Position<double>> &posNoeuds)
{
    Graphe Gcopy = G;

    std::vector<double> angles, depXs, depYs, zooms;
    for (int i = 0; i < nbTestsRotation; i++) angles.push_back(i * 2.0 * PI / nbTestsRotation);
    for (int i = 0; i < nbTestsTranslationX; i++) depXs.push_back((i+1) * 3.0 / (nbTestsTranslationX+1) - 1.5);
    for (int i = 0; i < nbTestsTranslationY; i++) depYs.push_back((i+1) * 3.0 / (nbTestsTranslationY+1) - 1.5);
    for (int i = 0; i < nbTestsZoom; i++) zooms.push_back((i+1) * 2.0 / (nbTestsZoom+1));
    std::vector<Transformation> transformations;
    
    int A = angles.size(), Z = zooms.size(), X = depXs.size(), Y = depYs.size();
    /*
    std::vector<std::vector<string>> tabCSVTrans(Z*(Y+2) + 2, std::vector<string>(A*(X+2) + 2, ""));
    std::vector<std::vector<string>> tabCSVRot(Y*(Z+2) + 2, std::vector<string>(X*(A+2) + 2, ""));
    tabCSVTrans[0][0] = "Z\\A";
    for (int a = 0; a < A; a++)
    {
        int colB = (X+2)*a + 2;
        tabCSVTrans[0][colB] = to_string(angles[a]);
        for (int z = 0; z < Z; z++)
        {
            int ligB = (Y+2)*z + 2;
            tabCSVTrans[ligB][0] = to_string(zooms[z]);
            tabCSVTrans[ligB][colB] = "Y\\X";
            for (int x = 0; x < X; x++)
            {
                tabCSVTrans[ligB][colB + x + 1] = to_string(depXs[x]);
            }
            for (int y = 0; y < Y; y++)
            {
                tabCSVTrans[ligB + y + 1][colB] = to_string(depYs[y]);
            }
        }
    }
    tabCSVRot[0][0] = "Y\\X";
    for (int x = 0; x < X; x++)
    {
        int colB = (A+2)*x + 2;
        tabCSVRot[0][colB] = to_string(depXs[x]);
        for (int y = 0; y < Y; y++)
        {
            int ligB = (Z+2)*y + 2;
            tabCSVRot[ligB][0] = to_string(depYs[y]);
            tabCSVRot[ligB][colB] = "Z\\A";
            for (int a = 0; a < A; a++)
            {
                tabCSVRot[ligB][colB + a + 1] = to_string(angles[a]);
            }
            for (int z = 0; z < Z; z++)
            {
                tabCSVRot[ligB + z + 1][colB] = to_string(zooms[z]);
            }
        }
    }
    */
    
    std::vector<Position<double>> posEmp;
    for (int i = 0; i < G.getNbEmplacements(); i++) posEmp.push_back(G.getEmplacement(i).getPos());
    for (int x = 0; x < depXs.size(); x++)
    {
        double depX = depXs[x];
        for (int y = 0; y < depYs.size(); y++)
        {
            double depY = depYs[y];
            std::vector<Position<double>> posTrans = translationRelative(posNoeuds, {depX, depY});
            for (int a = 0; a < angles.size(); a++)
            {
                double angle = angles[a];
                std::vector<Position<double>> posRot = rotationRelative(posTrans, angle);
                for (int z = 0; z < zooms.size(); z++)
                {
                    double zoom = zooms[z];
                    std::vector<Position<double>> posZoom = zoomRelatif(posRot, zoom);
                    fusionneBoitesEnglobantes(posEmp, posZoom);

                    Graphe GNoeuds;
                    for (Position<double> pos : posZoom) GNoeuds.ajouteNoeud(pos);
                    for (int idEmp = 0; idEmp < G.getNbEmplacements(); idEmp++) GNoeuds.ajouteEmplacement(G.getEmplacement(idEmp).getPos());
                    std::vector<int> places = placeurFinal->calcPositionNoeuds(GNoeuds);

                    Gcopy.placeNoeuds(places);
                    transformations.push_back({angle, depX, depY, zoom, Gcopy.getScoreTotal()});

                    /*
                    Gcopy.placeNoeuds(std::vector<int>(Gcopy.getNbNoeuds(), -1));
                    Gcopy.bougeNoeuds(posZoom);
                    cout << "Transformation (" << depX << "," << depY << "," << angle << "," << zoom << ") : " << transformations.back().score << "\n";
                    dispOpenGL(Gcopy);
                    Gcopy.placeNoeuds(places);
                    dispOpenGL(Gcopy);
                    */

                    //tabCSVTrans[(Y+2)*z + y + 3][(X+2)*a + x + 3] = to_string(Gcopy.getScoreTotal());
                    //tabCSVRot[(Z+2)*y + z + 3][(A+2)*x + a + 3] = to_string(Gcopy.getScoreTotal());
                }
            }
        }
    }
    
    //genere_csv_std::vector(tabCSVTrans, fichierStatsTrans);
    //genere_csv_std::vector(tabCSVRot, fichierStatsRot);

    return transformations;
}