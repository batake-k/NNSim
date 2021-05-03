#include "polyomino.hpp"

#include <iostream>

using namespace std;

namespace {

  void addBoard(vector<vector<int>> &board, vector<vector<int>> &added){
    for(uint32_t i=0; i<board.size(); ++i){
      for(uint32_t j=0; j<board[0].size(); ++j){
        board[i][j] += added[i][j];
      }
    }
  }

};

Polyomino::Polyomino(ifstream &ifs){
  
  uint32_t neurons_size;
  ifs.read((char*)&neurons_size, sizeof(uint32_t));
  neurons.resize(neurons_size);

  for(uint32_t i=0; i<neurons_size; ++i){
    uint32_t size;
    ifs.read((char*)&size, sizeof(uint32_t));
    neurons[i].resize(size);

    for(uint32_t j=0; j<size; ++j){
      uint32_t s;
      ifs.read((char*)&s, sizeof(uint32_t));
      neurons[i][j].resize(s);
      ifs.read((char*)&neurons[i][j][0], sizeof(int)*s);
    }
  }

	ifs.close();
}

bool Polyomino::isGoal(const std::vector<float>& outputs){
  if(outputs.size() != neurons.size()){
    cerr << "[ERROR] neurons size differ from outputs size" << endl;
    exit(1);
  }

	vector<vector<int>> board(neurons[0].size(), vector<int>(neurons[0][0].size()));

	for(uint32_t i=0; i<outputs.size(); ++i){
		if(outputs[i] >= 0.5){
      addBoard(board, neurons[i]);
		}
	}

  for(const auto &b : board){
    for(const auto &bb : b){
      if(bb != 1) return false;
    }
  }

  return true;
}

string Polyomino::getGoalStatus(const vector<float> &outputs){
  if(isGoal(outputs)){
    return ",1";
  }else{
    return ",0";
  }
}
