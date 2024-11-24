#include <iostream>
#include <mcpp/mcpp.h>
#include <string>
#include <cstdlib>
#include <limits>
#include "menuUtils.h"
#include "Maze.h"
#include "Agent.h"

#define NORMAL_MODE 0
#define TESTING_MODE 1

enum States{
    ST_Main = 0,
    ST_GetMaze = 1,
    ST_BuildMaze = 2,
    ST_SolveMaze = 3,
    ST_Creators = 4,
    ST_Exit = 5
};

void generateMazeMenu(Maze*& maze, bool mode);
void solveMazeMenu(Maze* maze, bool mode, bool& solveManuallySelected);
void getBasePoint(mcpp::Coordinate& basePoint, bool mode);

int main(int argc, char* argv[]){

    srand(time(0));

    bool mode = NORMAL_MODE;
    
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-testmode") {
            mode = TESTING_MODE;
        }
    }

    mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day");
    Maze *maze = nullptr;
    
    States curState = ST_Main;
    int userInput = 0;

    bool solveManuallySelected = false;

    printStartText();
    
    while (curState != ST_Exit) {

        curState = ST_Main;
        printMainMenu();
        std::cin >> userInput;

        if (std::cin.fail() || userInput < 1 || userInput > 5) { 
            if (std::cin.eof()) { 
                curState = ST_Exit; 
            }
            else { 
                handleInputError(1, 5); 
            }
        }
        else { 
            curState = static_cast<States>(userInput); 
        }
        
        if (curState == ST_GetMaze) {
            printGenerateMazeMenu();
            mc = mcpp::MinecraftConnection();
            generateMazeMenu(maze, mode);
        }

        else if (curState == ST_BuildMaze) {
            if (maze == nullptr) {
                std::cout << "Error: Please generate a maze first ...." << std::endl;
            }

            else if (maze->builtMaze) {
                std::cout << "Error: Maze already built ...." << std::endl;
            }

            else {
                maze->buildMaze();
                std::cout << "Maze built successfully" << std::endl;
            }
        }

        else if (curState == ST_SolveMaze) {
            if (maze == nullptr || !maze->builtMaze) {
                std::cout << "Error: Please build a maze first ...." << std::endl;
            }
            else {
                solveMazeMenu(maze, mode, solveManuallySelected);
            }
        }

        else if (curState == ST_Creators) {
            printTeamInfo();
        }

    }

    if (maze != nullptr) { delete maze; }
    printExitMassage();

    return EXIT_SUCCESS;
}

