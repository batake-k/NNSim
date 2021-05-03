for penalty in 1
do
	#./hex_penalty puzzle/M10P1 M10P1_B1C1penalty${penalty} 2 1 1 0 ${penalty}
	qsub -g tga-pharma -N M10P2_same_penalty${penalty} -v penalty=${penalty} jobscript.sh
done
