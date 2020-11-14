#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include <vector>
#include <string>
#include <fstream>
#include <random>

class NeuralNetworkModel {
public:

	typedef struct {
		std::string weights_file;
		std::string biases_file;
		std::string output_file;

		bool synchronize;
		uint32_t seed;
		uint32_t generations;
		uint32_t time_constant;
		float delta_t;
	}Parameters;

	typedef struct {
		uint32_t neuron_id;
		float weight;
	}Weight;

	NeuralNetworkModel() {};
	NeuralNetworkModel(const Parameters& parameters);
	virtual ~NeuralNetworkModel() {};

	virtual void simulate() {};

protected:
	void output();
	void outputU();
	double calcEnergy();

	std::vector<float> potentials;
	std::vector<float> outputs;
	std::vector<float> outputs_old;
	std::vector<float> biases;
	std::vector<std::vector<Weight>> weights;

	std::mt19937 mt;
	std::uniform_int_distribution<> rand_int;

	Parameters parameters;
	uint32_t num_neurons;
	float reciprocal_time_constant;

private:
	virtual void initNeurons() {};
	virtual float func() {return 0;};
	virtual float inverseFunc() {return 0;};

	void readWeights();
	void readBiases();

	std::ofstream ofs;
};

#endif
