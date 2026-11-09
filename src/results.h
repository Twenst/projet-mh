#pragma once
#include <string>
#include <fstream>
#include "coloration.h"

class Result
{
public:
    Result(const std::string &_instanceName,
           int _numColors, const Coloration &_coloration,
           long long _timeTaken, long long _exploredSolutions) : coloration(_coloration)
    {
        instanceName = _instanceName;
        numColors = _numColors;
        timeTaken = _timeTaken;
        conflictCount = _coloration.getConflictCount();
        exploredSolutions = _exploredSolutions;
    }

    std::string getInstanceName() const { return instanceName; }
    void setInstanceName(const std::string &name) { instanceName = name; }
    int getNumColors() const { return numColors; }
    const Coloration &getColoration() const { return coloration; }
    long long getTimeTaken() const { return timeTaken; }
    int getConflictCount() const { return conflictCount; }
    long long getExploredSolutions() const { return exploredSolutions; }
    void saveToFile() const
    {
        const std::string &cleanedInstanceName = instanceName.substr(instanceName.find_last_of("/\\") + 1); // Remove path
        const std::string &filename = "results/" + cleanedInstanceName + "_" + std::to_string(numColors) + ".txt";
        std::ofstream file(filename);
        if (file.is_open())
        {
            file << "Instance: " << instanceName << "\n";
            file << "Colors: " << numColors << "\n";
            file << "Conflicts: " << conflictCount << "\n";
            file << "Time (ms): " << timeTaken << "\n";
            file.close();
        }
    }

    void checkIfBetterAndSave() const
    {
        const std::string &cleanedInstanceName = instanceName.substr(instanceName.find_last_of("/\\") + 1); // Remove path
        const std::string &filename = "results/" + cleanedInstanceName + "_" + std::to_string(numColors) + ".txt";

        std::ifstream file(filename);
        if (!file.is_open())
        {
            saveToFile();
            return;
        }

        int existingConflicts = -1;
        std::string line;
        while (std::getline(file, line))
        {
            if (line.find("Conflicts:") != std::string::npos)
            {
                existingConflicts = std::stoi(line.substr(line.find(":") + 1));
                break;
            }
        }

        if (existingConflicts == -1 || existingConflicts > conflictCount)
        {
            cout << "New best result for instance " << instanceName << " with " << numColors << " colors: " << conflictCount << " conflicts. Saving to file." << endl;
            saveToFile();
        }
    }

private:
    std::string instanceName;
    int numColors;
    Coloration coloration;
    long long timeTaken; // in milliseconds
    int conflictCount;
    long long exploredSolutions;
};
