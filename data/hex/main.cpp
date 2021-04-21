/**
 * main関数を記述
 * @author Kazuki Takabatake
 * @date 2021/3/2
 */

#include "hex.hpp"
#include "piece.hpp"
#include "neuron.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

float B,C,D;

typedef struct {
	float before_bias;
	float after_bias;
} Bias;

typedef struct {
	uint32_t neuron_id;
	float before_weight;
	float after_weight;
} Weight;

std::vector<std::string> split(const std::string &str, const char delim){
		std::vector<std::string> elems;
	std::string item;
	for(char ch: str){
			if(ch == delim){
				elems.push_back(item);
			item.clear();
		}else{
				item += ch;
		}
	}

	elems.push_back(item);
	return elems;
}


void read(ifstream &ifs, vector<Hex> &board, vector<Piece> &pieces){
	string line;
	getline(ifs, line);
	vector<string> splited_line = split(line, ',');

	int num_pieces = stoi(splited_line[0]);
	int board_size = stoi(splited_line[1]);

	cout << "num pieces: " << num_pieces << endl;
	cout << "board size: " << board_size << endl;

	getline(ifs, line);

	for(int i=0; i<board_size; ++i){
		getline(ifs, line);
		vector<string> splited_line = split(line, ',');

		int point = stoi(splited_line[0]);
		int x = stoi(splited_line[1]);
		int y = stoi(splited_line[2]);

		if((x + y) % 2 != 0){
			cout << "wrong hex: " << x << ", " << y << endl;
			exit(0);
		}

		board.emplace_back(Hex(point, x, y));
	}

	if(board_size != board.size()){
		cout << "wrong board size" << endl;
		exit(0);
	}

	for(int i=0; i<num_pieces; ++i){
		getline(ifs, line);
		getline(ifs, line);
		int piece_size = stoi(line);

		vector<Hex> piece_hexs;
		getline(ifs, line);
		vector<string> rotation_flags = split(line, ',');
		getline(ifs, line);
		vector<string> inversion_flags = split(line, ',');

		for(int j=0; j<piece_size; ++j){
			getline(ifs, line);
			vector<string> splited_line = split(line, ',');
			piece_hexs.emplace_back(Hex(0, stoi(splited_line[0]), stoi(splited_line[1])));
		}

		Piece piece(piece_hexs, rotation_flags, inversion_flags, i);
		pieces.emplace_back(piece);
	}
}

void AddSurroundingHex(vector<Hex> &surrounding_hex, const vector<Hex> &board,
		const vector<Hex> &hexs, const int x, const int y){
	Hex h(0,x,y);
	auto iter = find(board.begin(), board.end(), h);

	if(find(surrounding_hex.begin(), surrounding_hex.end(), h) == surrounding_hex.end()
			&& find(hexs.begin(), hexs.end(), h) == hexs.end()
			&& iter != board.end()){
		h.setPoint(iter->getPoint());
		surrounding_hex.emplace_back(h);
	}
}

void AddSurroundingHex(vector<Hex> &surrounding_hex, const vector<Hex> &hexs, const int x, const int y){
	Hex h(0,x,y);

	if(find(surrounding_hex.begin(), surrounding_hex.end(), h) == surrounding_hex.end()
			&& find(hexs.begin(), hexs.end(), h) == hexs.end()){
		surrounding_hex.emplace_back(h);
	}
}

