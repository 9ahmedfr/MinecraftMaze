#ifndef COORDINATE_H
#define COORDINATE_H
#include <mcpp/mcpp.h>

class Coordinate {

  public:

    Coordinate() {
      this->x = 0;
      this->y = 0;
      this->z = 0;
      this->block = mcpp::Blocks::AIR;
    }

    Coordinate(int x, int y, int z) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->block = mcpp::Blocks::AIR;
    }

    Coordinate(int x, int y, int z, mcpp::BlockType block) {
      this->x = x;
      this->y = y;
      this->z = z;
      this->block = block;
    }

    int getX() const { return x; }
    int getY() const { return y; }
    int getZ() const { return z; }
    mcpp::BlockType getBlock() const { return block; }
    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
    void setZ(int z) { this->z = z; }
    void setBlock(mcpp::BlockType block) { this->block = block; }

  private:
    int x;
    int y;
    int z;
    mcpp::BlockType block;
};

#endif