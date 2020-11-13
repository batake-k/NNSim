#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>
#include <vector>

class GaussianModel : public NeuralNetworkModel {
public:

	typedef struct{
		float Tmf;
		uint32_t time_constant_Tmf;
		std::vector<float> reciprocal_Tmf;
	}SharpeningParameters;

	GaussianModel(const Parameters& parameters, const SharpeningParameters& s_parameters, const float standard_deviation);

	void simulate();

private:
	void calculateReciprocalTmf(const uint32_t generations);

	void initNeurons();

	float func(const float input, const uint32_t generation);
	float inverseFunc(const float input);

	SharpeningParameters sharpening_parameters;

	std::normal_distribution<> rand_dist;
};

#endif
