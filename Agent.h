#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <iostream>
#include <mcpp/mcpp.h>
#include <random>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>

#define MOVE_XPLUS mcpp::Coordinate(1,0,0)
#define MOVE_XMINUS mcpp::Coordinate(-1,0,0)
#define MOVE_ZPLUS mcpp::Coordinate(0,0,1)
#define MOVE_ZMINUS mcpp::Coordinate(0,0,-1)

#define AIR mcpp::Blocks::AIR
#define PATH mcpp::Blocks::LIME_CARPET
#define EXIT mcpp::Blocks::BLUE_CARPET
#define MAZE_WALL mcpp::Blocks::ACACIA_WOOD_PLANK

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATDH_FIRST_SEARCH,
};

enum AgentOrientation{
    X_PLUS,
    Z_PLUS,
    X_MINUS,
    Z_MINUS
};

class Agent
{
public:
    Agent();
    Agent(mcpp::Coordinate startLoc,int x,int z,int y,bool mode);
    ~Agent();
    
    void SolveRightHand();
    void RightHandSearchAlgorithm(int &step);
    void FindExitBreadthFirstSearch();
    void GoToExit(const std::vector<mcpp::Coordinate>& solution);
    bool AtExit();
    bool IsWallRight(AgentOrientation orientation);
    bool IsWallInFront(AgentOrientation orientation);
    void MoveForward(AgentOrientation orientation);
    void TurnLeft(AgentOrientation &orientation);
    void TurnRight(AgentOrientation &orientation);
    void SearchWhereRightIs(AgentOrientation &orientation);
private:
    /* data */
    mcpp::Coordinate currentLoc = mcpp::Coordinate(0,0,0);
    mcpp::MinecraftConnection mc;
    mcpp::Coordinate NextLoc;
    AgentOrientation orientation = Z_MINUS; // is north
    bool mode;
    //int length; /unused so i have to comment sorry calvin
    //int width;

};



#endif //ASSIGN3_AGENT_H