bool checkBubble(const vector<Hex> &board, const vector<Hex> &hexs, const int cut_bubble_size){
	vector<Hex> surrounding_hex;

	for(const auto& h : hexs){
		int x = h.getX();
		int y = h.getY();

		AddSurroundingHex(surrounding_hex, board, hexs, x + 1, y + 1);
		AddSurroundingHex(surrounding_hex, board, hexs, x + 2, y);
		AddSurroundingHex(surrounding_hex, board, hexs, x + 1, y - 1);
		AddSurroundingHex(surrounding_hex, board, hexs, x - 1, y - 1);
		AddSurroundingHex(surrounding_hex, board, hexs, x - 2, y);
		AddSurroundingHex(surrounding_hex, board, hexs, x - 1, y + 1);
	}

	for(uint32_t i=0; i<surrounding_hex.size(); ++i){
		vector<Hex> bubble_hex;
		bubble_hex.emplace_back(surrounding_hex[i]);

		for(uint32_t j=0; j<bubble_hex.size(); ++j){
			int x = bubble_hex[j].getX();
			int y = bubble_hex[j].getY();

			AddSurroundingHex(bubble_hex, board, hexs, x + 1, y + 1);
			AddSurroundingHex(bubble_hex, board, hexs, x + 2, y);
			AddSurroundingHex(bubble_hex, board, hexs, x + 1, y - 1);
			AddSurroundingHex(bubble_hex, board, hexs, x - 1, y - 1);
			AddSurroundingHex(bubble_hex, board, hexs, x - 2, y);
			AddSurroundingHex(bubble_hex, board, hexs, x - 1, y + 1);

			if(bubble_hex.size() > cut_bubble_size){
				break;
			}
		}

		// バブルは生成される
		if(bubble_hex.size() <= cut_bubble_size){
			int point = 0;
			for(const auto& h : bubble_hex){
				point += h.getPoint();
			}
			if(point > 0){
				return true;
			}
		}
	}

	return false;
}

Weight calcWeight(const vector<Hex> &board, const vector<Neuron> &neurons, const uint32_t i, const uint32_t j, const int cut_bubble_size){
	float before_weight = 0, after_weight = 0;

	// B
	int overlap_point = calcOverlapPoint(neurons[i], neurons[j]);
	before_weight += - B * overlap_point;
	after_weight += - B * overlap_point;

	// C
	vector<Hex> sum_hexs(neurons[i].getHexs());
	auto temp = neurons[j].getHexs();
	copy(temp.begin(), temp.end(), back_inserter(sum_hexs));

	if(checkBubble(board, sum_hexs, cut_bubble_size)){
		before_weight += -C;
		after_weight += -C;
	}

	// D
	before_weight += D * calcConnectEdge(neurons[i], neurons[j]);
	after_weight += D * calcConnectEdge(neurons[i], neurons[j]);

	Weight w = {j, before_weight, after_weight};

	return w;
}

int calcConnectWall(const vector<Hex> &hexs, const vector<Hex> &walls){
	int num = 0;

	for(const auto& h : hexs){
		if(h.getPoint() == 0){
			continue;
		}

		for(const auto& w : walls){
			if(h.isConnect(w)){
				++num;
			}
		}
	}

	return num;
}

