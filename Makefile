MODE:=Experiment
#Experiment or GUI

PROGRAM=

CXX=g++
CXX_FLAGS=-Wall -std=c++11 -O3 -fopenmp

ifeq ($(MODE),Experiment)
	CXX_FLAGS += -DEXP
	PROGRAM=nnsim
else ifeq ($(MODE),GUI)
	CXX_FLAGS += -DGUI
	PROGRAM=nnsim-gui
endif

CPP_SRC = ./src/gaussian_model.cpp \
  ./src/neural_network_model.cpp \
	./src/simulator.cpp \
  ./src/utils.cpp \
  ./src/for_problems/polyomino.cpp \

OBJS =
OBJS += $(CPP_SRC:./src/%.cpp=objs/%.o)

TEST_SRC=./src/test_neural_network_model.cpp 

TEST_OBJS = $(TEST_SRC:./src/%.cpp=objs/%.o)
TEST_OBJS += $(OBJS)

.PHONY: all
all:dir $(PROGRAM) unittest

$(PROGRAM): objs/main.o $(OBJS) 
	$(CXX) $(CXX_FLAGS) -o $@ objs/main.o $(OBJS) -lboost_program_options

objs/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

unittest: objs/test_main.o $(TEST_OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ objs/test_main.o $(TEST_OBJS) -lboost_unit_test_framework -lboost_program_options

dir:
	mkdir -p objs objs/for_problems

.PHONY: clean
clean:
	rm -rf objs $(PROGRAM) unittest
