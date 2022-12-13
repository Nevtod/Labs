#!/bin/bash

set -e

file=time.txt
rm -f $file
mpicc  main.c -o main -lm
# echo $'NP\tTime\tRan' > $file

# for NP in $(seq 1 50); do  
# for run in $(seq 1 1 4); do 
    # mpirun -n $NP ./main $run >> $file
mpirun -n 10 ./main $run >> $file
done
done
