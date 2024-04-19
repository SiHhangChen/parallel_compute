#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

void matrixLU(float** matA, int sizeA){
    // #pragma omp parallel for schedule(static)
    for(int k = 0; k < sizeA; k++){
        #pragma omp parallel for schedule(static)
        for(int i = k + 1; i < sizeA; i++){
            matA[i][k] = matA[i][k]/matA[k][k];
            // #pragma omp parallel for schedule(static)
            // for(int j = k + 1; j < sizeA; j++){
            //     matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
            // }
        }
        #pragma omp parallel for schedule(static)
        for(int i = k + 1; i < sizeA; i++){
            #pragma omp parallel sections
            {
                #pragma omp section
                for(int j = k + 1; j < (sizeA + k + 1) / 2; j++){
                    matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
                }
                #pragma omp section
                for(int j = (sizeA + k + 1) / 2; j < sizeA; j++){
                    matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
                }
                // #pragma omp section 
                // for(int j = k + 1; j < (sizeA + k + 1) / 3; j++){
                //     matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
                // }
                // #pragma omp section
                // for(int j = (sizeA + k + 1) / 3; j < 2*(sizeA + k + 1) / 3; j++){
                //     matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
                // }
                // #pragma omp section
                // for(int j = 2*(sizeA + k + 1) / 3; j < sizeA; j++){
                //     matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
                // }
            }
        }
    }
}

int main(int* argc, char* argv[]){
    int threadNum = atoi(argv[1]);
    int sizeA;
    scanf("%d", &sizeA);
    float** matA = (float**)malloc(sizeof(float*)*sizeA);
    for(int i = 0; i < sizeA; i++)
        matA[i] = (float*)malloc(sizeof(float)*sizeA);

    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            scanf("%f", &matA[i][j]);
        }
    }
    double startTime = omp_get_wtime();
    srand(time(NULL)+rand());
    omp_set_num_threads(threadNum);
    matrixLU(matA, sizeA);
    double endTime = omp_get_wtime();
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            if(i > j) printf("%.2f ", matA[i][j]);
            else if(i == j) printf("1.00 ");
            else printf("0.00 ");
        }
        printf("\n");
    }
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            if(i <= j) printf("%.2f ", matA[i][j]);
            else printf("0.00 ");
        }
        printf("\n");
    }
    printf("Time: %lf\n", endTime-startTime);
    return 0;
}