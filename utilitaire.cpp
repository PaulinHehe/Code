#include "utilitaire.h"

mutex randMutex;

long long int getMS(){
    long long int ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    return ms;
}

int getRand()
{
    lock_guard<mutex> lock(randMutex);
    return rand();
}

int generateRand(int n) {
    return getRand()%n;
}

double generateRandDouble(double mini, double maxi)
{
    if (mini == maxi) return mini;
    long long int choix = getRand();
    while (choix == RAND_MAX) choix = getRand();
    return (1.0*choix / (RAND_MAX) * (maxi - mini)) + mini;
}