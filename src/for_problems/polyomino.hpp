#ifndef POLYOMINO_HPP
#define POLYOMINO_HPP

#include "problem.hpp"

#include <fstream>

class Polyomino : public Problem {
public:
	Polyomino(std::ifstream &ifs);
	Polyomino(){};
	~Polyomino(){};

	bool isGoal(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
  std::vector<std::vector<std::vector<int>>> neurons;
};

#endif
