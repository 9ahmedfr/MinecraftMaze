#ifndef COORDINATE_ARRAY_H
#define COORDINATE_ARRAY_H

#include "Coordinate.h"

class CoordinateArray {
    
  public:

    CoordinateArray(int initialCapacity = 10) : capacity(initialCapacity), size(0) {
      array = new Coordinate[capacity];
    }

    ~CoordinateArray() {
      delete[] array;
    }

    void add(Coordinate coord) {
      if (size == capacity) { resize(); }
      array[size++] = coord;
    }

    Coordinate get(int index) const {
      return array[index];
    }

    int length() const {
      return size;
    }

  private:
    
    Coordinate* array;
    int capacity;
    int size;

    void resize() {
      capacity *= 2;
      Coordinate* newArray = new Coordinate[capacity];

      for (int i = 0; i < size; ++i) {
        newArray[i] = array[i];
      }

      delete[] array;
      array = newArray;
    }
};

#endif //COORDINATE_ARRAY_H
