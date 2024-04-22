#include <iostream>
#include <random>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

using namespace std;

__global__ void smm(float* denseMatrix, int* sparseMatrixIdx, float* sparseMatrixVal, float* result, int M, int N, int P, int K) {
    int idx = blockIdx.x;
    if (idx > K) return;
    else {
        int row = sparseMatrixIdx[idx * 2];
        int col = sparseMatrixIdx[idx * 2 + 1];
        float value = sparseMatrixVal[idx];
        int threadId = threadIdx.x;
        if (threadId < N) {
            atomicAdd(&result[threadId * P + col], denseMatrix[threadId * N + row] * value);
        }
    }
    __syncthreads();
    //---------------低配版加速----------------------------------
    // int idx = blockIdx.x * blockDim.x + threadIdx.x;
    // if (idx < K) {
    //     printf("K: %d\n", K);
    //     for (int i = 0; i < K; i++) {
    //         printf("sparseMatrixIdx[%d][0]: %d\n", i, sparseMatrixIdx[i * 2]);
    //         printf("sparseMatrixIdx[%d][1]: %d\n", i, sparseMatrixIdx[i * 2 + 1]);
    //         printf("sparseMatrixVal[%d]: %f\n", i, sparseMatrixVal[i]);
    //     }
    //     printf("idx: %d\n", idx);
    //     int row = sparseMatrixIdx[idx * 2];
    //     int col = sparseMatrixIdx[idx * 2 + 1];
    //     float value = sparseMatrixVal[idx];
    //     printf("row: %d, col: %d, value: %f\n", row, col, value);
    //     for (int i = 0; i < N; i++) {
    //         atomicAdd(&result[i * P + col], denseMatrix[i * N + row] * value);
    //     }
    // }
}

int main() {
    srand(time(NULL));
    // 这里由于不知道最后OJ会给多大的测试，因此这里暂且分配256x256的矩阵，到时候详细OJ描述下来之后还要修改
    //------------------DEFINE-----------------------------------
    const int TPB = 1024;
    const int BPG = 64;
    const int DATA_SIZE = TPB * BPG;

    int M, N, P, K;
    cin >> M >> N >> P >> K;
    float* denseMatrix, *result, *sparseMatrixVal;
    int* sparseMatrixIdx;

    //------------------INPUT-----------------------------------
    denseMatrix = new float[M * N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> denseMatrix[i * N + j];
        }
    }

    sparseMatrixIdx = new int[K * 2];
    sparseMatrixVal = new float[K];
    for (int i = 0; i < K; i++) {
        int row, col;
        float value;
        cin >> row >> col >> value;
        sparseMatrixIdx[i * 2] = row;
        sparseMatrixIdx[i * 2 + 1] = col;
        sparseMatrixVal[i] = value;
    }

    
    result = new float[M * P];
    //------------------CUDA-----------------------------------
    float* d_denseMatrix, * d_sparseMatrixVal, * d_result;
    int* d_sparseMatrixIdx;
    cudaMalloc((void**)&d_denseMatrix, sizeof(float) * M * N);
    cudaMalloc((void**)&d_sparseMatrixIdx, sizeof(int) * K * 2);
    cudaMalloc((void**)&d_sparseMatrixVal, sizeof(float) * K);
    cudaMalloc((void**)&d_result, sizeof(float) * M * P);
    cudaMemcpy(d_denseMatrix, denseMatrix, sizeof(float) * M * N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_sparseMatrixIdx, sparseMatrixIdx, sizeof(int) * K * 2, cudaMemcpyHostToDevice);
    cudaMemcpy(d_sparseMatrixVal, sparseMatrixVal, sizeof(float) * K, cudaMemcpyHostToDevice);

    dim3 threadPerBlock = dim3(TPB);
    dim3 blockPerGrid = dim3((DATA_SIZE + TPB - 1) / TPB);
    smm << <BPG, TPB >> > (d_denseMatrix, d_sparseMatrixIdx, d_sparseMatrixVal, d_result, M, N, P, K);
    cudaMemcpy(result, d_result, sizeof(float) * M * P, cudaMemcpyDeviceToHost);
    //------------------OUTPUT-----------------------------------
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < P; j++) {
            cout << result[i * P + j] << " ";
        }
        cout << endl;
    }
    //------------------FREE-----------------------------------
    cudaFree(d_denseMatrix);
    cudaFree(d_sparseMatrixIdx);
    cudaFree(d_sparseMatrixVal);
    cudaFree(d_result);
    return 0;
}
/*
4 4 4 5
2.32 3.2 6.0 7.2
0.9 3.5 2.0 3.1
2.1 3.2 0.6 9.2
3.2 0.9 3.2 5.3
0 2 0.3
1 0 6.3
2 3 0.6
3 1 0.6
3 2 2.3
*/