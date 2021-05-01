#ifndef HEX_HPP
#define HEX_HPP

#include "problem.hpp"

#include <fstream>

typedef struct {
  int point;
  int x;
  int y;

  operator==(const HexStruct &other) const{
    return (x == other.x && y == other.y);
  }
} HexStruct;

class Hex : public Problem {
public:
	Hex(std::ifstream &ifs);
	Hex(){};
	~Hex(){};

	bool isGoal(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
	std::vector<HexStruct> board;
  std::vector<std::vector<HexStruct>> neurons;
};

#endif
