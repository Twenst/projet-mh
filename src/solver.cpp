#include "solver.h"
#include <list>
#include <algorithm>

Coloration heuristicSolve(Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);
    std::list<int> vertexToColor = {};
    for (int i = 0; i < graph.n; ++i)
    {
        vertexToColor.push_back(i);
    }

    // Pour parcourir toutes les composantes connexes du graphe
    while (!vertexToColor.empty())
    {
        int vertex = vertexToColor.back();
        vertexToColor.pop_back();

        std::list<int> vertexesAccessible;
        vertexesAccessible.push_back(vertex);

        // Parcours en profondeur
        while (!vertexesAccessible.empty())
        {
            int currentVertex = vertexesAccessible.back();
            vertexesAccessible.pop_back();

            int color = coloration.firstAvailableColor(currentVertex);
            if (color != -1)
            {
                coloration.setColor(currentVertex, color);
            }
            vertexToColor.remove(currentVertex);
            for (auto& neighbor : graph.getNeighbors(currentVertex))
            {
                auto it = std::find(vertexToColor.begin(), vertexToColor.end(), neighbor); // si neighbor est tjrs à colorer
                if (coloration.getColor(neighbor) == -1 && it != vertexToColor.end())
                {
                    vertexesAccessible.push_back(neighbor);
                }
            }
        }
    }

    coloration.fillColorationWithMinConflict();

    return coloration;
}

Coloration DSATUR(Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);

    std::list<int> vertexList = {};
    std::list<vector<int>> vertexToColor = {};
    for (int i = 0; i < graph.n; ++i)
    {
        vertexList.push_back(i);
        int deg = graph.getNeighbors(i).size();
        vertexToColor.push_back({deg, i});
    }
    vertexToColor.sort([](const vector<int>& a, const vector<int>& b) {
        return a[0] > b[0]; // Degré décroissant
    });

    int maxDegreeVertex = vertexToColor.front()[1];
    coloration.setColor(maxDegreeVertex, 0);
    vertexList.remove(maxDegreeVertex);

    while (!vertexList.empty())
    {
        int vertex = coloration.getMaxDSAT(vertexList);
        vertexList.remove(vertex);

        int color = coloration.firstAvailableColor(vertex);
        if (color != -1) coloration.setColor(vertex, color);
    }

    coloration.fillColorationWithMinConflict();

    return coloration;
}