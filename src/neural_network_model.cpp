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

NeuralNetworkModel::NeuralNetworkModel(int num_neurons, std::string weights_file, std::string bias_file, std::string output_file, int seed){

  ofs.open(output_file, ios::out);

  vector<double> bias = readBias(bias_file);
  if(num_neurons != bias.size()){
    cout << "[ERROR] wrong bias size, num of neurons: " << num_neurons << ", bias size: " << bias.size() << endl;
    exit(1); 
  }

  for(int i=0; i<num_neurons; ++i){
    neurons.emplace_back(Neuron(bias[i]));
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
  mt  = mt19937(seed);
  rand_int = uniform_int_distribution<>(0, num_neurons -1);
}

void NeuralNetworkModel::output(int N){
  for(int i=0; i<neurons.size(); ++i){
    if(i % N == 0 && i != 0){
      ofs << endl;
    }

    ofs << neurons[i].getOutput() << '\t';
  }
  ofs << endl << endl;
}

void NeuralNetworkModel::calcE(int N){

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

  cout << "E: " << E << endl;
  ofs << "E: " << E << endl;
}
