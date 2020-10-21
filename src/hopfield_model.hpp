#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class HopfieldModel : public NeuralNetworkModel {
public:
  HopfieldModel(const bool _sync, const double _potential, const std::string& weights_file, const std::string& bias_file,
			const std::string& output_file, const int seed);

  void update();
  void updateSync();
  void updateAsync();

private:
  bool sync;
  double potential;

  double func(double input_sum);
};

#endif
