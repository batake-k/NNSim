#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include <string>
#include <vector>

#include "cube.hpp"

class Generator {
public:
  struct Parameter {
    int space_size;
    std::string polycube_sizes;
    std::string output_path;
  };

  Generator(Parameter p) : parameter(p){};
  Generator(){};
  ~Generator(){};

  void run();
  void writeData();

private:
  Parameter parameter;

  std::vector<Cube> space_cubes;
  std::vector<std::vector<Cube>> polycubes;
};

#endif
