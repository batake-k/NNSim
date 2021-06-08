#include "nqueen.hpp"

#include <iostream>

using namespace std;

Nqueen::Nqueen(ifstream &ifs) {
  ifs.read((char *)&N, sizeof(int));
  ifs.close();
}

bool Nqueen::isGoal(const std::vector<float> &outputs) {
  int num = 0;

  for (uint32_t i = 0; i < outputs.size(); ++i) {
    if (outputs[i] >= 0.5) ++num;
  }
  if (num != N) return false;

  // A, B
  for (int i = 0; i < N; ++i) {
    int a = 0, b = 0;

    for (int j = 0; j < N; ++j) {
      if (outputs[i * N + j] >= 0.5) ++a;
      if (outputs[j * N + i] >= 0.5) ++b;
    }

    if (a != 1 || b != 1) return false;
  }

  // C
  for (int i = 0; i < 2 * N - 3; ++i) {
    int s = i + 1;
    int c = 0;

    for (int j = 0; j < N; ++j) {
      if (s - j >= 0 && s - j < N && outputs[j * N + s - j] >= 0.5) ++c;
    }

    if (c > 1) return false;
  }

  // D
  for (int i = 0; i < N - 1; ++i) {
    int d1 = 0, d2 = 0;

    for (int j = 0; j < N; ++j) {
      if (i + j >= N) break;
      if (outputs[(i + j) * N + j] >= 0.5) ++d1;
      if (outputs[j * N + i + j] >= 0.5) ++d2;
    }

    if (d1 > 1 || d2 > 1) return false;
  }

  return true;
}

string Nqueen::getGoalStatus(const vector<float> &outputs) {

  if (isGoal(outputs)) {
    return ",1";
  } else {
    return ",0";
  }
}
