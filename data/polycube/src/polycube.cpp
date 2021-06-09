#include "polycube.hpp"

using namespace std;

#include <climits>
#include <iostream>
#include <algorithm>

namespace {

void alignWithStandards(vector<Cube> &cubes) {
  // 基準となるcubeの決定
  int minx = INT_MAX, miny = INT_MAX, minz = INT_MAX;

  for (const auto &c : cubes) {
    int x = c.getX(), y = c.getY(), z = c.getZ();

    if (x < minx) {
      minx = x;
      miny = y;
      minz = z;
    } else if (x == minx && y < miny) {
      miny = y;
      minz = z;
    } else if (x == minx && y == miny && z < minz) {
      minz = z;
    }
  }

  // 基準を(0,0,0)に。他hexも揃える
  Cube diff(-minx, -miny, -minz);

  for (auto &c : cubes) {
    c += diff;
  }
}

bool isEqual(const vector<Cube> &cubes1, const vector<Cube> &cubes2) {

  for (const auto &c : cubes1) {
    if (find(cubes2.begin(), cubes2.end(), c) == cubes2.end()) return false;
  }
  return true;
}

void addState(vector<vector<Cube>> &states, const vector<Cube> &state) {
  for (auto &s : states) {
    if (isEqual(s, state)) return;
  }
  states.emplace_back(state);
}

};  // namespace

PolyCube::PolyCube(vector<Cube> &cubes, int _id) : id(_id) {

  for (int num_x = 0; num_x < 4; ++num_x) {
    for (int num_y = 0; num_y < 4; ++num_y) {
      for (int num_z = 0; num_z < 4; ++num_z) {

        alignWithStandards(cubes);
        addState(states, cubes);

        for (auto &c : cubes) {
          c.rotate90toZ();
        }
      }

      for (auto &c : cubes) {
        c.rotate90toY();
      }
    }

    for (auto &c : cubes) {
      c.rotate90toX();
    }
  }

}
