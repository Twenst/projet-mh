#include <iostream>
#include "parser.h"
#include "solver.h"

int main() {
    int instanceNumber = 6;
    const std::string filename = instanceFilenames[instanceNumber];
    try {
        Graph graph = parseGraphFromFile(filename);
        std::cout << "Graph parsed successfully from file: " << filename << std::endl;
        
        int numColors = k[instanceNumber][0]; // Premiere valeur de k pour le premier graphe
        Coloration coloration = DSATUR(graph, numColors);
        std::cout << "Coloration completed successfully." << std::endl;
        // coloration.printColoration();
        std::cout << "Is valid partial coloring: " << (coloration.isValidPartialColoring() ? "Yes" : "No") << std::endl;
        std::cout << "Is valid coloring: " << (coloration.isValidColoring() ? "Yes" : "No") << std::endl;
        std::cout << "Number of conflicts: " << coloration.getConflitNumber() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}