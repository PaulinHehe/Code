#include "tests_graphe.h"

Position<double> generePosition(int xmax, int ymax){
    int x = rand()%xmax;
    int y = rand()%ymax;
    //cout << "x : " << x << " ; y : " << y << "\n";
    return Position<double>(x, y);
}

void genereNoeuds(Graphe& G, int n, int min){
    for(int i=min; i<n; i++){
        G.ajouteNoeud(generePosition());
        //cout << "noeud " << i << " : " << G.getNoeud(i).getPos().getX() << " " << G.getNoeud(i).getPos().getY() << "\n";
    }
}
void genereEmplacements(Graphe& G, int n, int min){
    /*for(int i=min; i<n; i++){
        G.ajouteEmplacement(generePosition());
        //cout << "emplacement " << i << " : " << G.getEmplacement(i).getPos().getX() << " " << G.getEmplacement(i).getPos().getY() << "\n";
    }*/
    G.ajouteEmplacement(Position<double>(10,10));
    G.ajouteEmplacement(Position<double>(10,10));
    G.ajouteEmplacement(Position<double>(25,25));
    G.ajouteEmplacement(Position<double>(70,30));
    G.ajouteEmplacement(Position<double>(50,50));
    G.ajouteEmplacement(Position<double>(10,90));
    G.ajouteEmplacement(Position<double>(10,90));
    G.ajouteEmplacement(Position<double>(20,80));
    G.ajouteEmplacement(Position<double>(70,70));
    G.ajouteEmplacement(Position<double>(100,110));
}
void genereAretes(Graphe& G, int n, int min){
    for(int i=min; i<n; i++){
        int d = rand()%G.getNbNoeuds();
        int a = rand()%G.getNbNoeuds();
        while(a==d){a = rand()%G.getNbNoeuds();}
        
        G.ajouteArete(d, a);
        //cout << "arete " << i << " : " << G.getArete(i).getNoeudArr() << " " << G.getArete(i).getNoeudDep() << "\n";
    }
}
void placeAleatoire(Graphe& G){
    for(int i=0; i<G.getNbNoeuds(); i++){
        int e = rand()%G.getNbEmplacements();
        while(!G.getEmplacement(e).estDisponible()){ e = rand()%G.getNbEmplacements(); }
        //cout << "noeud " << i << "  emplacement " << e << "\n";
        G.placeNoeud(i, e);
        //cout << "noeud " << i << " : " << G.getNoeud(i).getPos().getX() << " " << G.getNoeud(i).getPos().getY() << "\n";
    }
}

void affichePlacement(Graphe&G){
    std::cout << "{ ";
    for(Emplacement empl : G.getEmplacements()){
        std::cout << empl.getId() << " : " ;
        if(empl.estDisponible()){
            std::cout << "_ | ";
        } else {
            std::cout << G.getNoeud(empl.getNoeud()).getId()  << " | ";
        }
    }
    std::cout <<  G.getEmplacement(G.getNbEmplacements()-1).getId() << " : " << G.getNoeud(G.getEmplacement(G.getNbEmplacements()-1).getNoeud()).getId() << " }\n";
}
void afficheNoeuds(Graphe& G){
    std::cout << "{ \n";
    for(Noeud n : G.getNoeuds()){
        std::cout << n.getId() << " : " ;       
        std::cout << n.getPos().getX() << " x " << n.getPos().getY()  << " \n ";       
    }
}

void remplirGraphe(Graphe &G, int nbN, int nbE, int nbA){
    genereNoeuds(G, nbN);
    genereEmplacements(G, nbE);
    genereAretes(G, nbA);
    placeAleatoire(G);
    //affichePlacement(G);
    
    G.creeGrille(G.getNbNoeuds(), G.getNbNoeuds());
}