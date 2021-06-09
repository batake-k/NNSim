#include "calculator.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <exception>

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

void readInputFile(const string &input_file, vector<Cube> &space_cubes, vector<PolyCube> &polycubes) {
  ifstream ifs(input_file);
  string line;

  // space
  getline(ifs, line);
  int space_size = stoi(line);

  for (int i = 0; i < space_size; ++i) {
    getline(ifs, line);
    auto splited = split(line, ',');
    space_cubes.emplace_back(Cube(stoi(splited[0]), stoi(splited[1]), stoi(splited[2])));
  }
  getline(ifs, line);

  // polycube
  getline(ifs, line);
  int num_polycubes = stoi(line);

  for (int i = 0; i < num_polycubes; ++i) {
    getline(ifs, line);
    int polycube_size = stoi(line);
    vector<Cube> cubes;

    for (int j = 0; j < polycube_size; ++j) {
      getline(ifs, line);
      auto splited = split(line, ',');
      cubes.emplace_back(Cube(stoi(splited[0]), stoi(splited[1]), stoi(splited[2]), stoi(splited[3])));
    }

    PolyCube polycube(cubes, i);
    polycubes.emplace_back(polycube);
    getline(ifs, line);
  }
}

void addSurroundingCube(const Cube &cube, vector<Cube> &surrounding_cube, const vector<Cube> &cubes,
                       const vector<Cube> &space_cubes = vector<Cube>()) {
  int x = cube.getX();
  int y = cube.getY();
  int z = cube.getZ();
  vector<Cube> possible_cubes{Cube(x + 1, y, z, 0), Cube(x - 1, y, z, 0),
                              Cube(x, y + 1, z, 0), Cube(x, y - 1, z, 0),
                              Cube(x, y, z + 1, 0), Cube(x, y, z - 1, 0)};

  for (auto &c : possible_cubes) {
    if (find(surrounding_cube.begin(), surrounding_cube.end(), c) != surrounding_cube.end() ||
        find(cubes.begin(), cubes.end(), c) != cubes.end()) {
      continue;
    }

    if (!space_cubes.empty()) {
      auto iter = find(space_cubes.begin(), space_cubes.end(), c);
      if (iter == space_cubes.end()) continue;
    }

    surrounding_cube.emplace_back(c);
  }
}

bool checkBubble(const vector<Cube> &space_cubes, const vector<Cube> &cubes, const int cut_bubble_size) {
  vector<Cube> surrounding_cube;

  // cubesの周りのバブルになり得るcubeを列挙
  for (const auto &c : cubes) {
    addSurroundingCube(c, surrounding_cube, cubes, space_cubes);
  }

  // 周りのcubeそれぞれから周辺のcubeを付加していき、バブルかどうか判定
  for (const auto &c : surrounding_cube) {
    vector<Cube> bubble_cube;
    bubble_cube.emplace_back(c);

    for (uint32_t i = 0; i < bubble_cube.size(); ++i) {
      addSurroundingCube(bubble_cube[i], bubble_cube, cubes, space_cubes);
      if (bubble_cube.size() > (uint32_t)cut_bubble_size) break;
    }

    if (bubble_cube.size() <= (uint32_t)cut_bubble_size) {
      return true;
    }
  }
  return false;
}

int calcConnectWall(const vector<Cube> &cubes, const vector<Cube> &walls) {
  int num = 0;

  for (const auto &c : cubes) {
    for (const auto &w : walls) {
      if (c.isConnect(w)) ++num;
    }
  }
  return num;
}

};  // namespace

