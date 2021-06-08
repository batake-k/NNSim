#include "calculator.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <utility>
#include <iostream>
#include <algorithm>

#include "piece.hpp"
#include "neuron.hpp"

using namespace std;

namespace {

/**
 * 文字列分割関数
 * @param str 分割前文字列
 * @param delim 分割文字
 * @return 分割された文字列のvector
 */
std::vector<std::string> split(const std::string& str, const char delim) {
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

/**
 * 入力ファイルから、盤面と各ピース（ポリオミノ）の情報を読み込む
 * @param ifs 入力ファイルストリーム
 * @param board 盤面
 * @param pieces 全ピース
 * @param rotation 回転フラグ
 * @param inversion 反転フラグ
 */
void readInputFile(const string& input_file, vector<vector<int>>& board, vector<Piece>& pieces, bool rotation,
                   bool inversion) {
  ifstream ifs(input_file);
  string line;

  getline(ifs, line);
  vector<string> splited_line = split(line, ',');

  int num_pieces = stoi(splited_line[0]);
  cout << "num pieces: " << num_pieces << endl;
  int boardx = stoi(splited_line[1]);
  int boardy = stoi(splited_line[2]);
  board.resize(boardx, vector<int>(boardy));
  cout << "board size: " << boardx << " by " << boardy << endl;

  for (int i = 0; i < num_pieces; ++i) {
    getline(ifs, line);
    getline(ifs, line);
    vector<string> piece_size = split(line, ',');
    int piecex = stoi(piece_size[0]);
    int piecey = stoi(piece_size[1]);
    vector<vector<int>> default_piece_state(piecex, vector<int>(piecey));

    for (int j = 0; j < piecex; ++j) {
      getline(ifs, line);
      vector<string> piece_column = split(line, ',');

      for (int k = 0; k < piecey; ++k) {
        default_piece_state[j][k] = stoi(piece_column[k]);
      }
    }

    Piece piece(default_piece_state, i, rotation, inversion);
    pieces.emplace_back(piece);
  }

  ifs.close();
}

/**
 * 盤面、ピース、位置からニューロンになるか判定し、盤面にピースを置く
 * @param board 盤面
 * @param piece ピース
 * @param x ピースを置くx座標
 * @param y ピースを置くy座標
 * @return ピースがおける場合はtrue,置けない場合はfalse
 */
bool generateNeuronBoard(vector<vector<int>>& board, vector<vector<int>>& piece, int x, int y) {
  for (uint32_t i = 0; i < piece.size(); ++i) {
    for (uint32_t j = 0; j < piece[0].size(); ++j) {
      if (piece[i][j] == 1) {
        if (x + i >= board.size() || y + j >= board[0].size()) {
          return false;
        } else {
          board[x + i][y + j] = 1;
        }
      }
    }
  }
  return true;
}

/**
 * 指定座標が盤面上で、かつピースが存在するか判定
 * @param board 盤面
 * @param x 指定するx座標
 * @param y 指定するy座標
 * @return ピースが存在する座標であればtrue,そうでなければfalse
 */
bool isEmpty(vector<vector<int>>& board, int x, int y) {
  if (x < 0 || x >= (int)board.size() || y < 0 || y >= (int)board[0].size()) {
  } else if (board[x][y] == 0) {
    board[x][y] = 1;
    return true;
  }
  return false;
}

/**
 * 盤面にバブル（指定数以下のピースや壁に囲まれた空間）が存在するかどうか判定
 * @param board 盤面
 * @param but_bubble_size 空間がこの値以下であればバブルと判定
 * @return バブルが存在する場合はtrue,存在しない場合はfalse
 */
bool checkBubble(const vector<vector<int>>& board, const int cut_bubble_size) {
  vector<vector<int>> check_board(board);

  for (uint32_t x = 0; x < check_board.size(); ++x) {
    for (uint32_t y = 0; y < check_board[0].size(); ++y) {
      if (check_board[x][y] == 0) {
        vector<pair<int, int>> cells;
        cells.emplace_back(make_pair(x, y));
        check_board[x][y] = 1;

        for (uint32_t i = 0; i < cells.size(); ++i) {
          int current_x = cells[i].first;
          int current_y = cells[i].second;

          if (isEmpty(check_board, current_x + 1, current_y)) {
            cells.emplace_back(make_pair(current_x + 1, current_y));
          }
          if (isEmpty(check_board, current_x - 1, current_y)) {
            cells.emplace_back(make_pair(current_x - 1, current_y));
          }
          if (isEmpty(check_board, current_x, current_y + 1)) {
            cells.emplace_back(make_pair(current_x, current_y + 1));
          }
          if (isEmpty(check_board, current_x, current_y - 1)) {
            cells.emplace_back(make_pair(current_x, current_y - 1));
          }
        }

        if (cells.size() <= (uint32_t)cut_bubble_size) {
          return true;
        }
      }
    }
  }
  return false;
}

};  // namespace

Calculator::Calculator(Parameter& _parameter) : parameter(_parameter) {
  try {
    readInputFile(parameter.input_file, board, pieces, parameter.rotation, parameter.inversion);
  } catch (const exception& e) {
    cerr << "[ERROR] " << e.what() << endl;
    exit(1);
  }

  uint32_t sum_piece_sizes = 0;

  for (const auto& s : pieces) {
    sum_piece_sizes += s.getSize();
  }

  if (board.size() * board[0].size() != sum_piece_sizes) {
    cout << "[ERROR] board cells: " << board.size() * board[0].size() << ", sum piece sizes: " << sum_piece_sizes
         << endl;
    exit(0);
  }

  vector<int> Pms(pieces.size(), 0);
  vector<int> Lns(board.size() * board[0].size(), 0);

  for (uint32_t i = 0; i < pieces.size(); ++i) {
    for (const auto& piece_state : pieces[i].getStates()) {
      vector<vector<int>> state = piece_state.getState();

      for (uint32_t x = 0; x < board.size(); ++x) {
        for (uint32_t y = 0; y < board[0].size(); ++y) {
          vector<vector<int>> current_board(board);
          bool is_generated = generateNeuronBoard(current_board, state, x, y);

          if (is_generated) {
            bool has_bubble = checkBubble(current_board, parameter.cut_bubble_size);

            if (!has_bubble) {
              ++Pms[pieces[i].getNumber()];

              for (uint32_t x2 = 0; x2 < current_board.size(); ++x2) {
                for (uint32_t y2 = 0; y2 < current_board[0].size(); ++y2) {
                  if (current_board[x2][y2] == 1) {
                    ++Lns[x2 * current_board[0].size() + y2];
                  }
                }
              }

              Neuron neuron(current_board, i, pieces[i].getSize(), pieces[i].getNumEdges());
              neurons.emplace_back(neuron);
            }
          }
        }
      }
    }
  }
  cout << "generate " << neurons.size() << " neurons" << endl;
}

void Calculator::writeInfo() {
  ofstream ofs(parameter.output_file + "_info");
  ofs << board.size() << "," << board[0].size() << "," << neurons.size() << endl << endl;

  for (auto& n : neurons) {
    n.write(ofs);
  }
  ofs.close();
}

vector<BiasDetail> Calculator::calcBiasDetail() {
  vector<BiasDetail> biases;

  for (const auto& n : neurons) {
    float b_A, b_B, b_C, b_D, b_E;
    float a_A, a_B, a_C, a_D, a_E;
    b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = 0;

    float ai = n.getSize();

    b_A = (parameter.gamma - 0.5) * parameter.A;
    a_A = 0.5 * parameter.A;

    b_B = (parameter.gamma - 0.5) * parameter.B * ai;
    a_B = 0.5 * parameter.B * ai;

    float wall_edge = countWallEdge(n);
    b_E = parameter.E * wall_edge;
    a_E = parameter.E * wall_edge;

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

    if (neurons[neuron_id].getPieceNumber() == neurons[i].getPieceNumber()) {
      b_A = -parameter.A;
      a_A = -parameter.A;
    }

    int overlap = calcOverlap(neurons[neuron_id], neurons[i]);
    b_B = -parameter.B * overlap;
    a_B = -parameter.B * overlap;

    if (checkBubble(neurons[neuron_id] + neurons[i], parameter.cut_bubble_size)) {
      b_C = -parameter.C;
      a_C = -parameter.C;
    }

    float overlap_edge = (float)countOverlapEdge(neurons[neuron_id], neurons[i]);

    b_D = parameter.D * overlap_edge;
    // a_D = parameter.D * overlap_edge;

    b_F = -parameter.F;

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
    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&bias_details[0], sizeof(BiasDetail) * neurons_size);
  }

