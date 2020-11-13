#include "gaussian_model.hpp"

#include "timer.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

GaussianModel::GaussianModel(const Parameters& parameters, const SharpeningParameters& s_parameters, const float standard_deviation):
		NeuralNetworkModel(parameters), sharpening_parameters(s_parameters){

	initNeurons();
	calculateReciprocalTmf(parameters.generations);

  rand_dist = normal_distribution<>(0.0, standard_deviation);
}

void GaussianModel::simulate(){

	calcEnergy();
	Timer timer;

	if(parameters.synchronize){

		for(uint32_t generation=0; generation<parameters.generations; ++generation ){
			timer.restart();
			for(uint32_t i=0; i<num_neurons; ++i){
				float input_sum = 0;
				for(const auto& w : weights[i]){
					input_sum += outputs_old[w.neuron_id] * w.weight;
				}
				potentials[i] += parameters.delta_t * (- reciprocal_time_constant * potentials[i] + input_sum + biases[i] + rand_dist(mt));
				outputs[i] = func(potentials[i], generation);
			}

			for(uint32_t i=0; i<num_neurons; ++i){
				outputs_old[i] = outputs[i];
			}
			timer.elapsed("update", 2);
			calcEnergy();
		}

	}else{

		for(uint32_t generation=0; generation<parameters.generations; ++generation ){
			timer.restart();
			for(uint32_t i=0; i<num_neurons; ++i){
				uint32_t id = rand_int(mt);
				float input_sum = 0;
				for(const auto& w : weights[id]){
					input_sum += outputs[w.neuron_id] * w.weight;
				}
				potentials[id] += parameters.delta_t * (-reciprocal_time_constant * potentials[id] + input_sum + biases[id] + rand_dist(mt));
				outputs[id] = func(potentials[id], generation);
			}
			timer.elapsed("update", 2);
			calcEnergy();
		}

	}
}

void GaussianModel::initNeurons(){
	uniform_real_distribution<> rand_real(-0.5, 0.5);
	for(uint32_t i=0; i<num_neurons; ++i){
		outputs[i] = 0.5 + 0.001 * rand_real(mt);
		outputs_old[i] = outputs[i];
		potentials[i] = inverseFunc(outputs[i]);
	}
}

void GaussianModel::calculateReciprocalTmf(const uint32_t generations){
	for(uint32_t i=0; i<generations; ++i){
		float Tmf_candidate1 = sharpening_parameters.Tmf * (1 - (float)i / sharpening_parameters.time_constant_Tmf);
		float Tmf_candidate2 = 1e-6;
		float Tmf = std::max(Tmf_candidate1, Tmf_candidate2);
		sharpening_parameters.reciprocal_Tmf.emplace_back(1.0 / Tmf);
	}
}

//logit
float GaussianModel::inverseFunc(const float input){
	return log(input / (1.0 - input)) * sharpening_parameters.Tmf;
}

//sigmoid
float GaussianModel::func(const float input, const uint32_t generation){
	return 1.0 / (1.0 + exp(- input * sharpening_parameters.reciprocal_Tmf[generation]));
}
