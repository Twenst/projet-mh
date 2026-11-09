#pragma once
#include "coloration.h"
#include "results.h"

Coloration heuristicSolve(const Graph& graph, int numColors);
Coloration DSATUR(const Graph& graph, int numColors);
Coloration randomSolve(const Graph& graph, int numColors);
Coloration VariableNeighborSearchStochastic(const Graph& graph, int numColors, int sampleSize, int maxTime);
Coloration SimulatedAnnealing(const Graph& graph, int numColors, float initTemp, float decay, int maxTime);
Result SimulatedAnnealing2(const Graph& graph, int numColors, float initTemp, float decay, int maxTime);
Coloration VariableNeighborSearchStochasticSimulatedAnnealing(const Graph& graph, int numColors, int sampleSize, float decay, int maxTime);
Coloration TabouSearch(const Graph& graph, int numColors, int tabouCooldown, int maxTime);
Result VariableNeighborSearchStochasticSimulatedAnnealing2(const Graph& graph, int numColors, int initSampleSize, float decay, int maxTime, long long tabouCooldown);

Coloration getBestFromVK(
    const Coloration& col,
    int k,
    int sampleSize,
    const Tabou& tabou,
    long long currentTime);
