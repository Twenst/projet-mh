#pragma once
#include <vector>
#include "graph.h"
#include <list>
#include <random>

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
                && colors[vertex] == colors[j]  && colors[j] != -1) {
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

    int getConflitCount() 
    {
        int conflictCount = 0;
        for (int i = 0; i < graph.n; i++)
        {
            for (int j = i + 1; j < graph.n; j++)
            {
                if (graph.isLinked(i, j) && colors[i] == colors[j] && colors[i] != -1)
                {
                    conflictCount++;
                }
            }
        }
        return conflictCount;
    }

    int DSAT(int vertex) {
        std::vector<bool> usedColors(numColors, false);
        for (int j = 0; j < graph.n; j++) {
            if (graph.isLinked(vertex, j) && colors[j] != -1) {
                usedColors[colors[j]] = true;
            }
        }
        int dsat = 0;
        for (bool used : usedColors) {
            if (used) {
                dsat++;
            }
        }
        return dsat;
    }

    int getMaxDSAT(std::list<int> vertexes)
    {
        int maxDSAT = -1;
        int maxDeg = -1;
        int argmaxDSAT = -1;
        for (auto i : vertexes)
        {
            if (getColor(i) == -1)
            {
                int dsat = DSAT(i);
                if (dsat == maxDSAT)
                {
                    int deg = graph.getNeighbors(i).size();
                    if (deg > maxDeg)
                    {
                        maxDeg = deg;
                        argmaxDSAT = i;
                    }
                }
                else if (dsat > maxDSAT)
                {
                    maxDSAT = dsat;
                    maxDeg = graph.getNeighbors(i).size();
                    argmaxDSAT = i;
                }
            }
        }

        return argmaxDSAT;
    }

    int getMaxDSAT()
    {
        std::list<int> uncoloredVertexes = {};
        for (int i = 0; i < graph.n; i ++)
        {
            if (getColor(i) == -1)
            {
                uncoloredVertexes.push_back(i);
            }
        }
        return getMaxDSAT(uncoloredVertexes);
    }
        
    /**
    * Met à chaque sommet non coloré la couleur qui est là moins présente parmi ses voisins.
    */
    void fillColorationWithMinConflict()
    {
        std::list<vector<int>> vertexToColor = {};
        for (int i = 0; i < graph.n; i++)
        {
            if (getColor(i) == -1)
            {
                vertexToColor.push_back({int(graph.getNeighbors(i).size()), i});
            }
        }
        vertexToColor.sort([](const vector<int>& a, const vector<int>& b) {
            return a[0] < b[0]; // Degré croissant
        });

        for (auto vertDeg : vertexToColor)
        {
            int vert = vertDeg[1];
            if (getColor(vert) == -1)
            {
                vector<int> colorCounts(numColors, 0);
                for (auto& neighbor : graph.getNeighbors(vert))
                {
                    int neighborColor = getColor(neighbor);
                    if (neighborColor != -1)
                    {
                        colorCounts[neighborColor]++;
                    }
                }
                int minColor = graph.n; 
                for (auto colorCount : colorCounts)
                {
                    if (colorCount < minColor) minColor = colorCount;
                }
                setColor(vert, minColor);
            }
        }
    }

    Coloration copy() 
    {
        Coloration newColoration(graph, numColors);
        newColoration.colors = colors;
        newColoration._isValidColoring = _isValidColoring;
        newColoration._isValidPartialColoring = _isValidPartialColoring;
        return newColoration;
    }

    /**
    * Retourne une coloration voisine de la coloration actuelle en changeant la couleur d'un sommet choisi aléatoirement.
    * La nouvelle couleur est choisie aléatoirement mais elle doit être différente de sorte à produire une solution différente.
    */
    Coloration getRandomNeighborColorationFromV1()
    {
        Coloration newColoration = copy();
        
        int randomVertex = std::rand() % graph.n;
        int randomColor = std::rand() % numColors;
        int currentColor = newColoration.getColor(randomVertex); 

        while (randomColor == currentColor)
        {
            randomColor = std::rand() % numColors;
        }

        newColoration.setColor(randomVertex, randomColor);

        return newColoration;
    }

    /**
    * Retourne une coloration voisine de la coloration actuelle en échangeant les couleurs
    * de 2 sommets aléatoires dont les couleurs sont différentes de sorte à produire une solution différente.
    */
    Coloration getRandomNeighborColorationFromV2()
    {
        Coloration newColoration = copy();
        
        int randomVertex1 = std::rand() % graph.n;
        int currentColor1 = newColoration.getColor(randomVertex1); 
        int randomVertex2 = std::rand() % graph.n;
        int currentColor2 = newColoration.getColor(randomVertex2);

        while (currentColor2 == currentColor1)
        {
            randomVertex2 = std::rand() % graph.n;
            currentColor2 = newColoration.getColor(randomVertex2);
        }

        newColoration.setColor(randomVertex1, currentColor2);
        newColoration.setColor(randomVertex2, currentColor1);

        return newColoration;
    }

    /**
    * Retourne une coloration voisine de la coloration actuelle en échangeant les couleurs
    * de 3 sommets aléatoires dont les couleurs sont toutes différentes de sorte à produire une solution différente.
    */
    Coloration getRandomNeighborColorationFromV3()
    {
        Coloration newColoration = copy();
        
        int randomVertex1 = std::rand() % graph.n;
        int currentColor1 = newColoration.getColor(randomVertex1); 
        int randomVertex2 = std::rand() % graph.n;
        int currentColor2 = newColoration.getColor(randomVertex2);
        int randomVertex3 = std::rand() % graph.n;
        int currentColor3 = newColoration.getColor(randomVertex3);

        while (currentColor2 == currentColor1
            || currentColor2 == currentColor3
            || currentColor3 == currentColor1)
        {
            if (currentColor2 == currentColor1)
            {
                randomVertex2 = std::rand() % graph.n;
                currentColor2 = newColoration.getColor(randomVertex2);
            }
            if (currentColor1 == currentColor3 || currentColor2 == currentColor3)
            {
                randomVertex3 = std::rand() % graph.n;
                currentColor3 = newColoration.getColor(randomVertex3);
            }
        }

        newColoration.setColor(randomVertex1, currentColor2);
        newColoration.setColor(randomVertex2, currentColor3);
        newColoration.setColor(randomVertex3, currentColor1);


        return newColoration;
    }

    Coloration &operator=(const Coloration &other) {
        if (this != &other) {
            this->graph = other.graph;
            this->numColors = other.numColors;
            this->colors = other.colors;
            this->_isValidColoring = other._isValidColoring;
            this->_isValidPartialColoring = other._isValidPartialColoring;
        }
        return *this;
    }

private:
    Graph& graph;
    int numColors;
    std::vector<int> colors;
    bool _isValidColoring;
    bool _isValidPartialColoring;
};