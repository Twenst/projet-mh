#include "solver.h"
#include <list>
#include <algorithm>
#include <iostream>
#include "clock.h"
#include "tabou.h"
#include "results.h"
#include <cmath>

Coloration randomSolve(const Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);

    for (int vertex = 0; vertex < graph.getNumVertices(); ++vertex)
    {
        coloration.setColor(vertex, rand() % numColors);
    }

    return coloration;
}

Coloration heuristicSolve(const Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);
    std::list<int> vertexToColor = {};
    for (int i = 0; i < graph.getNumVertices(); ++i)
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

Coloration DSATUR(const Graph& graph, int numColors)
{
    Coloration coloration(graph, numColors);

    const int n = graph.getNumVertices();
    std::vector<bool> colored(n, false);

    for (int step = 0; step < n; ++step)
    {
        int bestVertex = -1;
        int bestDSAT = -1;
        int bestDegree = -1;

        for (int vertex = 0; vertex < n; ++vertex)
        {
            if (colored[vertex])
                continue;

            int dsat = coloration.DSAT(vertex);
            int degree = graph.getDegree(vertex);

            if (dsat > bestDSAT ||
                (dsat == bestDSAT && degree > bestDegree))
            {
                bestVertex = vertex;
                bestDSAT = dsat;
                bestDegree = degree;
            }
        }

        if (bestVertex == -1)
            break;

        int color = coloration.firstAvailableColor(bestVertex);

        if (color == -1)
            color = 0;

        coloration.setColor(bestVertex, color);
        colored[bestVertex] = true;
    }

    coloration.fillColorationWithMinConflict();
    return coloration;
}


