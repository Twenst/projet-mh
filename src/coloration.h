#pragma once
#include <vector>
#include "graph.h"

class Coloration {
public:
    Coloration(Graph& graph, int numColors) : graph(graph), numColors(numColors) {
        colors = std::vector<int>(graph.n, -1);
        _isValidColoring = false;
        _isValidPartialColoring = true;
    }

    bool isValidColoring() {
        for (int i = 0; i < graph.n; i++) {
            if (!isValidColoring(i)) {
                return false;
            }
        }
        return true;
    }

    bool isValidPartialColoring() {
        for (int i = 0; i < graph.n; i++) {
            if (!isValidPartialColoring(i)) {
                return false;
            }
        }
        return true;
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
                && colors[vertex] == colors[j] && colors[vertex] != -1 && colors[j] != -1) {
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

    int getColor(int vertex) {
        if (vertex < 0 || vertex >= graph.n) {
            throw std::out_of_range("Vertex out of range");
        }
        return colors[vertex];
    }

    void removeColor(int vertex) {
        if (vertex < 0 || vertex >= graph.n) {
            throw std::out_of_range("Vertex out of range");
        };
        colors[vertex] = -1; // Reset color
    }

    int firstAvailableColor(int vertex) {
        std::vector<bool> usedColors(numColors, false);
        for (int j = 0; j < graph.n; j++) {
            if (graph.isLinked(vertex, j) && colors[j] != -1) {
                usedColors[colors[j]] = true;
            }
        }
        for (int color = 0; color < numColors; color++) {
            if (!usedColors[color]) {
                return color;
            }
        }
        return -1; // No available color
    }

    void printColoration() {
        for (int i = 0; i < graph.n; i++) {
            std::cout << "Vertex " << i << ": Color " << colors[i] << std::endl;
        }
    }

private:
    Graph& graph;
    int numColors;
    std::vector<int> colors;
    bool _isValidColoring;
    bool _isValidPartialColoring;
};