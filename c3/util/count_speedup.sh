#!/bin/zsh

declare -a arr_size=("100000" "1000000" "10000000")
echo "" > results
for i in "${arr_size[@]}"
do
	echo "############## N = $i ###############"
	for j in {2..8}
	do
		gcc -fopenmp -DN=$i -DNUM_OF_THREADS=$j  l_p_parallel.c &&
		A=$(./a.out | awk '{print $5}') &&
		echo "A is $A"
		gcc -DN=$i l_p_seq.c &&
		B=$(./a.out | awk '{print $5}') &&
		echo "B is $B" &&
		S=$((B/A)) &&
	  echo "speedup = $S" &&
	  echo "size = $i num of threads = $j, speedup is $S" >> results &&
	  rm a.out
	done
done
