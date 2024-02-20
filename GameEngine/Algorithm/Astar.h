#pragma once

#include <fstream>
#include <stack>
#include <list>
#include <queue>
#include <string>

#include "Node.h"
#include "../GameMap.h"
#include "Point.h"
using namespace std;

class Astar
{
    GameMap* mapData;
    int** tempMapData;
    bool** visitInfo;
    Point** parent;
    stack<Point> path;
    list<Point> visitNode;
    priority_queue<Node> pq;
    bool found;
public:
    constexpr static Point directions[4] = {
        { 0, -1 },
        { 0, 1 },
        { -1, 0 },
        { 1, 0 }
    };
    
    Astar(GameMap* map) : mapData(map), found(false)
    {
        const int height = map->GetHeight();
        const int width = map->GetWidth();

        tempMapData = new int * [height];
        visitInfo = new bool * [height];
        parent = new Point * [height];

        for (int i = 0; i < height; i++)
        {
            tempMapData[i] = new int[width];
            visitInfo[i] = new bool[width];
            parent[i] = new Point[width];

            fill_n(tempMapData[i], width, INT_MAX);
            fill_n(visitInfo[i], width, false);
        }
    }
    
    ~Astar()
    {
        for (int i = 0; i < mapData->GetHeight(); i++)
        {
            delete[] tempMapData[i];
            delete[] visitInfo[i];
            delete[] parent[i];
        }
        delete[] tempMapData;
        delete[] visitInfo;
        delete[] parent;
    }

    void RelaxEdges(const Node& current, int destX, int destY);
    bool FindPath(int sx, int sy, int dx, int dy);
    stack<Point> GetPath();
};
