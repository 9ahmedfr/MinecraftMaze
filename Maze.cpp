#include "Maze.h"
#include <mcpp/mcpp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "CoordinateArray.h"
#include "Coordinate.h"

Maze::Maze() {
  this->xlen = 0;
  this->zlen = 0;
  this->testMode = false;
  this->enhance = false;
  this->basePoint = mcpp::Coordinate(0, 0, 0);
  this->endCoord = mcpp::Coordinate(0, 0, 0);
  this->maze = std::vector<std::vector<char>>();
  this->builtMaze = false;
  this->saveBlocks = new CoordinateArray();
}

Maze::Maze(mcpp::Coordinate basePoint, size_t xlen, size_t zlen, bool testMode) {
  this->xlen = xlen;
  this->zlen = zlen;
  this->testMode = testMode;
  this->enhance = false;
  this->basePoint = basePoint;
  this->endCoord = basePoint + mcpp::Coordinate(xlen , 0, zlen);
  this->maze.resize(zlen, std::vector<char>(xlen));
  this->heightWall.resize(xlen, std::vector<int>(zlen));
  this->builtMaze = false;
  this->saveBlocks = new CoordinateArray();
}

Maze::~Maze() {
  if (builtMaze) {
    revertChunk();
  }
  delete saveBlocks;
}

std::vector<std::vector<char>> Maze::getMaze() {
  return this->maze;
}

void Maze::buildMaze() {
  mcpp::MinecraftConnection mc;
  mc.mcpp::MinecraftConnection::setPlayerPosition (basePoint + mcpp::Coordinate(0, 5, 0)); 
  if(!enhance){
    flattenLand();
  } 
  mc.postToChat("Building Maze...");
  placeMaze();
  builtMaze = true;
}

bool Maze::checkMaze() {

  bool validMaze = true;

  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      if (maze[i][j] != 'x' && maze[i][j] != '.') {
        validMaze = false;
      }
    }
  }

  return validMaze;
}

void Maze::setMaze(std::vector<std::vector<char>> maze) {
  this->maze = maze;
}

void Maze::readMaze(bool validate) {

  // Read the maze from the console
  
  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      std::cin >> maze[i][j];
      while (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cin >> maze[i][j];
      }
    }
  }

  // Validate the maze

  if (validate) {
    fixMaze();
    validateMaze();
    flipMaze();
    validateMaze();
    flipMaze();
  }
}

void Maze::displayMaze() {
  for (const auto& row : maze) {
      for (const auto& cell : row) {
          std::cout << cell;
      }
      std::cout << std::endl;
  }
}

void Maze::placePlayerAtBasePoint() {
  mcpp::MinecraftConnection mc;
  mc.setPlayerPosition(basePoint + mcpp::Coordinate(0, 5, 0));
}

void Maze::placePlayerTestMode() {
  mcpp::MinecraftConnection mc;
  mc.setPlayerPosition(endCoord + mcpp::Coordinate(-2, 0, -2));
}

void Maze::placePlayerRandomly() {
  mcpp::MinecraftConnection mc;

  std::vector<std::pair<int, int>> freePositions;


  // Find all the free positions in the maze
  for (size_t i = 1; i < zlen-1; ++i) {
    for (size_t j = 1; j < xlen-1; ++j) {
      if (maze[i][j] == '.') {
        freePositions.push_back({i, j});
      }
    }
  }

  int randomIndex = rand() % freePositions.size();
  mcpp::Coordinate playerCoord = basePoint + mcpp::Coordinate(freePositions[randomIndex].first, 0, freePositions[randomIndex].second);
  mc.setPlayerPosition(playerCoord);
}

void Maze::generateMaze() {
  enhance = false;
  int currX, currY;
  std::vector<std::pair<int, int>> blankPositions;

  initializeMaze(blankPositions);

  if (testMode) {
    placeDotandExitTest(currX, currY);
    movePointerTest(currX, currY);
  } 

  else {
    placeRandomDot(blankPositions, currX, currY);
    findClosestExit(currX, currY);
    movePointerRandomly(currX, currY);
  }
}

