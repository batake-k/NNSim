#include "neuron.hpp"

#include <algorithm>

using namespace std;

void Neuron::write(ofstream& ofs) const {
  ofs << piece_id << endl;
  ofs << squares.size() << endl;
  for (const auto& s : squares) {
    ofs << s.first << "," << s.second << endl;
  }
  ofs << endl;
}

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j) {
  int overlap = 0;

  auto i_squares = neuron_i.getSquares();
  auto j_squares = neuron_j.getSquares();

  for (const auto& i : i_squares) {
    if (find(j_squares.begin(), j_squares.end(), i) != j_squares.end()) {
      ++overlap;
    }
  }

  return overlap;
}

int calcOverlapPoint(const Neuron& neuron_i, const Neuron& neuron_j) {
  int point = 0;

  auto i_squares = neuron_i.getSquares();
  auto j_squares = neuron_j.getSquares();

  for (const auto& i : i_squares) {
    if (find(j_squares.begin(), j_squares.end(), i) != j_squares.end()) {
      point += 1;
    }
  }

  return point;
}

int calcConnectEdge(const Neuron& neuron_i, const Neuron& neuron_j) {
  int edge = 0;

  if (calcOverlap(neuron_i, neuron_j) > 0) {
    return 0;
  }

  auto i_squares = neuron_i.getSquares();
  auto j_squares = neuron_j.getSquares();

  for (const auto& i : i_squares) {
    for (const auto& j : j_squares) {
      if ((i.first - j.first)*(j.first - i.first) + (i.second - j.second)*(j.second - i.second) >= -1) {
        ++edge;
      }
    }
  }

  return edge;
}
