#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

# define M_PI  3.14159265358979323846
# define N 10000000

double Buffon_Laplace(int a, int b, int l){
    int distance_a = 0, distance_b = 0, n = 0, theta = 0;
    int x = 0, y = 0, xDest = 0, yDest = 0;
    for(int i = 0; i < N; i++){
        x = rand() % (a + 1);
        y = rand() % (b + 1);
        theta = rand() % (360 + 1);
        xDest = x + l * cos(theta * M_PI / 180);
        yDest = y + l * sin(theta * M_PI / 180);
        if(xDest <= 0 || xDest > a || yDest <= 0 || yDest > b){
            n++;
        }
    }
    printf("n: %d\n", n);
    double p_outpi = (double)((2 * (a + b) - l) * l) / (a * b);
    return (double)N / n * p_outpi;
}

int main(){
    double start_time = clock();
    srand(time(NULL));
    double a = 1000, b = 1000, l = 618;
    printf("The value of pi is: %lf\n", Buffon_Laplace(a, b, l));
    double end_time = clock();
    double time = (end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time: %lf\n", time);
    return 0;
}