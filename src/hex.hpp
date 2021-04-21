#ifndef HEX_HPP
#define HEX_HPP

#include <vector>
#include <string>

class Hex {
public:
	Hex(const std::string& input_file);
	Hex(){};
	~Hex(){};

	bool isGoal(const std::vector<float>& outputs);

private:
	int num_tiles;
	int num_neurons;
	std::vector<std::vector<std::pair<int,int>>> neurons_info;
};

#endif
