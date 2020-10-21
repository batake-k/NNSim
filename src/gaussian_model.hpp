#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class GaussianModel : public NeuralNetworkModel {
public:
  GaussianModel(bool _sync, double _gain, int num_neurons, std::string weights_file, std::string bias_file, std::string output_file, int seed);

  void update();
  void updateSync();
  void updateAsync();

private:
  bool sync;
  double gain;

  double func(double input_sum);

  std::normal_distribution<> rand_dist;
};

#endif
