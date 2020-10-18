#include <iostream>
#include <fstream>

using namespace std;

int N, A, B, C, D;

int KDelta(int a, int b){
  if (a == b){
    return 1;
  }else{
    return 0;
  }
}

int Weight(int x, int y, int X, int Y){
  int w = - A * KDelta(x, X) * (1 - KDelta(y, Y));
  w += - B * KDelta(y, Y) * (1 - KDelta(x, X));
  w += - C * KDelta(x + y, X + Y) * (1 - KDelta(x, X));
  w += - D * KDelta(x - y, X - Y) * (1 - KDelta(x, X));

  return w;
}

int Bias(){
  return (A + B) / 2;
}

int main(int argc, char *argv[]){
  if(argc != 6){
    cout << "./nqueen [n] [A] [B] [C] [D]" << endl;
    return 0;
  }

  N = stoi(argv[1]);
  A = stoi(argv[2]);
  B = stoi(argv[3]);
  C = stoi(argv[4]);
  D = stoi(argv[5]);

  cout << "N: " << N << endl
       << "A: " << A << endl
       << "B: " << B << endl
       << "C: " << C << endl
       << "D: " << D << endl;

  string wfile = "nqueen_weights.N" + to_string(N) + ".A" + to_string(A) + ".B" + to_string(B) + ".C" + to_string(C) + ".D" + to_string(D);
  string bfile = "nqueen_bias.N" + to_string(N) + ".A" + to_string(A) + ".B" + to_string(B) + ".C" + to_string(C) + ".D" + to_string(D);
  ofstream wofs(wfile);
  ofstream bofs(bfile);

  for(int x=0; x<N; ++x){
    for(int y=0; y<N; ++y){
      bofs << Bias() << endl;
    }
  }
  bofs.close();

  int count = 0;

  for(int x=0; x<N; ++x){
    for(int y=0; y<N; ++y){
      for(int X=0; X<N; ++X){
        for(int Y=0; Y<N; ++Y){
          int weight = Weight(x, y, X, Y);
          if(weight != 0){
            count++;
            wofs << N * x + y << "," << N * X + Y << "," << Weight(x, y, X, Y) << endl;
          }
        }
      }
    }
  }
  wofs.close();

  cout << "weight: " << count << " / " << N * N * N * N << endl;

  return 0;
}
