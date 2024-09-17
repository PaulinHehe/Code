#include "manipulateur_json.h"

bool ManipulateurJson::lireNoeudsDepuisJson(const string &fichier, Graphe &G) const
{
    ifstream fin(fichier);
    if (!fin) return false;
    Graphe Gtemp;
    json file;
    fin >> file;
    if (!_ajouteNoeudsDepuisJson(file, Gtemp)) return false;
    G = Gtemp;
    return true;
}

bool ManipulateurJson::lireEmplacementsDepuisJson(const string &fichier, Graphe &G) const
{
    ifstream fin(fichier);
    if (!fin) return false;
    Graphe Gtemp;
    json file;
    fin >> file;
    if (!_ajouteEmplacementsDepuisJson(file, Gtemp)) return false;
    G = Gtemp;
    return true;
}

bool ManipulateurJson::lireGrapheDepuisJson(const string &fichier, Graphe &G) const
{
    ifstream fin(fichier);
    if (!fin) return false;
    Graphe Gtemp;
    json file;
    fin >> file;
    if (!_ajouteNoeudsDepuisJson(file, Gtemp)) return false;
    if (!_ajouteEmplacementsDepuisJson(file, Gtemp)) return false;
    G = Gtemp;
    return true;
}

void ManipulateurJson::ecrireNoeudsDansJson(const string &fichier, const Graphe &G) const
{
    ofstream fout(fichier);
    json file;
    _ajouteNoeudsDansJson(file, G);
    fout << setw(4) << file << endl;
}

void ManipulateurJson::ecrireEmplacementsDansJson(const string &fichier, const Graphe &G) const
{
    ofstream fout(fichier);
    json file;
    _ajouteEmplacementsDansJson(file, G);
    fout << setw(4) << file << endl;
}

void ManipulateurJson::ecrireGrapheDansJson(const string &fichier, const Graphe &G) const
{
    ofstream fout(fichier);
    json file;
    _ajouteNoeudsDansJson(file, G);
    _ajouteEmplacementsDansJson(file, G);
    fout << setw(4) << file << endl;
}

void ManipulateurJson::_ajouteNoeudsDansJson(json &file, const Graphe &G) const
{
    file["nodes"] = json::array();
    for (int i = 0; i < G.getNbNoeuds(); i++)
    {
        Noeud noeud = G.getNoeud(i);
        json noeudJson;
        noeudJson["id"] = noeud.getId();
        noeudJson["x"] = noeud.getPos().getX();
        noeudJson["y"] = noeud.getPos().getY();
        file["nodes"].push_back(noeudJson);
    }

    file["edges"] = json::array();
    for (int i = 0; i < G.getNbAretes(); i++)
    {
        Arete arete = G.getArete(i);
        json areteJson;
        areteJson["source"] = arete.getNoeudDep();
        areteJson["target"] = arete.getNoeudArr();
        file["edges"].push_back(areteJson);
    }
}

void ManipulateurJson::_ajouteEmplacementsDansJson(json &file, const Graphe &G) const
{
    file["width"] = G.getLargeurGrille();
    file["height"] = G.getHauteurGrille();

    file["points"] = json::array();
    for (int i = 0; i < G.getNbEmplacements(); i++)
    {
        Emplacement emp = G.getEmplacement(i);
        json empJson;
        empJson["id"] = emp.getId();
        empJson["x"] = emp.getPos().getX();
        empJson["y"] = emp.getPos().getY();
        file["points"].push_back(empJson);
    }
}

bool ManipulateurJson::_ajouteNoeudsDepuisJson(const json &file, Graphe &G) const
{
    Graphe Gtemp = G;
    if (!file.contains("nodes") || !file.contains("edges"))
    {
        return false;
    }

    int nbNoeuds = (int)file["nodes"].size();
    std::vector<Position<double>> posNoeuds(nbNoeuds);
    for (int i = 0; i < nbNoeuds; i++)
    {
        json noeud = file["nodes"][i];
        if (!noeud.contains("id") || !noeud.contains("x") || !noeud.contains("y"))
        {
            return false;
        }
        int id = noeud["id"];
        Position<double> pos = {noeud["x"], noeud["y"]};
        posNoeuds[id] = pos;
    }
    for (int i = 0; i < nbNoeuds; i++)
    {
        Gtemp.ajouteNoeud(posNoeuds[i]);
    }

    int nbAretes = (int)file["edges"].size();
    for (int i = 0; i < nbAretes; i++)
    {
        json arete = file["edges"][i];
        if (!arete.contains("source") || !arete.contains("target"))
        {
            return false;
        }
        int source = arete["source"];
        int target = arete["target"];
        Gtemp.ajouteArete(source, target);
    }

    G = Gtemp;

    return true;
}

bool ManipulateurJson::_ajouteEmplacementsDepuisJson(const json &file, Graphe &G) const
{
    Graphe Gtemp = G;
    if (!file.contains("points"))
    {
        return false;
    }

    if (file.contains("width")) Gtemp.setLargeurGrille(file["width"]);
    else Gtemp.setLargeurGrille(1000000);

    if (file.contains("height")) Gtemp.setHauteurGrille(file["height"]);
    else Gtemp.setHauteurGrille(1000000);

    int nbEmp = (int)file["points"].size();
    std::vector<Position<int>> posEmp(nbEmp);
    for (int i = 0; i < nbEmp; i++)
    {
        json emp = file["points"][i];
        if (!emp.contains("id") || !emp.contains("x") || !emp.contains("y"))
        {
            return false;
        }
        int id = emp["id"];
        Position<int> pos = {emp["x"], emp["y"]};
        posEmp[id] = pos;
    }
    for (int i = 0; i < nbEmp; i++)
    {
        Gtemp.ajouteEmplacement(posEmp[i]);
    }

    G = Gtemp;

    return true;
}