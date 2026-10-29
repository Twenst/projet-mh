#pragma once
#include "coloration.h"

Coloration heuristicSolve(Graph& graph, int numColors);
Coloration DSATUR(Graph& graph, int numColors);
Coloration VariableNeighborSearchStochastic(Graph& graph, int numColors, int sampleSize, int maxTime);
Coloration SimulatedAnnealing(Graph& graph, int numColors, float initTemp, float decay, int maxTime);
Coloration VariableNeighborSearchStochasticSimulatedAnnealing(Graph& graph, int numColors, int sampleSize, float decay, int maxTime);
