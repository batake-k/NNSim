#include "hopfield_model.hpp"

#include <iostream>
#include <math.h>

using namespace std;

HopfieldModel::HopfieldModel(const bool _sync, const float _potential, const string& weights_file, const string& bias_file,
		const string& output_file, const int seed): 
		NeuralNetworkModel(weights_file, bias_file, output_file, seed),sync(_sync),potential(_potential){
  
}

void HopfieldModel::updateSync(){
  for(uint32_t i=0; i<neurons.size(); ++i){

    //calc input sum
    float input_sum = 0;
    for(const auto& w : weights[i]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += neurons[i].getBias();

    //calc output
    float output = func(input_sum);

    //set output
    neurons[i].setOutput(output);
  }

  for(auto& neuron : neurons){
    neuron.update();
  }
}

void HopfieldModel::updateAsync(){
  for(uint32_t i=0; i<neurons.size(); ++i){
    //determine neuron id
    int id = rand_int(mt);

    //calc input sum
    float input_sum = 0;
    for(const auto& w : weights[id]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += neurons[id].getBias();

    //calc output
    float output = func(input_sum);

    //set output
    neurons[id].setOutput(output);

    //update
    neurons[id].update();
  }

}

void HopfieldModel::update(){
  if(sync){
    updateSync();
  }else{
    updateAsync();
  }
}

float HopfieldModel::func(float input_sum){
	return (std::tanh(input_sum / potential) + 1) / 2;
}
