#include "hopfield_model.hpp"

#include <iostream>

using namespace std;

HopfieldModel::HopfieldModel(const string& weights_file, const string& bias_file,
		const int seed, const int time_constant, const float base_potential): 
		NeuralNetworkModel(weights_file, bias_file, seed, time_constant, base_potential){
  
}

void HopfieldModel::update(){
	//update random neuron num_neuron times
  for(uint32_t i=0; i<num_neurons; ++i){
    //select random neuron id
    int id = rand_int(mt);

    //calc input sum
    float input_sum = 0;
    for(const auto& w : neuron_weights[id]){
      input_sum += neuron_outputs[w.neuron_id] * w.weight;
    }
    input_sum += neuron_bias[id];

		//update neuron_potentials
		float potential = neuron_potentials[id] * time_constant_for_multi + input_sum;
		neuron_potentials[id] = potential;

		//update neuron_outputs
		neuron_outputs[id] = func(potential);
  }
}