Calculator::Calculator(Parameter &_parameter) : parameter(_parameter) {
  try {
    readInputFile(parameter.input_file, space_cubes, polycubes);
  } catch (const exception &e) {
    std::cerr << "[ERROR] " << e.what() << std::endl;
    exit(1);
  }

  for (const auto &polycube : polycubes) {
    auto states = polycube.getStates();

    for (const auto &state : states) {
      for (const auto &space_cube : space_cubes) {
        vector<Cube> new_state(state);
        bool can_be_neuron = true;

        for (auto &c : new_state) {
          c += space_cube;
          auto iter = find(space_cubes.begin(), space_cubes.end(), c);

          if (iter == space_cubes.end()) {
            can_be_neuron = false;
            break;
          }
        }

        if (can_be_neuron && checkBubble(space_cubes, new_state, parameter.cut_bubble_size)) can_be_neuron = false;
        if (can_be_neuron) {
          neurons.emplace_back(new_state, polycube.getId());
        }
      }
    }
  }

  cout << "space size:    " << space_cubes.size() << endl
       << "num polycubes: " << polycubes.size() << endl
       << "num neurons:   " << neurons.size() << endl;
}

void Calculator::writeInfo() {
  ofstream ofs(parameter.output_file + "_info");
  ofs << polycubes.size() << "," << neurons.size() << "," << space_cubes.size() << endl << endl;

  vector<Cube> surrounding_cubes;
  for (const auto &c : space_cubes) {
    addSurroundingCube(c, surrounding_cubes, space_cubes);
  }

  ofs << surrounding_cubes.size() << endl;
  for (const auto &c : surrounding_cubes) {
    ofs << c.getStatus() << endl;
  }
  ofs << endl;

  for (const auto &n : neurons) {
    n.write(ofs);
  }
  ofs.close();
}

vector<BiasDetail> Calculator::calcBiasDetail() {
  vector<BiasDetail> biases;

  vector<Cube> surrounding_cubes;
  for (const auto &c : space_cubes) {
    addSurroundingCube(c, surrounding_cubes, space_cubes);
  }

  for (const auto &n : neurons) {
    float b_A, b_B, b_C, b_D, b_E;
    float a_A, a_B, a_C, a_D, a_E;
    b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = 0;

    auto cubes = n.getCubes();
    int an = n.getSize();

    b_A = - parameter.A / an;
    a_A = - parameter.A / an;

    b_B = 0.5 * parameter.B * an;
    a_B = 0.5 * parameter.B * an;

    int wall_face = calcConnectWall(cubes, surrounding_cubes);

    b_E = parameter.E * wall_face;
    a_E = parameter.E * wall_face;

    BiasDetail bd = {b_A, b_B, b_C, b_D, b_E, a_A, a_B, a_C, a_D, a_E};
    biases.emplace_back(bd);
  }
  return biases;
}

vector<WeightDetail> Calculator::calcWeightDetail(const uint32_t neuron_id) {
  vector<WeightDetail> weights;

  for (uint32_t i = 0; i < neurons.size(); ++i) {
    if (neuron_id == i) continue;

    float b_A, b_B, b_C, b_D, b_E, b_F;
    float a_A, a_B, a_C, a_D, a_E, a_F;
    b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = b_F = a_F = 0;

    int overlap = calcOverlap(neurons[neuron_id], neurons[i]);
    b_B = - parameter.B * overlap;
    a_B = - parameter.B * overlap;

    vector<Cube> sum_cubes(neurons[neuron_id].getCubes());
    auto temp = neurons[i].getCubes();
    copy(temp.begin(), temp.end(), back_inserter(sum_cubes));

    if (checkBubble(space_cubes, sum_cubes, parameter.cut_bubble_size)) {
      b_C = - parameter.C;
      a_C = - parameter.C;
    }

    float connect_face = (float)calcConnectFace(neurons[neuron_id], neurons[i]);
    b_D = parameter.D * connect_face;
    // a_D = parameter.D * connect_face;

    b_F = - parameter.F;

    if (!(b_A == 0 && b_B == 0 && b_C == 0 && b_D == 0 && b_E == 0 && b_F == 0) ||
        !(a_A == 0 && a_B == 0 && a_C == 0 && a_D == 0 && a_E == 0 && a_F == 0)) {
      WeightDetail wd = {i, b_A, b_B, b_C, b_D, b_E, b_F, a_A, a_B, a_C, a_D, a_E, a_F};
      weights.emplace_back(wd);
    }
  }
  return weights;
}

