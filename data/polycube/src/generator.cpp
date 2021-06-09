#include "generator.hpp"

#include <random>
#include <climits>
#include <fstream>
#include <iostream>
#include <algorithm>

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

// 各候補Cubeの周り6つを確認し、自身がバブルになっていなければ自分を一度埋める。
// 最後まで続け、自身がバブルになるCubeが存在する場合は元の立体もバブルあり。
bool hasBubble(vector<Cube> &v, vector<Cube> &sur, Cube &added) {
  vector<Cube> test_v(v);

  for (const auto& s : sur) {
    if (s == added) {
      test_v.emplace_back(added);
      continue;
    }

    int connect = 0;

    for (const auto& e : test_v) {
      if (s.isConnect(e)) ++connect;
    }

    if (connect >= 6) {
      return true;
    }

    test_v.emplace_back(s);
  }

  return false;
}

void addCube(vector<Cube> &v, vector<Cube> &v_sur, Cube &added) {
  v.emplace_back(added);

  vector<vector<int>> sur = {{1, 0, 0}, {-1, 0, 0}, {0, 1, 0}, {0, -1, 0}, {0, 0, 1}, {0, 0, -1}};

  for (const auto &s : sur) {
    Cube sur_cube(s[0], s[1], s[2]);
    sur_cube += added;

    if (find(v_sur.begin(), v_sur.end(), sur_cube) == v_sur.end() && find(v.begin(), v.end(), sur_cube) == v.end()) {
      v_sur.emplace_back(sur_cube);
    }
  }
}

void addCube(vector<Cube> &v, vector<Cube> &v_sur) {
  while (true) {
    std::random_device rd;
    std::mt19937 eng(rd());
    std::uniform_int_distribution<int> dist(0, v_sur.size() - 1);

    Cube added_cube = v_sur[dist(eng)];

    if (!hasBubble(v, v_sur, added_cube)) {
      addCube(v, v_sur, added_cube);

      auto iter = find(v_sur.begin(), v_sur.end(), added_cube);
      v_sur.erase(iter);
      return;
    }
  }
}

void alignWithStandards(vector<Cube> &cubes) {
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

  Cube diff(-minx, -miny, -minz);

  for (auto &c : cubes) {
    c += diff;
  }
}

bool isSame(vector<Cube> one, vector<Cube> two) {
  alignWithStandards(one);

  for (int num_x = 0; num_x < 4; ++num_x) {
    for (int num_y = 0; num_y < 4; ++num_y) {
      for (int num_z = 0; num_z < 4; ++num_z) {

        alignWithStandards(two);
        bool is_same_flag = true;

        for (const auto &e : two) {
          if (find(one.begin(), one.end(), e) == one.end()) {
            is_same_flag = false;
          }
        }

        if (is_same_flag) return true;

        for (auto &c : two) {
          c.rotate90toZ();
        }
      }

      for (auto &c : two) {
        c.rotate90toY();
      }
    }

    for (auto &c : two) {
      c.rotate90toX();
    }
  }

  return false;
}

};  // namespace

void Generator::run() {
  Cube base(0, 0, 0, 0);

  // generate space
  cout << "space size: " << parameter.space_size << endl;
  vector<Cube> space_sur;
  addCube(space_cubes, space_sur, base);

  for (int i = 1; i < parameter.space_size; ++i) {
    addCube(space_cubes, space_sur);
  }

  // generate polycubes
  vector<string> s_polycube_sizes = split(parameter.polycube_sizes, ',');
  cout << "num polycubes: " << s_polycube_sizes.size() << endl;

  for (int i = 0; i < s_polycube_sizes.size(); ++i) {
    vector<Cube> polycube;
    vector<Cube> polycube_sur;
    addCube(polycube, polycube_sur, base);

    int polycube_size = stoi(s_polycube_sizes[i]);
    for (int j = 1; j < polycube_size; ++j) {
      addCube(polycube, polycube_sur);
    }

    bool is_already_exists = false;

    for (const auto &e_polycubes : polycubes) {
      if (e_polycubes.size() != polycube.size()) {
        continue;
      }

      if (isSame(e_polycubes, polycube)) {
        is_already_exists = true;
        break;
      }
    }

    if (is_already_exists) {
      --i;
    } else {
      polycubes.emplace_back(polycube);
    }
  }
}

void Generator::writeData() {
  ofstream ofs(parameter.output_path);

  ofs << space_cubes.size() << endl;

  for (const auto &c : space_cubes) {
    ofs << c.getStatus() << endl;
  }
  ofs << endl;

  ofs << polycubes.size() << endl;

  for (const auto &polycube : polycubes) {
    ofs << polycube.size() << endl;

    for (const auto &c : polycube) {
      ofs << c.getStatus() << endl;
    }
    ofs << endl;
  }

  ofs.close();
}
