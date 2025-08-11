#include "Agent.h"


Agent::Agent(){
    mc = mcpp::MinecraftConnection();
    currentLoc = mc.getPlayerPosition();
    NextLoc = currentLoc;
    this->mode = false;
}
Agent::Agent(mcpp::Coordinate startLoc, int x, int z, int y, bool mode)
{   

    srand(time(0));
    this->mode = mode;
    currentLoc = startLoc;

    if (mode) {
        orientation = Z_PLUS;
        y = mc.getHeight(startLoc.x + x - 1, startLoc.z + z - 1);
        startLoc = mcpp::Coordinate(startLoc.x + x - 1, y + 1, startLoc.z + z - 1);
    } else {
        while (mc.getBlock(startLoc) != AIR || mc.getBlock(NextLoc) == MAZE_WALL || mc.getBlock(NextLoc) == EXIT) {
            startLoc = currentLoc;
            int xSpawn = (rand() % x);
            int zSpawn = (rand() % z);
            y = mc.getHeight(x, z);
            startLoc = mcpp::Coordinate(startLoc.x + xSpawn, y + 1, startLoc.z + zSpawn);
            NextLoc = mcpp::Coordinate(startLoc.x, startLoc.y - 1, startLoc.z);
        }
        y = mc.getHeight(startLoc.x, startLoc.z);
        startLoc = mcpp::Coordinate(startLoc.x, y + 1, startLoc.z);
    }

    currentLoc = startLoc;
    NextLoc = startLoc;
    mc.setPlayerPosition(startLoc);
}


/**
 * Performs a breadth-first search (BFS) to find the exit from the current location.
 * The function explores all possible paths in four cardinal directions (X+, X-, Z+, Z-)
 * using a queue to track the coordinates to be explored next. It maintains a map of
 * visited coordinates to avoid revisiting and to reconstruct the path once the exit is
 * found.
 * Upon reaching the exit, it backtracks to form the solution path and triggers the
 * navigation
 * to the exit location using GoToExit.
 */
void Agent::FindExitBreadthFirstSearch() //
{
    mcpp::Coordinate temp = currentLoc;
    // BFS setup
    std::cout << "Searching for fastest exit!\nPlease be patient...\n";
    std::vector<mcpp::Coordinate> directions = {MOVE_XPLUS, 
                                                MOVE_XMINUS, // Self explanatory
                                                MOVE_ZPLUS, 
                                                MOVE_ZMINUS};
    // where to search
    
    auto getKey = [](const mcpp::Coordinate& coord) {
        return coord.x * 10000 + coord.z * 100 + coord.y;
    }; // this gives me a key which can be traced back to the coord

    std::unordered_map<int, mcpp::Coordinate> visited;
    std::queue<mcpp::Coordinate> queue;  // declaration of variables
    std::vector<mcpp::Coordinate> solution;

    queue.push(currentLoc);
    visited[getKey(currentLoc)] = currentLoc;
    bool done = false;
    while (!queue.empty() && !done) {
        NextLoc = queue.front();
        queue.pop();
        for (auto direction : directions) {
            if(!done){
                mcpp::Coordinate next = NextLoc + direction;
                int nextKey = getKey(next);
                if ((mc.getBlock(next) == AIR && visited.find(nextKey) == visited.end()) || mc.getBlock(next) == EXIT) {
                    if (mc.getBlock(next) == EXIT) {
                        mcpp::Coordinate loc = NextLoc;
                        while (loc != currentLoc) {
                            solution.push_back(loc);
                            loc = visited[getKey(loc)];
                        }
                        std::reverse(solution.begin(), solution.end());
                        GoToExit(solution);
                        done = true;
                    } else {
                        queue.push(next);
                        visited[nextKey] = NextLoc; // record the next location
                    }
                }
            }
        }
    }
     currentLoc = temp;
     NextLoc = temp;
}


void Agent::GoToExit(const std::vector<mcpp::Coordinate>& solution)
{
    int step = 1;
    for (auto& loc : solution) {
        mc.setBlock(loc, PATH);
        std::cout << "Step[" << std::to_string(step) << "]:" << loc << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        mc.setBlock(loc, AIR);
        step++;
    }
}


/**
 * Solve the maze using the right-hand rule. The agent turns right whenever
 * possible and moves forward until the exit is found. The agent then
 * backtracks to the start location.
 */
void Agent::SolveRightHand()
{
    mcpp::Coordinate startLoc = currentLoc;
    int step =0;
    if(mode){
        while(!IsWallRight(orientation)){ TurnRight(orientation); }
    }else{
        SearchWhereRightIs(orientation);
    }
    while(!AtExit()){
        NextLoc = currentLoc;
        RightHandSearchAlgorithm(step);
    }
    mc.setBlock(NextLoc, AIR);
    currentLoc = startLoc;
}

