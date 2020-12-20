#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <iostream>
#include <fstream>

class NEURON {
public:

	std::vector<std::vector<int>> board;

	NEURON() {};
	~NEURON() {};

	NEURON(std::vector<std::vector<int>>& _board, int _piece_number):
		board(_board),piece_number(_piece_number) {};

	void Write(std::ofstream& ofs);
	inline int getPieceNumber() const {
		return piece_number;
	}

	std::vector<std::vector<int>> operator + (NEURON neuron) const {
		std::vector<std::vector<int>> sum_board(this->board);
		for(uint32_t i=0; i<this->board.size(); ++i){
			for(uint32_t j=0; j<this->board[0].size(); ++j){
				sum_board[i][j] += neuron.board[i][j];
			}
		}
		return sum_board;
	}

private:

	int piece_number;

};

int countOverlapEdge(const NEURON& i, const NEURON& j);
int CalcOverlap(const NEURON& i, const NEURON& j);

#endif
