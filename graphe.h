#ifndef INCLUDED_GRAPHE_H
#define INCLUDED_GRAPHE_H
#include "noeud.h"
#include "emplacement.h"
#include "arete.h"
#include "grille.h"
#include "position.h"
#include <vector>
using namespace std;

struct NbIntersections
{
    long long nbNormales;
    long long nbIllegales;
    long long nbSelf;
};

class Graphe
{
public:
    Graphe();
    ~Graphe();
    Graphe(const Graphe &G);
    Graphe& operator=(const Graphe &G);

    //Getters
    //Renvoie le nombre de noeuds
    int getNbNoeuds() const;
    //Renvoie le tableau des noeuds tries par id
    std::vector<Noeud> getNoeuds() const;
    //Renvoie le noeud numero idNoeud
    Noeud getNoeud(int idNoeud) const;
    //Renvoie un tableau contenant l'emplacement sur lequel se situe chaque noeud
    std::vector<int> getPlacementNoeuds() const;
    //Renvoie le nombre d'aretes
    int getNbAretes() const;
    //Renvoie le tableau des aretes triees par id
    std::vector<Arete> getAretes() const;
    //Renvoie l'arete numero idArete
    Arete getArete(int idArete) const;
    //Renvoie le tableau des voisins du graphe
    //Voisins[i] contient les ids des aretes partant du noeud numero i
    std::vector<std::vector<int>> getVoisins() const;
    //Renvoie voisins[idNoeud]
    std::vector<int> getVoisinsNoeud(int idNoeud) const;
    //Renvoie la matrice d'adjacence du graphe
    std::vector<std::vector<int>> getMatriceAdjacence() const;
    //Renvoie le nombre d'emplacements
    int getNbEmplacements() const;
    //Renvoie le tableau des emplacements tries par id
    std::vector<Emplacement> getEmplacements() const;
    //Renvoie l'emplacement numero idEmp
    Emplacement getEmplacement(int idEmp) const;
    //Renvoie la grille utilisee par le graphe
    //ATTENTION : s'il n'y a pas de grille, le programme plante
    const Grille& getGrille() const;
    //Renvoie la hauteur du plan (pas le nombre de lignes de la grille)
    int getHauteurGrille() const;
    //Renvoie la largeur du plan (pas le nombre de colonnes de la grille)
    int getLargeurGrille() const;
    //Renvoie vrai si les coordonnees des noeuds sont des reels
    bool isCoordReel() const;
    //Renvoie vrai si le graphe utilise une grille
    bool hasGrid() const;
    
    //Setters
    void setHauteurGrille(int hauteur);
    void setLargeurGrille(int largeur);

    //Renvoie les coordonées du point en bas à gauche de la boite englobante du graphe
    Position<double> getXYmin() const;
    //Renvoie les coordonées du point en haut à droite de la boite englobante du graphe
    Position<double> getXYmax() const; 

    //Ajoute un noeud au graphe, les ids sont donnes par ordre d'arrivee
    void ajouteNoeud(const Position<double> &pos);
    //Ajoute une arete BIDIRECTIONELLE entre noeudDep et noeudArr
    void ajouteArete(int noeudDep, int noeudArr);
    //Ajoute un emplacement au graphe, les ids sont donnes par ordre d'arrivee
    void ajouteEmplacement(const Position<int> &pos);

    //Cree une grille placee automatiquement en fonction des noeuds et emplacements du graphe
    //Place egalement tous les composants du graphe dans la grille
    //nbLignes et nbColonnes sont le nombre de ligne et de colonnes de la grille
    void creeGrille(int nbLignes, int nbColonnes);
    //Supprime la grille en place
    void supprimeGrille();
    
    //Change le type de coordonnees pour le graphe et tous ses noeuds
    void changeTypeCoord();
    //Bouge le noeud dans le plan
    //ATTENTION : ne retire pas le noeud de son emplacement
    void bougeNoeud(int idNoeud, const Position<double> &pos);
    //Bouge tous les noeuds du graphe dans le plan
    //ATTENTION : ne retire pas les noeuds de leurs emplacements
    void bougeNoeuds(const std::vector<Position<double>> &posNoeuds);
    //Place le noeud sur un emplacement
    void placeNoeud(int idNoeud, int idEmp);
    //Place tous les noeuds sur les emplacements donnes par le tableau
    void placeNoeuds(const std::vector<int> &posNoeuds);
    
    //Calcule le nombre de croisements en moins si on place le noeud idNoeud a l'emplacement idEmp
    //TODO : Rendre la methode const
    long long calcAmeliorationPlacement(int idNoeud, int idEmp);
    
