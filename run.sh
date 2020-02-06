#!/bin/bash

echo "--TSP Benchmark--"

make

k=1
for instance in instances/*; do
	echo $instance >> ./benchmark/bm.txt

	echo "Running $instance"
	echo "Instance $k of 22" 

	for i in {1..10}; do
		./mlp ${instance} | grep 'Custo\|Tempo' | awk "{print $1}" >> ./benchmark/bm.txt
	done

	k=$(($k + 1))
done

echo "-" >> ./benchmark/bm.txt
