/**
 * 各ピースの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

#include "hex.hpp"

class Piece {
public:
  Piece(std::vector<Hex> &piece_hexs, int _piece_id);
  Piece(){};
  ~Piece(){};

  std::vector<std::vector<Hex>> getStates() const { return states; };
  int getPieceId() const { return piece_id; };

private:
  std::vector<std::vector<Hex>> states;
  int piece_id;
};

#endif
