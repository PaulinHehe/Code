#ifndef INCLUDED_GRAPHE_TEMPLATE_CPP
#define INCLUDED_GRAPHE_TEMPLATE_CPP
#include "graphe.h"
#include "intersection.h"

template <typename T>
int Graphe::_getTypeIntersection(const Position<T> &dep1, const Position<T> &arr1, const Position<T> &dep2, const Position<T> &arr2) const
{
    bool isIllegal;
    if (seCroisent(dep1, arr1, dep2, arr2, isIllegal))
    {
        if (isIllegal)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return -1;
    }
}

#endif