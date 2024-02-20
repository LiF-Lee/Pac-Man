#pragma once
#include <Windows.h>
#include <vector>

using VectorMap = std::vector<std::vector<int>>;

enum
{
    DIAMETER = 30
};

enum Type
{
    None = 0,
    Wall = 1,
    TransparentWall = 2,
    Pellet = 3,
    PowerPellet = 4
};

class GameMap
{
    int width, height;
    VectorMap mapData;
public:     
    GameMap() : width(0), height(0) {}
    
    void LoadLevel_1();

    int GetWidth() const { return width; }
    int GetHeight() const { return height; }
    VectorMap& GetMapData() { return mapData; }
    
    void Paint(const HDC& hdc) const;
};
