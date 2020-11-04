#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class GaussianModel : public NeuralNetworkModel {
public:
  GaussianModel(const Parameters& parameters, const float standard_deviation);

  void simulate();

private:
  std::normal_distribution<> rand_dist;
};

#endif
