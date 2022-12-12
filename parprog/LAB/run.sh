#!/bin/bash

set -e
file=time.txt
rm -f $file
make build
echo $'NP\tTime\tRan' > $file    # N - количество точек вдоль одной оси

for NP in $(seq 1 6); do  
for run in $(seq 1 1 8); do 
# for run in $(seq 1 8); do   # усредняем по нескольким точкам
    # echo "$run " >> $file
    OMP_NUM_THREADS=$NP ./main $run >> $file
done
done
# done
