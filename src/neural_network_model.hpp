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
  NeuralNetworkModel(const float _potential, const std::string& weights_file, const std::string& bias_file, const int seed, const int _time_constant);
  virtual ~NeuralNetworkModel() {};

  virtual void update() {};

	uint32_t getNumNeuron(){
		return neurons.size();
	}

  std::string output(int N);
  double calcE(int N);

protected:
	float func(float input_sum);

  std::vector<Neuron> neurons;
  std::vector<std::vector<Weight>> weights;
	std::vector<float> bias;

  std::mt19937 mt;
  std::uniform_int_distribution<> rand_int;
	float time_constant_for_multi;
private:
	void readWeights(const std::string& weights_file);
	void readBias(const std::string& bias_file);

	float potential;
};

#endif
