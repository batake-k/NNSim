#ifndef HEX_HPP
#define HEX_HPP

#include "problem.hpp"

#include <fstream>

struct HexStruct{
  int point;
  int x;
  int y;

  bool operator==(const HexStruct &other) const{
    return (x == other.x && y == other.y);
  }
};

class Hex : public Problem {
public:
	Hex(std::ifstream &ifs);
	Hex(){};
	~Hex(){};

	int getScore(const std::vector<float> &outputs);
  std::string getGoalStatus(const std::vector<float> &outputs);

private:
	std::vector<HexStruct> board;
  std::vector<std::vector<HexStruct>> neurons;
};

#endif
