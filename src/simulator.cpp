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

	ofs.open(parameters.output_file);
	if((ofs.fail())){
		cout << "[ERROR] file open error: " << parameters.output_file << endl;
		exit(1);
	}

	std::shared_ptr<NeuralNetworkModel> model;
	if(parameters.model == 'h'){
		model = make_shared<HopfieldModel>(parameters.weights_file, parameters.bias_file, parameters.seed, parameters.time_constant, parameters.base_potential);
	}else{
		model = make_shared<GaussianModel>(parameters.weights_file, parameters.bias_file, parameters.seed, parameters.time_constant, parameters.base_potential, parameters.standard_deviation);
	}
	timer.elapsed("init network model", 1);

	//TODO
	uint32_t num_neurons = model->getNumNeuron();
	uint32_t N = (uint32_t)sqrt(num_neurons);
	if(N * N != num_neurons){
		cout << "N^2: " << N*N << ", num neurons: " << num_neurons << endl;
		exit(0);
	}

	timer.restart();
	ofs << model->calcE(N) << endl;

	Timer timer2;
	for(int i=0; i<parameters.generations; ++i){
		timer2.restart();
		model->update(parameters.internal_potential);
		ofs << model->calcE(N) << endl;
		timer2.elapsed("update", 2);
	}

	//ofs << model->output(N) << endl;

	timer.elapsed("update neurons", 1);
}

void Simulator::setParameters(po::variables_map &vm){
	parameters.output_file = vm["output"].as<string>();
	parameters.weights_file = vm["weights"].as<string>();
	parameters.bias_file = vm["bias"].as<string>();

	parameters.model = vm["network_model"].as<char>();
	parameters.internal_potential = vm["internal_potential"].as<bool>();
	parameters.seed = vm["random_seed"].as<int>();
	parameters.generations = vm["generations"].as<int>();
	parameters.time_constant = vm["time_constant"].as<int>();
	parameters.base_potential = vm["base_potential"].as<float>();
	parameters.standard_deviation = vm["standard_deviation"].as<float>();
}
