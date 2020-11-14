#include "gaussian_model.hpp"

#include "timer.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

GaussianModel::GaussianModel(const Parameters& p, const SharpeningParameters& sp, const AnnealingParameters& ap):
		NeuralNetworkModel(p), sharpening_parameters(sp), annealing_parameters(ap){

	calculateT_mf(0);
	calculateT_epsilon(0);

	Timer timer;
	initNeurons();
	timer.elapsed("init neurons", 2);
}

void GaussianModel::simulate(){
	//output();
	calcEnergy();
	Timer timer;

	if(parameters.synchronize){

		for(uint32_t generation=0; generation<parameters.generations; ++generation ){
			timer.restart();
			calculateT_mf(generation);
			calculateT_epsilon(generation);
			std::normal_distribution<> dist = std::normal_distribution<>(0.0, std::sqrt(2 * annealing_parameters.current_T_epsilon));

			for(uint32_t i=0; i<num_neurons; ++i){
				float input_sum = 0;
				for(const auto& w : weights[i]){
					input_sum += outputs_old[w.neuron_id] * w.weight;
				}
				potentials[i] += parameters.delta_t * (- reciprocal_time_constant * potentials[i] + input_sum + biases[i] + dist(mt));
				outputs[i] = func(potentials[i]);
			}

			for(uint32_t i=0; i<num_neurons; ++i){
				outputs_old[i] = outputs[i];
			}
			timer.elapsed("update", 2);
			//output();
			calcEnergy();
		}

	}else{

		for(uint32_t generation=0; generation<parameters.generations; ++generation ){
			timer.restart();
			calculateT_mf(generation);
			calculateT_epsilon(generation);
			std::normal_distribution<> dist = std::normal_distribution<>(0.0, std::sqrt(2 * annealing_parameters.current_T_epsilon));

			for(uint32_t i=0; i<num_neurons; ++i){
				uint32_t id = rand_int(mt);
				float input_sum = 0;
				for(const auto& w : weights[id]){
					input_sum += outputs[w.neuron_id] * w.weight;
				}
				potentials[id] += parameters.delta_t * (-reciprocal_time_constant * potentials[id] + input_sum + biases[id] + dist(mt));
				outputs[id] = func(potentials[id]);
			}
			timer.elapsed("update", 2);
			calcEnergy();
		}

	}
}

void GaussianModel::initNeurons(){
	//TODO
	int N = std::sqrt(num_neurons);
	float value = 1.0 / N;

	uniform_real_distribution<> rand_real(0, 2 * value);
	for(uint32_t i=0; i<num_neurons; ++i){
		outputs[i] = rand_real(mt);
		outputs_old[i] = outputs[i];
		potentials[i] = inverseFunc(outputs[i]);
	}
}

void GaussianModel::calculateT_epsilon(const uint32_t generation){
	// D論
	annealing_parameters.current_T_epsilon = annealing_parameters.T_epsilon / std::pow(1.0+(float)generation/annealing_parameters.time_constant_T_epsilon, 2);

	/*
	float T_epsilon_candidate = annealing_parameters.T_epsilon * (1.0 - (float)generation / annealing_parameters.time_constant_T_epsilon);
	annealing_parameters.current_T_epsilon = std::max(T_epsilon_candidate, (float)1e-6);
	*/
}

void GaussianModel::calculateT_mf(const uint32_t generation){
	// D論
	sharpening_parameters.current_T_mf = sharpening_parameters.T_mf / (1.0 + (float)generation / sharpening_parameters.time_constant_T_mf);
	sharpening_parameters.reciprocal_current_T_mf = 1.0 / sharpening_parameters.current_T_mf;

	/*
	float T_mf_candidate = sharpening_parameters.T_mf * (1.0 - (float)generation / sharpening_parameters.time_constant_T_mf);
	sharpening_parameters.current_T_mf = std::max(T_mf_candidate, (float)1e-6);
	sharpening_parameters.reciprocal_current_T_mf = 1.0/sharpening_parameters.current_T_mf;
	*/
}

//logit
float GaussianModel::inverseFunc(const float input){
	return log(input / (1.0 - input)) * sharpening_parameters.current_T_mf;
}

//sigmoid
float GaussianModel::func(const float input){
	return 1.0 / (1.0 + exp(- input * sharpening_parameters.reciprocal_current_T_mf));
}

void GaussianModel::calcFreeEnergy(const uint32_t generation){
	double E = calcEnergy();

	double S = 0;
	for(uint32_t i=0; i<num_neurons; ++i){
		float v = outputs[i];
		if(v == 0 || v == 1){
		}else{
			S += v * std::log(1.0 / v) + (1.0 - v) * std::log(1.0 / (1.0 - v));
		}
	}

	double F = E - sharpening_parameters.current_T_mf * S;
	cout << "F: " << F << endl;
}
