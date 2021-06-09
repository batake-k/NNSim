#ifndef POLYCUBE_HPP
#define POLYCUBE_HPP

#include <fstream>

#include "problem.hpp"

struct CubeStruct {
  int x, y, z;
  int type;

  bool operator==(const CubeStruct &other) const {
    return (x == other.x && y == other.y && z == other.z);
  }
};

class PolyCube : public Problem {
public:
  PolyCube(std::ifstream &ifs);
  PolyCube(){};
  ~PolyCube(){};

  int getScore(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
  std::vector<CubeStruct> space_cubes;
  std::vector<std::vector<CubeStruct>> neurons;
};

#endif
