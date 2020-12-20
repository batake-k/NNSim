#include "simulator.hpp"
#include "gaussian_model.hpp"
#include "timer.hpp"

#include <iostream>
#include <string>

using namespace std;

namespace{
	void setNNMParameters(const Simulator::Parameters& p, NeuralNetworkModel::Parameters& NNMp){
		NNMp.weights_file = p.weights_file;
		NNMp.biases_file = p.biases_file;
		NNMp.output_folder = p.output_folder;

		NNMp.synchronize = p.synchronize;
		NNMp.seed = p.seed;
		NNMp.generations = p.generations;
		NNMp.threads = p.threads;
		NNMp.time_constant = p.time_constant;
		NNMp.delta_t = p.delta_t;
	}

	void setGMSParameters(const Simulator::Parameters& p, GaussianModel::SharpeningParameters& GMSp){
		GMSp.T_mf = p.T_mf;
		GMSp.time_constant_T_mf = p.time_constant_T_mf;
	}

	void setGMAParameters(const Simulator::Parameters& p, GaussianModel::AnnealingParameters& GMAp){
		GMAp.T_epsilon = p.T_epsilon;
		GMAp.time_constant_T_epsilon = p.time_constant_T_epsilon;
	}
};

void Simulator::run(po::variables_map &vm){
	Timer timer;
	setParameters(vm);

	NeuralNetworkModel::Parameters NNM_parameters;
	setNNMParameters(parameters, NNM_parameters);

	GaussianModel::SharpeningParameters GMS_parameters;
	setGMSParameters(parameters, GMS_parameters);

	GaussianModel::AnnealingParameters GMA_parameters;
	setGMAParameters(parameters, GMA_parameters);

	GaussianModel model(NNM_parameters, GMS_parameters, GMA_parameters);
	timer.elapsed("init network model", 1);

	timer.restart();
	model.simulate();
	timer.elapsed("simulate Neural Network", 1);
}

void Simulator::setParameters(po::variables_map &vm){
	// Required Options
	cout << "[Required Options]" << endl;
	parameters.output_folder = vm["output"].as<string>();
	parameters.weights_file = vm["weights"].as<string>();
	parameters.biases_file = vm["biases"].as<string>();

	cout << "weights file:  " << parameters.weights_file << endl;
	cout << "biases file:   " << parameters.biases_file << endl;
	cout << "output folder: " << parameters.output_folder << endl << endl;

	// Network Options
	cout << "[Network Options]" << endl;
	parameters.model = vm["network_model"].as<char>();
	if(parameters.model == 'h'){
		cout << "network model:   hopfield" << endl;
	}else if(parameters.model == 'g'){
		cout << "network model:   gaussian" << endl;
	}

	parameters.synchronize = vm["synchronize"].as<bool>();
	cout << "synchronize:     " << parameters.synchronize << endl;

	parameters.inner_potential = vm["inner_potential"].as<bool>();
	parameters.time_constant = vm["time_constant"].as<uint32_t>();
	parameters.delta_t = vm["delta_t"].as<float>();
	if(!parameters.inner_potential){
		parameters.time_constant = 1.0;
		parameters.delta_t = 1.0;
	}
	cout << "inner potential: " << parameters.inner_potential << endl;
	cout << "time_constant:   " << parameters.time_constant << endl;
	cout << "delta_t:         " << parameters.delta_t << endl;

	parameters.seed = vm["random_seed"].as<uint32_t>();
	cout << "random seed:     " << parameters.seed << endl;

	parameters.generations = vm["generations"].as<uint32_t>();
	cout << "generations:     " << parameters.generations << endl;

	parameters.threads = vm["threads"].as<uint32_t>();
	cout << "threads:         " << parameters.threads << endl << endl;

	//Gaussian Sharpening Parameters
	cout << "[Sharpening Parameters]" << endl;
	parameters.T_mf = vm["T_mf"].as<float>();
	cout << "T_mf:                    " << parameters.T_mf << endl;

	parameters.time_constant_T_mf = vm["time_constant_T_mf"].as<uint32_t>();
	cout << "time constant T_mf:      " << parameters.time_constant_T_mf << endl << endl;

	//Gaussian Annealing Parameters
	cout << "[Annealing Parameters]" << endl;
	if(parameters.model == 'g'){
		parameters.T_epsilon = vm["T_epsilon"].as<float>();
	}else{
		parameters.T_epsilon = 0.0;
	}
	cout << "T_epsilon:               " << parameters.T_epsilon << endl;

	parameters.time_constant_T_epsilon = vm["time_constant_T_epsilon"].as<uint32_t>();
	cout << "time constant T_epsilon: " << parameters.time_constant_T_epsilon << endl << endl;
}
