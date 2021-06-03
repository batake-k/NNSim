#include "hex.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

Hex::Hex(ifstream &ifs){
  
	{ // info board
    uint32_t size;
    ifs.read((char*)&size, sizeof(uint32_t));

    board.resize(size);
    ifs.read((char*)&board[0], sizeof(HexStruct)*size);
  }

	{ // info neurons
    uint32_t neurons_size;
    ifs.read((char*)&neurons_size, sizeof(uint32_t));
    neurons.resize(neurons_size);

    for(uint32_t i=0; i<neurons_size; ++i){
      uint32_t size;
      ifs.read((char*)&size, sizeof(uint32_t));
      neurons[i].resize(size);
      ifs.read((char*)&neurons[i][0], sizeof(HexStruct)*size);
    }
  }

	ifs.close();
}

int Hex::getScore(const std::vector<float>& outputs){
	int score = 0;

  if(outputs.size() != neurons.size()){
    cerr << "[ERROR] neurons size differ from outputs size" << endl;
    exit(1);
  }

	vector<HexStruct> tiles;

	for(uint32_t i=0; i<outputs.size(); ++i){
		if(outputs[i] >= 0.5){

			for(const auto &n : neurons[i]){

        // 重なりがある
				if(find(tiles.begin(), tiles.end(), n) != tiles.end()){
          // 重なってはならない場所
          if(n.point != 0){
						--score;
					}
        }else{
          tiles.emplace_back(n);
        }
			}
		}
	}

	for(const auto &b : board){

    if(b.point == 0) continue;

    if(find(tiles.begin(), tiles.end(), b) == tiles.end()){
			--score;
		}
  }

  return score;
}

string Hex::getGoalStatus(const vector<float> &outputs){
  int count = 0;
  for(const auto o : outputs){
    if(o >= 0.5) ++count;
  }

	int score = getScore(outputs);

  if(score == 0){
    return ",1," + to_string(count) + "," + to_string(score);
  }else{
    return ",0," + to_string(count) + "," + to_string(score);
  }
}
