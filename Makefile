MODE:=Experiment
#Experiment or GUI

CXX=g++
CXX_FLAGS=-Wall -std=c++11 -O3 -fopenmp

ifeq ($(MODE),Experiment)
	CXX_FLAGS += -DEXP
else ifeq ($(MODE),GUI)
	CXX_FLAGS += -DGUI
endif

#C_SRC= \

CPP_SRC=./src/main.cpp \
  ./src/gaussian_model.cpp \
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