void Maze::generateMazeWithoutFlattening(){
  enhance = true;
  int currX, currY;
  std::vector<std::pair<int, int>> blankPositions;
  mcpp::MinecraftConnection mc;
  mcpp::HeightMap height_all = mc.getHeights(basePoint, endCoord);

  initializeMaze(blankPositions);
  placeRandomDot(blankPositions, currX, currY);
  findClosestExit(currX, currY);
  movePointerRandomly(currX, currY);
  removeWallAboveObj(height_all);
}

void Maze::flattenLand () {

  mcpp::MinecraftConnection mc;

  mcpp::Coordinate flatStart(basePoint.x - 1, basePoint.y -1, basePoint.z - 1);
  mcpp::Coordinate flatEnd(endCoord.x + 1, endCoord.y -1, endCoord.z + 1);

  mcpp::HeightMap heightMap = mc.getHeights(flatStart, flatEnd);
  mcpp::BlockType currBlock;

  for (size_t i = 0; i < zlen + 2; i++) {
    for (size_t j = 0; j < xlen + 2; j++) {

      mcpp::Coordinate currCoord(flatStart.x + j, heightMap.get(j,i), flatStart.z + i);
      int diff = currCoord.y - flatStart.y;
      
      if (diff != 0) { 
        currBlock = validateBlock(mc.getBlock(currCoord));
      }

      else {
        mc.setBlock(currCoord + mcpp::Coordinate(0, 1, 0), mcpp::Blocks::STONE);
        mc.setBlock(currCoord + mcpp::Coordinate(0, 1, 0), mcpp::Blocks::AIR);
      }

      while (diff != 0) {

        if (diff < 0) {

          currCoord.y++;
          
          Coordinate saveCoor(currCoord.x, currCoord.y, currCoord.z);
          saveBlocks->add(saveCoor);
          mc.setBlock(currCoord, currBlock);

          diff = currCoord.y - flatStart.y;
        }

        else if (diff > 0) {

            Coordinate saveCoor(currCoord.x, currCoord.y, currCoord.z, mc.getBlock(currCoord));
            saveBlocks->add(saveCoor);
            mc.setBlock(currCoord, mcpp::Blocks::AIR);
            currCoord.y--;
            diff = currCoord.y - flatStart.y;

            if (diff == 0) {
              currCoord.y = mc.getHeight(currCoord.x, currCoord.z);
              currBlock = validateBlock(mc.getBlock(currCoord));
              diff = currCoord.y - flatStart.y;
            }
        }
      }

      if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
    }
  }
}

void Maze::revertChunk () {

  removeMaze();

  mcpp::MinecraftConnection mc;
  Coordinate currCoord;
  mcpp::Coordinate mcCoord;

  for (int i = saveBlocks->length(); i >= 0; --i) {
    currCoord = saveBlocks->get(i);
    mcCoord = mcpp::Coordinate(currCoord.getX(), currCoord.getY(), currCoord.getZ());
    mc.setBlock(mcCoord, currCoord.getBlock());
    if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
  }
}

