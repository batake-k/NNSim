#include <iomanip>
#include <string>
#include <iostream>

#include "timer.hpp"
#include "utils.hpp"
#include "simulator.hpp"

#ifdef _OPENMP
  #include <omp.h>
#endif

using namespace std;

namespace po = boost::program_options;

namespace {

po::options_description DefineOption() {
  po::options_description opt;
  po::options_description req_opt("(Required)");
  po::options_description n_opt("(Network Options)");
  po::options_description g_opt("(Gaussian Machine Options)");

  req_opt.add_options()
    ("input,i", po::value<std::string>(), "input data file")
    ("output,o", po::value<std::string>(), "output file");

  n_opt.add_options()
    ("network_model,n", po::value<char>()->default_value('g'), "network model, h(hopfield) or g(gaussian)")
    ("synchronize,s", po::value<bool>()->default_value(false), "syncronously update, true or false")
    ("inner_potential,p", po::value<bool>()->default_value(true), "have inner potential, true or false")
    ("random_seed,r", po::value<uint32_t>()->default_value(0), "random seed")
    ("generations,g", po::value<uint32_t>()->default_value(300), "number of generations")
    ("threads,T", po::value<uint32_t>()->default_value(1), "number of threads")
    ("time_constant,t", po::value<uint32_t>()->default_value(1), "time constant")
    ("delta_t,d", po::value<float>()->default_value(1), "delta t");

  g_opt.add_options()
    ("T_mf,m", po::value<float>()->default_value(1), "[sharpening] T_mf value at start")
    ("time_constant_T_mf,M", po::value<uint32_t>(), "[sharpening] time constant T_mf. If not set, equal to generations")
    ("T_epsilon,e", po::value<float>()->default_value(0), "[annealing] T_epsilon value at start")
    ("time_constant_T_epsilon,E", po::value<uint32_t>(), "[annealing] time constant T_epsilon. If not set, equal to generations");

  opt.add(req_opt).add(n_opt).add(g_opt);
  return opt;
}

po::variables_map CommandParse(int argc, char *argv[], po::options_description opt) {
  po::variables_map vm;
  try {
    po::store(po::parse_command_line(argc, argv, opt), vm);
  } catch (const po::error_with_option_name &e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }
  po::notify(vm);
  return vm;
}

};  // namespace

int main(int argc, char *argv[]) {
  Timer timer;

  po::options_description opt = DefineOption();
  po::variables_map vm = CommandParse(argc, argv, opt);

  if (!vm.count("input") || !vm.count("output")) {
    cout << "usage: nnsim [<options>]" << endl << opt << endl;
    exit(0);
  }

// openmp setting
#ifdef _OPENMP
  omp_set_num_threads(vm["threads"].as<uint32_t>());
#endif
  Simulator simulator;
  simulator.run(vm);

  timer.elapsed("real time", 0);
  cout << "[peak memory usage] " << utils::getMaxMemoryConsumption() << " GB" << endl;
}
