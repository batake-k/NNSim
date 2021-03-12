/**
 * 各ピースの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#ifndef PIECE_HPP
#define PIECE_HPP

#include "hex.hpp"

#include <vector>
#include <string>

class Piece {
public:
	Piece() {};
	~Piece() {};

	Piece(std::vector<Hex> &piece_hexs,
			std::vector<std::string> &rotation_flags,
			std::vector<std::string> &inversion_flags,
			int _piece_id);

	void writeAllState();
	inline std::vector<std::vector<Hex>> getStates() const { return states; };
	inline int getPieceId() const { return piece_id; };

private:
	std::vector<std::vector<Hex>> states;
	int piece_id;
};

#endif
