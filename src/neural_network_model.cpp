#include "neural_network_model.hpp"

#include "utils.hpp"
#include "timer.hpp"

#include <iostream>

using namespace std;

void NeuralNetworkModel::readBiases(){
	ifstream ifs;
	utils::fileOpen(ifs, parameters.biases_file, ios::binary);

	uint32_t size;
	ifs.read((char*)&size, sizeof(uint32_t));
  biases.resize(size);

	ifs.read((char*)&biases[0], sizeof(float)*size);
	ifs.close();
}

void NeuralNetworkModel::readWeights(){
	ifstream ifs;
	utils::fileOpen(ifs, parameters.weights_file, ios::binary);

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

NeuralNetworkModel::NeuralNetworkModel(const Parameters& _parameters):parameters(_parameters){

	//read bias file, and init biases
	Timer timer;
  readBiases();
	timer.elapsed("read biases file", 2);

  //read weights file, and init weights
	timer.restart();
	readWeights();
	timer.elapsed("read weights file", 2);

	utils::fileOpen(ofs, parameters.output_file, ios::out);

	num_neurons = biases.size();

	//set mt and rand_int for select random neuron
	mt = mt19937(parameters.seed);
	rand_int = uniform_int_distribution<>(0, num_neurons -1);

	//calc reciprocal_time_constant and reciprocal_base_potential for update potentials
	reciprocal_base_potential = 1.0 / parameters.base_potential;
	reciprocal_time_constant = 1.0 / parameters.time_constant;

	//init potentials, neuron_outputs
	timer.restart();
	potentials.resize(num_neurons);
	outputs.resize(num_neurons);
	outputs_old.resize(num_neurons);

	uniform_real_distribution<> rand_real(0.0, 1.0);
  for(uint32_t i=0; i<num_neurons; ++i){
		outputs[i] = 0.5 + 0.001 * rand_real(mt);
		outputs_old[i] = outputs[i];
		potentials[i] = inverseFunc(outputs[i]);
  }
	timer.elapsed("init neurons", 2);
}


void NeuralNetworkModel::output(){
	int N = sqrt(num_neurons);

  for(uint32_t i=0; i<num_neurons; ++i){
    if(i % N == 0 && i != 0){
      ofs << endl;
    }

		ofs << outputs[i] << ",";
  }
  ofs << endl;
}

void NeuralNetworkModel::calcEnergyNQueen(){

	int N = sqrt(num_neurons);
  double E = 0;

  for(int x=0; x<N; ++x){
    double a = 0;
    for(int y=0; y<N; ++y){
      a += outputs[x * N + y];
    }
    E += (a - 1) * (a - 1);
  }

  for(int y=0; y<N; ++y){
    double b = 0;
    for(int x=0; x<N; ++x){
      b += outputs[x * N + y];
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
        E += outputs[x * N + y] * outputs[X * N + Y];
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
        E += outputs[x * N + y] * outputs[X * N + Y];
      }
    }
  }

  ofs << "E: " << E << endl;
}

//logit
float NeuralNetworkModel::inverseFunc(const float input){
	return log(input / (1.0 - input)) / reciprocal_base_potential;
}

//sigmoid
float NeuralNetworkModel::func(const float input){
	//return (std::tanh(input / base_potential) + 1) / 2;
	return 1.0 / (1.0 + exp(- input * reciprocal_base_potential));
}
