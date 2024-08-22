#include <boost/test/unit_test.hpp>
#include "neural_network_model.hpp"
#include "simulator.hpp"

BOOST_AUTO_TEST_CASE(binary_reader_ok_1){
  // generated the input file with 
  //  ./polyomino/polyomino generate -b 12 -p 3,4,5 -o src/test_data/tmp
  //  ./polyomino/polyomino calculate -i src/test_data/tmp -o src/test_data/tmp -p 1 -s 2
  //  mv src/test_data/tmp_data src/test_data/example_polyomino_binary

  NeuralNetworkModel::Parameters parameters;
  parameters.input_path = "src/test_data/example_polyomino_binary";
  parameters.output_path = "src/test_data/tmp";
  parameters.synchronize = false;
  parameters.seed = 0;
  parameters.generations = 300;
  parameters.threads = 1;
  parameters.time_constant = 1;
  parameters.delta_t = 1.0;
  NeuralNetworkModel model(parameters);
}
BOOST_AUTO_TEST_CASE(text_reader_ok_1){

}