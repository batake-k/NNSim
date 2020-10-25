#include "simulator.hpp"
#include "utils.hpp"
#include "timer.hpp"

#include <string>
#include <iostream>
#include <iomanip>

using namespace std;

namespace po = boost::program_options;

namespace {

	po::options_description DefineOption(){
		po::options_description opt;
		po::options_description req_opt("(Required)");
		po::options_description opt_opt("(Optional)");

		req_opt.add_options()
			("weights,w", po::value<std::string>(), "neuron weights file")
			("bias,b", po::value<std::string>(), "neuron bias file")
			("output,o", po::value<std::string>(), "output file");

		opt_opt.add_options()
			("network_model,m", po::value<char>()->default_value('g'), "network model, h(hopfield) or g(gaussian)")
			//("syncronize,s", po::value<bool>()->default_value(0), "syncronously update neurons, 0(async) or 1(sync)")
			("internal_potential,i", po::value<bool>()->default_value(true), "have internal potential, true of false")
      ("random_seed,r", po::value<int>()->default_value(0), "random seed")
      ("generations,g", po::value<int>()->default_value(100), "number of generations")
			("time_constant,t", po::value<int>()->default_value(100), "time constant")
			("base_potential,B", po::value<float>()->default_value(0.01), "base potential in tanh")
			("standard_deviation,S", po::value<float>()->default_value(0.1), "standard deviation for noise");

		opt.add(req_opt).add(opt_opt);
		return opt;
	}

	po::variables_map CommandParse(int argc, char *argv[], po::options_description opt){
			po::variables_map vm;
		try{
				po::store(po::parse_command_line(argc, argv, opt), vm);
		}catch(const po::error_with_option_name& e){
				std::cerr << e.what() << std::endl;
			exit(1);
		}
		po::notify(vm);
		return vm;
	}

}

int main(int argc, char *argv[]){

	Timer timer;

	po::options_description opt = DefineOption();
	po::variables_map vm = CommandParse(argc, argv, opt);

	if(!vm.count("output") || !vm.count("weights") || !vm.count("bias")){
		cout << "usage: nnsim [<options>]" << endl
				 << opt << endl;
		exit(0);
	}

	Simulator simulator;
	simulator.run(vm);

	timer.elapsed("real time", 0);
	cout << "[peak memory usage] " << utils::Get_max_memory_consumption() << " GB" << endl;
}
