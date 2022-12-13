#!/bin/bash

set -e

file=time.txt
rm -f $file main
mpicc  main.c -o main -lm
echo $'NP\tTime\tRan' > $file
for NP in $(seq 1 6); do  
for run in $(seq 1 1 4); do 
    mpirun -n $NP ./main $run >> $file
# mpirun -n 6 ./main
done
done

python3 plot.py
