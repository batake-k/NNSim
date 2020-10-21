#include "simulator.hpp"
#include "utils.hpp"

#include <string>
#include <iostream>
#include <iomanip>

#include <omp.h>

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
			("network_model,m", po::value<char>()->default_value('g'), "network model, h(hopfield) or g(gaussian) [g]")
			("update_method,u", po::value<bool>()->default_value(0), "update method, 0(async) or 1(sync) [0]")
      ("random_seed,r", po::value<int>()->default_value(0), "random seed [0]")
      ("generations,g", po::value<int>()->default_value(100), "number of generations [100]")
			("base_potential,B", po::value<double>()->default_value(0.01), "base potential [0.01]")
			("standard_deviation,s", po::value<double>()->default_value(0.1), "standard deviation for gaussian noise [0.1]");

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

	double start, end;
	start = omp_get_wtime();

	po::options_description opt = DefineOption();
	po::variables_map vm = CommandParse(argc, argv, opt);

	if(!vm.count("output") || !vm.count("weights") || !vm.count("bias")){
		cout << "output: " << vm.count("output") << endl
				 << "weights: " << vm.count("weights") << endl
				 << "bias: " << vm.count("bias") << endl;
		cout << "usage: nnsim [<options>]" << endl
				 << opt << endl;
		exit(0);
	}

	Simulator simulator;
	simulator.run(vm);

	end = omp_get_wtime();
	cout << std::fixed << std::setprecision(2)
			 << "real time: " << end - start <<  " sec." << endl;
	cout << "peak memory usage: " << utils::Get_max_memory_consumption() << " GB" << endl;
}
