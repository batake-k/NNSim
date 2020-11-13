#include "gaussian_model.hpp"

#include "timer.hpp"

#include <iostream>

using namespace std;

GaussianModel::GaussianModel(const Parameters& parameters, const float standard_deviation):
    NeuralNetworkModel(parameters){
  rand_dist = normal_distribution<>(0.0, standard_deviation);
}

void GaussianModel::simulate(){

	bool temp = calcEnergyNQueen();
	Timer timer;

	if(parameters.synchronize){

		for(uint32_t iter=0; iter<parameters.generations; ++iter ){
			timer.restart();
			for(uint32_t i=0; i<num_neurons; ++i){
				float input_sum = 0;
				for(const auto& w : weights[i]){
					input_sum += outputs_old[w.neuron_id] * w.weight;
				}
				potentials[i] += parameters.delta_t * (- reciprocal_time_constant * potentials[i] + input_sum + biases[i] + rand_dist(mt));
				outputs[i] = func(potentials[i]);
			}

			for(uint32_t i=0; i<num_neurons; ++i){
				outputs_old[i] = outputs[i];
			}
			timer.elapsed("update", 2);
			if(calcEnergyNQueen()) break;
		}

	}else{

		for(uint32_t iter=0; iter<parameters.generations; ++iter ){
			timer.restart();
			for(uint32_t i=0; i<num_neurons; ++i){
				uint32_t id = rand_int(mt);
				float input_sum = 0;
				for(const auto& w : weights[id]){
					input_sum += outputs[w.neuron_id] * w.weight;
				}
				potentials[id] += parameters.delta_t * (-reciprocal_time_constant * potentials[id] + input_sum + biases[id] + rand_dist(mt));
				outputs[id] = func(potentials[id]);
			}
			timer.elapsed("update", 2);
			if(calcEnergyNQueen()) break;
		}

	}

}