void Maze::placeMaze() {

  mcpp::MinecraftConnection mc;
  int highestWall = 0;
  mcpp::HeightMap heights = mc.getHeights(basePoint, basePoint);

  if(enhance){
    //find the heightest wall block in the area 
    heights = mc.getHeights(basePoint, endCoord);

    highestWall = heights.get(0, 0);
    for (size_t i = 0; i < zlen; ++i) {
      for (size_t j = 0; j < xlen; ++j) {
        if(maze[i][j] == 'x'){
          if(heights.get(j,i) > highestWall){
            highestWall = heights.get(j,i);
          }
        }
      }
    }
  }

  for (size_t i = 0; i < zlen; i++) {
    for (size_t j = 0; j < xlen; j++) {
      
      char block = maze[i][j];

      if (enhance) {
        //building maze for Extension by placing wall above current highest non-air block 
        //until it has the same height as heighest non-air block of area and plus by 3
        mcpp::Coordinate currHeigthCoor(basePoint.x, heights.get(j,i), basePoint.z);

        if (block == 'x') {

          int k = 0;

          while (currHeigthCoor.y < highestWall + 3) {
            currHeigthCoor.y++;
            mc.setBlock(currHeigthCoor + mcpp::Coordinate(j, 0, i), mcpp::Blocks::ACACIA_WOOD_PLANK);
            if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
            k++;
          }
          //record height of the wall each coordinate for remove maze.
          heightWall[j][i] = k;
        }

        if((i == 0 && block == '.') || (j == 0 && block == '.') || (j == xlen-1 && block == '.') || (i == zlen-1 && block == '.')){
          mc.setBlock(currHeigthCoor + mcpp::Coordinate(j, 1, i), mcpp::Blocks::BLUE_CARPET);
          if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
        }
      }
      
      else{
        //place maze for normal
        if (block == '.') {
          mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::STONE);
          mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::AIR);
        }

        if (block == 'x') {
          mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::ACACIA_WOOD_PLANK);
          if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
          mc.setBlock(basePoint + mcpp::Coordinate(j, 1, i), mcpp::Blocks::ACACIA_WOOD_PLANK);
          if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
          mc.setBlock(basePoint + mcpp::Coordinate(j, 2, i), mcpp::Blocks::ACACIA_WOOD_PLANK);
          if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
        }

        else if ((i == 0 && block == '.') || (j == 0 && block == '.') || (j == xlen-1 && block == '.') || (i == zlen-1 && block == '.')) {
          mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::BLUE_CARPET);
          if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
        }
      }
    }
  }
}

void Maze::removeMaze() {

  mcpp::MinecraftConnection mc;
  mcpp::Coordinate blockEnd(endCoord.x, endCoord.y+2, endCoord.z);
  mcpp::HeightMap heights = mc.getHeights(basePoint, blockEnd);

  if (builtMaze) {
    for (size_t i = 0; i < zlen; i++) {
      for (size_t j = 0; j < xlen; j++) {
        
        char block = maze[i][j];

        if(enhance){

          //remove the wall from the top and going down by height of the wall that has been recorded from placeMaze().
          mcpp::Coordinate currHeigthCoor(basePoint.x + j, heights.get(j,i), basePoint.z + i);

          if(block == 'x') {

            for(int k = 0; k < heightWall[j][i]; k++){
              mc.setBlock(currHeigthCoor, mcpp::Blocks::AIR);
              currHeigthCoor.y--;
            }
            
            if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
          }

          else if((i == 0 && block == '.') || (j == 0 && block == '.') || (j == xlen-1 && block == '.') || (i == zlen-1 && block == '.')){
            currHeigthCoor.y++;
            mc.setBlock(currHeigthCoor, mcpp::Blocks::AIR);
          }
        }

        else{
          //normal remove
          if (block == 'x') {
            mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::AIR);
            if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
            mc.setBlock(basePoint + mcpp::Coordinate(j, 1, i), mcpp::Blocks::AIR);
            if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
            mc.setBlock(basePoint + mcpp::Coordinate(j, 2, i), mcpp::Blocks::AIR);
            if (!testMode) { std::this_thread::sleep_for(std::chrono::milliseconds(25)); }
          }

          if ((i == 0 && block == '.') || (j == 0 && block == '.') || (j == xlen-1 && block == '.') || (i == zlen-1 && block == '.')) {
            mc.setBlock(basePoint + mcpp::Coordinate(j, 0, i), mcpp::Blocks::AIR);
          }
        }
      }
    }
  }
}

