#include "hopfield_model.hpp"
#include "gaussian_model.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  if(argc != 9){
    cout << "./nnsim [N] [weight file] [bias file] [output file] [gain] [times] [sync] [seed]" << endl;
    return 0;
  }

  int num_neurons = stoi(argv[1]);
  string weight_file = argv[2];
  string bias_file = argv[3];
  string output_file = argv[4];
  int gain = stoi(argv[5]);
  int times = stoi(argv[6]);
  int _sync = stoi(argv[7]);
  bool sync = true;
  int seed = stoi(argv[8]);

  if(_sync == 0){
    sync = false;
  }

  cout << "sync: " << sync << endl
       << "gain: " << gain << endl
       << "times: " << times << endl;

  HopfieldModel hm(sync, gain, num_neurons * num_neurons, weight_file, bias_file, output_file, seed);
  //GaussianModel hm(sync, gain, num_neurons * num_neurons, weight_file, bias_file, output_file, seed);

  hm.calcE(num_neurons);
  hm.output(num_neurons);

  for(int i = 0; i<times; ++i){
    hm.update();
    hm.calcE(num_neurons);
    hm.output(num_neurons);
  }

}
