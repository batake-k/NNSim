#include "hopfield_model.hpp"

#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  if(argc != 8){
    cout << "./nnsim [num neurons] [weight file] [bias file] [output file] [gain] [times] [sync]" << endl;
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

  if(_sync == 0){
    sync = false;
  }

  cout << "sync: " << sync << endl
       << "gain: " << gain << endl
       << "times: " << times << endl;

  HopfieldModel hm(sync, gain, num_neurons, weight_file, bias_file, output_file);

  hm.output(5);

  for(int i = 0; i<times; ++i){
    hm.update();
    hm.output(5);
  }
}
