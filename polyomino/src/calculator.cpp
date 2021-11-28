#include "calculator.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <iostream>

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

void readInputFile(const string &input_file, vector<pair<int,int>> &board, vector<Piece> &pieces) {
  ifstream ifs(input_file);
  string line;

  // board
  getline(ifs, line);
  int board_size = stoi(line);

  for (int i = 0; i < board_size; ++i) {
    getline(ifs, line);
    auto splited_line = split(line, ',');
    board.emplace_back(make_pair(stoi(splited_line[0]), stoi(splited_line[1])));
  }
  getline(ifs, line);

  // piece
  getline(ifs, line);
  int num_pieces = stoi(line);

  for (int i = 0; i < num_pieces; ++i) {
    getline(ifs, line);
    int piece_size = stoi(line);
    vector<pair<int,int>> piece_squares;

    for (int j = 0; j < piece_size; ++j) {
      getline(ifs, line);
      auto splited_line = split(line, ',');
      piece_squares.emplace_back(make_pair(stoi(splited_line[0]), stoi(splited_line[1])));
    }
    Piece piece(piece_squares, i);
    pieces.emplace_back(piece);
    getline(ifs, line);
  }
}

void addSurroundingSquare(const int x, const int y, vector<pair<int,int>> &surrounding_s,
                          const vector<pair<int,int>> &squares,
                          const vector<pair<int,int>> &board = vector<pair<int,int>>()) {
  vector<pair<int,int>> possible_s = {{x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}};

  for (auto &s : possible_s) {
    if (find(surrounding_s.begin(), surrounding_s.end(), s) != surrounding_s.end() ||
        find(squares.begin(), squares.end(), s) != squares.end()) {
      continue;
    }

    if (!board.empty()) {
      auto iter = find(board.begin(), board.end(), s);
      if (iter == board.end()) continue;
    }

    surrounding_s.emplace_back(s);
  }
}

void addBoardSurroundingSquare(const int x, const int y, vector<pair<int,int>> &surrounding_s,
                          const vector<pair<int,int>> &squares,
                          const vector<pair<int,int>> &board = vector<pair<int,int>>()) {
  vector<pair<int,int>> possible_s = {{x+1, y}, {x-1, y}, {x, y+1}, {x, y-1}, {x+1, y+1}, {x+1,y-1}, {x-1, y+1}, {x-1, y-1}};

  for (auto &s : possible_s) {
    if (find(surrounding_s.begin(), surrounding_s.end(), s) != surrounding_s.end() ||
        find(squares.begin(), squares.end(), s) != squares.end()) {
      continue;
    }

    if (!board.empty()) {
      auto iter = find(board.begin(), board.end(), s);
      if (iter == board.end()) continue;
    }

    surrounding_s.emplace_back(s);
  }
}

bool checkBubble(const vector<pair<int,int>> &board, const vector<pair<int,int>> &squares, const int cut_bubble_size) {
  vector<pair<int,int>> surrounding_s;

  // squaresの周りのバブルになり得るsquareを列挙
  for (const auto &s : squares) {
    addSurroundingSquare(s.first, s.second, surrounding_s, squares, board);
  }

  // 周りのsquareそれぞれから周辺のsquareを付加していき、バブルかどうか判定
  for (const auto &s : surrounding_s) {
    vector<pair<int,int>> bubble_s;
    bubble_s.emplace_back(s);

    for (uint32_t i = 0; i < bubble_s.size(); ++i) {
      addSurroundingSquare(bubble_s[i].first, bubble_s[i].second, bubble_s, squares, board);
      if (bubble_s.size() > (uint32_t)cut_bubble_size) break;
    }

    if (bubble_s.size() <= (uint32_t)cut_bubble_size) {
      return true;
    }
  }
  return false;
}

int calcConnectWall(const vector<pair<int,int>> &squares, const vector<pair<int,int>> &walls) {
  int num = 0;

  for (const auto &s : squares) {
    for (const auto &w : walls) {
      if ((s.first - w.first)*(w.first - s.first) + (s.second - w.second)*(w.second - s.second) >= -1) ++num;
      //if ((w.first - s.first == 1 && w.second == s.second) || (w.second - s.second == 1 && w.first == s.first)) ++num;
    }
  }
  return num;
}

};  // namespace

Calculator::Calculator(Parameter &_parameter) : parameter(_parameter) {
  try {
    readInputFile(parameter.input_file, board, pieces);
  } catch (const exception &e) {
    std::cerr << "[ERROR] " << e.what() << std::endl;
    exit(1);
  }

  for (const auto &piece : pieces) {
    auto states = piece.getStates();

    for (const auto &state : states) {
      for (const auto &board_square : board) {
        vector<pair<int,int>> new_state(state);
        bool can_be_neuron = true;

        for (auto &h : new_state) {
          h.first += board_square.first;
          h.second += board_square.second;
          auto iter = find(board.begin(), board.end(), h);

          if (iter == board.end()) {
            can_be_neuron = false;
            break;
          }
        }

        if (can_be_neuron && checkBubble(board, new_state, parameter.cut_bubble_size)) can_be_neuron = false;
        if (can_be_neuron) {
          neurons.emplace_back(new_state, piece.getPieceId());
        }
      }
    }
  }

  cout << "board size:  " << board.size() << endl
       << "num pieces:  " << pieces.size() << endl
       << "num neurons: " << neurons.size() << endl;
}

