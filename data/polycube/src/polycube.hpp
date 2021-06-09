/**
 * 各polycubeの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/6/9
 */

#ifndef POLYCUBE_HPP
#define POLYCUBE_HPP

#include <vector>

#include "cube.hpp"

class PolyCube {
public:
  PolyCube(std::vector<Cube> &cubes, int _id);
  PolyCube(){};
  ~PolyCube(){};

  std::vector<std::vector<Cube>> getStates() const { return states; };
  int getId() const { return id; };

private:
  std::vector<std::vector<Cube>> states;
  int id;
};

#endif
