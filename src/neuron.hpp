#ifndef NEURON_HPP
#define NEURON_HPP

class Neuron {
public:
  Neuron() {};
  Neuron(float _output, float _potential);
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

	float getPotential(){
		return potential;
	}

	void setPotential(float value){
		potential = value;
	}

private:
  float output;
  float output_old;
	float potential;
};

#endif
