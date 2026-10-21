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
        for (auto& neighbor : graph.getNeighbors(vertex))
        {
            vertexesAccessible.push_back(neighbor);
        }

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

    return coloration;
}