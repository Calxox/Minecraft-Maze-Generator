// alissa - inputs for making maze, MY PART
#include <iostream>
#include <limits>
#include <algorithm>
#include <vector>
#include <random>
#include <mcpp/mcpp.h>
#include <cctype>
#include <queue>

#define WALL 'x'
#define EMPTY_SPACE '.'


mcpp::Coordinate LocationOfMaze(bool mode, mcpp::MinecraftConnection mc){
    mcpp::Coordinate coord(4848, 71, 4369);
    std::string input;
    bool isDone = false;

    while (!isDone) {
        std::cout << "In Minecraft, navigate to where you need the maze to be built in Minecraft and type - done:" << std::endl;
        std::getline(std::cin, input);

        // Remove whitespace and convert to lowercase
        input.erase(std::remove_if(input.begin(), input.end(), ::isspace), input.end());
        std::transform(input.begin(), input.end(), input.begin(), ::tolower);

        // Check if input is "done"
        if (input == "done") {
            isDone = true;
        } else {
            std::cerr << "Please type 'done' when ready." << std::endl;
        }
    }
    bool gotPlayerPosition = false;
    while(!gotPlayerPosition){
        try{
            if(!mode){
                coord = mc.getPlayerPosition();
            }
            gotPlayerPosition = true;
        }catch(const std::exception& e){
            std::cerr << "Error obtaining player position: " << e.what() << std::endl;
        }
    }
    
    return coord;
}

std::vector<std::vector<char>> getMazeStructure(int length, int width){
    std::vector<std::vector<char>> output;
    std::cout << "Enter the maze structure:" << std::endl;
    bool validInput = false;
    while(!validInput){
        try{
            for(int i = 0; i < length; i++){
                std::vector<char> innerLayer;
                for(int j = 0; j <width; j++){
                    char inputChar;
                    std::cin >> inputChar;
                    if(inputChar == WALL || inputChar == EMPTY_SPACE){
                        innerLayer.push_back(inputChar);
                    }
                }
                output.push_back(innerLayer);
            }
            int isMazeEnclosed = (length *2 + width *2)-1;
            int oneExit = 0;
            for(int i = 0; i < length; i++){
                if(output.at(i).at(0) == EMPTY_SPACE){
                    oneExit++;
                }
                if(output.at(i).at(length-1) == EMPTY_SPACE){
                    oneExit++;
                }
                if(output.at(i).at(length-1) == WALL){
                    isMazeEnclosed--;
                }
                if(output.at(i).at(0) == WALL){
                    isMazeEnclosed--;
                }
            }
            for(int j = 0; j <width; j++){
                if(output.at(0).at(j) == EMPTY_SPACE){
                    oneExit++;
                }
                if(output.at(width-1).at(j) == EMPTY_SPACE){
                    oneExit++;
                }
                if(output.at(width-1).at(j) == WALL){
                    isMazeEnclosed--;
                }
                if(output.at(0).at(j) == WALL){
                    isMazeEnclosed--;
                }     
            }
            std::cout << std::to_string(isMazeEnclosed) << " " << std::to_string(oneExit) << std::endl;
            if(isMazeEnclosed != 0 || oneExit != 1){
                throw std::invalid_argument("Maze read unsuccessful");
            }
            validInput = true;
            std::cout << "Maze read successfully" << std::endl;
        }catch(const std::invalid_argument& e){
            throw std::invalid_argument("Maze read unsuccessful");
        }catch(const std::out_of_range& e){
            throw std::out_of_range("Maze read unsuccessful");
        }
    }
    
    return output;
}

