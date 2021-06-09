/**
 * neuronに対応するクラスを記述
 * @author Kazuki Takabatake
 * @date 2021/6/9
 */

#ifndef NEURON_HPP
#define NEURON_HPP

#include <vector>
#include <fstream>
#include <iostream>

#include "cube.hpp"

class Neuron {
public:
  Neuron(std::vector<Cube>& _cubes, int _id) : cubes(_cubes), id(_id){};
  Neuron(){};
  ~Neuron(){};

  void write(std::ofstream& ofs) const;

  int getSize() const { return cubes.size(); }
  int getId() const { return id; }
  std::vector<Cube> getCubes() const { return cubes; }

private:
  std::vector<Cube> cubes;
  int id;
};

int calcOverlap(const Neuron& neuron_i, const Neuron& neuron_j);
int calcConnectFace(const Neuron& neuron_i, const Neuron& neuron_j);

#endif
