#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int N, A, B, C, D;

typedef struct{
	int neuron_id;
	float weight;
} W;

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
  ofstream wofs(wfile, ios::binary);
  ofstream bofs(bfile, ios::binary);

	uint32_t size = N * N;

	vector<float> v_bias;
  for(int x=0; x<N; ++x){
    for(int y=0; y<N; ++y){
      v_bias.emplace_back((float)Bias());
    }
  }
	bofs.write((char*)&size, sizeof(uint32_t));
	bofs.write((char*)&v_bias[0], sizeof(float)*size);
  bofs.close();


	wofs.write((char*)&size, sizeof(uint32_t));
  for(int x=0; x<N; ++x){
    for(int y=0; y<N; ++y){

			vector<W> v_W;

      for(int X=0; X<N; ++X){
        for(int Y=0; Y<N; ++Y){
          float weight = (float)Weight(x, y, X, Y);
          if(weight != 0){
						W w = {N*X+Y, weight};
						v_W.emplace_back(w);
          }
        }
      }

			uint32_t v_W_size = v_W.size();
			wofs.write((char*)&v_W_size, sizeof(uint32_t));
			wofs.write((char*)&v_W[0], sizeof(W)*v_W_size);

    }
  }
  wofs.close();

  return 0;
}
