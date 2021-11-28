import sys

num_threads = int(sys.argv[1])

num = 0
sum_score = 0
sum_piece_score = 0
num_solved = 0

for i in range(num_threads):
	file_name = "output_" + str(1+i)
	
	file = open(file_name)
	for line in file:
		num += 1
		sp = line.split(',')

		if sp[2] == "0.0" and sp[3] == "0.0":
			num_solved += 1

		sum_score += float(sp[2])
		sum_piece_score += float(sp[3])

ave_score = sum_score / num
ave_piece_score = sum_piece_score / num

print("num_solved: " + str(num_solved) + ", ave score: " + str(ave_score) + ", ave piece score: " + str(ave_piece_score))

ave_score += ave_piece_score

ofs = open("score", 'w')

if num_solved > 0:
	ofs.write(str(-num_solved))
else:
	ofs.write(str(-ave_score))
