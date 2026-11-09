#pragma once
#include <vector>

class Coloration;

class Tabou
{
public:
    Tabou(Coloration coloration, long long tabouCooldown, long long currentTime);

    void addToTabou(int vertex, int color, long long time);

    bool isTabou(int vertex, int color, long long time) const;

    void addDiffToTabou(const Coloration& oldColoration, const Coloration& newColoration, long long time);

private:
    int n;
    int numColors;
    long long tabouCooldown;
    std::vector<std::vector<long long>> tabouList;
};