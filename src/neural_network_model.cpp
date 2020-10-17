#include "neural_network_model.hpp"

#include "discrete_neuron.hpp"

#include <iostream>

using namespace std;

NeuralNetworkModel::NeuralNetworkModel(bool _sync, uint32_t num_neurons, std::string weights_file):
    sync(_sync){

  for(uint32_t i=0; i<num_neurons; ++i){
    neurons.emplace_back(std::make_shared<DiscreteNeuron>());
  }

  cout << "sync: " << sync << endl
       << "num neurons: " << neurons.size() << endl;
}
