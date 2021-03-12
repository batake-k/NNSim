/**
 * neuronに対応するクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#ifndef NEURON_HPP
#define NEURON_HPP

#include "hex.hpp"

#include <vector>
#include <iostream>
#include <fstream>

class Neuron {
public:
	Neuron() {};
	~Neuron() {};

	Neuron(std::vector<Hex> &_hexs, int _piece_id)
		:hexs(_hexs),piece_id(_piece_id) {};

	void write(std::ofstream &ofs) const;
	int getSize() const {
		return hexs.size();
	}
	std::vector<Hex> getHexs() const {
		return hexs;
	}

private:
	std::vector<Hex> hexs;
	int piece_id;
};

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j);
int calcOverlapPoint(const Neuron& neuron_i, const Neuron& neuron_j);

int calcConnectEdge(const Neuron& neuron_i, const Neuron& neuron_j);

#endif
