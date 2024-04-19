#include <stdio.h>
#include "mpi.h"
int main(){
    float data[1024], buff[10];
    for(int i = 0; i < 10; i++)
        buff[i] = data[i*32];
    MPI_Send(buff, 10, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
}
