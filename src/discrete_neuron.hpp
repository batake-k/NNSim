#ifndef DISCRETE_NEURON_HPP
#define DISCRETE_NEURON_HPP

#include "neuron.hpp"

class DiscreteNeuron : public Neuron {
public:
  DiscreteNeuron();

  void update(){
    output_old = output;
  }

  void setOutput(bool value){
    output = value;
  }

  bool getOutput(){
    return output_old;
  }

  //TODO
  void status();

private:
  bool output;
  bool output_old;
 
};

#endif
