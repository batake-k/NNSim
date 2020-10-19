#include "mcculloch_pitts_model.hpp"

#include <iostream>

using namespace std;

namespace {
  bool func(double input_sum){
    if(input_sum >= 0){
      return 1;
    }else{
      return 0;
    }
  }

};

MccullochPittsModel::MccullochPittsModel(bool _sync, int num_neurons, string weights_file, string bias_file):
    NeuralNetworkModel(num_neurons, weights_file, bias_file),sync(_sync){

    cout << "sync: " << sync << endl;
}

void MccullochPittsModel::update(){
  //TODO
  /* if (sync){
    
  }else{
    
  }*/

  
}
