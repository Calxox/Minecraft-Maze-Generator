#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <mcpp/mcpp.h>
#define Wall 'x'
#define terrainWall -99
#define openSpace '.'
#include <thread>
#include <chrono>
#include <random>
#include <iostream>
class Maze
{

public:
    /*functions*/
    Maze(mcpp::Coordinate basePoint, int xlen, int zlen,std::vector<std::vector<char>> mazeStructre, bool mode);
    ~Maze();
    int getLength();
    int getWidth();
    int getXofMaze();
    int getFloorHeight();
    mcpp::Coordinate getBasePoint();
    void Level();
    void Remake();
    void mazeMake();
    void readTerrain();
    std::vector<int> getExit();
    void editMaze(int x, int z, int oldX, int oldZ);
    void makeMazeEmpty();
    void exitIdentifier();
    void mazeMakeWithTerrain();
    void clearOldPosAndMaze();
private:
    /* data */
    bool TESTING_MODE;
    int xlen;
    int zlen;
    int ylen;
    mcpp::Coordinate basePoint;
    std::vector<std::vector<int>> origLayout; 
    std::vector<mcpp::BlockType> origLayoutBlock;
    std::vector<std::vector<char>> mazeStructure;
    mcpp::MinecraftConnection mc;
    std::vector<mcpp::Coordinate> addedBlocks;
    std::vector<std::vector<int>> terrainheight;
    std::vector<std::vector<int>> oldPos;
    std::vector<std::vector<char>> mazeWithTerrain;
    };




#endif //ASSIGN_MAZE_H