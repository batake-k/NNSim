CXX=g++
CXX_FLAGS=-Wall -std=c++11 -O3 -fopenmp

#C_SRC= \

CPP_SRC=./src/calculator.cpp \
  ./src/main.cpp \
	./src/neuron.cpp \
  ./src/piece.cpp \

OBJS =
OBJS += $(C_SRC:%.c=%.o)
OBJS += $(CPP_SRC:./src/%.cpp=objs/%.o)

.PHONY: all
all:dir polyomino

polyomino: $(OBJS)
	$(CXX) $(CXX_FLAGS) -o $@ $(OBJS) -lboost_program_options
objs/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

dir:
	mkdir objs

.PHONY: clean
clean:
	rm -rf objs polyomino