    //Encore plus de getters (pour le score cette fois)
    //Nombre d'intersections ponderees du graphe
    long long getScoreTotal() const;
    //Score des noeuds
    //Le score d'un noeud est la somme des scores de chacune de ses aretes
    //ATTENTION : Methode en temps lineaire pour chaque noeud et quadratique pour tout le graphe
    std::vector<long long> getScoreNoeuds() const;
    //Score du noeud idNoeud
    //Le score d'un noeud est la somme des scores de chacune de ses aretes
    //ATTENTION : Methode en temps lineaire
    long long getScoreNoeud(int idNoeud) const;
    //Nombre d'intersections de chaque type de chaque arete
    std::vector<NbIntersections> getNbIntersectionsAretes() const;
    //Nombre d'intersections de chaque type de l'arete idArete
    NbIntersections getNbIntersectionsArete(int idArete) const;
private:
    std::vector<Noeud> noeuds;
    std::vector<std::vector<int>> voisins; //voisins[idNoeud] contient les ids de toutes les aretes parcourues par le noeud idNoeud
    std::vector<Arete> aretes;
    int nbAretes;
    std::vector<Emplacement> emplacements;
    bool coordReel;
    Grille* grille;
    long long scoreTotal; //Score total des aretes du graphe
    std::vector<NbIntersections> nbIntersectionsAretes; //Compteur pour le nombre d'intersections de chaque type pour chaque arete, ATTENTION : la somme des intersections est le double du nombre d'intersections total
    int hauteurGrille;
    int largeurGrille;

    //Attribut d'optimisation de temps (evite la creation d'un nouveau tableau a chaque fois)
    std::vector<int> _listeAretesPotentielles;

    //Copie des attributs du graphe pour le constructeur par recopie et operateur =
    void _copieGraphe(const Graphe &G);

    //Ajoute les intersections de l'arete idArete aux compteurs d'intersections
    //On suppose que l'arete n'est pas encore comptabilisee parmi les intersections
    void _ajouteIntersectionsArete(int idArete);

    //Supprime les intersections de l'arete idArete aux compteurs d'intersections
    //On suppose que l'arete est deja comptabilisee correctement parmi les intersections
    void _supprimeIntersectionsArete(int idArete);

    //Met a jour les intersections de l'arete idArete aux compteurs d'intersections
    //Coef est a -1 en cas de suppression et +1 en cas d'ajout
    void _miseAJourIntersectionsArete(int idArete, int coef);

    //Met a jour l'intersection de l'arete idArete1 et idArete2
    //Coef est a -1 en cas de suppression et +1 en cas d'ajout
    void _miseAJourIntersectionsArete(int idArete1, int idArete2, int coef);

    //Renvoie la liste des aretes qui peuvent potentiellement toucher l'arete idArete
    //S'il y a une grille, ce sont les aretes qui passent par des cellules en commun
    //S'il n'y a pas de grille, ce sont toutes les aretes sauf l'arete idArete
    //listeAretes contient la liste des aretes trouvees
    void _getAretesPotentielles(int idArete, std::vector<int> &listeAretes);

    //Renvoie la liste des aretes qui peuvent potentiellement toucher l'arete idArete
    //Ce sont toutes les aretes du graphe sauf l'arete idArete
    //listeAretes contient la liste des aretes trouvees
    void _getAretesPotentiellesSansGrille(int idArete, std::vector<int> &listeAretes);

    //Renvoie le type d'intersection entre les aretes 1 et 2
    //-1 : pas d'intersection
    //0 : intersection normale
    //1 : intersection illegale
    //2 : intersection self
    int _getTypeIntersection(int id1, int id2) const;

    //Renvoie le type d'intersection entre les aretes 1 et 2 donnees par les positions des noeuds
    //NE FAIS PAS LA DIFFERENCE ENTRE INTERSECTION ILLEGALE ET SELF
    //-1 : pas d'intersection
    //0 : intersection normale
    //1 : intersection illegale
    template <typename T>
    int _getTypeIntersection(const Position<T> &dep1, const Position<T> &arr1, const Position<T> &dep2, const Position<T> &arr2) const;

    //Renvoie la penalite en cas d'intersection illegale (nombre de noeuds)
    long long _getPenalite() const;

    //Recalcule le score et les intersections
    //Utile pour les methodes qui modifie le placement de plusieurs noeuds a la fois (pour eviter de recalculer a chaque etape)
    void _recalcIntersections();
};

#include "graphe_template.cpp"

#endif