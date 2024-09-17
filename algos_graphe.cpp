#include "algos_graphe.h"
#include "utilitaire.h"

std::vector<std::vector<int>> AlgosGraphe::calcComposantesFortementConnexes(const Graphe &G) const
{
    std::vector<std::vector<int>> CFC;
    std::vector<std::vector<int>> voisins = G.getVoisins();
    std::vector<Arete> aretes = G.getAretes();
    std::vector<bool> dejaFait(G.getNbNoeuds(), false);
    for (int dep = 0; dep < G.getNbNoeuds(); dep++)
    {
        if (!dejaFait[dep])
        {
            CFC.push_back({dep});
            std::vector<int> aFaire = {dep};
            dejaFait[dep] = true;
            while (!aFaire.empty())
            {
                int noeud = aFaire.back();
                aFaire.pop_back();
                for (int idArete : voisins[noeud])
                {
                    int prochain = aretes[idArete].getAutreExtremite(noeud);
                    if (!dejaFait[prochain])
                    {
                        aFaire.push_back(prochain);
                        CFC.back().push_back(prochain);
                        dejaFait[prochain] = true;
                    }
                }
            }
        }
    }
    return CFC;
}

void AlgosGraphe::compteNoeudsAretes(const std::vector<std::vector<AreteAlgo>> &voisins, int &n, int &m) const
{
    n = voisins.size();
    m = 0;
    for (int i = 0; i < n; i++) m += voisins[i].size();
    m /= 2;
}

//X et Y contiennent les noeuds de chaque partie du graphe biparti
void AlgosGraphe::calcPartiesGrapheBiparti(const std::vector<std::vector<AreteAlgo>> &voisins, std::vector<int> &X, std::vector<int> &Y) const
{
    int n = voisins.size();
    X.clear();
    Y.clear();
    std::vector<int> dejaFait(n, -1);
    for (int noeud = 0; noeud < n; noeud++)
    {
        if (dejaFait[noeud] == -1)
        {
            std::vector<int> aFaire = {noeud}, aRajouter;
            dejaFait[noeud] = 1;
            int cote = 0;
            while (!aFaire.empty())
            {
                for (int noeud : aFaire)
                {
                    if (cote == 0) X.push_back(noeud);
                    else Y.push_back(noeud);
                    for (AreteAlgo prochain : voisins[noeud])
                    {
                        if (dejaFait[prochain.noeud] == -1)
                        {
                            aRajouter.push_back(prochain.noeud);
                            dejaFait[prochain.noeud] = cote;
                        }
                        else if (dejaFait[prochain.noeud] != cote) //Pas un graphe biparti
                        {
                            X.clear();
                            Y.clear();
                            return;
                        }
                    }
                }
                aFaire = aRajouter;
                aRajouter.clear();
                cote = 1-cote;
            }
        }
    }
}

void AlgosGraphe::bfsCouplageMaxBiparti(const std::vector<int> &departs, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<bool> &estLibre, const std::vector<bool> &estDansCouplage, std::vector<int> &dist, std::vector<int> &libresTrouves) const
{
    int n = voisins.size();
    libresTrouves.clear();
    dist.clear();
    dist.resize(n, -1);

    std::vector<int> aFaire = departs, aRajouter;
    std::vector<bool> dejaFait(n, false);
    int d = 0;
    bool suivreCouplage = false;
    while (!aFaire.empty() && libresTrouves.empty())
    {
        for (int noeud : aFaire)
        {
            dist[noeud] = d;
            if (d != 0 && estLibre[noeud])
            {
                libresTrouves.push_back(noeud);
            }
            for (AreteAlgo prochain : voisins[noeud])
            {
                if (!dejaFait[prochain.noeud] && suivreCouplage == estDansCouplage[prochain.id])
                {
                    dejaFait[prochain.noeud] = true;
                    aRajouter.push_back(prochain.noeud);
                }
            }
        }
        aFaire = aRajouter;
        aRajouter.clear();
        d++;
        suivreCouplage = !suivreCouplage;
    }
}

//Chemin contient les ids des AreteAlgos
bool AlgosGraphe::dfsCouplageMaxBiparti(int noeud, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<int> &dist, const std::vector<bool> &estLibre, const std::vector<bool> &estDansCouplage, std::vector<bool> &dejaFait, std::vector<AreteAlgo> &chemin) const
{
    dejaFait[noeud] = true;
    if (!chemin.empty() && estLibre[noeud])
    {
        return true;
    }

    bool suivreCouplage = (dist[noeud]%2 == 0);
    for (AreteAlgo prochain : voisins[noeud])
    {
        if (!dejaFait[prochain.noeud] && dist[prochain.noeud] == dist[noeud]-1 && suivreCouplage == estDansCouplage[prochain.id])
        {
            chemin.push_back(prochain);
            if (dfsCouplageMaxBiparti(prochain.noeud, voisins, dist, estLibre, estDansCouplage, dejaFait, chemin))
            {
                return true;
            }
            chemin.pop_back();
        }
    }
    return false;
}