void generateMazeMenu(Maze*& maze, bool mode) {
    
    int userInput = 0;

    while (userInput != 5){

        int l, w;
        mcpp::Coordinate basePoint;

        std::cin >> userInput;

        if (userInput < 1 || userInput > 5 || std::cin.fail()) {
            handleInputError(1, 5);
        }
        
        else if (userInput == 1) {
            bool validMaze = false;          
            if (maze != nullptr) { delete maze; }
            bool validate = false;
            getBasePoint(basePoint, mode);
            getLengthWidth(l, w);
            maze = new Maze(basePoint, l, w, mode);
            std::cout << "Enter the maze structure:" << std::endl;
            while (!validMaze) {
                maze->readMaze(validate);
                validMaze = maze->checkMaze();
                if (!validMaze) {
                    std::cout << "Invalid Maze: Please use only 'x' and '.' characters ...." << std::endl;
                    std::cout << "Enter the maze structure:" << std::endl;
                }
            }
            std::cout << "Maze read successfully" << std::endl;
            std::cout << "**Printing Maze**" << std::endl;
            std::cout << "BasePoint: (" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;
            std::cout << "Structure" << std::endl;
            maze->displayMaze();
            std::cout << "**End Printing Maze**" << std::endl;
            userInput = 5;
        }

        else if (userInput == 2) {
            if (maze != nullptr) { delete maze; }
            getBasePoint(basePoint, mode);
            getLengthWidth(l, w);
            maze = new Maze(basePoint, l, w, mode);
            maze->generateMaze();
            std::cout << "Maze generated successfully" << std::endl;
            std::cout << "**Printing Maze**" << std::endl;
            std::cout << "BasePoint: (" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;
            std::cout << "Structure:" << std::endl;
            maze->displayMaze();
            std::cout << "**End Printing Maze**" << std::endl;
            userInput = 5;
        }

        else if (userInput == 3) {
            if (maze != nullptr) { delete maze; }
            bool validate = true;
            getBasePoint(basePoint, mode);
            getLengthWidth(l, w);
            maze = new Maze(basePoint, l, w, mode);
            std::cout << "Enter the maze structure to be validated:" << std::endl;
            maze->readMaze(validate);
            std::cout << "Maze read successfully" << std::endl;
            std::cout << "**Printing Maze**" << std::endl;
            std::cout << "BasePoint: (" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;
            std::cout << "Validated Structure" << std::endl;
            maze->displayMaze();
            std::cout << "**End Printing Maze**" << std::endl;
            userInput = 5;
        }

        else if (userInput == 4) {
            if (maze != nullptr) { delete maze; }
            getBasePoint(basePoint, mode);
            getLengthWidth(l, w);
            maze = new Maze(basePoint, l, w, mode);
            maze->generateMazeWithoutFlattening();
            std::cout << "Maze generated successfully" << std::endl;
            std::cout << "**Printing Maze**" << std::endl;
            std::cout << "BasePoint: (" << basePoint.x << ", " << basePoint.y << ", " << basePoint.z << ")" << std::endl;
            std::cout << "Structure:" << std::endl;
            maze->displayMaze();
            std::cout << "**End Printing Maze**" << std::endl;
            userInput = 5;
        }
    }
}

void solveMazeMenu(Maze* maze, bool mode, bool& solveManuallySelected) {

    int userInput = 0;
    
    while (userInput != 3){

        printSolveMazeMenu();

        std::cin >> userInput;

        if (userInput < 1 || userInput > 3 || std::cin.fail()) {
            handleInputError(1, 3);
        }
       
        else if (userInput == 1) {

            std::cout << "Maze ready to Solve..." << std::endl;

            if(mode == NORMAL_MODE){
                maze->placePlayerRandomly();
            }
            else if(mode == TESTING_MODE){
                maze->placePlayerTestMode();
            }
            solveManuallySelected = true;
            
        }

        else if (userInput == 2 && solveManuallySelected) {

            std::cout << "Enter search type (1 for WallFollower, 2 for BreadthFirstSearch):" << std::endl;
            
            int search_type = 0;
            std::cin >> search_type;

            while (search_type < 1 || search_type > 2 || std::cin.fail()) {
                handleInputError(1, 2);
                std::cin >> search_type;
            }

            mcpp::MinecraftConnection mc;
            mcpp::Coordinate player_pos = mc.getPlayerPosition();

            if (search_type == 1) {
                Agent agent(player_pos, mode, "WallFollower");
            } 
            
            else if (search_type == 2) {
                Agent agent(player_pos, mode, "BreathFirstSearch");
            }

            solveManuallySelected = false;
        }
        else if (userInput == 2 && !solveManuallySelected) {

            std::cout << "Please enter \"Solve Manually\" in order to show escape route" << std::endl;
            
        }
    }   
    
}

void getBasePoint(mcpp::Coordinate& basePoint, bool mode) {

    if (mode == NORMAL_MODE) {
        mcpp::MinecraftConnection mc;
        std::string userInput = "";

        std::cout << "In Minecraft, navigate to where you need the maze" << std::endl;
        std::cout << "to be built in Minecraft and type - done:" << std::endl;

        while (std::cin.fail() || userInput != "done") {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
            std::cin >> userInput;
        }

        basePoint = mc.getPlayerPosition();
    }

    else if (mode == TESTING_MODE) {
        basePoint = mcpp::Coordinate(4848, 71, 4369);
    }
}