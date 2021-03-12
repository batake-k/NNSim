#include "piece.hpp"

using namespace std;

#include <iostream>

Piece::Piece(std::vector<Hex> &piece_hexs,
		std::vector<string> &rotation_flags,
		std::vector<string> &inversion_flags,
		int _piece_id)
		:piece_id(_piece_id){
	// rotation
	for(int i=0; i<6; ++i){
		for(auto& h : piece_hexs){
			h.rotate60();
		}
		if(rotation_flags[i] == "1"){
			states.emplace_back(piece_hexs);
		}
	}

	// inverse
	for(auto& h : piece_hexs){
		h.inverse();
	}

	for(int i=0; i<6; ++i){
		for(auto& h : piece_hexs){
			h.rotate60();
		}
		if(inversion_flags[i] == "1"){
			states.emplace_back(piece_hexs);
		}
	}
}

void Piece::writeAllState(){
	for(int i=0; i<states.size(); ++i){
		cout << "state: " << i << endl;
		for(auto h : states[i]){
			h.write();
		}
	}
	cout << endl;
}
