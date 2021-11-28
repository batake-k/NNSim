dir=$(cd $(dirname $0); pwd)
cd $dir

num_threads=56

proc(){
	for i in 0
	do
		rand=$(( ${num_threads} * $i + $1 ))

		./../../../NNSim/nnsim \
			-i ../data/data \
			-o output_$1 \
			-m $2 \
			-e $3 \
			-g 300 \
			-r $rand > /dev/null
	done
}

for i in `seq 1 ${num_threads}`
do
	proc $i $1 $2 &
done
wait

python3 organize.py $num_threads

for i in `seq 1 ${num_threads}`
do
	rm output_${i}
done
