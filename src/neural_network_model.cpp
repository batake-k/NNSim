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
  bias.resize(size);

	ifs.read((char*)&bias[0], sizeof(float)*size);
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
	weights.resize(neuron_size);

	for(uint32_t i=0; i<neuron_size; ++i){
		ifs.read((char*)&size, sizeof(uint32_t));
		weights[i].resize(size);
		ifs.read((char*)&weights[i][0], sizeof(Weight)*size);
	}

	ifs.close();
}

NeuralNetworkModel::NeuralNetworkModel(const float _potential, const string& weights_file, const string& bias_file, const int seed,
		 const int _time_constant): potential(_potential){

	Timer timer;

  readBias(bias_file);
	timer.elapsed("read bias file", 2);

  int num_neurons = bias.size();

	//random
	mt = mt19937(seed);
	rand_int = uniform_int_distribution<>(0, num_neurons -1);

	if(_time_constant < 1){
		cout << "[ERROR] time constant should > 1" << endl;
	}
	time_constant_for_multi = (_time_constant -1) / _time_constant;

	timer.restart();
	uniform_real_distribution<> rand_real(0.0, 1.0);
  for(int i=0; i<num_neurons; ++i){
		float potential = rand_real(mt);
    neurons.emplace_back(Neuron(func(potential), potential));
  }
	timer.elapsed("init neurons", 2);

  //TODO
	timer.restart();
	readWeights(weights_file);
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

float NeuralNetworkModel::func(float input_sum){
	return (std::tanh(input_sum / potential) + 1) / 2;
}
