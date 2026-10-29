#include <iostream>
#include "parser.h"
#include "solver.h"

int main() {
    int instanceNumber = 1;
    const std::string filename = instanceFilenames[instanceNumber];
    try {
        Graph graph = parseGraphFromFile(filename);
        std::cout << "Graph parsed successfully from file: " << filename << std::endl;
        
        int numColors = k[instanceNumber][0]; // Premiere valeur de k pour le premier graphe
        // Coloration coloration = VariableNeighborSearchStochastic(graph, numColors, 20, 300*1000);
        // Coloration coloration = SimulatedAnnealing(graph, numColors, 100, 0.999, 300*1000);
        Coloration coloration = VariableNeighborSearchStochasticSimulatedAnnealing(graph, numColors, 30, 0.99, 3*60*1000);
        std::cout << "Number of conflicts: " << coloration.getConflitCount() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}