#!/bin/bash
echo "BL_MPI"
for((i=10000; i<=10000000; i=i*10))
do
    for((j=1; j<=3; j++))
    do
        mpirun -np $[2*j] mpi_buffon $i
    done
done