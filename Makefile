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

#C_SRC= \

CPP_SRC=./src/main.cpp \
  ./src/gaussian_model.cpp \
  ./src/neural_network_model.cpp \
	./src/simulator.cpp \
  ./src/utils.cpp \
  ./src/for_problems/polyomino.cpp \

OBJS =
OBJS += $(C_SRC:%.c=%.o)
OBJS += $(CPP_SRC:./src/%.cpp=objs/%.o)

.PHONY: all
all:dir $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJS) -lboost_program_options
objs/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

dir:
	mkdir objs objs/for_problems

.PHONY: clean
clean:
	rm -rf objs $(PROGRAM)
