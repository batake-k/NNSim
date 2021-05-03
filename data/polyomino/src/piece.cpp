/** 
 * ポリオミノの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2020/12/20
 */
#include "piece.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

namespace{

	/**
	 * 入力された状態を反転した状態を返す
	 * @param before 反転前の状態
	 * @return 反転後の状態
	 */
	vector<vector<int>> inverse(vector<vector<int>>& before){
		vector<vector<int>> after(before.size(), vector<int>(before[0].size()));

		for(int x=0; x<(int)after.size(); ++x){
			for(int y=0; y<(int)after[0].size(); ++y){
				after[x][y] = before[after.size()-x-1][y];
			}
		}

		return after;
	}

	/**
	 * 入力された状態を90度回転させた状態を返す
	 * @param before 回転前の状態
	 * @return 回転後の状態
	 */
	vector<vector<int>> rotate90(vector<vector<int>>& before){
		vector<vector<int>> after(before[0].size(), vector<int>(before.size()));

		for(int x=0; x<(int)after.size(); ++x){
			for(int y=0; y<(int)after[0].size(); ++y){
				after[x][y] = before[before.size()-y-1][x];
			}
		}

		return after;
	}

}

/**
 * Pieceクラスのコンストラクタ
 * @param default_state 初期状態、情報ファイルに記述されている状態
 * @param piece_number ポリオミノのid
 * @param rotation 回転を考慮するか
 * @param inversion 反転を考慮するか
 */
Piece::Piece(vector<vector<int>>& default_state, int piece_number, bool rotation, bool inversion){
	number = piece_number;

	int piece_size = 0;
	for(const auto& vv : default_state){
		for(const auto& v : vv){
			piece_size += v;
		}
	}
	size = piece_size;

	int number_of_edges = 0;
	for(int x=0; x<(int)default_state.size(); ++x){
		for(int y=0; y<(int)default_state[0].size(); ++y){
			if(default_state[x][y] != 1){
				continue;
			}

			if(x-1 < 0 || default_state[x-1][y] == 0){
				++number_of_edges;
			}
			if(x+1 >= (int)default_state.size() || default_state[x+1][y] == 0){
				++number_of_edges;
			}
			if(y-1 < 0 || default_state[x][y-1] == 0){
				++number_of_edges;
			}
			if(y+1 >= (int)default_state[0].size() || default_state[x][y+1] == 0){
				++number_of_edges;
			}
		}
	}
	num_edges = number_of_edges;

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

// 各状態を出力
void PieceState::outputState() const{
	for(const auto& vv : state){
		for(const auto& v : vv){
			cout << v;
		}
		cout << endl;
	}
	cout << endl;
}

// ピースの全状態を出力
void Piece::outputStates() const{
	cout << "Piece Number: " << number << endl;

	for(const auto& piece_state : states){
		piece_state.outputState();
	}
	cout << endl;
}
