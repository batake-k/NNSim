#include "discrete_neuron.hpp"

//TODO
#include <iostream>

using namespace std;

DiscreteNeuron::DiscreteNeuron(){
  output = 1;
  output_old = 1;
  Neuron(1.0);
}

void DiscreteNeuron::status(){
  cout << "output:     " << output << endl
       << "output_old: " << output_old << endl
       << "bias:       " << bias << endl;
}
