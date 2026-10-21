#pragma once
#include <vector>
#include <iostream>
using namespace std;

class Graph {
public:
    Graph(std::vector<std::vector<bool>> matrix) {
        incidenceMatrix = matrix;
        n = matrix.size();
        m = 0;
        for (const auto& row : matrix) {
            for (bool val : row) {
                if (val) {
                    m++;
                }
            }
        }
    };
    Graph(std::vector<std::vector<bool>> matrix, int n, int m) {
        incidenceMatrix = matrix;
        this->n = n;
        this->m = m;
    };
    bool isLinked(int i, int j) {
        if (i < 0 || i >= incidenceMatrix.size() || j < 0 || j >= incidenceMatrix.size()) {
            return false; // Out of bounds
        }
        return incidenceMatrix[i][j];
    }
    int n; // Number of vertices
    int m; // Number of edges 

    void printIncidenceMatrix() {
        for (const auto& row : incidenceMatrix) {
            for (bool val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
private:
    std::vector<std::vector<bool>> incidenceMatrix;
};