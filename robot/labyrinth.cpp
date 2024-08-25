#include "labyrinth.h"
#include <algorithm>

#define MAX_DISTANCE 5

void Labyrinth::step(DIRECTIONS d) {
    switch (d)
    {
    case DIRECTIONS::RIGHT:
        ++robot_x;
        break;
    case DIRECTIONS::LEFT:
        --robot_x;
        break;
    case DIRECTIONS::UP:
        --robot_y;
        break;
    case DIRECTIONS::DOWN:
        ++robot_y;
        break;
    }
}

void Labyrinth::test_alive() {
    if (mapa[{robot_x, robot_y}] == own_types::CELL_VALUE::WALL)
        throw std::runtime_error("Dead");
}

void Labyrinth::print() {
    auto s = mapa.get_sub_dimentions({});
    std::cout << "============================"<<std::endl;
    for (size_t j = 0; j < s[1]; j++) {
        for (size_t i = 0; i < s[0]; i++) {

            if (robot_x == i && robot_y == j) {
                std::cout << "R";
                continue;
            }
            if (!scanned[{i,j}]) {
                std::cout << "U";
                continue;
            }
            switch (mapa[{i,j}])
            {
            case own_types::CELL_VALUE::UNDEFINED:
                std::cout << "U";
                break;
            case own_types::CELL_VALUE::EMPTY:
                std::cout << " ";
                break;
            case own_types::CELL_VALUE::WALL:
                std::cout << "W";
                break;
            case own_types::CELL_VALUE::EXIT:
                std::cout << "E";
                break;
            default:
                break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << "============================"<<std::endl;
}

Labyrinth::Labyrinth(std::istream &input) : mapa({1, 1}), scanned({1,1}) {
    size_t X_SIZE, Y_SIZE;
    input >> X_SIZE >> Y_SIZE;
    input >> robot_x >> robot_y >> robot_drones;

    mapa = own_types::MultiArray<own_types::CELL_VALUE>({X_SIZE, Y_SIZE});
    scanned = own_types::MultiArray<int>({X_SIZE, Y_SIZE});

    char INP;
    for (size_t y = 0; y < Y_SIZE; y++) {
        for (size_t x = 0; x < X_SIZE; x++) {
            input >> INP;
            switch (INP) {
            case 'W':
                mapa[{x,y}] = own_types::CELL_VALUE::WALL;
                break;
            case ' ':
                mapa[{x,y}] = own_types::CELL_VALUE::EMPTY;
                break;
            case 'E':
                mapa[{x,y}] = own_types::CELL_VALUE::EXIT;
                break;
            }
        }
    }
}

Labyrinth::Labyrinth(const std::string &filename): mapa({1, 1}), scanned({1,1}) {
    std::ifstream input;
    input.open(filename);
    size_t X_SIZE, Y_SIZE;
    input >> X_SIZE >> Y_SIZE;
    input >> robot_x >> robot_y >> robot_drones;

    mapa = own_types::MultiArray<own_types::CELL_VALUE>({X_SIZE, Y_SIZE});
    scanned = own_types::MultiArray<int>({X_SIZE, Y_SIZE});
    char INP;
    for (size_t y = 0; y < Y_SIZE; y++) {
        for (size_t x = 0; x < X_SIZE; x++) {
            input >> INP;
            switch (INP) {
            case 'W':
                mapa[{x,y}] = own_types::CELL_VALUE::WALL;
                break;
            case '_':
                mapa[{x,y}] = own_types::CELL_VALUE::EMPTY;
                break;
            case 'E':
                mapa[{x,y}] = own_types::CELL_VALUE::EXIT;
                break;
            }
        }
    }
    input.close();
}

void Labyrinth::moveRobotRight(int distance) {
    while (distance) {
        step(DIRECTIONS::RIGHT);
        --distance;
        test_alive();
    }
    print();
}
void Labyrinth::moveRobotDown(int distance) {
    while (distance) {
        step(DIRECTIONS::DOWN);
        --distance;
        test_alive();
    }
    print();
}
void Labyrinth::moveRobotLeft(int distance)
{
    while (distance) {
        step(DIRECTIONS::LEFT);
        --distance;
        test_alive();
    }
    print();
}
void Labyrinth::moveRobotUp(int distance) {
    while (distance) {
        step(DIRECTIONS::UP);
        --distance;
        test_alive();
    }
    print();
}
own_types::Array<own_types::subarray<own_types::CELL_VALUE>> Labyrinth::sendDrones(int count) {
    if (count <= 0)
        throw std::logic_error("Negative count");
    own_types::Array<own_types::subarray<own_types::CELL_VALUE>> res = own_types::Array(2*MAX_DISTANCE+1, own_types::subarray(2*MAX_DISTANCE+1, own_types::CELL_VALUE::UNDEFINED));
    int truecount = (count < robot_drones)? count : robot_drones;
    count = truecount;
    while (truecount) {
        size_t x{MAX_DISTANCE},y{MAX_DISTANCE}, d{MAX_DISTANCE};
        res[x][y] =  mapa[{robot_x+x-MAX_DISTANCE,robot_y+y-MAX_DISTANCE}];
        while (d) {
            auto r = std::rand()%4;
            int x_ = x, y_ = y;
            int c = 0;
            do {
                x_ = x; y_ = y;
                switch (r)
                {
                case 1:
                    x_++;
                    break;
                case 2:
                    x_--;
                    break;
                case 3:
                    y_++;
                    break;
                case 0:
                    y_--;
                    break;
                }
                r = (r+1)%4;
                ++c;
            } while (!(x_+robot_x-MAX_DISTANCE >= 0 && x_+robot_x-MAX_DISTANCE < mapa.get_sub_dimentions({})[0] 
                && y_+robot_y-MAX_DISTANCE >= 0 && y_+robot_y-MAX_DISTANCE < mapa.get_sub_dimentions({})[1])
                && c <= 4);
            x = x_;
            y = y_;
            if (c <= 4) {
                res[x][y] = mapa[{robot_x+x-MAX_DISTANCE,robot_y+y-MAX_DISTANCE}];
                scanned[{robot_x+x-MAX_DISTANCE,robot_y+y-MAX_DISTANCE}] = true;
            }
            --d;
        }
        truecount--;
    }
    scanned[{robot_x,robot_y}] = true;
    robot_drones -= count;
    return res;
}

int Labyrinth::getDronesCount() {
    return robot_drones;
}
