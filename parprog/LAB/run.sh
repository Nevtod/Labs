#!/bin/bash

set -e
file=res.txt
rm -f $file
make all
echo 'N NP Time' > $file    # N - количество точек вдоль одной оси
for N in 10 100 200 300 400 500 1000 2000 3000; do
for NP in $(seq 1 16); do   
for run in $(seq 1 8); do   # усредняем по нескольким точкам
    # echo "$run " >> $file
    mpirun -n $NP ./main $N $run >> $file
done
done
done
