#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

./hex generate -b 100 -p 4,4,4,4,5,5,5,5,6,6,6,6,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10 -o puzzle/M28P1