int main(int argc, char *argv[]){

	if(argc != 8){
		cout << "./hex [input_file] [output_file] [cut_bubble_size] [B] [C] [D] [penalty]" << endl;
		exit(0);
	}

	string input_file = argv[1];
	string output_file = argv[2];

	int cut_bubble_size = stoi(argv[3]);

	B = stof(argv[4]);
	C = stof(argv[5]);
	D = stof(argv[6]);
	float penalty = stof(argv[7]);

	vector<Hex> board;
	vector<Piece> pieces;

	ifstream ifs(input_file);

	read(ifs, board, pieces);
	ifs.close();


	vector<Neuron> neurons;

	for(const auto& piece : pieces){
		auto states = piece.getStates();

		for(const auto& state : states){
			for(const auto& board_hex : board){

				vector<Hex> new_state(state);
				bool canBeNeuron = true;

				int point = 0;
				for(auto& h : new_state){
					h += board_hex;
					auto iter = find(board.begin(), board.end(), h);
					if(iter == board.end()){
						canBeNeuron = false;
						break;
					}else{
						int p = iter->getPoint();
						point += p;
						h.setPoint(p);
					}
				}

				if(point == 0){
					canBeNeuron = false;
				}

				if(canBeNeuron && checkBubble(board, new_state, cut_bubble_size)){
					canBeNeuron = false;
					
					/*cout << "has bubble with single piece" << endl;
					for(const auto& s : new_state){
						s.write();
					}*/
					
				}

				if(canBeNeuron){
					Neuron neuron(new_state, piece.getPieceId());
					neurons.emplace_back(neuron);
				}
			}
		}
	}

	cout << "generate neurons: " << neurons.size() << endl;

	// surrounding hex
	vector<Hex> sur_hexs;
	for(const auto& h : board){
		int x = h.getX();
		int y = h.getY();

		AddSurroundingHex(sur_hexs, board, x + 1, y + 1);
		AddSurroundingHex(sur_hexs, board, x + 2, y);
		AddSurroundingHex(sur_hexs, board, x + 1, y - 1);
		AddSurroundingHex(sur_hexs, board, x - 1, y + 1);
		AddSurroundingHex(sur_hexs, board, x - 2, y);
		AddSurroundingHex(sur_hexs, board, x - 1, y - 1);
	}

	// info
	ofstream ofs_info(output_file + "_info");
	ofs_info << pieces.size() << "," << neurons.size() << "," << board.size() << endl << endl;

	ofs_info << "-1" << endl;
	ofs_info << sur_hexs.size() << endl;
	for(const auto& h : sur_hexs){
		h.write(ofs_info);
	}
	ofs_info << endl;

	for(uint32_t i = 0; i<neurons.size(); ++i){
		neurons[i].write(ofs_info);
	}
	ofs_info.close();

	uint32_t neurons_size = neurons.size();

	// biases
	ofstream ofs_biases(output_file + "_biases", ios::binary);
	vector<Bias> biases;

	cout << "biases:" << endl;

	for(uint32_t i = 0; i<neurons_size; ++i){
		// B
		vector<Hex> hexs = neurons[i].getHexs();
		int pn = 0;

		for(const auto& h : hexs){
			pn += h.getPoint();
		}

		float b1 = 0.5 * B * pn;
		float b2 = 0.5 * B * pn;

		// D wall
		//int hi = calcConnectWall(hexs, sur_hexs);
		//b1 += D * hi;

		//b1 -= penalty / neurons[i].getSize();
		//b2 -= penalty / neurons[i].getSize();

		b1 -= penalty;
		b2 -= penalty;

		/*
		if(neurons[i].getSize() == 3){
			b1 = 0.1;
			b2 = 0.1;
		}else if(neurons[i].getSize() == 4){
			b1 = 0.2;
			b2 = 0.2;
		}else if(neurons[i].getSize() == 5){
			b1 = 0.4;
			b2 = 0.4;
		}else if(neurons[i].getSize() == 6){
			b1 = 0.8;
			b2 = 0.8;
		}else{
			b1 = 1.6;
			b2 = 1.6;
		}
		*/

		cout << neurons[i].getPieceId() << "\t" << b1 << "\t" << b2 << endl;

		Bias b = {b1, b2};
		biases.emplace_back(b);
	}

	ofs_biases.write((char*)&neurons_size, sizeof(uint32_t));
	ofs_biases.write((char*)&biases[0], sizeof(Bias) * neurons_size);
	ofs_biases.close();


	// weights
	ofstream ofs_weights(output_file + "_weights", ios::binary);
	ofs_weights.write((char*)&neurons_size, sizeof(uint32_t));

	for(uint32_t i=0; i<neurons_size; ++i){
			vector<Weight> Ws;

		for(uint32_t j=0; j<neurons_size; ++j){
				if(i == j){
					continue;
			}

			Weight w = calcWeight(board, neurons, i, j, cut_bubble_size);
			Ws.emplace_back(w);
		}

		uint32_t Ws_size = Ws.size();
		ofs_weights.write((char*)&Ws_size, sizeof(uint32_t));
		ofs_weights.write((char*)&Ws[0], sizeof(Weight) * Ws_size);
	}

	ofs_weights.close();

	return 0;
}
