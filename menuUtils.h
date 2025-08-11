#include "MazeGenUtils.h"

#define WALL 'x'
#define EMPTY_SPACE '.'
void printStartText(void){
    std::cout << std::endl;
    std::cout << "Welcome to MineCraft MazeRunner!" << std::endl;
    std::cout << "--------------------------------" << std::endl;
}

void printMainMenu(void){
    std::cout << std::endl;
    std::cout << "------------- MAIN MENU -------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Build Maze in MineCraft" << std::endl;
    std::cout << "3) Solve Maze" << std::endl;
    std::cout << "4) Show Team Information" << std::endl;
    std::cout << "5) Exit" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printGenerateMazeMenu(void){
    std::cout << std::endl;
    std::cout << "------------- GENERATE MAZE -------------" << std::endl;
    std::cout << "1) Read Maze from terminal" << std::endl;
    std::cout << "2) Generate Random Maze" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printMakeMazeMenu(void){
    std::cout << std::endl; // MAKE MAZE
    std::cout << "--------------- MAKE MAZE ---------------" << std::endl;
    std::cout << "1) Generate Maze" << std::endl;
    std::cout << "2) Generate Maze Without Affecting Terrain" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printSolveMazeMenu(void){
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route (RHS Follower)" << std::endl;
    std::cout << "3) Show Escape Route (BFS Follower)" << std::endl;
    std::cout << "4) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}


void printTeamInfo(void){
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;

    //TODO: change this part to reflect your team**********
    std::cout << "\t [1] Calvin Darren Bayno O'Flaherty (s4093869@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [2] Alissa Ngyuen (s4085541@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [3] Jason Le (s4090189@student.rmit.edu.au)" << std::endl;
    //*************

    std::cout << std::endl;
}

void printExitMassage(void){
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
    std::cout << std::endl;
}

int GetUserChoiceMainMenu(){ // getting users choice from user if not int throw exception
    std::string choice;
    std::getline(std::cin, choice);
    choice.erase(std::remove_if(choice.begin(), choice.end(), ::isspace), choice.end());
    if(std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Enter a number between 1 and 5 ....");
    }
    try{
        if(std::stoi(choice) < 1 || std::stoi(choice) > 5 ){
            throw std::out_of_range("Enter a number between 1 and 5 ....");
        }

        return std::stoi(choice);
    }catch(const std::invalid_argument& e){
        throw std::invalid_argument("Enter a number between 1 and 5 ....");
    }catch(const std::out_of_range& e){
        throw std::out_of_range("Enter a number between 1 and 5 ....");
    }
}

int GetUserChoiceMazeMenu(){ // getting users choice from user if not int throw exception (MazeMenu)
    std::string choice;
    
    try{
        std::getline(std::cin, choice); // for some reason getline doesnt work here so i used cin instead::cin >> choice;
        choice.erase(std::remove_if(choice.begin(), choice.end(), ::isspace), choice.end());
        if(std::cin.fail()){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            throw std::invalid_argument("");
        }
        if(std::stoi(choice) < 1 || std::stoi(choice) > 3){
            throw std::out_of_range("Enter a number between 1 and 3 ....");
        }
        return std::stoi(choice);
    }catch(const std::invalid_argument& e){
        throw std::invalid_argument("");
    }catch(const std::out_of_range& e){
        throw std::out_of_range("Enter a number between 1 and 3 ....");
    }
}
// for BFS algo options
int GetUserChoiceSolverMenu(){ // getting users choice from user if not int throw exception (MazeMenu)
    std::string choice;
    std::getline(std::cin, choice); // for some reason getline doesnt work here so i used cin instead::cin >> choice;
    choice.erase(std::remove_if(choice.begin(), choice.end(), ::isspace), choice.end());
    if(std::cin.fail()){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Enter a number between 1 and 4 ....");
    }
    try{
        if(std::stoi(choice) < 1 || std::stoi(choice) > 4){
            throw std::out_of_range("Enter a number between 1 and 4 ....");
        }
        return std::stoi(choice);
    }catch(const std::invalid_argument& e){
        throw std::invalid_argument("Enter a number between 1 and 4 ....");
    }catch(const std::out_of_range& e){
        throw std::out_of_range("Enter a number between 1 and 4 ....");
    }
}
    

enum States{
    ST_Main,
    ST_GenerateMaze,
    ST_MakeMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit
};

States curStateMainMenu(States curState){
    try{
        printMainMenu();
        int mainMenuInt = GetUserChoiceMainMenu();
        if(mainMenuInt == 1){
            curState = ST_GenerateMaze;
        }else if(mainMenuInt == 2){
            curState = ST_MakeMaze;
        }else if(mainMenuInt == 3){
            curState = ST_SolveMaze;
        }else if(mainMenuInt == 4){
            curState = ST_Creators;
        }else if(mainMenuInt == 5){
            curState = ST_Exit;
        }
    }catch(const std::invalid_argument& e){
        std::cerr << "Input Error: " << e.what() << std::endl;
    }catch(const std::exception& e){
        std::cerr << "Input Error: " << e.what() << std::endl;
    }catch  (...){
        throw std::runtime_error("An unknown error has occured");
    }
    return curState;
}

void curStateGenMazeMenu(int &curState){
    bool validInput = false;
    while(!validInput){
        try{
            curState = GetUserChoiceMazeMenu();
            validInput = true;
        }catch(const std::invalid_argument& e){
        }catch(const std::exception& e){
            std::cerr << "Input Error: " << e.what() << std::endl;
        }catch  (...){
            throw std::runtime_error("An unknown error has occured");
        }
    }
}

void curStateMakeMazeMenu(int &curState){
    bool validInput = false;
    while(!validInput){
        try{
            curState = GetUserChoiceMazeMenu();
            validInput = true;
        }catch(const std::invalid_argument& e){
            std::cerr << "Input Error: " << e.what() << std::endl;
        }catch(const std::exception& e){
            std::cerr << "Input Error: " << e.what() << std::endl;
        }catch  (...){
            throw std::runtime_error("An unknown error has occured");
        }
    }
}

void curStateSolverMenu(int &curState){
    bool validInput = false;
    while(!validInput){
        try{
            curState = GetUserChoiceSolverMenu();
            validInput = true;
        }catch(const std::invalid_argument& e){
            std::cerr << "Input Error: " << e.what() << std::endl;
        }catch(const std::exception& e){
            std::cerr << "Input Error: " << e.what() << std::endl;
        }catch  (...){
            throw std::runtime_error("An unknown error has occured");
        }
    }
}