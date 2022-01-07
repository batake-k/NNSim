#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=24:00:00

delete_num=1

proc() {
	./../../../NNSim/nnsim \
		-i temp_data \
		-o result_$2 \
		-g $2 \
		-r $1 \
		-m 1 > /dev/null
}

module load python/3.6.5

for i in `seq 0 999`
do
	sh del.sh $delete_num

	for g in 50 100 200 300 400 500
	do
		proc $i $g
	done
done
