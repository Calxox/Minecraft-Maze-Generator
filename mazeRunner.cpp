#include <iostream>

#include <mcpp/mcpp.h>


#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"
#define NORMAL_MODE 0
#define TESTING_MODE 1

void menu(States &curState, bool mode){
    bool connectionSuccess = false;

    while (!connectionSuccess) {
        try {
            mcpp::MinecraftConnection mc;
            mc.doCommand("time set day");
            connectionSuccess = true;
        } catch (const std::exception& e) {
            std::cout << e.what() << "(Waiting 1 second and retrying)"<< std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));  // Optional: Wait before retrying
        }
    }

     // for some reason the mc doesnt carry on from int main so i had to do it again
    
    Agent *solver = nullptr;
    Maze *maze = nullptr;
    while (curState != ST_Exit)
    { 
        curState = curStateMainMenu(curState);
        if(curState == ST_GenerateMaze){ // Generate Maze. Maze.cpp // alissa 
            while(curState == ST_GenerateMaze){ // do this in functions or make another file
                int mazeMenuInt=0;
                printGenerateMazeMenu();
                curStateGenMazeMenu(mazeMenuInt); // this basically prints the maze menu and returns the user choice
                // use functions to branch here cant use 4 indentations, put it in menuUtils.h
                if(mazeMenuInt == 1){ // Read Maze from terminal, Maze.cpp getting input from user
                if(maze !=nullptr){
                    maze->Remake();
                    maze = nullptr;
                }
                while(maze == nullptr){
                    mcpp::Coordinate coord;
                    int length= 0;
                    int width=0;
                    std::vector<std::vector<char>> mazeStructure;

                    mazeGenMenuInput(coord,length,width,mazeStructure,mode);
                    
                    maze = new Maze(coord,length,width,mazeStructure,mode);
                }


                }else if(mazeMenuInt == 2){ // Generate Random Maze, Maze.cpp
                if(maze !=nullptr){
                    maze->Remake();
                    maze = nullptr;
                }
                while(maze == nullptr){
                    mcpp::Coordinate coord;
                    int length;
                    int width;
                    std::vector<std::vector<char>> mazeStructure;
                    
                    mazeGenMenu(coord, length, width, mazeStructure,mode, mcpp::MinecraftConnection());
                    maze = new Maze(coord, length, width,mazeStructure,mode);
                }

                }else if(mazeMenuInt == 3){ // Back, menuUtils.h
                    curState = ST_Main;
                }
            }
            
        }else if(curState == ST_MakeMaze){ // Build Maze Maze.cpp //jason
            if(maze != nullptr){
                int mazeChoice;
                printMakeMazeMenu();
                curStateMakeMazeMenu(mazeChoice);

                if(mazeChoice==1){
                    maze->Remake();
                    maze->Level();
                    maze->mazeMake();
                    maze->exitIdentifier();
                } else if(mazeChoice==2){
                    maze->Remake();
                    std::vector<int> exitPos = maze->getExit();
                    maze->readTerrain();
                    maze->makeMazeEmpty();
                    maze->editMaze(exitPos.at(0), exitPos.at(1), -1, -1);
                    maze->clearOldPosAndMaze();
                    maze->makeMazeEmpty();
                    maze->editMaze(exitPos.at(0), exitPos.at(1), -1, -1);
                    maze->mazeMakeWithTerrain();
                    maze->exitIdentifier();
                } else if(mazeChoice==3){
                    curState = ST_Main;
                } 
            }else{
                std::cout << "Maze doesn't exist\n";
            }

            
            curState = ST_Main;
            
        }else if(curState == ST_SolveMaze){ // Solve Maze, Agent.cpp // I might claim this one - calvin
            while(curState == ST_SolveMaze){ 
                int solveMazeMenuInt=0;
                printSolveMazeMenu();
                curStateSolverMenu(solveMazeMenuInt); // same as ST_GenerateMaze, put it in menuUtils.h if need branch
                

                if(solveMazeMenuInt == 1){ // Solve Manually, Agent.cpp
                    // testing
                    if(maze == nullptr){
                        std::cout << "Maze doesn't exist\n";
                    }else{
                        mcpp::Coordinate startLoc = maze->getBasePoint();
                        int length = maze->getLength();
                        int width = maze->getWidth(); // assuming it gives correct inputs for length and width
                        int height = maze->getFloorHeight();
                        solver = new Agent(startLoc, length, width,height, mode);
                    }
                    

                }else if(solveMazeMenuInt == 2){ // Show Escape Route, Agent.cpp
                    if(maze == nullptr && solver == nullptr){
                        solver = new Agent();
                        solver->FindExitBreadthFirstSearch();
                    }else if(solver == nullptr){
                        solver = new Agent();
                        solver->SolveRightHand();
                    }else{
                        solver->SolveRightHand();
                    }
                    

                }else if(solveMazeMenuInt == 3){ // Back, menuUtils.h
                    if(maze == nullptr && solver == nullptr){
                        solver = new Agent();
                        solver->FindExitBreadthFirstSearch();
                    }else if(solver == nullptr){
                        solver = new Agent();
                        solver->FindExitBreadthFirstSearch();
                    }else{
                        solver->FindExitBreadthFirstSearch();
                    }
                }else if(solveMazeMenuInt == 4){ // Back, menuUtils.h
                    curState = ST_Main;
                }
            }
        }else if(curState == ST_Creators){ // Team Info, menuUtils.h
            printTeamInfo();
            curState = ST_Main;
        }
    }
    if(maze != nullptr){
        maze->Remake();
        maze = nullptr;
    }
    if(solver != nullptr){
        solver = nullptr;
    }
    delete[] maze;
    delete[] solver;
    
    
}

int main(void){

     bool mode = NORMAL_MODE; // WE'LL USE THIS LATER DONT FORGOT - using this - alissa
    //read Mode
    
    States curState = ST_Main;

    printStartText();
    //State machine for menu
    menu(curState, mode);

    printExitMassage() ; // for the person doing the maze generation remove this here when it exits


    return EXIT_SUCCESS;

}