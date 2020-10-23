#ifndef NEURON_HPP
#define NEURON_HPP

class Neuron {
public:
  Neuron() {};
  Neuron(float _output, float _bias);
  ~Neuron() {};

  void update(){
    output_old = output;
  }

  void setOutput(float value){
    output = value;
  }

  float getOutput(){
    return output_old;
  }

  float getBias(){
    return bias;
  }

private:
  float output;
  float output_old;
  float bias;
};

#endif
