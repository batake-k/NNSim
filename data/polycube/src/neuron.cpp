#include "neuron.hpp"

#include <algorithm>

using namespace std;

void Neuron::write(ofstream& ofs) const {
  ofs << id << endl;
  ofs << cubes.size() << endl;
  for (const auto& c : cubes) {
    ofs << c.getStatus() << endl;
  }
  ofs << endl;
}

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j) {
  int overlap = 0;

  auto i_cubes = neuron_i.getCubes();
  auto j_cubes = neuron_j.getCubes();

  for (const auto& i : i_cubes) {
    if (find(j_cubes.begin(), j_cubes.end(), i) != j_cubes.end()) {
      ++overlap;
    }
  }

  return overlap;
}

int calcConnectFace(const Neuron& neuron_i, const Neuron& neuron_j) {
  int face = 0;

  if (calcOverlap(neuron_i, neuron_j) > 0) {
    return 0;
  }

  auto i_cubes = neuron_i.getCubes();
  auto j_cubes = neuron_j.getCubes();

  for (const auto& i : i_cubes) {
    for (const auto& j : j_cubes) {
      if (i.isConnect(j)) {
        ++face;
      }
    }
  }

  return face;
}
