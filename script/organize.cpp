#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace {
std::vector<std::string> split(const std::string& str, const char delim) {
  std::vector<std::string> elems;
  std::string item;
  for (char ch : str) {
    if (ch == delim) {
      elems.push_back(item);
      item.clear();
    } else {
      item += ch;
    }
  }

  elems.push_back(item);
  return elems;
}
};  // namespace

class Goal {
public:
  float energy;
  string Tmf;
  string Te;
  int seed;
  int times;

  Goal(float _energy, string _Tmf, string _Te, int _seed) {
    energy = _energy;
    Tmf = _Tmf;
    Te = _Te;
    seed = _seed;
    times = 1;
  }

  bool operator==(const Goal& other) const { return energy == other.energy; }

  bool operator<(const Goal& other) const { return energy < other.energy; }
};

int main(int argc, char* argv[]) {
  if (argc != 7) {
    cout << "./a.out [directory] [output] [one] [one list] [two] [two list]";
    exit(0);
  }

  string dir = argv[1];
  string out = argv[2];

  string one = argv[3];
  vector<string> one_list = split(argv[4], ',');

  string two = argv[5];
  vector<string> two_list = split(argv[6], ',');

  ofstream ofs_ave("ave_" + out);
  ofstream ofs_goal("goal_" + out);
  ofstream ofs_list("list_" + out);

  std::map<int, int> num_pieces;
  std::vector<Goal> goals;

  for (const auto& one_e : one_list) {
    for (const auto& two_e : two_list) {
      string input_file = dir + one + one_e + two + two_e;
      ifstream ifs(input_file);
      if (!ifs) {
        cerr << "[ERROR] open file error: " << input_file << endl;
        exit(1);
      }

      int goal_count = 0;
      int trails = 0;
      vector<float> energies;

      string line;

      while (getline(ifs, line)) {
        ++trails;
        vector<string> splited_line = split(line, ',');

        float energy = stof(splited_line[0]);
        energies.emplace_back(energy);

        if (splited_line[1] == "1") {
          ++goal_count;

          Goal goal = Goal(energy, one_e, two_e, trails);

          auto itr = find(goals.begin(), goals.end(), goal);
          if (itr != goals.end()) {
            (*itr).times += 1;
          } else {
            goals.emplace_back(goal);
          }

          if (splited_line.size() > 2) {
            int num = stoi(splited_line[2]);
            if (num_pieces.find(num) != num_pieces.end()) {
              num_pieces[num]++;
            } else {
              num_pieces.insert(std::make_pair(num, 1));
            }
          }
        }
      }
      ifs.close();

      float sum = 0;
      for (int i = 0; i < trails; ++i) {
        sum += energies[i];
      }

      ofs_ave << sum / trails;
      ofs_goal << goal_count;

      if (two_e != two_list[two_list.size() - 1]) {
        ofs_ave << ",";
        ofs_goal << ",";
      }
    }

    ofs_ave << endl;
    ofs_goal << endl;
  }

  sort(goals.begin(), goals.end());

  for (const auto& g : goals) {
    ofs_list << g.energy << endl;
    ofs_list << g.times << endl;
    ofs_list << "Tmf: " << g.Tmf << ", Te: " << g.Te << ", seed: " << g.seed << endl << endl;
  }

  if (!num_pieces.empty()) {
    for (const auto& n : num_pieces) {
      ofs_list << n.first << " " << n.second << endl;
    }
  }

  ofs_ave.close();
  ofs_goal.close();
  ofs_list.close();

  return 0;
}
