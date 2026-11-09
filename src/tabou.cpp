#include "tabou.h"
#include "coloration.h"


Tabou::Tabou(Coloration coloration, long long tabouCooldown, long long currentTime)
{
    n = coloration.getNumVertices();
    numColors = coloration.getNumColors();
    this->tabouCooldown = tabouCooldown;
    tabouList = std::vector<std::vector<long long>>(n, std::vector<long long>(numColors, currentTime));
    // for (int vertex = 0; vertex < n; vertex++) {
    //     int color = coloration.getColor(vertex);
    //     if (color != -1) {
    //         addToTabou(vertex, color, currentTime);
    //     }
    // }
}

void Tabou::addToTabou(int vertex, int color, long long time)
{
    if (vertex < 0 || vertex >= n || color < 0 || color >= numColors) {
        throw std::out_of_range("Vertex or color out of range");
    }
    tabouList[vertex][color] = tabouCooldown + time;
}

bool Tabou::isTabou(int vertex, int color, long long time) const
{
    if (vertex < 0 || vertex >= n || color < 0 || color >= numColors) {
        throw std::out_of_range("Vertex or color out of range");
    }
    return tabouList[vertex][color] > time;
}

void Tabou::addDiffToTabou(const Coloration& oldColoration, const Coloration& newColoration, long long time)
{
    int n = oldColoration.getNumVertices();
    for (int vertex = 0; vertex < n; vertex++)
    {
        int oldColor = oldColoration.getColor(vertex);
        int newColor = newColoration.getColor(vertex);
        if (oldColor != newColor)
        {
            addToTabou(vertex, oldColor, time);
        }
    }
}
