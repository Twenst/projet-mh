#include "coloration.h"
#include <vector>
#include "graph.h"
#include <list>
#include <random>
#include "tabou.h"
#include "clock.h"

Coloration::Coloration(const Graph &graph, int numColors) : graph(graph), numColors(numColors)
{
    colors = std::vector<int>(graph.getNumVertices(), -1);
    _conflictCount = 0;
    conflictCountsPerVertex = std::vector<int>(graph.getNumVertices(), 0);
}

void Coloration::setColor(int vertex, int color)
{
    if (vertex < 0 || vertex >= graph.getNumVertices() || color < 0 || color >= numColors)
    {
        throw std::out_of_range("Vertex or color out of range");
    }
    int oldColor = colors[vertex];
    if (oldColor == color)
        return; // Pas de changement de nb de conflits
    colors[vertex] = color;
    for (int neighbor : graph.getNeighbors(vertex))
    {
        if (colors[neighbor] == color && colors[neighbor] != -1 && color != -1)
        {
            conflictCountsPerVertex[vertex]++;
            conflictCountsPerVertex[neighbor]++;
            _conflictCount += 2;
        }
        if (colors[neighbor] == oldColor && colors[neighbor] != -1 && oldColor != -1)
        {
            conflictCountsPerVertex[vertex]--;
            conflictCountsPerVertex[neighbor]--;
            _conflictCount -= 2;
        }
    }
}

int Coloration::getColor(int vertex) const
{
    if (vertex < 0 || vertex >= graph.getNumVertices())
    {
        throw std::out_of_range("Vertex out of range");
    }
    return colors[vertex];
}

int Coloration::getConflictCountPerVertex(int vertex) const
{
    if (vertex < 0 || vertex >= graph.getNumVertices())
    {
        throw std::out_of_range("Vertex out of range");
    }
    return conflictCountsPerVertex[vertex];
}

int Coloration::firstAvailableColor(int vertex) const
{
    std::vector<bool> usedColors(numColors, false);
    for (int j : graph.getNeighbors(vertex))
    {
        if (colors[j] != -1)
        {
            usedColors[colors[j]] = true;
        }
    }
    for (int color = 0; color < numColors; color++)
    {
        if (!usedColors[color])
        {
            return color;
        }
    }
    return -1; // No available color
}

void Coloration::printColoration() const
{
    for (int i = 0; i < graph.getNumVertices(); i++)
    {
        std::cout << "Vertex " << i << ": Color " << colors[i] << std::endl;
    }
}

int Coloration::getConflictCount() const
{
    return _conflictCount / 2;
}

int Coloration::getPenalisedConflictCount() const
{
    int conflictCount = 0;
    for (int i = 0; i < graph.getNumVertices(); i++)
    {
        conflictCount += graph.getDegree(i) * getConflictCountPerVertex(i);
    }
    return conflictCount / 2;
}

int Coloration::DSAT(int vertex) const
{
    std::vector<bool> usedColors(numColors, false);
    int dsat = 0;
    for (int j : graph.getNeighbors(vertex))
    {
        if (colors[j] != -1)
        {
            if (!usedColors[colors[j]])
            {
                dsat++;
            }
            usedColors[colors[j]] = true;
        }
    }

    return dsat;
}

int Coloration::getMaxDSAT(std::list<int> vertexes) const
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
                int deg = graph.getDegree(i);
                if (deg > maxDeg)
                {
                    maxDeg = deg;
                    argmaxDSAT = i;
                }
            }
            else if (dsat > maxDSAT)
            {
                maxDSAT = dsat;
                maxDeg = graph.getDegree(i);
                argmaxDSAT = i;
            }
        }
    }

    return argmaxDSAT;
}

