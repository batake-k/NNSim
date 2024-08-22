#include <boost/test/unit_test.hpp>
#include "neural_network_model_binary_reader.hpp"
#include "simulator.hpp"

BOOST_AUTO_TEST_CASE(binary_reader_ok_1){
  // generated the input file with 
  //  ./polyomino/polyomino generate -b 12 -p 3,4,5 -o src/test_data/tmp
  //  ./polyomino/polyomino calculate -i src/test_data/tmp -o src/test_data/tmp -p 1 -s 2
  //  mv src/test_data/tmp_data src/test_data/example_polyomino_binary
  NeuralNetworkModelBinaryReader reader;
  reader.open("src/test_data/example_polyomino_binary");
  reader.read_problem_type(); 
  reader.read_biases();
  reader.read_weights();
}
BOOST_AUTO_TEST_CASE(binary_reader_ng_wrong_order){
  NeuralNetworkModelBinaryReader reader;
  reader.open("src/test_data/example_polyomino_binary");
  BOOST_CHECK_THROW(reader.read_weights(), std::bad_alloc);
}
// BOOST_AUTO_TEST_CASE(binary_reader_ng_text_file){
//   NeuralNetworkModelBinaryReader reader;
//   reader.open("src/test_data/example_problem_text");
//   BOOST_CHECK_THROW(reader.read_weights(), SOMETHING); // process will be killed, and we cannot catch error
// }
