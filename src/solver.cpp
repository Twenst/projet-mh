#include "solver.h"
#include <list>
#include <algorithm>
#include <iostream>
#include "clock.h"

Coloration heuristicSolve(Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);
    std::list<int> vertexToColor = {};
    for (int i = 0; i < graph.n; ++i)
    {
        vertexToColor.push_back(i);
    }

    // Pour parcourir toutes les composantes connexes du graphe
    while (!vertexToColor.empty())
    {
        int vertex = vertexToColor.back();
        vertexToColor.pop_back();

        std::list<int> vertexesAccessible;
        vertexesAccessible.push_back(vertex);

        // Parcours en profondeur
        while (!vertexesAccessible.empty())
        {
            int currentVertex = vertexesAccessible.back();
            vertexesAccessible.pop_back();

            int color = coloration.firstAvailableColor(currentVertex);
            if (color != -1)
            {
                coloration.setColor(currentVertex, color);
            }
            vertexToColor.remove(currentVertex);
            for (auto& neighbor : graph.getNeighbors(currentVertex))
            {
                auto it = std::find(vertexToColor.begin(), vertexToColor.end(), neighbor); // si neighbor est tjrs à colorer
                if (coloration.getColor(neighbor) == -1 && it != vertexToColor.end())
                {
                    vertexesAccessible.push_back(neighbor);
                }
            }
        }
    }

    coloration.fillColorationWithMinConflict();

    return coloration;
}

Coloration DSATUR(Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);

    std::list<int> vertexList = {};
    std::list<vector<int>> vertexToColor = {};
    for (int i = 0; i < graph.n; ++i)
    {
        vertexList.push_back(i);
        int deg = graph.getNeighbors(i).size();
        vertexToColor.push_back({deg, i});
    }
    vertexToColor.sort([](const vector<int>& a, const vector<int>& b) {
        return a[0] > b[0]; // Degré décroissant
    });

    int maxDegreeVertex = vertexToColor.front()[1];
    coloration.setColor(maxDegreeVertex, 0);
    vertexList.remove(maxDegreeVertex);

    while (!vertexList.empty())
    {
        int vertex = coloration.getMaxDSAT(vertexList);
        vertexList.remove(vertex);

        int color = coloration.firstAvailableColor(vertex);
        if (color != -1) coloration.setColor(vertex, color);
    }

    coloration.fillColorationWithMinConflict();

    return coloration;
}


/** Retourne la meilleure soluation voisine dans le `k`-ème voisinage parmi un échantillonage
* aléatoire du voisinage de taille `sample_size`. */
Coloration getBestFromVK(Coloration col, int k, int sampleSize)
{
    switch (k)
    {
    case 0:
    {
        Coloration bestColoration = col.getRandomNeighborColorationFromV1();
        int bestConflictCount = bestColoration.getConflitCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflitCount();

            if (bestConflictCount > colTestConflictCount)
            {
                bestColoration = colTest;
                bestConflictCount = colTestConflictCount;
            }
        }
        return bestColoration;
    }
    case 1:
    {
        Coloration bestColoration = col.getRandomNeighborColorationFromV2();
        int bestConflictCount = bestColoration.getConflitCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflitCount();

            if (bestConflictCount > colTestConflictCount)
            {
                bestColoration = colTest;
                bestConflictCount = colTestConflictCount;
            }
        }
        return bestColoration;
    }
    case 2:
    {
        Coloration bestColoration = col.getRandomNeighborColorationFromV3();
        int bestConflictCount = bestColoration.getConflitCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflitCount();

            if (bestConflictCount > colTestConflictCount)
            {
                bestColoration = colTest;
                bestConflictCount = colTestConflictCount;
            }
        }
        return bestColoration;
    }
    default:
        throw "Voisinage invalide";
    }

}

Coloration VariableNeighborSearchStochastic(Graph& graph, int numColors, int sampleSize, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    int currentConflictCount = currentSolution.getConflitCount();
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    int kMax = 2;

    while (getTimeInMs() - startTimeMs < maxTime)
    {
        int k = 0;
        while (k <= kMax)
        {
            Coloration testSolution = getBestFromVK(currentSolution, k, sampleSize);
            exploredSolutionsSinceLastUpgrade += sampleSize;
            int testConflictCount = testSolution.getConflitCount();

            if (testConflictCount < currentConflictCount)
            {
                currentSolution = testSolution;
                currentConflictCount = testConflictCount;
                int currentTime = getTimeInMs();
                cout << "Best conflict count = " << currentConflictCount << " (Took " << (currentTime - lastTimeUpgrade) << " ms and " << exploredSolutionsSinceLastUpgrade << " solutions explored, k=" << k <<".)" <<endl;
                lastTimeUpgrade = currentTime;
                exploredSolutionsSinceLastUpgrade = 0;
                k = 0;
            }
            else k++;
        }
    }

    return currentSolution;
}

