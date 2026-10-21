#pragma once
#include <vector>
#include <string>
#include <fstream>
#include "graph.h"

Graph parseGraphFromFile(const std::string& filename);

const std::vector<std::string> instanceFilenames = {
    "instances/dsjc125.1.col",
    "instances/dsjc125.9.col",
    "instances/dsjc250.1.col",
    "instances/dsjc250.5.col",
    "instances/dsjc250.9.col",
    "instances/flat300_26_0.col",
    "instances/le450_15c.col",
    "instances/dsjc1000.5.col",
    "instances/dsjc1000.9.col",
    "instances/C2000.5.col",
    "instances/C4000.5.col"
};
    
const std::vector<std::vector<int>> k = {
    {5},
    {44, 22},
    {8},
    {28},
    {72, 36},
    {26, 13},
    {15, 8},
    {86, 85, 84, 42},
    {224, 223, 222, 111},
    {147, 146, 73},
    {261, 260, 130}
};