#ifndef GAUSSIAN_MODEL_HPP
#define GAUSSIAN_MODEL_HPP

#include "neural_network_model.hpp"

#include <string>

class GaussianModel : public NeuralNetworkModel {
public:

	typedef struct{
		uint32_t time_constant_T_mf;
		float T_mf;
		float current_T_mf;
		float reciprocal_current_T_mf;
	}SharpeningParameters;

	typedef struct{
		uint32_t time_constant_T_epsilon;
		float T_epsilon;
		float current_T_epsilon;
	}AnnealingParameters;

	GaussianModel(const Parameters& p, const SharpeningParameters& sp, const AnnealingParameters& ap);

	void simulate();

private:
	void initNeurons();
	void calculateT_mf(const uint32_t generation);
	void calculateT_epsilon(const uint32_t generation);
	void calcFreeEnergy(const uint32_t generation);

	float func(const float input);
	float inverseFunc(const float input);

	SharpeningParameters sharpening_parameters;
	AnnealingParameters annealing_parameters;
};

#endif
