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
		std::string bias_file;

		char model;
		bool sync;
		int seed;
		int generations;
		int time_constant;
		float potential;
		float standard_deviation;
	}Parameters;

	Simulator(){};
	~Simulator(){};

	void run(po::variables_map &vm);

private:
	Parameters parameters;
	std::ofstream ofs;

	void setParameters(po::variables_map &vm);
};

#endif
