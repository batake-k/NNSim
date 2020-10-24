#include "gaussian_model.hpp"

#include <iostream>
#include <math.h>

using namespace std;

GaussianModel::GaussianModel(const bool _sync, const float _potential, const string& weights_file, const string& bias_file,
		const int seed, const int time_constant, const float standard_deviation):
    NeuralNetworkModel(_potential, weights_file, bias_file, seed, time_constant),sync(_sync){
  rand_dist = normal_distribution<>(0.0, standard_deviation);
}

void GaussianModel::updateSync(){
  for(uint32_t i=0; i<neurons.size(); ++i){

    //calc input sum
    float input_sum = 0;
    for(const auto& w : weights[i]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += bias[i];
    input_sum += rand_dist(mt);

		float p = neurons[i].getPotential() * time_constant_for_multi + input_sum;
		neurons[i].setPotential(p);

    //calc output
    float output = func(p);

    //set output
    neurons[i].setOutput(output);
  }

  for(auto& neuron : neurons){
    neuron.update();
  }
}

void GaussianModel::updateAsync(){
  for(uint32_t i=0; i<neurons.size(); ++i){
    //determine neuron id
    int id = rand_int(mt);

    //calc input sum
    float input_sum = 0;
    for(const auto& w : weights[id]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += bias[id];
    input_sum += rand_dist(mt);

		float p = neurons[id].getPotential() * time_constant_for_multi + input_sum;
		neurons[id].setPotential(p);

    //calc output
    float output = func(p);

    //set output
    neurons[id].setOutput(output);

    //update
    neurons[id].update();
  }

}

void GaussianModel::update(){
  if(sync){
    updateSync();
  }else{
    updateAsync();
  }
}
