#include "hex.hpp"
#include "utils.hpp"

#include <fstream>
#include <algorithm>

using namespace std;

Hex::Hex(const string& input_file){
	ifstream ifs(input_file);

	string line;
	getline(ifs,line);

	vector<string> splited_line = utils::split(line, ',');
	num_neurons = stoi(splited_line[1]);
	num_tiles = stoi(splited_line[2]);

	neurons_info.resize(num_neurons);

	getline(ifs,line);
	getline(ifs,line);

	if(line != "-1"){
		cout << "[ERROR] wrong format: " << input_file << endl;
		exit(1);
	}

	getline(ifs,line);

	int num_surrounding_tiles = stoi(line);
	for(int i=0; i<num_surrounding_tiles; ++i){
		getline(ifs,line);
	}

	getline(ifs,line);

	for(int i=0; i<num_neurons; ++i){
		getline(ifs,line);
		getline(ifs,line);
		int size = stoi(line);

		for(int j=0; j<size; ++j){
			getline(ifs,line);
			vector<string> splited = utils::split(line, ',');
			neurons_info[i].emplace_back(make_pair(stoi(splited[0]), stoi(splited[1])));
		}
		getline(ifs,line);
	}

	ifs.close();
}

bool Hex::isGoal(const std::vector<float>& outputs){
	vector<pair<int,int>> tiles;

	for(int i=0; i<num_neurons; ++i){
		if(outputs[i] >= 0.5){
			for(const auto& e : neurons_info[i]){
				tiles.emplace_back(e);
			}
		}
	}

	int size1 = tiles.size();

	if(size1 != num_tiles){
		return false;
	}

	std::sort(tiles.begin(), tiles.end());
	tiles.erase(std::unique(tiles.begin(), tiles.end()), tiles.end());

	if(size1 == (int)tiles.size()){
		return true;
	}else{
		return false;
	}
}
