#include "calculator.hpp"

#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>

using namespace std;

namespace{

  std::vector<std::string> split(const std::string &str, const char delim){
    std::vector<std::string> elems;
    std::string item;
    for(char ch: str){
      if(ch == delim){
        elems.push_back(item);
        item.clear();
      }else{
        item += ch;
      }
    }

    elems.push_back(item);
    return elems;
  }

  void readInputFile(const string &input_file, vector<Hex> &board, vector<Piece> &pieces){
    ifstream ifs(input_file);
    string line;

    // board
    getline(ifs, line);
    int board_size = stoi(line);

    for(int i=0; i<board_size; ++i){
      getline(ifs, line);
      auto splited_line = split(line, ',');
      board.emplace_back(Hex(stoi(splited_line[0]), stoi(splited_line[1]), stoi(splited_line[2])));
    }
    getline(ifs, line);

    // piece
    getline(ifs, line);
    int num_pieces = stoi(line);

    for(int i=0; i<num_pieces; ++i){
      getline(ifs, line);
      int piece_size = stoi(line);
      vector<Hex> piece_hexs;

      for(int j=0; j<piece_size; ++j){
        getline(ifs, line);
        auto splited_line = split(line, ',');
        piece_hexs.emplace_back(Hex(0, stoi(splited_line[0]), stoi(splited_line[1])));
      }
      Piece piece(piece_hexs, i);
      pieces.emplace_back(piece);
      getline(ifs, line);
    }
  }

  void addSurroundingHex(const int x, const int y, vector<Hex> &surrounding_hex,
      const vector<Hex> &hexs, const vector<Hex> &board = vector<Hex>()){
    vector<Hex> possible_hexs{
      Hex(0, x + 1, y + 1),
      Hex(0, x + 2, y),
      Hex(0, x + 1, y - 1),
      Hex(0, x - 1, y - 1),
      Hex(0, x - 2, y),
      Hex(0, x - 1, y + 1)
    };

    for(auto &h : possible_hexs){
      if(find(surrounding_hex.begin(), surrounding_hex.end(), h) != surrounding_hex.end()
          || find(hexs.begin(), hexs.end(), h) != hexs.end()){
        continue;
      }

      if(!board.empty()){
        auto iter = find(board.begin(), board.end(), h);
        if(iter == board.end()) continue;
        h.setPoint(iter->getPoint());
      }

      surrounding_hex.emplace_back(h);
    }
  }

  bool checkBubble(const vector<Hex> &board, const vector<Hex> &hexs, const int cut_bubble_size){
    vector<Hex> surrounding_hex;

    // hexsの周りのバブルになり得るhexを列挙
    for(const auto &h : hexs){
      addSurroundingHex(h.getX(), h.getY(), surrounding_hex, hexs, board);
    }

    // 周りのhexそれぞれから周辺のhexを付加していき、バブルかどうか判定
    for(const auto &h : surrounding_hex){
      vector<Hex> bubble_hex;
      bubble_hex.emplace_back(h);

      for(uint32_t i=0; i<bubble_hex.size(); ++i){
        addSurroundingHex(bubble_hex[i].getX(), bubble_hex[i].getY(), bubble_hex, hexs, board);
        if(bubble_hex.size() > (uint32_t)cut_bubble_size) break;
      }

      if(bubble_hex.size() <= (uint32_t)cut_bubble_size){
        int point = 0;
        for(const auto &bh : bubble_hex){
          point += bh.getPoint();
        }
        if(point > 0) return true;
      }
    }
    return false;
  }

  int calcConnectWall(const vector<Hex> &hexs, const vector<Hex> &walls){
    int num = 0;

    for(const auto &h : hexs){
      if(h.getPoint() == 0) continue;

      for(const auto &w : walls){
        if(h.isConnect(w)) ++num;
      }
    }
    return num;
  }

};

Calculator::Calculator(Parameter &_parameter):parameter(_parameter){
  try{
    readInputFile(parameter.input_file, board, pieces);
  }catch(const exception &e){
    std::cerr << "[ERROR] " << e.what() << std::endl;
    exit(1); 
  }

  for(const auto &piece : pieces){
    auto states = piece.getStates();

    for(const auto &state : states){
      for(const auto &board_hex : board){
        vector<Hex> new_state(state);
        bool can_be_neuron = true;
        int point = 0;

        for(auto &h : new_state){
          h += board_hex;
          auto iter = find(board.begin(), board.end(), h);

          if(iter == board.end()){
            can_be_neuron = false;
            break;
          }else{
            int p = iter->getPoint();
            point += p;
            h.setPoint(p);
          }
        }

        if(point == 0) can_be_neuron = false;
        if(can_be_neuron && checkBubble(board, new_state, parameter.cut_bubble_size)) can_be_neuron = false;
        if(can_be_neuron){
          neurons.emplace_back(new_state, piece.getPieceId());
        }
      }
    }
  }

  cout << "board size:  " << board.size() << endl
       << "num pieces:  " << pieces.size() << endl
       << "num neurons: " << neurons.size() << endl;
}

void Calculator::writeInfo(){
  ofstream ofs(parameter.output_file + "_info");
  ofs << pieces.size() << "," << neurons.size() << "," << board.size() << endl << endl;

  vector<Hex> surrounding_hexs;
  for(const auto &h : board){
    addSurroundingHex(h.getX(), h.getY(), surrounding_hexs, board);
  }

  ofs << surrounding_hexs.size() << endl;
  for(const auto &h : surrounding_hexs){
    ofs << h.getStatus() << endl;
  }
  ofs << endl;

  for(const auto &n : neurons){
    n.write(ofs);
  }
  ofs.close();
}