//Renvoie les ids des AreteAlgos du couplage
std::vector<int> AlgosGraphe::couplageMaxBiparti(const std::vector<std::vector<AreteAlgo>> &voisins) const
{
    int n, m;
    compteNoeudsAretes(voisins, n, m);

    //Detection des deux parties
    std::vector<int> X, Y;
    calcPartiesGrapheBiparti(voisins, X, Y);

    //Creation du couplage
    std::vector<bool> estLibre(n, true);
    std::vector<bool> estDansCouplage(m, false);
    bool fini = false;
    while (!fini)
    {
        //Init du parcours en largeur
        std::vector<int> departs;
        for (int noeud : X)
        {
            if (estLibre[noeud])
            {
                departs.push_back(noeud);
            }
        }

        //Parcours en largeur
        std::vector<int> libresTrouves;
        std::vector<int> dist;
        bfsCouplageMaxBiparti(departs, voisins, estLibre, estDansCouplage, dist, libresTrouves);
        fini = libresTrouves.empty();

        //Parcours en profondeur
        std::vector<bool> dejaFait(n, false);
        for (int depart : libresTrouves)
        {
            std::vector<AreteAlgo> chemin;
            if (dfsCouplageMaxBiparti(depart, voisins, dist, estLibre, estDansCouplage, dejaFait, chemin))
            {
                for (AreteAlgo arete : chemin)
                {
                    estDansCouplage[arete.id] = !estDansCouplage[arete.id];
                }
                estLibre[depart] = false;
                estLibre[chemin.back().noeud] = false;
            }
        }
    }

    //Couplage final
    std::vector<int> ans;
    for (int id = 0; id < m; id++)
    {
        if (estDansCouplage[id])
        {
            ans.push_back(id);
        }
    }
    return ans;
}

//restreint est a vrai si on part de noeuds libres (il faut suivre un chemin alternant)
void AlgosGraphe::bfsMinVertexCoverBipartite(const std::vector<int> &departs, const std::vector<std::vector<AreteAlgo>> &voisins, const std::vector<bool> &estDansCouplage, std::vector<bool> &dejaFait, std::vector<int> &vertexCover, bool restreint) const
{
    for (int noeud : departs) dejaFait[noeud] = true;
    std::vector<int> aFaire = departs, aRajouter;
    bool suivreCouplage = false;

    while (!aFaire.empty())
    {
        for (int noeud : aFaire)
        {
            if (suivreCouplage)
            {
                vertexCover.push_back(noeud);
            }
            for (AreteAlgo prochain : voisins[noeud])
            {
                if (!dejaFait[prochain.noeud] && (suivreCouplage == estDansCouplage[prochain.id] || !restreint))
                {
                    dejaFait[prochain.noeud] = true;
                    aRajouter.push_back(prochain.noeud);
                }
            }
        }
        aFaire = aRajouter;
        aRajouter.clear();
        suivreCouplage = !suivreCouplage;
    }
}

std::vector<int> AlgosGraphe::minVertexCoverBipartite(const std::vector<std::vector<AreteAlgo>> &voisins) const
{
    int n, m;
    compteNoeudsAretes(voisins, n, m);

    //Calcul des noeuds libres
    std::vector<int> couplage = couplageMaxBiparti(voisins);
    std::vector<bool> estDansCouplage(m, false);
    for (int id : couplage)
    {
        estDansCouplage[id] = true;
    }
    std::vector<bool> estLibre(n, true);
    for (int noeud = 0; noeud < n; noeud++)
    {
        for (AreteAlgo arete : voisins[noeud])
        {
            if (estDansCouplage[arete.id])
            {
                estLibre[noeud] = false;
            }
        }
    }

    //Init du BFS
    std::vector<int> departs;
    std::vector<bool> dejaFait(n, false);
    std::vector<int> vertexCover;
    for (int noeud = 0; noeud < n; noeud++)
    {
        if (estLibre[noeud])
        {
            departs.push_back(noeud);
        }
    }

    //BFS initial a partir des noeuds libres
    bfsMinVertexCoverBipartite(departs, voisins, estDansCouplage, dejaFait, vertexCover, true);

    //BFS pour les noeuds restants
    for (int noeud = 0; noeud < n; noeud++)
    {
        if (!dejaFait[noeud])
        {
            std::vector<int> depart = {noeud};
            bfsMinVertexCoverBipartite(depart, voisins, estDansCouplage, dejaFait, vertexCover, false);
        }
    }

    return vertexCover;
}

