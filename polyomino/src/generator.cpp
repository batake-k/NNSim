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

bool hasBubble(vector<pair<int, int>> &v, pair<int, int> &added) {
  int x = added.first;
  int y = added.second;

  vector<pair<int, int>> side;
  vector<pair<int, int>> edge;

  vector<pair<int, int>> _side = {{x+1,y}, {x-1,y}, {x,y+1}, {x,y-1}};
  vector<pair<int, int>> _edge = {{x+1,y+1}, {x+1,y-1}, {x-1,y+1}, {x-1,y-1}};

  for (const auto & _s : _side){
    if(find(v.begin(), v.end(), _s) != v.end()) side.emplace_back(_s);
  }

  for (const auto & _e : _edge){
    if(find(v.begin(), v.end(), _e) != v.end()) edge.emplace_back(_e);
  }

  if (side.size() >= 3) return false;
  if (side.size() == 2 && (side[0].first == side[1].first || side[0].second == side[1].second)) return true;

  //角の四角に隣接するものが無い場合はreturn true
  for(const auto &e : edge){
    int xx = e.first;
    int yy = e.second;
    bool has = true;

    vector<pair<int, int>> temp = {{xx+1,yy}, {xx-1,yy}, {xx,yy+1}, {xx,yy-1}};

    for(const auto &t : temp){
      if(find(side.begin(), side.end(), t) != side.end()) has = false;
    }

    if(has) return true;
  }
  return false;
}

void add(vector<pair<int, int>> &v, vector<pair<int, int>> &v_sur, pair<int, int> &added) {
  v.emplace_back(added);

  vector<pair<int, int>> sur = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

  for (const auto &s : sur) {
    pair<int, int> p = {added.first + s.first, added.second + s.second};

    if (find(v_sur.begin(), v_sur.end(), p) == v_sur.end() && find(v.begin(), v.end(), p) == v.end()) {
      v_sur.emplace_back(p);
    }
  }
}

void add(vector<pair<int, int>> &v, vector<pair<int, int>> &v_sur) {
  while (true) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> dist(0, v_sur.size() - 1);

    auto added = v_sur[dist(eng)];

    if (!hasBubble(v, added)) {
      add(v, v_sur, added);

      auto iter = find(v_sur.begin(), v_sur.end(), added);
      v_sur.erase(iter);
      return;
    }
  }
}

void alignWithStandards(vector<pair<int, int>> &p) {
  int minx = INT_MAX, miny = INT_MAX;

  for (const auto &pp : p) {
    int x = pp.first, y = pp.second;

    if (x < minx) {
      minx = x;
      miny = y;
    } else if (x == minx && y < miny) {
      miny = y;
    }
  }

  for (auto &pp : p) {
    pp.first -= minx;
    pp.second -= miny;
  }
}

bool isSame(vector<pair<int,int>> one, vector<pair<int,int>> two) {
  alignWithStandards(one);

  for (int i = 0; i < 4; ++i) {
    for (auto &e_two : two) {
      int x = e_two.first;
      e_two.first = e_two.second;
      e_two.second = - x;
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
    e_two.first *= -1;
  }

  for (int i = 0; i < 4; ++i) {
    for (auto &e_two : two) {
      int x = e_two.first;
      e_two.first = e_two.second;
      e_two.second = - x;
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
  pair<int,int> base = {0, 0};

  // generate board
  cout << "board size: " << parameter.board_size << endl;
  // board.emplace_back(base);
  vector<pair<int, int>> board_sur;
  add(board, board_sur, base);

  for (int i = 1; i < parameter.board_size; ++i) {
    add(board, board_sur);
  }

  // generate pieces
  vector<string> s_piece_sizes = split(parameter.piece_sizes, ',');
  cout << "num pieces: " << s_piece_sizes.size() << endl;

  for (int i = 0; i < s_piece_sizes.size(); ++i) {
    vector<pair<int,int>> piece;
    vector<pair<int,int>> piece_sur;
    add(piece, piece_sur, base);

    int piece_size = stoi(s_piece_sizes[i]);
    for (int j = 1; j < piece_size; ++j) {
      add(piece, piece_sur);
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
    ofs << b.first << "," << b.second << endl;
  }
  ofs << endl;

  ofs << pieces.size() << endl;

  for (const auto &piece : pieces) {
    ofs << piece.size() << endl;

    for (const auto &p : piece) {
      ofs << p.first << "," << p.second << endl;
    }
    ofs << endl;
  }

  ofs.close();
}
