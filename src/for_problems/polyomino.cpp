#include "polyomino.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

Polyomino::Polyomino(ifstream &ifs) {

  {  // info board
    uint32_t size;
    ifs.read((char *)&size, sizeof(uint32_t));

    board.resize(size);
    ifs.read((char *)&board[0], sizeof(pair<int,int>) * size);
  }

  {  // info neurons
    uint32_t neurons_size;
    ifs.read((char *)&neurons_size, sizeof(uint32_t));
    neurons.resize(neurons_size);
		piece_ids.resize(neurons_size);

    for (uint32_t i = 0; i < neurons_size; ++i) {
			ifs.read((char *)&piece_ids[i], sizeof(int));

      uint32_t size;
      ifs.read((char *)&size, sizeof(uint32_t));
      neurons[i].resize(size);
      ifs.read((char *)&neurons[i][0], sizeof(pair<int,int>) * size);
    }
  }

  ifs.close();
}

int Polyomino::getPieceScore(const std::vector<float> &outputs) {
	vector<int> used_piece_ids;

  for (uint32_t i = 0; i < outputs.size(); ++i) {
    if (outputs[i] >= 0.5) {
			used_piece_ids.emplace_back(piece_ids[i]);
    }
  }

	int before_size = used_piece_ids.size();
	sort(used_piece_ids.begin(), used_piece_ids.end());
	used_piece_ids.erase(unique(used_piece_ids.begin(), used_piece_ids.end()), used_piece_ids.end());
	int after_size = used_piece_ids.size();

  return after_size - before_size;
}

int Polyomino::getScore(const std::vector<float> &outputs) {
  int score = 0;

  if (outputs.size() != neurons.size()) {
    cerr << "[ERROR] neurons size differ from outputs size" << endl;
    exit(1);
  }

  vector<pair<int,int>> tiles;

  for (uint32_t i = 0; i < outputs.size(); ++i) {

    if (outputs[i] >= 0.5) {

      for (const auto &n : neurons[i]) {

        // 重なりがある
        if (find(tiles.begin(), tiles.end(), n) != tiles.end()) {
          --score;
        } else {
          tiles.emplace_back(n);
        }
      }
    }
  }

  for (const auto &b : board) {

    if (find(tiles.begin(), tiles.end(), b) == tiles.end()) {
      --score;
    }
  }

  return score;
}

string Polyomino::getGoalStatus(const vector<float> &outputs) {
  int count = 0;
  for (const auto o : outputs) {
    if (o >= 0.5) ++count;
  }

  int score = getScore(outputs);
	int piece_score = getPieceScore(outputs);

	int count_3 = 0;
	int count_4 = 0;
	int count_5 = 0;
	int count_6 = 0;

	for (uint32_t i=0; i<=73; ++i){
		if (outputs[i] >= 0.5) ++count_3;
	}
	for (uint32_t i=74; i<=192; ++i){
		if (outputs[i] >= 0.5) ++count_4;
	}
	for (uint32_t i=193; i<=383; ++i){
		if (outputs[i] >= 0.5) ++count_5;
	}
	for (uint32_t i=384; i<=692; ++i){
		if (outputs[i] >= 0.5) ++count_6;
	}

	return "," + to_string(count) + "," + to_string(score) + "," + to_string(piece_score) + "," + to_string(count_3) + "," + to_string(count_4) + "," + to_string(count_5) + "," + to_string(count_6);
}
