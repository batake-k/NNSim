#include "simulator.hpp"
#include "gaussian_model.hpp"
#include "hopfield_model.hpp"
#include "timer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <memory>

using namespace std;

void Simulator::run(po::variables_map &vm){
	Timer timer;

	setParameters(vm);

	std::shared_ptr<NeuralNetworkModel> model;

	if(parameters.model == 'h'){
		model = make_shared<HopfieldModel>(parameters.sync, parameters.potential, parameters.weights_file, parameters.bias_file, parameters.output_file, parameters.seed);
	}else{
		model = make_shared<GaussianModel>(parameters.sync, parameters.potential, parameters.weights_file, parameters.bias_file, parameters.output_file, parameters.seed, parameters.standard_deviation);
	}

	timer.elapsed("init network model", 1);
	timer.restart();

	//TODO
	uint32_t num_neurons = model->getNumNeuron();
	int N = (int)sqrt(num_neurons);
	if(N * N != num_neurons){
		cout << "N^2: " << N*N << ", num neurons: " << num_neurons << endl;
		exit(0);
	}

	model->calcE(N);

	Timer timer2;
	for(int i=0; i<parameters.generations; ++i){
		timer2.restart();
		model->update();
		model->calcE(N);
		timer2.elapsed("update", 2);
	}

	model->output(N);

	timer.elapsed("update neurons", 1);
}

void Simulator::setParameters(po::variables_map &vm){
	parameters.output_file = vm["output"].as<string>();
	parameters.weights_file = vm["weights"].as<string>();
	parameters.bias_file = vm["bias"].as<string>();

	parameters.model = vm["network_model"].as<char>();
	parameters.sync = vm["syncronize"].as<bool>();

	parameters.seed = vm["random_seed"].as<int>();
	parameters.generations = vm["generations"].as<int>();

	parameters.potential = vm["base_potential"].as<float>();
	parameters.standard_deviation = vm["standard_deviation"].as<float>();
}