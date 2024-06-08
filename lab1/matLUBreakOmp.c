#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

void matrixLU(int** matA, int sizeA){
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
            #pragma omp parallel for schedule(static)
            for(int j = k + 1; j < sizeA; j++){
                matA[i][j] = matA[i][j] - matA[i][k]*matA[k][j];
            }
        }
    }
}

int main(int* argc, char* argv[]){
    int threadNum = 8;
    int sizeA;
    scanf("%d", &sizeA);
    int** matA = (int**)malloc(sizeof(int*)*sizeA);
    for(int i = 0; i < sizeA; i++)
        matA[i] = (int*)malloc(sizeof(int)*sizeA);

    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            scanf("%d", &matA[i][j]);
        }
    }
    // double startTime = omp_get_wtime();
    // srand(time(NULL)+rand());
    omp_set_num_threads(threadNum);
    matrixLU(matA, sizeA);
    // double endTime = omp_get_wtime();
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            if(i > j) printf("%d ", matA[i][j]);
            else if(i == j) printf("1 ");
            else printf("0 ");
        }
        printf("\n");
    }
    for(int i = 0; i < sizeA; i++){
        for(int j = 0; j < sizeA; j++){
            if(i <= j) printf("%d ", matA[i][j]);
            else printf("0 ");
        }
        printf("\n");
    }
    //printf("Time: %lf\n", endTime-startTime);
    return 0;
}