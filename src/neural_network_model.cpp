#include "neural_network_model.hpp"

#include "utils.hpp"
#include "timer.hpp"

#include <iostream>

using namespace std;

namespace{
  vector<float> readBias(const string& bias_file){
    ifstream ifs(bias_file);
    if(ifs.fail()){
      cout << "[ERROR] file open error: " << bias_file << endl;
      exit(1);
    }

    vector<float> bias;
    string line;
    while(getline(ifs, line)){
      bias.emplace_back(stof(line));
    }

    return bias;
  }
}

NeuralNetworkModel::NeuralNetworkModel(const string& weights_file, const string& bias_file, const int seed){

	Timer timer;

  vector<float> bias = readBias(bias_file);
	timer.elapsed("read bias file", 2);

  int num_neurons = bias.size();

	//random
	mt = mt19937(seed);
	rand_int = uniform_int_distribution<>(0, num_neurons -1);

	timer.restart();
	uniform_real_distribution<> rand_real(0.0, 1.0);
  for(int i=0; i<num_neurons; ++i){
    neurons.emplace_back(Neuron(rand_real(mt), bias[i]));
  }
	timer.elapsed("init neurons", 2);

  //TODO
	timer.restart();
  weights.resize(num_neurons);
  ifstream ifs(weights_file);

  string line;
  while(getline(ifs, line)){
    vector<string> split_line = utils::split(line, ',');
    Weight w = {stoi(split_line[0]), stof(split_line[2])};
    weights[stoi(split_line[1])].emplace_back(w);
  }
	timer.elapsed("read weights file", 2);

}

string NeuralNetworkModel::output(int N){
	string output = "";
  for(uint32_t i=0; i<neurons.size(); ++i){
    if(i % N == 0 && i != 0){
      output += "\n";
    }

		output += to_string(neurons[i].getOutput()) + ",";
  }
  output += "\n";
	return output;
}

double NeuralNetworkModel::calcE(int N){

  double E = 0;

  for(int x=0; x<N; ++x){
    double a = 0;
    for(int y=0; y<N; ++y){
      a += neurons[x * N + y].getOutput();
    }
    E += (a - 1) * (a - 1);
  }

  for(int y=0; y<N; ++y){
    double b = 0;
    for(int x=0; x<N; ++x){
      b += neurons[x * N + y].getOutput();
    }
    E += (b - 1) * (b - 1);
  }

  for(int i=0; i <= 2*N-2; ++i){
    for(int x=0; x<N; ++x){

      int y = i - x;
      if(y < 0 || y >= N){
        continue;
      }

      for(int X=0; X<N; ++X){

        int Y = i - X;
        if(Y < 0 || Y >= N || x == X){
          continue;
        }
        E += neurons[x * N + y].getOutput() * neurons[X * N + Y].getOutput();
      }
    }
  }

  for(int j=1-N; j <= N-1; ++j){
    for(int x=0; x<N; ++x){

      int y = j + x;
      if(y < 0 || y >= N){
        continue;
      }

      for(int X=0; X<N; ++X){

        int Y = j + X;
        if(Y < 0 || Y >= N || x == X){
          continue;
        }
        E += neurons[x * N + y].getOutput() * neurons[X * N + Y].getOutput();
      }
    }
  }

  return E;
}
