#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <vector>
#include <string>

class Problem {
public:
  virtual bool isGoal(const std::vector<float> &outputs) = 0;
  virtual std::string getGoalStatus(const std::vector<float> &outputs) = 0;
};

#endif
