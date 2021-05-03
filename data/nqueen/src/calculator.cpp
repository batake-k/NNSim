#include "calculator.hpp"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

namespace {
  int KDelta(int a, int b){
	  if (a == b){
		  return 1;
	  }else{
		  return 0;
	  }
  }
};

vector<WeightDetail> Calculator::calcWeightDetail(const int x, const int y){
  vector<WeightDetail> weights;

  for(int X=0; X<parameter.N; ++X){
    for(int Y=0; Y<parameter.N; ++Y){

      if(x*parameter.N + y == X*parameter.N + Y) continue;

      float b_A, b_B, b_C, b_D, b_E;
      float a_A, a_B, a_C, a_D, a_E;
      b_A = b_B = b_C = b_D = b_E = a_A = a_B = a_C = a_D = a_E = 0;

      b_A = - parameter.A * KDelta(x, X) * (1 - KDelta(y, Y));
      a_A = - parameter.A * KDelta(x, X) * (1 - KDelta(y, Y));

      b_B = - parameter.B * KDelta(y, Y) * (1 - KDelta(x, X));
      a_B = - parameter.B * KDelta(y, Y) * (1 - KDelta(x, X));

      b_C = - parameter.C * KDelta(x + y, X + Y) * (1 - KDelta(x, X));
      a_C = - parameter.C * KDelta(x + y, X + Y) * (1 - KDelta(x, X));

      b_D = - parameter.D * KDelta(x - y, X - Y) * (1 - KDelta(x, X));
      a_D = - parameter.D * KDelta(x - y, X - Y) * (1 - KDelta(x, X));

      if((b_A + b_B + b_C + b_D + b_E) != 0 && (a_A + a_B + a_C + a_D + a_E) != 0){
        uint32_t id = X * parameter.N + Y;
        WeightDetail wd = {id, b_A, b_B, b_C, b_D, b_E, a_A, a_B, a_C, a_D, a_E};
        weights.emplace_back(wd);
      }
    }
  }
  return weights;
}

vector<BiasDetail> Calculator::calcBiasDetail(){
  vector<BiasDetail> biases;

  for(int i=0; i<parameter.N; ++i){
    for(int j=0; j<parameter.N; ++j){
      float b_A, b_B, a_A, a_B;
      b_A = parameter.A / 2;
      a_A = parameter.A / 2;

      b_B = parameter.B / 2;
      a_B = parameter.B / 2;

      BiasDetail bd = {b_A, b_B, 0, 0, 0, a_A, a_B, 0, 0, 0};
      biases.emplace_back(bd);
    }
  }
  return biases;
}

void Calculator::writeDataDetail(){
  ofstream ofs(parameter.output_file + "_data_detail", ios::binary);
  uint32_t neurons_size = parameter.N * parameter.N;

  { // bias
    auto bias_details = calcBiasDetail();
    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&bias_details[0], sizeof(BiasDetail)*neurons_size);
  }

  { // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for(int x=0; x<parameter.N; ++x){
      for(int y=0; y<parameter.N; ++y){
        auto weight_details = calcWeightDetail(x, y);
        uint32_t size = weight_details.size();
        ofs.write((char*)&size, sizeof(uint32_t));
        ofs.write((char*)&weight_details[0], sizeof(WeightDetail)*size);
      }
    }
  }

  ofs.close();
}

void Calculator::writeData(){
  ofstream ofs(parameter.output_file + "_data", ios::binary);
  uint32_t neurons_size = parameter.N * parameter.N;

  { // problem type
    int type = 0;
    ofs.write((char*)&type, sizeof(int));
  }

  { // bias
    auto bias_details = calcBiasDetail();
    vector<Bias> biases;

    for(const auto &bd : bias_details){
      float b = bd.b_A + bd.b_B + bd.b_C + bd.b_D + bd.b_E;
      float a = bd.a_A + bd.a_B + bd.a_C + bd.a_D + bd.a_E;
      Bias bias = {b, a};
      biases.emplace_back(bias);
    }

    ofs.write((char*)&neurons_size, sizeof(uint32_t));
    ofs.write((char*)&biases[0], sizeof(Bias)*neurons_size);
  }

  { // weight
    ofs.write((char*)&neurons_size, sizeof(uint32_t));

    for(int x=0; x<parameter.N; ++x){
      for(int y=0; y<parameter.N; ++y){
        auto weight_details = calcWeightDetail(x, y);
        vector<Weight> weights;

        for(const auto &wd : weight_details){
          float b = wd.b_A + wd.b_B + wd.b_C + wd.b_D + wd.b_E;
          float a = wd.a_A + wd.a_B + wd.a_C + wd.a_D + wd.a_E;

          Weight weight = {wd.neuron_id, b, a};
          weights.emplace_back(weight);
        }

        uint32_t size = weights.size();
        ofs.write((char*)&size, sizeof(uint32_t));
        ofs.write((char*)&weights[0], sizeof(Weight)*size);
      }
    }
  }

  { // info
    ofs.write((char*)&parameter.N, sizeof(int));
  }

  ofs.close();
}