/** Retourne la meilleure soluation voisine dans le `k`-ème voisinage parmi un échantillonage
* aléatoire du voisinage de taille `sample_size`. */
Coloration getBestFromVK(const Coloration& col, int k, int sampleSize)
{
    switch (k)
    {
    case 0:
    {
        Coloration bestColoration = col.getRandomNeighborColorationFromV1();
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflictCount();

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
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflictCount();

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
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1();
            int colTestConflictCount = colTest.getConflictCount();

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

Coloration getBestFromVK(const Coloration& col, int k, int sampleSize, const Tabou& tabou, long long currentTime)
{
    switch (k)
    {
    case 0:
    {
        Coloration bestColoration = col.getRandomNeighborColorationFromV1(tabou, currentTime);
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV1(tabou, currentTime);
            int colTestConflictCount = colTest.getConflictCount();

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
        Coloration bestColoration = col.getRandomNeighborColorationFromV2(tabou, currentTime);
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV2(tabou, currentTime);
            int colTestConflictCount = colTest.getConflictCount();

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
        Coloration bestColoration = col.getRandomNeighborColorationFromV3(tabou, currentTime);
        int bestConflictCount = bestColoration.getConflictCount();

        for (int i = 0; i < sampleSize-1; i++)
        {
            Coloration colTest = col.getRandomNeighborColorationFromV3(tabou, currentTime);
            int colTestConflictCount = colTest.getConflictCount();

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

Coloration VariableNeighborSearchStochastic(const Graph& graph, int numColors, int sampleSize, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    int currentConflictCount = currentSolution.getConflictCount();
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
            int testConflictCount = testSolution.getConflictCount();

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

Coloration SimulatedAnnealing(const Graph& graph, int numColors, float initTemp, float decay, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflictCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    float temp = initTemp;

    while (getTimeInMs() - startTimeMs < maxTime)
    {
        int palierSize = 10;
        for (int i = 0; i < palierSize; i++)
        {    
            Coloration testSolution = currentSolution.getRandomNeighborColorationFromV1();
            exploredSolutionsSinceLastUpgrade++;
            int testConflictCount = testSolution.getConflictCount();

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
                int delta = - testConflictCount + currentConflictCount;
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

/** Recuit simulé avec un voisinage aléatoire à chaque itération */
Result SimulatedAnnealing2(const Graph& graph, int numColors, float initTemp, float decay, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflictCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    int totalExploredSolutions = 0;
    float temp = initTemp;

    while (getTimeInMs() - startTimeMs < maxTime && bestConflictCount > 0 && temp > 0.0001)
    {
        int palierSize = 30;
        for (int i = 0; i < palierSize; i++)
        {   
            int k = rand()%3;
            Coloration testSolution = getBestFromVK(currentSolution, k, 1); 
            exploredSolutionsSinceLastUpgrade++;
            totalExploredSolutions++;
            int testConflictCount = testSolution.getConflictCount();

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
                int delta = - testConflictCount + currentConflictCount;
                double prob = exp(delta/temp);

                if (static_cast<float>(rand()) / RAND_MAX < prob)
                {
                    currentSolution = testSolution;
                    currentConflictCount = testConflictCount;
                }
            }
        }
        temp *= decay;
    }

    Result result("", numColors, bestSolution, getTimeInMs() - startTimeMs, totalExploredSolutions);


    return result;
}

/** Mix entre VNS Stochastique avec du recuit simulé,
 * la température augmente lorsque l'on reste bloqué dans le dernier voisinage pendant longtemps*/
Coloration VariableNeighborSearchStochasticSimulatedAnnealing(const Graph& graph, int numColors, int initSampleSize, float decay, int maxTime)
{
    long long startTimeMs = getTimeInMs();

    Coloration currentSolution = DSATUR(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflictCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    float temp = 0;
    int kMax = 2;
    int sampleSize = initSampleSize;

    while (getTimeInMs() - startTimeMs < maxTime)
    {
        int k = 0;
        while (k <= kMax)
        {
            Coloration testSolution = getBestFromVK(currentSolution, k, sampleSize);
            exploredSolutionsSinceLastUpgrade += sampleSize;
            int testConflictCount = testSolution.getConflictCount();

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
                k ++;
                int delta = currentConflictCount - testConflictCount;

                if (temp > 0.1)
                {
                    double prob = exp(delta/temp);
                    if (static_cast<float>(rand()) / RAND_MAX < prob)
                    {
                        currentSolution = testSolution;
                        currentConflictCount = testConflictCount;
                        cout << "Current conflict count increased = " << currentConflictCount << " (Best=" << bestConflictCount<<", k="<<k<<", Temp=" <<temp<< ".)" <<endl;
                    }
                }
            }
        }
        temp++;
        sampleSize++;
        temp *= decay;
    }

    return bestSolution;
}

Result VariableNeighborSearchStochasticSimulatedAnnealing2(const Graph& graph, int numColors, int initSampleSize, float decay, int maxTime, long long tabouCooldown)
{
    srand48(getTimeInMs());
    Coloration currentSolution = randomSolve(graph, numColors);
    Coloration bestSolution = currentSolution;
    int currentConflictCount = currentSolution.getConflictCount();
    int bestConflictCount = currentConflictCount;
    int lastTimeUpgrade = getTimeInMs();
    int exploredSolutionsSinceLastUpgrade = 0;
    long long totalExploredSolutions = 0;
    float temp = 0;
    int kMax = 2;
    int sampleSize = initSampleSize;
    
    long long startTimeMs = getTimeInMs();
    Tabou tabou(currentSolution, tabouCooldown, getTimeInMs());

    while (getTimeInMs() - startTimeMs < maxTime && bestConflictCount > 0)
    {
        int k = 0;
        while (k <= kMax)
        {
            Coloration testSolution = getBestFromVK(currentSolution, k, sampleSize, tabou, getTimeInMs());
            tabou.addDiffToTabou(currentSolution, testSolution, getTimeInMs());
            totalExploredSolutions += sampleSize;
            exploredSolutionsSinceLastUpgrade += sampleSize;
            int testConflictCount = testSolution.getConflictCount();

            if (testConflictCount <= currentConflictCount)
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
                    // cout << "Current conflict count decreased = " << currentConflictCount << " (Best=" << bestConflictCount<<", k="<<k<<", Temp=" <<temp<< ".)" <<endl;
                }
                currentSolution = testSolution;
                currentConflictCount = testConflictCount;
            }
            else 
            {
                k ++;
                int delta = currentConflictCount - testConflictCount;
                double prob = exp(delta/temp);

                if (static_cast<float>(rand()) / RAND_MAX < prob)
                {
                    currentSolution = testSolution;
                    currentConflictCount = testConflictCount;
                    // cout << "Current conflict count increased = " << currentConflictCount << " (Best=" << bestConflictCount<<", k="<<k<<", Temp=" <<temp<< ", prob=" << prob << ")." <<endl;
                }
            }
        }
        temp++;
        sampleSize++;
        sampleSize = std::min(sampleSize, 100);
        temp *= decay;
    }

    Result result("", numColors, bestSolution, getTimeInMs() - startTimeMs, totalExploredSolutions);

    return result;
}
