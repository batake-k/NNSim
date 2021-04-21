#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=2:00:00
#$ -N M10P1_penalty

for D in 0.1
do
	#./hex_penalty puzzle/M10P1 M10P1_B1C1penalty${penalty} 2 1 1 0 ${penalty}
	qsub -g tga-pharma -N M10P2_same_penalty1.4_D${D} -v D=${D} jobscript.sh
done
