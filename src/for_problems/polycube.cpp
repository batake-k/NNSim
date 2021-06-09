#include "polycube.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

PolyCube::PolyCube(ifstream &ifs) {

  {  // info space cubes
    uint32_t size;
    ifs.read((char *)&size, sizeof(uint32_t));

    space_cubes.resize(size);
    ifs.read((char *)&space_cubes[0], sizeof(CubeStruct) * size);
  }

  {  // info neurons
    uint32_t neurons_size;
    ifs.read((char *)&neurons_size, sizeof(uint32_t));
    neurons.resize(neurons_size);

    for (uint32_t i = 0; i < neurons_size; ++i) {
      uint32_t size;
      ifs.read((char *)&size, sizeof(uint32_t));
      neurons[i].resize(size);
      ifs.read((char *)&neurons[i][0], sizeof(CubeStruct) * size);
    }
  }

  ifs.close();
}

int PolyCube::getScore(const std::vector<float> &outputs) {
  int score = 0;

  if (outputs.size() != neurons.size()) {
    cerr << "[ERROR] neurons size differ from outputs size" << endl;
    exit(1);
  }

  vector<CubeStruct> tiles;

  for (uint32_t i = 0; i < outputs.size(); ++i) {

    if (outputs[i] >= 0.5) {

      for (const auto &n : neurons[i]) {

        // 重なりがある
        if (find(tiles.begin(), tiles.end(), n) != tiles.end()) {
          --score;
        } else {
          tiles.emplace_back(n);
        }
      }
    }
  }

  for (const auto &c : space_cubes) {
    if (find(tiles.begin(), tiles.end(), c) == tiles.end()) {
      --score;
    }
  }

  return score;
}

string PolyCube::getGoalStatus(const vector<float> &outputs) {
  int count = 0;

  for (const auto o : outputs) {
    if (o >= 0.5) ++count;
  }

  int score = getScore(outputs);

  if (score == 0) {
    return ",1," + to_string(count) + "," + to_string(score);
  } else {
    return ",0," + to_string(count) + "," + to_string(score);
  }
}