std::vector<int> inputLengthAndWidth(){// i will fix this later
    std::vector<int> output;
    int userInput = 0;
    std::cout << "Enter the length and width of maze:" << std::endl;
    bool validInput = false;
    bool thrown = false;    
    while(!validInput){
        try{
            while(output.size() <2){
            std::cin >> userInput;
            if(std::cin.fail()){
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            if(userInput >= 5){
                output.push_back(userInput);
                validInput = true;
            }else{
                throw std::invalid_argument("Inputs must be greater than 5");
                thrown = true;
            }
        }
        }catch(const std::invalid_argument& e){
            if(!thrown){
                std::cout << e.what() << std::endl;
            }
        }
    }
    return output;
}

// Recursive back tracking algorithym for the maze

enum Directions {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void randomiseDirections(std::vector<Directions>& pathing) {
    for (unsigned int i = 0; i < pathing.size(); i++) {
        int j = rand() % (i + 1);
        std::swap(pathing.at(i), pathing.at(j));
    }
}

void RecursiveBacktracking(std::vector<std::vector<char>> &maze,   
                           int lNode, int wNode, bool mode) {
    
    std::vector<Directions> pathing = {UP, DOWN, LEFT, RIGHT};

    if(!mode){
        randomiseDirections(pathing);
    }

    for(Directions path : pathing){

        int nextlNode = lNode, nextwNode = wNode;

        if(path == UP){
            nextlNode -= 2;
        }else if(path == DOWN){
            nextlNode += 2;
        }else if(path == LEFT){
            nextwNode -= 2;
        }else if(path == RIGHT){
            nextwNode += 2;
        }
        int length = maze.size(), width = maze.at(0).size();
        if((nextlNode > 0 && nextlNode < length -1 ) && 
           (nextwNode > 0 && nextwNode < width -1) && 
           (maze.at(nextlNode).at(nextwNode) != EMPTY_SPACE)) {

            // Mark path and decrement nodes
            maze.at((nextlNode +lNode)/2).at((nextwNode+wNode)/2) = EMPTY_SPACE;
            maze.at(nextlNode).at(nextwNode) = EMPTY_SPACE;

            RecursiveBacktracking(maze, nextlNode, nextwNode, mode);
        }
    }
}

std::vector<std::vector<char>> mazeGeneration(int length, int width, bool mode){
    srand(time(0));
    if(length % 2 == 0){length = length +1;}
    if(width % 2 == 0){width = width +1;}
    std::vector<std::vector<char>> maze(length, std::vector<char>(width, WALL));

    int entranceLength = 0, entranceWidth = 0;
    int side = rand() % 4;

    if(side == 0){
        entranceWidth = rand() % (width / 2) * 2 + 1;
    }else if(side == 1){
        entranceLength = length - 1;
        entranceWidth = rand() % (width / 2) * 2 + 1;
    }else if(side == 2){
        entranceLength = rand() % (length / 2) * 2 + 1;
    }else{
        entranceLength = rand() % (length / 2) * 2 + 1;
        entranceWidth = width - 1;
    }

    maze.at(entranceLength).at(entranceWidth) = EMPTY_SPACE; // making the entrance for the maze

    // where to start the maze WIP
    int startLength = entranceLength, startWidth = entranceWidth;

    if(entranceLength == 0){
        startLength += 1;
    }else if(entranceLength == length -1){
        startLength -= 1;
    }else if(entranceWidth == 0){
        startWidth += 1;
    }else if(entranceWidth == width - 1){
        startWidth -= 1;
    }

    RecursiveBacktracking(maze,startLength, startWidth, mode);
    return maze;

}
void printMaze(std::vector<std::vector<char>> mazeStructure){
    for(auto i:mazeStructure){
        for(auto j: i){
            std::cout << j;
        }
        std::cout << std::endl;
    }
}
/*int startLength = rand() % (length - 1);
    int startWidth = rand() % (width - 1);

    while (tempMaze.at(startLength).at(startWidth) != EMPTY_SPACE) {
        startLength = rand() % (length - 1);
        startWidth = rand() % (width - 1);
    }
    */
// Flood Fill
struct Position{
    int x, y;
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    Position operator+=(const Position& other) const {
        Position temp = *this;
        temp.x = x + other.x;
        temp.y = y + other.y;
        return temp;
    }
    Position operator+(const Position& rhs) const {
        return {x + rhs.x, y + rhs.y};
    }
};
#define UP Position{0,1}
#define DOWN Position{0,-1}
#define LEFT Position{-1, 0}
#define RIGHT Position{1, 0}


std::vector<std::vector<bool>> FloodFill(std::vector<std::vector<char>>& maze, int length, int width, int startLength, int startWidth, 
                std::vector<Position> fill){
    std::vector<std::vector<bool>> visited(length, std::vector<bool>(width, false));
    std::queue<Position> queue;
    queue.push({startLength, startWidth});
    visited.at(startLength).at(startWidth) = true;

    while (!queue.empty()) {
        Position pos = queue.front();
        queue.pop();
        for (auto path : fill) {
            Position newPos = pos + path;
            if (newPos.x >= 0 && newPos.y >= 0 && newPos.x < length && newPos.y < width) {
                if (maze.at(newPos.x).at(newPos.y) == EMPTY_SPACE && !visited.at(newPos.x).at(newPos.y)) {
                    queue.push(newPos);
                    visited.at(newPos.x).at(newPos.y) = true;
                }
            }
        }
    }
    return visited;
}

void BranchingToAvaliableWalls(std::vector<std::vector<char>>& tempMaze, int length, int width, 
                                std::vector<std::vector<bool>>& visited, std::vector<Position> fill,
                                bool& foundImpurity){
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            if (tempMaze.at(i).at(j) == EMPTY_SPACE && !visited.at(i).at(j)) {
                foundImpurity = true;
                bool wallAlreadyPlaced = false;
                for(auto path:fill){
                    Position pos = {i + (path.x*2), j + (path.y*2)};
                    if(pos.x >= 0 && pos.y >= 0 && pos.x < length && pos.y < width && 
                       tempMaze.at(pos.x).at(pos.y) == EMPTY_SPACE){
                        Position newPos = {i + path.x, j + path.y};
                        if(newPos.x > 0 && newPos.y > 0 && newPos.x < length-1 && newPos.y < width-1 && !wallAlreadyPlaced){
                            tempMaze.at(newPos.x).at(newPos.y) = EMPTY_SPACE;
                            wallAlreadyPlaced =true;
                        }
                    }
                }
                visited = FloodFill(tempMaze,length,width,i,j,fill);
            }
        }
    }
}

