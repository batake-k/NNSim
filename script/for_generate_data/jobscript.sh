#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=1:00:00

./hex -i puzzle/M10P1 \
      -o M10P1_B1C1_same_penalty${penalty} \
      -b 1 \
      -c 1 \
      -e ${penalty} \
      -s 2 \
      -x true \
      -y true \
      -z false
