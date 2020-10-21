#include "neuron.hpp"

using namespace std;

Neuron::Neuron(double _bias): bias(_bias){
  output = 1.0;
  output_old = 1.0;
}
