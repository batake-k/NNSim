#include "simulator.hpp"
#include "gaussian_model.hpp"
#include "hopfield_model.hpp"
#include "timer.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

namespace{
	void setNNMParameters(const Simulator::Parameters& p, NeuralNetworkModel::Parameters& NNMp){
		NNMp.weights_file = p.weights_file;
		NNMp.biases_file = p.biases_file;
		NNMp.output_file = p.output_file;

		NNMp.synchronize = p.synchronize;
		NNMp.inner_potential = p.inner_potential;
		NNMp.seed = p.seed;
		NNMp.generations = p.generations;
		NNMp.time_constant = p.time_constant;
		NNMp.delta_t = p.delta_t;
		NNMp.base_potential = p.base_potential;
	}
};

void Simulator::run(po::variables_map &vm){
	Timer timer;
	setParameters(vm);

	NeuralNetworkModel::Parameters NNM_parameters;
	setNNMParameters(parameters, NNM_parameters);

	std::shared_ptr<NeuralNetworkModel> model;
	if(parameters.model == 'h'){
		model = make_shared<HopfieldModel>(NNM_parameters);
	}else if(parameters.model == 'g'){
		model = make_shared<GaussianModel>(NNM_parameters, parameters.standard_deviation);
	}
	timer.elapsed("init network model", 1);

	timer.restart();
	model->simulate();
	timer.elapsed("update neurons", 1);
}

void Simulator::setParameters(po::variables_map &vm){
	parameters.output_file = vm["output"].as<string>();
	parameters.weights_file = vm["weights"].as<string>();
	parameters.biases_file = vm["biases"].as<string>();

	cout << "weights file:       " << parameters.weights_file << endl;
	cout << "biases file:        " << parameters.biases_file << endl;
	cout << "output file:        " << parameters.output_file << endl;

	parameters.model = vm["network_model"].as<char>();

	if(parameters.model == 'h'){
		cout << "network model:      hopfield" << endl;
	}else if(parameters.model == 'g'){
		cout << "network model:      gaussian" << endl;
	}

	parameters.synchronize = vm["synchronize"].as<bool>();
	cout << "shynchronize:       " << parameters.synchronize << endl;

	parameters.inner_potential = vm["inner_potential"].as<bool>();
	cout << "inner potential: " << parameters.inner_potential << endl;

	parameters.seed = vm["random_seed"].as<uint32_t>();
	cout << "random seed:        " << parameters.seed << endl;

	parameters.generations = vm["generations"].as<uint32_t>();
	cout << "generations:        " << parameters.generations << endl;

	parameters.time_constant = vm["time_constant"].as<uint32_t>();
	parameters.delta_t = vm["delta_t"].as<float>();
	parameters.base_potential = vm["base_potential"].as<float>();
	parameters.standard_deviation = vm["standard_deviation"].as<float>();
}
