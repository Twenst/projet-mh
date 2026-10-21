#include <iostream>
#include "parser.h"

int main() {
    const std::string filename = instanceFilenames[0];
    try {
        Graph graph = parseGraphFromFile(filename);
        std::cout << "Graph parsed successfully from file: " << filename << std::endl;
        std::cout << "Number of vertices: " << graph.n << std::endl;
        std::cout << "Number of edges: " << graph.m << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}