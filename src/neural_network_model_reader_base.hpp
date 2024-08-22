#ifndef NEURAL_NTWORK_MODEL_READER_BASE_HPP
#define NEURAL_NTWORK_MODEL_READER_BASE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>
#include "for_problems/polyomino.hpp"


typedef struct {
  float before_bias;
  float after_bias;
} Bias;

typedef struct {
  uint32_t neuron_id;
  float before_weight;
  float after_weight;
} Weight;

class NeuralNetworkModelReaderBase {
public:
  virtual void open(const std::string& input_path) = 0;
  virtual void close() { this->ifs.close(); };
  virtual int read_problem_type() = 0;
  virtual std::vector<Bias> read_biases() = 0;
  virtual std::vector<std::vector<Weight>> read_weights() = 0;
  virtual Polyomino read_polyomino() = 0;
  ~NeuralNetworkModelReaderBase() { this->close(); };
protected:
  std::ifstream ifs;
};

#endif