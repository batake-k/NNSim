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
	reciprocal_time_constant = 1.0 / parameters.time_constant;

	//init potentials, neuron_outputs
	timer.restart();
	potentials.resize(num_neurons);
	outputs.resize(num_neurons);
	outputs_old.resize(num_neurons);

	timer.elapsed("init neurons", 2);
}


void NeuralNetworkModel::output(){
	int N = sqrt(num_neurons);

	for(uint32_t i=0; i<num_neurons; ++i){
		if((i+1) % N == 0){
			ofs << outputs[i] << endl;
		}else{
			ofs << outputs[i] << ",";
		}
	}
	ofs << endl;
}

void NeuralNetworkModel::outputU(){
	int N = sqrt(num_neurons);

	for(uint32_t i=0; i<num_neurons; ++i){
		if((i+1) % N == 0){
			ofs << potentials[i] << endl;
		}else{
			ofs << potentials[i] << ",";
		}
	}
	ofs << endl;
}

void NeuralNetworkModel::calcEnergy(){
	double E = 0;

	for(uint32_t i=0; i<num_neurons; ++i){
		for(const auto& w : weights[i]){
			E -= 0.5 * outputs[i] * outputs[w.neuron_id] * w.weight;
		}

		E -= outputs[i] * biases[i];
	}

	ofs << E << endl;
}