vector<BiasDetail> Calculator::calcBiasDetail(){
  vector<BiasDetail> biases;

  for(const auto &n : neurons){
    float b_A, b_B, b_C, b_D, b_E;
    float a_A, a_B, a_C, a_D, a_E;
    b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = 0;

    auto hexs = n.getHexs();
    int pn = 0;

    for(const auto &h : hexs){
      pn += h.getPoint();
    }

    b_B = 0.5 * parameter.B * pn;
    a_B = 0.5 * parameter.B * pn;

    b_E = - parameter.E;
    a_E = - parameter.E;

    BiasDetail bd = {b_A, b_B, b_C, b_D, b_E, a_A, a_B, a_C, a_D, a_E};
    biases.emplace_back(bd);
  }
  return biases;
}

vector<WeightDetail> Calculator::calcWeightDetail(const uint32_t neuron_id){
  vector<WeightDetail> weights;

  for(uint32_t i=0; i<neurons.size(); ++i){
    if(neuron_id == i) continue;

    float b_A, b_B, b_C, b_D, b_E;
    float a_A, a_B, a_C, a_D, a_E;
    b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = 0;

    int overlap_point = calcOverlapPoint(neurons[neuron_id], neurons[i]);
    b_B = - parameter.B * overlap_point;
    a_B = - parameter.B * overlap_point;

    vector<Hex> sum_hexs(neurons[neuron_id].getHexs());
    auto temp = neurons[i].getHexs();
    copy(temp.begin(), temp.end(), back_inserter(sum_hexs));

    if(checkBubble(board, sum_hexs, parameter.cut_bubble_size)){
      b_C = - parameter.C;
      a_C = - parameter.C;
    }

    /*
    b_D = parameter.D * calcConnectEdge(neurons[neuron_id], neurond[i]);
    a_D = parameter.D * calcConnectEdge(neurons[neuron_id], neurond[i]);
    */

    WeightDetail wd = {i, b_A, b_B, b_C, b_D, b_E, a_A, a_B, a_C, a_D, a_E};
    weights.emplace_back(wd);
  }
  return weights;
}

void Calculator::writeDataDetail(){
  ofstream ofs(parameter.output_file + "_data_detail", ios::binary);
  uint32_t neurons_size = neurons.size();

  { // bias
    auto bias_details = calcBiasDetail();
    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&bias_details[0], sizeof(BiasDetail)*neurons_size);
  }

  { // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for(uint32_t i=0; i<neurons_size; ++i){
      auto weight_details = calcWeightDetail(i);
      uint32_t size = weight_details.size();
      ofs.write((char*)&size, sizeof(uint32_t));
      ofs.write((char*)&weight_details[0], sizeof(WeightDetail)*size);
    }
  }

  ofs.close();
}

void Calculator::writeData(){
  ofstream ofs(parameter.output_file + "_data", ios::binary);
  uint32_t neurons_size = neurons.size();

  { // problem type
    int type = 2;
    ofs.write((char*)&type, sizeof(int));
  }

  { // bias
    auto bias_details = calcBiasDetail();
    vector<Bias> biases;

    for(const auto &bd : bias_details){
      float b = bd.b_A + bd.b_B + bd.b_C + bd.b_D + bd.b_E;
      float a = bd.a_A + bd.a_B + bd.a_C + bd.a_D + bd.a_E;
      Bias bias = {b, a};
      biases.emplace_back(bias);
    }

    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&biases[0], sizeof(Bias)*neurons_size);
  }

  { // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for(uint32_t i=0; i<neurons_size; ++i){
      auto weight_details = calcWeightDetail(i);
      vector<Weight> weights;

      for(const auto &wd : weight_details){
        float b = wd.b_A + wd.b_B + wd.b_C + wd.b_D + wd.b_E;
        float a = wd.a_A + wd.a_B + wd.a_C + wd.a_D + wd.a_E;
        Weight weight = {wd.neuron_id, b, a};
        weights.emplace_back(weight);
      }

      uint32_t size = weights.size();
      ofs.write((char*)&size, sizeof(uint32_t));
      ofs.write((char*)&weights[0], sizeof(Weight)*size);
    }
  }

  { // info board
    uint32_t size = board.size();
    ofs.write((char*)&size, sizeof(uint32_t));

    vector<HexStruct> hs;
    for(const auto &h : board){
      HexStruct _hs = {h.getPoint(), h.getX(), h.getY()};
      hs.emplace_back(_hs);
    }

    ofs.write((char*)&hs[0], sizeof(HexStruct)*size);
  }

  { // info neurons
    uint32_t size = neurons.size();
    ofs.write((char*)&size, sizeof(uint32_t));

    for(const auto &n : neurons){
      auto hexs = n.getHexs();
      uint32_t hexs_size = hexs.size();
      ofs.write((char*)&hexs_size, sizeof(uint32_t));

      vector<HexStruct> hs;
      for(const auto &h : hexs){
        HexStruct _hs = {h.getPoint(), h.getX(), h.getY()};
        hs.emplace_back(_hs);
      }

      ofs.write((char*)&hs[0], sizeof(HexStruct)*hexs_size);
    }
  }

  ofs.close();
}
