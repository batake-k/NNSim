#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <cerrno>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

namespace utils{
	//void WriteString(std::ofstream &ofs, const std::string &s);
	//std::string LoadString(std::ifstream &ifs);

	double getMaxMemoryConsumption();
	std::vector<std::string> split(const std::string &str, const char delim);

	template<typename T>
	void fileOpen(T& fs, const std::string& filepath, const std::ios_base::openmode mode){
		fs.open(filepath, mode);
		if(fs.fail()){
			std::cerr << "[ERROR] file open " << filepath << ": " << std::strerror(errno) << std::endl;
			exit(1);
		}
	};
} 

#endif
