#!/bin/bash
#$ -cwd
#$ -l f_node=1
#$ -l h_rt=1:30:00

gen=500

proc(){
	for i in `seq 0 19`
	do
		rand=$(( 50 * $i + $1 ))

		./../../../NNSim/nnsim \
			-i data \
			-o output_$gen \
			-m 1 \
			-e 0 \
			-g $gen \
			-r $rand > /dev/null
	done
}

for i in `seq 1 50`
do
	proc $i &
done
wait
