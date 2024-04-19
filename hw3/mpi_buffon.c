#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
#include<time.h>
#include<math.h>

# define M_PI  3.14159265358979323846


double mpi_buffon(int a, int b, int l, int N, int rank, int size){
    int distance_a = 0, distance_b = 0, n = 0, theta = 0;
    int x = 0, y = 0, xDest = 0, yDest = 0;
    for(int i = rank; i < N; i += size){
        x = rand() % (a + 1);
        y = rand() % (b + 1);
        theta = rand() % (360 + 1);
        xDest = x + l * cos(theta * M_PI / 180);
        yDest = y + l * sin(theta * M_PI / 180);
        if(xDest <= 0 || xDest > a || yDest <= 0 || yDest > b){
            n++;
        }
    }
    int n_total = 0;
    // printf("rank: %d; n: %d; N: %d\n", rank, n, N);
    MPI_Reduce(&n, &n_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    double p_outpi = (double)((2 * (a + b) - l) * l) / (a * b);
    return (double)N / n_total * p_outpi;
}

int main(int argc, char* argv[]){
    int rank, size;
    int N = atoi(argv[1]);
    MPI_Init(&argc, &argv);
    // printf("N: %d\n", N);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    double start_time = MPI_Wtime();
    srand(time(NULL));
    double a = 1000, b = 1000, l = 618;
    double pi = mpi_buffon(a, b, l, N, rank, size);
    MPI_Barrier(MPI_COMM_WORLD);
    if(rank == 0){
        printf("The value of pi is: %lf\n", pi);
    }
    double end_time = MPI_Wtime();
    double time = (end_time - start_time);
    if(rank == 0){
        printf("Time: %lf\n", time);
        FILE *fp;
        fp = fopen("BL_MPI_result.txt", "a");
        fprintf(fp, "%d %d %lf\n", size, N, time);
        fclose(fp);
    }
    MPI_Finalize();
    return 0;
}