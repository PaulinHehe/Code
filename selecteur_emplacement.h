#ifndef SELECTEUREMPLACEMENT_H_GUARD
#define SELECTEUREMPLACMENT_H_GUARD
#pragma once

#include <random>
#include "graphe.h"
#include "geometrie.h"
#include "utilitaire.h"


class SelecteurEmplacement{
public:
    virtual int selectionneEmplacement(const Graphe &G, int nodeId) = 0;
    virtual SelecteurEmplacement* clone() const = 0;
};

#endif