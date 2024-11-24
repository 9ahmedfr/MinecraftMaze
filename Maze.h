#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <mcpp/mcpp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include "CoordinateArray.h"
#include "Coordinate.h"

class Maze {

public:
    Maze();
    Maze(mcpp::Coordinate basePoint, size_t xlen, size_t zlen, bool testMode);
    ~Maze();
    bool builtMaze;
    void generateMaze();
    void generateMazeWithoutFlattening();
    void displayMaze();
    void buildMaze();
    void placePlayerAtBasePoint();
    void placePlayerTestMode();
    void placePlayerRandomly();
    void setMaze(std::vector<std::vector<char>> maze);
    void readMaze(bool validate);
    void revertChunk ();
    bool checkMaze();
    std::vector<std::vector<char>> getMaze();   

private:
    size_t xlen;
    size_t zlen;
    bool testMode;
    bool enhance;
    CoordinateArray* saveBlocks;
    mcpp::Coordinate basePoint;
    mcpp::Coordinate endCoord;
    std::vector<std::vector<char>> maze;
    std::vector<std::vector<int>> heightWall;

    // Build functions
    void flattenLand ();
    void placeMaze();
    void removeMaze();
    mcpp::BlockType validateBlock(mcpp::BlockType block);

    // Maze Initialization
    void initializeMaze(std::vector<std::pair<int, int>>& blankPositions);
    void findPossibleMoves(std::vector<std::pair<int, int>>& validMoves, int currX, int currY);
    void removeWallAboveObj(const mcpp::HeightMap&);

    // Normal functions
    void placeRandomDot(const std::vector<std::pair<int, int>>& blankPositions, int& currX, int& currY);
    void findClosestExit(int currX, int currY); 
    void movePointerRandomly(int& currX, int& currY);

    // Test functions
    void placeDotandExitTest(int& currX, int& currY);
    void movePointerTest(int& currX, int& currY);

    // Validation functions
    void validateMaze();
    void flipMaze();
    void fixMaze();
};

#endif //ASSIGN_MAZE_H
