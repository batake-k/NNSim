#ifndef NEURAL_NETWORK_MODEL_BINARY_READER_HPP
#define NEURAL_NETWORK_MODEL_BINARY_READER_HPP

#include "neural_network_model_reader_base.hpp"
#include "for_problems/polyomino.hpp"

class NeuralNetworkModelBinaryReader : public NeuralNetworkModelReaderBase {
public:
  NeuralNetworkModelBinaryReader() {};
  void open(const std::string& input_path);
  int read_problem_type();
  std::vector<Bias> read_biases();
  std::vector<std::vector<Weight>> read_weights();
  Polyomino read_polyomino();
};

#endif