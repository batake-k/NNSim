#include "neuron.hpp"

using namespace std;

Neuron::Neuron(double _bias): bias(_bias){
  output = 0.5;
  output_old = 0.5;
}
