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
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
  std::vector<std::vector<std::vector<int>>> neurons;
  std::vector<int> piece_numbers;
};

#endif
