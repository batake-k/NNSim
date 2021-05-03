for penalty in 1 2
do
	ident=M10P2_B1C1_same_penalty${penalty}

	data=data/hex/${ident}_data
	output=results_${ident}

	mkdir ${output}

	qsub -g tga-pharma -N ${ident} -v data=${data} -v output=${output} jobscript.sh
done
