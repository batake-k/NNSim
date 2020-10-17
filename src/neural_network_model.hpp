#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include "neuron.hpp"

#include <vector>
#include <string>
#include <memory>

class NeuralNetworkModel {
public:

  struct Weight {
    uint32_t neuron_id;
    double weight;
  };

  NeuralNetworkModel();
  NeuralNetworkModel(bool _sync, uint32_t num_neurons, std::string weights_file);
  virtual ~NeuralNetworkModel() {};

private:
  bool sync;
  std::vector<std::shared_ptr<Neuron>> neurons;
  std::vector<std::vector<Weight>> weights;
};

#endif
