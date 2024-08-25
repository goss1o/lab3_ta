#pragma once

#include "../own_system.h"
#include <iostream>
#include <fstream>

enum class DIRECTIONS {
    RIGHT,
    LEFT,
    UP,
    DOWN,
};

class Labyrinth
{
private:
    own_types::MultiArray<own_types::CELL_VALUE> mapa;
    own_types::MultiArray<int> scanned;

    size_t robot_x = 0;
    size_t robot_y = 0;
    size_t robot_drones = 0;
    void step(DIRECTIONS d);
    void test_alive();
public:
    Labyrinth(std::istream& input);
    Labyrinth(const std::string& filename);

    void moveRobotRight(int distance);
    void moveRobotLeft(int distance);
    void moveRobotUp(int distance);
    void moveRobotDown(int distance);

    own_types::Array<own_types::subarray<own_types::CELL_VALUE>> sendDrones(int count);
    int getDronesCount();

    void print();

    ~Labyrinth() = default;
};
