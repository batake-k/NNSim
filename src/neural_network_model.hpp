#ifndef NEURAL_NETWORK_MODEL_HPP
#define NEURAL_NETWORK_MODEL_HPP

#include <fstream>
#include <random>
#include <string>
#include <vector>


typedef struct {
  float before_bias;
  float after_bias;
} Bias;

typedef struct {
  uint32_t neuron_id;
  float before_weight;
  float after_weight;
} Weight;

class NeuralNetworkModel {
public:

  typedef struct {
    std::string input_path;
    std::string output_path;

    bool synchronize;
    uint32_t seed;
    uint32_t generations;
    uint32_t threads;
    uint32_t time_constant;
    float delta_t;
  } Parameters;

  NeuralNetworkModel(){};
  NeuralNetworkModel(const Parameters& parameters);
  virtual ~NeuralNetworkModel(){};

  virtual void simulate(){};

protected:
  void writeData(const uint32_t generation);
  void writeOutputs(std::ofstream& ofs);
  void writePotentials(std::ofstream& ofs);

  double calcEnergy(const uint32_t generation);

  std::vector<float> potentials;
  std::vector<float> outputs;
  std::vector<float> outputs_old;
  std::vector<Bias> biases;
  std::vector<std::vector<Weight>> weights;

  std::vector<std::mt19937> mt;
  std::uniform_int_distribution<> rand_int;

  Parameters parameters;
  uint32_t num_neurons;
  float reciprocal_time_constant;

private:
  virtual void initNeurons(){};
  virtual float func(const float input) { return 0; };
  virtual float inverseFunc(const float input) { return 0; };

  void readWeights(std::ifstream& ifs);
  void readBiases(std::ifstream& ifs);

  void binarization();

  int problem_type;
};


class NeuralNetworkModelReaderBase {
public:
  virtual void open(const std::string& input_path) = 0;
  virtual void close() { this->ifs.close(); };
  virtual int read_problem_type() = 0;
  virtual std::vector<Bias> read_biases() = 0;
  virtual std::vector<std::vector<Weight>> read_weights() = 0;
  ~NeuralNetworkModelReaderBase() { this->close(); };
protected:
  std::ifstream ifs;
};

class NeuralNetworkModelBinaryReader : public NeuralNetworkModelReaderBase {
public:
  NeuralNetworkModelBinaryReader() {};
  void open(const std::string& input_path);
  int read_problem_type();
  std::vector<Bias> read_biases();
  std::vector<std::vector<Weight>> read_weights();
};

class NeuralNetworkModelTextReader : public NeuralNetworkModelReaderBase {
public:
  NeuralNetworkModelTextReader() {};
  void open(const std::string& input_path);
  int read_problem_type();
  std::vector<Bias> read_biases();
  std::vector<std::vector<Weight>> read_weights();
};

#endif
