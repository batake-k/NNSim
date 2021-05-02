#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

D=1

./polyomino -i puzzle/puzzle2 \
            -o puzzle2_A1B0.2C1D${D} \
            -a 1 \
            -b 0.2 \
            -c 1 \
            -d ${D} \
            -s 1 \
            -x true \
            -y true \
            -z true
