import sys
import random

# input
args = sys.argv

input_file_name = args[1]
output_file_name = args[2]
delete_num = int(args[3])

# 0: random, 1: back
mode = int(args[4])

# read input file
input_file = open(input_file_name)

## board
board_size = int(input_file.readline())
board = []
for i in range(board_size):
	board.append(input_file.readline())

input_file.readline()

## piece
piece_num = int(input_file.readline())
pieces = []

for i in range(piece_num):
	size = int(input_file.readline())
	piece = []

	for j in range(size):
		piece.append(input_file.readline())

	pieces.append(piece)
	input_file.readline()

input_file.close()

# delete
if mode == 0:
	for i in range(delete_num):
		del_index = random.randrange(len(pieces))
		print("del " + str(len(pieces[del_index])))
		pieces.pop(del_index)

elif mode == 1:
	for i in range(delete_num):
		del_index = len(pieces)-1
		print("del " + str(len(pieces[del_index])))
		pieces.pop(del_index)

# output
output_file = open(output_file_name, mode = 'w')

## board
output_file.write(str(board_size) + "\n")
for b in board:
	output_file.write(b)
output_file.write("\n")

## piece
output_file.write(str(len(pieces)) + "\n")
for piece in pieces:
	output_file.write(str(len(piece)) + "\n")

	for p in piece:
		output_file.write(p)

	output_file.write("\n")

output_file.close()
