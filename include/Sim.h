#pragma once

#include <vector>
#include <cstdint>

class ThinDrawer;
class CircuitUnit;

typedef struct grid_vector
{
    int x;
    int y;
} grid_vector;

class Sim
{
public:
    static const int gridSizeX = 100;
    static const int gridSizeY = 100;

    static const int midPosX = gridSizeX / 2;
    static const int midPosY = gridSizeY / 2;

    grid_vector* startPos = NULL;
    int isHoriz = 1;

    ThinDrawer* td;
    CircuitUnit* grid[gridSizeY][gridSizeX];
    CircuitUnit* dotGrid[gridSizeY][gridSizeX];
    std::vector<CircuitUnit*> units;
    Sim();

    void lupp();
    void ioH();
    void surfaceRecreation();
    void fillGrid(CircuitUnit* ptr, grid_vector position, grid_vector size);
};