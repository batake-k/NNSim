/**
 * main関数を記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#include "calculator.hpp"

#include <string>
#include <iostream>

#include <boost/program_options.hpp>

using namespace std;

namespace po = boost::program_options;

namespace {

  po::options_description DefineOption(){
    po::options_description opt;
    opt.add_options()
      ("input_file,i", po::value<std::string>(), "hex problem file")
      ("output_file,o", po::value<std::string>(), "output file")
      ("A,a", po::value<float>()->default_value(0), "Constraint A: None")
      ("B,b", po::value<float>()->default_value(0), "Constraint B: for overlap")
      ("C,c", po::value<float>()->default_value(0), "Constraint C: for bubble")
      ("D,d", po::value<float>()->default_value(0), "Constraint D: None")
      ("E,e", po::value<float>()->default_value(0), "Constraint E: for penalty")
      ("cut_bubble_size,s", po::value<int>(), "maximum bubble size")
      ("output_data,x", po::value<bool>()->default_value(false), "flag to output data file")
      ("output_data_detail,y", po::value<bool>()->default_value(false), "flag to output data detail file")
      ("output_info,z", po::value<bool>()->default_value(false), "flag to output info file");
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

};

int main(int argc, char *argv[]){

  po::options_description opt = DefineOption();
  po::variables_map vm = CommandParse(argc, argv, opt);

  if(!vm.count("input_file") || !vm.count("output_file") || !vm.count("cut_bubble_size")){
    cout << "usage: hex [<options>]" << endl
         << opt << endl;
    exit(0);
  }

  string input_file = vm["input_file"].as<string>();
  string output_file = vm["output_file"].as<string>();
  float A = vm["A"].as<float>();
  float B = vm["B"].as<float>();
  float C = vm["C"].as<float>();
  float D = vm["D"].as<float>();
  float E = vm["E"].as<float>();
  int cut_bubble_size = vm["cut_bubble_size"].as<int>();
  bool flag_info = vm["output_info"].as<bool>();
  bool flag_data = vm["output_data"].as<bool>();
  bool flag_data_detail = vm["output_data_detail"].as<bool>();

  if(!flag_info && !flag_data && !flag_data_detail){
    cout << "please set some output option true." << endl;
    exit(0);
  }

  cout << "input file:  " << input_file << endl
       << "output file: " << output_file << endl
       << "A:           " << A << endl
       << "B:           " << B << endl
       << "C:           " << C << endl
       << "D:           " << D << endl
       << "E:           " << E << endl
       << "bubble size: " << cut_bubble_size << endl
       << "info file:   " << flag_info << endl
       << "data file:   " << flag_data << endl
       << "detail file: " << flag_data_detail << endl;

  Calculator::Parameter p =
  {
    input_file,
    output_file,
    A, B, C, D, E,
    cut_bubble_size
  };

  Calculator calculator(p);

  if(flag_info){
    calculator.writeInfo();
  }
  if(flag_data){
    calculator.writeData();
  }
  if(flag_data_detail){
    calculator.writeDataDetail();
  }

  return 0;
}
