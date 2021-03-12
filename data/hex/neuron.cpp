#include "neuron.hpp"

#include <algorithm>

using namespace std;

void Neuron::write(ofstream &ofs) const {
	ofs << piece_id << endl;
	ofs << hexs.size() << endl;
	for(const auto& h : hexs){
		h.write(ofs);
	}
	ofs << endl;
}

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j) {
	int overlap = 0;

	auto i_hexs = neuron_i.getHexs();
	auto j_hexs = neuron_j.getHexs();

	for(const auto& i : i_hexs){
		if(find(j_hexs.begin(), j_hexs.end(), i) != j_hexs.end()){
			++overlap;
		}
	}

	return overlap;
}

int calcOverlapPoint(const Neuron& neuron_i, const Neuron& neuron_j) {
	int point = 0;

	auto i_hexs = neuron_i.getHexs();
	auto j_hexs = neuron_j.getHexs();

	for(const auto& i : i_hexs){
		if(find(j_hexs.begin(), j_hexs.end(), i) != j_hexs.end()){
			point += i.getPoint();
		}
	}

	return point;
}

int calcConnectEdge(const Neuron& neuron_i, const Neuron& neuron_j) {
	int edge = 0;

	if(calcOverlap(neuron_i, neuron_j) > 0){
		return 0;
	}

	auto i_hexs = neuron_i.getHexs();
	auto j_hexs = neuron_j.getHexs();

	for(const auto& i : i_hexs){
		for(const auto& j : j_hexs){
			if(i.isConnect(j)){
				++edge;
			}
		}
	}

	return edge;
}
