#include "neural_network_model.hpp"

#include "discrete_neuron.hpp"

#include <iostream>
#include <fstream>

using namespace std;

namespace{
  vector<int> readBias(string bias_file){
    ifstream ifs(bias_file);
    if(ifs.fail()){
      cout << "[ERROR] file open error: " << bias_file << endl;
      exit(1);
    }

    vector<int> bias;
    string line;
    while(getline(ifs, line)){
      bias.emplace_back(stoi(line));
    }

    return bias;
  }
}

NeuralNetworkModel::NeuralNetworkModel(bool _sync, uint32_t num_neurons, std::string weights_file, std::string bias_file):
    sync(_sync){

  vector<int> bias = readBias(bias_file);
  if(num_neurons != bias.size()){
    cout << "[ERROR] wrong bias size, num of neurons: " << num_neurons << ", bias size: " << bias.size() << endl;
    exit(1); 
  }

  for(uint32_t i=0; i<num_neurons; ++i){
    neurons.emplace_back(std::make_shared<DiscreteNeuron>(bias[i]));
  }

  cout << "sync: " << sync << endl
       << "num neurons: " << neurons.size() << endl;
}

void NeuralNetworkModel::update(){
  
}
