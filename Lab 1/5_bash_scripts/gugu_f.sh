#!/bin/bash

for dan in {2..9}
do
	echo "-------$dan단-------"
	for i in {1..9}
	do
		echo "$dan X $i = $((dan *i))"
	done
done


dan=2
while [ $dan -le 9]
do
	echo "-------$dan 단-------"
	i=1
	while [ $i -le 9]
	do
		ehco "$dam X $i = $((dan*i))
		i=$((i+1))
	done
	dan=$((dan+1))
done
