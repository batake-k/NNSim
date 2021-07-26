/**
 * 各ピースの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/7/26
 */

#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

class Piece {
public:
  Piece(std::vector<std::pair<int,int>> &piece_squares, int _piece_id);
  Piece(){};
  ~Piece(){};

  std::vector<std::vector<std::pair<int,int>>> getStates() const { return states; };
  int getPieceId() const { return piece_id; };

private:
  std::vector<std::vector<std::pair<int,int>>> states;
  int piece_id;
};

#endif
