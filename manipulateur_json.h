#ifndef INCLUDED_MANIPULATEUR_JSON_H
#define INCLUDED_MANIPULATEUR_JSON_H
#include "graphe.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "position.h"
using nlohmann::json;
using namespace std;

class ManipulateurJson
{
public:
    bool lireNoeudsDepuisJson(const string &fichier, Graphe &G) const;
    bool lireEmplacementsDepuisJson(const string &fichier, Graphe &G) const;
    bool lireGrapheDepuisJson(const string &fichier, Graphe &G) const;
    void ecrireNoeudsDansJson(const string &fichier, const Graphe &G) const;
    void ecrireEmplacementsDansJson(const string &fichier, const Graphe &G) const;
    void ecrireGrapheDansJson(const string &fichier, const Graphe &G) const;
private:
    bool _ajouteNoeudsDepuisJson(const json &file, Graphe &G) const;
    bool _ajouteEmplacementsDepuisJson(const json &file, Graphe &G) const;
    void _ajouteNoeudsDansJson(json& file, const Graphe &G) const;
    void _ajouteEmplacementsDansJson(json& file, const Graphe &G) const;
};

#endif