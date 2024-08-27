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

void NeuralNetworkModel::readBiases(ifstream& ifs) {
  uint32_t size;
  ifs.read((char*)&size, sizeof(uint32_t));
  biases.resize(size);
  ifs.read((char*)&biases[0], sizeof(Bias) * size);
}

void NeuralNetworkModel::readWeights(ifstream& ifs) {
  uint32_t neuron_size, size;
  ifs.read((char*)&neuron_size, sizeof(uint32_t));
  weights.resize(neuron_size);

  for (uint32_t i = 0; i < neuron_size; ++i) {
    ifs.read((char*)&size, sizeof(uint32_t));
    weights[i].resize(size);
    ifs.read((char*)&weights[i][0], sizeof(Weight) * size);
  }
}

NeuralNetworkModel::NeuralNetworkModel(const Parameters& p) : parameters(p) {
  ifstream ifs;
  utils::fileOpen(ifs, parameters.input_path, ios::binary);

  ifs.read((char*)&problem_type, sizeof(int));
  cout << "problem type: " << problem_type << endl;

  // read bias file, and init biases
  Timer timer;
  readBiases(ifs);
  timer.elapsed("read biases file", 2);

  // read weights file, and init weights
  timer.restart();
  readWeights(ifs);
  timer.elapsed("read weights file", 2);

  // polyomino = Polyomino(ifs);

  ifs.close();

// #ifdef GUI
//   #if defined __linux__ || defined __APPLE__
//     if (mkdir(parameters.output_path.c_str(), 0755) != 0) {
//       cerr << "[ERROR] failed to create directory" << endl;
//       exit(0);
//     }
//   #elif _WIN32
//     if (_makedir(parameters.output_path) != 0) {
//       cerr << "[ERROR] failed to create directory" << endl;
//       exit(0);
//     }
//   #else
//   #endif
// #endif

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
// #ifdef GUI

//   ofstream ofs;
//   utils::fileOpen(ofs, parameters.output_path + "/" + to_string(generation), ios::out);
//   ofs << calcEnergy(generation) << endl << endl;

//   writeOutputs(ofs);
//   writePotentials(ofs);

//   ofs.close();

// #elif defined(EXP)

//   if (generation == parameters.generations) {
//     ofstream ofs;
//     utils::fileOpen(ofs, parameters.output_path, ios::out | ios::app);

//     binarization();
//     ofs << calcEnergy(generation);
//     ofs << polyomino.getGoalStatus(outputs) << endl;

//     ofs.close();
//   }

// #endif
  ofstream ofs;
  if (generation == 0) {
    utils::fileOpen(ofs, parameters.output_path, ios::out);
  } else {
    utils::fileOpen(ofs, parameters.output_path, ios::out | ios::app);
  }
  ofs << to_string(generation) << endl;
  ofs << calcEnergy(generation) << endl;

  writeOutputs(ofs);
  writePotentials(ofs);
  ofs << endl;

  ofs.close();
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
