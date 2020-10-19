#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include "neuron.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <random>

class NeuralNetworkModel {
public:

  struct Weight {
    int neuron_id;
    double weight;
  };

  NeuralNetworkModel();
  NeuralNetworkModel(int num_neurons, std::string weights_file, std::string bias_file, std::string output_file, int seed);
  virtual ~NeuralNetworkModel() {};

  virtual void update() {};
  void output(int N);
  void calcE(int N);

protected:
  std::vector<Neuron> neurons;
  std::vector<std::vector<Weight>> weights;

  std::mt19937 mt;
  std::uniform_int_distribution<> rand_int;
private:
  std::ofstream ofs;
};

#endif
