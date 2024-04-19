#!/bin/bash
echo "BL_OMP"
for((i=10000; i<=10000000; i=i*10))
do
    for((j=1; j<=3; j++))
    do
        ./omp_buffon $[2*j] $i
    done
done