# generate board
./polyomino generate \
	-b 50 \
	-p 3,3,4,4,5,5 \
	-o test_problem

# calculate network_data for NNSim
./polyomino calculate \
	-i test_problem \
	-o test_problem \
	-p 3 \
	-a 1 \
	-b 1 \
	-c 1 \
	-d 1 \
	-e 1 \
	-f 1 \
	-g 1 \
	-s 2 \
	-x 1 \
	-y 0 \
	-z 1
