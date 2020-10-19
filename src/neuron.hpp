#ifndef NEURON_HPP
#define NEURON_HPP

class Neuron {
public:
  Neuron() {};
  Neuron(double _bias);
  ~Neuron() {};

  void update(){
    output_old = output;
  }

  void setOutput(double value){
    output = value;
  }

  double getOutput(){
    return output_old;
  }

  double getBias(){
    return bias;
  }

private:
  double output;
  double output_old;
  double bias;
};

#endif
