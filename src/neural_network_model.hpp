#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

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
  NeuralNetworkModel(const std::string& weights_file, const std::string& bias_file, const int seed, const int time_constant, const float _base_potential);
  virtual ~NeuralNetworkModel() {};

  virtual void update() {};

	uint32_t getNumNeuron(){
		return num_neurons;
	}

  std::string output(int N);
  double calcE(int N);

protected:
	float func(const float input);

  std::vector<float> neuron_potentials;
	std::vector<float> neuron_outputs;
	std::vector<float> neuron_bias;
  std::vector<std::vector<Weight>> neuron_weights;

  std::mt19937 mt;
  std::uniform_int_distribution<> rand_int;

	uint32_t num_neurons;
	float time_constant_for_multi;

private:
	void readWeights(const std::string& weights_file);
	void readBias(const std::string& bias_file);

	float base_potential;
};

#endif
