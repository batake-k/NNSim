/**
 * main関数を記述
 * @author Kazuki Takabatake
 * @date 2021/5/3
 */

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "calculator.hpp"

using namespace std;

namespace po = boost::program_options;

namespace {

po::options_description DefineOption() {
  po::options_description opt;
  opt.add_options()
    ("output_file,o", po::value<std::string>(), "output file")
    ("N,n", po::value<int>(), "number of queens")
    ("A,a", po::value<float>()->default_value(0), "Constraint A: vertical")
    ("B,b", po::value<float>()->default_value(0), "Constraint B: horizontal")
    ("C,c", po::value<float>()->default_value(0), "Constraint C: diagonal 1")
    ("D,d", po::value<float>()->default_value(0), "Constraint D: diagonal 2")
    ("E,e", po::value<float>()->default_value(0), "Constraint E: None")
    ("F,f", po::value<float>()->default_value(0), "Constraint F: None")
    ("output_data,x", po::value<bool>()->default_value(false), "flag to output data file")
    ("output_data_detail,y", po::value<bool>()->default_value(false), "flag to output data detail file");
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
  po::options_description opt = DefineOption();
  po::variables_map vm = CommandParse(argc, argv, opt);

  if (!vm.count("output_file") || !vm.count("N")) {
    cout << "usage: nqueen [<options>]" << endl << opt << endl;
    exit(0);
  }

  string output_file = vm["output_file"].as<string>();
  int N = vm["N"].as<int>();
  float A = vm["A"].as<float>();
  float B = vm["B"].as<float>();
  float C = vm["C"].as<float>();
  float D = vm["D"].as<float>();
  float E = vm["E"].as<float>();
  float F = vm["F"].as<float>();
  bool flag_data = vm["output_data"].as<bool>();
  bool flag_data_detail = vm["output_data_detail"].as<bool>();

  if (!flag_data && !flag_data_detail) {
    cout << "please set some output option true." << endl;
    exit(0);
  }

  cout << "output file: " << output_file << endl
       << "N:           " << N << endl
       << "A:           " << A << endl
       << "B:           " << B << endl
       << "C:           " << C << endl
       << "D:           " << D << endl
       << "E:           " << E << endl
       << "F:           " << F << endl
       << "data file:   " << flag_data << endl
       << "detail file: " << flag_data_detail << endl;

  Calculator::Parameter p = {
      output_file, N, A, B, C, D, E, F,
  };

  Calculator calculator(p);

  if (flag_data) {
    calculator.writeData();
  }
  if (flag_data_detail) {
    calculator.writeDataDetail();
  }

  return 0;
}
