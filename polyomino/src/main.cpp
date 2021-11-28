/**
 * main関数を記述
 * @author Kazuki Takabatake
 * @date 2021/7/26
 */

#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "calculator.hpp"
#include "generator.hpp"

using namespace std;

namespace po = boost::program_options;

namespace {

po::options_description DefineCalcOption() {
  po::options_description opt;

  opt.add_options()
    ("input_file,i", po::value<std::string>(), "problem file")
    ("output_file,o", po::value<std::string>(), "output file")
    ("problem_type,p", po::value<int>(), "problem type (1, 2, 3)")
    ("A,a", po::value<float>()->default_value(0), "Constraint A: for number of use")
    ("B,b", po::value<float>()->default_value(0), "Constraint B: for overlap")
    ("C,c", po::value<float>()->default_value(0), "Constraint C: for bubble")
    ("D,d", po::value<float>()->default_value(0), "Constraint D: for piece connections")
    ("E,e", po::value<float>()->default_value(0), "Constraint E: for wall connections")
    ("F,f", po::value<float>()->default_value(0), "Constraint F: for minus weights")
    ("G,g", po::value<float>()->default_value(0), "Constraint G: for size")
    ("cut_bubble_size,s", po::value<int>(), "maximum bubble size")
    ("output_data,x", po::value<bool>()->default_value(false), "flag to output data file")
    ("output_data_detail,y", po::value<bool>()->default_value(false), "flag to output data detail file")
    ("output_info,z", po::value<bool>()->default_value(false), "flag to output info file");

  return opt;
}

po::options_description DefineGenOption() {
  po::options_description opt;

  opt.add_options()
    ("board_size,b", po::value<int>(), "board size")
    ("piece_sizes,p", po::value<std::string>(), "piece sizes")
    ("output_path,o", po::value<std::string>(), "output path");

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
  if (argc > 1 && strcmp(argv[1], "calculate") == 0) {
    po::options_description opt = DefineCalcOption();
    po::variables_map vm = CommandParse(argc - 1, argv + 1, opt);

    if (!vm.count("input_file") || !vm.count("output_file") || !vm.count("problem_type") || !vm.count("cut_bubble_size")) {
      cout << "usage: polyomino calculate [<options>]" << endl << opt << endl;
      exit(0);
    }

    string input_file = vm["input_file"].as<string>();
    string output_file = vm["output_file"].as<string>();
    int problem_type = vm["problem_type"].as<int>();
    float A = vm["A"].as<float>();
    float B = vm["B"].as<float>();
    float C = vm["C"].as<float>();
    float D = vm["D"].as<float>();
    float E = vm["E"].as<float>();
    float F = vm["F"].as<float>();
    float G = vm["G"].as<float>();
    int cut_bubble_size = vm["cut_bubble_size"].as<int>();
    bool flag_info = vm["output_info"].as<bool>();
    bool flag_data = vm["output_data"].as<bool>();
    bool flag_data_detail = vm["output_data_detail"].as<bool>();

    if (!flag_info && !flag_data && !flag_data_detail) {
      cout << "please set some output option true." << endl;
      exit(0);
    }

    if(problem_type == 1){
      G = 0;
    }else if(problem_type == 2){
      A = 0;
      G = 0;
    }else if(problem_type == 3){
      A = 0;
    }else{
      cout << "plese set problem_type appropriately." << endl;
      exit(0);
    }

    cout << "input file:  " << input_file << endl
         << "output file: " << output_file << endl
         << "A:           " << A << endl
         << "B:           " << B << endl
         << "C:           " << C << endl
         << "D:           " << D << endl
         << "E:           " << E << endl
         << "F:           " << F << endl
         << "G:           " << G << endl
         << "bubble size: " << cut_bubble_size << endl
         << "info file:   " << flag_info << endl
         << "data file:   " << flag_data << endl
         << "detail file: " << flag_data_detail << endl;

    Calculator::Parameter p = {input_file, output_file, problem_type, A, B, C, D, E, F, G, cut_bubble_size};

    Calculator calculator(p);

    if (flag_info) {
      calculator.writeInfo();
    }
    if (flag_data) {
      calculator.writeData();
    }
    if (flag_data_detail) {
      calculator.writeDataDetail();
    }

  } else if (argc > 1 && strcmp(argv[1], "generate") == 0) {
    po::options_description opt = DefineGenOption();
    po::variables_map vm = CommandParse(argc - 1, argv + 1, opt);

    if (!vm.count("board_size") || !vm.count("piece_sizes") || !vm.count("output_path")) {
      cout << "usage: polyomino generate [<options>]" << endl << opt << endl;
      exit(0);
    }

    int board_size = vm["board_size"].as<int>();
    string piece_sizes = vm["piece_sizes"].as<string>();
    string output_path = vm["output_path"].as<string>();

    Generator::Parameter p = {board_size, piece_sizes, output_path};

    Generator generator(p);
    generator.run();
    generator.writeData();

  } else {
    cout << "There are two sub-commands:" << endl
         << "calculate: calculate data for NNSim" << endl
         << "generate:  generate puzzle" << endl;
  }

  return 0;
}
