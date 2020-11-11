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
			("biases,b", po::value<std::string>(), "neuron biases file")
			("output,o", po::value<std::string>(), "output file");

		opt_opt.add_options()
			("network_model,m", po::value<char>()->default_value('g'), "network model, h(hopfield) or g(gaussian)")
			("synchronize,s", po::value<bool>()->default_value(true), "syncronously update, true or false")
			("inner_potential,i", po::value<bool>()->default_value(true), "have inner potential, true or false")
			("random_seed,r", po::value<uint32_t>()->default_value(0), "random seed")
			("generations,g", po::value<uint32_t>()->default_value(100), "number of generations")
			("time_constant,t", po::value<uint32_t>()->default_value(1), "time constant")
			("delta_t,d", po::value<float>()->default_value(0.01), "delta t")
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

	if(!vm.count("output") || !vm.count("weights") || !vm.count("biases")){
		cout << "usage: nnsim [<options>]" << endl
				 << opt << endl;
		exit(0);
	}

	Simulator simulator;
	simulator.run(vm);

	timer.elapsed("real time", 0);
	cout << "[peak memory usage] " << utils::getMaxMemoryConsumption() << " GB" << endl;
}
