#ifndef NEURON_HPP
#define NEURON_HPP

class Neuron {
public:
  Neuron();
  Neuron(double _bias){
    bias = _bias;
  }

  virtual ~Neuron();

  double getBias(){
    return bias;
  }

protected:
  double bias;
};

#endif