void Calculator::writeDataDetail() {
  ofstream ofs(parameter.output_file + "_data_detail", ios::binary);
  uint32_t neurons_size = neurons.size();

  {  // bias
    auto bias_details = calcBiasDetail();
    ofs.write((char *)&neurons_size, sizeof(uint32_t));
    ofs.write((char *)&bias_details[0], sizeof(BiasDetail) * neurons_size);
  }

  {  // weight
    ofs.write((char *)&neurons_size, sizeof(uint32_t));

    for (uint32_t i = 0; i < neurons_size; ++i) {
      auto weight_details = calcWeightDetail(i);
      uint32_t size = weight_details.size();
      ofs.write((char *)&size, sizeof(uint32_t));
      ofs.write((char *)&weight_details[0], sizeof(WeightDetail) * size);
    }
  }

  ofs.close();
}

void Calculator::writeData() {
  ofstream ofs(parameter.output_file + "_data", ios::binary);
  uint32_t neurons_size = neurons.size();

  {  // problem type
    int type = 3;
    ofs.write((char *)&type, sizeof(int));
  }

  {  // bias
    auto bias_details = calcBiasDetail();
    vector<Bias> biases;

    for (const auto &bd : bias_details) {
      float b = bd.b_A + bd.b_B + bd.b_C + bd.b_D + bd.b_E;
      float a = bd.a_A + bd.a_B + bd.a_C + bd.a_D + bd.a_E;
      Bias bias = {b, a};
      biases.emplace_back(bias);
    }

    ofs.write((char *)&neurons_size, sizeof(uint32_t));
    ofs.write((char *)&biases[0], sizeof(Bias) * neurons_size);
  }

  {  // weight
    ofs.write((char *)&neurons_size, sizeof(uint32_t));

    for (uint32_t i = 0; i < neurons_size; ++i) {
      auto weight_details = calcWeightDetail(i);
      vector<Weight> weights;

      for (const auto &wd : weight_details) {
        float b = wd.b_A + wd.b_B + wd.b_C + wd.b_D + wd.b_E + wd.b_F;
        float a = wd.a_A + wd.a_B + wd.a_C + wd.a_D + wd.a_E + wd.a_F;

        if (b != 0 || a != 0) {
          Weight weight = {wd.neuron_id, b, a};
          weights.emplace_back(weight);
        }
      }

      uint32_t size = weights.size();
      ofs.write((char *)&size, sizeof(uint32_t));
      ofs.write((char *)&weights[0], sizeof(Weight) * size);
    }
  }

  {  // info space_cubes
    uint32_t size = space_cubes.size();
    ofs.write((char *)&size, sizeof(uint32_t));

    vector<CubeStruct> cs;
    for (const auto &c : space_cubes) {
      CubeStruct _cs = {c.getX(), c.getY(), c.getZ(), c.getType()};
      cs.emplace_back(_cs);
    }

    ofs.write((char *)&cs[0], sizeof(CubeStruct) * size);
  }

  {  // info neurons
    uint32_t size = neurons.size();
    ofs.write((char *)&size, sizeof(uint32_t));

    for (const auto &n : neurons) {
      auto cubes = n.getCubes();
      uint32_t cube_size = cubes.size();
      ofs.write((char *)&cube_size, sizeof(uint32_t));

      vector<CubeStruct> cs;
      for (const auto &c : cubes) {
        CubeStruct _cs = {c.getX(), c.getY(), c.getZ(), c.getType()};
        cs.emplace_back(_cs);
      }

      ofs.write((char *)&cs[0], sizeof(CubeStruct) * cube_size);
    }
  }

  ofs.close();
}
