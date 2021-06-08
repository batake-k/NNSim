#include "polyomino.hpp"

#include <iostream>
#include <algorithm>

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
	piece_numbers.resize(neurons_size);

  for(uint32_t i=0; i<neurons_size; ++i){
		int piece_number;
		ifs.read((char*)&piece_number, sizeof(int));
		piece_numbers[i] = piece_number;

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

int Polyomino::getScore(const std::vector<float>& outputs){
  int score = 0;

	vector<int> numbers;
	vector<vector<int>> board(neurons[0].size(), vector<int>(neurons[0][0].size()));

	for(uint32_t i=0; i<outputs.size(); ++i){
		if(outputs[i] >= 0.5){
			numbers.emplace_back(piece_numbers[i]);
      addBoard(board, neurons[i]);
		}
	}

  int before_size = numbers.size();
	sort(numbers.begin(), numbers.end());
	numbers.erase(unique(numbers.begin(), numbers.end()), numbers.end());
  int after_size = numbers.size();
  score = after_size - before_size;

  for(const auto &b : board){
    for(const auto &bb : b){
      if(bb != 1) --score;
    }
  }

  return score;
  
}

string Polyomino::getGoalStatus(const vector<float> &outputs){
  int score = getScore(outputs);

  if(score == 0){
    return ",1," + to_string(score);
  }else{
    return ",0," + to_string(score);
  }
}
