#ifndef HOPFIELD_MODEL_HPP
#define HOPFIELD_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class HopfieldModel : public NeuralNetworkModel {
public:
  HopfieldModel(bool _sync, double _gain, int num_neurons, std::string weights_file, std::string bias_file, std::string output_file);

  void update();
  void updateSync();
  void updateAsync();

private:
  bool sync;
  double gain;

  double func(double input_sum);
};

#endif
