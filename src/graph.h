#pragma once
#include <vector>
#include <list>
#include <iostream>
using namespace std;

class Graph {
public: 
    Graph(std::vector<std::vector<bool>> matrix) {
        incidenceMatrix = matrix;
        adjacencyList.resize(matrix.size());
        degrees.resize(matrix.size(), 0);
        n = matrix.size();
        m = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j]) {
                    m++;
                    adjacencyList[i].push_back(j);
                    degrees[i]++;}
            }
        }

    }

    int getNumVertices() const {
        return n;
    }

    int getNumEdges() const {
        return m;
    }

    Graph(std::vector<std::vector<bool>> matrix, int n, int m) {
        incidenceMatrix = matrix;
        adjacencyList.resize(n);
        degrees.resize(n, 0);
        this->n = n;
        this->m = m;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j]) {
                    adjacencyList[i].push_back(j);
                    degrees[i]++;}
            }
        }
    };

    bool isLinked(int i, int j) const {
        if (i < 0 || i >= incidenceMatrix.size() || j < 0 || j >= incidenceMatrix.size()) {
            return false; // Out of bounds
        }
        return incidenceMatrix[i][j] || incidenceMatrix[j][i];
    }

    const std::list<int>& getNeighbors(int vertex) const
    {
        return adjacencyList[vertex];
    }

    int getDegree(int vertex) const {
        return degrees[vertex];
    }

    ~Graph() {
        incidenceMatrix.clear();
        degrees.clear();
        adjacencyList.clear();
    }
private:
    std::vector<std::vector<bool>> incidenceMatrix;
    std::vector<std::list<int>> adjacencyList;
    std::vector<int> degrees;
    int n; // Number of vertices
    int m; // Number of edges
};