void Maze::initializeMaze(std::vector<std::pair<int, int>>& blankPositions) {
  // Fill the maze with walls
  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      maze[i][j] = 'x';

      // Place exits at the edges of the maze
      if ((i % 2 == 1 && j == 0) || (i % 2 == 1 && i == zlen - 1) || (i == 0 && j % 2 == 1) || (j == xlen - 1 && j % 2 == 1)) {
        maze[i][j] = 'e';
      }
    }
  }

  // Create gaps at odd-numbered positions and store their coordinates
  for (size_t i = 1; i < zlen; i += 2) {
    for (size_t j = 1; j < xlen; j += 2) {
        maze[i][j] = ' ';
        blankPositions.push_back({i, j});
    }
  }

  if(enhance){
    mcpp::MinecraftConnection mc;
    mcpp::HeightMap height_all = mc.getHeights(basePoint, endCoord);

    //Place wall around objects that has height more than 1 blocks
    std::vector<std::pair<int, int>> moves1 = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    int x = xlen;
    int z = zlen;
    for (size_t i = 0; i < zlen; ++i) {
      for (size_t j = 0; j < xlen; ++j) {
        for(int k = 0; k < 4; k++){
        bool levelDiffPass = true;
        int newX1 = j + moves1[k].first;
        int newZ1 = i + moves1[k].second;

        bool withInBounds = newX1 >= 0 && newX1 < x && newZ1 >= 0 && newZ1 < z;
        if(withInBounds){
          //Find block that has height more than on and, put a wall on top, left, right and bottom of the block
          if(abs(height_all.get(newX1, newZ1) - height_all.get(j, i)) > 1){
            levelDiffPass = false;
          }
        }

        if (withInBounds && !levelDiffPass) {
          maze[newZ1][newX1] = 'x';
          }
        }
      }
    }
  }
}

void Maze::placeRandomDot(const std::vector<std::pair<int, int>>& blankPositions, int& currX, int& currY) {
  if (!blankPositions.empty()) {
    // Generate a random index to select a position from blankPositions
    int randomIndex = rand() % blankPositions.size();
    currX = blankPositions[randomIndex].first;
    currY = blankPositions[randomIndex].second;
    if(enhance){
      mcpp::MinecraftConnection mc;
      //keep finding the position that is not a wall
      while(maze[currX][currY] != ' '){
        randomIndex = rand() % blankPositions.size();
        currX = blankPositions[randomIndex].first;
        currY = blankPositions[randomIndex].second;
      }
    }


    // Place the dot in the randomly selected position
    maze[currX][currY] = '*';
  }
}

void Maze::findClosestExit(int currX, int currY) {
  std::vector<std::pair<int, int>> exitPositions;
  int size = maze.size();

  // Find all the exit positions
  for (int i = 0; i < size; ++i) {

    int innerSize = maze[i].size();

    for (int j = 0; j < innerSize; ++j) {
      if (maze[i][j] == 'e') {
        exitPositions.push_back({i, j});
        maze[i][j] = 'x';
      }
    }
  }

  
  int minDistance = -1, exitX = 0, exitY = 0;
  if(enhance){
    //find exit position that is not block by wall and, closest exit.
    std::vector<std::pair<int, int>> moves1 = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    for (const auto& exit : exitPositions) {
      int distance = abs(exit.first - currX) + abs(exit.second - currY);
      if ((distance < minDistance || minDistance < 0)) {
        for(int i = 0; i < 4; i++){
          size_t newX = exit.first + moves1[i].first;
          size_t newZ = exit.second + moves1[i].second;
          bool inRange = newX >= 0 && newX < maze.size() && newZ >= 0 && newZ < maze[0].size();
          if(inRange){
            if(maze[newX][newZ] == ' '){
              minDistance = distance;
              exitX = exit.first;
              exitY = exit.second;
            }
          }
        }
      }
    }
  }

  else{
    // Manhatten distance
    for (const auto& exit : exitPositions) {
      int distance = abs(exit.first - currX) + abs(exit.second - currY);
      if (distance < minDistance || minDistance < 0) {
        minDistance = distance;
        exitX = exit.first;
        exitY = exit.second;
      }
    }
  }

  maze[exitX][exitY] = '.';
}

