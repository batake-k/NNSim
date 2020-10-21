MODE:=Release
#Release or Debug

CXX=g++
CXX_FLAGS=-Wall -std=c++11 -fopenmp

ifeq ($(MODE),Release)
	CXX_FLAGS += -O3
else ifeq ($(MODE),Debug)
	CXX_FLAGS += -g -DDEBUG -O3 #-O0 本来debugなら最適化レベルを下げる
endif

#C_SRC= \

CPP_SRC=./src/main.cpp \
  ./src/gaussian_model.cpp \
  ./src/hopfield_model.cpp \
  ./src/neuron.cpp \
  ./src/neural_network_model.cpp \
	./src/simulator.cpp \
  ./src/utils.cpp \

OBJS =
OBJS += $(C_SRC:%.c=%.o)
OBJS += $(CPP_SRC:./src/%.cpp=objs/%.o)

.PHONY: all
all:dir nnsim

nnsim: $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJS) -lboost_program_options
objs/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

dir:
	mkdir objs

.PHONY: clean
clean:
	rm -rf objs nnsim
