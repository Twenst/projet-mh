#include "parser.h"
#include <regex>

Graph parseGraphFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }

    int n = 0;
    int m = 0;
    std::vector<std::vector<bool>> incidenceMatrix;

    std::string line;
    while (std::getline(file, line)) {
        // chaque ligne commencant avec un c est un commentaire -> skip
        if (line[0] == 'p') {
            // ligne de type "p edge %d %d"
            std::regex pattern(R"(p [^0-9]+ (\d+) (\d+))");
            std::smatch matches;
            if (std::regex_search(line, matches, pattern)) {
                n = std::stoi(matches[1]);
                m = std::stoi(matches[2]);
                incidenceMatrix.resize(n, std::vector<bool>(n, false)); // Initialize the incidence matrix with false
            }
        }
        if (line[0] == 'e') {
            // ligne de type "e %d %d"
            std::regex pattern(R"(e (\d+) (\d+))");
            std::smatch matches;
            if (std::regex_search(line, matches, pattern)) {
                int u = std::stoi(matches[1]);
                int v = std::stoi(matches[2]);
                incidenceMatrix[u - 1][v - 1] = true;
                incidenceMatrix[v - 1][u - 1] = true;
                if (u > n || v > n) {
                    throw std::runtime_error("Vertex index out of bounds in file: " + filename);
                }
            }
        }
    }

    return Graph(incidenceMatrix, n, m);
}
