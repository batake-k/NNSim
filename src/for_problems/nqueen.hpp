#ifndef NQUEEN_HPP
#define NQUEEN_HPP

#include <fstream>

#include "problem.hpp"

class Nqueen : public Problem {
public:
  Nqueen(std::ifstream &ifs);
  Nqueen(){};
  ~Nqueen(){};

  bool isGoal(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
  int N;
};

#endif
