#ifndef GENERATOR_HPP
#define GENERATOR_HPP

#include "hex.hpp"

#include <string>
#include <vector>

class Generator {
public:

  struct Parameter {
    int board_size;
    std::string piece_sizes;
    std::string output_path;
  };

  Generator(Parameter p):parameter(p) {};
  Generator(){};
  ~Generator(){};

  void run();
  void writeData();

private:

  Parameter parameter;

  std::vector<Hex> board;
  std::vector<std::vector<Hex>> pieces;
};

#endif
