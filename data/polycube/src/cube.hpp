/**
 * cubeクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/6/9
 */

#ifndef CUBE_HPP
#define CUBE_HPP

#include <cmath>
#include <string>
#include <iostream>

// for binary io
struct CubeStruct{
  int x,y,z;
  int type;
};

class Cube {
public:
  Cube(const int _x, const int _y, const int _z, const int _type = 0)
      : x(_x), y(_y), z(_z), type(_type) {}
  Cube() {}
  ~Cube() {}

  void operator+=(const Cube& other) {
    x += other.getX();
    y += other.getY();
    z += other.getZ();
  }

  // note that don't care about cube type
  bool operator==(const Cube& other) const {
    if (x == other.getX() && y == other.getY() && z == other.getZ()) {
      return true;
    }
    return false;
  }

  bool isConnect(const Cube& other) const {
    int d = (int)std::abs(x - other.getX()) + (int)std::abs(y - other.getY()) + (int)std::abs(z - other.getZ());
    if (d == 0) {
      std::cerr << "[Warning] connection check for the same Cube occurred" << std::endl;
    }else if (d == 1) {
      return true;
    }
    return false;
  }

  void rotate90toX() {
    int _y = - z;
    int _z = y;
    y = _y;
    z = _z;
  }

  void rotate90toY() {
    int _x = z;
    int _z = - x;
    x = _x;
    z = _z;
  }

  void rotate90toZ() {
    int _x = - y;
    int _y = x;
    x = _x;
    y = _y;
  }

  int getX() const { return x; }
  int getY() const { return y; }
  int getZ() const { return z; }
  int getType() const { return type; }

  std::string getStatus() const {
    return std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "," + std::to_string(type);
  }

private:
  int x, y, z;
  int type;
};

#endif
