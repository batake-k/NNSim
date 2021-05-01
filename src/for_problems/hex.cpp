#include "hex.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

Hex::Hex(ifstream &ifs){
  
	{ // info board
    uint32_t size;
    ofs.read((char*)&size, sizeof(uint32_t));

    board.resize(size);
    ofs.read((char*)&board[0], sizeof(HexStruct)*size);
  }

	{ // info neurons
    uint32_t neurons_size;
    ofs.read((char*)&neurons_size, sizeof(uint32_t));
    neurons.resize(neurons_size);

    for(uint32_t i=0; i<neurons_size; ++i){
      uint32_t size;
      ofs.read((char*)&size, sizeof(uint32_t));
      neurons[i].resize(size);
      ofs.read((char*)&neurons[i][0], sizeof(HexStruct)*size);
    }
  }

	ifs.close();
}

bool Hex::isGoal(const std::vector<float>& outputs){
  if(outputs.size() != neurons.size()){
    cerr << "[ERROR] neurons size differ from outputs size" << endl;
    exit(1);
  }

	vector<HexStruct> tiles;

	for(int i=0; i<outputs.size(); ++i){
		if(outputs[i] >= 0.5){

			for(const auto &n : neurons[i]){

        // 重なりがあるか判断
				if(find(tiles.begin(), tiles.end(), n) != tiles.end()){
          // 重なってはならない場所か判断
          if(n.point != 0) return false;
        }else{
          tiles.emplace_back(n);
        }
			}
		}
	}

	for(const auto &b : board){

    if(b.point != 0) continue;

    if(find(tiles.begin(), tiles.end(), b) == tiles.end()) return false;
  }

  return true;
}

string Hex::getGoalStatus(const vector<float> &outputs){
  int count = 0;
  for(const auto o : outputs){
    if(o >= 0.5) ++count;
  }

  if(isGoal(outputs)){
    return ",1," + to_string(count);
  }else{
    return ",0," + to_string(count);
  }
}
