#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

./polyomino calculate -i puzzle/M3-6N60 -o M3-6N60 -a 1 -b 1 -c 1 -d 1 -e 1 -f 1 -s 2 -x 1 -y 1 -z 1
