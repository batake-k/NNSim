#include "neural_network_model.hpp"

#include "utils.hpp"
#include "timer.hpp"

#include <iostream>

using namespace std;

void NeuralNetworkModel::readBias(const string& bias_file){
  ifstream ifs(bias_file, ios::binary);
  if(ifs.fail()){
    cout << "[ERROR] file open error: " << bias_file << endl;
    exit(0);
  }

	uint32_t size;
	ifs.read((char*)&size, sizeof(uint32_t));
  neuron_bias.resize(size);

	ifs.read((char*)&neuron_bias[0], sizeof(float)*size);
	ifs.close();
}

void NeuralNetworkModel::readWeights(const string& weights_file){
	ifstream ifs(weights_file, ios::binary);
	if(ifs.fail()){
		cout << "[ERROR] file open error: " << weights_file << endl;
		exit(0);
	}

	uint32_t neuron_size, size;
	ifs.read((char*)&neuron_size, sizeof(uint32_t));
	neuron_weights.resize(neuron_size);

	for(uint32_t i=0; i<neuron_size; ++i){
		ifs.read((char*)&size, sizeof(uint32_t));
		neuron_weights[i].resize(size);
		ifs.read((char*)&neuron_weights[i][0], sizeof(Weight)*size);
	}

	ifs.close();
}

NeuralNetworkModel::NeuralNetworkModel(const string& weights_file, const string& bias_file, const int seed,
		 const int time_constant, const float _base_potential): base_potential(_base_potential){

	//read bias file, and init neuron_bias
	Timer timer;
  readBias(bias_file);
	timer.elapsed("read bias file", 2);

  //read weights file, and init neuron_weights
	timer.restart();
	readWeights(weights_file);
	timer.elapsed("read weights file", 2);

	num_neurons = neuron_bias.size();

	//set mt and rand_int for select random neuron
	mt = mt19937(seed);
	rand_int = uniform_int_distribution<>(0, num_neurons -1);

	//init neuron_potentials, neuron_outputs
	timer.restart();
	neuron_potentials.resize(num_neurons);
	neuron_outputs.resize(num_neurons);

	uniform_real_distribution<> rand_real(0.0, 1.0);
  for(uint32_t i=0; i<num_neurons; ++i){
		neuron_potentials[i] = rand_real(mt);
    neuron_outputs[i] = func(neuron_potentials[i]);
  }
	timer.elapsed("init neurons", 2);

	//calc time_constant_for_multi for update neuron_potentials
	if(time_constant <= 1){
		cout << "[ERROR] time constant should > 1" << endl;
		exit(0);
	}
	time_constant_for_multi = (float)(time_constant -1) / time_constant;
}

string NeuralNetworkModel::output(int N){
	string output = "";
  for(uint32_t i=0; i<num_neurons; ++i){
    if(i % N == 0 && i != 0){
      output += "\n";
    }

		output += to_string(neuron_outputs[i]) + ",";
  }
  output += "\n";
	return output;
}

double NeuralNetworkModel::calcE(int N){

  double E = 0;

  for(int x=0; x<N; ++x){
    double a = 0;
    for(int y=0; y<N; ++y){
      a += neuron_outputs[x * N + y];
    }
    E += (a - 1) * (a - 1);
  }

  for(int y=0; y<N; ++y){
    double b = 0;
    for(int x=0; x<N; ++x){
      b += neuron_outputs[x * N + y];
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
        E += neuron_outputs[x * N + y] * neuron_outputs[X * N + Y];
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
        E += neuron_outputs[x * N + y] * neuron_outputs[X * N + Y];
      }
    }
  }

  return E;
}

float NeuralNetworkModel::func(const float input){
	return (std::tanh(input / base_potential) + 1) / 2;
}
