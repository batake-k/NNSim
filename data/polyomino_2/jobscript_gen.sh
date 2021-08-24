#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

p="3,3"
p=$p+",4,4,4,4,4"

for i in `seq 1 12`
do
	p=$p+",5"
done

for i in `seq 1 35`
do
	p=$p+",6"
done

#for i in `seq 1 54`
#do
#	p=$p+",7"
#done

./polyomino generate -b 60 -p $p -o puzzle/M3-6N60_2
