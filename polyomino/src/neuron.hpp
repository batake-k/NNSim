/**
 * neuronに対応するクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/7/26
 */

#ifndef NEURON_HPP
#define NEURON_HPP

#include <fstream>
#include <iostream>
#include <vector>

class Neuron {
public:
  Neuron(std::vector<std::pair<int,int>>& _squares, int _piece_id) : squares(_squares), piece_id(_piece_id){};
  Neuron(){};
  ~Neuron(){};

  void write(std::ofstream& ofs) const;

  int getSize() const { return squares.size(); }
  int getPieceId() const { return piece_id; }
  std::vector<std::pair<int,int>> getSquares() const { return squares; }

private:
  std::vector<std::pair<int,int>> squares;
  int piece_id;
};

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j);
int calcConnectEdge(const Neuron& neuron_i, const Neuron& neuron_j, const int problem_type);
int calcOverlapPoint(const Neuron& neuron_i, const Neuron& neuron_j);

#endif
