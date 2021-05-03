#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

proc () {
	ident=M10P2_B1C1_same_penalty${1}

	Tmf="0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27"
	Te="0,0.01,0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.09,0.1"

	./organize results_${ident}/ ${ident}.csv Tmf "${Tmf}" /Te "${Te}"
}

for penalty in 1 2
do
	proc ${penalty}
done
