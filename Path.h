#ifndef PATH_H
#define PATH_H

#include <mcpp/mcpp.h>

class Path {
public:
    Path(mcpp::Coordinate coord, Path* prevPath) {
        this->coordinate = coord;
        this->prevPath = prevPath;
    }

    Path() : coordinate(), prevPath(nullptr) {}
    ~Path() {}

    mcpp::Coordinate getCoordinate() { return coordinate; }
    Path* getPrevPath() { return prevPath; }

private:
    mcpp::Coordinate coordinate;
    Path* prevPath;
};

#endif //PATH_H