void Maze::movePointerRandomly(int& currX, int& currY) {

  bool done = false;

  std::vector<std::pair<int, int>> validMoves;
  findPossibleMoves(validMoves, currX, currY);

  // Find another position if there are no valid moves
  if (validMoves.empty()) {

    maze[currX][currY] = '.';
    bool found = false;

    for (size_t i = 1; i < zlen && !found; i += 2) {
      for (size_t j = 1; j < xlen && !found; j += 2) {

        if (maze[i][j] == '.') {

          std::vector<std::pair<int, int>> findNextValid;
          findPossibleMoves(findNextValid, i, j);

          if (!findNextValid.empty()) {
            currX = i;
            currY = j;
            maze[currX][currY] = '*';
            found = true;
          }
        }
      }
    }

    if (!found) {
      done = true;
    }
  }

  else {
    int randomIndex = rand() % validMoves.size();

    // Mark the current position as visited
    maze[currX][currY] = '.';

    // Remove the Wall in between and mark the wall as visited
    currX = (currX + validMoves[randomIndex].first) / 2;
    currY = (currY + validMoves[randomIndex].second) / 2;
    maze[currX][currY] = '.';

    currX = validMoves[randomIndex].first;
    currY = validMoves[randomIndex].second;
    maze[currX][currY] = '*';
  } 

  if (done) {
    maze[currX][currY] = '.';
  } 

  else {
    movePointerRandomly(currX, currY);
  }
}

void Maze::findPossibleMoves(std::vector<std::pair<int, int>>& validMoves, int currX, int currY) {
  
  std::vector<std::pair<int, int>> moves = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};

  // Check if the moves are valid
  for (const auto& move : moves) {
    int size = maze.size();
    int innerSize = maze[0].size();
    int newX = currX + move.first;
    int newY = currY + move.second;

    bool withInBounds = newX >= 0 && newX < size && newY >= 0 && newY < innerSize 
                     && maze[newX][newY] != 'x' && maze[newX][newY] != 'e';

    if (withInBounds && maze[newX][newY] != '.') {
      validMoves.push_back({newX, newY});
    }
  }
}

void Maze::removeWallAboveObj(const mcpp::HeightMap& height_all){
  std::vector<std::pair<int, int>> moves1 = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  std::vector<std::pair<int, int>> removeWall;
  int x = xlen;
  int z = zlen;
  
  for (int i = 0; i < z; ++i) {
    for (int j = 0; j < x; ++j) {
      //Fill all empty space
      if(maze[i][j] == ' '){
        maze[i][j] = 'x';
      }
      //remove walls on the object
      if(maze[i][j] == 'x'){
        int count = 0;
        int countDot = 0;
        bool withInBounds;
        for(int k = 0; k < 4; k++){
          int newZ1 = i + moves1[k].first;
          int newX1 = j + moves1[k].second;
          //check if the coordinate is inside the maze
          withInBounds = newX1 >= 0 && newX1 < x && newZ1 >= 0 && newZ1 < z;
          if(withInBounds){
            if(maze[newZ1][newX1] == 'x' || maze[newZ1][newX1] == ' '){
              count++;
            }
            if(maze[newZ1][newX1] == '.'){
              countDot++;
            }
          }
        }
        //remove wall at the cornner if there is no entrance nearby and remove wall where objects are.
        if (count > 3 || (count > 2 && (i == 0 || j == 0 || i == z-1 || j == x-1)) || ((i == 0 && j == 0 && countDot == 0) || (i == 0 && j == x-1 && countDot == 0) || (i == z-1 && j == 0 && countDot == 0) || (i == z-1 && j == x-1 && countDot == 0))) {
          removeWall.push_back(std::make_pair(i, j));
        }
      }
    }
  }
  //remove wall
  for(size_t i = 0; i < removeWall.size(); i++){
    maze[removeWall[i].first][removeWall[i].second] = ' ';
  }

  std::vector<std::pair<int, int>> addWall;

  for (int i = 0; i < z; ++i) {
    for (int j = 0; j < x; ++j) {
      // fix hole in the wall
      if(maze[i][j] == ' '){
        int count = 0;
        int levelDiffPass = 0;
        bool withInBounds;
        for(int k = 0; k < 4; k++){
          int newZ1 = i + moves1[k].first;
          int newX1 = j + moves1[k].second;
          //check if the coordinate is inside the maze
          withInBounds = newX1 >= 0 && newX1 < x && newZ1 >= 0 && newZ1 < z;
          if(withInBounds){
            if(maze[newZ1][newX1] == 'x'){
              count++;
            }
            if(abs(height_all.get(newX1, newZ1) - height_all.get(j, i)) > 1){
              levelDiffPass++;
            }
          }
        }
        if(count >= 2 && levelDiffPass <= 1){
          addWall.push_back(std::make_pair(i, j));
        }
      }
    }
  }

  for(auto& add : addWall){
    maze[add.first][add.second] = 'x';
  }
}

