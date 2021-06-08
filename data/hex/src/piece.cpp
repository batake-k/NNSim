#include "piece.hpp"

using namespace std;

#include <algorithm>
#include <climits>
#include <iostream>

namespace {

void alignWithStandards(vector<Hex> &hexs) {
  // 基準となるhexの決定
  int minx = INT_MAX, miny = INT_MAX;

  for (const auto &h : hexs) {
    int x = h.getX(), y = h.getY();

    if (x < minx) {
      minx = x;
      miny = y;
    } else if (x == minx && y < miny) {
      miny = y;
    }
  }

  // 基準を(0,0)に。他hexも揃える
  Hex diff(0, -minx, -miny);

  for (auto &h : hexs) {
    h += diff;
  }
}

bool isEqual(const vector<Hex> &hexs1, const vector<Hex> &hexs2) {
  if (hexs1.size() != hexs2.size()) return false;

  for (const auto &h : hexs1) {
    if (find(hexs2.begin(), hexs2.end(), h) == hexs2.end()) return false;
  }
  return true;
}

void addState(vector<vector<Hex>> &states, const vector<Hex> &state) {
  for (auto &s : states) {
    if (isEqual(s, state)) return;
  }
  states.emplace_back(state);
}

};  // namespace

Piece::Piece(vector<Hex> &hexs, int _piece_id) : piece_id(_piece_id) {
  for (int i = 0; i < 6; ++i) {
    alignWithStandards(hexs);
    addState(states, hexs);

    for (auto &h : hexs) {
      h.rotate60();
    }
  }

  for (auto &h : hexs) {
    h.inverse();
  }

  for (int i = 0; i < 6; ++i) {
    alignWithStandards(hexs);
    addState(states, hexs);

    for (auto &h : hexs) {
      h.rotate60();
    }
  }
}
