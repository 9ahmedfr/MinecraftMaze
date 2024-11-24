#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <iostream>
#include <mcpp/mcpp.h>
#include <list>
#include "Path.h"
#include <queue>

#define MOVE_XPLUS mcpp::Coordinate(1,0,0)
#define MOVE_XMINUS mcpp::Coordinate(-1,0,0)
#define MOVE_ZPLUS mcpp::Coordinate(0,0,1)
#define MOVE_ZMINUS mcpp::Coordinate(0,0,-1)

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATH_FIRST_SEARCH,
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
    Agent(mcpp::Coordinate startLoc, bool mode, std::string type);
    ~Agent();

    

private:
    /* data */
    mcpp::MinecraftConnection mc;

    void WallFollower(mcpp::Coordinate startLoc, bool mode);
    void findDirection(mcpp::MinecraftConnection& mc, mcpp::Coordinate& currCo, int& dir, mcpp::BlockType& front, mcpp::BlockType& right, mcpp::BlockType& left);

    void BreathFirstSearch(mcpp::Coordinate startLoc);
    bool checkIfVisited(std::list<Path*>& visited, Path* coord);

    std::list<Path*> visited;
    std::queue<Path*> unvisited;
};



#endif //ASSIGN3_AGENT_H