#!/bin/bash
#$ -cwd
#$ -l s_core=1
#$ -l h_rt=2:00:00

./hex_same_penalty_D puzzle/M10P2 M10P2_B1C1same_penalty1.4D${D} 2 1 1 ${D} 1.4
