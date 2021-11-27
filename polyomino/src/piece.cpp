#include "piece.hpp"

using namespace std;

#include <algorithm>
#include <climits>
#include <iostream>

namespace {

void alignWithStandards(vector<pair<int,int>> &squares) {
  // 基準
  int minx = INT_MAX, miny = INT_MAX;

  for (const auto &s : squares) {
    int x = s.first, y = s.second;

    if (x < minx) {
      minx = x;
      miny = y;
    } else if (x == minx && y < miny) {
      miny = y;
    }
  }

  for (auto &s : squares) {
    s.first -= minx;
    s.second -= miny;
  }
}

bool isEqual(const vector<pair<int,int>> &squares1, const vector<pair<int,int>> &squares2) {
  if (squares1.size() != squares2.size()) return false;

  for (const auto &s : squares1) {
    if (find(squares2.begin(), squares2.end(), s) == squares2.end()) return false;
  }
  return true;
}

void addState(vector<vector<pair<int,int>>> &states, const vector<pair<int,int>> &state) {
  for (auto &s : states) {
    if (isEqual(s, state)) return;
  }
  states.emplace_back(state);
}

};  // namespace

Piece::Piece(vector<pair<int,int>> &piece_squares, int _piece_id) : piece_id(_piece_id) {
  for (int i = 0; i < 4; ++i) {
    alignWithStandards(piece_squares);
    addState(states, piece_squares);

    for (auto &s : piece_squares) {
      int x = s.first;
      s.first = s.second;
      s.second = - x;
    }
  }

  for (auto &s : piece_squares) {
    s.first *= -1;
  }

  for (int i = 0; i < 4; ++i) {
    alignWithStandards(piece_squares);
    addState(states, piece_squares);

    for (auto &s : piece_squares) {
      int x = s.first;
      s.first = s.second;
      s.second = - x;
    }
  }
}
