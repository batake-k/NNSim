#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include "neuron.hpp"

#include <vector>
#include <string>
#include <memory>

class NeuralNetworkModel {
public:

  struct Weight {
    int neuron_id;
    double weight;
  };

  NeuralNetworkModel();
  NeuralNetworkModel(int num_neurons, std::string weights_file, std::string bias_file);
  virtual ~NeuralNetworkModel() {};

  virtual void update() {};

private:
  std::vector<std::shared_ptr<Neuron>> neurons;
  std::vector<std::vector<Weight>> weights;
};

#endif
