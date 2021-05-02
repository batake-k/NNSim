for D in 1
do
	qsub -g tga-pharma -N puzzle2_A1B0.2C1D${D} -v D=${D} jobscript.sh
done
