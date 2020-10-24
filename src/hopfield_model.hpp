#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class HopfieldModel : public NeuralNetworkModel {
public:
  HopfieldModel(const std::string& weights_file, const std::string& bias_file,
			const int seed, const int time_constant, const float base_potential);

  void update();

private:
};

#endif
