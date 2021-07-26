/**
 * 結合重み・バイアス値を計算するクラス
 * @author Kazuki Takabatake
 * @date 2021/7/26
 */

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <vector>

#include "neuron.hpp"
#include "piece.hpp"

typedef struct {
  float before_bias;
  float after_bias;
} Bias;

typedef struct {
  float b_A, b_B, b_C, b_D, b_E;
  float a_A, a_B, a_C, a_D, a_E;
} BiasDetail;

typedef struct {
  uint32_t neuron_id;
  float before_weight;
  float after_weight;
} Weight;

typedef struct {
  uint32_t neuron_id;
  float b_A, b_B, b_C, b_D, b_E, b_F;
  float a_A, a_B, a_C, a_D, a_E, a_F;
} WeightDetail;

class Calculator {
public:
  typedef struct {
    std::string input_file;
    std::string output_file;
    float A, B, C, D, E, F;
    int cut_bubble_size;
  } Parameter;

  Calculator(Parameter &_parameter);
  Calculator(){};
  ~Calculator(){};

  std::vector<BiasDetail> calcBiasDetail();
  std::vector<WeightDetail> calcWeightDetail(const uint32_t neuron_id);

  void writeInfo();
  void writeData();
  void writeDataDetail();

private:
  Parameter parameter;

  std::vector<std::pair<int,int>> board;
  std::vector<Piece> pieces;
  std::vector<Neuron> neurons;
};

#endif