void IsolatedAreaCheck(std::vector<std::vector<char>>& maze, std::vector<Position> fill,
                         bool &isolatedArea, int startLength, int startWidth){

    std::queue<Position> queue;
    
    std::vector<std::vector<char>> tempMaze = maze;

    int length = maze.size(), width = maze.at(0).size();
    startLength = rand() % (length - 1);
    startWidth = rand() % (width - 1);

    while (maze.at(startLength).at(startWidth) != EMPTY_SPACE) {
        startLength = rand() % (length - 1);
        startWidth = rand() % (width - 1);
    }
    
    std::vector<std::vector<bool>> visited =
    FloodFill(maze, length, width, startLength, startWidth, fill);

    BranchingToAvaliableWalls(tempMaze, length, width, visited, fill, isolatedArea);

    maze = tempMaze;
}

void LoopAreaCheck(std::vector<std::vector<char>>& maze, std::vector<Position> fill,
                    bool &loopArea, int startLength, int startWidth){

    std::queue<Position> queue;
    
    std::vector<std::vector<char>> tempMaze = maze;

    for (auto& inner : tempMaze) {
        for (auto& outer : inner) {
            outer = (outer == WALL) ? EMPTY_SPACE : WALL;
        }
    }
    int length = maze.size(), width = maze.at(0).size();
    startLength = rand() % (length - 1);
    startWidth = rand() % (width - 1);

    while (maze.at(startLength).at(startWidth) != EMPTY_SPACE) {
        startLength = rand() % (length - 1);
        startWidth = rand() % (width - 1);
    }
    
    std::vector<std::vector<bool>> visited =
    FloodFill(maze, length, width, startLength, startWidth, fill);
       
    BranchingToAvaliableWalls(tempMaze, length, width, visited, fill, loopArea);

    for (auto& inner : tempMaze) {
        for (auto& outer : inner) {
            outer = (outer == WALL) ? EMPTY_SPACE : WALL;
        }
    }
    maze = tempMaze;
}


void MazePerfecting(std::vector<std::vector<char>>& maze, bool isGenerating){
    std::vector<Position> fill = {UP, DOWN, LEFT, RIGHT};

    bool isLoop = false, isIsolated = false;
    IsolatedAreaCheck(maze, fill, isIsolated, 0, 0);

    LoopAreaCheck(maze, fill, isLoop, 0, 0);
    if(!isGenerating){
        if(isLoop){
        std::cout << "Loop Detected and Fixed\n";
        }
        if(isIsolated){
            std::cout << "Isolated Detected and Fixed\n";
        }
    }
}


void mazeGenMenu(mcpp::Coordinate &coord,int &length, int &width,
                std::vector<std::vector<char>> &mazeStructure, bool mode,
                mcpp::MinecraftConnection mc){
    mcpp::Coordinate check;

    try{
        while(coord == check){
            coord = LocationOfMaze(mode, mcpp::MinecraftConnection());
        }
        std::vector<int> lengthAndWidth;
        while(lengthAndWidth.empty()){
            lengthAndWidth= inputLengthAndWidth();
        }
        while(mazeStructure.empty()){
            length = lengthAndWidth.at(0);
            width = lengthAndWidth.at(1);
            mazeStructure = mazeGeneration(length, width, mode);
        }
            MazePerfecting(mazeStructure, true);
            std::cout << "**Printing Maze**\n";
            printMaze(mazeStructure);
            std::cout << "BasePoint: " << coord << "\nStructure:"<<std::endl;
            std::cout << "**End Printing Maze**\n";
    }catch(const std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
    }catch(const std::out_of_range& e){
        std::cerr << e.what() <<std::endl;
    }
    
}

void mazeGenMenuInput(mcpp::Coordinate &coord,int &length, int &width,
                std::vector<std::vector<char>> &mazeStructure, bool mode){
    try{
        mcpp::Coordinate coord = LocationOfMaze(mode, mcpp::MinecraftConnection());
        std::vector<int> lengthAndWidth= inputLengthAndWidth();
        int length = lengthAndWidth.at(0);
        int width = lengthAndWidth.at(1);
        std::vector<std::vector<char>> mazeStructure = getMazeStructure(length, width);
        std::cout << "**Printing Maze**\n";
        MazePerfecting(mazeStructure, false);
        std::cout << "BasePoint: " << coord << "\nStructure:"<<std::endl;
        for(auto i:mazeStructure){
            for(auto j: i){
                std::cout << j;
            }
            std::cout << std::endl;
        }
        std::cout << "**End Printing Maze**\n";
        
    }catch(const std::invalid_argument& e){
        std::cerr << e.what() << std::endl;
    }catch(const std::out_of_range& e){
        std::cerr << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
    }

}
//alissa = MY PART // .-.