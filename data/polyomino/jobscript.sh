#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

D=1

./polyomino -i puzzle/puzzle1 \
            -o puzzle1_A1B0.2C1D${D} \
            -a 1 \
            -b 0.2 \
            -c 1 \
            -d ${D} \
            -s 4 \
            -x true \
            -y false \
            -z false
