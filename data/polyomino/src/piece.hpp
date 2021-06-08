/**
 * ポリオミノの情報を持つクラスを記述
 * @author Kazuki Takabatake
 * @date 2020/12/20
 */
#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>

// ポリオミノの一つの状態に対応するクラス
class PieceState {
public:
  PieceState(){};
  ~PieceState(){};

  PieceState(std::vector<std::vector<int>>& _state) : state(_state){};

  bool operator==(const PieceState& other) const {
    std::vector<std::vector<int>> other_state = other.getState();
    if (other_state.size() != this->state.size() || other_state[0].size() != this->state[0].size()) {
      return false;
    }

    for (int x = 0; x < (int)(this->state.size()); ++x) {
      for (int y = 0; y < (int)(this->state[0].size()); ++y) {
        if (this->state[x][y] != other_state[x][y]) {
          return false;
        }
      }
    }
    return true;
  }

  inline std::vector<std::vector<int>> getState() const { return state; };

  void outputState() const;

private:
  std::vector<std::vector<int>> state;  // 状態
};

// 各ポリオミノに対応するクラス
class Piece {
public:
  Piece(){};
  ~Piece(){};

  Piece(std::vector<std::vector<int>>& default_state, int piece_number, bool rotation, bool inversion);

  inline std::vector<PieceState> getStates() const { return states; };
  inline int getNumber() const { return number; };
  inline int getSize() const { return size; };
  inline int getNumEdges() const { return num_edges; };

  void outputStates() const;

private:
  std::vector<PieceState> states;  // 全状態の情報
  int number;                      // ポリオミノのid
  int size;                        // 大きさ
  int num_edges;                   // 辺の数
};

#endif
