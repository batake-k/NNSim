#!/bin/bash
#$ -cwd
#$ -l f_node=1
#$ -l h_rt=1:00:00

module load python/3.6.5

pip install --user optuna

python3 bayesian.py optimize sqlite:///db result.csv
