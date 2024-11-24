#include <iostream>

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
    std::cout << "3) Read Maze to be validated from terminal" << std::endl;
    std::cout << "4) Generate Maze Without Flattening the Terrain" << std::endl;
    std::cout << "5) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}

void printSolveMazeMenu(void){
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------" << std::endl;
    std::cout << "1) Solve Manually" << std::endl;
    std::cout << "2) Show Escape Route" << std::endl;
    std::cout << "3) Back" << std::endl;
    std::cout << std::endl;
    std::cout << "Enter Menu item to continue: " << std::endl;
}


void printTeamInfo(void){
    std::cout << std::endl;
    std::cout << "Team members:" << std::endl;

    std::cout << "\t [1] Ahmed Falulur Rahuman (s4043677@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [2] Imas Imthiyas (s4036988@student.rmit.edu.au)" << std::endl;
    std::cout << "\t [3] Krittapart Veerachaipakdee (s4069451@student.rmit.edu.au)" << std::endl;

    std::cout << std::endl;
}

void printExitMassage(void){
    std::cout << std::endl;
    std::cout << "The End!" << std::endl;
}

void handleInputError(int x, int y){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
    std::cout << "Input Error: Enter a number between " << x << " and " << y << " ...." << std::endl;
}

void getLengthWidth(int& l, int& w){

    std::cout << "Enter the length and width of maze: " << std::endl;
    
    std::cin >> l;
    std::cin >> w;
    
    while (l < 3 || l % 2 == 0 || w < 3 || w % 2 == 0 || std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  
        std::cout << "Input Error: length and width must be odd numbers and must be 3 or greater ...." << std::endl;
        std::cout << "Enter the length and width of maze: " << std::endl;
        std::cin >> l;
        std::cin >> w;
    }
}