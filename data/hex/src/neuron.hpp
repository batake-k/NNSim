/**
 * neuronに対応するクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#ifndef NEURON_HPP
#define NEURON_HPP

#include <fstream>
#include <iostream>
#include <vector>

#include "hex.hpp"

class Neuron {
public:
  Neuron(std::vector<Hex>& _hexs, int _piece_id) : hexs(_hexs), piece_id(_piece_id){};
  Neuron(){};
  ~Neuron(){};

  void write(std::ofstream& ofs) const;

  int getSize() const { return hexs.size(); }
  int getPieceId() const { return piece_id; }
  std::vector<Hex> getHexs() const { return hexs; }

private:
  std::vector<Hex> hexs;
  int piece_id;
};

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j);
int calcConnectEdge(const Neuron& neuron_i, const Neuron& neuron_j);
int calcOverlapPoint(const Neuron& neuron_i, const Neuron& neuron_j);

#endif
