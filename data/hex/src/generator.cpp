#include "generator.hpp"

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <random>

using namespace std;

namespace {

std::vector<std::string> split(const std::string &str, const char delim) {
  std::vector<std::string> elems;
  std::string item;
  for (char ch : str) {
    if (ch == delim) {
      elems.push_back(item);
      item.clear();
    } else {
      item += ch;
    }
  }

  elems.push_back(item);
  return elems;
}

bool hasBubble(vector<Hex> &v, Hex &added) {
  vector<Hex> sur_hexs;

  for (const auto &vv : v) {
    if (added.isConnect(vv)) {
      sur_hexs.emplace_back(vv);
    }
  }

  int sur_hexs_size = (int)sur_hexs.size();

  if (sur_hexs_size == 1) {
    return false;
  }

  int one = 0, two = 0;
  for (const auto &s1 : sur_hexs) {
    int connect = 0;

    for (const auto &s2 : sur_hexs) {
      if (s1 == s2) continue;
      if (s1.isConnect(s2)) ++connect;
    }

    if (connect == 1) {
      ++one;
    } else if (connect == 2) {
      ++two;
    } else {
      return true;
    }
  }

  if (one == 2 && two == sur_hexs_size - 2) {
    return false;
  }
  return true;
}

void addHex(vector<Hex> &v, vector<Hex> &v_sur, Hex &added) {
  v.emplace_back(added);

  vector<pair<int, int>> sur = {{2, 0}, {1, -1}, {-1, -1}, {-2, 0}, {-1, 1}, {1, 1}};

  for (const auto &s : sur) {
    Hex sur_hex(0, s.first, s.second);
    sur_hex += added;

    if (find(v_sur.begin(), v_sur.end(), sur_hex) == v_sur.end() && find(v.begin(), v.end(), sur_hex) == v.end()) {
      v_sur.emplace_back(sur_hex);
    }
  }
}

void addHex(vector<Hex> &v, vector<Hex> &v_sur) {
  while (true) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> dist(0, v_sur.size() - 1);

    Hex added_hex = v_sur[dist(eng)];

    if (!hasBubble(v, added_hex)) {
      addHex(v, v_sur, added_hex);

      auto iter = find(v_sur.begin(), v_sur.end(), added_hex);
      v_sur.erase(iter);
      return;
    }
  }
}

void alignWithStandards(vector<Hex> &hexs) {
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

  Hex diff(0, -minx, -miny);

  for (auto &h : hexs) {
    h += diff;
  }
}

bool isSame(vector<Hex> one, vector<Hex> two) {
  alignWithStandards(one);

  for (int i = 0; i < 6; ++i) {
    for (auto &e_two : two) {
      e_two.rotate60();
    }

    alignWithStandards(two);
    bool is_same_flag = true;

    for (const auto &e_two : two) {
      if (find(one.begin(), one.end(), e_two) == one.end()) {
        is_same_flag = false;
      }
    }

    if (is_same_flag) {
      return true;
    }
  }

  for (auto &e_two : two) {
    e_two.inverse();
  }

  for (int i = 0; i < 6; ++i) {
    for (auto &e_two : two) {
      e_two.rotate60();
    }

    alignWithStandards(two);
    bool is_same_flag = true;

    for (const auto &e_two : two) {
      if (find(one.begin(), one.end(), e_two) == one.end()) {
        is_same_flag = false;
      }
    }

    if (is_same_flag) {
      return true;
    }
  }

  return false;
}

};  // namespace

void Generator::run() {
  Hex base(0, 0, 0);

  // generate board
  cout << "board size: " << parameter.board_size << endl;
  // board.emplace_back(base);
  vector<Hex> board_sur;
  addHex(board, board_sur, base);

  for (int i = 1; i < parameter.board_size; ++i) {
    addHex(board, board_sur);
  }

  // generate pieces
  vector<string> s_piece_sizes = split(parameter.piece_sizes, ',');
  cout << "num pieces: " << s_piece_sizes.size() << endl;

  for (int i = 0; i < s_piece_sizes.size(); ++i) {
    vector<Hex> piece;
    vector<Hex> piece_sur;
    addHex(piece, piece_sur, base);

    int piece_size = stoi(s_piece_sizes[i]);
    for (int j = 1; j < piece_size; ++j) {
      addHex(piece, piece_sur);
    }

    bool is_already_exists = false;

    for (const auto &e_pieces : pieces) {
      if (e_pieces.size() != piece.size()) {
        continue;
      }

      if (isSame(e_pieces, piece)) {
        is_already_exists = true;
        break;
      }
    }

    if (is_already_exists) {
      --i;
    } else {
      pieces.emplace_back(piece);
    }
  }
}

void Generator::writeData() {
  ofstream ofs(parameter.output_path);

  ofs << board.size() << endl;

  for (const auto &b : board) {
    ofs << "1," << b.getStatus() << endl;
  }
  ofs << endl;

  ofs << pieces.size() << endl;

  for (const auto &piece : pieces) {
    ofs << piece.size() << endl;

    for (const auto &p : piece) {
      ofs << p.getStatus() << endl;
    }
    ofs << endl;
  }

  ofs.close();
}
