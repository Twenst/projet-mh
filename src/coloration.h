#pragma once

#include <list>
#include <vector>

#include "graph.h"

class Tabou;

class Coloration
{
public:
    Coloration(const Graph& graph, int numColors);
    Coloration& operator=(const Coloration &other);
    ~Coloration();

    void setColor(int vertex, int color);
    int getColor(int vertex) const;
    int getConflictCountPerVertex(int vertex) const;
    int firstAvailableColor(int vertex) const;
    void printColoration() const;

    int getConflictCount() const;
    int getPenalisedConflictCount() const;
    int DSAT(int vertex) const;

    int getMaxDSAT(std::list<int> vertexes) const;
    int getMaxDSAT() const;
    int getMaxDSAT(const std::vector<int>& vertices) const;

    void fillColorationWithMinConflict();

    Coloration copy() const;

    Coloration getRandomNeighborColorationFromV1() const;
    Coloration getRandomNeighborColorationFromV1(
        const Tabou& tabou, long long currentTime) const;

    Coloration getRandomNeighborColorationFromV2() const;
    Coloration getRandomNeighborColorationFromV2(
        const Tabou& tabou, long long currentTime) const;

    Coloration getRandomNeighborColorationFromV3() const;
    Coloration getRandomNeighborColorationFromV3(
        const Tabou& tabou, long long currentTime) const;

    int getNumColors() const;
    int getNumVertices() const;

private:
    int numColors;
    const Graph& graph;
    std::vector<int> colors;
    std::vector<int> conflictCountsPerVertex;
    int _conflictCount;
};