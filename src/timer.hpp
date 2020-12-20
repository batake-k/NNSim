#ifndef TIMER_HPP
#define TIMER_HPP

#include <string>
#include <iostream>

#ifdef _OPENMP

	#include <omp.h>

	class Timer {
	public:
		Timer() {restart();}
		~Timer() {}

		void restart(){
			start_time = omp_get_wtime();
		}

		void elapsed(const std::string& s, int hierarchy){
			double end_time = omp_get_wtime();

			if(hierarchy != 0){
				for(int i=0; i<hierarchy -1; ++i){
					std::cout << "  ";
				}
				std::cout << " -";
			}

			std::cout << "[" << s << "] " << end_time - start_time << " sec." << std::endl;
		}

	private:
		double start_time;
	};

#else

	#include <time.h>

	class Timer {
	public:
		Timer() {restart();}
		~Timer() {}

		void restart(){
			start_time = clock();
		}

		void elapsed(const std::string& s, int hierarchy){
			clock_t end_time = clock();

			if(hierarchy != 0){
				for(int i=0; i<hierarchy -1; ++i){
					std::cout << "  ";
				}
				std::cout << " -";
			}

			std::cout << "[" << s << "] " << (double)(end_time - start_time) / CLOCKS_PER_SEC << " sec." << std::endl;
		}

	private:
		clock_t start_time;
	};

#endif

#endif
