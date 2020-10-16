//TODO
#include "discrete_neuron.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  DiscreteNeuron dn;
  dn.status();
  cout << "get bias" << dn.getBias() << endl;
}
