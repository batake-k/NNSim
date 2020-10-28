#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>
#include <fstream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Simulator {

public:
	typedef struct {
		std::string output_file;
		std::string weights_file;
		std::string biases_file;

		char model;
		bool synchronize;
		bool inner_potential;
		uint32_t seed;
		uint32_t generations;
		uint32_t time_constant;
		float delta_t;
		float base_potential;
		float standard_deviation;
	}Parameters;

	Simulator(){};
	~Simulator(){};

	void run(po::variables_map &vm);

private:
	Parameters parameters;

	void setParameters(po::variables_map &vm);
};

#endif
