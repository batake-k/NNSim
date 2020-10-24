#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class GaussianModel : public NeuralNetworkModel {
public:
  GaussianModel(const std::string& weights_file, const std::string& bias_file,
			const int seed, const int time_constant, const float base_potential, const float standard_deviation);

  void update();

private:
  std::normal_distribution<> rand_dist;
};

#endif