void Maze::placeDotandExitTest(int& currX, int& currY) {

  // Place the dot at the starting position
  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      if (maze[i][j] == 'e') {
        maze[i][j] = 'x';
      }
    }
  }

  currX = 1, currY = 1;
  maze[currX][currY] = '*';
  maze[1][0] = '.';
}

void Maze::movePointerTest(int& currX, int& currY) {

  bool done = false;

  std::vector<std::pair<int, int>> validMoves;
  findPossibleMoves(validMoves, currX, currY);

  // Find another position if there are no valid moves
  if (validMoves.empty()) {

    maze[currX][currY] = '.';
    bool found = false;

    for (size_t i = 1; i < zlen && !found; i += 2) {
      for (size_t j = 1; j < xlen && !found; j += 2) {

        if (maze[i][j] == '.') {

          std::vector<std::pair<int, int>> findNextValid;
          findPossibleMoves(findNextValid, i, j);

          if (!findNextValid.empty()) {
            currX = i;
            currY = j;
            maze[currX][currY] = '*';
            found = true;
          }
        }
      }
    }

    if (!found) {
      done = true;
    }
  }

  else {

    // Mark the current position as visited
    maze[currX][currY] = '.';

    // Remove the Wall in between and mark the wall as visited
    currX = (currX + validMoves[0].first) / 2;
    currY = (currY + validMoves[0].second) / 2;
    maze[currX][currY] = '.';

    currX = validMoves[0].first;
    currY = validMoves[0].second;
    maze[currX][currY] = '*';
  } 

  if (done) {
    maze[currX][currY] = '.';
  } 

  else {
    movePointerTest(currX, currY);
  }
}

void Maze::validateMaze() {

  std::vector<std::pair<int, int>> moves = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
  std::vector<std::pair<int, int>> unvisitedCoordinates;
  std::vector<std::pair<int, int>> wallCoordinates;
  
  int currX = 0, currY = 0;
  bool found = false;

  // Find the first empty space in the maze
  for (size_t i = 0; i < zlen && !found; ++i) {
    for (size_t j = 0; j < xlen && !found; ++j) {
      if (maze[i][j] == '.') {
        currX = i;
        currY = j;
        found = true;
      }
    }
  }

  unvisitedCoordinates.push_back({currX, currY});

  // Perform a breadth-first search to find all the empty spaces in the maze and mark them as visited
  while (!unvisitedCoordinates.empty() && xlen > 0 && zlen > 0) {

    currX = unvisitedCoordinates.front().first;
    currY = unvisitedCoordinates.front().second;
    unvisitedCoordinates.erase(unvisitedCoordinates.begin());

    while (maze[currX][currY] == 'F' && !unvisitedCoordinates.empty()) {
      currX = unvisitedCoordinates.front().first;
      currY = unvisitedCoordinates.front().second;
      unvisitedCoordinates.erase(unvisitedCoordinates.begin());
    }

    maze[currX][currY] = 'F';

    for (const auto& move : moves) {
      size_t newX = currX + move.first;
      size_t newY = currY + move.second;
      bool isWithinBounds = newX >= 0 && newX < zlen && newY >= 0 && newY < xlen;

      if (isWithinBounds && maze[newX][newY] == '.') {
        unvisitedCoordinates.push_back({newX, newY});
      }
    }

  }
  
  // Find all the walls that are adjacent to empty spaces and add them to the list of wall coordinates
  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {

      if (maze[i][j] == '.') {
        for (const auto& move : moves) {
          size_t newX = i + move.first;
          size_t newY = j + move.second;
          bool isFilledAdjacent = false;
          bool isNotBorder = newX > 0 && newX < zlen - 1 && newY > 0 && newY < xlen - 1;
          bool isNotEven = newX % 2 == 1 || newY % 2 == 1;

          for (const auto& secondMove : moves) {
            size_t jumpX = newX + secondMove.first;
            size_t jumpY = newY + secondMove.second;
            bool isWithinBounds = jumpX >= 0 && jumpX < zlen && jumpY >= 0 && jumpY < xlen;

            if (isWithinBounds && maze[jumpX][jumpY] == 'F') {
              isFilledAdjacent = true;
            }
          }

          if (isNotBorder && maze[newX][newY] == 'x' && isNotEven && isFilledAdjacent) {
            wallCoordinates.push_back({newX, newY});
          }
        }
      }
    }
  }

  // Reverse the process and mark all the empty spaces as unvisited
  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      if (maze[i][j] == 'F') {
        maze[i][j] = '.';
      }
    }
  }

  // Remove a random wall from the list of wall coordinates
  if (!wallCoordinates.empty()) {
    int randomIndex = rand() % wallCoordinates.size();
    maze[wallCoordinates[randomIndex].first][wallCoordinates[randomIndex].second] = '.';
    validateMaze();
  } 

}