void Calculator::writeInfo() {
  ofstream ofs(parameter.output_file + "_info");
  ofs << pieces.size() << "," << neurons.size() << "," << board.size() << endl << endl;

  vector<pair<int,int>> surrounding_s;
  for (const auto &s : board) {
    addBoardSurroundingSquare(s.first, s.second, surrounding_s, board);
  }

  ofs << surrounding_s.size() << endl;
  for (const auto &s : surrounding_s) {
    ofs << s.first << "," << s.second << endl;
  }
  ofs << endl;

  for (const auto &n : neurons) {
    n.write(ofs);
  }
  ofs.close();
}

vector<BiasDetail> Calculator::calcBiasDetail() {
  vector<BiasDetail> biases;

  vector<pair<int,int>> surrounding_s;
  for (const auto &s : board) {
    addSurroundingSquare(s.first, s.second, surrounding_s, board);
  }

  for (const auto &n : neurons) {
    float b_A, b_B, b_E, b_G;
    float a_A, a_B, a_E, a_G;
    b_A = b_B = b_E = b_G = a_A = a_B = a_E = a_G = 0;

    auto squares = n.getSquares();
    int pn = squares.size();

    b_A = 0.5 * parameter.A;
    a_A = 0.5 * parameter.A;

    b_B = 0.5 * parameter.B * pn;
    a_B = 0.5 * parameter.B * pn;

    int wall_edge = calcConnectWall(squares, surrounding_s);

    b_E = parameter.E * wall_edge;
    a_E = parameter.E * wall_edge;

    b_G = - parameter.G / n.getSize();
    a_G = - parameter.G / n.getSize();

    BiasDetail bd = {b_A, b_B, b_E, b_G, a_A, a_B, a_E, a_G};
    biases.emplace_back(bd);
  }
  return biases;
}

vector<WeightDetail> Calculator::calcWeightDetail(const uint32_t neuron_id) {
  vector<WeightDetail> weights;

  for (uint32_t i = 0; i < neurons.size(); ++i) {
    if (neuron_id == i) continue;

    float b_A, b_B, b_C, b_D, b_F;
    float a_A, a_B, a_C, a_D, a_F;
    b_A = b_B = b_C = b_D = b_F = a_A = a_B = a_C = a_D = a_F = 0;

    if(neurons[neuron_id].getPieceId() == neurons[i].getPieceId()){
      b_A = - parameter.A;
      a_A = - parameter.A;
    }

    int overlap_point = calcOverlapPoint(neurons[neuron_id], neurons[i]);
    b_B = -parameter.B * overlap_point;
    a_B = -parameter.B * overlap_point;

    vector<pair<int,int>> sum_squares(neurons[neuron_id].getSquares());
    auto temp = neurons[i].getSquares();
    copy(temp.begin(), temp.end(), back_inserter(sum_squares));

    if (checkBubble(board, sum_squares, parameter.cut_bubble_size)) {
      b_C = -parameter.C;
      a_C = -parameter.C;
    }

    float overlap_edge = (float)calcConnectEdge(neurons[neuron_id], neurons[i], parameter.problem_type);
    b_D = parameter.D * overlap_edge;

    b_F = -parameter.F;

    if (!(b_A == 0 && b_B == 0 && b_C == 0 && b_D == 0 && b_F == 0) ||
        !(a_A == 0 && a_B == 0 && a_C == 0 && a_D == 0 && a_F == 0)) {
      WeightDetail wd = {i, b_A, b_B, b_C, b_D, b_F, a_A, a_B, a_C, a_D, a_F};
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
/*
  {  // problem type
    int type = 4;
    ofs.write((char *)&type, sizeof(int));
  }

  {  // bias
    auto bias_details = calcBiasDetail();
    vector<Bias> biases;

    for (const auto &bd : bias_details) {
      float b = bd.b_A + bd.b_B + bd.b_E + bd.b_G;
      float a = bd.a_A + bd.a_B + bd.a_E + bd.b_G;
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
        float b = wd.b_A + wd.b_B + wd.b_C + wd.b_D + wd.b_F;
        float a = wd.a_A + wd.a_B + wd.a_C + wd.a_D + wd.a_F;

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
*/
  {  // info board
    uint32_t size = board.size();
    ofs.write((char *)&size, sizeof(uint32_t));
    ofs.write((char *)&board[0], sizeof(pair<int,int>) * size);
  }

  {  // info neurons
    uint32_t size = neurons.size();
    ofs.write((char *)&size, sizeof(uint32_t));

    for (const auto &n : neurons) {
			int piece_id = n.getPieceId();
			ofs.write((char *)&piece_id, sizeof(int));

      auto squares = n.getSquares();
      uint32_t squares_size = squares.size();
      ofs.write((char *)&squares_size, sizeof(uint32_t));
      ofs.write((char *)&squares[0], sizeof(pair<int,int>) * squares_size);
    }
  }

  ofs.close();
}
