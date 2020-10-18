//TODO
//#include "discrete_neuron.hpp"
#include "neural_network_model.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  if(argc != 4){
    cout << "./nnsim [num neurons] [weight file] [bias file]" << endl;
    return 0;
  }

  int num_neurons = stoi(argv[1]);
  string weight_file = argv[2];
  string bias_file = argv[3];

  NeuralNetworkModel nnm(true, num_neurons, weight_file, bias_file);
}