void Maze::flipMaze() {

  // Flip the maze horizontally

  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      if (maze[i][j] == 'x') {
        maze[i][j] = '.';
      }

      else if (maze[i][j] == '.') {
        maze[i][j] = 'x';
      }
    }
  }
}

void Maze::fixMaze() {

  bool foundExit = false;

  // Fix the maze by adding walls to the borders and edges and changing invalid characters to either x or .

  for (size_t i = 0; i < zlen; ++i) {
    for (size_t j = 0; j < xlen; ++j) {
      bool isBorder = i == 0 || j == xlen - 1 || j == 0 || i == zlen - 1;
      bool isEdge = (i == 0 && j == 0) || (j == xlen - 1 && i == zlen - 1) || (i == 0 && i == zlen - 1) || (j == xlen - 1 && j == 0);

      if (maze[i][j] != '.' && maze[i][j] != 'x' && !isBorder) {
        maze[i][j] = '.';
      }

      if (maze[i][j] != '.' && maze[i][j] != 'x' && isBorder) {
        maze[i][j] = 'x';
      }

      if (maze[i][j] != 'x' && isEdge) {
        maze[i][j] = 'x';
      }
      
      if (maze[i][j] != 'x' && isBorder && foundExit) {
        maze[i][j] = 'x';
      }

      if (maze[i][j] == '.' && isBorder && !isEdge) {
        foundExit = true;
      }
    }
  }

  // Add an exit if there is no exit in the maze

  if (!foundExit && xlen > 0 && zlen > 0) {
    maze[1][0] = '.';
  }


  // Add paths to the maze at odd-numbered positions
  for (size_t i = 1; i < zlen; i += 2) {
    for (size_t j = 1; j < xlen; j += 2) {
      bool withinBounds = i > 0 && i < zlen - 1 && j > 0 && j < xlen - 1;
      if (maze[i][j] != '.' && withinBounds) {
        maze[i][j] = '.';
      }
    }
  }

  // Add walls to the maze at even-numbered positions
  for (size_t i = 0; i < zlen; i += 2) {
    for (size_t j = 0; j < xlen; j += 2) {
      bool withinBounds = i > 0 && i < zlen -1 && j > 0 && j < xlen - 1;
      if (maze[i][j] != 'x' && withinBounds) {
        maze[i][j] = 'x';
      }
    }
  }
}

mcpp::BlockType Maze::validateBlock(mcpp::BlockType block) {
  if (block == mcpp::Blocks::STILL_WATER || block == mcpp::Blocks::FLOWING_WATER ||
      block == mcpp::Blocks::STILL_LAVA || block == mcpp::Blocks::FLOWING_LAVA)
  {
    block = mcpp::Blocks::STONE;
  }
  return block;
}


