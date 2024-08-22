#include "neural_network_model_binary_reader.hpp"
#include "utils.hpp"

void NeuralNetworkModelBinaryReader::open(const std::string& filepath) {
  utils::fileOpen(this->ifs, filepath, std::ios::in | std::ios::binary);
}

int NeuralNetworkModelBinaryReader::read_problem_type() {
  int problem_type;
  ifs.read((char*)&problem_type, sizeof(int));
  return problem_type;
}
std::vector<Bias> NeuralNetworkModelBinaryReader::read_biases() {
  uint32_t size;
  std::vector<Bias> biases;
  ifs.read((char*)&size, sizeof(uint32_t));
  biases.resize(size);
  ifs.read((char*)&biases[0], sizeof(Bias) * size);
  return biases;
}

std::vector<std::vector<Weight>> NeuralNetworkModelBinaryReader::read_weights() {
  std::vector<std::vector<Weight>> weights;
  uint32_t neuron_size, size;
  ifs.read((char*)&neuron_size, sizeof(uint32_t));
  weights.resize(neuron_size);

  for (uint32_t i = 0; i < neuron_size; ++i) {
    ifs.read((char*)&size, sizeof(uint32_t));
    weights[i].resize(size);
    ifs.read((char*)&weights[i][0], sizeof(Weight) * size);
  }
  return weights;
}

Polyomino NeuralNetworkModelBinaryReader::read_polyomino() {
  return Polyomino(ifs);
}