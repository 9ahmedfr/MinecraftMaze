#include "Agent.h"
#include <chrono> 
#include <thread>
#include <queue>
#include "Path.h"

// Constructor for the Agent class
Agent::Agent(mcpp::Coordinate startLoc, bool mode, std::string type) {
    // Initialize the unvisited and visited containers
    unvisited = std::queue<Path*>();
    visited = std::list<Path*>();

    // Determine the type of agent and call the appropriate method
    if (type == "WallFollower") {
        WallFollower(startLoc, mode);
    } 
    else if (type == "BreathFirstSearch") {
        BreathFirstSearch(startLoc);
    }
}

// Destructor for the Agent class
Agent::~Agent() {
    std::cout << "Agent Destructor Called, Freeing Memory" << std::endl;
    
    // Free memory for visited paths
    for (auto it = visited.begin(); it != visited.end(); ++it) {
        delete *it; 
    }
    visited.clear(); 

    // Free memory for unvisited paths
    while (!unvisited.empty()) {
        Path* temp = unvisited.front();
        delete temp;
        unvisited.pop();
    }
}

// WallFollower algorithm implementation
void Agent::WallFollower(mcpp::Coordinate startLoc, bool mode) {
    mcpp::Coordinate currCoord = startLoc;
    mcpp::Coordinate prevCoord = currCoord;
    bool firstBlock = true;
    mcpp::Coordinate startpos = currCoord;
    mcpp::MinecraftConnection mc;

    // Get the block types around the starting location
    mcpp::BlockType blockXPLUS = mc.getBlock(currCoord + MOVE_XPLUS);
    mcpp::BlockType blockZPLUS = mc.getBlock(currCoord + MOVE_ZPLUS);
    mcpp::BlockType blockXMINUS = mc.getBlock(currCoord + MOVE_XMINUS);
    mcpp::BlockType blockZMINUS = mc.getBlock(currCoord + MOVE_ZMINUS);

    mcpp::BlockType frontBlock = blockXPLUS;
    mcpp::BlockType rightBlock = blockZPLUS;
    mcpp::BlockType leftBlock = blockZMINUS;

    mcpp::BlockType carpet = mcpp::Blocks::LIME_CARPET;
    mcpp::BlockType air = mcpp::Blocks::AIR;

    int dir = 0;

    // Determine initial direction based on mode
    if (mode) { 
        dir = 0; 
        rightBlock = blockXPLUS;
        if (rightBlock.id == 0) {
            dir = 90;
            rightBlock = blockZPLUS;
            if (rightBlock.id == 0) {
                dir = 180;
                rightBlock = blockXMINUS;
                if (rightBlock.id == 0) {
                    dir = 270;
                    rightBlock = blockZMINUS;
                }
            }
        }
    } 
    else {
        if (blockXPLUS.id != 0) {dir = 0;}
        else if (blockZPLUS.id != 0) {dir = 90;}
        else if (blockXMINUS.id != 0) {dir = 180;}
        else if (blockZMINUS.id != 0) {dir = 270;}
    }

    // If all surrounding blocks are air, move in a random direction
    int randDir = 0;
    if (blockXPLUS.id == 0 && blockXMINUS.id == 0 && blockZMINUS.id == 0 && blockZPLUS.id == 0) {
        srand(time(0));
        randDir = (rand() % 4) + 1;
        if (randDir != 0) {
            if (randDir == 1){dir = 0;}
            else if (randDir == 2){dir = 90;}
            else if (randDir == 3){dir = 180;}
            else if (randDir == 4){dir = 270;}
        }
    }

    int step = 1;
    int startDir = dir;

    // Main loop for the WallFollower algorithm
    while (dir != -1) {
        mc.setBlock(currCoord, carpet);
        std::cout << "Step[" << step << "]: (" << currCoord.x << ", " << currCoord.y << ", " << currCoord.z << ")" << std::endl;

        if (!firstBlock) {
            mc.setBlock(prevCoord, air);
            if (prevCoord == startpos && step != 2 && dir == startDir) {
                std::cout << "No Exit Found!\nIncorrect Maze Provided" << std::endl;
                mc.setBlock(currCoord, air);
                return;
            }
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        prevCoord = currCoord;

        // Find the next direction to move
        findDirection(mc, currCoord, dir, frontBlock, rightBlock, leftBlock);

        firstBlock = false;
        ++step;
    }

    mc.setBlock(currCoord, air);
}

// Helper function to find the next direction to move
void Agent::findDirection(mcpp::MinecraftConnection& mc, mcpp::Coordinate& currCo, int& dir, mcpp::BlockType& front, mcpp::BlockType& right, mcpp::BlockType& left) {
    bool found = false;

    while (!found) {
        mcpp::Coordinate frontMove, rightMove, leftMove;

        // Determine the movement coordinates based on the current direction
        if (dir == 0) {
            frontMove = MOVE_ZMINUS;
            rightMove = MOVE_XPLUS;
            leftMove = MOVE_XMINUS;
        } 
        else if (dir == 90) {
            frontMove = MOVE_XPLUS;
            rightMove = MOVE_ZPLUS;
            leftMove = MOVE_ZMINUS;
        } 
        else if (dir == 180) {
            frontMove = MOVE_ZPLUS;
            rightMove = MOVE_XMINUS;
            leftMove = MOVE_XPLUS;
        }
        else if (dir == 270) {
            frontMove = MOVE_XMINUS;
            rightMove = MOVE_ZMINUS;
            leftMove = MOVE_ZPLUS;
        }

        // Get the block types in the new directions
        front = mc.getBlock(currCo + frontMove);
        right = mc.getBlock(currCo + rightMove);
        left = mc.getBlock(currCo + leftMove);

        // Check if any of the blocks are the exit
        if (front == mcpp::Blocks::BLUE_CARPET || right == mcpp::Blocks::BLUE_CARPET || left == mcpp::Blocks::BLUE_CARPET) {
            dir = -1;
            std::cout << "Exit Found!" << std::endl;
            found = true;
        }
        else {
            // Determine the next move based on the block types
            if (right.id == 0) {
                currCo = currCo + rightMove;
                found = true;
                dir = (dir + 90) % 360;
            } 
            else if (front.id == 0) {
                currCo = currCo + frontMove;
                found = true;
            }
            else if (left.id == 0) {
                currCo = currCo + leftMove;
                found = true;
                dir = (dir + 270) % 360;  
            }
            else {
                dir = (dir + 90) % 360;
            }
        }
    }
}

// BreathFirstSearch algorithm implementation
void Agent::BreathFirstSearch(mcpp::Coordinate startLoc) {
    mcpp::BlockType carpet = mcpp::Blocks::LIME_CARPET;
    mcpp::BlockType bluecarpet = mcpp::Blocks::BLUE_CARPET;
    bool reachedEnd = false;
    mcpp::MinecraftConnection mc;

    // Ensure the player is still for 2 seconds
    mcpp::Coordinate currCoord = mc.getPlayerPosition();
    int baseHeight = mc.getHeight(currCoord.x, currCoord.z) + 1;
    currCoord.y = baseHeight;
    std::this_thread::sleep_for(std::chrono::seconds(2));
    mcpp::Coordinate currPos = mc.getPlayerPosition();
    currPos.y = baseHeight;

    while (currPos != currCoord) {
        currCoord = currPos;
        currCoord.y = baseHeight;
        std::this_thread::sleep_for(std::chrono::seconds(2));
        currPos = mc.getPlayerPosition();
        currPos.y = baseHeight;
        mc.postToChat("Stay Still for 2 seconds...");
    }

    // Initialize the starting path
    Path* start = new Path(currCoord, nullptr);
    unvisited.push(start);

    mcpp::BlockType blockXPLUS;
    mcpp::BlockType blockZPLUS;
    mcpp::BlockType blockXMINUS;
    mcpp::BlockType blockZMINUS;
    Path* curr = unvisited.front();
    mc.postToChat("Searching Path...");

    // Main loop for the BreathFirstSearch algorithm
    while (!reachedEnd) {
        curr = unvisited.front();
        unvisited.pop();

        // Skip visited paths
        while (checkIfVisited(visited, curr) && !unvisited.empty()) {
            curr = unvisited.front();
            unvisited.pop();
        }

        currCoord = curr->getCoordinate();

        // Get the block types around the current location
        blockXPLUS = mc.getBlock(currCoord + mcpp::Coordinate(1, 0, 0));
        blockZPLUS = mc.getBlock(currCoord + mcpp::Coordinate(0, 0, 1));
        blockXMINUS = mc.getBlock(currCoord + mcpp::Coordinate(-1, 0, 0));
        blockZMINUS = mc.getBlock(currCoord + mcpp::Coordinate(0, 0, -1));

        // Check if any of the blocks are the exit
        if (blockXPLUS == bluecarpet || blockZPLUS == bluecarpet || blockXMINUS == bluecarpet || blockZMINUS == bluecarpet) {
            reachedEnd = true;
        }

        // Add new paths to the unvisited queue
        if (blockXPLUS.id != 5) {
            Path* next = new Path(currCoord + mcpp::Coordinate(1, 0, 0), curr);
            unvisited.push(next);
        }

        if (blockZPLUS.id != 5) {
            Path* next = new Path(currCoord + mcpp::Coordinate(0, 0, 1), curr);
            unvisited.push(next);
        }

        if (blockXMINUS.id != 5) {
            Path* next = new Path(currCoord + mcpp::Coordinate(-1, 0, 0), curr);
            unvisited.push(next);
        }

        if (blockZMINUS.id != 5) {
            Path* next = new Path(currCoord + mcpp::Coordinate(0, 0, -1), curr);
            unvisited.push(next);
        }

        visited.push_back(curr);
    }

    mc.postToChat("Found Path!");

    // Trace back the path and mark it with carpet
    Path* prev = curr;
    while (prev != nullptr) {
        mc.setBlock(prev->getCoordinate(), carpet);
        prev = prev->getPrevPath();
    }

    mc.postToChat("Follow the Green Path!");

    // Ensure the player follows the path
    mcpp::Coordinate player = mc.getPlayerPosition();
    player.y = baseHeight;
    mcpp::Coordinate prevPlayer = player;
    prevPlayer.y = baseHeight;
    bool firstIT = true;

    // Live carpet reaction to player's movement
    while (player != currCoord) {
        player = mc.getPlayerPosition();
        player.y = baseHeight;

        if (!firstIT && player != prevPlayer && mc.getBlock(player).id == carpet.id) {
            mc.setBlock(prevPlayer, mcpp::Blocks::AIR);
        }

        if (!firstIT && player != prevPlayer && mc.getBlock(player).id == 0) {
            mc.setBlock(prevPlayer, carpet);
            mc.postToChat("You are off the Path! Follow the Green Path!");
        }

        firstIT = false;
        prevPlayer = player;
    }

    mc.setBlock(prevPlayer, mcpp::Blocks::AIR);
    delete prev;
}

// Helper function to check if a path has been visited
bool Agent::checkIfVisited(std::list<Path*>& visited, Path* coord) {
    bool found = false;

    for (auto it = visited.begin(); it != visited.end() ; it++) {
        if ((*it)->getCoordinate() == coord->getCoordinate()) {
            found = true;
        }
    }
    return found;
}
