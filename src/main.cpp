#include <iostream>
#include "parser.h"
#include "solver.h"
#include "results.h"
#include "clock.h"

void compareHeuristiques()
{
    for (int instanceNumber = 0; instanceNumber < instanceFilenames.size(); instanceNumber++)
    {
        const std::string filename = instanceFilenames[instanceNumber];
        try {
            Graph graph = parseGraphFromFile(filename);
            // std::cout << "Graph parsed successfully from file: " << filename << std::endl;
            
            for (int numColors : k[instanceNumber])
            {

                int avgConflictCount1 = 0;
                double avgDuration1 = 0;
                for (int i = 0; i < 10; i++)
                {

                    long long startTime = getTimeInMs();
                    Coloration coloration3 = randomSolve(graph, numColors);
                    double duration3 = double(getTimeInMs() - startTime)/1000;
                    avgConflictCount1 += coloration3.getConflictCount()/10;
                    avgDuration1 += duration3/10;
                }

                std::cout << filename << " & " << numColors << " & " << avgConflictCount1 << " & " << avgDuration1 << " \\\\ \\hline" << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing graph from file: " << filename << ". Error: " << e.what() << std::endl;
        }
    }
}

void testMetaheuristique()
{    
    for (int instanceNumber = 2; instanceNumber < 6; instanceNumber++)
    {
        
        const std::string filename = instanceFilenames[instanceNumber];
        Graph graph = parseGraphFromFile(filename);
        try {
            std::cout << "Graph parsed successfully from file: " << filename << std::endl;  
            vector<int> numsColors = k[instanceNumber];   
            // numsColors.erase(numsColors.begin());
            for (int numColors: numsColors)
            {
                int avgConflictCount1 = 0;
                int bestConflictCount1 = RAND_MAX;
                for (int i = 0; i < 10; i++)
                {
                    srand48(getTimeInMs());
                    Result result = SimulatedAnnealing2(
                        graph, numColors, 100, 0.999, 5*60*1000);
                        
                    avgConflictCount1 += result.getConflictCount()/10;
                    if (result.getConflictCount() < bestConflictCount1) {
                        bestConflictCount1 = result.getConflictCount();
                    }
                    result.setInstanceName(filename);
                    result.checkIfBetterAndSave();
                }
                std::cout << filename << " & " << numColors << " & " << avgConflictCount1 << " & " << bestConflictCount1 << " \\\\ \\hline" << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error parsing graph from file: " << filename << ". Error: " << e.what() << std::endl;
        }
    }
}


int main() {
    testMetaheuristique();

    return 0;
}