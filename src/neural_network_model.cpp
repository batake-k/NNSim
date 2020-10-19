#include "neural_network_model.hpp"

#include "utils.hpp"

#include <iostream>

using namespace std;

namespace{
  vector<double> readBias(string bias_file){
    ifstream ifs(bias_file);
    if(ifs.fail()){
      cout << "[ERROR] file open error: " << bias_file << endl;
      exit(1);
    }

    vector<double> bias;
    string line;
    while(getline(ifs, line)){
      bias.emplace_back(stod(line));
    }

    return bias;
  }
}

NeuralNetworkModel::NeuralNetworkModel(int num_neurons, std::string weights_file, std::string bias_file, std::string output_file){

  ofs.open(output_file, ios::out);

  vector<double> bias = readBias(bias_file);
  if(num_neurons != bias.size()){
    cout << "[ERROR] wrong bias size, num of neurons: " << num_neurons << ", bias size: " << bias.size() << endl;
    exit(1); 
  }

  for(int i=0; i<num_neurons; ++i){
    neurons.emplace_back(bias[i]);
  }

  //TODO
  weights.resize(num_neurons);
  ifstream ifs(weights_file);

  string line;
  while(getline(ifs, line)){
    vector<string> split_line = utils::split(line, ',');
    Weight w = {stoi(split_line[0]), stod(split_line[2])};
    weights[stoi(split_line[1])].emplace_back(w);
  }

  //random
  mt  = mt19937(0);
  rand_int = uniform_int_distribution<>(0, num_neurons -1);
}

void NeuralNetworkModel::output(int N){
  for(int i=0; i<neurons.size(); ++i){
    if(i % N == 0){
      ofs << endl;
    }

    ofs << neurons[i].getOutput() << '\t';
  }
  ofs << endl;
}
