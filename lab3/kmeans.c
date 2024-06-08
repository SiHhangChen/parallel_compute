#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <omp.h>

#define INF FLT_MAX

float distance(const float* a, const float* b, int demension) {
    float distance = 0.0;
    for (int i = 0; i < demension; ++i) {
        distance += (a[i] - b[i]) * (a[i] - b[i]);
    }
    return sqrtf(distance);
}

void iter(int rank, int size, int N, int M, int K, float* data, float* centers, int* cBelong, float* vecSum) {
    for (int i = 0; i < K; i++) {
        cBelong[i] = 0;
        for (int j = 0; j < M; j++) {
            vecSum[i * M + j] = 0.0;
        }
    }
    int belong;
    float minDist, dist;
    for (int i = rank; i < N; i += size) {
        minDist = INF;
        belong = 0;
        for (int j = 0; j < K; j++) {
            dist = distance(data + i * M, centers + j * M, M);
            if (dist < minDist) {
                minDist = dist;
                belong = j;
            }
        }
        cBelong[belong]++;
        for (int j = 0; j < M; j++) {
            vecSum[belong * M + j] += data[i * M + j];
        }
    }
}



int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //-------------GET DATA----------------
    int N, M, K;
    if (rank == 0) {
        scanf("%d %d %d", &N, &M, &K);
    }
    
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&K, 1, MPI_INT, 0, MPI_COMM_WORLD);

    float* data = (float*)malloc(N * M * sizeof(float));
    if (rank == 0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                scanf("%f", data + i * M + j);
            }
        }
    }
    //--------------RAND CENTER----------------
    float* centers = (float*)malloc(K * M * sizeof(float));
    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < K; i++) {
            for (int j = 0; j < M; j++) {
                centers[i * M + j] = data[(rand()%N) * M + j];
            }
        }
    }
    MPI_Bcast(centers, K * M, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(data, N * M, MPI_FLOAT, 0, MPI_COMM_WORLD);

    //--------------KMEANS----------------
    int* cBelong = (int*)calloc(K, sizeof(int)); // number of points in each cluster
    float* vecSum = (float*)malloc(K * M * sizeof(float)); // sum of points in each cluster

    int* allCBelong = (int*)malloc(K * sizeof(int));
    float* allVecSum = (float*)malloc(K * M * sizeof(float));
    
    //------------iteration----------------
    for (int i = 0; i < 100; i++) {
        iter(rank, size, N, M, K, data, centers, cBelong, vecSum);
        MPI_Allreduce(cBelong, allCBelong, K, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(vecSum, allVecSum, K * M, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

        if (!rank) {
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < M; j++) {
                    centers[i * M + j] = allVecSum[i * M + j] / allCBelong[i];
                }
            }
        }
        
        MPI_Bcast(centers, K * M, MPI_FLOAT, 0, MPI_COMM_WORLD);
    }

    float localDist = 0.0;
    float minDist, dist;
    for (int i = rank; i < N; i += size) {
        minDist = INF;
        for (int j = 0; j < K; j++) {
            dist = distance(data + i * M, centers + j * M, M);
            if (dist < minDist) {
                minDist = dist;
            }
        }
        localDist += minDist;
    }

    float TotalDist;
    MPI_Reduce(&localDist, &TotalDist, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("%.2f\n", TotalDist);
    }

    MPI_Finalize();
    // free(cBelong);
    // free(vecSum);
    // free(allCBelong);
    // free(allVecSum);
    // free(data);
    // free(centers);
    return 0;
}
