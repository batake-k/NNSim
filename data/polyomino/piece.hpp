#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

class PieceState{
public:
	PieceState() {};
	~PieceState() {};

	PieceState(std::vector<std::vector<int>>& _state):
		state(_state) {};

	bool operator==(const PieceState& other) const{
		std::vector<std::vector<int>> other_state = other.getState();
		if(other_state.size() != this->state.size() || other_state[0].size() != this->state[0].size()){
			return false;
		}

		for(int x=0; x<this->state.size(); ++x){
			for(int y=0; y<this->state[0].size(); ++y){
				if(this->state[x][y] != other_state[x][y]){
					return false;
				}
			}
		}
		return true;
	}

	inline std::vector<std::vector<int>> getState() const { return state; };

	void outputState() const;

private:
	std::vector<std::vector<int>> state;
};



class Piece{
public:
	Piece() {};
	~Piece() {};

	Piece(std::vector<std::vector<int>>& default_state, int piece_number, bool rotation, bool inversion);

	inline std::vector<PieceState> getStates() const { return states; };
	inline int getNumber() const { return number; };
	inline int getSize() const { return size; };

	void outputStates() const;

private:
	std::vector<PieceState> states;
	int number;
	int size;
};

#endif
