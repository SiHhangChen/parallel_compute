#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

# define M_PI  3.14159265358979323846

double Buffon_Laplace(int a, int b, int l, int N){
    int distance_a = 0, distance_b = 0, n = 0, theta = 0;
    int x = 0, y = 0, xDest = 0, yDest = 0;
    #pragma omp parallel for reduction(+:n) private(x, y, theta, xDest, yDest)
    for(int i = 0; i < N; i++){
        x = rand() % (a + 1);
        y = rand() % (b + 1);
        theta = rand() % (360 + 1);
        xDest = x + l * cos(theta * M_PI / 180);
        yDest = y + l * sin(theta * M_PI / 180);
        if(xDest <= 0 || xDest > a || yDest <= 0 || yDest > b){
            #pragma omp atomic
            n++;
        }
    }
    printf("n: %d\n", n);
    double p_outpi = (double)((2 * (a + b) - l) * l) / (a * b);
    return (double)N / n * p_outpi;
}

int main(int* argc, char* argv[]){
    int threadNum = atoi(argv[1]);
    int N = atoi(argv[2]);
    double start_time = omp_get_wtime();
    srand(time(NULL));
    double a = 1000, b = 1000, l = 618;
    omp_set_num_threads(threadNum);
    printf("The value of pi is: %lf\n", Buffon_Laplace(a, b, l, N/threadNum));
    double end_time = omp_get_wtime();
    double time = (end_time - start_time);
    printf("Time: %lf\n", time);
    FILE *fp;
    fp = fopen("BL_Openmp_result.txt", "a");
    fprintf(fp, "%d %d %lf\n", threadNum, N, time);
    fclose(fp);
    return 0;
}