Coloration SimulatedAnnealing(Graph& graph, int numColors, float initTemp, float decay, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflitCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    float temp = initTemp;

    while (getTimeInMs() - startTimeMs < maxTime)
    {
        int palierSize = 30;
        for (int i = 0; i < palierSize; i++)
        {    
            Coloration testSolution = currentSolution.getRandomNeighborColorationFromV1();
            exploredSolutionsSinceLastUpgrade++;
            int testConflictCount = testSolution.getConflitCount();

            if (testConflictCount < currentConflictCount)
            {
                if (testConflictCount < bestConflictCount)
                {
                    bestSolution = testSolution;
                    bestConflictCount = testConflictCount;
                    int currentTime = getTimeInMs();
                    cout << "Best conflict count = " << bestConflictCount << " (Took " << (currentTime - lastTimeUpgrade) << " ms and " << exploredSolutionsSinceLastUpgrade << " solutions explored, Temp = " << temp << ".)" <<endl;
                    lastTimeUpgrade = currentTime;
                    exploredSolutionsSinceLastUpgrade = 0;
                }
                currentSolution = testSolution;
                currentConflictCount = testConflictCount;
            }
            else 
            {
                int delta = testConflictCount - currentConflictCount;
                double prob = exp(delta/temp);

                if (rand()/RAND_MAX < prob)
                {
                    currentSolution = testSolution;
                    currentConflictCount = testConflictCount;
                }
            }
        }
        temp *= decay;
    }

    return bestSolution;
}

/** Mix entre VNS Stochastique avec du recuit simulé,
 * la température augmente lorsque l'on reste bloqué dans le dernier voisinage pendant longtemps*/
Coloration VariableNeighborSearchStochasticSimulatedAnnealing(Graph& graph, int numColors, int initSampleSize, float decay, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflitCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    float temp = 0;
    int kMax = 2;
    int sampleSize = initSampleSize;

    int k = 0;
    while (getTimeInMs() - startTimeMs < maxTime)
    {
        Coloration testSolution = getBestFromVK(currentSolution, k, sampleSize);
        exploredSolutionsSinceLastUpgrade += sampleSize;
        int testConflictCount = testSolution.getConflitCount();

        if (testConflictCount < currentConflictCount)
        {
            if (testConflictCount < bestConflictCount)
            {
                bestSolution = testSolution;
                bestConflictCount = testConflictCount;
                int currentTime = getTimeInMs();
                cout << "Best conflict count = " << bestConflictCount << " (Took " << (currentTime - lastTimeUpgrade) << " ms and " << exploredSolutionsSinceLastUpgrade << " solutions explored, k="<<k<<", Temp=" <<temp<< ".)" <<endl;
                lastTimeUpgrade = currentTime;
                exploredSolutionsSinceLastUpgrade = 0;
                temp = 0;
                k = 0;
                sampleSize = initSampleSize;
            }
            else
            {
                cout << "Current conflict count decreased = " << currentConflictCount << " (Best=" << bestConflictCount<<", k="<<k<<", Temp=" <<temp<< ".)" <<endl;
            }
            currentSolution = testSolution;
            currentConflictCount = testConflictCount;
        }
        else 
        {
            if (temp > .1)
            {
                int delta = testConflictCount - currentConflictCount;
                double prob = exp(delta/temp);

                if (rand()/RAND_MAX < prob)
                {
                    currentSolution = testSolution;
                    currentConflictCount = testConflictCount;
                    cout << "Current conflict count increased = " << currentConflictCount << " (Best=" << bestConflictCount<<", k="<<k<<", Temp=" <<temp<< ".)" <<endl;
                }
            }
            if (k == kMax)
            {
                temp += 5;
                sampleSize++;
            }
            else k++;
        }

        temp *= decay;
        
    }

    return bestSolution;
}