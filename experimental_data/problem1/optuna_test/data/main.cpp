#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

struct BiasDetail {
	float b_A, b_B, b_E, b_G;
	float a_A, a_B, a_E, a_G;
};

struct Bias {
	float b;
	float a;
};

struct WeightDetail {
	uint32_t neuron_id;
	float b_A, b_B, b_C, b_D, b_F;
	float a_A, a_B, a_C, a_D, a_F;
};

struct Weight {
	uint32_t neuron_id;
	float b;
	float a;
};


int main(int argc, char *argv[]){

	if(argc != 8){
		cerr << "./a.out [A] [B] [C] [D] [E] [F] [G]" << endl;
		exit(1);
	}

	float A = stof(argv[1]);
	float B = stof(argv[2]);
	float C = stof(argv[3]);
	float D = stof(argv[4]);
	float E = stof(argv[5]);
	float F = stof(argv[6]);
	float G = stof(argv[7]);

	ofstream ofs("data", ios::binary);

	{ // type 0:nqueen 1:polyomino 2:hex 3:polycube 4:polyomino2
		int type = 4;
		ofs.write((char*)&type, sizeof(int));
	}

	ifstream detail_ifs("data_detail", ios::binary);

	{ // bias
		uint32_t neurons_size;
		detail_ifs.read((char*)&neurons_size, sizeof(uint32_t));

		vector<BiasDetail> v_bias_detail(neurons_size);
		vector<Bias> v_bias(neurons_size);

		detail_ifs.read((char*)&v_bias_detail[0], sizeof(BiasDetail)*neurons_size);

		for(uint32_t i=0; i<neurons_size; ++i){
			v_bias[i].b = A * v_bias_detail[i].b_A + 
										B * v_bias_detail[i].b_B + 
										E * v_bias_detail[i].b_E + 
										G * v_bias_detail[i].b_G;

			v_bias[i].a = A * v_bias_detail[i].a_A + 
										B * v_bias_detail[i].a_B + 
										E * v_bias_detail[i].a_E + 
										G * v_bias_detail[i].a_G;
		}

		ofs.write((char*)&neurons_size, sizeof(uint32_t));
		ofs.write((char*)&v_bias[0], sizeof(Bias)*neurons_size);
	}

	{ // weight
		uint32_t neurons_size;
		detail_ifs.read((char*)&neurons_size, sizeof(uint32_t));

		vector<vector<WeightDetail>> v_weight_detail(neurons_size);
		vector<vector<Weight>> v_weight(neurons_size);

		for(uint32_t i=0; i<neurons_size; ++i){
			uint32_t size;
			detail_ifs.read((char*)&size, sizeof(uint32_t));
			v_weight_detail[i].resize(size);
			v_weight[i].resize(size);

			detail_ifs.read((char*)&v_weight_detail[i][0], sizeof(WeightDetail)*size);

			for(uint32_t j=0; j<size; ++j){
				v_weight[i][j].neuron_id = v_weight_detail[i][j].neuron_id;
				v_weight[i][j].b = A * v_weight_detail[i][j].b_A + 
													 B * v_weight_detail[i][j].b_B + 
													 C * v_weight_detail[i][j].b_C + 
													 D * v_weight_detail[i][j].b_D + 
													 F * v_weight_detail[i][j].b_F;

				v_weight[i][j].a = A * v_weight_detail[i][j].a_A + 
													 B * v_weight_detail[i][j].a_B + 
													 C * v_weight_detail[i][j].a_C + 
													 D * v_weight_detail[i][j].a_D + 
													 F * v_weight_detail[i][j].a_F;
			}
		}

		ofs.write((char*)&neurons_size, sizeof(uint32_t));

		for(uint32_t i=0; i<neurons_size; ++i){
			uint32_t size = v_weight[i].size();
			ofs.write((char*)&size, sizeof(uint32_t));
			ofs.write((char*)&v_weight[i][0], sizeof(Weight)*size);
		}
	}

	detail_ifs.close();

	{ // neuron info
		ifstream info_ifs("neuron_info", ios::binary);

		ofs << info_ifs.rdbuf();
		info_ifs.close();
	}

	ofs.close();

	return 0;
}