void Agent::RightHandSearchAlgorithm(int &step){
    if(IsWallRight(orientation)){
        if(!IsWallInFront(orientation)){
            MoveForward(orientation);
            step++;
            std::cout << "Step[" << std::to_string(step) << "]:" << currentLoc << std::endl;
        }else{
            TurnLeft(orientation);
        }
    }else{
        if(!AtExit()){
            TurnRight(orientation);
            MoveForward(orientation);
            step++;
            std::cout << "Step[" << std::to_string(step) << "]:" << currentLoc << std::endl;
        }
    }
}

bool Agent::AtExit(){
    bool atExit = false;
    if(mc.getBlock(NextLoc+MOVE_XPLUS) == EXIT) atExit = true; 
    if(mc.getBlock(NextLoc+MOVE_XMINUS) == EXIT) atExit = true; 
    if(mc.getBlock(NextLoc+MOVE_ZPLUS) == EXIT) atExit = true; 
    if(mc.getBlock(NextLoc+MOVE_ZMINUS) == EXIT) atExit = true;
    return atExit;
}

bool Agent::IsWallRight(AgentOrientation orientation) {
    bool isWallRight = true;

    if(orientation == X_PLUS){
        mcpp::Coordinate temp = NextLoc + MOVE_ZPLUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) { 
                isWallRight = false;
            }
        }
    }else if(orientation == X_MINUS){
        mcpp::Coordinate temp = NextLoc + MOVE_ZMINUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallRight = false;
            }
        }
    }else if(orientation == Z_PLUS){
        mcpp::Coordinate temp = NextLoc + MOVE_XMINUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallRight = false;
            }
        }
    }else if(orientation == Z_MINUS){
        mcpp::Coordinate temp = NextLoc + MOVE_XPLUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallRight = false;
            }
        }
    }

    return isWallRight;
}


void Agent::SearchWhereRightIs(AgentOrientation &orientation){
    if(mc.getBlock(NextLoc +MOVE_XPLUS) == AIR){ orientation = Z_MINUS; } 
    else if(mc.getBlock(NextLoc + MOVE_XMINUS) == AIR){ orientation = Z_PLUS; } 
    else if(mc.getBlock(NextLoc + MOVE_ZPLUS) == AIR){ orientation = X_PLUS; }
    else if(mc.getBlock(NextLoc+MOVE_ZMINUS) == AIR){ orientation = X_MINUS; }
}

bool Agent::IsWallInFront(AgentOrientation orientation){
    bool isWallInFront = true; 
    if(orientation == X_PLUS){
        mcpp::Coordinate temp = NextLoc + MOVE_XPLUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallInFront = false;

            }
        }
        if(mc.getBlock(NextLoc + MOVE_XPLUS) == AIR) isWallInFront = false; 
    }else if(orientation == X_MINUS){
        mcpp::Coordinate temp = NextLoc + MOVE_XMINUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {

            }
        }
    }else if(orientation == Z_PLUS){
        mcpp::Coordinate temp = NextLoc + MOVE_ZPLUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallInFront = false;

            }
        }
    }else if(orientation == Z_MINUS){
        mcpp::Coordinate temp = NextLoc + MOVE_ZMINUS;
        mcpp::Coordinate ifTraversable = NextLoc;
        temp = mcpp::Coordinate(temp.x,mc.getHeight(temp.x,temp.z),temp.z);
        if(ifTraversable.y - temp.y > -1){
            temp.y += 1;
            if(mc.getBlock(temp) == AIR) {
                isWallInFront = false;
            }
        }
    }
    return isWallInFront;
}

void Agent::MoveForward(AgentOrientation orientation){
    mcpp::MinecraftConnection mc;
    mc.setBlock(NextLoc, AIR);
    if(orientation == X_PLUS){ currentLoc = currentLoc + MOVE_XPLUS; }
    else if(orientation == X_MINUS){ currentLoc = currentLoc + MOVE_XMINUS; }
    else if(orientation == Z_PLUS){ currentLoc = currentLoc + MOVE_ZPLUS; }
    else if(orientation == Z_MINUS){ currentLoc = currentLoc + MOVE_ZMINUS; }
    int y = mc.getHeight(currentLoc.x, currentLoc.z);
    currentLoc = mcpp::Coordinate(currentLoc.x, y+1, currentLoc.z);
    mc.setBlock(currentLoc, PATH);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
}

void Agent::TurnLeft(AgentOrientation &orientation){
    if(orientation == X_PLUS){ 
        orientation = Z_MINUS; 
    }else if(orientation == X_MINUS){
        orientation = Z_PLUS; 
    }else if(orientation == Z_PLUS){
        orientation = X_PLUS;
    }else if(orientation == Z_MINUS){
        orientation = X_MINUS;
    }
}

void Agent::TurnRight(AgentOrientation &orientation){
    if(orientation == X_PLUS){
        orientation = Z_PLUS;
    }else if(orientation == X_MINUS){
        orientation = Z_MINUS;
    }else if(orientation == Z_PLUS){
        orientation = X_MINUS;
    }else if(orientation == Z_MINUS){
        orientation = X_PLUS;
    }
}

Agent::~Agent()
{

}