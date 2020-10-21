#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include <string>

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
		double potential;
		double standard_deviation;
	}Parameters;

	Simulator(){};
	~Simulator(){};

	void run(po::variables_map &vm);

private:
	Parameters parameters;
	void setParameters(po::variables_map &vm);
};

#endif
