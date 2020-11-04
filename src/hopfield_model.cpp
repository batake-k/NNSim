#include "hopfield_model.hpp"

#include "timer.hpp"

#include <iostream>

using namespace std;

HopfieldModel::HopfieldModel(const Parameters& parameters):NeuralNetworkModel(parameters){
  
}

void HopfieldModel::simulate(){

	calcEnergyNQueen();
	Timer timer;

	if(parameters.synchronize){
		if(parameters.inner_potential){

			//sync = true, inner_potential = true
			for(uint32_t iter=0; iter<parameters.generations; ++iter ){
				timer.restart();
				for(uint32_t i=0; i<num_neurons; ++i){
					float input_sum = 0;
					for(const auto& w : weights[i]){
						input_sum += outputs_old[w.neuron_id] * w.weight;
					}
					potentials[i] += parameters.delta_t * (- reciprocal_time_constant * potentials[i] + input_sum + biases[i]);
					outputs[i] = func(potentials[i]);
				}

				for(uint32_t i=0; i<num_neurons; ++i){
					outputs_old[i] = outputs[i];
				}
				timer.elapsed("update", 2);
				calcEnergyNQueen();
			}

		}else{

			//sync = true, inner_potential = false
			for(uint32_t iter=0; iter<parameters.generations; ++iter ){
				timer.restart();
				for(uint32_t i=0; i<num_neurons; ++i){
					float input_sum = 0;
					for(const auto& w : weights[i]){
						input_sum += outputs_old[w.neuron_id] * w.weight;
					}
					outputs[i] = func(input_sum + biases[i]);
				}

				for(uint32_t i=0; i<num_neurons; ++i){
					outputs_old[i] = outputs[i];
				}
				timer.elapsed("update", 2);
				calcEnergyNQueen();
			}

		}
	}else{
		if(parameters.inner_potential){

			//sync = false, inner_potential = true
			for(uint32_t iter=0; iter<parameters.generations; ++iter ){
				timer.restart();
				for(uint32_t i=0; i<num_neurons; ++i){
					uint32_t id = rand_int(mt);
					float input_sum = 0;
					for(const auto& w : weights[id]){
						input_sum += outputs[w.neuron_id] * w.weight;
					}
					potentials[id] += parameters.delta_t * (- reciprocal_time_constant * potentials[id] + input_sum + biases[id]);
					outputs[id] = func(potentials[id]);
				}
				timer.elapsed("update", 2);
				calcEnergyNQueen();
			}

		}else{

			//sync = false, inner_potential = false
			for(uint32_t iter=0; iter<parameters.generations; ++iter ){
				timer.restart();
				for(uint32_t i=0; i<num_neurons; ++i){
					uint32_t id = rand_int(mt);
					float input_sum = 0;
					for(const auto& w : weights[id]){
						input_sum += outputs[w.neuron_id] * w.weight;
					}
					outputs[id] = func(input_sum + biases[id]);
				}
				timer.elapsed("update", 2);
				calcEnergyNQueen();
			}

		}
	}

	output();
}
