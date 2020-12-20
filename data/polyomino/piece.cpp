#include "piece.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

namespace{

	vector<vector<int>> inverse(vector<vector<int>>& before){
		vector<vector<int>> after(before.size(), vector<int>(before[0].size()));

		for(int x=0; x<after.size(); ++x){
			for(int y=0; y<after[0].size(); ++y){
				after[x][y] = before[after.size()-x-1][y];
			}
		}

		return after;
	}

	vector<vector<int>> rotate90(vector<vector<int>>& before){
		vector<vector<int>> after(before[0].size(), vector<int>(before.size()));

		for(int x=0; x<after.size(); ++x){
			for(int y=0; y<after[0].size(); ++y){
				after[x][y] = before[before.size()-y-1][x];
			}
		}

		return after;
	}

}

Piece::Piece(vector<vector<int>>& default_state, int piece_number, bool rotation, bool inversion){
	number = piece_number;

	int piece_size = 0;
	for(const auto& vv : default_state){
		for(const auto& v : vv){
			piece_size += v;
		}
	}
	size = piece_size;

	PieceState state(default_state);
	states.emplace_back(state);

	if(rotation){
		vector<vector<int>> rotate_90 = rotate90(default_state);
		PieceState state_90(rotate_90);
		if(find(states.begin(), states.end(), state_90) == states.end()){
			states.emplace_back(state_90);
		}
		vector<vector<int>> rotate_180 = rotate90(rotate_90);
		PieceState state_180(rotate_180);
		if(find(states.begin(), states.end(), state_180) == states.end()){
			states.emplace_back(state_180);
		}
		vector<vector<int>> rotate_270 = rotate90(rotate_180);
		PieceState state_270(rotate_270);
		if(find(states.begin(), states.end(), state_270) == states.end()){
			states.emplace_back(state_270);
		}
	}

	if(inversion){
	
		vector<vector<int>> inv = inverse(default_state);
		PieceState state_inv(inv);
		if(find(states.begin(), states.end(), state_inv) == states.end()){
			states.emplace_back(state_inv);
		}

		if(rotation){
			vector<vector<int>> inv_rotate_90 = rotate90(inv);
			PieceState state_inv_90(inv_rotate_90);
			if(find(states.begin(), states.end(), state_inv_90) == states.end()){
				states.emplace_back(state_inv_90);
			}
			vector<vector<int>> inv_rotate_180 = rotate90(inv_rotate_90);
			PieceState state_inv_180(inv_rotate_180);
			if(find(states.begin(), states.end(), state_inv_180) == states.end()){
				states.emplace_back(state_inv_180);
			}
			vector<vector<int>> inv_rotate_270 = rotate90(inv_rotate_180);
			PieceState state_inv_270(inv_rotate_270);
			if(find(states.begin(), states.end(), state_inv_270) == states.end()){
				states.emplace_back(state_inv_270);
			}
		}
	}
}

void PieceState::outputState() const{
	for(const auto& vv : state){
		for(const auto& v : vv){
			cout << v;
		}
		cout << endl;
	}
	cout << endl;
}

void Piece::outputStates() const{
	cout << "Piece Number: " << number << endl;

	for(const auto& piece_state : states){
		piece_state.outputState();
	}
	cout << endl;
}
