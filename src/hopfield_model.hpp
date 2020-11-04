#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class HopfieldModel : public NeuralNetworkModel {
public:
  HopfieldModel(const Parameters& parameters);

  void simulate();

private:
};

#endif