  {  // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for (uint32_t i = 0; i < neurons_size; ++i) {
      auto weight_details = calcWeightDetail(i);
      uint32_t size = weight_details.size();
      ofs.write((char*)&size, sizeof(uint32_t));
      ofs.write((char*)&weight_details[0], sizeof(WeightDetail) * size);
    }
  }

  ofs.close();
}

void Calculator::writeData() {
  ofstream ofs(parameter.output_file + "_data", ios::binary);
  uint32_t neurons_size = neurons.size();

  {  // problem type
    int type = 1;
    ofs.write((char*)&type, sizeof(int));
  }

  {  // bias
    auto bias_details = calcBiasDetail();
    vector<Bias> biases;

    for (const auto& bd : bias_details) {
      float b = bd.b_A + bd.b_B + bd.b_C + bd.b_D + bd.b_E;
      float a = bd.a_A + bd.a_B + bd.a_C + bd.a_D + bd.a_E;
      Bias bias = {b, a};
      biases.emplace_back(bias);
    }

    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&biases[0], sizeof(Bias) * neurons_size);
  }

  {  // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for (uint32_t i = 0; i < neurons_size; ++i) {
      auto weight_details = calcWeightDetail(i);
      vector<Weight> weights;

      for (const auto& wd : weight_details) {
        float b = wd.b_A + wd.b_B + wd.b_C + wd.b_D + wd.b_E + wd.b_F;
        float a = wd.a_A + wd.a_B + wd.a_C + wd.a_D + wd.a_E + wd.a_F;

        if (b != 0 || a != 0) {
          Weight weight = {wd.neuron_id, b, a};
          weights.emplace_back(weight);
        }
      }

      uint32_t size = weights.size();
      ofs.write((char*)&size, sizeof(uint32_t));
      ofs.write((char*)&weights[0], sizeof(Weight) * size);
    }
  }

  {  // info neurons
    uint32_t size = neurons.size();
    ofs.write((char*)&size, sizeof(uint32_t));

    for (const auto& n : neurons) {
      int piece_number = n.getPieceNumber();
      ofs.write((char*)&piece_number, sizeof(int));

      auto b = n.getBoard();
      uint32_t b_size = b.size();
      ofs.write((char*)&b_size, sizeof(uint32_t));

      for (const auto& bb : b) {
        uint32_t bb_size = bb.size();
        ofs.write((char*)&bb_size, sizeof(uint32_t));
        ofs.write((char*)&bb[0], sizeof(int) * bb_size);
      }
    }
  }

  ofs.close();
}