std::vector<int> AlgosGraphe::assignementProblem(const std::vector<std::vector<double>> &c) const
{
    std::vector<std::vector<double>> costs = c;
    int nbW = costs.size(), nbJ = costs[0].size();
    //fill missing workers
    for (int i = nbW; i < nbJ; i++)
    {
        costs.push_back(std::vector<double>(nbJ, 0));
    }
    //fill missing jobs
    for (int i = nbJ; i < nbW; i++)
    {
        for (int w = 0; w < nbW; w++)
        {
            costs[w].push_back(0);
        }
    }
    int n = costs.size();

    //Row and column reduction
    for (int row = 0; row < n; row++)
    {
        double mini = costs[row][0];
        for (int col = 1; col < n; col++)
        {
            mini = min(mini, costs[row][col]);
        }
        for (int col = 0; col < n; col++)
        {
            costs[row][col] -= mini;
        }
    }
    for (int col = 0; col < n; col++)
    {
        double mini = costs[0][col];
        for (int row = 1; row < n; row++)
        {
            mini = min(mini, costs[row][col]);
        }
        for (int row = 0; row < n; row++)
        {
            costs[row][col] -= mini;
        }
    }

    //Lines and columns crossed out
    std::vector<bool> rowCrossed(n, false), colCrossed(n, false);
    std::vector<int> crossRows, crossCols;
    std::vector<int> selectRows, selectCols;
    while (selectRows.size() < n)
    {
        //cout << "start of step with select size " << selectRows.size() << "\n";

        //Shift zeros
        double mini = DBL_MAX;
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (!rowCrossed[row] && !colCrossed[col])
                {
                    mini = min(mini, costs[row][col]);
                }
            }
        }
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (rowCrossed[row] && colCrossed[col])
                {
                    costs[row][col] += mini;
                }
                else if (!rowCrossed[row] && !colCrossed[col])
                {
                    costs[row][col] -= mini;
                }
            }
        }

        //Init bipartite graph matching and cover
        std::vector<std::vector<AreteAlgo>> voisins(2*n);
        std::vector<int> zerosRows, zerosCols;
        for (int row = 0; row < n; row++)
        {
            for (int col = 0; col < n; col++)
            {
                if (costs[row][col] == 0)
                {
                    int id = zerosRows.size();
                    voisins[row].push_back({id, col+n});
                    voisins[col+n].push_back({id, row});
                    zerosRows.push_back(row);
                    zerosCols.push_back(col);
                }
            }
        }

        //Contains zeros (edges)
        std::vector<int> maxMatching = couplageMaxBiparti(voisins);

        //Contains crossed rows and columns (vertices)
        std::vector<int> minCover = minVertexCoverBipartite(voisins);

        //DEBUG
        int N, M;
        compteNoeudsAretes(voisins, N, M);
        std::vector<bool> covered(M, false);
        for (int noeud : minCover)
        {
            for (AreteAlgo arete : voisins[noeud])
            {
                covered[arete.id] = true;
            }
        }
        for (int id = 0; id < M; id++)
        {
            if (!covered[id])
            {
                cout << "not covered : " << id << "\n";
            }
        }

        //Update crossed rows and columns
        crossRows.clear();
        crossCols.clear();
        selectRows.clear();
        selectCols.clear();
        rowCrossed.clear();
        colCrossed.clear();
        rowCrossed.resize(n, false);
        colCrossed.resize(n, false);
        for (int id : maxMatching)
        {
            selectRows.push_back(zerosRows[id]);
            selectCols.push_back(zerosCols[id]);
        }
        for (int node : minCover)
        {
            if (node < n) //Row
            {
                crossRows.push_back(node);
                rowCrossed[node] = true;
            }
            else //Column
            {
                crossCols.push_back(node - n);
                colCrossed[node - n] = true;
            }
        }
    }

    std::vector<int> ans(nbW, -1);
    for (int i = 0; i < n; i++)
    {
        int row = selectRows[i], col = selectCols[i];
        if (row < nbW && col < nbJ)
        {
            ans[row] = col;
        }
    }
    return ans;
}

std::vector<int> AlgosGraphe::greedyAssignementProblem(const std::vector<std::vector<double>> &c) const
{
    struct Assignement
    {
        int W, J;
        double dist;
        
        bool operator<(const Assignement &autre) const
        {
            return dist < autre.dist;
        }
    };

    std::vector<Assignement> liste;
    for (int i = 0; i < c.size(); i++)
    {
        for (int j = 0; j < c[i].size(); j++)
        {
            liste.push_back({i, j, c[i][j]});
        }
    }
    sort(liste.begin(), liste.end());
    std::vector<int> places(c.size(), -1);
	std::vector<bool> empDispo(c[0].size(), true);
	int nbPlaces = 0;
	int i = 0;
	while (nbPlaces < c.size())
	{
		Assignement place = liste[i];
		i++;
		if (places[place.W] == -1 && empDispo[place.J])
		{
			places[place.W] = place.J;
			empDispo[place.J] = false;
			nbPlaces++;
		}
	}

	return places;
}