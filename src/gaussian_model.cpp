#include "gaussian_model.hpp"

#include <iostream>
#include <math.h>

using namespace std;

GaussianModel::GaussianModel(bool _sync, double _gain, int num_neurons, string weights_file, string bias_file, string output_file, int seed):
    NeuralNetworkModel(num_neurons, weights_file, bias_file, output_file, seed),sync(_sync),gain(_gain){
  rand_dist = normal_distribution<>(0.0, 1.0);
}

void GaussianModel::updateSync(){
  for(int i=0; i<neurons.size(); ++i){

    //calc input sum
    double input_sum = 0;
    for(const auto& w : weights[i]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += neurons[i].getBias();
    input_sum += rand_dist(mt);

    //calc output
    double output = func(input_sum);

    //set output
    neurons[i].setOutput(output);
  }

  for(auto& neuron : neurons){
    neuron.update();
  }
}

void GaussianModel::updateAsync(){
  for(int i=0; i<neurons.size(); ++i){
    //determine neuron id
    int id = rand_int(mt);

    //calc input sum
    double input_sum = 0;
    for(const auto& w : weights[id]){
      input_sum += neurons[w.neuron_id].getOutput() * w.weight;
    }
    input_sum += neurons[id].getBias();
    input_sum += rand_dist(mt);

    //calc output
    double output = func(input_sum);

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

double GaussianModel::func(double input_sum){
  return 1 / (1 + exp(- gain * input_sum));
}
