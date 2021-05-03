#!/bin/bash
#$ -cwd
#$ -l f_node=1
#$ -l h_rt=3:00:00

trials=500

proc () {

	for Te in 0 0.01 0.02 0.03 0.04 0.05 0.06 0.07 0.08 0.09 0.1
	do

		for i in `seq 1 ${trials}`
		do
			./nnsim -i ${data} \
							-o ${output}/Tmf${1}/Te${Te} \
							-m ${1} \
							-e ${Te} \
							-g 300 \
							-r ${i} > /dev/null
		done

	done

	echo finish Tmf ${1}
}

for Tmf in 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27
do
		mkdir ${output}/Tmf${Tmf}
		proc ${Tmf} &
done

wait
