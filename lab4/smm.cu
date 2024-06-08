#include <iostream>
#include <random>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>
#include "device_launch_parameters.h"

using namespace std;

__global__ void smm(int* denseMatrix, int* sparseMatrixIdx, int* sparseMatrixVal, int* result, int M, int N, int P, int K) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int sum;
    if (idx > K) return;
    else {
        int row = sparseMatrixIdx[idx * 2];
        int col = sparseMatrixIdx[idx * 2 + 1];
        int value = sparseMatrixVal[idx];
        for(int i = 0; i < M; i++) {
            // result[i * P + col] += denseMatrix[i * N + row] * value;
            sum += denseMatrix[i * N + row] * value;
            // atomicAdd(&result[i * P + col], denseMatrix[i * N + row] * value);
        }
        result[row * P + col] = sum;
    }
    // __syncthreads();
}

int main() {
    srand(time(NULL));
    // 这里由于不知道最后OJ会给多大的测试，因此这里暂且分配256x256的矩阵，到时候详细OJ描述下来之后还要修改
    //------------------DEFINE-----------------------------------
    int M, N, P, K;
    cin >> M >> N >> P >> K;
    int* denseMatrix, *result, *sparseMatrixVal;
    int* sparseMatrixIdx;

    const int TPB = 256;
    const int BPG = (K + TPB - 1) / TPB;

    //------------------INPUT-----------------------------------
    denseMatrix = new int[M * N];
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            cin >> denseMatrix[i * N + j];
        }
    }

    sparseMatrixIdx = new int[K * 2];
    sparseMatrixVal = new int[K];
    for (int i = 0; i < K; i++) {
        int row, col;
        int value;
        cin >> row >> col >> value;
        sparseMatrixIdx[i * 2] = row;
        sparseMatrixIdx[i * 2 + 1] = col;
        sparseMatrixVal[i] = value;
    }

    
    result = new int[M * P];
    //------------------CUDA-----------------------------------
    int* d_denseMatrix, * d_sparseMatrixVal, * d_result;
    int* d_sparseMatrixIdx;
    cudaMalloc((void**)&d_denseMatrix, sizeof(int) * M * N);
    cudaMalloc((void**)&d_sparseMatrixIdx, sizeof(int) * K * 2);
    cudaMalloc((void**)&d_sparseMatrixVal, sizeof(int) * K);
    cudaMalloc((void**)&d_result, sizeof(int) * M * P);
    cudaMemcpy(d_denseMatrix, denseMatrix, sizeof(int) * M * N, cudaMemcpyHostToDevice);
    cudaMemcpy(d_sparseMatrixIdx, sparseMatrixIdx, sizeof(int) * K * 2, cudaMemcpyHostToDevice);
    cudaMemcpy(d_sparseMatrixVal, sparseMatrixVal, sizeof(int) * K, cudaMemcpyHostToDevice);

    dim3 threadPerBlock = dim3(TPB);
    dim3 blockPerGrid = dim3(BPG);
    smm <<<BPG, TPB >>> (d_denseMatrix, d_sparseMatrixIdx, d_sparseMatrixVal, d_result, M, N, P, K);
    cudaMemcpy(result, d_result, sizeof(int) * M * P, cudaMemcpyDeviceToHost);
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