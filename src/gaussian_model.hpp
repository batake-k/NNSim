#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class GaussianModel : public NeuralNetworkModel {
public:
  GaussianModel(const bool _sync, const float _potential, const std::string& weights_file, const std::string& bias_file,
			const std::string& output_file, const int seed, const float standard_deviation);

  void update();
  void updateSync();
  void updateAsync();

private:
  float func(float input_sum);

  bool sync;
  float potential;
  std::normal_distribution<> rand_dist;
};

#endif