int Coloration::getMaxDSAT() const
{
    std::list<int> uncoloredVertexes = {};
    for (int i = 0; i < graph.getNumVertices(); i++)
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
void Coloration::fillColorationWithMinConflict()
{
    std::list<vector<int>> vertexToColor = {};
    for (int i = 0; i < graph.getNumVertices(); i++)
    {
        if (getColor(i) == -1)
        {
            vertexToColor.push_back({graph.getDegree(i), i});
        }
    }
    vertexToColor.sort([](const vector<int> &a, const vector<int> &b)
                       {
                           return a[0] < b[0]; // Degré croissant
                       });

    for (auto vertDeg : vertexToColor)
    {
        int vert = vertDeg[1];
        if (getColor(vert) == -1)
        {
            vector<int> colorCounts(numColors, 0);
            for (auto neighbor : graph.getNeighbors(vert))
            {
                int neighborColor = getColor(neighbor);
                if (neighborColor != -1)
                {
                    colorCounts[neighborColor]++;
                }
            }
            int minColor = graph.getNumVertices();
            for (auto colorCount : colorCounts)
            {
                if (colorCount < minColor)
                    minColor = colorCount;
            }
            setColor(vert, minColor);
        }
    }
}

Coloration Coloration::copy() const
{
    Coloration newColoration(graph, numColors);
    newColoration.colors = colors;
    newColoration.conflictCountsPerVertex = conflictCountsPerVertex;
    newColoration._conflictCount = _conflictCount;
    return newColoration;
}

/**
 * Retourne une coloration voisine de la coloration actuelle en changeant la couleur d'un sommet choisi aléatoirement.
 * La nouvelle couleur est choisie aléatoirement mais elle doit être différente de sorte à produire une solution différente.
 */
Coloration Coloration::getRandomNeighborColorationFromV1() const
{
    Coloration newColoration = copy();

    int n = graph.getNumVertices();
    int randomVertex = std::rand() % n;
    int randomColor = std::rand() % numColors;
    int currentColor = newColoration.getColor(randomVertex);

    while (randomColor == currentColor)
    {
        randomColor = std::rand() % numColors;
    }

    newColoration.setColor(randomVertex, randomColor);

    return newColoration;
}

Coloration Coloration::getRandomNeighborColorationFromV1(const Tabou &tabou, long long currentTime) const
{
    Coloration newColoration = copy();

    int n = graph.getNumVertices();
    int randomVertex = std::rand() % n;
    int randomColor = std::rand() % numColors;
    int currentColor = newColoration.getColor(randomVertex);

    while (randomColor == currentColor || tabou.isTabou(randomVertex, randomColor, currentTime))
    {
        currentTime = getTimeInMs();
        randomVertex = std::rand() % n;
        randomColor = std::rand() % numColors;
    }

    newColoration.setColor(randomVertex, randomColor);

    // currentTime = getTimeInMs();
    // tabou.addToTabou(randomVertex, currentColor, currentTime);

    return newColoration;
}

/**
 * Retourne une coloration voisine de la coloration actuelle en échangeant les couleurs
 * de 2 sommets aléatoires dont les couleurs sont différentes de sorte à produire une solution différente.
 */
Coloration Coloration::getRandomNeighborColorationFromV2() const
{
    Coloration newColoration = copy();

    int n = graph.getNumVertices();
    int randomVertex1 = std::rand() % n;
    int currentColor1 = newColoration.getColor(randomVertex1);
    int randomVertex2 = std::rand() % n;
    int currentColor2 = newColoration.getColor(randomVertex2);

    while (currentColor2 == currentColor1)
    {
        randomVertex2 = std::rand() % n;
        currentColor2 = newColoration.getColor(randomVertex2);
    }

    newColoration.setColor(randomVertex1, currentColor2);
    newColoration.setColor(randomVertex2, currentColor1);

    return newColoration;
}

Coloration Coloration::getRandomNeighborColorationFromV2(const Tabou &tabou, long long currentTime) const
{
    Coloration newColoration = copy();

    int n = graph.getNumVertices();
    int randomVertex1 = std::rand() % n;
    int currentColor1 = newColoration.getColor(randomVertex1);
    int randomVertex2 = std::rand() % n;
    int currentColor2 = newColoration.getColor(randomVertex2);

    while (currentColor2 == currentColor1
        || tabou.isTabou(randomVertex2, currentColor1, currentTime)
        || tabou.isTabou(randomVertex1, currentColor2, currentTime))
    {
        currentTime = getTimeInMs();
        randomVertex1 = std::rand() % n;
        currentColor1 = newColoration.getColor(randomVertex1);
        randomVertex2 = std::rand() % n;
        currentColor2 = newColoration.getColor(randomVertex2);
    }

    newColoration.setColor(randomVertex1, currentColor2);
    newColoration.setColor(randomVertex2, currentColor1);

    // currentTime = getTimeInMs();
    // tabou.addToTabou(randomVertex1, currentColor1, currentTime);
    // tabou.addToTabou(randomVertex2, currentColor2, currentTime);

    return newColoration;
}

/**
 * Retourne une coloration voisine de la coloration actuelle en échangeant les couleurs
 * de 3 sommets aléatoires dont les couleurs sont toutes différentes de sorte à produire une solution différente.
 */
Coloration Coloration::getRandomNeighborColorationFromV3() const
{
    Coloration newColoration = copy();
    int n = graph.getNumVertices();

    int randomVertex1 = std::rand() % n;
    int currentColor1 = newColoration.getColor(randomVertex1);
    int randomVertex2 = std::rand() % n;
    int currentColor2 = newColoration.getColor(randomVertex2);
    int randomVertex3 = std::rand() % n;
    int currentColor3 = newColoration.getColor(randomVertex3);

    while (currentColor2 == currentColor1 || currentColor2 == currentColor3 || currentColor3 == currentColor1)
    {
        if (currentColor2 == currentColor1)
        {
            randomVertex2 = std::rand() % n;
            currentColor2 = newColoration.getColor(randomVertex2);
        }
        if (currentColor1 == currentColor3 || currentColor2 == currentColor3)
        {
            randomVertex3 = std::rand() % n;
            currentColor3 = newColoration.getColor(randomVertex3);
        }
    }

    newColoration.setColor(randomVertex1, currentColor2);
    newColoration.setColor(randomVertex2, currentColor3);
    newColoration.setColor(randomVertex3, currentColor1);

    return newColoration;
}

Coloration Coloration::getRandomNeighborColorationFromV3(const Tabou &tabou, long long currentTime) const
{
    Coloration newColoration = copy();
    int n = graph.getNumVertices();

    int randomVertex1 = std::rand() % n;
    int currentColor1 = newColoration.getColor(randomVertex1);
    int randomVertex2 = std::rand() % n;
    int currentColor2 = newColoration.getColor(randomVertex2);
    int randomVertex3 = std::rand() % n;
    int currentColor3 = newColoration.getColor(randomVertex3);

    while (currentColor2 == currentColor1
        || currentColor2 == currentColor3
        || currentColor3 == currentColor1
        || tabou.isTabou(randomVertex1, currentColor2, currentTime)
        || tabou.isTabou(randomVertex2, currentColor3, currentTime)
        || tabou.isTabou(randomVertex3, currentColor1, currentTime)
    )
    {
        currentTime = getTimeInMs();
        randomVertex1 = std::rand() % n;
        currentColor1 = newColoration.getColor(randomVertex1);
        randomVertex2 = std::rand() % n;
        currentColor2 = newColoration.getColor(randomVertex2);
        randomVertex3 = std::rand() % n;
        currentColor3 = newColoration.getColor(randomVertex3);
    }

    newColoration.setColor(randomVertex1, currentColor2);
    newColoration.setColor(randomVertex2, currentColor3);
    newColoration.setColor(randomVertex3, currentColor1);

    // currentTime = getTimeInMs();
    // tabou.addToTabou(randomVertex1, currentColor1, currentTime);
    // tabou.addToTabou(randomVertex2, currentColor2, currentTime);
    // tabou.addToTabou(randomVertex3, currentColor3, currentTime);

    return newColoration;
}

Coloration &Coloration::operator=(const Coloration &other)
{
    if (this != &other)
    {
        this->numColors = other.numColors;
        this->colors = other.colors;
        this->conflictCountsPerVertex = other.conflictCountsPerVertex;
        this->_conflictCount = other._conflictCount;
    }
    return *this;
}

Coloration::~Coloration()
{
    colors.clear();
    conflictCountsPerVertex.clear();
}

int Coloration::getNumColors() const
{
    return numColors;
}

int Coloration::getNumVertices() const
{
    return graph.getNumVertices();
}
