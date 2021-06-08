/**
 * 結合重み・バイアス値を計算するクラス
 * @author Kazuki Takabatake
 * @date 2021/5/3
 */

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP

#include <string>
#include <vector>

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
    std::string output_file;
    int N;
    float A, B, C, D, E, F;
  } Parameter;

  Calculator(Parameter &_parameter) : parameter(_parameter){};
  Calculator(){};
  ~Calculator(){};

  std::vector<BiasDetail> calcBiasDetail();
  std::vector<WeightDetail> calcWeightDetail(const int x, const int y);

  void writeData();
  void writeDataDetail();

private:
  Parameter parameter;
};

#endif
