#ifndef POLYOMINO_HPP
#define POLYOMINO_HPP

#include <fstream>

#include "problem.hpp"

class Polyomino : public Problem {
public:
  Polyomino(std::ifstream &ifs);
  Polyomino(){};
  ~Polyomino(){};

  int getScore(const std::vector<float> &outputs);
  int getPieceScore(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
	std::vector<int> piece_ids;
  std::vector<std::pair<int,int>> board;
  std::vector<std::vector<std::pair<int,int>>> neurons;
};

#endif
