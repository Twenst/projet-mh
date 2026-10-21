#pragma once
#include <vector>
#include "graph.h"

class Coloration {
public:
    Coloration(Graph& graph, int numColors) : graph(graph), numColors(numColors) {
        colors.resize(graph.n, -1); // Initialize all vertices with no color (-1)
        _isValidColoring = false;
        _isValidPartialColoring = true;
    }

    bool isValidColoring(int vertex) {
        for (int j = 0; j < graph.n; j++) {
            if (graph.isLinked(vertex, j) && colors[vertex] == colors[j]) {
                return false; // sommets voisins de mm couleur
            }
        }
        return true;
    }

    bool isValidPartialColoring(int vertex) {
        for (int j = 0; j < graph.n; j++) {
            if (graph.isLinked(vertex, j) 
                && (colors[vertex] == colors[j] && colors[vertex] == -1 && colors[j] == -1)) {
                return false; // sommets voisins déjà colorés de mm couleur
            }
        }
        return true;
    }

    void setColor(int vertex, int color) {
        if (vertex < 0 || vertex >= graph.n || color < 0 || color >= numColors) {
            throw std::out_of_range("Vertex or color out of range");
        }
        colors[vertex] = color;
        _isValidPartialColoring = _isValidPartialColoring && isValidPartialColoring(vertex);
    }

    void removeColor(int vertex) {
        if (vertex < 0 || vertex >= graph.n) {
            throw std::out_of_range("Vertex out of range");
        };
        colors[vertex] = -1; // Reset color
    }
private:
    Graph& graph;
    int numColors;
    std::vector<int> colors;
    bool _isValidColoring;
    bool _isValidPartialColoring;
};