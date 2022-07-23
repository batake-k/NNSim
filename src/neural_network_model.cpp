#include "neural_network_model.hpp"

#include "timer.hpp"
#include "utils.hpp"

#include <iostream>

#if defined __linux__ || defined __APPLE__
  #include <sys/stat.h>
#elif defined _WIN32
  #include <direct.h>
#endif

using namespace std;

void NeuralNetworkModelBinaryReader::open(const std::string& filepath) {
  utils::fileOpen(this->ifs, filepath, std::ios::in | std::ios::binary);
}
void NeuralNetworkModelTextReader::open(const std::string& filepath) {
  utils::fileOpen(this->ifs, filepath, std::ios::in);
}

int NeuralNetworkModelBinaryReader::read_problem_type() {
  int problem_type;
  ifs.read((char*)&problem_type, sizeof(int));
  return problem_type;
}
int NeuralNetworkModelTextReader::read_problem_type() {
  int problem_type;
  ifs >> problem_type;
  return problem_type;
}
std::vector<Bias> NeuralNetworkModelBinaryReader::read_biases() {
  uint32_t size;
  std::vector<Bias> biases;
  ifs.read((char*)&size, sizeof(uint32_t));
  biases.resize(size);
  ifs.read((char*)&biases[0], sizeof(Bias) * size);
  return biases;
}
std::vector<Bias> NeuralNetworkModelTextReader::read_biases() {
  uint32_t size;
  std::vector<Bias> biases;
  ifs >> size;
  biases.resize(size);
  for (uint32_t i = 0; i < size; ++i) {
    ifs >> biases[i].before_bias >> biases[i].after_bias;
  }
  return biases;
}

std::vector<std::vector<Weight>> NeuralNetworkModelBinaryReader::read_weights() {
  std::vector<std::vector<Weight>> weights;
  uint32_t neuron_size, size;
  ifs.read((char*)&neuron_size, sizeof(uint32_t));
  weights.resize(neuron_size);

  for (uint32_t i = 0; i < neuron_size; ++i) {
    ifs.read((char*)&size, sizeof(uint32_t));
    weights[i].resize(size);
    ifs.read((char*)&weights[i][0], sizeof(Weight) * size);
  }
  return weights;
}
std::vector<std::vector<Weight>> NeuralNetworkModelTextReader::read_weights() {
  std::vector<std::vector<Weight>> weights;
  uint32_t neuron_size, size;
  ifs >> neuron_size;
  weights.resize(neuron_size);
  for (uint32_t i = 0; i < neuron_size; ++i) {
    ifs >> size;
    weights[i].resize(size);
    for (uint32_t j = 0; j < size; ++j) {
      ifs >> weights[i][j].before_weight >> weights[i][j].after_weight;
    }
  }
  return weights;
}


NeuralNetworkModel::NeuralNetworkModel(const Parameters& p) : parameters(p) {
  NeuralNetworkModelReaderBase *reader;
  if (utils::isBinaryFile(parameters.input_path)) {
    reader = new NeuralNetworkModelBinaryReader();
  } else {
    std::cout << "NeuralNetworkModelTextReader" << std::endl;
    reader = new NeuralNetworkModelTextReader();
  }
  reader->open(parameters.input_path);

  this->problem_type = reader->read_problem_type();
  cout << "problem type: " << problem_type << endl;

  // read bias file, and init biases
  Timer timer;
  this->biases = reader->read_biases();
  timer.elapsed("read biases file", 2);
  cout << "biases size: " << biases.size() << endl;

  // read weights file, and init weights
  timer.restart();
  this->weights = reader->read_weights();
  timer.elapsed("read weights file", 2);
  cout << "weights size: " << weights.size() << " " << weights[0].size() << endl;

  reader->close();
  delete reader;

#ifdef GUI
  #if defined __linux__ || defined __APPLE__
    if (mkdir(parameters.output_path.c_str(), 0755) != 0) {
      cerr << "[ERROR] failed to create directory" << endl;
      exit(0);
    }
  #elif _WIN32
    if (_makedir(parameters.output_path) != 0) {
      cerr << "[ERROR] failed to create directory" << endl;
      exit(0);
    }
  #else
  #endif
#endif

  num_neurons = biases.size();

  // set mt and rand_int for select random neuron
  for (uint32_t i = 0; i < parameters.threads; ++i) {
    mt.emplace_back(mt19937(parameters.seed + i));
  }

  rand_int = uniform_int_distribution<>(0, num_neurons - 1);

  // calc reciprocal_time_constant for update potentials
  reciprocal_time_constant = 1.0 / parameters.time_constant;

  // resize data
  potentials.resize(num_neurons);
  outputs.resize(num_neurons);
  outputs_old.resize(num_neurons);
}

void NeuralNetworkModel::writeData(const uint32_t generation) {
#ifdef GUI

  ofstream ofs;
  utils::fileOpen(ofs, parameters.output_path + "/" + to_string(generation), ios::out);
  ofs << calcEnergy(generation) << endl << endl;

  writeOutputs(ofs);
  writePotentials(ofs);

  ofs.close();

#elif defined(EXP)

  if (generation == parameters.generations) {
    ofstream ofs;
    utils::fileOpen(ofs, parameters.output_path, ios::out | ios::app);

    binarization();
    ofs << calcEnergy(generation);

    ofs.close();
  }

#endif
}

void NeuralNetworkModel::writeOutputs(ofstream& ofs) {

  for (uint32_t i = 0; i < num_neurons; ++i) {

    if (i == num_neurons - 1) {
      ofs << outputs[i];
    } else {
      ofs << outputs[i] << ",";
    }
  }

  ofs << endl;
}

void NeuralNetworkModel::writePotentials(ofstream& ofs) {

  for (uint32_t i = 0; i < num_neurons; ++i) {

    if (i == num_neurons - 1) {
      ofs << potentials[i];
    } else {
      ofs << potentials[i] << ",";
    }
  }

  ofs << endl;
}

void NeuralNetworkModel::binarization() {

  for (uint32_t i = 0; i < num_neurons; ++i) {

    if (outputs[i] >= 0.5) {
      outputs[i] = 1.0;
    } else {
      outputs[i] = 0.0;
    }
  }
}

void NeuralNetworkModel::step() {
  if (this->parameters.synchronize) {
    this->stepSync();
  } else {
    this->stepAsync();
  }
}

double NeuralNetworkModel::calcEnergy(const uint32_t generation) {
  double E = 0;

  for (uint32_t i = 0; i < num_neurons; ++i) {

    for (const auto& w : weights[i]) {
      float weight = w.before_weight + (w.after_weight - w.before_weight) * generation / parameters.generations;
      E -= 0.5 * outputs[i] * outputs[w.neuron_id] * weight;
    }

    float bias =
        biases[i].before_bias + (biases[i].after_bias - biases[i].before_bias) * generation / parameters.generations;
    E -= outputs[i] * bias;
  }

  return E;
}
