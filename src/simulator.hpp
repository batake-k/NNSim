#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Simulator {
public:

	typedef struct {
		std::string output_folder;
		std::string weights_file;
		std::string biases_file;

		std::string hex_info_file;

		char model;
		bool synchronize;
		bool inner_potential;
		uint32_t seed;
		uint32_t generations;
		uint32_t threads;
		uint32_t time_constant;
		float delta_t;

		//Gaussian Sharpening Parameters
		float T_mf;
		uint32_t time_constant_T_mf;

		//Gaussian Annealing Parameters
		float T_epsilon;
		uint32_t time_constant_T_epsilon;
	}Parameters;

	Simulator(){};
	~Simulator(){};

	void run(po::variables_map &vm);

private:

	void setParameters(po::variables_map &vm);

	Parameters parameters;
};

#endif
