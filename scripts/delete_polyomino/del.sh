rm temp
rm temp_data

python3 delete_piece.py default temp $1 0

./../../../NNSim/polyomino/polyomino calculate \
	-i temp \
	-o temp \
	-p 3 \
	-b 3.38 \
	-c 5.70 \
	-d 0.615 \
	-e 0.518 \
	-f 3.91 \
	-s 2 \
	-x 1
