#ifndef MCCULLOCH_PITTS_MODEL_HPP
#define MCCULLOCH_PITTS_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class MccullochPittsModel : NeuralNetworkModel {
public:
  MccullochPittsModel(bool _sync, int num_neurons, std::string weights_file, std::string bias_file);
  void update();

private:
  bool sync;
};

#endif
