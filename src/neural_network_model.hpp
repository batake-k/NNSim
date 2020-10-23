#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include "neuron.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <random>

class NeuralNetworkModel {
public:

  struct Weight {
    int neuron_id;
    float weight;
  };

  NeuralNetworkModel();
  NeuralNetworkModel(const std::string& weights_file, const std::string& bias_file, const std::string& output_file, const int seed);
  virtual ~NeuralNetworkModel() {};

  virtual void update() {};

	uint32_t getNumNeuron(){
		return neurons.size();
	}

  void output(int N);
  void calcE(int N);

protected:
  std::vector<Neuron> neurons;
  std::vector<std::vector<Weight>> weights;

  std::mt19937 mt;
  std::uniform_int_distribution<> rand_int;
private:
  std::ofstream ofs;
};

#endif
