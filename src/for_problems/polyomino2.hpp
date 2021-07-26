#ifndef POLYOMINO2_HPP
#define POLYOMINO2_HPP

#include <fstream>

#include "problem.hpp"

class Polyomino2 : public Problem {
public:
  Polyomino2(std::ifstream &ifs);
  Polyomino2(){};
  ~Polyomino2(){};

  int getScore(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
  std::vector<std::pair<int,int>> board;
  std::vector<std::vector<std::pair<int,int>>> neurons;
};

#endif
